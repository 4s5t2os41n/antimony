#include "app/undo/undo_add_link.h"
#include "app/undo/stack.h"

UndoAddLinkCommand::UndoAddLinkCommand(GraphScene* g, Link* link)
    : UndoDeleteLinkCommand(g, link), first(true)
{
    setText("'add link'");
}

void UndoAddLinkCommand::redo()
{
    // The first time redo is called, we've just finished
    // dragging a connection into place so all of the UI
    // is already in place.
    if (first)
        first = false;
    else
        UndoDeleteLinkCommand::undo();
}

void UndoAddLinkCommand::undo()
{
    UndoDeleteLinkCommand::redo();
}
