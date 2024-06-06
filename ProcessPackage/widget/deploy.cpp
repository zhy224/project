#include "deploy.h"
#include "ui_deploy.h"

#if _MSC_VER>=1600
#pragma execution_character_set("utf-8");
#endif

Deploy::Deploy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Deploy)
{
    ui->setupUi(this);

    // 通信类
    cn = CN::GetInstance();
    //
    dataInit();


    // 网络连接
    connect(ui->pushButton_netConnect, &QPushButton::clicked, this, &Deploy::onConnect);
    // 连接超时
    connect(&outTimer, &QTimer::timeout, this, &Deploy::on_timeout);
    // 连接状态
    connect(cn, &CN::connectChanged, this, &Deploy::onConnectChanged); //cn->SetWork值不一样才会发送connectChanged信号
    // 数据接收
    connect(cn, &CN::senddata, this, &Deploy::onReceiveData);
    // 定时查询
    connect(&connectStatusTimer,&QTimer::timeout, this, &Deploy::connectStatus);
    // 切换语言
    connect(ui->switchLanguage, &QPushButton::clicked, this, &Deploy::switchLanguage);
}

Deploy::~Deploy()
{
    delete ui;
}
void Deploy::dataInit()
{
    ui->comboBox_selectLanguage->setCurrentIndex(1);
    switchLanguage();
}
/**
 * 语言切换按钮
 */
void Deploy::switchLanguage()
{
    //QString strPath = QCoreApplication::applicationDirPath(); //exe文件路径
    qDebug()<<ui->comboBox_selectLanguage->currentIndex();
    int index = ui->comboBox_selectLanguage->currentIndex();
    CN::switchLanguage(index);
    ui->retranslateUi(this);
}
/**
 * 网络连接按钮
 */
void Deploy::onConnect()
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

        msg = Command::queryNetwork();

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
 * @brief 通信超时
 */
void Deploy::on_timeout()
{
    msgBox->confirmQBox(tr("通信提示"),tr("通信超时!(网络连接)"),tr("确认"));
    // 关闭定时器
    outTimer.stop();
}

/**
 * @brief 网路连接状态
 * @param value
 */
void Deploy::onConnectChanged(bool value)
{
    //qDebug()<<"收到网络改变信号"<<cn->isNetwork();
    if(value) {
        outTimer.stop();
    }
    if(cn->isNetwork()) {
        outTimer.stop();
        // 开启定时查询
        connectStatusTimer.start(CONNECT_STATUS_TIMEROUT);
        ui->pushButton_netConnect->setText(tr("断开"));
        ui->light_state->setGreen();
    }
    else {
        // 关闭定时查询
        connectStatusTimer.stop();
        ui->pushButton_netConnect->setText(tr("连接"));
        ui->light_state->setGray();
    }


}
/**
 * @brief 通信状态
 * @param
 */
void Deploy::connectStatus()
{
    //qDebug()<<"connectStatus()";
    // 发送查询命令
    QByteArray msg;
    // 查询网络
    msg = Command::queryNetwork();

    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);

}
/**
 * @brief 数据接收
 *
 */
void Deploy::onReceiveData(QByteArray arr) // 第一次收到数据更新网卡
{
    //qDebug()<<"NetworkConnect收到data:"<<arr.toHex();
    if(arr.at(0) == DT::Header)
    {
        outTimer.stop();
    }
}


