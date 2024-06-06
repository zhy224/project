#include "networkconnect.h"
#include "ui_networkconnect.h"
#include "mainwindow.h"

#if _MSC_VER>=1600
#pragma execution_character_set("utf-8");
#endif

static bool updateFlag = true;

NetworkConnect::NetworkConnect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkConnect)
{
    ui->setupUi(this);

    // 通信类
    cn = CN::GetInstance();//new CN(this);
    // 初始化网卡
    interFaceUpdate();




    // 通信连接
    //connect(this, &NetworkConnect::connectNetwork, cn, &CN::onConnectNetwork);
    // 通信断开
    //connect(this, &NetworkConnect::disconnectNetwork, cn, &CN::onDisconnectNetwork);

    // 网络连接
    connect(ui->pushButton_connect, &QPushButton::clicked, this, &NetworkConnect::onConnect);
    // 连接超时
    connect(&outTimer, &QTimer::timeout, this, &NetworkConnect::on_timeout);
    // 连接状态
    connect(cn, &CN::connectChanged, this, &NetworkConnect::onConnectChanged); //cn->SetWork值不一样才会发送connectChanged信号
    // 数据接收
    connect(cn, &CN::senddata, this, &NetworkConnect::onReceiveData,Qt::AutoConnection);
    // 通信状态(定时)
    //connect(&connectStatusTimer,&QTimer::timeout, this, &NetworkConnect::connectStatus);


}

NetworkConnect::~NetworkConnect()
{
    delete ui;
}
/**
 * 网卡更新
 */
void NetworkConnect::interFaceUpdate()
{
    //qint16 itemCount = ui->comboBox_selectInterfaces->count();
    QStringList allInterfaces = cn->getNetworkInterfaces();

    for(int i=0;i<allInterfaces.count();i++)
    {
        ui->comboBox_selectInterfaces->addItem(allInterfaces.at(i));
        if(allInterfaces.at(i) == "以太网")
        {
            ui->comboBox_selectInterfaces->setCurrentIndex(i);
        }

    }

}
/**
 * 网卡选择按钮
 */
void NetworkConnect::on_comboBox_selectInterfaces_currentIndexChanged(int index)
{
    QString currentIP = cn->getCurrentInterfacesIP(index);
    cn->currInface = cn->face_list.at(index);
    //cn->setLocalIP_Port(currentIP,12345);
    ui->lineEdit_local_IP->setText(currentIP);
}
/**
 * 网络连接按钮
 */
void NetworkConnect::onConnect()
{
    QString targetIP = ui->lineEdit_targetIP->text();
    qint16 port = ui->lineEdit_targetPort->text().toUInt();
    if(!cn->isValidIP(targetIP))
    {
        ui->lineEdit_targetIP->setStyleSheet("color:red;");
        return;
    }
    ui->lineEdit_targetIP->setStyleSheet("color:black;");

    // 连接
    if(!cn->isNetwork())
    {
        // 设置目标ip
        cn->setTargetIP_Port(targetIP,port);
        // 连接网络
        cn->onConnectNetwork();
        // 发送消息
        QByteArray msg;
        int curIndex = ui->comboBox_TestBoard->currentIndex(); // 0=主控板，1=综合板
        switch (curIndex) {
        case 0:
            msg = Command::query_main_CMD();
            break;
        case 1:
            msg = Command::query_syn_CMD();
        default:
            break;
        }
        cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);
        // 通信超时开启
        outTimer.start(TIMER_TIMEROUT);

    }
    else
    {
        // 断开连接
        cn->onDisconnectNetwork();
        cn->setNetwork(false);
    }
}
/**
 * ip检查
 */
void NetworkConnect::on_lineEdit_targetIP_editingFinished()
{
    qDebug()<<"on_lineEdit_targetIP_editingFinished()";
    if(!cn->isValidIP(ui->lineEdit_targetIP->text()))
    {
        qDebug()<<"ip无效";
        ui->lineEdit_targetIP->clear();
        ui->lineEdit_targetIP->setPlaceholderText("ip无效，请重新输入...");
    }
}
/**
 * @brief 通信超时
 */
void NetworkConnect::on_timeout()
{
    // 通信超时信号
    //emit netTimeoutSignal();
    // 通信超时
    QMessageBox::about(this,"通信提示","通信超时");
    // 关闭定时器
    outTimer.stop();
    connectStatusTimer.stop();
    // 网络未连接
    //cn->setNetwork(false); // 无效
}

/**
 * @brief 网路连接状态
 * @param value
 */
void NetworkConnect::onConnectChanged(bool value)
{
    qDebug()<<"收到网络改变信号"<<cn->isNetwork();
    if(value) {
        outTimer.stop();
    }
    if(cn->isNetwork()) {
        // 开启定时查询
        if(!connectStatusTimer.isActive())
        {
            connectStatusTimer.start(CONNECT_STATUS_TIMEROUT);
        }
        ui->pushButton_connect->setText("断开");
        ui->label_StatusLight->setStyleSheet("background:green;min-width:30px;min-height:30px;max-width:30px; max-height:30px;border-radius:15px;");
    }
    else {
        // 关闭定时查询
        if(connectStatusTimer.isActive())
        {
            connectStatusTimer.stop();
        }
        ui->pushButton_connect->setText("连接");
        ui->label_StatusLight->setStyleSheet("background:grey;min-width:30px;min-height:30px;max-width:30px; max-height:30px;border-radius:15px;");
    }


}
/**
 * @brief 通信状态
 * @param
 */
void NetworkConnect::connectStatus()
{
    qDebug()<<"connectStatus()";
    // 重置状态
    cn->networkState = false;
    // 发送查询命令
    QByteArray msg;
    int curIndex = ui->comboBox_TestBoard->currentIndex(); // 0=主控板，1=综合板
    switch (curIndex) {
    case 0:
        msg = Command::query_main_CMD();
        break;
    case 1:
        msg = Command::query_syn_CMD();
    default:
        break;
    }
    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);
    // 延时
    SC::Sleep(500);
    // 获取连接状态
    if(cn->isNetwork())
    {
        qDebug()<<"通信正常...........";
    }
    else
    {
        // 通信断开
        cn->onDisconnectNetwork();
        // 通信改变
        onConnectChanged(false);
        qDebug()<<"通信断开...........";
        QMessageBox::about(this,"通信提示","通信断开");
    }
}
/**
 * @brief 数据接收
 *
 */
void NetworkConnect::onReceiveData(QByteArray arr) // 第一次收到数据更新网卡
{
    qDebug()<<"NetworkConnect收到data:"<<cn->isNetwork()<<arr.toHex();
    if(updateFlag)
    {
        if(cn->networkState)
        {
            // 设置本地ip
            cn->setLocalIP_Port(cn->udpSocket->localAddress().toString(),cn->udpSocket->localPort());
            // 当前连接网卡显示到界面
            QString connectInterface = cn->findInterfacesByIp(cn->sLocalIP);
            ui->comboBox_selectInterfaces->setCurrentText(connectInterface);
            ui->lineEdit_local_IP->setText(cn->sLocalIP);
        }
        updateFlag = false;
    }

}
