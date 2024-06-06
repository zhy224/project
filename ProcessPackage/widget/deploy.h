#ifndef DEPLOY_H
#define DEPLOY_H

#include <QWidget>

#include "Public/cn.h"

namespace Ui {
class Deploy;
}

class Deploy : public QWidget
{
    Q_OBJECT

public:
    explicit Deploy(QWidget *parent = nullptr);
    ~Deploy();

private:
    Ui::Deploy *ui;

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
    void dataInit();


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

};

#endif // DEPLOY_H
