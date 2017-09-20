#include "cameracontroller.h"

#include <QKeyEvent>


CameraController::CameraController(QObject *parent) :
    QObject(parent),
    m_keyDirection(KeyDirection::None)
{}

CameraController::~CameraController()
{}

bool CameraController::eventFilter(QObject *obj, QEvent *e)
{
    const QEvent::Type eventType = e->type();

    if (eventType == QEvent::KeyPress) {
        auto keyEvent = static_cast<QKeyEvent *>(e);

        keyPressEvent(keyEvent);

        return false;
    }
    else if (eventType == QEvent::KeyRelease) {
        auto keyEvent = static_cast<QKeyEvent *>(e);

        keyReleaseEvent(keyEvent);

        return false;
    }

    return QObject::eventFilter(obj, e);
}

QVector3D CameraController::computeDirectionFromKeys(const QVector3D &eye,
                                                     const QVector3D &target,
                                                     const QVector3D &up) const
{
    const QVector3D frontDir = (target - eye).normalized();
    const QVector3D rightDir = QVector3D::crossProduct(frontDir, up);

    QVector3D ret;

    switch (m_keyDirection) {
    case KeyDirection::None:
        break;
    case KeyDirection::Up:
        ret = frontDir;
        break;
    case KeyDirection::Down:
        ret = frontDir * -1;
        break;
    case KeyDirection::Left:
        ret = rightDir * -1;
        break;
    case KeyDirection::Right:
        ret = rightDir;
        break;
    }

    return ret;
}

void CameraController::updateViewMatrix(QVector3D &eye,
                                        QVector3D &target,
                                        QVector3D &up)
{
    const QVector3D oldEye = eye;

    const float moveAmount = (12 / 1000.0f);
    const QVector3D newEye =
            computeDirectionFromKeys(oldEye, target, up) * moveAmount + oldEye;

    eye = newEye;
    target += (newEye - oldEye);
}

void CameraController::keyPressEvent(QKeyEvent *event)
{
    // Key handling
    const int key = event->key();

    KeyDirection newDirection = KeyDirection::None;

    if (key == Qt::Key_Z) {
        newDirection = KeyDirection::Up;
    }
    else if (key == Qt::Key_S) {
        newDirection = KeyDirection::Down;
    }
    else if (key == Qt::Key_Q) {
        newDirection = KeyDirection::Left;
    }
    else if (key == Qt::Key_D) {
        newDirection = KeyDirection::Right;
    }

    m_keyDirection = (KeyDirection) ((int) m_keyDirection | (int) newDirection);
}

void CameraController::keyReleaseEvent(QKeyEvent *event)
{
    const int key = event->key();

    KeyDirection directionToExit = KeyDirection::None;

    if (key == Qt::Key_Z) {
        directionToExit = KeyDirection::Up;
    }
    else if (key == Qt::Key_S) {
        directionToExit = KeyDirection::Down;
    }
    else if (key == Qt::Key_Q) {
        directionToExit = KeyDirection::Left;
    }
    else if (key == Qt::Key_D) {
        directionToExit = KeyDirection::Right;
    }
    else {
        directionToExit = KeyDirection::None;
    }

    m_keyDirection = (KeyDirection) ((int) m_keyDirection & (~(int) directionToExit));
}
