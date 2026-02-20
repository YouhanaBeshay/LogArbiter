#include "AppController.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include <QStandardPaths>
#include <QDebug>
#include <QFileInfo>

AppController::AppController(QObject *parent)
    : QObject(parent)
    , appProcess_(nullptr)
    , fileGenProcess_(nullptr)
    , socketGenProcess_(nullptr)
    , someIPProcess_(nullptr)
    , darkTheme_(true)
    , infoCount_(0)
    , warningCount_(0)
    , criticalCount_(0)
    , isWatchingLogs_(false)
{
    qDebug() << "=== AppController Initializing ===";
    setupPaths();
    
    logWatchTimer_ = new QTimer(this);
    connect(logWatchTimer_, &QTimer::timeout, this, &AppController::watchLogFiles);
    // Don't start timer yet - wait for app to start
    
    loadConfig(configPath_);
    qDebug() << "=== AppController Ready ===";
}

AppController::~AppController() {
    qDebug() << "AppController shutting down...";
    stopApp();
    
    // Clean up file generator
    if (fileGenProcess_) {
        fileGenProcess_->disconnect();
        fileGenProcess_->terminate();
        if (!fileGenProcess_->waitForFinished(1000)) {
            fileGenProcess_->kill();
            fileGenProcess_->waitForFinished(1000);
        }
        delete fileGenProcess_;
        fileGenProcess_ = nullptr;
    }
    
    // Clean up socket generator
    if (socketGenProcess_) {
        socketGenProcess_->disconnect();
        socketGenProcess_->terminate();
        if (!socketGenProcess_->waitForFinished(1000)) {
            socketGenProcess_->kill();
            socketGenProcess_->waitForFinished(1000);
        }
        delete socketGenProcess_;
        socketGenProcess_ = nullptr;
    }
    
    // Clean up SomeIP
    if (someIPProcess_) {
        someIPProcess_->disconnect();
        someIPProcess_->terminate();
        if (!someIPProcess_->waitForFinished(1000)) {
            someIPProcess_->kill();
            someIPProcess_->waitForFinished(1000);
        }
        delete someIPProcess_;
        someIPProcess_ = nullptr;
    }
    
    qDebug() << "AppController shutdown complete";
}

void AppController::setupPaths() {
    QString basePath = QCoreApplication::applicationDirPath();
    qDebug() << "Application directory:" << basePath;
    
    QStringList possibleRoots = {
        basePath + "/../..",
        basePath + "/../../..",
        basePath + "/../../../..",
    };
    
    QString projectRoot;
    for (const QString &root : possibleRoots) {
        QDir testDir(root);
        QString normalized = testDir.absolutePath();
        
        if (QFileInfo::exists(normalized + "/App/config.json")) {
            projectRoot = normalized;
            qDebug() << "Found project root:" << projectRoot;
            break;
        }
    }
    
    if (projectRoot.isEmpty()) {
        qWarning() << "Could not find project root!";
        projectRoot = basePath + "/../..";
    }
    
    logArbiterPath_ = projectRoot + "/build/LogArbiter";
    fileGenScript_ = projectRoot + "/provided/shell_log_src.sh";
    socketGenScript_ = projectRoot + "/provided/shell_log_src_socket.sh";
    someIPServerPath_ = projectRoot + "/CommonApi/vSomeipServer/build/vSomeipServer_GpuUsage";
    configPath_ = projectRoot + "/App/config.json";
    
    qDebug() << "LogArbiter:" << logArbiterPath_ << "Exists:" << QFileInfo::exists(logArbiterPath_);
    qDebug() << "Config:" << configPath_ << "Exists:" << QFileInfo::exists(configPath_);
    
    QString buildDir = projectRoot + "/build/";
    logFilePaths_ << buildDir + "output.log" << buildDir + "backup.log";
}

