import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import tr.gen.caba.qmltypes 1.0
Rectangle {
    color: "lightsteelblue"

    property int renderStyle: btnModelView.checked ? 0 : 1

    Rectangle {
        //  a thin border
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 2
        color: "darkGray"
    }


    ColumnLayout {
        spacing: 10
        anchors.fill: parent
        anchors.margins: 5

        GroupBox {
            title: " Render Style "
            Layout.fillWidth: true

            Column {
                spacing: 10
                RadioButton {
                    id: btnModelView
                    text: "ModelView"
                    checked: true
                    enabled: mazeGen.genState !== MazeGenerator.Solving
                }

                RadioButton {
                    id: btnInstancing
                    text: "Instancing"
                    enabled: mazeGen.genState !== MazeGenerator.Solving
                }
            }
        }

        GroupBox {
            title: " Grid Size "
            Layout.fillWidth: true

            Row {
                spacing: 5
                SpinBox {
                    id: gridSize
                    from: 1
                    to: 1000
                    value: 10
                    enabled: mazeGen.genState !== MazeGenerator.Solving
                    editable: true
                }

                Button {
                    id: btnInit
                    enabled: mazeGen.genState !== MazeGenerator.Solving
                    text: "Set"
                    onClicked: {
                        mazeGen.init(gridSize.value)
                    }
                }
            }
        }

        GroupBox {
            title: " Interval "
            Layout.fillWidth: true

            Row {
                spacing: 5

                SpinBox {
                    id: speed
                    from: 0
                    to: 10000 // milliseconds
                    value: mazeGen.interval
                    enabled: mazeGen.genState !== MazeGenerator.Solving
                    editable: true
                }

                Button {
                    id: btnSetSpeed
                    enabled: mazeGen.genState !== MazeGenerator.Solving
                    text: "Set"
                    onClicked: {
                        mazeGen.setInterval(speed.value)
                    }
                }
            }
        }

        Button {
            id: btnStart
            Layout.fillWidth: true
            enabled: mazeGen.genState !== MazeGenerator.Idle
            text: mazeGen.genState === MazeGenerator.Stopped
                  ? "Resume"
                  : mazeGen.genState === MazeGenerator.Solving
                    ? "Stop"
                    : "Start"
            onClicked: {
                if(mazeGen.genState === MazeGenerator.Solving)
                    mazeGen.stop()
                else
                    mazeGen.start()
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    }
}
