#pragma once

#include <QApplication>
#include <QAction>

#include "app/update.h"

class Graph;
class GraphProxy;
class UndoCommand;
class UndoStack;

class App : public QApplication
{
    Q_OBJECT
public:
    explicit App(int& argc, char **argv);
    ~App();

    /*
     *  Constructs a canvas and viewport window for the root graph.
     */
    void makeDefaultWindows();

    /*
     *  Helper function to get running instance.
     */
    static App* instance();

    /*
     *  Returns the path to the node directories
     *  (which varies depending on OS).
     */
    QString bundledNodePath() const;
    QString userNodePath() const;

    /*
     *  Returns the top-level graph proxy object
     */
    GraphProxy* getProxy() const { return proxy; }

    /*
     *  Global Undo and Redo operations
     */
    void undo();
    void redo();

    /*
     *  Pushes an undo command to the stack
     */
    void pushUndoStack(UndoCommand* c);

    /*
     *  Begins a multi-command undo macro with the given description
     */
    void beginUndoMacro(QString text);

    /*
     *  Ends a multi-command undo macro
     */
    void endUndoMacro();

    /*
     *  Get undo and redo actions to populate in menus
     */
    QAction* getUndoAction();
    QAction* getRedoAction();

public slots:
    /*
     *  Commands from File menu
     */
    void onNew();
    void onSave();
    void onSaveAs();
    void onOpen();
    void onQuit();

    /*
     *  Open new windows for the root graph.
     */
    void newCanvasWindow();
    void newViewportWindow();
    void newQuadWindow();

    /*
     *  Help menu
     */
    void onAbout();
    void onUpdateCheck();

protected:
    /*
     *  Loads a file specified by name
     */
    void loadFile(QString f);

    /*
     *  Overload event handling to detect drag-and-drop events
     */
    bool event(QEvent* event);

    Graph* graph;
    GraphProxy* proxy;
    UndoStack* undo_stack;

    QString filename;

    UpdateChecker update_checker;
};
