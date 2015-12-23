#include <Python.h>

#include "graph/proxy/graph.h"
#include "graph/proxy/datum.h"
#include "graph/proxy/node.h"
#include "graph/proxy/util.h"

#include "graph/datum.h"
#include "canvas/datum_row.h"
#include "canvas/connection/connection.h"

DatumProxy::DatumProxy(Datum* d, NodeProxy* parent)
    : BaseDatumProxy(d, parent), row(new DatumRow(d, parent->getInspector()))
{
    d->installWatcher(this);
    NULL_ON_DESTROYED(row);
}

DatumProxy::~DatumProxy()
{
    if (row)
        delete row;

    // I don't understand why this is necessary, but doing the deletions
    // directly from the QHash leads to a reproducible crash.
    QList<Connection*> cs;
    for (Connection* c : connections)
        cs.push_back(c);

    for (auto c : cs)
        delete c;
}

void DatumProxy::trigger(const DatumState& state)
{
    row->update(state);

    if (state.sigil == Datum::SIGIL_NONE ||
        state.sigil == Datum::SIGIL_CONNECTION)
    {
        updateHash(state.links, &connections, this, true);
    }
}

void DatumProxy::setIndex(int i)
{
    row->setIndex(i);
}

////////////////////////////////////////////////////////////////////////////////

InputPort* DatumProxy::inputPort() const
{
    return row->inputPort();
}

OutputPort* DatumProxy::outputPort() const
{
    return row->outputPort();
}

GraphProxy* DatumProxy::graphProxy() const
{
    return static_cast<GraphProxy*>(parent()->parent());
}