// Property getters
bool AppController::appRunning() const {
    return appProcess_ && appProcess_->state() == QProcess::Running;
}
bool AppController::fileGenRunning() const {
    return fileGenProcess_ && fileGenProcess_->state() == QProcess::Running;
}
bool AppController::socketGenRunning() const {
    return socketGenProcess_ && socketGenProcess_->state() == QProcess::Running;
}
bool AppController::someIPRunning() const {
    return someIPProcess_ && someIPProcess_->state() == QProcess::Running;
}
bool AppController::darkTheme() const { return darkTheme_; }
QVariantMap AppController::config() const { return configJson_.toVariantMap(); }
int AppController::infoCount() const { return infoCount_; }
int AppController::warningCount() const { return warningCount_; }
int AppController::criticalCount() const { return criticalCount_; }

void AppController::setDarkTheme(bool dark) {
    if (darkTheme_ != dark) {
        darkTheme_ = dark;
        emit darkThemeChanged();
    }
}

// ==================== App Control ====================

void AppController::startApp() {
    qDebug() << "=== Starting LogArbiter ===";
    
    if (appRunning()) return;
    
    QFileInfo configInfo(configPath_);
    QString scriptPath = configInfo.absolutePath() + "/../runLogArbiter.sh";
    
    if (!QFileInfo::exists(scriptPath)) {
        emit error("Launch script not found: " + scriptPath);
        return;
    }
    

    appProcess_ = new QProcess(this);
    
    QString scriptDir = QFileInfo(scriptPath).absolutePath();
    appProcess_->setWorkingDirectory(scriptDir);
    
    connect(appProcess_, &QProcess::readyReadStandardOutput, this, &AppController::onProcessOutput);
    connect(appProcess_, &QProcess::readyReadStandardError, this, &AppController::onProcessError);
    connect(appProcess_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this](int exitCode, QProcess::ExitStatus) {
                qDebug() << "LogArbiter exited with code:" << exitCode;
                stopLogWatching();
                emit appRunningChanged();
            });
    
    // Run the script directly 
    appProcess_->start("sh", QStringList() << scriptPath);
    
    if (appProcess_->waitForStarted(5000)) {
        qDebug() << "LogArbiter started, PID:" << appProcess_->processId();
        startLogWatching();
        emit appRunningChanged();
    } else {
        emit error("Failed to start: " + appProcess_->errorString());
        appProcess_->deleteLater();
        appProcess_ = nullptr;
    }
}

void AppController::stopApp() {
    if (!appProcess_) return;
    
    qDebug() << "=== Stopping LogArbiter ===";
    stopLogWatching();
    
    appProcess_->terminate();
    if (!appProcess_->waitForFinished(3000)) {
        appProcess_->kill();
    }
    appProcess_->deleteLater();
    appProcess_ = nullptr;
    emit appRunningChanged();
}

void AppController::startLogWatching() {
    if (isWatchingLogs_) return;
    
    qDebug() << "[AppController] Starting log watchers";
    isWatchingLogs_ = true;
    logWatchTimer_->start(500);
}

void AppController::stopLogWatching() {
    if (!isWatchingLogs_) return;
    
    qDebug() << "[AppController] Stopping log watchers";
    isWatchingLogs_ = false;
    logWatchTimer_->stop();
}

// ==================== Service Control ====================

void AppController::toggleFileGen() {
    qDebug() << "=== Toggle File Generator ===" << "Currently running:" << fileGenRunning();
    
    if (fileGenRunning()) {
        qDebug() << "Stopping file generator...";
        fileGenProcess_->disconnect();
        fileGenProcess_->terminate();
        if (!fileGenProcess_->waitForFinished(2000)) {
            fileGenProcess_->kill();
            fileGenProcess_->waitForFinished(1000);
        }
        fileGenProcess_->deleteLater();
        fileGenProcess_ = nullptr;
        qDebug() << "File generator stopped";
    } else {
        if (!QFileInfo::exists(fileGenScript_)) {
            QString msg = "Script not found: " + fileGenScript_;
            qWarning() << msg;
            emit error(msg);
            emit fileGenRunningChanged();
            return;
        }
        
        qDebug() << "Starting file generator:" << fileGenScript_;
        fileGenProcess_ = new QProcess(this);
        
        QFileInfo scriptInfo(fileGenScript_);
        QString workingDir = scriptInfo.absolutePath();
        fileGenProcess_->setWorkingDirectory(workingDir);
        qDebug() << "Setting working directory to:" << workingDir;
        
        connect(fileGenProcess_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int exitCode, QProcess::ExitStatus status) {
                    qDebug() << "File generator finished, exit code:" << exitCode;
                    if (fileGenProcess_) {
                        fileGenProcess_->deleteLater();
                        fileGenProcess_ = nullptr;
                        emit fileGenRunningChanged();
                    }
                });
        
        connect(fileGenProcess_, &QProcess::errorOccurred, this, [this](QProcess::ProcessError err) {
            if (!fileGenProcess_) return;
            qDebug() << "File generator error:" << err;
            emit error("File generator error: " + fileGenProcess_->errorString());
            fileGenProcess_->deleteLater();
            fileGenProcess_ = nullptr;
            emit fileGenRunningChanged();
        });
        
        fileGenProcess_->start("/bin/bash", QStringList() << fileGenScript_);
        
        if (fileGenProcess_->waitForStarted(3000)) {
            qDebug() << "File generator started! PID:" << fileGenProcess_->processId();
        } else {
            qWarning() << "Failed to start file generator:" << fileGenProcess_->errorString();
            emit error("Failed to start file generator: " + fileGenProcess_->errorString());
            fileGenProcess_->deleteLater();
            fileGenProcess_ = nullptr;
            emit fileGenRunningChanged();
        }
    }
    
    emit fileGenRunningChanged();
}

