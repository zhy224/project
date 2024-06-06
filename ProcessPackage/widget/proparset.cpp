#include "proparset.h"
#include "ui_proparset.h"

#pragma execution_character_set("utf-8")

static bool updateCurr = true;

proParSet::proParSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::proParSet)
{
    ui->setupUi(this);

    // 通信类
    cn = CN::GetInstance();
    // 初始化
    dataInit();

    // 数据接收
    connect(cn, &CN::senddata, this, &proParSet::onReceiveData);
    // 通信状态
    connect(cn, &CN::connectChanged, this, &proParSet::onConnectChanged);
    // 通信超时
    connect(&outTimer, &QTimer::timeout, this, &proParSet::onTimeout);
    // 参数操作
    //connect(ui->updateAvailPack, &QPushButton::clicked, this, &proParSet::queryAvailablePackage);
    connect(ui->updateCurrPack, &QPushButton::clicked, this, &proParSet::queryCurrentPackage);
    connect(ui->setCurrPack, &QPushButton::clicked, this, &proParSet::setCurrentPackage);
    connect(ui->saveCurrPack, &QPushButton::clicked, this, &proParSet::saveCurrentPackage);
    // 文件操作
    connect(ui->selectFilePath, &QToolButton::clicked, this, &proParSet::fromFileImport);
    connect(ui->pushButton_save, &QPushButton::clicked, this, &proParSet::exportToFile);


}

proParSet::~proParSet()
{
    delete ui;
}
QString proParSet::AppTranslate(const char* cchar)
{
    const char* context = NULL;
    context = QT_TR_NOOP(cchar);
    return tr(context);
}
QString proParSet::saveFileDialog() {
    QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                   "Save File",
                                                   "",
                                                   "Text Files (*.csv)");
    return fileName;
}
/**
 * 重写界面事件（语言改变）
 */
void proParSet::changeEvent(QEvent *event)
{
    //qDebug()<<"执行了改变事件";
    if(event->type() == QEvent::LanguageChange)
    {
        dataInit();
        this->ui->retranslateUi(this);
    }
}
/**
 * 重写界面事件（界面显示）
 */
void proParSet::showEvent(QShowEvent *event)
{
    //qDebug()<<"执行了界面显示事件";
    Q_UNUSED(event)
    if (!cn->isNetwork()) {
        return;
    }
    // 查询当前焊接包
    if(updateCurr)
    {
        queryCurrentPackage();
        updateCurr = false;
    }
}

/**
 * @brief 数据初始化
 */
void proParSet::dataInit()
{
    // 存文件字段
    packageHeadList.clear();
    packageHeadList.append({tr("激光功率"),tr("摆动频率"),tr("摆动宽度"), tr("开光气体延迟"),tr("关光气体延迟"),tr("送丝速度"), tr("送丝延时"),tr("回丝长度"),tr("补丝长度"), \
                            tr("焊接方式"),tr("点焊次数"),tr("点焊出光时间"),tr("点焊出光间隔时间"), tr("激光占空比"), tr("激光频率"),tr("功率缓升时间"), tr("功率缓降时间"),\
                            tr("预热时长"),tr("预热功率"),tr("预热缓升时间"),tr("预热缓降时间")});
    // 焊接模式
    if(modeMap.size()>0)
    {
        modeMap.clear();
    }
    modeMap.insert(0, tr("连续"));
    modeMap.insert(1, tr("脉冲"));
    modeMap.insert(2, tr("点焊"));
    modeMap.insert(3, tr("连续点焊"));
    modeMap.insert(4, tr("连续点焊(开启预热)"));
    modeMap.insert(5, tr("连续(开启预热)"));
    ui->combox_mode->clear();
    ui->combox_mode->insertItem(0, modeMap.value(0));
    ui->combox_mode->insertItem(1, modeMap.value(1));
    ui->combox_mode->insertItem(2, modeMap.value(2));
    ui->combox_mode->insertItem(3, modeMap.value(3));
    ui->combox_mode->insertItem(4, modeMap.value(4));
    ui->combox_mode->insertItem(5, modeMap.value(5));
}
/**
 * @brief 通信超时槽函数
 */
void proParSet::onTimeout()
{
    msgBox->confirmQBox(tr("通信提示"),tr("通信超时！(操作命令)"),tr("确定"));
    outTimer.stop();
}
/**
 * @brief 通信状态改变
 * @param value
 */
