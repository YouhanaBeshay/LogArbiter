import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Controls.Material 2.15

Window {
    id: splashWindow
    width: 1920  
    height: 1080  
    visible: true
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    color: "transparent"

    Rectangle {
        width: 450
        height: 450
        radius: 225
        anchors.centerIn: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1a1a2e" }
            GradientStop { position: 1.0; color: "#16213e" }
        }
        
        border.color: "#aa272d"
        border.width: 3
        
        // Add anti-aliasing for smooth circular edges
        antialiasing: true
        layer.enabled: true
        layer.samples: 8
        
        Column {
            anchors.centerIn: parent
            spacing: 25
            
            // Logo Image
            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/logo/logo.png"  // Your PNG logo path
                width: 150
                height: 150
                fillMode: Image.PreserveAspectFit
                smooth: true
                // Fallback if image not found - show emoji
                onStatusChanged: {
                    if (status === Image.Error) {
                        visible = false
                        fallbackIcon.visible = true
                    }
                }
            }
            
            // Fallback icon if image fails to load
            Text {
                id: fallbackIcon
                anchors.horizontalCenter: parent.horizontalCenter
                text: "🔧"
                font.pixelSize: 100
                visible: false
            }
            
            // App Name
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "LogArbiter"
                font.pixelSize: 38
                font.bold: true
                color: "#aa272d"
            }
            
            // Subtitle
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Control Panel"
                font.pixelSize: 18
                color: "#eaeaea"
                opacity: 0.8
            }
            
            // Loading animation
            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                running: true
                Material.accent: "#aa272d"
                width: 50
                height: 50
            }
            
            // Simple loading text
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Starting application..."
                font.pixelSize: 14
                color: "#eaeaea"
                opacity: 0.7
            }
        }
        
        // Copyright at bottom
        Text {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 20
            text: "© 2026 ITI ES Intake 46"
            font.pixelSize: 10
            color: "#eaeaea"
            opacity: 0.4
        }
    }
    
    // Auto-close timer
    Timer {
        id: closeTimer
        interval: 2500
        running: true
        onTriggered: {
            splashWindow.close()
        }
    }
}