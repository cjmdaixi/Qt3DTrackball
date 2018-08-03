import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Scene3D 2.0

Window {
    id: window
    visible: true
    width: 800
    height: 600
    title: qsTr("Qt3D Trackball")

    Scene3D{
        id: scene3D
        anchors.fill: parent
        focus: true
        aspects: ["input", "logic"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        MyEntity{}

    }
}