void proParSet::onConnectChanged(bool value)
{
}
/**
 * @brief 更新可用焊接包数据
 * @param data
 */
/*
void proParSet::updateAvailablePackage(QByteArray data)
{
    // 清空焊接包组名
    ui->combox_groupName->clear();
    // 清空焊接包包名
    ui->combox_packageName->clear();

    // 清空焊接包map
    allPackageMap.clear();

    // 解析数据
    int i = 4;
    while (i < data.size() - 2) {
        // 组编号
        quint8 groupNumber = data.at(i);
        // 包编号
        quint8 packageNumber = data.at(i + 1);
        // 插入map
        allPackageMap.insertMulti(groupNumber, packageNumber);
        i += 2;
    }
    // 添加组名称
    QList<int> groupList = allPackageMap.keys();
    // 去重
    groupList = SC::getIntListDistinct(groupList);
    // 插入焊接包组名下拉框
    for (int i = 0; i < groupList.size(); i++) {
        //QString name = groupNameMap.value(groupList.at(i));
        ui->combox_groupName->insertItem(groupList.at(i), groupNameMap.value(groupList.at(i)));
    }
    // 清空界面
    QList<QLineEdit*> lineEdits1 = ui->group_welding->findChildren<QLineEdit*>();
    for (QLineEdit* lineEdit : lineEdits1)
    {
        lineEdit->clear();
    }
    QList<QLineEdit*> lineEdits2 = ui->group_mode->findChildren<QLineEdit*>();
    for (QLineEdit* lineEdit : lineEdits2)
    {
        lineEdit->clear();
    }
}
*/
/**
 * @brief 更新显示当前焊接包参数
 * @param info
 */
void proParSet::updateCurrentPackage(UI_PACKAGE_DATA info)
{
    qDebug()<<"updateCurrentPackage";
    // 焊接包组名
    //ui->combox_groupName->setCurrentIndex(info.GroupNameNumber);
    //ui->combox_groupName->setCurrentText(groupNameMap.value(info.GroupNameNumber) );
    // 焊接包包名
    //ui->combox_packageName->setCurrentText(userPackageNameMap.value(info.PackageNameNumber));
    groupNumber = info.GroupNameNumber;
    packageNumber = info.PackageNameNumber;
    // 激光功率
    ui->edit_welding_01->setText(QString::number(info.LaserPower * 0.1));
    // 摆动频率
    ui->edit_welding_02->setText(QString::number(info.SwingFrequence));
    // 摆动宽度
    ui->edit_welding_03->setText(QString::number(info.SwingAmplitude * 0.1, 'f', 1));
    // 开光气体延时
    ui->edit_welding_04->setText(QString::number(info.StartDelay));
    // 关光气体延时
    ui->edit_welding_05->setText(QString::number(info.StopDelay));
    // 送丝速度
    ui->edit_welding_06->setText(QString::number(info.wirefeedSpeed));
    // 送丝延时
    ui->edit_welding_07->setText(QString::number(info.wirefeedDelay));
    // 回丝长度
    ui->edit_welding_08->setText(QString::number(info.backwireLength));
    // 补丝长度
    ui->edit_welding_09->setText(QString::number(info.wirefillingLength));

    // 焊接方式
    ui->combox_mode->setCurrentText(modeMap.value(info.LightHandMode));
    // 点焊次数
    ui->edit_mode_01->setText(QString::number(info.SpotWeldingTimes));
    // 点焊时间
    ui->edit_mode_02->setText(QString::number(info.SpotWeldingTime));
    // 点焊间隔
    ui->edit_mode_03->setText(QString::number(info.SpotWeldinginterval));
    // 脉冲占空比
    ui->edit_mode_04->setText(QString::number(info.PulseDuty));
    // 脉冲频率
    ui->edit_mode_05->setText(QString::number(info.PulseFrequence));
    // 上升时间
    ui->edit_mode_06->setText(QString::number(info.RiseTime));
    // 下降时间
    ui->edit_mode_07->setText(QString::number(info.DescentTime));
    // 预热时长
    ui->edit_mode_08->setText(QString::number(info.preheatTime));
    // 预热功率
    ui->edit_mode_09->setText(QString::number(info.preheatPower * 0.1, 'f', 1));
    // 预热缓升时间
    ui->edit_mode_10->setText(QString::number(info.preheatRiseTime));
    // 预热缓降时间
    ui->edit_mode_11->setText(QString::number(info.preheatDescentTime));
}
/**
 * @brief 工艺参数设置按钮
 */
