#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

#include <Public/cn.h>

class Command : public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = nullptr);

public:
    // 查询网络连接状态(主控板)
    static QByteArray query_main_CMD();
    // 查询网络连接状态(综合板)
    static QByteArray query_syn_CMD();
    // 开始测试
    static QByteArray startTest_CMD();

signals:

};

#endif // COMMAND_H
