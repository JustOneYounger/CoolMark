#pragma once
#include "qplaintextedit.h"
#include <QWidget>
#include "qtextobject.h"
#include "CoolMark_Editer_Highlight.h"

class CoolMark_Editer:public QPlainTextEdit
{
	Q_OBJECT
public:
	CoolMark_Editer(QWidget* parent = nullptr);
	~CoolMark_Editer();

public:
    //左侧行号
    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();

protected:
    //左侧行号
    void resizeEvent(QResizeEvent* event) override;

private slots:
    //左侧行号
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect& rect, int dy);

private:
    QWidget* coolmark_editer_linenumberarea;
    CoolMark_Editer_Highlight* coolmark_editer_highlight;
};

