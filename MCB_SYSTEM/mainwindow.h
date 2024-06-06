#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "Public/cn.h"
#include "SubWidget/networkconnect.h"
#include "SubWidget/mcb_test.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    CN *cn;

private:
    Ui::MainWindow *ui;
private:
    // 界面map
    QMap<DT::CHANGED_PAGE,QWidget*> widgetMap;
    // 网络连接界面
    NetworkConnect *netConnect;
    // 主控测试界面
    MCB_Test *mcb_test;

private slots:
    // 界面切换
    void onPageChanged(DT::CHANGED_PAGE index);
    // 切换按钮
    void on_toolButton_NetWork_clicked();
    void on_toolButton_MCB_test_clicked();
};
#endif // MAINWINDOW_H
