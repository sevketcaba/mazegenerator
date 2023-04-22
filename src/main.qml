import QtQuick
import QtQuick.Controls
import QtQuick3D
import QtQuick3D.Helpers
import tr.gen.caba.qmltypes 1.0

import "common"
import "modelview"
import "instancerendering"

Window {
    id: window
    width: 1024
    height: 768
    visible: true
    title: qsTr("Maze Generator")

    property double defaultBoxSize: 100 // this is the size of the #Cube primitive model
    property double desiredBoxSize: 1 // this is the size that we want
    property double scaleFactor: desiredBoxSize / defaultBoxSize

    Item {
        id: root
        anchors.fill: parent

        states: [
            State {
                when: controlArea.renderStyle == 0
                PropertyChanges {
                    target: mazeGen
                    instanceTable: null
                    cubeModel: cubeModel
                }
                PropertyChanges {
                    target: sceneNode
                    w: cubeModel.cols * desiredBoxSize
                }
                PropertyChanges {
                    target: instanceBlock
                    visible: false
                }
                PropertyChanges {
                    target: cubeModelRepeater
                    visible: true
                }
            },
            State {
                when: controlArea.renderStyle == 1
                PropertyChanges {
                    target: mazeGen
                    instanceTable: instanceTable
                    cubeModel: null
                }
                PropertyChanges {
                    target: sceneNode
                    w: instanceTable.cols * desiredBoxSize
                }
                PropertyChanges {
                    target: instanceBlock
                    visible: true
                }
                PropertyChanges {
                    target: cubeModelRepeater
                    visible: false
                }
            }
        ]

        ControlArea {
            id: controlArea
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 180
        }

        CubeModel {
            id: cubeModel
        }

        InstanceTable {
            id: instanceTable
            boxSize: desiredBoxSize
        }

        MazeGenerator {
            id: mazeGen
        }

        View3D {
            id: view3D
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: controlArea.left

            environment: SceneEnvironment {
                backgroundMode: SceneEnvironment.Color;
                clearColor: "steelblue"
                antialiasingMode: SceneEnvironment.MSAA
                antialiasingQuality: SceneEnvironment.VeryHigh
            }

            PerspectiveCamera {
                id: pcam
                position: Qt.vector3d(0,0,1115 * scaleFactor)
                clipNear: 1
                clipFar: 10000
            }

            OrbitCameraController {
                anchors.fill: parent
                camera: pcam
                origin: sceneNode
                xInvert: true
                yInvert: false
                xSpeed: .5
                ySpeed: .5
            }

            Node {
                id: sceneNode
                property double w: 0

                InstanceBlock {
                    id: instanceBlock
                    instancing: instanceTable
                    scale: Qt.vector3d(scaleFactor, scaleFactor, scaleFactor)
                }

                Repeater3D {
                    id: cubeModelRepeater
                    model: cubeModel
                    delegate: DelegateBlock {
                        property double x: ((model.column - (cubeModel.cols/2)) * desiredBoxSize) + (desiredBoxSize / 2.0)
                        property double y: ((model.row - (cubeModel.rows/2)) * desiredBoxSize) + (desiredBoxSize / 2.0)
                        property double z: 0

                        position: Qt.vector3d(x,y,z)
                        scale: Qt.vector3d(scaleFactor, scaleFactor, scaleFactor/2.0)

                        isVisited: model.visited
                        isCurrent: model.current
                        leftVisible: model.column === 0
                        topVisible: model.row === 0
                        rightVisible: model.right
                        bottomVisible: model.bottom
                    }
                }

                DirectionalLight {
                    id: directionalLight
                    castsShadow: true
                    shadowMapQuality: Light.ShadowMapQualityHigh
                    eulerRotation: Qt.vector3d(-30, -30, 0)
                    color: Qt.rgba(0.4, 0.4, 0.4, 1.0)
                    ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
                }


                Node {
                    id: directionalLightIndicator
                    position: Qt.vector3d((-sceneNode.w / 2) - (desiredBoxSize/2.0),
                                          (sceneNode.w / 2) + (desiredBoxSize/2.0),
                                          (desiredBoxSize*3.0))
                    scale: Qt.vector3d(scaleFactor, scaleFactor, scaleFactor)
                    eulerRotation: directionalLight.eulerRotation

                    Model {
                        source: "#Cone"
                        castsShadows: false
                        eulerRotation.x: 90 // to align the cone shape with the default directional light direction
                        materials: DefaultMaterial {
                            diffuseColor: directionalLight.color
                            lighting: DefaultMaterial.NoLighting
                            opacity: .6
                        }
                    }
                }

                /// NOTE: to mark the center point
//                Model {
//                    source: "#Sphere"
//                    scale: Qt.vector3d(scaleFactor/10.0,scaleFactor/10.0,scaleFactor/10.0)
//                    materials: DefaultMaterial {
//                        diffuseColor: directionalLight.color
//                        lighting: DefaultMaterial.NoLighting
//                        opacity: .6
//                    }
//                }
            }


        }

        DebugView {
            id: rectStats
            source: view3D
        }

    }
}
