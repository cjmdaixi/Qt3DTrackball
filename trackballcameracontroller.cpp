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
        this->m_mouseLastPosition = QPoint(pressedEvent->x(),
                                           pressedEvent->y());
    });

    QObject::connect(mouseHandler, &Qt3DInput::QMouseHandler::released,
                     [this](Qt3DInput::QMouseEvent *releasedEvent) {
        //qDebug()<<"mouse pressed at: x:"<<pressedEvent->x() <<"y:"<<pressedEvent->y();
        //qDebug()<<"window size: width:"<<m_windowSize.width()<<"height:"<<m_windowSize.height();
        releasedEvent->setAccepted(true);
        this->m_mouseLastPosition = QPoint(releasedEvent->x(),
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

QVector3D TrackballCameraController::projectScreenToTrackball(const QPoint &screenCoords,
                                                              const QSize &windowSize,
                                                              float trackballRadius) const
{
    QVector2D p2D(2.0 * camera()->aspectRatio()*((float)screenCoords.x()/(float)windowSize.width() - 0.5),
                  -2.0*((float)screenCoords.y()/(float)windowSize.height() - 0.5));

    //QVector2D p2D((float)screenCoords.x()/(float)windowSize.width(), (float)screenCoords.y()/(float)windowSize.height());
    qDebug() << p2D;

    QVector3D p3D(p2D, 0);

    float r = trackballRadius;
    float z0 = r * 0.5f;

    float z = 0;
    // If point over trackball, compute Z for the sphere
    if (r*r - p3D.lengthSquared() >= z0*z0) {
        z = sqrt(r*r - p3D.lengthSquared());
    }
    // Else, compute Z for an hyperbole
    else {
        z = z0*std::sqrt(r*r - z0*z0) / p3D.length();
    }

    p3D[2] = z;
    return p3D;
}

QQuaternion TrackballCameraController::createRotation(const QPoint &firstPoint,
                                                              const QPoint &nextPoint,
                                                              const QSize &windowSize,
                                                              float trackBallRadius) const
{
    QVector3D lastPos3D = projectScreenToTrackball(firstPoint, windowSize, trackBallRadius);
    QVector3D currentPos3D = projectScreenToTrackball(nextPoint, windowSize, trackBallRadius);

    QVector3D posAvg = (lastPos3D + currentPos3D) / 2;

    // Compute axis of rotation:
    QVector3D dir = QVector3D::crossProduct(currentPos3D,lastPos3D);

    // Approximate rotation angle:
    float t = dir.length()/posAvg.length();

    QQuaternion q = QQuaternion::fromAxisAndAngle(dir, t);
    return q;
}

void TrackballCameraController::moveCamera(const Qt3DExtras::QAbstractCameraController::InputState &state, float dt)
{
    auto theCamera = camera();

    if(theCamera == nullptr)
        return;

    if(state.leftMouseButtonActive){
        QQuaternion rotation = createRotation(m_mouseLastPosition,
                                              m_mouseCurrentPosition,
                                              m_windowSize,
                                              m_trackballRadius);

        QQuaternion currentRotation = theCamera->transform()->rotation();
        QQuaternion currentRotationInversed = currentRotation.conjugated();

        QVector3D rotatedAxis = currentRotationInversed.rotatedVector(rotation.vector());
        float angle = m_rotationSpeed*rotation.scalar();

        theCamera->rotateAboutViewCenter(QQuaternion::fromAxisAndAngle(rotatedAxis,
                                                                      angle));
        m_mouseLastPosition = m_mouseCurrentPosition;
    }
}
