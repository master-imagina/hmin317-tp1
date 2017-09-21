/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "geometryengine.h"

#include <QVector2D>
#include <QVector3D>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
GeometryEngine::GeometryEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    initPlaneGeometry();
}

GeometryEngine::~GeometryEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void GeometryEngine::initCubeGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
    };

    GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 34 * sizeof(GLushort));
}

    /*while(k < 16*16)
    {
         vertices[k] = {QVector3D(j, -1.0f,  0.0f), QVector2D(0.0f, 0.0f)};
         vertices[k+1] = {QVector3D(i, -1.0f,  0.0f), QVector2D(0.33f, 0.0f)};
         vertices[k+2] = {QVector3D(j, 1.0f,  0.0f), QVector2D(0.0f, 0.5f)};
         vertices[k+3] = {QVector3D(i, 1.0f,  0.0f), QVector2D(0.33f, 0.5f)};
         k = k+4;

         i = -1.0f + -1.0f * k;

         if(k != 0) j = 1.0f + 1.0f * k;

         cout << k << endl;
    }*/

    /*VertexData vertices[] = {


        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  0.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( 1.0f, -1.0f,  0.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-1.0f,  1.0f,  0.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( 1.0f,  1.0f,  0.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( 1.0f, -1.0f,  0.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( 3.0f, -1.0f, -0.0f), QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( 1.0f,  1.0f,  0.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( 3.0f,  1.0f, -0.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D(-1.0f, -3.0f, -0.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D( 1.0f, -3.0f, -0.0f), QVector2D(1.0f, 0.5f)},  // v9
        {QVector3D(-1.0f, -1.0f, -0.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D( 1.0f, -1.0f, -0.0f), QVector2D(1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D( 1.0f, -3.0f, -0.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D( 3.0f, -3.0f,  0.0f), QVector2D(1.0f, 0.0f)},  // v13
        {QVector3D( 1.0f, -1.0f, -0.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D( 3.0f, -1.0f,  0.0f), QVector2D(1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-1.0f, -5.0f, -0.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( 1.0f, -5.0f, -0.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-1.0f, -3.0f,  0.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( 1.0f, -3.0f,  0.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D( 1.0f,  -5.0f,  0.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( 3.0f,  -5.0f,  0.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D( 1.0f,  -3.0f, -0.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( 3.0f,  -3.0f, -0.0f), QVector2D(0.66f, 1.0f)}  // v23
    };*/

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
   /*GLushort indices[] = {
             0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
             4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
             8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
            12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
            16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
            20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
        };*/

    /*GLushort indices[] = {
         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         //4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         //8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
       // 12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        //16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        //20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };*/

