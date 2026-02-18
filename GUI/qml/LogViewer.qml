import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15

Rectangle {
    id: logViewer
    color: root.cardColor
    radius: 8
    
    property bool autoScroll: true
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8
        
        // Toolbar
        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            
            // Filter
            ComboBox {
                id: filterCombo
                Layout.preferredWidth: 150
                model: ["ALL", "INFO", "WARNING", "CRITICAL", "WARNING+"]
                onCurrentTextChanged: {
                    console.log("Filter changed to:", currentText)
                    logModel.filterLevel = currentText
                }
                Material.background: root.bgColor
            }
            
            // Search
            TextField {
                id: searchField
                Layout.fillWidth: true
                placeholderText: "🔍 Search..."
                onTextChanged: {
                    console.log("Search text:", text)
                    logModel.searchText = text
                }
                background: Rectangle {
                    color: root.bgColor
                    border.color: searchField.activeFocus ? root.accentColor : "transparent"
                    radius: 4
                }
            }
            
            // Auto-scroll toggle
            Button {
                text: logViewer.autoScroll ? "📜 Auto" : "📜 Manual"
                checkable: true
                checked: logViewer.autoScroll
                onCheckedChanged: logViewer.autoScroll = checked
                Material.background: logViewer.autoScroll ? root.accentColor : undefined
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    color: {
                        if (logViewer.autoScroll) return "#FFFFFF"
                        return appController.darkTheme ? "#eaeaea" : "#1a1a2e"
                    }
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }
            
            
            // Clear
            Button {
                text: "🗑️ Clear"
                onClicked: {
                    console.log("Clear clicked")
                    logModel.clear()
                    appController.clearStats()
                }
            }
            
        }
        
        // Log list
        ListView {
            id: logListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: logModel
            spacing: 2
            
            // Debug: Show when count changes
            onCountChanged: {
                console.log("ListView count changed to:", count)
                if (logViewer.autoScroll && count > 0) {
                    positionViewAtEnd()
                }
            }
            
            delegate: Rectangle {
                id: delegateRoot
                width: logListView.width
                height: 40

                
                color: {
                    if (model.level === "CRITICAL") return Qt.rgba(244/255, 67/255, 54/255, 0.2)
                    if (model.level === "WARNING") return Qt.rgba(255/255, 152/255, 0/255, 0.2)
                    return index % 2 === 0 ? root.bgColor : "transparent"
                }
                radius: 4
                

                
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 4
                    spacing: 8
                    
                    // Timestamp
                    Label {
                        text: model.timestamp || ""
                        font.family: "monospace"
                        font.pixelSize: 11
                        color: Qt.darker(root.textColor, 1.2)
                        Layout.preferredWidth: 140
                    }
                    
                    // Level badge
                    Rectangle {
                        Layout.preferredWidth: 70
                        Layout.preferredHeight: 20
                        radius: 4
                        color: {
                            if (model.level === "INFO") return "#4CAF50"
                            if (model.level === "WARNING") return "#FF9800"
                            if (model.level === "CRITICAL") return "#F44336"
                            return "#9E9E9E"
                        }
                        
                        Label {
                            anchors.centerIn: parent
                            text: model.level || ""
                            font.pixelSize: 10
                            font.bold: true
                            color: "white"
                        }
                    }
                    
                    // Source
                    Label {
                        text: model.source || ""
                        font.family: "monospace"
                        font.pixelSize: 11
                        color: root.accentColor
                        Layout.preferredWidth: 120
                    }
                    
                    // Message
                    Label {
                        text: model.message || ""
                        font.pixelSize: 12
                        color: root.textColor
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                }
            }
            
            ScrollBar.vertical: ScrollBar {
                active: true
                policy: ScrollBar.AlwaysOn
            }
            
            // Show placeholder when empty
            Label {
                anchors.centerIn: parent
                text: logListView.count === 0 ? "No logs yet. Start the app to see logs." : ""
                color: root.textColor
                opacity: 0.5
                visible: logListView.count === 0
            }
        }
    }
}