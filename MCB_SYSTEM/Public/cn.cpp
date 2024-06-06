#include "cn.h"

#if _MSC_VER>=1600
#pragma execution_character_set("utf-8");
#endif

CN* CN::cn = nullptr;

CN::CN(QObject *parent) : QObject(parent)
{
    // 创建udp套接字
    udpSocket = new QUdpSocket;
    // 通信(接收)
    connect(udpSocket,&QUdpSocket::readyRead,this,&CN::readPendingDatagrams,Qt::AutoConnection);
    // socket状态
    connect(udpSocket, &QAbstractSocket::stateChanged, this, &CN::socketStateChanged, Qt::AutoConnection);
    // socket错误信息
    connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSocketError(QAbstractSocket::SocketError)), Qt::AutoConnection);

}
/**
 * 获取实例
 *
 */

CN* CN::GetInstance()
{
    if (cn == nullptr)
    {
        cn = new CN();
    }
    return  cn;
}

/**
 * 发送数据(十六进制)
 *
 */
void CN::sendMessage(const QByteArray &message, const QString &host, quint16 port)
{
    udpSocket->writeDatagram(message.toHex(), QHostAddress(host), port);
    udpSocket->flush();
    QCoreApplication::processEvents();
    qDebug()<<QString("T data(%1): %2").arg(message.size()).arg(QString(message.toHex(' ')));
}
/*
 * 获取所有网卡
 * 返回网卡名
 */
QList<QString> CN::getNetworkInterfaces()
{
    face_list = QNetworkInterface::allInterfaces();
    //qDebug()<<"face_list.size:"<<face_list.count();
    sInterfaceName.clear();
    for(int i = 0; i < face_list.count(); i++)
    {
        sInterfaceName<<face_list.at(i).humanReadableName();
    }
    qDebug()<<"sInterfaceName:"<<sInterfaceName;
    return sInterfaceName;
}
/*
 * 获取选择网卡的IPV4地址
 * 返回ip
 */
QString CN::getCurrentInterfacesIP(qint8 index)
{
    auto address_list = face_list.at(index).addressEntries();
    for (int i = 0; i < address_list.count(); i++)
    {
        auto t_ip = address_list.at(i).ip();
        // 判断IP是否是IPV4
        if (t_ip.protocol() == QAbstractSocket::IPv4Protocol)
        {
            sLocalIP = t_ip.toString();
            //qDebug()<<"sLocalIP:"<<sLocalIP;
        }
    }
    return  sLocalIP;
}
/*
 * 根据ip查找网卡
 * 返回网卡
 */
QString CN::findInterfacesByIp(const QString &ip) {
    QHostInfo hostInfo = QHostInfo::fromName(ip);
    QList<QHostAddress> addresses = hostInfo.addresses();
    QString interfaceName;
    foreach (QHostAddress address, addresses) {
        qDebug() << "local ip: " << address.toString();
        foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces()) {
            // 检查接口是否包含该IP地址
            auto addressList = interface.addressEntries();
            for (int i = 0; i < addressList.count(); i++)
            {
                if (addressList.at(i).ip() == address) {
                    qDebug() << "Found interface: " << interface.name();
                    interfaceName = interface.humanReadableName();
                }
            }

        }
    }
    return interfaceName;
}
/*
 * 判断ip
 *
 */
bool CN::isValidIP(QString ipStr) {
    QHostAddress address(ipStr);
    return address.protocol() == QAbstractSocket::IPv4Protocol ||
           address.protocol() == QAbstractSocket::IPv6Protocol;
}
/**
 * 连接网络
 *
 */
void CN::connectNetwork(QString ip, quint16 port)
{
    //qDebug()<<"connectNetwork:";
    qDebug()<<"target:"<<ip<<port;

    // 先断开之前的连接
    if(udpSocket->ConnectedState == QAbstractSocket::ConnectedState)
    {
        this->disconnectNetwork();
    }
    // 再连接
    udpSocket->connectToHost(QHostAddress(ip), port);
}
/**
 * 断开网络
 *
 */
void CN::disconnectNetwork()
{
    //qDebug()<<"disconnectNetwork:";
    udpSocket->abort();
    udpSocket->disconnectFromHost();
    udpSocket->close();
    //
    //this->setNetwork(false);

}
/**
 * 连接网络槽函数.
 *
 * \param ip
 * \param port
 */
void CN::onConnectNetwork()
{
    this->connectNetwork(sTargetIP, sTargetPort);
}

/**
 * 断开网络槽函数.
 *
 */
void CN::onDisconnectNetwork()
{
    this->disconnectNetwork();
}
/**
 * socket连接状态
 *
 */
QString CN::socketStateChanged(QUdpSocket::SocketState state)
{
    QString connectState;
    switch (state)
    {
    case QAbstractSocket::UnconnectedState:
        qDebug() << "udp socket state:Not connected!";
        connectState = "未连接！";
        break;
    case QAbstractSocket::HostLookupState:
        break;
    case QAbstractSocket::ConnectingState:
        qDebug() << "udp socket state:Connecting...";
        connectState = "连接中...";
        break;
    case QAbstractSocket::ConnectedState:
        qDebug() << "udp socket state:Connected!";
        connectState = "已连接！";
        break;
    case QAbstractSocket::BoundState:
        qDebug() << "udp socket state:Binding...";
        connectState = "绑定状态...";
        break;
    case QAbstractSocket::ListeningState:
        qDebug() << "udp socket state:Listening...";
        connectState = "监听状态...";
        break;
    case QAbstractSocket::ClosingState:
        qDebug() << "udp socket state:Closing...";
        connectState = "关闭连接中...";
        break;
    default:
        break;
    }
    return connectState;
}
/**
 * socket状态
 *
 */
