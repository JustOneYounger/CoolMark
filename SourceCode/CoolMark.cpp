#include "CoolMark.h"

CoolMark::CoolMark(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::CoolMarkClass())
    , markdownEditer(new CoolMark_Editer(this))
    , styleEditer(new CoolMark_Style_Editer(this))
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(main_path + "/markdown.ico"));
    basicWindow();
    initWindow();
    coolmark_shortcut();

    converterThread = new MarkdownConverterThread(python_converter_path, main_path + "/temp.md", main_path + "/temp.html");
    QWebEngineSettings* settings = ui->webEngineView->settings();
    settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    settings->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    settings->setAttribute(QWebEngineSettings::TouchIconsEnabled, true);
    settings->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
    settings->setAttribute(QWebEngineSettings::WebGLEnabled, true);
    settings->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, true);
    settings->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    settings->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    settings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    settings->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    settings->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);
    
    connect(markdownEditer, &QPlainTextEdit::textChanged, this, &CoolMark::markdown_to_html);
    connect(converterThread, &QThread::finished, this, [this]() {
        QFile temphtmlFile(main_path + "/temp.html");
        if (temphtmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream read_html(&temphtmlFile);
            converterHtmlContent = read_html.readAll();
            temphtmlFile.close();
        }
        if (temphtmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream append_css_js(&temphtmlFile);
            converterHtmlContent =
                "<!DOCTYPE html>"
                "<html lang=\"zh-CN\">"
                "<head>"
                "<meta charset=\"utf-8\">"
                "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1.0\">"
                "<link rel=\"stylesheet\" href=\"" + main_path + "/coolmark_style.css\">"
                "<script type=\"text/x-mathjax-config\">MathJax.Hub.Config({config:[\"MMLorHTML.js\"],jax:[\"input/TeX\",\"output/HTML-CSS\",\"output/NativeMML\"],extensions:[\"MathMenu.js\",\"MathZoom.js\"]});</script>"
                "<script type=\"text/javascript\" src=\"https://cdn.staticfile.org/mathjax/2.7.7/MathJax.js\"></script>"
                "</head>"
                "<body>"
                + converterHtmlContent +
                "</body>"
                "</html>";
            append_css_js << converterHtmlContent;
            temphtmlFile.close();
        }
        ui->webEngineView->reload();
        //ui->webEngineView->load(QUrl::fromLocalFile(main_path + "/temp.html"));
    });
    connect(styleEditer, &QPlainTextEdit::textChanged, this, [this]() {
        QFile cssFile(main_path + "/coolmark_style.css");
        if (cssFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream write_css(&cssFile);
            QString css = styleEditer->toPlainText();
            write_css << css;
        }
        ui->webEngineView->reload();
        //ui->webEngineView->load(QUrl::fromLocalFile(main_path + "/temp.html"));
        cssFile.close();
    });
}

CoolMark::~CoolMark()
{
    QFile tempmdFile(main_path+"/temp.md");
    QFile temphtmlFile(main_path+"/temp.html");
    tempmdFile.remove();
    temphtmlFile.remove();
    delete converterThread;
    delete splitter_main;
    delete markdownEditer;
    delete styleEditer;
    delete ui;
}

void CoolMark::basicWindow() {
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    connect(ui->pushButton_min, &QPushButton::clicked, this, [this]() {
        this->showMinimized();
    });
    connect(ui->pushButton_max, &QPushButton::clicked, this, [this]() {
        if (isMax == false) {
            isMax = true;
            this->showMaximized();
        }
        else {
            this->resize(1200, 700);
            isMax = false;
        }
    });
    connect(ui->pushButton_quit, &QPushButton::clicked, this, [this]() {
        this->close();
    });
}

void CoolMark::mousePressEvent(QMouseEvent* event) {
    if (ui->widget_headerline->rect().contains(event->pos())) {
        isMove = true;
        MousePos = event->pos();
    }
}
void CoolMark::mouseMoveEvent(QMouseEvent* event) {
    if (isMove) {
        move(pos() + event->pos() - MousePos);
    }
}
void CoolMark::mouseReleaseEvent(QMouseEvent* event) {
    isMove = false;
}

