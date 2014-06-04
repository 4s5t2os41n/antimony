#include <Python.h>
#include "datum/float.h"

FloatDatum::FloatDatum(QString name, QString expr, QObject *parent)
    : EvalDatum(name, parent)
{
    setExpr(expr);
}

bool FloatDatum::validate(PyObject *v) const
{
    return PyFloat_CheckExact(v);
}
