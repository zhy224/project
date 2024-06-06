#ifndef PROPARSET_H
#define PROPARSET_H

#include <QWidget>
#include <QListView>

#include "Public/cn.h"

namespace Ui {
class proParSet;
}

class proParSet : public QWidget
{
    Q_OBJECT

public:
    explicit proParSet(QWidget *parent = nullptr);
    ~proParSet();

private:
    Ui::proParSet *ui;
private:
    // 通信类
    CN* cn;
    // 通信超时timer
    QTimer outTimer;
    // 弹出框
    MsgBox *msgBox;
    // 焊接包map key=焊接包组编号 values=焊接包名编号list
    //QMap<int, int> allPackageMap;
    // 焊接包组名map
    //QMap<int, QString> groupNameMap;
    // 用户焊接包包名map
    //QMap<int, QString> userPackageNameMap;
    // 设备焊接包包名map
    //QMap<int, QString> devicePackageNameMap;
    // 焊接模式Map
    QMap<int, QString> modeMap;
    // 文件参数数据
    QList<QString> currPackageDataList;
    //
    QStringList packageHeadList = {};
    //组/包编号
    quint8 groupNumber = 0;
    quint8 packageNumber = 0;

private:
    QString AppTranslate(const char* cchar);
    QString saveFileDialog();
    // 数据初始化
    void dataInit();
    // 更新可用焊接包数据
    //void updateAvailablePackage(QByteArray data);
    // 更新显示当前焊接包参数
    void updateCurrentPackage(UI_PACKAGE_DATA info);


private slots:
    // 数据接收
    void onReceiveData(QByteArray data);
    // 通信状态改变
    void onConnectChanged(bool value);
    // 通信超时槽函数
    void onTimeout();

    // 查询可用焊接包
    //void queryAvailablePackage();
    // 查询当前焊接包
    void queryCurrentPackage();
    // 设置当前焊接包
    void setCurrentPackage();
    // 存储当前焊接包
    void saveCurrentPackage();
    // 从文件导入参数
    void fromFileImport();
    // 导出参数至文件
    void exportToFile();
    //

protected:
    void changeEvent(QEvent *) override;
    void showEvent(QShowEvent *event) override;
};

#endif // PROPARSET_H
