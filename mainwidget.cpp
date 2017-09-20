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

#include "mainwidget.h"

#include <math.h>

#include "cameracontroller.h"


MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    m_timer(),
    m_shaderProgram(),
    m_geometries(nullptr),
    m_texture(nullptr),
    m_eyeVec(5, 5, 5),
    m_targetVec(0, 0.2, 0),
    m_upVec(0, 0, 1),
    m_projectionMatrix(),
    m_cameraController(new CameraController(this))
{
    installEventFilter(m_cameraController);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete m_texture;
    delete m_geometries;
    doneCurrent();
}

void MainWidget::timerEvent(QTimerEvent *)
{
    update();
}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    m_geometries = new GeometryEngine;

    // Use QBasicTimer because its faster than QTimer
    m_timer.start(12, this);
}

void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/geom_textured.vert"))
        close();

    // Compile fragment shader
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/geom_textured.frag"))
        close();

    // Link shader pipeline
    if (!m_shaderProgram.link())
        close();

    // Bind shader pipeline for use
    if (!m_shaderProgram.bind())
        close();
}

void MainWidget::initTextures()
{
    // Load cube.png image
    m_texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());

    // Set nearest filtering mode for texture minification
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 100.0, fov = 45.0;

    // Reset projection
    m_projectionMatrix.setToIdentity();

    // Set perspective projection
    m_projectionMatrix.perspective(fov, aspect, zNear, zFar);
}

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_texture->bind();

    // Calculate view transformation
    m_cameraController->updateViewMatrix(m_eyeVec, m_targetVec, m_upVec);

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_eyeVec, m_targetVec, m_upVec);

    // Set modelview-projection matrix
    m_shaderProgram.setUniformValue("mvp_matrix", m_projectionMatrix * m_viewMatrix);

    // Use texture unit 0 which contains cube.png
    m_shaderProgram.setUniformValue("texture", 0);

    // Draw cube geometry
    m_geometries->drawCubeGeometry(&m_shaderProgram);
}
