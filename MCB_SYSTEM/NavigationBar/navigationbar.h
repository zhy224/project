#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QWidget>
#include <QLayout>
#include <QToolButton>
#include <QFile>
#include <QDebug>

#include "Public/DT.h"


namespace Ui {
class NavigationBar;
}

class NavigationBar : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationBar(QWidget *parent = nullptr);
    ~NavigationBar();

private:
    Ui::NavigationBar *ui;

    //

private:
    // ui初始化
    void uiInit();
    // 加载样式表
    void loadStyle();

public:
    // 插入bar
    void insertBar(int index, QString name, QString upIcon, QString downIcon,DT::CHANGED_PAGE page);

signals:
    // 信号 界面切换
    void pageChanged(DT::CHANGED_PAGE);
};

#endif // NAVIGATIONBAR_H
