#include "command.h"

Command::Command(QObject *parent) : QObject(parent)
{

}
/**
 * 查询网络连接状态主控板
 *
 */
QByteArray Command::query_main_CMD()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::QueryFrameHeader);
    // 命令
    cmd.append(DT::QueryComandCode);
    // 数据长度
    QByteArray QueryDataSize = SC::intToByteArray(DT::QueryDataSize,4);
    //qDebug()<<"QueryDataSize:"<<QueryDataSize;
    cmd.append(QueryDataSize);
    // 数据1
    cmd.append(DT::QueryDataDisplay);
    // 数据2
    cmd.append(DT::QueryDataDigital);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::QueryFrameEnd);
    qDebug()<<"query_main_CMD:"<<cmd<<cmd.toHex();

    return cmd;
}
/**
 * 查询网络连接状态综合板
 *
 */
QByteArray Command::query_syn_CMD()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::QueryFrameHeader_syn);
    // 命令
    cmd.append(DT::QueryComandCode_syn);
    // 数据长度
    QByteArray QueryDataSize_syn = SC::intToByteArray(DT::QueryDataSize_syn,4);
    cmd.append(QueryDataSize_syn);
    // 数据码
    cmd.append(DT::QueryDataCode_syn);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::QueryFrameEnd_syn);
    qDebug()<<"query_syn_CMD:"<<cmd.toHex();

    return cmd;
}
/**
 * 开始测试
 *
 */
QByteArray Command::startTest_CMD()
{
    QByteArray cmd;
    // 帧头
    cmd.append(DT::StartFrameHeader);
    // 命令
    cmd.append(DT::StartComandCode);
    // 数据长度
    QByteArray StartDataSize = SC::intToByteArray(DT::StartDataSize,4);
    cmd.append(StartDataSize);
    // 数据码
    cmd.append(DT::StartDataCode);
    // 校验
    quint8 check = SC::getCalibration(cmd);
    cmd.append(check);
    // 帧尾
    cmd.append(DT::StartFrameEnd);
    qDebug()<<"startTestCMD:"<<cmd.toHex();

    return cmd;
}
