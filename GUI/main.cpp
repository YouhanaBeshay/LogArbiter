#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QTimer> 
#include <QDebug>
#include <QQuickWindow>
#include "AppController.hpp"
#include "LogModel.hpp"
#include "LogFilterProxyModel.hpp" 

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName("LogArbiter Control Panel");
    QGuiApplication::setOrganizationName("LogArbiter");
    QQuickStyle::setStyle("Material");
    
    // Don't quit when last window closes (important!)
    app.setQuitOnLastWindowClosed(false);
    
    // Initialize app components FIRST
    AppController controller;
    LogModel logModel;
    LogFilterProxyModel proxyModel;
    
    proxyModel.setSourceModel(&logModel);
    
    qDebug() << "=== Setting up signal-slot connections ===";
    QObject::connect(
        &controller, &AppController::newLogEntry,
        &logModel, &LogModel::addEntry,
        Qt::QueuedConnection
    );
    qDebug() << "Connections established";
    
    // Load main application (but keep it hidden)
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("appController", &controller);
    engine.rootContext()->setContextProperty("logModel", &proxyModel);
    
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { 
            qCritical() << "QML object creation failed!";
            QCoreApplication::exit(-1); 
        }, Qt::QueuedConnection);
    
    engine.load(url);
    
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "No root objects created!";
        return -1;
    }
    
    qDebug() << "Main window loaded";
    
    // Get main window and keep it hidden initially
    QObject *rootObject = engine.rootObjects().first();
    QQuickWindow *mainWindow = qobject_cast<QQuickWindow*>(rootObject);
    
    if (!mainWindow) {
        qCritical() << "Could not get main window!";
        return -1;
    }
    
    qDebug() << "Main window reference obtained, keeping hidden";
    mainWindow->setVisible(false);
    
    // Load splash screen AFTER main window
    QQmlApplicationEngine splashEngine;
    splashEngine.load(QUrl(QStringLiteral("qrc:/qml/SplashScreen.qml")));
    
    if (splashEngine.rootObjects().isEmpty()) {
        qWarning() << "Failed to load splash screen, showing main window immediately";
        mainWindow->show();
    } else {
        qDebug() << "Splash screen loaded";
        
        // Get splash window reference
        QObject *splashObject = splashEngine.rootObjects().first();
        QQuickWindow *splashWindow = qobject_cast<QQuickWindow*>(splashObject);
        
        if (splashWindow) {
            splashWindow->setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            splashWindow->show();

            // Center splash on the primary screen
            QRect screenGeom = splashWindow->screen()->availableGeometry();
            splashWindow->setX(screenGeom.x() + (screenGeom.width() - splashWindow->width()) / 2);
            splashWindow->setY(screenGeom.y() + (screenGeom.height() - splashWindow->height()) / 2);
        }
        
        // Show main window and close splash after delay
        QTimer *showTimer = new QTimer(&app);
        showTimer->setSingleShot(true);
        QObject::connect(showTimer, &QTimer::timeout, [mainWindow, splashWindow, &app]() {
            qDebug() << "Timer triggered - closing splash and showing main window";
            
            if (splashWindow) {
                splashWindow->close();
            }
            
            mainWindow->setVisible(true);
            mainWindow->raise();
            mainWindow->requestActivate();
            
            // Re-enable quit on last window close
            app.setQuitOnLastWindowClosed(true);
        });
        showTimer->start(2450);
    }
    
    qDebug() << "=== Application started ===";
    return app.exec();
}
