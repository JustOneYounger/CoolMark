#include "CoolMark_Style_Editer.h"
#include "CoolMark_Style_Editer_LineNumber.h"

CoolMark_Style_Editer::CoolMark_Style_Editer(QWidget* parent)
	:QPlainTextEdit(parent)
	, coolmark_style_editer_linenumberarea(new CoolMark_Style_Editer_LineNumber(this))
{
    coolmark_style_editer_highlight = new CoolMark_Style_Editer_Highlight(document());
    setStyleSheet("background:transparent;color:#E6DB74;font-family:'Consolas';font-size:14px;border:none;");
    setTabStopDistance(fontMetrics().horizontalAdvance(" ") * 8);
    setWordWrapMode(QTextOption::NoWrap);
    //左侧行号渲染
    connect(this, &CoolMark_Style_Editer::blockCountChanged, this, &CoolMark_Style_Editer::updateLineNumberAreaWidth);
    connect(this, &CoolMark_Style_Editer::updateRequest, this, &CoolMark_Style_Editer::updateLineNumberArea);
    connect(this, &CoolMark_Style_Editer::cursorPositionChanged, this, &CoolMark_Style_Editer::highlightCurrentLine);
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

CoolMark_Style_Editer::~CoolMark_Style_Editer() {
    delete coolmark_style_editer_linenumberarea;
    delete coolmark_style_editer_highlight;
}

int CoolMark_Style_Editer::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}
void CoolMark_Style_Editer::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
void CoolMark_Style_Editer::updateLineNumberArea(const QRect& rect, int dy)
{
    if (dy)
        coolmark_style_editer_linenumberarea->scroll(0, dy);
    else
        coolmark_style_editer_linenumberarea->update(0, rect.y(), coolmark_style_editer_linenumberarea->width(), rect.height());
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
void CoolMark_Style_Editer::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    coolmark_style_editer_linenumberarea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
void CoolMark_Style_Editer::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        // QColor lineColor = QColor("#ECF5FF");
        // selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}
void CoolMark_Style_Editer::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(coolmark_style_editer_linenumberarea);
    // painter.fillRect(event->rect(), Qt::white);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    int currentLineNumber = textCursor().blockNumber();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            if (blockNumber == currentLineNumber) {
                painter.setPen(QColor("#E6DB74"));
                QFont font = painter.font();
                font.setBold(true);
                painter.setFont(font);
            }
            else {
                painter.setPen(QColor("#75715E"));
                QFont font = painter.font();
                font.setBold(false);
                painter.setFont(font);
            }
            painter.drawText(0, top, coolmark_style_editer_linenumberarea->width(), fontMetrics().height(),
                Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CoolMark_Style_Editer::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_ParenLeft || e->key() == Qt::Key_BraceLeft || e->key() == Qt::Key_BracketLeft) {
        QTextCursor cursor = textCursor();
        if (e->key() == Qt::Key_ParenLeft) {
            cursor.insertText("()");
        }
        else if (e->key() == Qt::Key_BraceLeft) {
            cursor.insertText("{}");
        }
        else {
            cursor.insertText("[]");
        }
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
    }
    else if (e->key() == Qt::Key_ParenRight || e->key() == Qt::Key_BraceRight || e->key() == Qt::Key_BracketRight) {
        QTextCursor cursor = textCursor();

        QTextCursor nextChar_cursor = textCursor();
        nextChar_cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        QString nextChar = nextChar_cursor.selectedText();

        if (e->key() == Qt::Key_ParenRight && nextChar == ")") {
            cursor.movePosition(QTextCursor::Right);
            setTextCursor(cursor);
        }
        else if (e->key() == Qt::Key_BraceRight && nextChar == "}") {
            cursor.movePosition(QTextCursor::Right);
            setTextCursor(cursor);
        }
        else if (e->key() == Qt::Key_BracketRight && nextChar == "]") {
            cursor.movePosition(QTextCursor::Right);
            setTextCursor(cursor);
        }
        else {
            cursor.insertText(e->text());
            setTextCursor(cursor);
        }
    }
    else if (e->text() == "'" || e->text() == "\"") {
        QTextCursor cursor_select = textCursor();
        QTextCursor cursor_work = textCursor();
        if (!cursor_select.atEnd()) {
            cursor_select.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
            QString select = cursor_select.selectedText();
            if (select == "'" && e->text() == "'") {
                cursor_work.movePosition(QTextCursor::Right);
                setTextCursor(cursor_work);
            }
            else if (select == "\"" && e->text() == "\"") {
                cursor_work.movePosition(QTextCursor::Right);
                setTextCursor(cursor_work);
            }
            else if (select != "'" && e->text() == "'") {
                cursor_work.insertText("''");
                cursor_work.movePosition(QTextCursor::Left);
                setTextCursor(cursor_work);
            }
            else if (select != "\"" && e->text() == "\"") {
                cursor_work.insertText("\"\"");
                cursor_work.movePosition(QTextCursor::Left);
                setTextCursor(cursor_work);
            }
        }
        else {
            if (e->text() == "'") {
                cursor_work.insertText("''");
                cursor_work.movePosition(QTextCursor::Left);
                setTextCursor(cursor_work);
            }
            else if (e->text() == "\"") {
                cursor_work.insertText("\"\"");
                cursor_work.movePosition(QTextCursor::Left);
                setTextCursor(cursor_work);
            }
        }
    }
    else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        QTextCursor cursor = textCursor();
        QString text = cursor.block().text();
        QTextCursor cursor_select_left = textCursor();
        QTextCursor cursor_select_right = textCursor();
        cursor_select_left.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
        cursor_select_right.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
        QString selectedChar_left = cursor_select_left.selectedText();
        QString selectedChar_right = cursor_select_right.selectedText();
        if (selectedChar_left == "{" && selectedChar_right == "}") {
            cursor.insertBlock();
            cursor.insertText("\t\n");
            cursor.movePosition(QTextCursor::Left);
            setTextCursor(cursor);
        }
        else {
            if (text.startsWith("\t")) {
                cursor.insertBlock();
                cursor.insertText("\t");
                setTextCursor(cursor);
            }
            else {
                cursor.insertBlock();
                setTextCursor(cursor);
            }
        }
    }
    else if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_Slash) {
        QTextCursor cursor = textCursor();
        QString text = cursor.block().text(); 
        if (text.startsWith("//")) {
            cursor.movePosition(QTextCursor::StartOfBlock);
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
            cursor.removeSelectedText();
            setTextCursor(cursor);
        }
        else {
            cursor.movePosition(QTextCursor::StartOfBlock);
            cursor.insertText("//");
            cursor.movePosition(QTextCursor::Down);
            setTextCursor(cursor);
        }
    }
    else {
        QPlainTextEdit::keyPressEvent(e);
    }
}