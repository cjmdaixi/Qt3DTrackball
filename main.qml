import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Scene3D 2.0
import QtQuick.Controls 2.4

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

        MyEntity{
            id: rootEntity
        }

    }

    Button{
        anchors{
            right: parent.right;
            bottom: parent.bottom;
            margins: 10
        }
        text: "Home"
        onClicked: {
            rootEntity.camera.viewAll();
        }
    }
}