void proParSet::setCurrentPackage()
{
    if (!cn->isNetwork()) {
            msgBox->confirmQBox(tr("通信提示"),tr("通信未连接!"),tr("确定"));
            return;
        }
    bool isSet = msgBox->askQBox("工艺参数设置","是否设置当前工艺参数?","是","否");
    if (!isSet) {
        return;
    }
    // 设置当前焊接包参数结构体(清零)
    SET_PACKAGE_DATA info;
    info.keep();

    // 焊接包组名编号
    info.GroupNameNumber = groupNumber;
    // 焊接包包名编号
    info.PackageNameNumber = packageNumber;
    // 激光功率
    info.LaserPower = SC::getLineEditUint16(ui->edit_welding_01, 10);
    // 摆动频率
    info.SwingFrequence = SC::getLineEditUint16(ui->edit_welding_02, 1);
    // 摆动宽度
    info.SwingAmplitude = SC::getLineEditUint16(ui->edit_welding_03, 10);
    // 开光气体延时
    info.StartDelay = SC::getLineEditUint16(ui->edit_welding_04, 1);
    // 关光气体延时
    info.StopDelay = SC::getLineEditUint16(ui->edit_welding_05, 1);
    // 送丝速度
    info.wirefeedSpeed = SC::getLineEditUint16(ui->edit_welding_06, 1);
    // 送丝延时
    info.wirefeedDelay = SC::getLineEditUint16(ui->edit_welding_07, 1);
    // 回丝长度
    info.backwireLength = SC::getLineEditUint16(ui->edit_welding_08, 1);
    // 补丝长度
    info.wirefillingLength = SC::getLineEditUint16(ui->edit_welding_09, 1);
    // 焊接模式
    info.LightHandMode = SC::__changeuint16(modeMap.key(ui->combox_mode->currentText()));
    // 点焊次数
    info.SpotWeldingTimes = SC::getLineEditUint16(ui->edit_mode_01, 1);
    // 点焊出光时间
    info.SpotWeldingTime = SC::getLineEditUint16(ui->edit_mode_02, 1);
    // 点焊间隔时间
    info.SpotWeldinginterval = SC::getLineEditUint16(ui->edit_mode_03, 1);
    // 脉冲占空比
    info.PulseDuty = SC::getLineEditUint16(ui->edit_mode_04, 1);
    // 脉冲频率
    info.PulseFrequence = SC::getLineEditUint16(ui->edit_mode_05, 1);
    // 功率缓升时间
    info.RiseTime = SC::getLineEditUint16(ui->edit_mode_06, 1);
    // 功率缓降时间
    info.DescentTime = SC::getLineEditUint16(ui->edit_mode_07, 1);
    // 预热时长
    info.preheatTime = SC::getLineEditUint16(ui->edit_mode_08, 1);
    // 预热功率
    info.preheatPower = SC::getLineEditUint16(ui->edit_mode_09, 10);
    // 预热缓升时间
    info.preheatRiseTime = SC::getLineEditUint16(ui->edit_mode_10, 1);
    // 预热缓降时间
    info.preheatDescentTime = SC::getLineEditUint16(ui->edit_mode_11, 1);

    // 设置当前焊接包
    QByteArray data;
    data.resize(sizeof(info));
    memcpy(data.data(), &info, sizeof(info));
    //qDebug()<<"data.size:"<<data.size();
    // 超时判断
    outTimer.start(TIMER_TIMEROUT);
    // 设置当前焊接包
    QByteArray msg = Command::set_curr_weld(data);
    cn->sendMessage(msg, cn->sTargetIP, cn->sTargetPort);

}
/**
 * @brief 工艺参数存储按钮
 */
void proParSet::saveCurrentPackage()
{
    if (!this->cn->isNetwork()) {
        msgBox->confirmQBox(tr("通信提示"),tr("通信未连接!"),tr("确定"));
        return;
    }
    bool isSet = msgBox->askQBox(tr("工艺参数存储"),tr("是否存储当前工艺参数?"),tr("是"),tr("否"));
    if (!isSet) {
        return;
    }
    // 超时判断
    outTimer.start(TIMER_TIMEROUT);
    // 当前焊接包参数存储
    QByteArray msg = Command::save_curr_weld();
    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);

}
/**
 * @brief 数据接收
 * @param data
 */
