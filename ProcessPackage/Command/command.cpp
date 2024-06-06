#include "command.h"


Command::Command(QObject *parent) : QObject(parent)
{

}
/**
 * 查询网络连接状态.
 *
 */
QByteArray Command::queryNetwork()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::Header);
    // 命令
    cmd.append(DT::Querybluetooth);
    // 数据长度
    quint16 size = 0x0001;
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
 * 查询可用焊接包编号命令
 *
 */
QByteArray Command::query_avail_weld()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::QueryFrameHeader_avail);
    // 命令
    cmd.append(DT::QueryComandCode_avail);
    // 数据长度
    QByteArray QueryDataSize = SC::intToByteArray(DT::QueryDataSize_avail,4);
    //qDebug()<<"QueryDataSize:"<<QueryDataSize;
    cmd.append(QueryDataSize);
    // 数据码
    cmd.append(DT::QueryDataCode_avail);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::QueryFrameEnd_avail);
    qDebug()<<"query_avail_weld:"<<cmd<<cmd.toHex();

    return cmd;
}
/**
 * 查询客户焊接包命令 0x12
 *
 */
QByteArray Command::query_cus_weld(PACKAGE_NUMBER package)
{
    QByteArray cmd;
    QByteArray data;
    data.append(package.GroupNumber);
    data.append(package.PackageNumber);
    // 帧头
    cmd.append(DT::QueryFrameHeader_cus);
    // 命令
    cmd.append(DT::QueryComandCode_cus);
    // 数据长度
    QByteArray QueryDataSize = SC::intToByteArray(DT::QueryDataSize_cus,4);
    //qDebug()<<"QueryDataSize:"<<QueryDataSize;
    cmd.append(QueryDataSize);
    // 数据
    cmd.append(data);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::QueryFrameEnd_cus);
    qDebug()<<"query_cus_weld:"<<cmd<<cmd.toHex();

    return cmd;
}
/**
 * 查询当前焊接包命令
 *
 */
QByteArray Command::query_curr_weld()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::QueryFrameHeader_curr);
    // 命令
    cmd.append(DT::QueryComandCode_curr);
    // 数据长度
    QByteArray QueryDataSize = SC::intToByteArray(DT::QueryDataSize_curr,4);
    //qDebug()<<"QueryDataSize:"<<QueryDataSize;
    cmd.append(QueryDataSize);
    // 数据码
    cmd.append(DT::QueryDataCode_curr);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::QueryFrameEnd_curr);
    qDebug()<<"query_curr_weld:"<<cmd<<cmd.toHex();

    return cmd;
}
/**
 * 设置当前焊接包命令
 *
 */
QByteArray Command::set_curr_weld(QByteArray data)
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::SetFrameHeader_curr);
    // 命令
    cmd.append(DT::SetComandCode_curr);
    // 数据长度
    QByteArray SetDataSize = SC::intToByteArray(DT::SetDataSize_curr,4);
    cmd.append(SetDataSize);
    // 数据
    cmd.append(data);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::SetFrameEnd_curr);
    qDebug()<<"set_curr_weld:"<<cmd.toHex();

    return cmd;
}
/**
 * 存储当前焊接包命令
 *
 */
QByteArray Command::save_curr_weld()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::SaveFrameHeader_curr);
    // 命令
    cmd.append(DT::SaveComandCode_curr);
    // 数据长度
    QByteArray SetDataSize = SC::intToByteArray(DT::SaveDataSize_curr,4);
    cmd.append(SetDataSize);
    // 数据码
    cmd.append(DT::SaveDataCode_curr);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::SaveFrameEnd_curr);
    qDebug()<<"save_curr_weld:"<<cmd.toHex();

    return cmd;
}