void AppController::toggleSocketGen() {
    qDebug() << "=== Toggle Socket Generator ===" << "Currently running:" << socketGenRunning();
    
    if (socketGenRunning()) {
        qDebug() << "Stopping socket generator...";
        socketGenProcess_->disconnect();
        socketGenProcess_->terminate();
        if (!socketGenProcess_->waitForFinished(2000)) {
            socketGenProcess_->kill();
            socketGenProcess_->waitForFinished(1000);
        }
        socketGenProcess_->deleteLater();
        socketGenProcess_ = nullptr;
        qDebug() << "Socket generator stopped";
    } else {
        if (!QFileInfo::exists(socketGenScript_)) {
            QString msg = "Script not found: " + socketGenScript_;
            qWarning() << msg;
            emit error(msg);
            emit socketGenRunningChanged();
            return;
        }
        
        qDebug() << "Starting socket generator:" << socketGenScript_;
        socketGenProcess_ = new QProcess(this);
        
        QFileInfo scriptInfo(socketGenScript_);
        QString workingDir = scriptInfo.absolutePath();
        socketGenProcess_->setWorkingDirectory(workingDir);
        qDebug() << "Setting working directory to:" << workingDir;
        
        connect(socketGenProcess_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int exitCode, QProcess::ExitStatus status) {
                    qDebug() << "Socket generator finished, exit code:" << exitCode;
                    if (socketGenProcess_) {
                        socketGenProcess_->deleteLater();
                        socketGenProcess_ = nullptr;
                        emit socketGenRunningChanged();
                    }
                });
        
        connect(socketGenProcess_, &QProcess::errorOccurred, this, [this](QProcess::ProcessError err) {
            if (!socketGenProcess_) return;
            qDebug() << "Socket generator error:" << err;
            emit error("Socket generator error: " + socketGenProcess_->errorString());
            socketGenProcess_->deleteLater();
            socketGenProcess_ = nullptr;
            emit socketGenRunningChanged();
        });
        
        socketGenProcess_->start("/bin/bash", QStringList() << socketGenScript_);
        
        if (socketGenProcess_->waitForStarted(3000)) {
            qDebug() << "Socket generator started! PID:" << socketGenProcess_->processId();
        } else {
            qWarning() << "Failed to start socket generator:" << socketGenProcess_->errorString();
            emit error("Failed to start socket generator: " + socketGenProcess_->errorString());
            socketGenProcess_->deleteLater();
            socketGenProcess_ = nullptr;
            emit socketGenRunningChanged();
        }
    }
    
    emit socketGenRunningChanged();
}