void proParSet::onReceiveData(QByteArray data)
{

    // 命令符
    quint8 command = data.at(1);
    //qDebug()<<"command:"<<command;
    switch (command)
    {
        // 焊接包参数
    case DT::Backpackagedata: {
        //qDebug()<<"Backpackagedata curr data:"<<data.toHex();
        // 通信超时timer
        if (outTimer.isActive()) {
            outTimer.stop();
        }
        // 解析焊接包参数
        UI_PACKAGE_DATA info = SC::getPackageData(data);
        updateCurrentPackage(info);
    }
        break;
        // 操作结果回传
    case DT::Backresults: {
        // 通信超时timer
        if (outTimer.isActive()) {
            outTimer.stop();
        }
        // 操作命令
        //quint8 order = data.at(4);
        // 操作结果
        quint8 result = data.at(5);
        switch (result) {
        case 0x00:
            msgBox->confirmQBox(tr("操作提示"),tr("命令操作成功!"),tr("确定"));
            break;
        case 0x01:
            msgBox->confirmQBox(tr("操作提示"),tr("命令格式错误!"),tr("确定"));
            break;
        case 0x02:
            msgBox->confirmQBox(tr("操作提示"),tr("操作条件不符!"),tr("确定"));
            break;
        case 0x03:
            msgBox->confirmQBox(tr("操作提示"),tr("命令执行失败!"),tr("确定"));
            break;
        case 0x04:
            msgBox->confirmQBox(tr("操作提示"),tr("当前状态不可用!"),tr("确定"));
            break;
        case 0x05:
            msgBox->confirmQBox(tr("操作提示"),tr("仅待机状态可用!"),tr("确定"));
            break;
        case 0x06:
            msgBox->confirmQBox(tr("操作提示"),tr("仅故障状态可用!"),tr("确定"));
            break;
        case 0x07:
            msgBox->confirmQBox(tr("操作提示"),tr("解锁次数已耗尽!"),tr("确定"));
            break;
        case 0x08:
            msgBox->confirmQBox(tr("操作提示"),tr("系统已休眠!"),tr("确定"));
            break;
        case 0x09:
            msgBox->confirmQBox(tr("操作提示"),tr("焊接包编号错误!"),tr("确定"));
            break;
        default:
            msgBox->confirmQBox(tr("操作提示"),tr("其他错误!"),tr("确定"));
            break;
        }

    }
        break;
    default:
        break;
    }
}
/**
 * @brief 查询可用焊接包编号
 */
/*
void proParSet::queryAvailablePackage()
{
    if (!this->cn->isNetwork()) {
        msgBox->confirmQBox(tr("通信提示"),tr("通信未连接!"),tr("确定"));
        return;
    }
    // 超时判断
    outTimer.start(TIMER_TIMEROUT);
    // 查询可用焊接包
    QByteArray msg = Command::query_avail_weld();
    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);

}
*/
/**
 * @brief 查询当前焊接包参数
 */
void proParSet::queryCurrentPackage()
{
    if (!this->cn->isNetwork()) {
        msgBox->confirmQBox(tr("通信提示"),tr("通信未连接!"),tr("确定"));
        return;
    }
    // 超时判断
    outTimer.start(TIMER_TIMEROUT);
    // 查询当前焊接包
    QByteArray msg = Command::query_curr_weld();
    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);

}
/**
 * @brief 焊接包组名下拉框(根据编号查询)
 * @param arg1
 */
/*
void proParSet::on_combox_groupName_activated(const QString &arg1)
{
    qDebug()<<"执行了："<<arg1;
    // 清空焊接包包名
    ui->combox_packageName->clear();
    // 焊接包组名编号
    int index = groupNameMap.key(arg1);
    // 获取当前焊接包组下的焊接包编号链表 反序
    QList<int> keys = allPackageMap.values(index);
    std::reverse(keys.begin(), keys.end());
    for (int i = 0; i < keys.size(); i++) {
        // 用户焊接包
        if(index == 0) {
            ui->combox_packageName->addItem(userPackageNameMap.value(keys.at(i)));
        }
        else {
            ui->combox_packageName->addItem(devicePackageNameMap.value(keys.at(i)));
        }
    }
    // 查询当前焊接包参数
    PACKAGE_NUMBER package;
    package.GroupNumber = index;
    if(index == 0) {
        package.PackageNumber = userPackageNameMap.key(ui->combox_packageName->currentText());
    }
    else {
        package.PackageNumber = devicePackageNameMap.key(ui->combox_packageName->currentText());
    }
    QByteArray msg = Command::query_cus_weld(package);
    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);
}
*/
/**
 * @brief 焊接包包名下拉框
 * @param arg1
 */
