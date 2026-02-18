import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    id: root
    width: 1200
    height: 800
    visible: false
    title: "LogArbiter Control Panel"
    
    
    Material.theme: appController.darkTheme ? Material.Dark : Material.Light
    Material.accent: Material.Pink
    Material.primary: Material.Indigo
    
    readonly property color bgColor: appController.darkTheme ? "#1a1a2e" : "#f5f5f5"
    readonly property color cardColor: appController.darkTheme ? "#16213e" : "#ffffff"
    readonly property color accentColor: "#aa272d"
    readonly property color textColor: appController.darkTheme ? "#eaeaea" : "#1a1a2e"
    
    background: Rectangle { color: root.bgColor }
    
    header: ToolBar {
        Material.background: root.cardColor
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: 8
            
            Label {
                text: "🔧 LogArbiter Control Panel"
                font.pixelSize: 18
                font.bold: true
                color: root.accentColor
            }
            
            Item { Layout.fillWidth: true }
            
            Switch {
                text: appController.darkTheme ? "🌙" : "☀️"
                checked: appController.darkTheme
                onCheckedChanged: appController.darkTheme = checked
                // Thumb (circle)
                Material.accent: "#aa272d"

                // Track background
                Material.foreground: root.accentColor
            }
        }
    }
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8
        
        ControlPanel {
            Layout.preferredWidth: 220
            Layout.fillHeight: true
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8
            
            LogViewer {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                color: root.cardColor
                radius: 8
                
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8
                    
                    Label {
                        text: "📊 Total: " + logModel.count
                        color: root.textColor
                    }
                    
                    Rectangle { width: 1; height: 20; color: root.accentColor }
                    
                    Label {
                        text: "ℹ️ Info: " + logModel.infoCount
                        color: "#4CAF50"
                    }
                    
                    Rectangle { width: 1; height: 20; color: root.accentColor }
                    
                    Label {
                        text: "⚠️ Warning: " + logModel.warningCount
                        color: "#FF9800"
                    }
                    
                    Rectangle { width: 1; height: 20; color: root.accentColor }
                    
                    Label {
                        text: "🚨 Critical: " + logModel.criticalCount
                        color: "#F44336"
                    }
                    
                    Item { Layout.fillWidth: true }
                }
            }
        }
    }
    
    ConfigEditor {
        id: configDialog
    }
    
    Dialog {
        id: emptyConfirmDialog
        anchors.centerIn: parent
        width: 450
        height: 300
        title: "⚠️ Empty Log Files"
        modal: true
        
        Material.background: root.cardColor
        
        ColumnLayout {
            anchors.fill: parent
            spacing: 16
            
            Label {
                text: "Are you sure you want to permanently delete all log file contents?"
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                color: root.textColor
                font.pixelSize: 14
            }
            
            Label {
                text: "⚠️ This action cannot be undone!"
                font.bold: true
                font.pixelSize: 15
                color: "#F44336"
                Layout.fillWidth: true
            }
            
            Item { Layout.fillHeight: true }
            
            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                
                Item { Layout.fillWidth: true }
                
                Button {
                    text: "Cancel"
                    onClicked: emptyConfirmDialog.close()
                }
                
                Button {
                    text: "Yes, Empty Files"
                    Material.background: "#F44336"
                    Material.foreground: "white"
                    onClicked: {
                        appController.emptyLogFiles()
                        logModel.clear()
                        appController.clearStats()
                        emptyConfirmDialog.close()
                    }
                }
            }
        }
    }
    
    Popup {
        id: errorPopup
        anchors.centerIn: parent
        width: 400
        height: 150
        modal: true
        
        property string errorMessage: ""
        
        background: Rectangle {
            color: root.cardColor
            border.color: "#F44336"
            border.width: 2
            radius: 8
        }
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            
            Label {
                text: "❌ Error"
                font.bold: true
                font.pixelSize: 16
                color: "#F44336"
            }
            
            Label {
                text: errorPopup.errorMessage
                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                color: root.textColor
            }
            
            Item { Layout.fillHeight: true }
            
            Button {
                text: "OK"
                Layout.alignment: Qt.AlignRight
                onClicked: errorPopup.close()
            }
        }
    }
    
    Connections {
        target: appController
        function onError(message) {
            errorPopup.errorMessage = message
            errorPopup.open()
        }
    }
}