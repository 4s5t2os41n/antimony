#include <Python.h>

#include "datum/datum.h"
#include "node/node.h"
#include "node/manager.h"
#include "node/proxy.h"

Node::Node(QObject* parent) :
    QObject(parent)
{
    if (parent == NULL)
    {
        setParent(NodeManager::manager());
    }
}


PyObject* Node::proxy()
{
    PyObject* p = PyObject_CallObject(proxyType(), NULL);
    ((proxy_ProxyObject*)p)->node = this;
    return p;
}

Datum* Node::getDatum(QString name)
{
    return findChild<Datum*>(name);
}
