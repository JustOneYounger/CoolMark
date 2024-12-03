#include "CoolMark_Editer_Highlight.h"

CoolMark_Editer_Highlight::CoolMark_Editer_Highlight(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    Title_Expression = QRegularExpression("^#{1,6} .*");
    Title_Format.setForeground(QColor("#FD971F"));

    //Text_Expression = QRegularExpression(R"(\*.*\*|\_.*\_|=.*=|~.*~|\^.*\^)");
    //Text_Format.setForeground(QColor("#F92672"));

    //List_Expression = QRegularExpression("[\*\-\+\d+]{1}.*");
    //List_Format.setForeground(QColor("#66D9Ef"));

    A_Expression = QRegularExpression("\\[.*\\]\\(http.*|<.*>|!\\[.*\\]\\(http.*");
    A_Format.setForeground(QColor("#66D9EF"));
    A_Format.setFontUnderline(true);

    Import_Expression = QRegularExpression("^>+ .*");
    Import_Format.setForeground(QColor("#F92672"));
}

void CoolMark_Editer_Highlight::highlightBlock(const QString& text) {
    QRegularExpressionMatchIterator match_Title_Iterator = Title_Expression.globalMatch(text);
    while (match_Title_Iterator.hasNext()) {
        QRegularExpressionMatch match_Title = match_Title_Iterator.next();
        setFormat(match_Title.capturedStart(), match_Title.capturedLength(), Title_Format);
    }
    /*QRegularExpressionMatchIterator match_Text_Iterator = Text_Expression.globalMatch(text);
    while (match_Text_Iterator.hasNext()) {
        QRegularExpressionMatch match_Text = match_Text_Iterator.next();
        setFormat(match_Text.capturedStart(), match_Text.capturedLength(), Text_Format);
    }*/
    /*QRegularExpressionMatchIterator match_List_Iterator = List_Expression.globalMatch(text);
    while (match_List_Iterator.hasNext()) {
        QRegularExpressionMatch match_List = match_List_Iterator.next();
        setFormat(match_List.capturedStart(), match_List.capturedLength(), List_Format);
    }*/
    QRegularExpressionMatchIterator match_A_Iterator = A_Expression.globalMatch(text);
    while (match_A_Iterator.hasNext()) {
        QRegularExpressionMatch match_A = match_A_Iterator.next();
        setFormat(match_A.capturedStart(), match_A.capturedLength(), A_Format);
    }
    QRegularExpressionMatchIterator match_Import_Iterator = Import_Expression.globalMatch(text);
    while (match_Import_Iterator.hasNext()) {
        QRegularExpressionMatch match_Import = match_Import_Iterator.next();
        setFormat(match_Import.capturedStart(), match_Import.capturedLength(), Import_Format);
    }
}
