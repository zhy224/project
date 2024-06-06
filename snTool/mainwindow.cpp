#include "mainwindow.h"
#include "ui_mainwindow.h"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 初始化
    dataInit();
    // 通信类
    cn = CN::GetInstance();

    // 网络连接
    connect(ui->pushButton_netConnect, &QPushButton::clicked, this, &MainWindow::onConnect);
    // 连接超时
    connect(&outTimer, &QTimer::timeout, this, &MainWindow::on_timeout);
    // 连接状态
    connect(cn, &CN::connectChanged, this, &MainWindow::onConnectChanged); //cn->SetWork值不一样才会发送connectChanged信号
    // 数据接收
    connect(cn, &CN::senddata, this, &MainWindow::onReceiveData);
    // 定时查询
    connect(&connectStatusTimer,&QTimer::timeout, this, &MainWindow::connectStatus);
    // 切换语言
    connect(ui->switchLanguage, &QPushButton::clicked, this, &MainWindow::switchLanguage);
    //
    connect(ui->pushButton_update, &QPushButton::clicked, this, &MainWindow::queryMakeInfo);
    connect(ui->pushButton_save, &QPushButton::clicked, this, &MainWindow::saveMakeInfo);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::dataInit()
{
    this->setFixedSize(QSize(510, 290));
    ui->comboBox_selectLanguage->setCurrentIndex(1);
    CN::switchLanguage(1);
    ui->retranslateUi(this);
}
/**
 * 语言切换按钮
 */
void MainWindow::switchLanguage()
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
void MainWindow::onConnect()
{
    QString targetIP = ui->lineEdit_targetIP->text();
    qint16 port = ui->lineEdit_targetPort->text().toUInt();
    // 连接
    if(!cn->isNetwork())
    {
        // 设置目标ip
        cn->setTargetIP_Port(targetIP,port);
        // 连接网络
        cn->onConnectNetwork();

        // 通信超时开启
        outTimer.start(TIMER_TIMEROUT);

        // 发送消息
        QByteArray msg;
        msg = Command::queryRunPar();
        cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);

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
void MainWindow::on_timeout()
{
    msgBox->confirmQBox(tr("通信提示"),tr("通信超时!"),tr("确认"));
    // 关闭定时器
    outTimer.stop();
}

/**
 * @brief 网路连接状态
 * @param value
 */
void MainWindow::onConnectChanged(bool value)
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
        ui->label_StatusLight->setStyleSheet("background:green;min-width:30px;min-height:30px;max-width:30px; max-height:30px;border-radius:15px;");
    }
    else {
        // 关闭定时查询
        connectStatusTimer.stop();
        ui->pushButton_netConnect->setText(tr("连接"));
        ui->label_StatusLight->setStyleSheet("background:grey;min-width:30px;min-height:30px;max-width:30px; max-height:30px;border-radius:15px;");
    }


}
/**
 * @brief 通信状态
 * @param
 */
void MainWindow::connectStatus()
{
    //qDebug()<<"connectStatus()";
    // 发送查询命令
    QByteArray msg;
    // 查询运行参数
    msg = Command::queryRunPar();
    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);

}
/**
 * @brief 生成DSN/蓝牙名称
 */
bool MainWindow::generateDSN()
{
    QString sn = ui->lineEdit_deviceSN->text();
    if (sn.length() != 17) {
        msgBox->confirmQBox(tr("输入错误提示"),tr("请输入17位SN码!"),tr("确认"));
        return false;
    }
    QString dsn = SC::snTodsn(sn);
    ui->lineEdit_deviceDSN->setText(dsn);
    // 截取sn设备型号和后四位生成蓝牙名称
    QString name = QString("%1 %2").arg(sn.mid(1, 6)).arg(sn.right(4));
    ui->lineEdit_buleToothName->setText(name);

    return true;
}
/**
 * @brief 读取制造信息
 */
void MainWindow::queryMakeInfo()
{
    // 通信超时开启
    outTimer.start(TIMER_TIMEROUT);
    // 发送查询命令
    QByteArray msg;
    // 查询制造信息
    msg = Command::queryMadeInfo();
    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);
    // 延时
    SC::Sleep(100);
    // 查询蓝牙
    msg = Command::queryBlueTooth();
    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);
}/**
 * @brief 保存制造信息
 */
