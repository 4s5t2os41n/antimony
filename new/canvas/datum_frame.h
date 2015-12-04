#pragma once

#include <QGraphicsObject>

#include "graph/watchers.h"

class Datum;
class DatumRow;

class DatumFrame : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit DatumFrame(Datum* d, QGraphicsScene* scene);

    /*
     *  Overloaded QGraphicsObject functions
     */
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    /*
     *  Update port visibility and editor state
     */
    void update(const DatumState& state);

public slots:
    /*
     *  Update layout of text labels and fields
     */
    void redoLayout();

protected:
    /*
     *  On mouse move, fake the left button being held down.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    /*
     *  On hover, set glow to true.
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    DatumRow* datum_row;

    // Ugly hack because simply grabbing the mouse doesn't set up all of the
    // magic that QGraphicsScene uses to drag items: upon first insertion,
    // set this flag to true (then overload mouseMoveEvent to work correctly)
    bool dragging;

    static const float PADDING;
};
