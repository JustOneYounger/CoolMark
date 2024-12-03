#include "CoolMark_Style_Editer_Highlight.h"

CoolMark_Style_Editer_Highlight::CoolMark_Style_Editer_Highlight(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    Class_Expression = QRegularExpression("(.*){");
    Class_Format.setForeground(QColor("#F92672"));

    Attribute_Expression = QRegularExpression(R"(\s([a-zA-Z0-9\-]*):.*;)");
    Attribute_Format.setForeground(QColor("#66D9EF"));

    Comment_Expression = QRegularExpression(R"(//.*)");
    Comment_Format.setForeground(QColor("#75715E"));
}

void CoolMark_Style_Editer_Highlight::highlightBlock(const QString& text) {
    QRegularExpressionMatchIterator match_Class_Iterator = Class_Expression.globalMatch(text);
    while (match_Class_Iterator.hasNext()) {
        QRegularExpressionMatch match_Class = match_Class_Iterator.next();
        setFormat(match_Class.capturedStart(1), match_Class.capturedLength(1), Class_Format);
    }

    QRegularExpressionMatchIterator match_Attribute_Iterator = Attribute_Expression.globalMatch(text);
    while (match_Attribute_Iterator.hasNext()) {
        QRegularExpressionMatch match_Attribute = match_Attribute_Iterator.next();
        setFormat(match_Attribute.capturedStart(1), match_Attribute.capturedLength(1), Attribute_Format);
    }

    QRegularExpressionMatchIterator match_Comment_Iterator = Comment_Expression.globalMatch(text);
    while (match_Comment_Iterator.hasNext()) {
        QRegularExpressionMatch match_Comment = match_Comment_Iterator.next();
        setFormat(match_Comment.capturedStart(), match_Comment.capturedLength(), Comment_Format);
    }
}