void MainWindow::saveMakeInfo()
{
    if(generateDSN())
    {
        // 检测sn和dsn
        QString sn = ui->lineEdit_deviceSN->text().trimmed();
        QString dsn = ui->lineEdit_deviceDSN->text().trimmed();

        // 获取并设置信息
        SET_MAKEINFO_DATA Info;
        Info.keep();
        // SN
        if (!sn.isEmpty()) {
            QByteArray sn_bytes;
            QStringList list = sn.split("");
            list.removeFirst();
            list.removeLast();
            for (int i = 0; i < list.size(); i++) {
                sn_bytes.append(list.at(i));
            }
            memcpy(Info.SN, sn_bytes.data(), 17);
        }
        else {
            memset(Info.SN, 0xff, 17);
        }
        // DSN
        if (!dsn.isEmpty()) {
            QByteArray DSN_Uint = SC::hexStringtoByteArray(dsn);
            // 反序
            SC::__changeData(DSN_Uint);
            memcpy(Info.DSN, DSN_Uint.data(), 4);
        }
        else {
            memset(Info.DSN, 0xff, 4);
        }

        // 通信超时开启
        outTimer.start(TIMER_TIMEROUT);

        // 存储制造信息
        QByteArray data;
        data.resize(sizeof(Info));
        memcpy(data.data(), &Info, sizeof(Info));
        // 设置制造信息
        QByteArray msg;
        msg = Command::setMadeInfo(data);
        cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);
        // 蓝牙名称 20Byte
        QByteArray nameArray;
        nameArray.fill(0, 20);
        QString name = ui->lineEdit_buleToothName->text();
        SC::stringToByteArray(nameArray, name, 20);
        // 存储蓝牙名称
        msg = Command::setBlueTooth(nameArray);
        cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);

    }

}
/**
 * @brief 更新界面制造信息数据
 * @param data
 */
void MainWindow::updateMakeinfoData(UI_MAKEINFO_DATA data)
{
    QString str = nullptr;
    // SN
    str = data.SN.remove(QRegExp("\\0"));
    ui->lineEdit_deviceSN->setText(str.toUpper());
    // DSN
    str = data.DSN.simplified();
    ui->lineEdit_deviceDSN->setText(data.DSN.toUpper());
}
/**
 * @brief 数据接收
 *
 */
void MainWindow::onReceiveData(QByteArray data) // 第一次收到数据更新网卡
{
    //qDebug()<<"NetworkConnect收到data:"<<data.toHex();
    // 获取命令
    quint8 command = data.at(1);
    switch (command)
    {
    // 运行参数回传
    case DT::Backrun:
        break;
    // 制造信息参数回传
    case DT::Backmakeinfo:
    { // 块定义会干扰编译器解释switch语句，加{}
        // 通信超时timer
        if (outTimer.isActive()) {
            outTimer.stop();
        }
        BACK_MAKEINFO_DATA DATA;
        memcpy(&DATA, data.data() + 4, data.size() - 6);
        UI_MAKEINFO_DATA MAKEINFO_DATA = SC::getMakeinfoData(DATA);
        updateMakeinfoData(MAKEINFO_DATA);
        break;
    }
    // 蓝牙名称回传
    case DT::Backbluetooth:
    {
        QByteArray name;
        name.resize(20);
        memset(name.data(), 0, name.size());
        memcpy(name.data(), data.data() + 4, name.size());
        QString str = QString::fromLocal8Bit(name.data(), name.size()).simplified();
        // 设置界面蓝牙名称
        ui->lineEdit_buleToothName->setText(str);
        break;
    }
    // 操作结果回传
    case DT::Backresults:
    {
        // 通信超时timer
        if (outTimer.isActive()) {
            outTimer.stop();
        }
        // 操作命令
        quint8 order = data.at(4);
        // 回传结果
        quint8 result = data.at(5);
        if(order == DT::Setmakeinfo)
        {
            switch (result) {
            case 0x00:
                msgBox->confirmQBox(tr("操作提示"),tr("命令操作成功!"),tr("确定"));
                break;
            default:
                msgBox->confirmQBox(tr("操作提示"),tr("命令操作失败!"),tr("确定"));
                break;
            }
        }
        break;
    }
    default:
    {
    }
        break;
    }
}

