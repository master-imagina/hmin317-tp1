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

#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(0)
{
    scanX = -8.0;
    scanY = -8.0;
    scanZ = -21.0;

    for (int i=0;i<10;i++)
    {
        imput[i] = false;
    }
}

MainWidget::~MainWidget()
{
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();

    if (key == Qt::Key_4) {e->accept(); imput[0] = true;}
    if (key == Qt::Key_6) {e->accept(); imput[1] = true;}
    if (key == Qt::Key_8) {e->accept(); imput[2] = true;}
    if (key == Qt::Key_2) {e->accept(); imput[3] = true;}

    if (key == Qt::Key_1) {e->accept(); imput[6] = true;}
    if (key == Qt::Key_3) {e->accept(); imput[7] = true;}
    if (key == Qt::Key_7) {e->accept(); imput[8] = true;}
    if (key == Qt::Key_9) {e->accept(); imput[9] = true;}

    if (key == Qt::Key_Plus) {e->accept(); imput[4] = true;}
    if (key == Qt::Key_Minus) {e->accept(); imput[5] = true;}

    update();
}

void MainWidget::keyReleaseEvent(QKeyEvent *e)
{
    int key = e->key();

    if (key == Qt::Key_4) {e->accept(); imput[0] = false;}
    if (key == Qt::Key_6) {e->accept(); imput[1] = false;}
    if (key == Qt::Key_8) {e->accept(); imput[2] = false;}
    if (key == Qt::Key_2) {e->accept(); imput[3] = false;}

    if (key == Qt::Key_1) {e->accept(); imput[6] = false;}
    if (key == Qt::Key_3) {e->accept(); imput[7] = false;}
    if (key == Qt::Key_7) {e->accept(); imput[8] = false;}
    if (key == Qt::Key_9) {e->accept(); imput[9] = false;}

    if (key == Qt::Key_Plus) {e->accept(); imput[4] = false;}
    if (key == Qt::Key_Minus) {e->accept(); imput[5] = false;}

    update();
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
    qreal acc = diff.length() / 100.0;
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    angularSpeed += acc;
}

void MainWidget::timerEvent(QTimerEvent *)
{

    angularSpeed *= 0.99;
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    }
    else { rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation; }

    if (imput[0]) scanX += 0.1;
    if (imput[1]) scanX -= 0.1;

    if (imput[2]) scanY -= 0.1;
    if (imput[3]) scanY += 0.1;

    if (imput[4]) scanZ += 0.1;
    if (imput[5]) scanZ -= 0.1;

    if (imput[6]) {scanX += 0.1; scanY += 0.1;}
    if (imput[7]) {scanX -= 0.1; scanY += 0.1;}
    if (imput[8]) {scanX += 0.1; scanY -= 0.1;}
    if (imput[9]) {scanX -= 0.1; scanY -= 0.1;}

    cout << scanX << " " << scanY << " " << scanZ << endl;

    update();
}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    geometries = new GeometryEngine;

    timer.start(12, this);
}

void MainWidget::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();
    if (!program.link())
        close();
    if (!program.bind())
        close();
}

void MainWidget::initTextures()
{
    texture = new QOpenGLTexture(QImage(":/clay.png").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);

    texture2 = new QOpenGLTexture(QImage(":/snow.png").mirrored());
    texture2->setMinificationFilter(QOpenGLTexture::Nearest);
    texture2->setMagnificationFilter(QOpenGLTexture::Linear);
    texture2->setWrapMode(QOpenGLTexture::Repeat);
}

void MainWidget::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 3.0, zFar = 100, fov = 45.0;
    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}

void MainWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind(0);
    texture2->bind(1);

    QMatrix4x4 matrix;
    matrix.translate(scanX, scanY, scanZ);

    program.setUniformValue("mvp_matrix", projection * matrix);
    program.setUniformValue("texture", 0);
    program.setUniformValue("texture2", 0);

    geometries->drawPlaneGeometry(&program);
}
