#ifndef MCB_TEST_H
#define MCB_TEST_H

#include <QWidget>
#include <QVector>

#include "Public/cn.h"
#include "QExcel/qexcel.h"
#include "Command/command.h"
//#include "networkconnect.h"

namespace Ui {
class MCB_Test;
}

class MCB_Test : public QWidget
{
    Q_OBJECT

public:
    explicit MCB_Test(QWidget *parent = nullptr);
    ~MCB_Test();

private:
    Ui::MCB_Test *ui;

private:
    // 通信类
    CN *cn;
    //
    //NetworkConnect *networkConnect;
    // 文件类
    QExcel *exc;
    // 测试参数
    QMap<QString,QString> Par_Test_Value;
    //

signals:

private slots:
    // 数据接收
    void main_ReceiveData(QByteArray);
    // 开始测试
    void startTest();
    // 初始化
    void MCB_uiInit();
    // 更新测试值
    void update_ParValue();
    // 获取保存文件目录
    void getSaveDir();
    // 保存文件
    void saveFile();

};

#endif // MCB_TEST_H
