#include <Python.h>

#include <QPainter>

#include "app/colors.h"
#include "canvas/datum_port.h"
#include "canvas/inspector/row.h"

DatumPort::DatumPort(Datum* d, InspectorRow* parent)
    : QGraphicsObject(parent), datum(d), hover(false),
      color(Colors::getColor(d))
{
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsScenePositionChanges);
}

QVariant DatumPort::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemScenePositionHasChanged)
        emit(moved());
    else if (change == ItemVisibleHasChanged)
        emit(hiddenChanged());
    return value;
}

QRectF DatumPort::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

void DatumPort::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(hover ? Colors::highlight(color) : color);
    painter->setPen(Qt::NoPen);
    painter->drawRect(portRect());
}

////////////////////////////////////////////////////////////////////////////////

InputPort::InputPort(Datum* d, InspectorRow* parent)
    : DatumPort(d, parent)
{
    // Nothing to do here
}

QRectF InputPort::portRect() const
{
    return QRectF(0, 5, 10, 10);
}

////////////////////////////////////////////////////////////////////////////////

OutputPort::OutputPort(Datum* d, InspectorRow* parent)
    : DatumPort(d, parent)
{
    // Nothing to do here
}

QRectF OutputPort::portRect() const
{
    return QRectF(10, 5, 10, 10);
}

void OutputPort::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /* XXX
    if (event->button() == Qt::LeftButton)
    {
        auto g = static_cast<GraphScene*>(scene());
        Connection* c = g->makeLinkFrom(datum);
        c->setDragPos(mapToScene(event->pos()));
        c->grabMouse();
        c->setFocus();

        // Turn off the hover highlighting.
        hover = false;
        update();
    }
    else
    {
        event->ignore();
    }
    */
}

void OutputPort::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    hover = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void OutputPort::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    hover = false;
    QGraphicsItem::hoverLeaveEvent(event);
}
