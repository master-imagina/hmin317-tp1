#ifndef MATRIX_H
#define MATRIX_H
#include "camerathird.h"


#include "external/glm/vec3.hpp"
#include "external/glm/mat4x4.hpp"
#include <external/glm/gtc/matrix_transform.hpp>

class Matrix
{
public:
    Matrix();

    static glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
    static glm::mat4 createViewMatrix(CameraThird camera);
    static glm::mat4 createProjectionMatrix(Camera camera, int w, int h);

};

#endif // MATRIX_H
