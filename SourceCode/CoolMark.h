#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CoolMark.h"
#include "CoolMark_include.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CoolMarkClass; };
QT_END_NAMESPACE

class CoolMark : public QMainWindow
{
    Q_OBJECT

public:
    CoolMark(QWidget *parent = nullptr);
    ~CoolMark();

    void basicWindow();
    void initWindow();
    void markdown_to_html();
    void coolmark_shortcut();

private:
    Ui::CoolMarkClass *ui;
    //是否正在拖动
    bool isMove;
    //鼠标位置
    QPoint MousePos;
    //是否是最大化窗口
    bool isMax = false;

    QSplitter* splitter_main;
    CoolMark_Editer* markdownEditer;
    CoolMark_Style_Editer* styleEditer;

private:
    QString main_path = QCoreApplication::applicationDirPath();
    QString python_converter_path = main_path + "/python/dist/MarkDownToHtml/MarkDownToHtml.exe";
    MarkdownConverterThread* converterThread;
    QString converterHtmlContent;

protected:
    //定义窗口拖动
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};