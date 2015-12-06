#include <Python.h>

#include "graph/proxy/node.h"
#include "graph/proxy/graph.h"
#include "graph/proxy/datum.h"
#include "graph/proxy/script.h"
#include "graph/proxy/util.h"

#include "canvas/inspector/frame.h"
#include "canvas/inspector/buttons.h"
#include "canvas/scene.h"

#include "graph/node.h"
#include "graph/graph_node.h"
#include "graph/script_node.h"

NodeProxy::NodeProxy(Node* n, GraphProxy* parent)
    : QObject(parent), node(n), script(NULL), subgraph(NULL),
      inspector(new InspectorFrame(n, parent->canvasScene())),
      show_hidden(new InspectorShowHiddenButton(inspector))
{
    if (auto graph_node = dynamic_cast<GraphNode*>(n))
        subgraph = new GraphProxy(graph_node->getGraph(), this);
    else if (auto script_node = dynamic_cast<ScriptNode*>(n))
        script = new ScriptProxy(script_node->getScriptPointer(), this);

    n->installWatcher(this);
    NULL_ON_DESTROYED(inspector);
    NULL_ON_DESTROYED(show_hidden);
}

NodeProxy::~NodeProxy()
{
    if (inspector)
        delete inspector;
    if (show_hidden)
        delete show_hidden;
}

void NodeProxy::trigger(const NodeState& state)
{
    updateHash(state.datums, &datums, this);

    bool has_hidden = false;
    {   // Set indices of datums for proper sorting
        int i=0;
        for (auto d : state.datums)
        {
            datums[d]->setIndex(i++);
            has_hidden |= (d->getName().find("_") == 0 &&
                           d->getName().find("__") != 0);
        }
    }

    // Update inspector
    inspector->setNameValid(state.name_valid);

    if (has_hidden != show_hidden->isVisible())
        show_hidden->setVisible(has_hidden);

    inspector->redoLayout();
}

////////////////////////////////////////////////////////////////////////////////

CanvasInfo NodeProxy::canvasInfo() const
{
    CanvasInfo out;
    out.inspector[node] = inspector->pos();

    if (subgraph)
        out.unite(subgraph->canvasInfo());

    return out;
}

void NodeProxy::setPositions(const CanvasInfo& info)
{
    if (info.inspector.contains(node))
        inspector->setPos(info.inspector[node]);

    if (subgraph)
        subgraph->setPositions(info);
}

////////////////////////////////////////////////////////////////////////////////

void NodeProxy::clearExportWorker()
{
    inspector->clearExportWorker();
}

void NodeProxy::setExportWorker(ExportWorker* worker)
{
    inspector->setExportWorker(worker);
}

