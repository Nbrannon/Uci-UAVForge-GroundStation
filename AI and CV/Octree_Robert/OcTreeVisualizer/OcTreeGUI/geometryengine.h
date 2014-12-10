#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QGLFunctions>
#include <QGLShaderProgram>

class GeometryEngine : protected QGLFunctions
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    void init();
    void drawCubeGeometry(QGLShaderProgram *program);

private:
    void initCubeGeometry();

    GLuint vboIds[2];
};


#endif // GEOMETRYENGINE_H
