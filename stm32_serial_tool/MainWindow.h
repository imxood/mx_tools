#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QDebug>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void processData();
    void setTextTermFormatting(QPlainTextEdit *textEdit, QString const &text);
    void parseEscapeSequence(int attribute, QListIterator<QString> &i, QTextCharFormat &textCharFormat, QTextCharFormat const &defaultTextCharFormat);

    Ui::MainWindow *ui;
    QSerialPort *m_device = NULL;
};

#endif // MAINWINDOW_H
