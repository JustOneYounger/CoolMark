#pragma once
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QProcess>
class MarkdownConverterThread : public QThread {
    Q_OBJECT
public:
    QString python_converter_path;
    QString input_file;
    QString output_file;
    MarkdownConverterThread(const QString& python_converter_path, const QString& input_file, const QString& output_file) : python_converter_path(python_converter_path), input_file(input_file), output_file(output_file) {}

protected:
    void run() override {
        QProcess process;
        process.setProgram(python_converter_path);
        process.setArguments({ input_file, output_file });
        process.start();
        process.waitForFinished();
    }
};