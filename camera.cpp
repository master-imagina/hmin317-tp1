#include "camera.h"
#include <QtMath>

void Camera::processKeyPress(Camera_Movement movement) {
    float cameraSpeed = 0.1f;
    if(movement == Camera_Movement::FORWARD) {
        cameraPos += cameraSpeed * cameraUp;
    }
    if(movement == Camera_Movement::BACKWARD) {
        cameraPos -= cameraSpeed * cameraUp;
    }
    if(movement == Camera_Movement::LEFT) {
        cameraPos -= QVector3D::crossProduct(cameraFront,cameraUp).normalized() * cameraSpeed;
    }
    if(movement == Camera_Movement::RIGHT) {
        cameraPos += QVector3D::crossProduct(cameraFront,cameraUp).normalized() * cameraSpeed;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    float mouseSensitivity = 0.1f;
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f) {
        pitch = 89.0f;
    }
    if(pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::lookAt(QMatrix4x4 &matrix) {
    matrix.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::updateCameraVectors() {
    QVector3D front;
    front.setX( cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front.setY(sin(qDegreesToRadians(pitch)));
    front.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    cameraFront = front.normalized();
    cameraRight = QVector3D::crossProduct(cameraFront, worldUp).normalized();
    cameraUp = QVector3D::crossProduct(cameraRight, cameraFront).normalized();
}
