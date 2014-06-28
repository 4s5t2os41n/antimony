#include <Python.h>

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "test_datum.h"
#include "datum/float_datum.h"
#include "datum/name_datum.h"

void TestDatum::FloatValid()
{
    FloatDatum* d = new FloatDatum("x","12.3");
    QVERIFY(d->getValid() == true);
    d->setExpr("1");
    QVERIFY(d->getValid() == true);
    delete d;
}

void TestDatum::FloatInvalid()
{
    FloatDatum* d = new FloatDatum("x","12.3!");
    QVERIFY(d->getValid() == false);
    delete d;
}

void TestDatum::FloatValidateFail()
{
    FloatDatum* d = new FloatDatum("x","'this is not a float'");
    QVERIFY(d->getValid() == false);
    delete d;
}

void TestDatum::FloatValidToInvalid()
{
    FloatDatum* d = new FloatDatum("x","12.3");
    QSignalSpy s(d, SIGNAL(changed()));
    d->setExpr("10.0!");
    QVERIFY(d->getValid() == false);
    QVERIFY(s.count() == 1);
    delete d;
}

void TestDatum::FloatInvalidToValid()
{
    FloatDatum* d = new FloatDatum("x","12.3!");
    QSignalSpy s(d, SIGNAL(changed()));
    d->setExpr("10.0");
    QVERIFY(d->getValid() == true);
    QVERIFY(s.count() == 1);
    delete d;
}

void TestDatum::FloatSetSame()
{
    FloatDatum* d = new FloatDatum("x","12.3");
    QSignalSpy s(d, SIGNAL(changed()));
    d->setExpr("12.3");
    QVERIFY(d->getValid() == true);
    QVERIFY(s.count() == 0);
    delete d;
}

void TestDatum::SingleInputAccepts()
{
    FloatDatum* a = new FloatDatum("a", "10.1");
    FloatDatum* b = new FloatDatum("b", "10.1");
    QVERIFY(a->acceptsLink(b->linkFrom()) == true);
    delete a;
    delete b;
}

void TestDatum::SingleInputHasValue()
{
    FloatDatum* a = new FloatDatum("a", "1");
    FloatDatum* b = new FloatDatum("b", "2!");
    QVERIFY(a->hasInputValue() == false);
    a->addLink(b->linkFrom());
    QVERIFY(a->hasInputValue() == true);
    delete a;
    delete b;
}

void TestDatum::SingleInputLink()
{
    FloatDatum* a = new FloatDatum("a", "1");
    FloatDatum* b = new FloatDatum("b", "2!");

    QSignalSpy s(a, SIGNAL(changed()));
    a->addLink(b->linkFrom());

    QVERIFY(a->getValid() == false);
    QVERIFY(s.count() == 1);
    delete a;
    delete b;
}

void TestDatum::RecursiveLink()
{
    FloatDatum* a = new FloatDatum("a", "1");
    FloatDatum* b = new FloatDatum("a", "1");
    a->addLink(b->linkFrom());

    Q_ASSERT(b->acceptsLink(a->linkFrom()) == false);

    delete a;
    delete b;
}

void TestDatum::SingleInputLinkDelete()
{
    FloatDatum* a = new FloatDatum("a", "1");
    FloatDatum* b = new FloatDatum("b", "2!");
    a->addLink(b->linkFrom());

    QSignalSpy s(a, SIGNAL(changed()));
    delete b;

    QVERIFY(a->hasInputValue() == false);
    QVERIFY(s.count() == 1);

    b = new FloatDatum("b", "2!");
    a->addLink(b->linkFrom());

    QVERIFY(a->hasInputValue() == true);
    QVERIFY(s.count() == 2);

    a->deleteLink(b);
    QVERIFY(a->hasInputValue() == false);
    QVERIFY(s.count() == 3);

    delete a;
    delete b;
}

void TestDatum::NameValidate()
{
    NameDatum* d;

    d = new NameDatum("a", "hello");
    QVERIFY(d->getValid() == true);
    delete d;

    d = new NameDatum("a", "with");
    QVERIFY(d->getValid() == false);
    delete d;

    d = new NameDatum("a", "  omgwtf  ");
    QVERIFY(d->getValid() == true);
    delete d;

    d = new NameDatum("a", "hi!");
    QVERIFY(d->getValid() == false);
    delete d;

    d = new NameDatum("a", "12a");
    QVERIFY(d->getValid() == false);
    delete d;

    d = new NameDatum("a", "a12");
    QVERIFY(d->getValid() == true);
    delete d;

    d = new NameDatum("a", "a'12");
    QVERIFY(d->getValid() == false);
    delete d;
}
