#include "mcb_test.h"
#include "ui_mcb_test.h"
#include "mainwindow.h"

#if _MSC_VER>=1600
#pragma execution_character_set("utf-8");
#endif

MCB_Test::MCB_Test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MCB_Test)
{
    ui->setupUi(this);


    // 通信类
    cn = CN::GetInstance();//new CN;
    // 初始化
    MCB_uiInit();

    //

    //connect(networkConnect, &networkConnect::senddata, this, &MCB_Test::main_ReceiveData,Qt::AutoConnection);
    // 开始测试
    connect(ui->pushButton_startTest, &QPushButton::clicked, this, &MCB_Test::startTest,Qt::AutoConnection);
    // 获取文件路径
    connect(ui->toolButton_selectSaveDir, &QToolButton::clicked, this, &MCB_Test::getSaveDir,Qt::AutoConnection);
    // 保存文件
    connect(ui->pushButton_Save, &QPushButton::clicked, this, &MCB_Test::saveFile,Qt::AutoConnection);
    // 数据接收
    connect(cn, &CN::senddata, this, &MCB_Test::main_ReceiveData);



}

MCB_Test::~MCB_Test()
{
    delete ui;
}
/**
 * @brief 初始化
 *
 */
void MCB_Test::MCB_uiInit()
{
    // 获取值控件
    QList<QPushButton*> buttons = ui->widget_testBool->findChildren<QPushButton*>();
    for (QPushButton* button : buttons)
    {
        button->setEnabled(false);
    }
    ui->pushButton_1->setEnabled(true);
    ui->label_Sampling_v->setText("0");
    ui->label_Voltage_v->setText("0");
    ui->label_LDTemp_v->setText("0");
    ui->label_CPSTemp_v->setText("0");
    ui->label_PD_v->setText("0");
    ui->label_DAC_v->setText("0");
    ui->label_PWM_v->setText("0");
    ui->label_Duty_v->setText("0");
    // 更新数据
    update_ParValue();

}
/**
 * 更新键值
 *
 */
void MCB_Test::update_ParValue()
{
    //qDebug()<<"saveConfigFile";
    // 清除键值对
    Par_Test_Value.clear();
    // 获取字段控件
    QList<QLabel*> labels = ui->widget_testBool->findChildren<QLabel*>();
    //qDebug() << "label:"<<labels.size();
    // 获取值控件
    QList<QPushButton*> buttons = ui->widget_testBool->findChildren<QPushButton*>();
    //qDebug() << "button:"<<buttons.size();
    // 转换值控件
    QStringList boolValue;
    for (QPushButton* button : buttons)
    {
        if(button->isEnabled())
        {
            boolValue<<"合格";
        }
        else
        {
            boolValue<<"不合格";
        }
    }
    // 存入键值对
    if(labels.size() == buttons.size())
    {
        for(int i=0;i<labels.size();i++)
        {
            Par_Test_Value[labels.at(i)->text()]=boolValue.at(i);
        }
    }
    // 添加键值对
    Par_Test_Value[ui->label_Sample->text()] = ui->label_Sampling_v->text();
    Par_Test_Value[ui->label_Voltage->text()] = ui->label_Voltage_v->text();
    Par_Test_Value[ui->label_LDTemp->text()] = ui->label_LDTemp_v->text();
    Par_Test_Value[ui->label_CPSTemp->text()] = ui->label_CPSTemp_v->text();
    Par_Test_Value[ui->label_PD->text()] = ui->label_PD_v->text();
    Par_Test_Value[ui->label_DAC->text()] = ui->label_DAC_v->text();
    Par_Test_Value[ui->label_PWM->text()] = ui->label_PWM_v->text();
    Par_Test_Value[ui->label_Duty->text()] = ui->label_Duty_v->text();

    qDebug() << "Par_Test_Value:"<<Par_Test_Value.size()<<Par_Test_Value;


}
/**
 * @brief 数据接收
 * @param data
 */
