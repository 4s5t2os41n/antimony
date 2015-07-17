#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <Python.h>

#include <QWidget>
#include <QLineEdit>
#include <QPointer>
#include <QGraphicsObject>

#include "ui/canvas/inspector/inspector_title.h"

#include "graph/node.h"
#include "graph/watchers.h"

class Datum;
class Canvas;

class InputPort;
class OutputPort;
class InspectorRow;

class DatumTextItem;

////////////////////////////////////////////////////////////////////////////////

class NodeInspector : public QGraphicsObject, NodeWatcher
{
    Q_OBJECT
public:
    explicit NodeInspector(Node* node);

    /*
     *  Watcher callbacks
     */
    void trigger(const NodeState& state) override;

    QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    Node* getNode();

    template <typename T> T* getButton() const
    {
        return title_row->getButton<T>();
    }

    OutputPort* datumOutputPort(Datum *d) const;
    InputPort* datumInputPort(Datum* d) const;

    QPointF datumOutputPosition(Datum* d) const;
    QPointF datumInputPosition(Datum* d) const;

    bool isDatumHidden(Datum* d) const;

    void setTitle(QString title);

signals:
    void moved();
    void hiddenChanged();
    void glowChanged(Node* node, bool g);

public slots:
    /** Updates layout of text labels and fields.
     */
    void onLayoutChanged();

    /*
     *  On mouse move, fake the left button being held down.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    /*
     *  On mouse press, open a 'jump to node' menu
     */
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

    /*
     *  On mouse move, fake the left button being held down.
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    /*
     *  On hover, set glow to true.
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    /*
     *  On hover, set glow to true.
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    /** Change focus to the next text item.
     */
    void focusNext(DatumTextItem* prev);

    /** Change focus to the previous text item.
     */
    void focusPrev(DatumTextItem* prev);

    void setDragging(bool d) { dragging = d; }

    void setGlow(bool g);
    void setShowHidden(bool h);

protected:
    /*
     *  On object moved, emit moved signal.
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    /** Returns the width of the largest label.
     */
    float maxLabelWidth() const;

    Node* node;
    InspectorTitle* title_row;
    QMap<Datum*, InspectorRow*> rows;

    // Ugly hack because simply grabbing the mouse doesn't set up all of the
    // magic that QGraphicsScene uses to drag items: upon first insertion,
    // set this flag to true (then overload mouseMoveEvent to work correctly)
    bool dragging;

    // Extra padding around the inspector (for glowing effect)
    int border;

    // Boolean to determine whether to draw glow
    bool glow;

    // Boolean to determine whether to show hidden datums.
    bool show_hidden;

    friend class InspectorRow;
    friend class InspectorTitle;
};

#endif // INSPECTOR_H
