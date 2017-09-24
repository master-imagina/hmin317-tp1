#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <QVector3D>

class QKeyEvent;
class QMouseEvent;


class CameraController : public QObject
{
    Q_OBJECT

public:
    CameraController(QObject *parent = nullptr);
    ~CameraController();

    bool eventFilter(QObject *obj, QEvent *e) override;

    void updateCamera(QVector3D &eye, QVector3D &target, QVector3D &up);

private:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    QVector3D computeDirectionFromKeys(const QVector3D &eye,
                                       const QVector3D &target,
                                       const QVector3D &up) const;

private:
    enum KeyDirection
    {
        None = 0,
        Up = 1,
        Down = 2,
        Right = 4,
        Left = 8
    };

    KeyDirection m_keyDirection;

    float m_moveSpeed;
    float m_turboSpeed;
    bool m_turboKeyPressed;
};

#endif // CAMERACONTROLLER_H
