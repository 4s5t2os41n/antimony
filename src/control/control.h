#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <QPointer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QPainter>

// Forward declarations
class Datum;
class Node;

class Control : public QObject
{
    Q_OBJECT
public:
    /** A control is a UI representation of a Node.
     *
     *  node is the target Node (or NULL in special cases)
     *  parent is a parent Control (as nested controls are allowed)
     */
    explicit Control(Node* node, QObject* parent=0);

#if 0
    /** Makes a control for the given node.
     */
    static Control* makeControlFor(Viewport* viewport, Node* n);
#endif

    /** This function is overloaded by children to return bounds.
     */
    virtual QRectF bounds(QMatrix4x4 m) const=0;

    /*
     *  Equivalent to QGraphicsObject::shape
     *  By default, returns the bounding rect
     */
    virtual QPainterPath shape(QMatrix4x4 m) const;

    /** Returns this control's relevant node.
     */
    Node* getNode() const;

    /** Gets the value of a specific datum (which must be a double).
     */
    double getValue(QString name) const;

    /** Calls deleteLater on the top-level node.
     */
    void deleteNode();

    /** This function should be defined by child nodes
     */
    virtual void paint(QMatrix4x4 m, bool highlight, QPainter* painter)=0;

    /** Called to drag the node around with the mouse.
     */
    virtual void drag(QVector3D center, QVector3D delta)=0;

signals:
    void redraw();

protected:
    /** Mark a set of datums as causing a re-render when changed.
     */
    void watchDatums(QVector<QString> datums);

    /** Attempts to drag a particular datum's value.
     */
    void dragValue(QString name, double delta);

    /** Sets a specific datum's value to the given value.
     */
    void setValue(QString name, double new_value);

    /** Returns the color to be used by the default pen.
     */
    virtual QColor defaultPenColor() const;

    /** Returns the color to be used by the default brush.
     */
    virtual QColor defaultBrushColor() const;

    /** Sets the painter pen to a reasonable default value.
     */
    void setDefaultPen(bool highlight, QPainter* painter) const;

    /** Sets the painter brush to a reasonable value.
     */
    void setDefaultBrush(bool highlight, QPainter* painter) const;

    QPointer<Node> node;
};


#endif // CONTROL_H
