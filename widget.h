//
// Created by zaiheshi on 1/29/23.
//

#ifndef QTTEST_WIDGET_H
#define QTTEST_WIDGET_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private:
    Ui::Widget *ui;
};


#endif//QTTEST_WIDGET_H
