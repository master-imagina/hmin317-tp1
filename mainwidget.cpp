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

#include <QMouseEvent>
#include <iostream>

#include <math.h>


MainWidget::MainWidget(int msFramerate, QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    sand(0),
    cliff(0),
    grass(0),
    rock(0),
    cliffNormal(0),
    angularSpeed(0)
{
    mouseHaveBeenPress = false;
    time = new QTimer;
    connect(time,SIGNAL(timeout()),this,SLOT(update()));
    time->start(msFramerate);
    keyZPressed=0,keySPressed=0,keyQPressed=0,keyDPressed=0,keySpacePressed=0,keyMajPressed=0;
    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);
    dx_autoRotate = 0;
    paused = 0;
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete sand;
    delete cliff;
    delete grass;
    delete rock;
    delete geometries;
    doneCurrent();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());

    mouseHaveBeenPress = true;
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    /*QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;*/

    mouseHaveBeenPress = false;
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx = (event->x()-anchor.x());
    float dy = (event->y()-anchor.y());
    if(mouseHaveBeenPress){
        this->dx = dx;
        this->dy = dy;
    }else{
        this->dx = 0;
        this->dy = 0;
    }
    anchor = event->pos();
}

void MainWidget::wheelEvent(QWheelEvent *event)
{
    wheelDelta = event->delta()/120;
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Z){
        keyZPressed=1;
    }
    if(event->key() == Qt::Key_S){
        keySPressed=1;
    }
    if(event->key() == Qt::Key_Q){
        keyQPressed=1;
    }
    if(event->key() == Qt::Key_D){
        keyDPressed=1;
    }
    if(event->key() == Qt::Key_Space){
        keySpacePressed = 1;
    }
    if(event->key() == Qt::Key_Shift){
        keyMajPressed=1;
    }

    if(event->key() == Qt::Key_Minus){
        keyMinusPressed=1;
    }

    if(event->key() == Qt::Key_Plus){
        keyPlusPressed=1;
    }

    if(event->key() == Qt::Key_P){
        paused = (paused +1)%2;

    }

}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Z){
        keyZPressed=0;
    }
    if(event->key() == Qt::Key_S){
        keySPressed=0;
    }
    if(event->key() == Qt::Key_Q){
        keyQPressed=0;
    }
    if(event->key() == Qt::Key_D){
        keyDPressed=0;
    }
    if(event->key() == Qt::Key_Space){
        keySpacePressed = 0;
    }
    if(event->key() == Qt::Key_Shift){
        keyMajPressed = 0;
    }

    if(event->key() == Qt::Key_Minus){
        keyMinusPressed=0;
    }

    if(event->key() == Qt::Key_Plus){
        keyPlusPressed=0;
    }
}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{

    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }

}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.2, 0.55, 1.0, 1);

    initShaders();
    initTextures();
    camera.move(0,0,0,0,0,0,0,0);
//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling

//! [2]

    geometries = new GeometryEngine;
    qreal aspect = qreal(this->size().width()) / qreal(this->size().height());

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 1000.0, fov = 45.0;

    QMatrix4x4 projection;
    projection.perspective(fov, aspect, zNear, zFar);
    camera.setProjectionMatrix(projection);

    // Use QBasicTimer because its faster than QTimer


}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{

    texture = new QOpenGLTexture(QImage(":/heightMap.png").mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);



    sand = new QOpenGLTexture(QImage(":/sand.png").mirrored());

    // Set nearest filtering mode for texture minification
    sand->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    sand->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    sand->setWrapMode(QOpenGLTexture::Repeat);
    sand->setAutoMipMapGenerationEnabled(true);

    rock = new QOpenGLTexture(QImage(":/rock.png").mirrored());

    // Set nearest filtering mode for texture minification
    rock->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    rock->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    rock->setWrapMode(QOpenGLTexture::Repeat);
    rock->setAutoMipMapGenerationEnabled(true);


    cliff = new QOpenGLTexture(QImage(":/cliff.png").mirrored());

    // Set nearest filtering mode for texture minification
    cliff->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    cliff->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    cliff->setWrapMode(QOpenGLTexture::Repeat);
    cliff->setAutoMipMapGenerationEnabled(true);

    grass = new QOpenGLTexture(QImage(":/grass.png").mirrored());

    // Set nearest filtering mode for texture minification
    grass->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    grass->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    grass->setWrapMode(QOpenGLTexture::Repeat);
    grass->setAutoMipMapGenerationEnabled(true);

    cliffNormal = new QOpenGLTexture(QImage(":/Normal.png").mirrored());

    // Set nearest filtering mode for texture minification
    cliffNormal->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    cliffNormal->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    cliffNormal->setWrapMode(QOpenGLTexture::Repeat);
    cliffNormal->setAutoMipMapGenerationEnabled(true);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 1000.0, fov = 45.0;



    QMatrix4x4 projection;
    projection.perspective(fov, aspect, zNear, zFar);
    camera.setProjectionMatrix(projection);
}
//! [5]

void MainWidget::paintGL()
{
    this->makeCurrent();
    if(paused ){
        camera.move(dx,dy,wheelDelta,keyZPressed,keySPressed,keyQPressed,keyDPressed,keySpacePressed-keyMajPressed);
    }
    else{
        dx_autoRotate += 0.2f * (keyPlusPressed - keyMinusPressed);
        camera.move(dx_autoRotate,dy,wheelDelta,keyZPressed,keySPressed,keyQPressed,keyDPressed,keySpacePressed-keyMajPressed);
    }

    dx=0,dy=0;
    wheelDelta = 0;
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind(0);
    sand->bind(1);
    grass->bind(2);
    cliff->bind(3);
    rock->bind(4);
    cliffNormal->bind(5);

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, 0.0);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", camera.getProjectionMatrix()*camera.getViewMatrix()  );
//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);
    program.setUniformValue("grass", 2);
    program.setUniformValue("sand", 1);
    program.setUniformValue("rock", 4);
    program.setUniformValue("cliff",3);
    program.setUniformValue("cliffNormal",5);
    program.setUniformValue("timer", timer->currentTime().msec());

    // Draw cube geometry
    geometries->drawPlaneGeometry(&program);
}
