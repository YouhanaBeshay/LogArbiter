import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

Rectangle {
    id: controlPanel
    color: root.cardColor
    radius: 8
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 16
        
        GroupBox {
            Layout.fillWidth: true
            title: "Application"
            
            background: Rectangle {
                y: parent.topPadding - parent.padding
                width: parent.width
                height: parent.height - parent.topPadding + parent.padding
                color: "transparent"
                border.color: root.accentColor
                radius: 4
            }
            
            label: Label {
                text: parent.title
                color: root.accentColor
                font.bold: true
            }
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 8
                
                RowLayout {
                    Layout.fillWidth: true
                    
                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        color: appController.appRunning ? "#4CAF50" : "#F44336"
                    }
                    
                    Label {
                        text: appController.appRunning ? "Running" : "Stopped"
                        color: root.textColor
                    }
                }
                
                Button {
                    Layout.fillWidth: true
                    text: appController.appRunning ? "■ Stop App" : "▶ Start App"
                    highlighted: !appController.appRunning
                    Material.background: appController.appRunning ? "#F44336" : "#4CAF50"
                    hoverEnabled: true   // IMPORTANT for desktop hover

                    ToolTip.visible: hovered && !appController.appRunning
                    ToolTip.delay: 500
                    ToolTip.text: "Start the application (requires password for vSomeIP multicast)"
                    onClicked: {
                        if (appController.appRunning) {
                            appController.stopApp()
                        } else {
                            appController.startApp()
                        }
                    }
                }
            }
        }
        
        GroupBox {
            Layout.fillWidth: true
            title: "Services"
            
            background: Rectangle {
                y: parent.topPadding - parent.padding
                width: parent.width
                height: parent.height - parent.topPadding + parent.padding
                color: "transparent"
                border.color: root.accentColor
                radius: 4
            }
            
            label: Label {
                text: parent.title
                color: root.accentColor
                font.bold: true
            }
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 8
                
                Button {
                    Layout.fillWidth: true
                    text: appController.fileGenRunning ? "■ File Generator" : "▶ File Generator"
                    Material.background: appController.fileGenRunning ? root.accentColor : undefined
                    onClicked: appController.toggleFileGen()
                }
                
                Button {
                    Layout.fillWidth: true
                    text: appController.socketGenRunning ? "■ Socket Generator" : "▶ Socket Generator"
                    Material.background: appController.socketGenRunning ? root.accentColor : undefined
                    onClicked: appController.toggleSocketGen()
                }
                
                Button {
                    Layout.fillWidth: true
                    text: appController.someIPRunning ? "■ SomeIP Server" : "▶ SomeIP Server"
                    Material.background: appController.someIPRunning ? root.accentColor : undefined
                    enabled: false         
                    hoverEnabled: true      

                    ToolTip.visible: hovered
                    ToolTip.delay: 400
                    ToolTip.text: "vSomeIP server is expected to be on the Raspberry Pi"

                    onClicked: appController.toggleSomeIP()
                }
            }
        }
        
        GroupBox {
            Layout.fillWidth: true
            title: "Configuration"
            
            background: Rectangle {
                y: parent.topPadding - parent.padding
                width: parent.width
                height: parent.height - parent.topPadding + parent.padding
                color: "transparent"
                border.color: root.accentColor
                radius: 4
            }
            
            label: Label {
                text: parent.title
                color: root.accentColor
                font.bold: true
            }
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 8
                
                Button {
                    Layout.fillWidth: true
                    text: "⚙ Configure"
                    onClicked: configDialog.open()
                }
            }
        }
        
        GroupBox {
            Layout.fillWidth: true
            title: "Log Management"
            
            background: Rectangle {
                y: parent.topPadding - parent.padding
                width: parent.width
                height: parent.height - parent.topPadding + parent.padding
                color: "transparent"
                border.color: root.accentColor
                radius: 4
            }
            
            label: Label {
                text: parent.title
                color: root.accentColor
                font.bold: true
            }
            
            ColumnLayout {
                anchors.fill: parent
                spacing: 8              
                
                Button {
                    Layout.fillWidth: true
                    text: "🗑️ Empty Log Files"
                    Material.background: "#F44336"
                    Material.foreground: "white"
                    enabled: !appController.appRunning
                    onClicked: emptyConfirmDialog.open()

                    
                    ToolTip.visible: hovered && appController.appRunning
                    ToolTip.text: "Stop the app first to empty log files"
                }
            }
        }
        
        Item { Layout.fillHeight: true }
    }
}