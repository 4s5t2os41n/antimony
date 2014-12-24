#include <Python.h>

#include "graph/node/manager.h"
#include "graph/node/node.h"

#include "graph/node/nodes/2d.h"

#include "graph/datum/datums/shape_input_datum.h"
#include "graph/datum/datums/name_datum.h"
#include "graph/datum/datums/float_datum.h"
#include "graph/datum/datums/shape_function_datum.h"
#include "graph/datum/datums/string_datum.h"

////////////////////////////////////////////////////////////////////////////////

Node* RotateXNode(float x, float y, float z, float scale, QObject* parent=0)
{
    Q_UNUSED(scale);

    Node* n = new Node(
            NodeType::ROTATEX, NodeManager::manager()->getName("r"), parent);
    new FloatDatum("_x", QString::number(x), n);
    new FloatDatum("y", QString::number(y), n);
    new FloatDatum("z", QString::number(z), n);
    new FloatDatum("a", QString::number(45), n);
    new ShapeInputDatum("input", n);
    new ShapeFunctionDatum("shape", n, "rotate_x",
            {"input", "a", "y", "z"});
    return n;
}

////////////////////////////////////////////////////////////////////////////////

Node* RotateYNode(float x, float y, float z, float scale, QObject* parent=0)
{
    Q_UNUSED(scale);

    Node* n = new Node(
            NodeType::ROTATEY, NodeManager::manager()->getName("r"), parent);
    new FloatDatum("x", QString::number(x), n);
    new FloatDatum("_y", QString::number(y), n);
    new FloatDatum("z", QString::number(z), n);
    new FloatDatum("a", QString::number(45), n);
    new ShapeInputDatum("input", n);
    new ShapeFunctionDatum("shape", n, "rotate_y",
            {"input", "a", "x", "z"});
    return n;
}

////////////////////////////////////////////////////////////////////////////////

Node* RotateZNode(float x, float y, float z, float scale, QObject* parent=0)
{
    Q_UNUSED(scale);

    Node* n = new Node(
            NodeType::ROTATEZ, NodeManager::manager()->getName("r"), parent);
    new FloatDatum("x", QString::number(x), n);
    new FloatDatum("y", QString::number(y), n);
    new FloatDatum("_z", QString::number(z), n);
    new FloatDatum("a", QString::number(45), n);
    new ShapeInputDatum("input", n);
    new ShapeFunctionDatum("shape", n, "rotate_z",
            {"input", "a", "x", "y"});
    return n;
}

////////////////////////////////////////////////////////////////////////////////

Node* ReflectXNode(float x, float y, float z, float scale, QObject* parent=0)
{
    Q_UNUSED(scale);

    Node* n = new Node(
            NodeType::REFLECTX, NodeManager::manager()->getName("r"), parent);
    new FloatDatum("x", QString::number(x), n);
    new FloatDatum("_y", QString::number(y), n);
    new FloatDatum("_z", QString::number(z), n);
    new ShapeInputDatum("input", n);
    new ShapeFunctionDatum("shape", n, "reflect_x",
            {"input", "x"});
    return n;
}

////////////////////////////////////////////////////////////////////////////////

Node* ReflectYNode(float x, float y, float z, float scale, QObject* parent)
{
    Q_UNUSED(scale);

    Node* n = new Node(
            NodeType::REFLECTY, NodeManager::manager()->getName("r"), parent);
    new FloatDatum("_x", QString::number(x), n);
    new FloatDatum("y", QString::number(y), n);
    new FloatDatum("_z", QString::number(z), n);
    new ShapeInputDatum("input", n);
    new ShapeFunctionDatum("shape", n, "reflect_y",
            {"input", "y"});
    return n;
}

////////////////////////////////////////////////////////////////////////////////

Node* ReflectZNode(float x, float y, float z, float scale, QObject* parent=0)
{
    Q_UNUSED(scale);

    Node* n = new Node(
            NodeType::REFLECTZ, NodeManager::manager()->getName("r"), parent);
    new FloatDatum("_x", QString::number(x), n);
    new FloatDatum("_y", QString::number(y), n);
    new FloatDatum("z", QString::number(z), n);
    new ShapeInputDatum("input", n);
    new ShapeFunctionDatum("shape", n, "reflect_z",
            {"input", "z"});
    return n;
}

////////////////////////////////////////////////////////////////////////////////

Node* RecenterNode(float x, float y, float z, float scale, QObject* parent=0)
{
    Q_UNUSED(scale);

    Node* n = new Node(
            NodeType::RECENTER, NodeManager::manager()->getName("r"), parent);
    new FloatDatum("x", QString::number(x), n);
    new FloatDatum("y", QString::number(y), n);
    new FloatDatum("z", QString::number(z), n);
    new ShapeInputDatum("input", n);
    new ShapeFunctionDatum("shape", n, "recenter",
            {"input", "x", "y", "z"});
    return n;
}


Node* TranslateNode(float x, float y, float z, float scale, QObject* parent=0)
{
    Node* n = new Node(
            NodeType::TRANSLATE, NodeManager::manager()->getName("t"), parent);
    new FloatDatum("_x", QString::number(x), n);
    new FloatDatum("_y", QString::number(y), n);
    new FloatDatum("_z", QString::number(z), n);
    new FloatDatum("dx", QString::number(scale), n);
    new FloatDatum("dy", QString::number(scale), n);
    new FloatDatum("dz", QString::number(scale), n);
    new ShapeInputDatum("input", n);
    new ShapeFunctionDatum("shape", n, "translate",
            {"input", "dx", "dy", "dz"});

    return n;
}

