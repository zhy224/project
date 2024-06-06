#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Public/cn.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

private:
    // 通信类
    CN *cn;
    // 弹出框
    MsgBox *msgBox;
    // 通信超时timer
    QTimer outTimer;
    // 网络通信状态timer
    QTimer connectStatusTimer;

private:
    // 初始化
    void dataInit();
    // 生成DSN和蓝牙名
    bool generateDSN();
    // 更新界面制造信息
    void updateMakeinfoData(UI_MAKEINFO_DATA data);

private slots:
    // 通信超时
    void on_timeout();
    // 网路连接状态
    void onConnectChanged(bool value);
    // 网络连接(断开)
    void onConnect();
    // 通信状态定时检测
    void connectStatus();
    // 数据接收
    void onReceiveData(QByteArray);
    // 语言切换
    void switchLanguage();
    // 刷新
    void queryMakeInfo();
    // 存储
    void saveMakeInfo();

};
#endif // MAINWINDOW_H
