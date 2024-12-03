#pragma once
#include "qsyntaxhighlighter.h"
#include "qregularexpression.h"
#include "qtextformat.h"
#include "qtextdocument.h"
class CoolMark_Style_Editer_Highlight : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    CoolMark_Style_Editer_Highlight(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    QRegularExpression Class_Expression;
    QTextCharFormat Class_Format;

    QRegularExpression Attribute_Expression;
    QTextCharFormat Attribute_Format;

    QRegularExpression Comment_Expression;
    QTextCharFormat Comment_Format;
};

