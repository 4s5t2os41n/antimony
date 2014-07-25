#ifndef NODE_H
#define NODE_H

#include <Python.h>
#include <QObject>

#include "node/node_types.h"

class Control;
class Datum;
class Canvas;

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject* parent=NULL);
    explicit Node(QString name, QObject* parent=NULL);

    /** Returns a Python proxy that calls getDatum when getattr is called */
    PyObject* proxy();

    /** Looks up a particular Datum by name, return NULL otherwise. */
    Datum* getDatum(QString name);

    /** Returns the NodeType, which is set as a templated parameter
     *  in a virtual function override.
     */
    virtual NodeType::NodeType getType() const=0;

    /** getDatum plus a dynamic cast.
     */
    template <class T>
    T* getDatum(QString name)
    {
        return dynamic_cast<T*>(getDatum(name));
    }

protected:
    Control* control;
};

// All nodes should be drived from _Node, so that they are forced to
// have an associated NodeType.  This prevents edge cases from slipping
// through the cracks in serialization.

template <NodeType::NodeType T>
class _Node : public Node
{
public:
    explicit _Node<T>(QString name, QObject* parent=NULL)
        : Node(name, parent) {}

    NodeType::NodeType getType() const override { return T; }
};


#endif // NODE_H
