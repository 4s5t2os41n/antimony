#include <Python.h>

#include "graph/node.h"
#include "graph/graph.h"
#include "graph/datum.h"
#include "graph/proxy.h"

Node::Node(std::string n, Graph* root)
    : name(n), uid(root->install(this)), parent(root)
{
    // Nothing to do here
}

uint32_t Node::install(Datum* d)
{
    return Root::install<Datum>(d, &datums);
}

PyObject* Node::proxyDict(Downstream* caller)
{
    return parent->proxyDict(this, caller);
}

void Node::makeInput(std::string n, PyTypeObject* type, std::string value)
{
    /*
    if (!isNameValid(name))
        throw "Name is not valid!";
        //throw IOHooks::Exception("Name is not valid.");
        */

    auto d = getByName(n, datums);
    // If the datum is of the wrong type, delete it.
    if (d != NULL && (d->type != type))
    {
        datums.remove_if([&](const std::unique_ptr<Datum>& d_)
                         { return d_.get() == d; });
        d = NULL;
    }

    if (d == NULL)
        d = new Datum(n, value, type, this);
    else
        // Move the existing datum to the end of the list
        // (so that ordering matches ordering in the script)
        for (auto itr = datums.begin(); itr != datums.end(); ++itr)
            if (itr->get() == d)
            {
                datums.splice(datums.end(), datums, itr);
                break;
            }

    script.active.insert(d);
}

/*
void Node::makeOutput(std::string n, PyObject* out)
{
    if (!Name::check(n, this))
        return;

    // If the datum is missing, of the wrong type, or is an input datum
    // (with input and expr), delete it and start over.
    auto d = getDatum(n);
    if (d != NULL && (d->value.type != out->ob_type ||
                      d->expr || d->input))
    {
        delete d;
        datums.remove(d);
        d = NULL;
    }

    if (d == NULL)
        d = Datum::OutputDatum(n, out, this);

    script.active.insert(d);
}
*/

PyObject* Node::pyGetAttr(std::string n, Downstream* caller) const
{
    auto d = get(n, datums);
    if (d)
    {
        // If the caller is a datum as well, check for recursive lookups.
        if (auto datum = dynamic_cast<Datum*>(caller))
        {
            datum->sources.insert(d->sources.begin(), d->sources.end());
            if (d->sources.find(datum) != d->sources.end())
            {
                throw Proxy::Exception("Recursive lookup of datum '" + n + "'");
            }
        }

        if (d->valid)
        {
            Py_INCREF(d->value);
            return d->value;
        }
        throw Proxy::Exception("Datum '" + n + "' is invalid");
    }
    else
    {
        return NULL;
    }
}
