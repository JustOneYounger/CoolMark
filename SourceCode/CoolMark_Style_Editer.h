#pragma once
#include "qplaintextedit.h"
#include <QWidget>
#include "qtextobject.h"
#include "CoolMark_Style_Editer_Highlight.h"

class CoolMark_Style_Editer:public QPlainTextEdit
{
	Q_OBJECT
public:
	CoolMark_Style_Editer(QWidget* parent = nullptr);
	~CoolMark_Style_Editer();

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
    QWidget* coolmark_style_editer_linenumberarea;
    CoolMark_Style_Editer_Highlight* coolmark_style_editer_highlight;

protected:
    void keyPressEvent(QKeyEvent* e) override;
};

