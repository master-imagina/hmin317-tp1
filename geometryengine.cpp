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

#include <cmath>

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

GeometryEngine::GeometryEngine()
    : m_indexVbo(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    m_arrayVbo.create();
    m_indexVbo.create();

    initTerrainGeometry();
}

GeometryEngine::~GeometryEngine()
{
    m_arrayVbo.destroy();
    m_indexVbo.destroy();
}

void GeometryEngine::initTerrainGeometry()
{
    // Vertices
    const std::size_t vertexCount = m_terrainSize * m_terrainSize;
    static std::vector<VertexData> vertices(vertexCount);

    for (std::size_t i = 0; i < vertices.size(); ++i) {
        const float iId = i % m_terrainSize;
        const float fl = std::floor(i / static_cast<float>(m_terrainSize));

        VertexData &vertex = vertices[i];

        vertex.position = {iId, 0.f, fl};
        vertex.texCoord = {iId / 3.f, fl / 2.f};
    }

    // Indices
    const std::size_t indexCount = 3 * 2 * std::pow(m_terrainSize - 1, 2);
    static std::vector<unsigned int> indices(indexCount);

    int indexCounter = 0;

    for (int z = 0; z < (m_terrainSize - 1); ++z) {
        for (int x = 0; x < (m_terrainSize - 1); ++x) {
            indices[indexCounter++] = z * m_terrainSize + x;
            indices[indexCounter++] = (z + 1) * m_terrainSize + x;
            indices[indexCounter++] = z * m_terrainSize + (x + 1);

            indices[indexCounter++] = (z + 1) * m_terrainSize + x;
            indices[indexCounter++] = (z + 1) * m_terrainSize + (x + 1);
            indices[indexCounter++] = z * m_terrainSize + (x + 1);
        }
    }

    // VBOs
    m_arrayVbo.bind();
    m_arrayVbo.allocate(vertices.data(), vertexCount * sizeof(VertexData));

    m_indexVbo.bind();
    m_indexVbo.allocate(indices.data(), indexCount * sizeof(unsigned int));
}

void GeometryEngine::drawTerrainGeometry(QOpenGLShaderProgram *program)
{
    m_arrayVbo.bind();
    m_indexVbo.bind();

    quintptr offset = 0;

    const int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    const int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    glDrawElements(GL_TRIANGLES, (m_terrainSize - 1) * (m_terrainSize - 1) * 6,
                   GL_UNSIGNED_INT, 0);
}
