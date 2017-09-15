#include "matrix.h"
#include <iostream>
#include <math.h>

Matrix::Matrix()
{

}

glm::mat4 Matrix::createViewMatrix(CameraThird camera){

    glm::mat4 viewMatrix = glm::mat4(1.0);

    viewMatrix = glm::rotate(viewMatrix,Utils::toRadians(camera.getPitch()), glm::vec3(1.0f, 0.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix,Utils::toRadians(camera.getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
    viewMatrix = glm::rotate(viewMatrix,Utils::toRadians(camera.getRoll()), glm::vec3(0.0f, 0.0f, 1.0f));
    viewMatrix = glm::translate(viewMatrix, glm::vec3(-camera.getPosition().x,-camera.getPosition().y,-camera.getPosition().z));
   /* viewMatrix = rotate(Utils::toRadians(camera.getPitch()), Vector(1.0f, 0.0f, 0.0f),viewMatrix);
    viewMatrix = rotate(Utils::toRadians(camera.getYaw()), Vector(0.0f, 1.0f, 0.0f),viewMatrix);
    viewMatrix = rotate(Utils::toRadians(camera.getRoll()), Vector(0.0f, 0.0f, 1.0f),viewMatrix);
    /*vmath::vec3 negativeCameraPos(-camera.getPosition().x,-camera.getPosition().y,-camera.getPosition().z);
    viewMatrix = vmath::translate(negativeCameraPos);*/

    return viewMatrix;

}

glm::mat4 Matrix::createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale){
    glm::mat4 matrix = glm::mat4(1.0);
    matrix = glm::translate(matrix,translation);

    matrix = glm::rotate(matrix,Utils::toRadians(rotation.x), glm::vec3(1.0,0.0,0.0));
    matrix = glm::rotate(matrix,Utils::toRadians(rotation.y), glm::vec3(0.0,1.0,0.0));
    matrix = glm::rotate(matrix,Utils::toRadians(rotation.z), glm::vec3(0.0,0.0,1.0));

    matrix = glm::scale(matrix,scale);
    return matrix;
}

glm::mat4 Matrix::createProjectionMatrix(Camera camera, int w, int h) {
    glm::mat4 projectionMatrix = glm::mat4();
    float aspectRatio = (float) w / (float) h;
    float y_scale = (float)((1.0f / tan(Utils::toRadians(camera.getFOV() / 2.0f))) * aspectRatio);
    float x_scale = y_scale / aspectRatio;
    float frustum_length = camera.getFAR_PLANE() - camera.getNEAR_PLANE();

    projectionMatrix[0][0] = x_scale;
    projectionMatrix[1][1] = y_scale;
    projectionMatrix[2][2] = -((camera.getFAR_PLANE() + camera.getNEAR_PLANE()) / frustum_length);
    projectionMatrix[2][3] = -1;
    projectionMatrix[3][2] = -((2 * camera.getNEAR_PLANE() * camera.getFAR_PLANE()) / frustum_length);
    projectionMatrix[3][3] = 0;

    return projectionMatrix;
}

