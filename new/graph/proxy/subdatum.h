#pragma once

#include <QObject>
#include <QPointer>

#include "graph/proxy/base_datum.h"
#include "canvas/info.h"

class GraphProxy;
class SubdatumFrame;

class SubdatumProxy : public BaseDatumProxy
{
Q_OBJECT

public:
    SubdatumProxy(Datum* d, GraphProxy* parent);
    ~SubdatumProxy();

    void trigger(const DatumState& state) override;

    /*
     *  Returns the position of the subdatum frame
     */
    CanvasInfo canvasInfo() const;

    /*
     * Sets the position of the subdatum frame
     */
    void setPositions(const CanvasInfo& info, bool select=false);

    /*
     *  Return input and output ports (from the associated row)
     *  (used in creating connections)
     */
    InputPort* inputPort() const override;
    OutputPort* outputPort() const override;

    /*
     *  Return the parent object (cast to a GraphProxy)
     */
    GraphProxy* graphProxy() const override;

protected:
    /*  Strong pointer to UI frame.  We use a QPointer here because otherwise
     *  subdatum frames can be deleted out from underneath us (i.e. when a
     *  window closes  */
    QPointer<SubdatumFrame> frame;
};
