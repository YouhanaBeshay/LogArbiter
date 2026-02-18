#pragma once

#include <QFile>
#include <QJsonObject>
#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QVariantMap>

class AppController : public QObject {
  Q_OBJECT

  // Properties exposed to QML
  Q_PROPERTY(bool appRunning READ appRunning NOTIFY appRunningChanged)
  Q_PROPERTY(
      bool fileGenRunning READ fileGenRunning NOTIFY fileGenRunningChanged)
  Q_PROPERTY(bool socketGenRunning READ socketGenRunning NOTIFY
                 socketGenRunningChanged)
  Q_PROPERTY(bool someIPRunning READ someIPRunning NOTIFY someIPRunningChanged)
  Q_PROPERTY(
      bool darkTheme READ darkTheme WRITE setDarkTheme NOTIFY darkThemeChanged)
  Q_PROPERTY(QVariantMap config READ config NOTIFY configChanged)
  Q_PROPERTY(int infoCount READ infoCount NOTIFY statsChanged)
  Q_PROPERTY(int warningCount READ warningCount NOTIFY statsChanged)
  Q_PROPERTY(int criticalCount READ criticalCount NOTIFY statsChanged)

public:
  explicit AppController(QObject *parent = nullptr);
  ~AppController();

  // Property getters
  bool appRunning() const;
  bool fileGenRunning() const;
  bool socketGenRunning() const;
  bool someIPRunning() const;
  bool darkTheme() const;
  QVariantMap config() const;
  int infoCount() const;
  int warningCount() const;
  int criticalCount() const;

  // Property setters
  void setDarkTheme(bool dark);

public slots:
  // App control
  void startApp();
  void stopApp();

  // Service control
  void toggleFileGen();
  void toggleSocketGen();
  void toggleSomeIP();

  // Configuration
  void loadConfig(const QString &path);
  void saveConfig(const QString &path);
  void updateConfig(const QVariantMap &newConfig);
  void applyDefaultConfig();

  // Log management
  void clearStats();
  void emptyLogFiles(); // Empty the actual log files

signals:
  void appRunningChanged();
  void fileGenRunningChanged();
  void socketGenRunningChanged();
  void someIPRunningChanged();
  void darkThemeChanged();
  void configChanged();
  void statsChanged();
  void newLogEntry(const QString &timestamp, const QString &level,
                   const QString &source, const QString &message);
  void error(const QString &message);


private slots:
  void watchLogFiles();
  void onProcessOutput();
  void onProcessError();

private:
  void parseLogLine(const QString &line);
  void setupPaths();
  void updateLogFilePathsFromConfig(); // ADD THIS

  // Processes
  QProcess *appProcess_;
  QProcess *fileGenProcess_;
  QProcess *socketGenProcess_;
  QProcess *someIPProcess_;

  // Paths
  QString logArbiterPath_;
  QString fileGenScript_;
  QString socketGenScript_;
  QString someIPServerPath_;
  QString configPath_;

  // Log file watching
  QTimer *logWatchTimer_;
  QStringList logFilePaths_;
  QMap<QString, qint64> filePositions_;

  // Config
  QJsonObject configJson_;

  // State
  bool darkTheme_;
  int infoCount_;
  int warningCount_;
  int criticalCount_;
     bool isWatchingLogs_;
    void startLogWatching();
    void stopLogWatching();
};