void AppController::toggleSomeIP() {
    qDebug() << "=== Toggle SomeIP ===" << "Currently running:" << someIPRunning();
    
    if (someIPRunning()) {
        qDebug() << "Stopping SomeIP server...";
        someIPProcess_->disconnect();
        someIPProcess_->terminate();
        if (!someIPProcess_->waitForFinished(2000)) {
            someIPProcess_->kill();
            someIPProcess_->waitForFinished(1000);
        }
        someIPProcess_->deleteLater();
        someIPProcess_ = nullptr;
        qDebug() << "SomeIP server stopped";
    } else {
        if (!QFileInfo::exists(someIPServerPath_)) {
            QString msg = "Server not found: " + someIPServerPath_;
            qWarning() << msg;
            emit error(msg);
            emit someIPRunningChanged();
            return;
        }
        
        qDebug() << "Starting SomeIP server:" << someIPServerPath_;
        someIPProcess_ = new QProcess(this);
        
        QFileInfo execInfo(someIPServerPath_);
        QString workingDir = execInfo.absolutePath();
        someIPProcess_->setWorkingDirectory(workingDir);
        qDebug() << "Setting working directory to:" << workingDir;
        
        connect(someIPProcess_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this, [this](int exitCode, QProcess::ExitStatus status) {
                    qDebug() << "SomeIP server finished, exit code:" << exitCode;
                    if (someIPProcess_) {
                        someIPProcess_->deleteLater();
                        someIPProcess_ = nullptr;
                        emit someIPRunningChanged();
                    }
                });
        
        connect(someIPProcess_, &QProcess::errorOccurred, this, [this](QProcess::ProcessError err) {
            if (!someIPProcess_) return;
            qDebug() << "SomeIP server error:" << err;
            emit error("SomeIP server error: " + someIPProcess_->errorString());
            someIPProcess_->deleteLater();
            someIPProcess_ = nullptr;
            emit someIPRunningChanged();
        });
        
        someIPProcess_->start(someIPServerPath_);
        
        if (someIPProcess_->waitForStarted(3000)) {
            qDebug() << "SomeIP server started! PID:" << someIPProcess_->processId();
        } else {
            qWarning() << "Failed to start SomeIP server:" << someIPProcess_->errorString();
            emit error("Failed to start SomeIP server: " + someIPProcess_->errorString());
            someIPProcess_->deleteLater();
            someIPProcess_ = nullptr;
            emit someIPRunningChanged();
        }
    }
    
    emit someIPRunningChanged();
}

// ==================== Configuration ====================

void AppController::loadConfig(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        applyDefaultConfig();
        return;
    }
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    file.close();
    
    if (parseError.error != QJsonParseError::NoError) {
        applyDefaultConfig();
        return;
    }
    
    configJson_ = doc.object();
    updateLogFilePathsFromConfig();
    emit configChanged();
}

void AppController::saveConfig(const QString &path) {
    QString savePath = path.isEmpty() ? configPath_ : path;
    
    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly)) {
        emit error("Failed to save: " + file.errorString());
        return;
    }
    
    file.write(QJsonDocument(configJson_).toJson(QJsonDocument::Indented));
    file.close();
    qDebug() << "Config saved to:" << savePath;
}

void AppController::updateConfig(const QVariantMap &newConfig) {
    configJson_ = QJsonObject::fromVariantMap(newConfig);
    updateLogFilePathsFromConfig();
    emit configChanged();
    saveConfig(configPath_);
}

void AppController::applyDefaultConfig() {
    QString defaultJson = R"({
        "log_manager": {"buffer_capacity": 100, "thread_pool_size": 2, "sink_flush_rate_ms": 1000},
        "sinks": {"console": {"enabled": true}, "files": [{"enabled": true, "path": "output.log"}]},
        "sources": {
            "file": {"enabled": false, "path": "", "parse_rate_ms": 1000, "policy": "cpu"},
            "socket": {"enabled": false, "path": "", "parse_rate_ms": 1000, "policy": "ram"},
            "someip": {"enabled": false, "parse_rate_ms": 1000, "policy": "gpu"}
        }
    })";
    configJson_ = QJsonDocument::fromJson(defaultJson.toUtf8()).object();
    emit configChanged();
}

void AppController::updateLogFilePathsFromConfig() {
    logFilePaths_.clear();
    
    QFileInfo configInfo(configPath_);
    QString buildDir = configInfo.absolutePath() + "/../build/";
    
    QJsonArray files = configJson_["sinks"].toObject()["files"].toArray();
    for (const auto &f : files) {
        QString path = f.toObject()["path"].toString();
        if (!path.isEmpty()) {
            logFilePaths_ << buildDir + path;
        }
    }
    qDebug() << "Log files to watch:" << logFilePaths_;
}

