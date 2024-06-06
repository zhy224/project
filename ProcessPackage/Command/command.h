#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

#include <Public/cn.h>
#include "Public/SD.h"

class Command : public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = nullptr);

public:
    // 查询网络
    static QByteArray queryNetwork();
    // 查询可用焊接包
    static QByteArray query_avail_weld();
    // 查询客户焊接包
    static QByteArray query_cus_weld(PACKAGE_NUMBER package);
    // 查询当前焊接包(网络连接)
    static QByteArray query_curr_weld();
    // 设置当前焊接包
    static QByteArray set_curr_weld(QByteArray data);
    // 存储当前焊接包
    static QByteArray save_curr_weld();

signals:

};

#endif // COMMAND_H