void CN::onSocketError(QAbstractSocket::SocketError Error)
{

    switch (Error)
    {
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "udp socket 错误: 连接被拒绝...";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "udp socket 错误: 典型错误...";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "udp socket 错误: 找不到主机...";
        break;
    case QAbstractSocket::SocketAccessError:
        qDebug() << "udp socket 错误: socket通道错误...";
        break;
    case QAbstractSocket::SocketResourceError:
        qDebug() << "udp socket 错误: 资源错误...";
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug() << "udp socket 错误: 连接超时...";
        break;
    case QAbstractSocket::DatagramTooLargeError:
        qDebug() << "udp socket 错误: DatagramTooLargeError...";
        break;
    case QAbstractSocket::NetworkError:
        qDebug() << "udp socket 错误: 网络错误...";
        break;
    case QAbstractSocket::AddressInUseError:
        qDebug() << "udp socket 错误: AddressInUseError...";
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug() << "udp socket 错误: SocketAddressNotAvailableError...";
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug() << "udp socket 错误: UnsupportedSocketOperationError...";
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug() << "udp socket 错误: UnfinishedSocketOperationError...";
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug() << "udp socket 错误: ProxyAuthenticationRequiredError...";
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        qDebug() << "udp socket 错误: SslHandshakeFailedError...";
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        qDebug() << "udp socket 错误: ProxyConnectionRefusedError...";
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        qDebug() << "udp socket 错误: ProxyConnectionClosedError...";
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        qDebug() << "udp socket 错误: ProxyConnectionTimeoutError...";
        break;
    case QAbstractSocket::ProxyNotFoundError:
        qDebug() << "udp socket 错误: ProxyNotFoundError...";
        break;
    case QAbstractSocket::ProxyProtocolError:
        qDebug() << "udp socket 错误: ProxyProtocolError...";
        break;
    case QAbstractSocket::OperationError:
        qDebug() << "udp socket 错误: OperationError...";
        break;
    case QAbstractSocket::SslInternalError:
        qDebug() << "udp socket 错误: SslInternalError...";
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        qDebug() << "udp socket 错误: SslInvalidUserDataError...";
        break;
    case QAbstractSocket::TemporaryError:
        qDebug() << "udp socket 错误: TemporaryError...";
        break;
    case QAbstractSocket::UnknownSocketError:
        qDebug() << "udp socket 错误: 未知错误...";
        break;
    default:
        qDebug() << "udp socket 错误: default...";
        break;
    }
}
/**
 * 设置网络连接状态.
 *
 * \param value
 */
void CN::setNetwork(bool value)
{
    if(this->networkState != value)
    {
        this->networkState = value;
        emit connectChanged(value);
    }
}


/**
 * 设置目标ip,port
 *
 */
void CN::setTargetIP_Port(QString ip,quint16 port)
{
    sTargetIP = ip;
    sTargetPort = port;

}
/**
 * 设置本机ip,port
 *
 */
void CN::setLocalIP_Port(QString ip,quint16 port)
{
    sLocalIP = ip;
    sLocalPort = port;
}
/**
 * 接收数据
 *
 */
void CN::readPendingDatagrams()
{
    qDebug()<<"readPendingDatagrams()";
    QHostAddress addr; //用于获取发送者的 IP 和端口
    quint16 port;
    //数据缓冲区
    QByteArray arr;
    while(udpSocket->hasPendingDatagrams())
    {
        //调整缓冲区的大小和收到的数据大小一致
        arr.resize(udpSocket->bytesAvailable());
        //接收数据
        udpSocket->readDatagram(arr.data(),arr.size(),&addr,&port);

        qDebug()<<QString("R data(%1): %2").arg(arr.size()).arg(QString(arr.toHex(' ')));

        // 命令符
        quint8 command = arr.at(1);
        switch (command)
        {
            // 通过判断是否回传主控板数据来判断网络是否连接成功
        case DT::BackComandCodeM:
        {
            // 判断数据长度
            if (arr.size() != BACK_DATA_SIZE_M) {
                return;
            }
            if (!this->isNetwork()) {
                // 有数据接收 则网络连接
                this->setNetwork(true);
            }
            // 获取解析数据
        }
            break;
            // 通过判断是否回传综合板数据来判断网络是否连接成功
        case DT::BackComandCodeS:
        {
            // 判断数据长度
            if (arr.size() != BACK_DATA_SIZE_S) {
                return;
            }
            if (!this->isNetwork()) {
                // 有数据接收 则网络连接
                this->setNetwork(true);
            }
            // 获取解析数据

        }
            break;
        default:
            break;
        }
        // 发送数据
        emit senddata(arr);

    }
}



