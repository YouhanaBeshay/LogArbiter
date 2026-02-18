import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import QtQuick.Dialogs

Dialog {
    id: configDialog
    title: "⚙ Configuration Editor"
    modal: true
    width: 700
    height: 700
    anchors.centerIn: parent
    
    property var configData: appController.config
    
    // File sinks list model
    ListModel {
        id: fileSinksModel
    }
    
    background: Rectangle {
        color: root.cardColor
        border.color: root.accentColor
        border.width: 2
        radius: 8
    }
    
    header: Rectangle {
        height: 50
        color: root.bgColor
        radius: 8
        
        Label {
            anchors.centerIn: parent
            text: "⚙ Configuration Editor"
            font.pixelSize: 18
            font.bold: true
            color: root.accentColor
        }
    }
    
    contentItem: ScrollView {
        clip: true
        contentWidth: availableWidth
        
        ColumnLayout {
            width: parent.width - 20
            spacing: 16
            
            // Log Manager Section
            GroupBox {
                Layout.fillWidth: true
                title: "Log Manager"
                
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
                
                GridLayout {
                    columns: 2
                    columnSpacing: 16
                    rowSpacing: 12
                    
                    Label { text: "Buffer Capacity:"; color: root.textColor }
                    SpinBox {
                        id: bufferCapacity
                        from: 10
                        to: 10000
                        value: configData.log_manager ? configData.log_manager.buffer_capacity : 100
                        editable: true
                    }
                    
                    Label { text: "Thread Pool Size:"; color: root.textColor }
                    SpinBox {
                        id: threadPoolSize
                        from: 1
                        to: 16
                        value: configData.log_manager ? configData.log_manager.thread_pool_size : 2
                    }
                    
                    Label { text: "Sink Flush Rate (ms):"; color: root.textColor }
                    SpinBox {
                        id: sinkFlushRate
                        from: 100
                        to: 10000
                        stepSize: 100
                        value: configData.log_manager ? configData.log_manager.sink_flush_rate_ms : 1000
                        editable: true
                    }
                }
            }
            
            // Sinks Section
            GroupBox {
                Layout.fillWidth: true
                title: "Sinks"
                
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
                    spacing: 12
                    
                    CheckBox {
                        id: consoleEnabled
                        text: "Console Output"
                        checked: configData.sinks && configData.sinks.console ? configData.sinks.console.enabled : true
                    }
                    
                    // File Sinks Header
                    RowLayout {
                        Layout.fillWidth: true
                        
                        Label {
                            text: "File Sinks:"
                            color: root.textColor
                            font.bold: true
                        }
                        
                        Item { Layout.fillWidth: true }
                        
                        Button {
                            text: "+ Add File"
                            onClicked: {
                                fileSinksModel.append({
                                    "enabled": true,
                                    "path": "new_log_" + (fileSinksModel.count + 1) + ".log"
                                })
                            }
                        }
                    }
                    
                    // File Sinks List
                    Repeater {
                        model: fileSinksModel
                        
                        delegate: RowLayout {
                            Layout.fillWidth: true
                            spacing: 8
                            
                            CheckBox {
                                checked: model.enabled
                                onCheckedChanged: fileSinksModel.setProperty(index, "enabled", checked)
                            }
                            
                            TextField {
                                Layout.fillWidth: true
                                text: model.path
                                placeholderText: "File path"
                                onTextChanged: fileSinksModel.setProperty(index, "path", text)
                            }
                            
                            Button {
                                text: "🗑️"
                                width: 40
                                onClicked: fileSinksModel.remove(index)
                            }
                        }
                    }
                    
                    // Show message if no file sinks
                    Label {
                        visible: fileSinksModel.count === 0
                        text: "No file sinks configured. Click '+ Add File' to add one."
                        color: root.textColor
                        opacity: 0.6
                        font.italic: true
                    }
                }
            }
            
            // Sources Section
            GroupBox {
                Layout.fillWidth: true
                title: "Sources"
                
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
                    spacing: 16
                    
                    // File Source
                    GroupBox {
                        Layout.fillWidth: true
                        title: "📁 File Source"
                        
                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8
                            
                            RowLayout {
                                CheckBox {
                                    id: fileSourceEnabled
                                    text: "Enabled"
                                    checked: configData.sources && configData.sources.file ? configData.sources.file.enabled : false
                                }
                                
                                Item { Layout.fillWidth: true }
                                
                                Label { text: "Policy:"; color: root.textColor }
                                ComboBox {
                                    id: filePolicy
                                    model: ["cpu", "ram", "gpu"]
                                    currentIndex: {
                                        var policy = configData.sources && configData.sources.file ? configData.sources.file.policy : "cpu"
                                        return model.indexOf(policy)
                                    }
                                    enabled: fileSourceEnabled.checked
                                }
                            }
                            
                            RowLayout {
                                Label { text: "Path:"; color: root.textColor; Layout.preferredWidth: 80 }
                                TextField {
                                    id: fileSourcePath
                                    Layout.fillWidth: true
                                    text: configData.sources && configData.sources.file ? configData.sources.file.path : ""
                                    enabled: fileSourceEnabled.checked
                                    placeholderText: "/path/to/telemetry/file"
                                }
                            }
                            
                            RowLayout {
                                Label { text: "Parse Rate:"; color: root.textColor; Layout.preferredWidth: 80 }
                                SpinBox {
                                    id: fileParseRate
                                    from: 100
                                    to: 10000
                                    stepSize: 100
                                    value: configData.sources && configData.sources.file ? configData.sources.file.parse_rate_ms : 1000
                                    enabled: fileSourceEnabled.checked
                                    editable: true
                                }
                                Label { text: "ms"; color: root.textColor }
                            }
                        }
                    }
                    
                    // Socket Source
                    GroupBox {
                        Layout.fillWidth: true
                        title: "🔌 Socket Source"
                        
                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8
                            
                            RowLayout {
                                CheckBox {
                                    id: socketSourceEnabled
                                    text: "Enabled"
                                    checked: configData.sources && configData.sources.socket ? configData.sources.socket.enabled : false
                                }
                                
                                Item { Layout.fillWidth: true }
                                
                                Label { text: "Policy:"; color: root.textColor }
                                ComboBox {
                                    id: socketPolicy
                                    model: ["cpu", "ram", "gpu"]
                                    currentIndex: {
                                        var policy = configData.sources && configData.sources.socket ? configData.sources.socket.policy : "ram"
                                        return model.indexOf(policy)
                                    }
                                    enabled: socketSourceEnabled.checked
                                }
                            }
                            
                            RowLayout {
                                Label { text: "Path:"; color: root.textColor; Layout.preferredWidth: 80 }
                                TextField {
                                    id: socketSourcePath
                                    Layout.fillWidth: true
                                    text: configData.sources && configData.sources.socket ? configData.sources.socket.path : ""
                                    enabled: socketSourceEnabled.checked
                                    placeholderText: "/path/to/socket.sock"
                                }
                            }
                            
                            RowLayout {
                                Label { text: "Parse Rate:"; color: root.textColor; Layout.preferredWidth: 80 }
                                SpinBox {
                                    id: socketParseRate
                                    from: 100
                                    to: 10000
                                    stepSize: 100
                                    value: configData.sources && configData.sources.socket ? configData.sources.socket.parse_rate_ms : 1000
                                    enabled: socketSourceEnabled.checked
                                    editable: true
                                }
                                Label { text: "ms"; color: root.textColor }
                            }
                        }
                    }
                    
                    // SomeIP Source
                    GroupBox {
                        Layout.fillWidth: true
                        title: "📡 SomeIP Source"
                        
                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 8
                            
                            RowLayout {
                                CheckBox {
                                    id: someipSourceEnabled
                                    text: "Enabled"
                                    checked: configData.sources && configData.sources.someip ? configData.sources.someip.enabled : false
                                }
                                
                                Item { Layout.fillWidth: true }
                                
                                Label { text: "Policy:"; color: root.textColor }
                                ComboBox {
                                    id: someipPolicy
                                    model: ["cpu", "ram", "gpu"]
                                    currentIndex: {
                                        var policy = configData.sources && configData.sources.someip ? configData.sources.someip.policy : "gpu"
                                        return model.indexOf(policy)
                                    }
                                    enabled: someipSourceEnabled.checked
                                }
                            }
                            
                            RowLayout {
                                Label { text: "Parse Rate:"; color: root.textColor; Layout.preferredWidth: 80 }
                                SpinBox {
                                    id: someipParseRate
                                    from: 100
                                    to: 10000
                                    stepSize: 100
                                    value: configData.sources && configData.sources.someip ? configData.sources.someip.parse_rate_ms : 1000
                                    enabled: someipSourceEnabled.checked
                                    editable: true
                                }
                                Label { text: "ms"; color: root.textColor }
                            }
                        }
                    }
                }
            }
        }
    }
    
    footer: DialogButtonBox {
        Button {
            text: "Cancel"
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
        }
        Button {
            text: "Apply"
            highlighted: true
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
        }
    }
    
    onOpened: {
        // Load current config
        configData = appController.config
        
        // Populate file sinks model
        fileSinksModel.clear()
        if (configData.sinks && configData.sinks.files) {
            for (var i = 0; i < configData.sinks.files.length; i++) {
                var sink = configData.sinks.files[i]
                fileSinksModel.append({
                    "enabled": sink.enabled || false,
                    "path": sink.path || ""
                })
            }
        }
    }
    
    onAccepted: {
        // Build file sinks array
        var fileSinks = []
        for (var i = 0; i < fileSinksModel.count; i++) {
            var item = fileSinksModel.get(i)
            if (item.path && item.path.trim() !== "") {
                fileSinks.push({
                    "enabled": item.enabled,
                    "path": item.path
                })
            }
        }
        
        // Build new config object
        var newConfig = {
            "log_manager": {
                "buffer_capacity": bufferCapacity.value,
                "thread_pool_size": threadPoolSize.value,
                "sink_flush_rate_ms": sinkFlushRate.value
            },
            "sinks": {
                "console": { "enabled": consoleEnabled.checked },
                "files": fileSinks
            },
            "sources": {
                "file": {
                    "enabled": fileSourceEnabled.checked,
                    "path": fileSourcePath.text,
                    "parse_rate_ms": fileParseRate.value,
                    "policy": filePolicy.currentText
                },
                "socket": {
                    "enabled": socketSourceEnabled.checked,
                    "path": socketSourcePath.text,
                    "parse_rate_ms": socketParseRate.value,
                    "policy": socketPolicy.currentText
                },
                "someip": {
                    "enabled": someipSourceEnabled.checked,
                    "parse_rate_ms": someipParseRate.value,
                    "policy": someipPolicy.currentText
                }
            }
        }
        
        console.log("Saving config:", JSON.stringify(newConfig, null, 2))
        appController.updateConfig(newConfig)
    }
}