void MCB_Test::main_ReceiveData(QByteArray data)
{
    qDebug()<<"MCB_Test收到data:";
    qDebug()<<data;
    qDebug()<<data.toHex();

}
/**
 * 开始测试
 *
 */
void MCB_Test::startTest()
{
    qDebug()<<"STARTTEST:"<<cn->isNetwork();
    if(!cn->isNetwork())
    {
        QMessageBox::about(this,"通信提示","通信未连接");
        return;
    }
    // 发送开始测试命令
    QByteArray cmd = Command::startTest_CMD();
    // 发送
    cn->sendMessage(cmd,cn->sTargetIP,cn->sTargetPort);

}


/**
 * 获取保存目录
 *
 */
void MCB_Test::getSaveDir()
{
    //QString openFilePath = QFileDialog::getExistingDirectory(this,"选择保存文件夹","F:");
    QString openFilePath = QFileDialog::getExistingDirectory(this, "选择一个目录","F:", QFileDialog::ShowDirsOnly);
    ui->lineEdit_saveFileDir->setText(openFilePath);

}
/**
 * 保存文件
 *
 */
void MCB_Test::saveFile()
{
    // 更新测试数据
    update_ParValue();

    QString saveFileDir = ui->lineEdit_saveFileDir->text().trimmed();
    if(saveFileDir.isEmpty())
    {
        QMessageBox::warning(this,"保存文件警告","保存路径为空！");
        return;
    }
    QFile m_file_save;
    m_file_save.setFileName(saveFileDir+"/"+"主控板测试结果"+".xlsx"); //
    if(SC::IsFileExist(m_file_save.fileName()))
    {
        MsgBox *msgBox = new MsgBox(this);
        msgBox->setTitle("写入文件提示");
        msgBox->setText("文件已存在，是否覆盖？");
        msgBox->setOK_Button("是");
        msgBox->setCANCEL_Button("否");
        int res = msgBox->exec();
        switch(res)
        {
            case QDialog::Accepted:	// 确定
                //m_file_save.open(QIODevice::WriteOnly|QIODevice::Truncate); //重新创建文件
                //删除文件
                if(!QFile::remove(m_file_save.fileName()))
                {
                    QMessageBox::warning(this,"覆盖文件警告","文件可能已打开！覆盖失败");
                    return;
                }
                break;
            case QDialog::Rejected:	// 取消
                return;
                break;
        }
        m_file_save.close();

    }
    qDebug()<<"wenjian:"<<SC::IsFileExist(m_file_save.fileName());
    //return;
    //QTextStream out(&m_file_save);

    exc = new QExcel(this); //新建对象
    qDebug()<<"m_file_save.fileName():"<<m_file_save.fileName();
    exc->createFile(m_file_save.fileName().replace("/", "\\")); // 创建excel文件
    exc->setColumnWidth(1,20);
    QList<QString> keyList = Par_Test_Value.keys();
    //exc->setCellString(1,1,QString::number(1));
    for(int i=1;i<=keyList.size();i++)
    {
        //qDebug()<<keyList[i];
        exc->setCellString(i,1,keyList[i-1]);
        exc->setCellTextCenter(i,1);
        exc->setCellString(i,2,Par_Test_Value.value(keyList[i-1]));
        exc->setCellTextCenter(i,2);
        if(Par_Test_Value.value(keyList[i-1]) == "不合格")
        {
            exc->setCellBackground(i,2,Qt::red);
            exc->setCellFontColor(i,2,Qt::white);
        }
        else if(Par_Test_Value.value(keyList[i-1]) == "合格")
        {
            exc->setCellBackground(i,2,Qt::green);
            exc->setCellFontColor(i,2,Qt::black);
        }
    }
    exc->save();
    exc->quit();
    // 关闭文件
    m_file_save.close();
    //
    QMessageBox::about(this,"保存文件提示","保存完成");
}