void GeometryEngine::initPlaneGeometry()
{

    int i[6] = {-1.0f,0.0f,0.0f,0.0f,1.0f,2.0f};
    int j[6] = {-1.0f,-2.0f,-1.0f,0.0f,-1.0f,-1.0f};

    const int faces = 16;
        const int nb_faces = faces*faces;
        VertexData vertices[nb_faces * 4];

        float x = 0, y = 0;
        float max_h = 5;
        int V = 0, iterator = 0;
        while(iterator < nb_faces) {
        float z = 0;
            //Départ
            if(x == 0 && y == 0) {

                float z = generateRand(max_h);

                if (z < 2)
                {
                    vertices[V + 0] = {QVector3D(x    ,y    , z), QVector2D(0.0f, 0.0f)};
                    vertices[V + 1] = {QVector3D(x + 1,y    , z), QVector2D(0.5f, 0.0f)};
                    vertices[V + 2] = {QVector3D(x    ,y + 1, z), QVector2D(0.0f, 1.f)};
                    vertices[V + 3] = {QVector3D(x + 1,y + 1, z), QVector2D(0.5f, 1.f)};
                }

                else
                {
                    vertices[V + 0] = {QVector3D(x    ,y    , z), QVector2D(1.0f, 1.0f)};
                    vertices[V + 1] = {QVector3D(x + 1,y    , z), QVector2D(1.5f, 1.0f)};
                    vertices[V + 2] = {QVector3D(x    ,y + 1, z), QVector2D(1.0f, 2.f)};
                    vertices[V + 3] = {QVector3D(x + 1,y + 1, z), QVector2D(1.5f, 2.f)};
                }
            }

            else if (x != 0 && y == 0) {

                float z = generateRand(max_h);

                if (z < 2)
                {
                    vertices[V + 0] = {QVector3D(vertices[V - 3].position), QVector2D(0.0f, 0.0f)};
                    vertices[V + 1] = {QVector3D(x + 1,y    , z), QVector2D(0.5f, 0.0f)};
                    vertices[V + 2] = {QVector3D(vertices[V - 1].position), QVector2D(0.0f, 1.f)};
                    vertices[V + 3] = {QVector3D(x + 1,y + 1, z), QVector2D(0.5f, 1.f)};
                }

                else
                {
                    vertices[V + 0] = {QVector3D(vertices[V - 3].position), QVector2D(1.0f, 1.0f)};
                    vertices[V + 1] = {QVector3D(x + 1,y    , z), QVector2D(1.5f, 1.0f)};
                    vertices[V + 2] = {QVector3D(vertices[V - 1].position), QVector2D(1.0f, 2.f)};
                    vertices[V + 3] = {QVector3D(x + 1,y + 1, z), QVector2D(1.5f, 2.f)};
                }
            }

            else if (x == 0 && y != 0) {

                float z = generateRand(max_h);

                if (z < 2)
                {
                    vertices[V + 0] = {QVector3D(vertices[( (int) y - 1) * faces * 4 + 2 + 4 * (int) x].position), QVector2D(0.0f, 0.0f)};
                    vertices[V + 1] = {QVector3D(vertices[( (int) y - 1) * faces * 4 + 3 + 4 * (int) x].position), QVector2D(0.5f, 0.0f)};
                    vertices[V + 2] = {QVector3D(x    ,y + 1, z), QVector2D(0.0f, 1.f)};
                    vertices[V + 3] = {QVector3D(x + 1,y + 1, z), QVector2D(0.5f, 1.f)};
                }

                else
                {
                    vertices[V + 0] = {QVector3D(vertices[( (int) y - 1) * faces * 4 + 2 + 4 * (int) x].position), QVector2D(1.0f, 1.0f)};
                    vertices[V + 1] = {QVector3D(vertices[( (int) y - 1) * faces * 4 + 3 + 4 * (int) x].position), QVector2D(1.5f, 1.0f)};
                    vertices[V + 2] = {QVector3D(x    ,y + 1, z), QVector2D(1.0f, 2.f)};
                    vertices[V + 3] = {QVector3D(x + 1,y + 1, z), QVector2D(1.5f, 2.f)};
                }
            }

            else {

                float z = generateRand(max_h);

                if (z < 2)
                {
                    vertices[V + 0] = {QVector3D(vertices[V - 3].position), QVector2D(0.0f, 0.0f)};
                    vertices[V + 1] = {QVector3D(vertices[( (int) y - 1) * faces * 4 + 3 + 4 * (int) x].position), QVector2D(0.5f, 0.0f)};
                    vertices[V + 2] = {QVector3D(vertices[V - 1].position), QVector2D(0.0f, 1.f)};
                    vertices[V + 3] = {QVector3D(x + 1,y + 1,z), QVector2D(0.5f, 1.f)};
                }

                else
                {
                    vertices[V + 0] = {QVector3D(vertices[V - 3].position), QVector2D(1.0f, 1.0f)};
                    vertices[V + 1] = {QVector3D(vertices[( (int) y - 1) * faces * 4 + 3 + 4 * (int) x].position), QVector2D(1.5f, 1.0f)};
                    vertices[V + 2] = {QVector3D(vertices[V - 1].position), QVector2D(1.0f, 2.f)};
                    vertices[V + 3] = {QVector3D(x + 1,y + 1,z), QVector2D(1.5f, 2.f)};
                }
            }

            x++;

            if( x == faces ) {
                x = 0; y++;
            }

            V += 4; iterator++;
        }

        // Indices for drawing cube faces using triangle strips.
        // Triangle strips can be connected by duplicating indices
        // between the strips. If connecting strips have opposite
        // vertex order then last index of the first strip and first
        // index of the second strip needs to be duplicated. If
        // connecting strips have same vertex order then only last
        // index of the first strip needs to be duplicated.

        const int taille_tableau_indice = nb_faces * 4 + nb_faces * 2 - 2; //4 * 256 + 256 * 2 - 2;
        GLushort indices[taille_tableau_indice] = {0};

        int q = 0;
        for (int i = 0 ; i < taille_tableau_indice; i += 6) {
            indices[i + 0] = q++;
            indices[i + 1] = q++;
            indices[i + 2] = q++;
            indices[i + 3] = q;

            if(i < taille_tableau_indice - 6) {
                indices[i + 4] = q++;
                indices[i + 5] = q;
            }

    }

        // Transfer vertex data to VBO 0
        arrayBuf.bind();
        arrayBuf.allocate(vertices, nb_faces * 4 * sizeof(VertexData));

        // Transfer index data to VBO 1
        indexBuf.bind();
        indexBuf.allocate(indices, taille_tableau_indice * sizeof(GLushort));
}


//! [2]
void GeometryEngine::drawCubeGeometry(QOpenGLShaderProgram *program)
{
    arrayBuf.bind();
    indexBuf.bind();

    quintptr offset = 0;

    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
    offset += sizeof(QVector3D);
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
    glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
}

void GeometryEngine::drawPlaneGeometry(QOpenGLShaderProgram *program)
{
    arrayBuf.bind();
    indexBuf.bind();

    quintptr offset = 0;

    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));
    offset += sizeof(QVector3D);
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
    glDrawElements(GL_TRIANGLE_STRIP, 1534, GL_UNSIGNED_SHORT, 0);
}

float GeometryEngine::generateRand(float max)
{
    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max));
}
