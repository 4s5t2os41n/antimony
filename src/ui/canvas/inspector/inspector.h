#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "graph/node/node.h"

#include <QWidget>
#include <QLineEdit>
#include <QPointer>
#include <QGraphicsObject>

class Datum;
class Node;
class Canvas;

class InspectorRow;
class InputPort;
class OutputPort;

class DatumTextItem;

////////////////////////////////////////////////////////////////////////////////

class NodeInspector : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit NodeInspector(Node* node);

    QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    Node* getNode();


    OutputPort* datumOutputPort(Datum *d) const;
    InputPort* datumInputPort(Datum* d) const;

    QPointF datumOutputPosition(Datum* d) const;
    QPointF datumInputPosition(Datum* d) const;

signals:
    void portPositionChanged();

public slots:
    /** Updates layout of text labels and fields.
     */
    void onLayoutChanged();

#if 0
    /** Opens the script editor for the given datum
     *  (which must be a ScriptDatum).
     */
    void openScript(Datum* d) const;
#endif

    /*
     *  On mouse move, fake the left button being held down.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    /*
     *  On mouse move, fake the left button being held down.
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    /** When datums are changed, update rows and layout.
     */
    void onDatumsChanged();

    /** Change focus to the next text item.
     */
    void focusNext(DatumTextItem* prev);

    /** Change focus to the previous text item.
     */
    void focusPrev(DatumTextItem* prev);

    void setDragging(bool d) { dragging = d; }

protected:
    /** On delete or backspace, delete node.
     */
    void keyPressEvent(QKeyEvent* event) override;

    /** Returns the width of the largest label.
     */
    float labelWidth() const;

    /** Fills in the grid from the source node.
     */
    void populateLists(Node* node);

    QPointer<Node> node;
    QMap<Datum*, InspectorRow*> rows;
    QGraphicsTextItem* title;

    // Ugly hack because simply grabbing the mouse doesn't set up all of the
    // magic that QGraphicsScene uses to drag items: upon first insertion,
    // set this flag to true (then overload mouseMoveEvent to work correctly)
    bool dragging;

    friend class InspectorRow;
};

#endif // INSPECTOR_H
