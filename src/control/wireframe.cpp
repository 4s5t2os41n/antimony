#include "control/wireframe.h"
#include "ui/util/colors.h"

ControlWireframe::ControlWireframe(Node* node)
    : Control(node), t(10), color(Colors::blue)
{
    // Nothing to do here
}

void ControlWireframe::update(QVector<QVector3D> pts_, float t_, QColor color_)
{
    bool changed = (pts != pts_) || (t != t_) || (color != color_);

    pts = pts_;
    t = t_;
    color = color_;

    if (changed)
        emit(redraw());
}

QPainterPath ControlWireframe::shape(QMatrix4x4 m) const
{
    QPainterPathStroker s;
    s.setWidth(20);
    return s.createStroke(path(m));
}

QPainterPath ControlWireframe::path(QMatrix4x4 m) const
{
    QPainterPath out;
    if (!pts.isEmpty())
        out.moveTo((m * pts[0]).toPointF());
    for (auto p : pts)
        out.lineTo((m * p).toPointF());
    return out;
}

void ControlWireframe::paint(QMatrix4x4 m, bool highlight, QPainter* painter)
{
    if (glow)
    {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QColor(255, 255, 255, Colors::base02.red()), 20));
        painter->drawPath(path(m));
    }

    painter->setPen(QPen(highlight ? Colors::highlight(color) : color, t));
    painter->drawPath(path(m));
}
