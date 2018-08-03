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
    Q_PROPERTY(float trackballSize READ trackballSize WRITE setTrackballSize NOTIFY trackballSizeChanged)

    TrackballCameraController(Qt3DCore::QNode *parent = nullptr);

    QSize windowSize() const
    {
        return m_windowSize;
    }

    float trackballSize() const
    {
        return m_trackballSize;
    }

public slots:
    void setWindowSize(QSize windowSize)
    {
        if (m_windowSize == windowSize)
            return;

        m_windowSize = windowSize;
        emit windowSizeChanged(m_windowSize);
    }

    void setTrackballSize(float trackballSize)
    {
        if (qFuzzyCompare(m_trackballSize, trackballSize))
            return;

        m_trackballSize = trackballSize;
        emit trackballSizeChanged(m_trackballSize);
    }

signals:
    void windowSizeChanged(QSize windowSize);

    void trackballSizeChanged(float trackballSize);

protected:
    void moveCamera(const Qt3DExtras::QAbstractCameraController::InputState &state, float dt) override;
    QVector3D projectToTrackball(const QPoint &screenCoords) const;
    QQuaternion createRotation(const QPoint &firstPoint,
                               const QPoint &nextPoint) const;

private:
    QPoint m_mouseLastPosition, m_mouseCurrentPosition;
    QSize m_windowSize;
    float m_trackballRadius = 1.0f;
    //Movement speed control
    float m_panSpeed = 1.0f;
    float m_zoomSpeed = 1.0f;
    float m_rotationSpeed = 1.0f;
    float m_zoomCameraLimit = 1.0f;
    float m_trackballSize = 0.7f;
};

#endif // TRACKBALLCAMERACONTROLLER_H
