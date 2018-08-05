import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtQuick 2.0 as QQ2
import TrackballCameraController 1.0
import QtQuick.Window 2.11

Entity{
    id: root

    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 0.0, 0.0, 40.0 )
        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    TrackballCameraController{
        camera: camera
        windowSize: Qt.size(window.width, window.height)
        rotationSpeed: 2.0
    }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                camera: camera
                clearColor: "transparent"
            }
        },
        InputSettings { }
    ]

    PhongMaterial {
        id: material
    }

//    TorusMesh {
//        id: torusMesh
//        radius: 5
//        minorRadius: 1
//        rings: 100
//        slices: 20
//    }
    CuboidMesh{
        id: cuboidMesh
        xExtent: 10
        yExtent: 10
        zExtent: 10
    }

    Transform {
        id: torusTransform
        //scale3D: Qt.vector3d(1.5, 1, 0.5)
        //rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
    }

    Entity {
        id: torusEntity
        components: [ cuboidMesh, material, torusTransform ]
    }
}
