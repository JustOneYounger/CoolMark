#pragma once
#include <QWidget>
#include "qevent.h"
#include "qpainter.h"
#include "CoolMark_Editer.h"

class CoolMark_Editer_LineNumber : public QWidget
{
public:
    CoolMark_Editer_LineNumber(CoolMark_Editer* editer) :QWidget(editer), markdown_editer(editer) {};
    QSize sizeHint() const override
    {
        return QSize(markdown_editer->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        markdown_editer->lineNumberAreaPaintEvent(event);
    }
private:
    CoolMark_Editer* markdown_editer;
};

