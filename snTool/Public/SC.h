#pragma once

#include <QObject>
#include <QDebug>
#include <QCryptographicHash>
#include <QLineEdit>
#include <QFile>
#include <QFileInfo>

#include <QTimer>
#include <QElapsedTimer> //纳秒
#include <QDateTime>
#include <QTime>
#include <QCoreApplication>

#include "SD.h"

/**
 * 公用静态方法类.
 */
class SC
{
public:
        SC();
        ~SC();

public:
        // 校验数组
        static quint64 checkArray[];
        // 当前文件路径
        static QString currentPath;


        static int QueryDataDisplay;// 数据码1
        static int QueryDataDigital;// 数据码2
public:
        // sn转DSN
        static QString snTodsn(QString string);
        // string转uint16
        static quint16 stringToUint16(QString str, float offset = 1.0);
        // string转uint16
        static qint16 stringToInt16(QString str, float offset = 1.0);
        // hexString转QByteArray
        static QByteArray hexStringtoByteArray(QString str);
        static void formatString(QString &org, int n, const QChar &ch);
        // string转固定长度QByteArray
        static bool stringToByteArray(QByteArray &array, QString str, int size);
        // string转固定长度Uint8Array
        static bool stringToUint8Array(quint8 *array, QString str, int size);
        // int转固定长度QByteArray
        static QByteArray intToByteArray(int d,int len); //byte = len/2;

        // 获取(计算)校验位
        static quint8 getCalibration(QByteArray data);
        // 获取CRC校验
        static quint16 getCRC16(QByteArray data);

        // QByteArray 添加双字节数据
        static void byteArrayFrom(QByteArray& data, quint16 value);
        // QByteArray 添加4字节数据
        static void byteArrayFrom(QByteArray& data, quint32 value);

        //解析BOOL
        static bool __bool(quint8 data, quint8 offset);
        static bool __bool(quint16 data, quint8 offset);
        static bool __bool(quint32 data, quint8 offset);

        // 解析无符号双字节数据
        static quint16 __uint16(quint8 data1, quint8 data2);
        // 解析有符号双字节数据
        static qint16 ___int16(quint8 data1, quint8 data2);

        // 解析无符号4字节数据
        static quint32 __uint32(quint8 data1, quint8 data2, quint8 data3, quint8 data4);
        // 解析无符号8字节数据
        static quint64 __uint64(char* data);

        // 字节反序
        static void __changeData(QByteArray &data);
        static qint16 __changeint16(quint16 data);
        static quint16 __changeuint16(quint16 data);
        static quint16 __changeuint16(quint8* data);
        static quint32 __changeuint32(quint32 data);
        static quint64 __changeuint64(quint64 data);

        // 故障编码 双字节高低字节反序 00 01 02 03 -> 02 03 00 01
        static quint32 __changeuintRead32(quint32 data);
        // 故障编码 双字节高低字节反序 00 01 02 03 -> 01 00 03 02
        static quint32 __changeuintSend32(quint32 data);

        // 获取编辑框中的数据
        static quint16 getLineEditUint16(QLineEdit *edit, float uint = 1);

        // 整型链表去重
        static QList<int> getIntListDistinct(QList<int> list);
        // 导出文件
        static bool exportfile(QString path, QByteArray array);
        // 获取当前文件路径
        static QString getCurrentPath() { return currentPath; }
        // 记录当前文件路径
        static void recordCurrentPath(QString path);

        /* 数据解析 */
        // 解析焊接包参数 0x34
        static UI_PACKAGE_DATA getPackageData(QByteArray data);
        // 解析制造信息 0x3B
        static UI_MAKEINFO_DATA getMakeinfoData(BACK_MAKEINFO_DATA data);

        // 获取操作结果提示
        static QString getResultString(quint8 value);
        //延时函数ms
        static void Sleep(int ms);
        // 文件是否存在
        static bool IsFileExist(QString fullFileName);




};
