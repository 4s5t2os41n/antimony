#include <Python.h>

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "test_function.h"
#include "datum/function_datum.h"
#include "datum/float_datum.h"

#include "node/node.h"
#include "node/3d.h"

void TestFunction::CircleFunction()
{
    Node* p = Point3DNode("p", "0.0", "0.0", "1.0");

    ShapeFunctionDatum* d = new ShapeFunctionDatum("c", p, "circle", {"x","y","z"});
    QVERIFY(d->getValid());

    delete p;
}

void TestFunction::InvalidCircle()
{
    Node* p = Point3DNode("p", "0.0", "0.0", "q.0");

    ShapeFunctionDatum* d1 = new ShapeFunctionDatum("c1", p, "circle", {"x","y","z"});
    QVERIFY(!d1->getValid());

    ShapeFunctionDatum* d2 = new ShapeFunctionDatum("c2", p, "circle", {"x","y","q"});
    QVERIFY(!d2->getValid());

    delete p;
}

void TestFunction::ChangeInput()
{
    Node* p = Point3DNode("p", "0.0", "0.0", "1.0");

    ShapeFunctionDatum* d = new ShapeFunctionDatum("c", p, "circle", {"x","y","z"});
    QSignalSpy s(d, SIGNAL(changed()));
    p->getDatum<FloatDatum>("x")->setExpr("1.0");
    QCOMPARE(s.count(), 1);

    delete p;
}


