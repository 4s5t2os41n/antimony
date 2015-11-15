#pragma once

#include <QApplication>

#include "app/update.h"

class Graph;
class GraphProxy;

class App : public QApplication
{
    Q_OBJECT
public:
    explicit App(int& argc, char **argv);

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
    Graph* graph;
    GraphProxy* proxy;

    UpdateChecker update_checker;
};
