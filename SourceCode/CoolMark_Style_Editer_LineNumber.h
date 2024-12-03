#pragma once
#include <QWidget>
#include "qevent.h"
#include "qpainter.h"
#include "CoolMark_Style_Editer.h"
class CoolMark_Style_Editer_LineNumber : public QWidget
{
public:
    CoolMark_Style_Editer_LineNumber(CoolMark_Style_Editer* editer) :QWidget(editer), style_editer(editer) {};
    QSize sizeHint() const override
    {
        return QSize(style_editer->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        style_editer->lineNumberAreaPaintEvent(event);
    }
private:
    CoolMark_Style_Editer* style_editer;
};

