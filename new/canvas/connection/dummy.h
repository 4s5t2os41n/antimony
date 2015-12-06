#pragma once

#include <QGraphicsObject>

#include "canvas/connection/base.h"

class OutputPort;
class CanvasScene;

class DummyConnection : public BaseConnection
{
    Q_OBJECT
public:
    explicit DummyConnection(OutputPort* source, CanvasScene* scene);

    QPointF startPos() const override;
    QPointF endPos() const override;

    /*
     *  Set the ending position of the connection, update state, and redraw
     */
    void setDragPos(QPointF p);

protected:
    /*
     *  On mouse move, update the end position and call setDragPos
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    /*
     *  On mouse release, construct a real Connection if we're on a valid port
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    /*
     *  Updates has_snap_pos and snap_pos based on the nearest port
     */
    void updateSnap();

    /*
     *  Catch spacebar and snap to nearest port when it is pressed
     */
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    OutputPort* source;
    QPointF drag_pos;
    QPointF snap_pos;

    /*
     *  snapping is true if we are trying to snap to an input port
     *  has_snap_pos is true when we have actually found a target to snap to
     */
    bool snapping;
    bool has_snap_pos;
};
