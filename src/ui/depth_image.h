#ifndef DEPTH_IMAGE_H
#define DEPTH_IMAGE_H

#include <QGraphicsObject>
#include <QImage>
#include <QVector3D>
#include <QtOpenGL/QGLFunctions>
#include <QtOpenGL/QGLShaderProgram>
#include <QtOpenGL/QGLBuffer>

class Canvas;

class DepthImageItem : public QGraphicsObject, protected QGLFunctions
{
    Q_OBJECT
public:
    DepthImageItem(QVector3D pos, QVector3D size,
                   QImage depth, QImage shaded, Canvas* canvas);
    ~DepthImageItem();
    QRectF boundingRect() const;

    /** Position of lower-left corner (in original scene units) */
    QVector3D pos;

    /** Scale (in rotated scene coordinates) */
    QVector3D size;

public slots:
    void reposition();

protected:
    void initializeGL();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QImage depth;
    QImage shaded;
    Canvas* canvas;

    GLuint depth_tex;
    GLuint shaded_tex;

    static QGLShaderProgram shader;
    static QGLBuffer vertices;
};

#endif // DEPTH_IMAGE_H
