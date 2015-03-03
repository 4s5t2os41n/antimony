#include <Python.h>

#include <QPainter>
#include <QMenu>
#include <QAction>

#include "ui/canvas/inspector/inspector_menu.h"

#include "graph/datum/datums/script_datum.h"
#include "ui/util/colors.h"
#include "app/app.h"

InspectorMenuButton::InspectorMenuButton(ScriptDatum* s, QGraphicsItem* parent)
    : GraphicsButton(parent), script(s)
{
    connect(this, &GraphicsButton::pressed,
            this, &InspectorMenuButton::onPressed);
}

QRectF InspectorMenuButton::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

void InspectorMenuButton::paint(QPainter* painter,
                                const QStyleOptionGraphicsItem* option,
                                QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(Qt::NoPen);
    painter->setBrush(hover ? Colors::base05 : Colors::base04);
    painter->drawRect(2, 2, 16, 3);
    painter->drawRect(2, 8, 16, 3);
    painter->drawRect(2, 14, 16, 3);
}

void InspectorMenuButton::onPressed()
{
    Q_ASSERT(!s.isNull());
    App::instance()->newEditorWindow(script);
}
