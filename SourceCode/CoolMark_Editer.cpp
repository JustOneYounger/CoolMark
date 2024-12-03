#include "CoolMark_Editer.h"
#include "CoolMark_Editer_LineNumber.h"

CoolMark_Editer::CoolMark_Editer(QWidget* parent) 
	:QPlainTextEdit(parent)
	,coolmark_editer_linenumberarea(new CoolMark_Editer_LineNumber(this))
{
    coolmark_editer_highlight = new CoolMark_Editer_Highlight(document());
    setStyleSheet("background:transparent;color:#E6DB74;font-family:'Consolas';font-size:14px;border:none;");
    setTabStopDistance(fontMetrics().horizontalAdvance(" ") * 8);
    setWordWrapMode(QTextOption::NoWrap);
    //左侧行号渲染
    connect(this, &CoolMark_Editer::blockCountChanged, this, &CoolMark_Editer::updateLineNumberAreaWidth);
    connect(this, &CoolMark_Editer::updateRequest, this, &CoolMark_Editer::updateLineNumberArea);
    connect(this, &CoolMark_Editer::cursorPositionChanged, this, &CoolMark_Editer::highlightCurrentLine);
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

CoolMark_Editer::~CoolMark_Editer() {
    delete coolmark_editer_linenumberarea;
    delete coolmark_editer_highlight;
}

int CoolMark_Editer::lineNumberAreaWidth()
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
void CoolMark_Editer::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}
void CoolMark_Editer::updateLineNumberArea(const QRect& rect, int dy)
{
    if (dy)
        coolmark_editer_linenumberarea->scroll(0, dy);
    else
        coolmark_editer_linenumberarea->update(0, rect.y(), coolmark_editer_linenumberarea->width(), rect.height());
    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
void CoolMark_Editer::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    coolmark_editer_linenumberarea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
void CoolMark_Editer::highlightCurrentLine()
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
void CoolMark_Editer::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(coolmark_editer_linenumberarea);
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
            painter.drawText(0, top, coolmark_editer_linenumberarea->width(), fontMetrics().height(),
                Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}