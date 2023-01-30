//
// Created by zaiheshi on 1/29/23.
//

#ifndef QTTEST_MAINWINDOW_H
#define QTTEST_MAINWINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};


#endif//QTTEST_MAINWINDOW_H
