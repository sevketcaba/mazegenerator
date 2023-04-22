import QtQuick
import QtQuick3D

Model {
    source: "#Cube"
    materials: CustomMaterial {
        vertexShader: "cubeMaterial.vert"
        fragmentShader: "cubeMaterial.frag"
        cullMode: Material.NoCulling
    }
}