// ==================== Log Processing ====================

void AppController::watchLogFiles() {
    if (!isWatchingLogs_) return;
    
    for (const QString &filePath : logFilePaths_) {
        QFile file(filePath);
        if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            continue;
        }
        
        qint64 lastPos = filePositions_.value(filePath, 0);
        qint64 fileSize = file.size();
        
        if (fileSize < lastPos) lastPos = 0;
        if (fileSize == lastPos) { file.close(); continue; }
        
        file.seek(lastPos);
        QTextStream in(&file);
        
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.trimmed().isEmpty()) {
                parseLogLine(line);
            }
        }
        
        filePositions_[filePath] = file.pos();
        file.close();
    }
}

void AppController::parseLogLine(const QString &line) {
    QString trimmed = line.trimmed();
    if (trimmed.isEmpty()) return;
    
    if (trimmed.contains("[info]") || trimmed.contains("vsomeip") || 
        trimmed.contains("[CAPI]") || trimmed.contains("endpoint") ||
        trimmed.contains("cei::") || trimmed.contains("local_uds")) {
        return;
    }
    
    if (!trimmed.startsWith('[')) return;
    
    int pos = 0;
    QString parts[4];
    
    for (int i = 0; i < 3; i++) {
        int start = trimmed.indexOf('[', pos);
        int end = trimmed.indexOf(']', start);
        if (start == -1 || end == -1) return;
        parts[i] = trimmed.mid(start + 1, end - start - 1);
        pos = end + 1;
    }
    parts[3] = trimmed.mid(pos).trimmed();
    
    QString timestamp = parts[0];
    QString level = parts[1];
    QString source = parts[2];
    QString message = parts[3];
    
    if (level != "INFO" && level != "WARNING" && level != "CRITICAL" && 
        level != "DEBUG" && level != "ERROR") {
        return;
    }
    
    qDebug() << "[LOG]" << level << "|" << source << "|" << message.left(40);
    
    if (level == "INFO") infoCount_++;
    else if (level == "WARNING") warningCount_++;
    else if (level == "CRITICAL") criticalCount_++;
    
    emit statsChanged();
    emit newLogEntry(timestamp, level, source, message);
}

void AppController::clearStats() {
    infoCount_ = 0;
    warningCount_ = 0;
    criticalCount_ = 0;
    filePositions_.clear();
    emit statsChanged();
}


void AppController::emptyLogFiles() {
    qDebug() << "[AppController] Emptying log files";
    
    if (appRunning()) {
        emit error("Cannot empty log files while app is running. Stop the app first.");
        return;
    }
    
    bool success = true;
    QStringList failedFiles;
    
    for (const QString &filePath : logFilePaths_) {
        QFile file(filePath);
        
        if (!file.exists()) {
            qDebug() << "File doesn't exist (skipping):" << filePath;
            continue;
        }
        
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            qWarning() << "Failed to empty file:" << filePath;
            failedFiles << filePath;
            success = false;
        } else {
            file.close();
            qDebug() << "Emptied log file:" << filePath;
        }
    }
    
    filePositions_.clear();
    
    if (!success) {
        emit error("Failed to empty some files: " + failedFiles.join(", "));
    } else {
        qDebug() << "All log files emptied successfully";
    }
}

void AppController::onProcessOutput() {
    auto *process = qobject_cast<QProcess*>(sender());
    if (!process) return;
    
    QString output = QString::fromUtf8(process->readAllStandardOutput());
    for (const QString &line : output.split('\n', Qt::SkipEmptyParts)) {
        parseLogLine(line);
    }
}

void AppController::onProcessError() {
    auto *process = qobject_cast<QProcess*>(sender());
    if (!process) return;
    
    QString errorOutput = QString::fromUtf8(process->readAllStandardError());
    
    if (errorOutput.contains("[CAPI][INFO]") || errorOutput.contains("[info]")) {
        return;
    }
    
    if (!errorOutput.trimmed().isEmpty()) {
        qWarning() << "Process stderr:" << errorOutput.left(200);
    }
}