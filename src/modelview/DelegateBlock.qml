import QtQuick
import QtQuick3D

Model {
    source: "#Cube"
    property alias topVisible: mat.topVisible
    property alias rightVisible: mat.rightVisible
    property alias bottomVisible: mat.bottomVisible
    property alias leftVisible: mat.leftVisible
    property alias isVisited: mat.isVisited
    property alias isCurrent: mat.isCurrent
    materials: CustomMaterial {
        id: mat
        property bool topVisible: true
        property bool rightVisible: true
        property bool bottomVisible: true
        property bool leftVisible: true
        property bool isVisited: false
        property bool isCurrent: false
        vertexShader: "cubeMaterial.vert"
        fragmentShader: "cubeMaterial.frag"
        cullMode: Material.NoCulling
    }
}

