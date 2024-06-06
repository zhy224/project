#ifndef NETWORKCONNECT_H
#define NETWORKCONNECT_H

#include <QWidget>

#include "Public/cn.h"


namespace Ui {
class NetworkConnect;
}

class NetworkConnect : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkConnect(QWidget *parent = nullptr);
    ~NetworkConnect();



private:
    Ui::NetworkConnect *ui;



private:
    // 通信类
    CN *cn;
    // 通信超时timer
    QTimer outTimer;
    // 网络通信状态timer
    QTimer connectStatusTimer;


signals:
    // 连接
    //void connectNetwork();
    // 断开连接
    //void disconnectNetwork();
    // 通信超时信号
    void netTimeoutSignal();


private slots:
    // 网卡更新
    void interFaceUpdate();
    // 获取当前网卡
    void on_comboBox_selectInterfaces_currentIndexChanged(int index);
    // 通信超时
    void on_timeout();
    // 网路连接状态
    void onConnectChanged(bool value);
    // 网络连接(断开)
    void onConnect();
    // 数据接收
    void onReceiveData(QByteArray);
    // 目标ip编辑完成检验
    void on_lineEdit_targetIP_editingFinished();
    // 通信状态定时检测
    void connectStatus();
};

#endif // NETWORKCONNECT_H
