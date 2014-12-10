#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "geometryengine.h"
#include <QGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QGLShaderProgram>

class GeometryEngine;

class GLWidget : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
	~GLWidget();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void timerEvent(QTimerEvent *e);
	
    void initializeGL();
    void paintGL();
    void resizeGL(int w,int h);
	void initShaders();
    void initTextures();

private:
    QBasicTimer timer;
    QGLShaderProgram program;
    GeometryEngine geometries;

    GLuint texture;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
	
};

#endif // GLWIDGET_H
