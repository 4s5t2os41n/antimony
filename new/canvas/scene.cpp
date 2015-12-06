#include "canvas/scene.h"
#include "canvas/view.h"

#include "app/app.h"
#include "canvas/inspector/frame.h"
#include "canvas/subdatum/subdatum_frame.h"

#include "undo/undo_move_node.h"
#include "undo/undo_move_datum.h"

CanvasScene::CanvasScene(Graph* g, QObject* parent)
    : QGraphicsScene(parent), g(g)
{
    // Nothing to do here
}

CanvasView* CanvasScene::getView(QWidget* parent)
{
    return new CanvasView(this, parent);
}

void CanvasScene::endDrag(QPointF delta)
{
    App::instance()->beginUndoMacro("'drag'");
    for (auto m : selectedItems())
        if (auto i = dynamic_cast<InspectorFrame*>(m))
            App::instance()->pushUndoStack(new UndoMoveNode(
                        i->getNode(), i->pos() - delta, i->pos()));
        else if (auto s = dynamic_cast<SubdatumFrame*>(m))
            App::instance()->pushUndoStack(new UndoMoveDatum(
                        s->getDatum(), s->pos() - delta, s->pos()));
    App::instance()->endUndoMacro();
}