/*
void proParSet::on_combox_packageName_activated(const QString &arg1)
{
    Q_UNUSED(arg1)
    int index = groupNameMap.key(ui->combox_groupName->currentText());
    // 查询当前焊接包参数
    PACKAGE_NUMBER package;
    package.GroupNumber = index;
    if(index == 0) {
        package.PackageNumber = userPackageNameMap.key(ui->combox_packageName->currentText());
    }
    else {
        package.PackageNumber = devicePackageNameMap.key(ui->combox_packageName->currentText());
    }
    QByteArray msg = Command::query_cus_weld(package);
    cn->sendMessage(msg,cn->sTargetIP,cn->sTargetPort);
}
void proParSet::importFilePath()
{
    QString openFilePath = QFileDialog::getOpenFileName(this,tr("选择参数文件"),"F:",tr("工艺参数文件(*.csv)"));
    ui->lineEdit_importFilePath->setText(openFilePath);
}
void proParSet::saveFileDir()
{
    QString saveFileDir = QFileDialog::getExistingDirectory();
    ui->lineEdit_saveFileDir->setText(saveFileDir);
}
*/
/**
 * @brief 导入参数
 *
 */
void proParSet::fromFileImport()
{
    //qDebug()<<"packageHeadList:"<<packageHeadList.length()<<packageHeadList;
    currPackageDataList.clear();


    QString importFilePath = QFileDialog::getOpenFileName(nullptr, "Open File",QDir::homePath(), "Text Files (*.csv)");
    QFile m_file_open;
    m_file_open.setFileName(importFilePath);
    QTextStream read(&m_file_open);
    if(importFilePath.isEmpty())
    {
        msgBox->confirmQBox(tr("导入文件提示"),tr("文件为空！"),tr("确认"));
        return;
    }
    if(m_file_open.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QStringList FileData = read.readAll().split("\n",QString::SkipEmptyParts);//每行以\N区分
        qDebug()<<"FileData:"<<FileData.count();
        for(int i=0;i<FileData.count();i++)//遍历行
        {
            if(i == (FileData.count()-1)) // 只导最后一行
            {
                QStringList strCol = FileData.at(i).split(",",QString::SkipEmptyParts); //第i行数据
                currPackageDataList<<strCol;
            }
        }
        qDebug()<<"currPackageDataList:"<<currPackageDataList.length()<<currPackageDataList;
        if(currPackageDataList.length() != packageHeadList.length())
        {
            msgBox->confirmQBox("导入文件提示","数据错误！","确认");
            return;
        }
        // 激光功率
        ui->edit_welding_01->setText(currPackageDataList.at(0));
        // 摆动频率
        ui->edit_welding_02->setText(currPackageDataList.at(1));
        // 摆动宽度
        ui->edit_welding_03->setText(currPackageDataList.at(2));
        // 开光气体延时
        ui->edit_welding_04->setText(currPackageDataList.at(3));
        // 关光气体延时
        ui->edit_welding_05->setText(currPackageDataList.at(4));
        // 送丝速度
        ui->edit_welding_06->setText(currPackageDataList.at(5));
        // 送丝延时
        ui->edit_welding_07->setText(currPackageDataList.at(6));
        // 回丝长度
        ui->edit_welding_08->setText(currPackageDataList.at(7));
        // 补丝长度
        ui->edit_welding_09->setText(currPackageDataList.at(8));

        // 焊接方式
        ui->combox_mode->setCurrentText(currPackageDataList.at(9));
        // 点焊次数
        ui->edit_mode_01->setText(currPackageDataList.at(10));
        // 点焊时间
        ui->edit_mode_02->setText(currPackageDataList.at(11));
        // 点焊间隔
        ui->edit_mode_03->setText(currPackageDataList.at(12));
        // 脉冲占空比
        ui->edit_mode_04->setText(currPackageDataList.at(13));
        // 脉冲频率
        ui->edit_mode_05->setText(currPackageDataList.at(14));
        // 上升时间
        ui->edit_mode_06->setText(currPackageDataList.at(15));
        // 下降时间
        ui->edit_mode_07->setText(currPackageDataList.at(16));
        // 预热时长
        ui->edit_mode_08->setText(currPackageDataList.at(17));
        // 预热功率
        ui->edit_mode_09->setText(currPackageDataList.at(18));
        // 预热缓升时间
        ui->edit_mode_10->setText(currPackageDataList.at(19));
        // 预热缓降时间
        ui->edit_mode_11->setText(currPackageDataList.at(20));
        //
        msgBox->confirmQBox(tr("导入数据提示"),tr("导入成功"),tr("确认"));
    }
    else
    {
        msgBox->confirmQBox(tr("打开文件提示"),tr("文件打开失败！"),tr("确认"));
    }

}
/**
 * @brief 导出参数
 *
 */
