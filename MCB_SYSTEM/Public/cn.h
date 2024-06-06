#ifndef CN_H
#define CN_H

#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QList>

#include <QByteArray>
#include <QUdpSocket>

#include <QNetworkInterface>
#include <QHostAddress>
#include <QHostInfo>


#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileDialog>

#include "Public/DT.h"
#include "Public/SC.h"
#include "MsgBox/msgbox.h"

class CN : public QObject
{
    Q_OBJECT
public:
    explicit CN(QObject *parent = nullptr);

signals:

    // 数据唯一信号 发送到各界面
    void senddata(QByteArray);
    // 网络连接状态信号
    void connectChanged(bool);

public:

    //创建udp对象
    QUdpSocket* udpSocket;
    // 本机网卡列表
    QList<QNetworkInterface> face_list;
    // 本机网卡名
    QList<QString> sInterfaceName;
    // 当前网卡
    QNetworkInterface currInface;
    // 本机IP
    QString sLocalIP;
    // 本机port
    quint16 sLocalPort;
    // 目标IP
    QString sTargetIP;
    // 目标Port
    quint16 sTargetPort;
    // 网络连接状态
    bool networkState = false;
    // 静态实例
    static CN* cn;
    // 获取实例
    static CN* GetInstance();
public:

    // 获取网卡名
    QList<QString> getNetworkInterfaces();
    // 获取网卡ip
    QString getCurrentInterfacesIP(qint8 index);
    // 设置本机ip,port
    void setLocalIP_Port(QString ip,quint16 port);
    // 设置目标ip,PORT
    void setTargetIP_Port(QString ip,quint16 port);
    // 根据ip查找网卡名
    QString findInterfacesByIp(const QString &ip);
    // ip地址是否有效
    bool isValidIP(QString ipStr);
    // 获取网络连接状态
    bool isNetwork() const { return networkState;}
    // 设置网络连接状态
    void setNetwork(bool value);
    // 连接网络槽函数
    void onConnectNetwork();
    // 断开网络槽函数
    void onDisconnectNetwork();
    // 发送数据(十六进制)
    void sendMessage(const QByteArray &message, const QString &host, quint16 port);



private:

    // 连接网络
    void connectNetwork(QString ip, quint16 port);
    // 断开网络
    void disconnectNetwork();


public slots:

    // socket连接状态
    QString socketStateChanged(QUdpSocket::SocketState state);
    // socket错误状态
    void onSocketError(QAbstractSocket::SocketError Error);
    // 接收数据
    void readPendingDatagrams();



};



#endif // CN_H
