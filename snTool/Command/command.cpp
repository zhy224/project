#include "command.h"


Command::Command(QObject *parent) : QObject(parent)
{

}
/**
 * 查询运行参数.
 *
 */
QByteArray Command::queryRunPar()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::Header);
    // 命令
    cmd.append(DT::Queryrun);
    // 数据长度
    quint16 size = 1;
    SC::byteArrayFrom(cmd, size);
    // 数据
    cmd.append(0x0F);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::End);

    return cmd;
}
/**
 * 查询蓝牙名称.
 *
 */
QByteArray Command::queryBlueTooth()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::Header);
    // 命令
    cmd.append(DT::Querybluetooth);
    // 数据长度
    quint16 size = 1;
    SC::byteArrayFrom(cmd, size);
    // 数据
    cmd.append(0x0F);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::End);

    qDebug()<<"queryBlueTooth:"<<cmd.toHex();

    return cmd;
}
/**
 * 设置蓝牙名称.
 *
 */
QByteArray Command::setBlueTooth(QByteArray data)
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::Header);
    // 命令
    cmd.append(DT::Setbluetoolth);
    // 数据长度
    quint16 size = 20;
    SC::byteArrayFrom(cmd, size);
    // 数据
    cmd.append(data);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::End);

    qDebug()<<"setBlueTooth:"<<cmd.toHex();

    return cmd;
}
/**
 * 查询制造信息.
 *
 */
QByteArray Command::queryMadeInfo()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::Header);
    // 命令
    cmd.append(DT::Querymakeinfo);
    // 数据长度
    quint16 size = 1;
    SC::byteArrayFrom(cmd, size);
    // 数据
    cmd.append(0x0F);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::End);

    qDebug()<<"queryMadeInfo:"<<cmd.toHex();

    return cmd;
}/**
 * @brief 设置制造信息
 * @param data
 * @return 命令帧
 */
QByteArray Command::setMadeInfo(QByteArray data)
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::Header);
    // 命令
    cmd.append(DT::Setmakeinfo);
    // 数据长度
    // 数据长度
    quint16 size = 480; //480
    SC::byteArrayFrom(cmd, size);
    // 数据
    cmd.append(data);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::End);
    qDebug()<<"setMadeInfo:"<<cmd.toHex();

    return cmd;
}
