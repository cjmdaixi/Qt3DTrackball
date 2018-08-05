#include "trackballcameracontroller.h"
#include <Qt3DRender/QCamera>
#include <QVector2D>
#include <Qt3DInput/QAction>
#include <Qt3DInput/QActionInput>
#include <Qt3DInput/QAxis>
#include <Qt3DInput/QAnalogAxisInput>
#include <Qt3DInput/QMouseDevice>
#include <Qt3DInput/QKeyboardDevice>
#include <Qt3DInput/QMouseHandler>
#include <Qt3DInput/QKeyboardHandler>
#include <Qt3DCore/QTransform>
#include <QtMath>

TrackballCameraController::TrackballCameraController(Qt3DCore::QNode *parent)
    : Qt3DExtras::QAbstractCameraController (parent)
{
    //// Mouse handler
    Qt3DInput::QMouseHandler *mouseHandler = new Qt3DInput::QMouseHandler(this);
    mouseHandler->setSourceDevice(mouseDevice());

    QObject::connect(mouseHandler, &Qt3DInput::QMouseHandler::pressed,
                     [this](Qt3DInput::QMouseEvent *pressedEvent) {
        //qDebug()<<"mouse pressed at: x:"<<pressedEvent->x() <<"y:"<<pressedEvent->y();
        //qDebug()<<"window size: width:"<<m_windowSize.width()<<"height:"<<m_windowSize.height();
        pressedEvent->setAccepted(true);
        m_mouseLastPosition = QPoint(pressedEvent->x(),
                                           pressedEvent->y());
        //projectToTrackball(m_mouseLastPosition);
    });

    QObject::connect(mouseHandler, &Qt3DInput::QMouseHandler::released,
                     [this](Qt3DInput::QMouseEvent *releasedEvent) {
        //qDebug()<<"mouse pressed at: x:"<<pressedEvent->x() <<"y:"<<pressedEvent->y();
        //qDebug()<<"window size: width:"<<m_windowSize.width()<<"height:"<<m_windowSize.height();
        releasedEvent->setAccepted(true);
        m_mouseLastPosition = QPoint(releasedEvent->x(),
                                           releasedEvent->y());
    });

    QObject::connect(mouseHandler, &Qt3DInput::QMouseHandler::positionChanged,
                     [this](Qt3DInput::QMouseEvent *positionChangedEvent) {
        //qDebug()<<"mouse moved at: x:"<<positionChangedEvent->x() <<"y:"<<positionChangedEvent->y();
        positionChangedEvent->setAccepted(true);
        this->m_mouseCurrentPosition = QPoint(positionChangedEvent->x(),
                                              positionChangedEvent->y());
    });
}

QVector3D TrackballCameraController::projectToTrackball(const QPoint &screenCoords) const
{
    float sx = screenCoords.x(), sy = m_windowSize.height() - screenCoords.y();

    QVector2D p2d(sx / m_windowSize.width() - 0.5f, sy / m_windowSize.height() - 0.5f);
    //qDebug()<<p2d;

    float z = 0.0f;
    float r2 = m_trackballSize * m_trackballSize;
    if (p2d.lengthSquared() <= r2 * 0.5f){
        z = sqrt(r2 - p2d.lengthSquared());
    }else{
        z = r2 * 0.5f / p2d.length();
    }
    QVector3D p3d(p2d, z);
    //qDebug()<<p3d;
    return p3d;
}

float clamp(float x)
{
    return x > 1? 1 : (x < -1? -1 : x);
}

QQuaternion TrackballCameraController::createRotation(const QPoint &firstPoint,
                                                              const QPoint &nextPoint) const
{
    QVector3D lastPos3D = projectToTrackball(firstPoint).normalized();
    QVector3D currentPos3D = projectToTrackball(nextPoint).normalized();

    // Compute axis of rotation:
    QVector3D dir = QVector3D::crossProduct(currentPos3D, lastPos3D).normalized();

    // Approximate rotation angle:
    float t = acos(clamp(QVector3D::dotProduct(currentPos3D, lastPos3D)));

    QQuaternion q = QQuaternion::fromAxisAndAngle(dir, t);
    //qDebug()<<"dir:"<<dir<<"angle:"<<t;
    return q;
}

void TrackballCameraController::moveCamera(const Qt3DExtras::QAbstractCameraController::InputState &state, float dt)
{
    auto theCamera = camera();

    if(theCamera == nullptr)
        return;

    if(state.leftMouseButtonActive){
//        theCamera->tiltAboutViewCenter(-state.ryAxisValue);
//        theCamera->panAboutViewCenter(-state.rxAxisValue);
        QQuaternion rotation = createRotation(m_mouseLastPosition, m_mouseCurrentPosition);

        QQuaternion currentRotation = theCamera->transform()->rotation();
        QQuaternion currentRotationInversed = currentRotation;//.conjugated();

        QVector3D rotatedAxis = currentRotationInversed.rotatedVector(rotation.vector());
        float angle = m_rotationSpeed * rotation.scalar();

        theCamera->rotateAboutViewCenter(QQuaternion::fromAxisAndAngle(rotatedAxis, angle));
        m_mouseLastPosition = m_mouseCurrentPosition;
    }
}
