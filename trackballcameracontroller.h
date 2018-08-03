#ifndef TRACKBALLCAMERACONTROLLER_H
#define TRACKBALLCAMERACONTROLLER_H

#include <Qt3DExtras/QAbstractCameraController>
#include <QPoint>
#include <QSize>

class TrackballCameraController : public Qt3DExtras::QAbstractCameraController
{
    Q_OBJECT
public:
    Q_PROPERTY(QSize windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY()
    TrackballCameraController(Qt3DCore::QNode *parent = nullptr);

    QSize windowSize() const
    {
        return m_windowSize;
    }

public slots:
    void setWindowSize(QSize windowSize)
    {
        if (m_windowSize == windowSize)
            return;

        m_windowSize = windowSize;
        emit windowSizeChanged(m_windowSize);
    }

signals:
    void windowSizeChanged(QSize windowSize);

protected:
    void moveCamera(const Qt3DExtras::QAbstractCameraController::InputState &state, float dt) override;
    QVector3D projectScreenToTrackball(const QPoint &screenCoords,
                                       const QSize &windowSize,
                                       float trackballRadius) const;
    QQuaternion createRotation(const QPoint &firstPoint,
                               const QPoint &nextPoint,
                               const QSize &windowSize,
                               float trackBallRadius) const;

private:
    QPoint m_mouseLastPosition, m_mouseCurrentPosition;
    QSize m_windowSize;
    float m_trackballRadius = 1.0f;
    //Movement speed control
    float m_panSpeed = 1.0f;
    float m_zoomSpeed = 1.0f;
    float m_rotationSpeed = 1.0f;
    float m_zoomCameraLimit = 1.0f;
};

#endif // TRACKBALLCAMERACONTROLLER_H
