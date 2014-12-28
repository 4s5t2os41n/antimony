#include <Python.h>

#include <QDebug>

#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QHelpEvent>
#include <QToolTip>

#include "graph/datum/datums/script_datum.h"

#include "ui/script/editor.h"
#include "ui/script/syntax.h"
#include "ui/util/colors.h"

#include "app/app.h"
#include "app/undo/undo_change_expr.h"


ScriptEditor::ScriptEditor(ScriptDatum* datum, QWidget* parent)
    : QPlainTextEdit(parent), datum(datum)
{
    QFont font;
    font.setFamily("Courier");
    QFontMetrics fm(font);
    setTabStopWidth(fm.width("    "));
    document()->setDefaultFont(font);

    new SyntaxHighlighter(document());
    setStyleSheet(QString(
        "QPlainTextEdit {"
        "    background-color: %1;"
        "    color: %2;"
        "}").arg(Colors::base00.name())
            .arg(Colors::base04.name()));

    QAbstractScrollArea::setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(document(), &QTextDocument::contentsChanged,
            this, &ScriptEditor::onTextChanged);
    connect(datum, &Datum::changed,
            this, &ScriptEditor::onDatumChanged);
    connect(datum, &Datum::destroyed,
            parent, &QWidget::deleteLater);

    connect(document(), &QTextDocument::undoCommandAdded,
            this, &ScriptEditor::onUndoCommandAdded);

    installEventFilter(this);

    onDatumChanged(); // update tooltip and text
}

void ScriptEditor::onTextChanged()
{
    qDebug() << "onTextChanged called";
    setToolTip("");
    QToolTip::hideText();
    if (datum)
        datum->setExpr(document()->toPlainText());
}

void ScriptEditor::onDatumChanged()
{
    if (datum)
    {
        if (!datum->getValid())
        {
            setToolTip(datum->getErrorTraceback());
            highlightError(datum->getErrorLine());
        }
        else
        {
            setToolTip("");
            QToolTip::hideText();
            setExtraSelections({});
        }

        if (datum->getExpr() != document()->toPlainText())
            document()->setPlainText(datum->getExpr());
    }
    else
    {
        setToolTip("");
        QToolTip::hideText();
    }
}

void ScriptEditor::onUndoCommandAdded()
{
    disconnect(document(), &QTextDocument::contentsChanged,
               this, &ScriptEditor::onTextChanged);

    document()->undo();
    QString before = document()->toPlainText();

    document()->redo();
    QString after = document()->toPlainText();

    App::instance()->pushStack(
            new UndoChangeExprCommand(datum, before, after));
    connect(document(), &QTextDocument::contentsChanged,
            this, &ScriptEditor::onTextChanged);
}

bool ScriptEditor::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == this && event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->matches(QKeySequence::Undo))
            App::instance()->undo();
        else if (keyEvent->matches(QKeySequence::Redo))
            App::instance()->redo();
        else
            return false;
        return true;
    }
    return false;
}


void ScriptEditor::highlightError(int lineno)
{
    QTextEdit::ExtraSelection line;

    line.format.setBackground(Colors::dim(Colors::red));
    line.format.setProperty(QTextFormat::FullWidthSelection, true);

    line.cursor = textCursor();
    line.cursor.setPosition(
            document()->findBlockByLineNumber(lineno - 1).position());
    line.cursor.clearSelection();

    setExtraSelections({line});
}
