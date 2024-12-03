#pragma once
#include "qsyntaxhighlighter.h"
#include "qregularexpression.h"
#include "qtextformat.h"
#include "qtextdocument.h"
class CoolMark_Editer_Highlight : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    CoolMark_Editer_Highlight(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    QRegularExpression Title_Expression;
    QTextCharFormat Title_Format;

    //QRegularExpression Text_Expression;
    //QTextCharFormat Text_Format;

    //QRegularExpression List_Expression;
    //QTextCharFormat List_Format;

    QRegularExpression A_Expression;
    QTextCharFormat A_Format;

    QRegularExpression Import_Expression;
    QTextCharFormat Import_Format;
};