void proParSet::exportToFile()
{
    currPackageDataList.clear();

    currPackageDataList.append(ui->edit_welding_01->text());
    currPackageDataList.append(ui->edit_welding_02->text());
    currPackageDataList.append(ui->edit_welding_03->text());
    currPackageDataList.append(ui->edit_welding_04->text());
    currPackageDataList.append(ui->edit_welding_05->text());
    currPackageDataList.append(ui->edit_welding_06->text());
    currPackageDataList.append(ui->edit_welding_07->text());
    currPackageDataList.append(ui->edit_welding_08->text());
    currPackageDataList.append(ui->edit_welding_09->text());
    currPackageDataList.append(ui->combox_mode->currentText());
    currPackageDataList.append(ui->edit_mode_01->text());
    currPackageDataList.append(ui->edit_mode_02->text());
    currPackageDataList.append(ui->edit_mode_03->text());
    currPackageDataList.append(ui->edit_mode_04->text());
    currPackageDataList.append(ui->edit_mode_05->text());
    currPackageDataList.append(ui->edit_mode_06->text());
    currPackageDataList.append(ui->edit_mode_07->text());
    currPackageDataList.append(ui->edit_mode_08->text());
    currPackageDataList.append(ui->edit_mode_09->text());
    currPackageDataList.append(ui->edit_mode_10->text());
    currPackageDataList.append(ui->edit_mode_11->text());

    if(currPackageDataList.length() == packageHeadList.length())
    {
        QString fileName = saveFileDialog();
        if (fileName.isEmpty())
        {
            msgBox->confirmQBox(tr("打开文件警告"),tr("文件为空！"),tr("确认"));
            return;
        }
        QFile m_file_save;
        m_file_save.setFileName(fileName);
        QFile file(m_file_save.fileName());
        /*
        if(file.exists())
        {
            int res = msgBox->askQBox(tr("写入文件提示"),tr("文件已存在，是否覆盖？"),tr("是"),tr("否"));
            switch(res)
            {
                case 1:	// 确定
                    //删除文件
                    if(!QFile::remove(m_file_save.fileName()))
                    {
                        msgBox->confirmQBox(tr("覆盖文件警告"),tr("文件可能已打开！覆盖失败"),tr("确认"));
                        return;
                    }
                    break;
                case 2:	// 取消
                    break;
            }
        }
        */
        //
        if(!m_file_save.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
        {
            msgBox->confirmQBox(tr("打开文件提示"),tr("文件打开失败!"),tr("确认"));
            return;
        }
        else
        {
            qDebug()<<"文件打开成功";
            QTextStream out(&m_file_save);
            //QStringList FileData = out.readAll().split("\n",QString::SkipEmptyParts);//每行以\N区分
            //qDebug()<<"FileData:"<<FileData.count();
            // 写入字段
            for(int i=0;i<packageHeadList.size();i++)
            {
                //qDebug()<<keyList[i];
                out<<packageHeadList[i]<<",";
            }
            out<<"\n";

            // 写入数据
            for(int i=0;i<currPackageDataList.size();i++)
            {
                //qDebug()<<keyList[i];
                out<<currPackageDataList[i]<<",";
            }
            out<<"\n";
        }
        m_file_save.close();
        msgBox->confirmQBox(tr("导出文件提示"),tr("导出成功"),tr("确认"));
    }

}