void CoolMark::initWindow() {
    //this->showMaximized();
    /*QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
    opacityEffect->setOpacity(0.9);
    ui->centralWidget->setGraphicsEffect(opacityEffect);*/

    splitter_main = new QSplitter(Qt::Horizontal,ui->widget_main);
    splitter_main->addWidget(ui->widget_MarkDownEditer);
    splitter_main->addWidget(ui->widget_WebView);
    splitter_main->addWidget(ui->widget_StyleEditer);

    QHBoxLayout* layout = new QHBoxLayout(ui->widget_main);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(splitter_main);
    
    splitter_main->setStyleSheet("QSplitter::handle{background-color:#66D9EF;}");
    splitter_main->setHandleWidth(2);
    QList<int> sizes;
    sizes << 300 << 500 << 200;
    splitter_main->setSizes(sizes);

    QVBoxLayout* layout_MarkDownEditer = new QVBoxLayout(ui->widget_MarkDownEditer);
    layout_MarkDownEditer->setContentsMargins(0, 0, 0, 0);
    layout_MarkDownEditer->addWidget(markdownEditer);

    QVBoxLayout* layout_StyleEditer = new QVBoxLayout(ui->widget_StyleEditer);
    layout_StyleEditer->setContentsMargins(0, 0, 0, 0);
    layout_StyleEditer->addWidget(styleEditer);

    QFile tempmdFile(main_path+"/temp.md");
    if (tempmdFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        tempmdFile.close();
    }

    QFile temphtmlFile(main_path+"/temp.html");
    if (temphtmlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream init_html(&temphtmlFile);
        init_html << "<link rel=\"stylesheet\" href=\"" + main_path + "/coolmark_style.css\">";
        temphtmlFile.close();
        ui->webEngineView->load(QUrl::fromLocalFile(main_path + "/temp.html"));
    }

    QFile cssFile(main_path + "/coolmark_style.css");
    if (cssFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream read_css(&cssFile);
        QString css = read_css.readAll();
        styleEditer->setPlainText(css);
        cssFile.close();
    }
}

void CoolMark::markdown_to_html() {
    QFile tempmdFile(main_path+"/temp.md");
    if (tempmdFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream write_markdown(&tempmdFile);
        write_markdown << markdownEditer->toPlainText();
        tempmdFile.close();
    }
    ui->label_status->setText("未保存");
    converterThread->start();
}

void CoolMark::coolmark_shortcut() {
    QShortcut* shortcut_openmd = new QShortcut(QKeySequence("Ctrl+O"), this);
    connect(shortcut_openmd, &QShortcut::activated, this, [this]() {
        QString filename = QFileDialog::getOpenFileName(nullptr, tr("Open MarkDown File"), "", tr("MarkDown File(*.md *.markdown *.mdown *.mkd *.mkdn)"));
        if (!filename.isEmpty()) {
            ui->label_openfile->setText(filename);
            markdownEditer->clear();
            QFile markdownfile(filename);
            if (markdownfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream read_markdown(&markdownfile);
                QString markdown_context = read_markdown.readAll();
                markdownEditer->insertPlainText(markdown_context);
                markdownfile.close();
            }
        }
    });

    QShortcut* shortcut_savemd = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(shortcut_savemd, &QShortcut::activated, this, [this]() {
        if (!ui->label_openfile->text().isEmpty()) {
            QString filepath = ui->label_openfile->text();
            QFile markdownfile(filepath);
            if (markdownfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream save_markdown(&markdownfile);
                QString markdown_context = markdownEditer->toPlainText();
                save_markdown << markdown_context;
                markdownfile.close();
            }
            ui->label_status->setText("已保存");
        }
        else {
            QString filename = QFileDialog::getSaveFileName(nullptr, tr("Save MarkDown File"), "NewMarkDownFile.md", tr("MarkDown File(*.md *.markdown *.mdown *.mkd *.mkdn)"));
            if (!filename.isEmpty()) {
                ui->label_openfile->setText(filename);
                QFile markdownfile(filename);
                if (markdownfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream save_markdown(&markdownfile);
                    QString markdown_context = markdownEditer->toPlainText();
                    save_markdown << markdown_context;
                    markdownfile.close();
                }
                ui->label_status->setText("已保存");
            }
        }
    });

    QShortcut* shortcut_clear = new QShortcut(QKeySequence("F12"), this);
    connect(shortcut_clear, &QShortcut::activated, this, [this]() {
        if (!ui->label_openfile->text().isEmpty()) {
            QString filepath = ui->label_openfile->text();
            QFile markdownfile(filepath);
            if (markdownfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream save_markdown(&markdownfile);
                QString markdown_context = markdownEditer->toPlainText();
                save_markdown << markdown_context;
                markdownfile.close();
            }

            ui->label_openfile->clear();
            ui->label_status->clear();
            markdownEditer->clear();
        }
        else {
            ui->label_status->clear();
            markdownEditer->clear();
        }
    });

    QShortcut* shortcut_output = new QShortcut(QKeySequence("F10"), this);
    connect(shortcut_output, &QShortcut::activated, this, [this]() {
        QString output_path = QFileDialog::getSaveFileName(nullptr, tr("Output MarkDown"), "MyPDF.pdf", tr("PDF Files(*.pdf)"));
        if (!output_path.isEmpty()) {
            QPageLayout layout;
            layout.setPageSize(QPageSize(QPageSize::A4));
            layout.setTopMargin(0);
            layout.setBottomMargin(0);
            layout.setLeftMargin(0);
            layout.setRightMargin(0);
            layout.setOrientation(QPageLayout::Portrait);
            ui->webEngineView->page()->printToPdf(output_path, layout);
            ui->label_status->setText("正在导出");
            connect(ui->webEngineView, &QWebEngineView::pdfPrintingFinished, this, [this]() {
                ui->label_status->setText("已导出");
            });
        }
    });


    QShortcut* shortcut_showhtml = new QShortcut(QKeySequence("Ctrl+L"), this);
    connect(shortcut_showhtml, &QShortcut::activated, this, [this]() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(main_path+"/temp.html"));
    });
}