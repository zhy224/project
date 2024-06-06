#include "SC.h"

#if _MSC_VER>=1600
#pragma execution_character_set("utf-8");
#endif


// 校验数组
quint64 SC::checkArray[] = {
    0x1234,0x2345,0x3456,0x4567,
    0x5678,0x6789,0x789A,0x89AB,
    0x9ABC,0xABCD,0xBCDE,0xCDEF,0xDEF1
};

// CRC-16查表对照
static const uint8_t dgus_auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

// CRC-16尾字节的查表对照
static const uint8_t dgus_auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
    0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
    0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
    0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
    0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
    0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
    0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
    0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
    0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

// 当前文件路径
QString SC::currentPath = nullptr;

int SC::QueryDataDisplay = 0;
int SC::QueryDataDigital = 0;

SC::SC()
{
}

SC::~SC()
{
}

/**
 * SN码转DSN码.
 *
 * \param string SN码
 * \return DSN码
 */
QString SC::snTodsn(QString string)
{
    static unsigned int crcTable[] = {
        0x0000000,  0x77073096, 0xee0e612c, 0x990951ba, 0x76dc419,  0x706af48f, 0xe963a535, 0x9e6495a3,
        0xedb8832,  0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x9b64c2b,  0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
        0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
        0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
        0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
        0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
        0x76dc4190, 0x1db7106,  0x98d220bc, 0xefd5102a, 0x71b18589, 0x6b6b51f,  0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0xf00f934,  0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x86d3d2d,  0x91646c97, 0xe6635c01,
        0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
        0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
        0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
        0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
        0xedb88320, 0x9abfb3b6, 0x3b6e20c,  0x74b1d29a, 0xead54739, 0x9dd277af, 0x4db2615,  0x73dc1683,
        0xe3630b12, 0x94643b84, 0xd6d6a3e,  0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0xa00ae27,  0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
        0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
        0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
        0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
        0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x26d930a,  0x9c0906a9, 0xeb0e363f, 0x72076785, 0x5005713,
        0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0xcb61b38,  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0xbdbdf21,
        0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
        0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
        0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
        0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
    };

    QByteArray cmd;
    cmd.append((qint8)0xcb);
    cmd.append((qint8)0xbc);
    cmd.append((qint8)0xb4);
    cmd.append((qint8)0xb4);

    // 输入的SN码
    for (int i = 0; i < string.size(); i++) {
        cmd.append(string.at(i));
    }

    cmd.append((qint8)0xd3);
    cmd.append((qint8)0xc5);
    cmd.append((qint8)0xb9);
    cmd.append((qint8)0xe2);

    unsigned int crc = 0xFFFFFFFF;
    for (int i = 0; i < 25; i++) {
        crc = ((crc >> 8) & 0x00FFFFFF) ^ crcTable[(crc ^ cmd.at(i)) & 0xFF];
    }

    QString str;
    str.sprintf("%08X", crc ^ 0xFFFFFFFF);

    return str;
}

/**
 * string转uint16.
 *
 * \param str		字符串
 * \param offset	单位偏移量
 * \return
 */
quint16 SC::stringToUint16(QString str, float offset)
{
    quint16 value;
    if (str.isEmpty()) {
        value = 0xFFFF;
    }
    else {
        value = str.toFloat() * offset;
    }
    return __changeuint16(value);
}

/**
 * @brief string转int16.
 * @param str
 * @param offset
 * @return
 */
qint16 SC::stringToInt16(QString str, float offset)
{
    qint16 value;
    if (str.isEmpty()) {
        value = 0xFFFF;
    }
    else {
        value = str.toFloat() * offset;
    }
    return __changeint16(value);
}

/**
 * @brief hexString转QByteArray
 * @param str
 * @return
 */
QByteArray SC::hexStringtoByteArray(QString str)
{
    if (str.startsWith("0x")|| str.startsWith("0X")) {
        str = str.right(str.length() - 2);
    }
    str = str.trimmed();
    QByteArray array;
    formatString(str, 2, ' ');
    QStringList sl = str.split(" ");
    for (int i = sl.size() - 1; i > -1; --i) {
        if (!sl.at(i).isEmpty()) {
            array.append((char)sl.at(i).toInt(0, 16) & 0xFF);
        }
    }
    return array;
}

void SC::formatString(QString &org, int n, const QChar &ch)
{
    int size = org.size();
    int space = qRound(size*1.0 / n + 0.5) - 1;
    if (space <= 0)
        return;
    for (int i = 0, pos = n; i < space; ++i, pos += (n + 1)) {
        org.insert(pos, ch);
    }
}

/**
 * @brief string转固定长度QByteArray
 * @param array
 * @param str
 * @param size
 * @return
 */
bool SC::stringToByteArray(QByteArray &array, QString str, int size)
{
    array.fill(0, size);
    QByteArray strArray = str.trimmed().toLocal8Bit();
    if (size <= strArray.size()) {
        memcpy(array.data(), strArray.data(), size);
    }
    else {
        array.clear();
        for (int i = 0; i < size; i++) {
            if (i < strArray.size()) {
                array.append(strArray.at(i));
            }
            else {
                array.append(' ');
            }
        }
    }
    return true;
}

/**
 * @brief string转固定长度Uint8Array
 * @param array
 * @param str
 * @param size
 * @return
 */
bool SC::stringToUint8Array(quint8 *array, QString str, int size)
{
    QByteArray strArray = str.trimmed().toLocal8Bit();
    if (size  <= strArray.size()) {
        memcpy(array, strArray.data(), size);
    }
    else {
        for (int i = 0; i < size; i++) {
            if (i < strArray.size()) {
                array[i] = strArray.at(i);
            }
            else {
                array[i] = ' ';
            }
        }
    }
    return true;
}
/**
 * int转固定长度QByteArray
 * QByteArray.
 */
QByteArray SC::intToByteArray(int d,int len)
{
    QString hexStr = QString("%1").arg(d,len,16,QLatin1Char('0'));
    QByteArray byteArr = QByteArray::fromHex(hexStr.toUtf8());
    return byteArr;
}

/**
 * 获取(计算)校验位
 * 校验码 = 命令符 ○+ 数据码长度 ○+ 数据码.
 *
 * \param data	数据码
 * \return		校验码
 */
quint8 SC::getCalibration(QByteArray data)
{
    quint8 check = 0;
    for (int i = 1; i < data.size(); ++i) {
//        if (data.at(i) == (qint8)0xEE) {
//            ++i;
//        }
        check ^= data.at(i);
    }
    return check;
}

/**
 * @brief 获取CRC校验 只计算包数据
 * @param data
 * @return
 */
quint16 SC::getCRC16(QByteArray data)
{
    quint8 uchCRCHi = 0xff;
    quint8 uchCRCLo = 0xff;
    quint8 uIndex = 0;

    for (int i = 0; i < data.size(); ++i) {
        uIndex   = uchCRCHi ^ data.at(i);
        uchCRCLo = dgus_auchCRCLo[uIndex];
        uchCRCHi = uchCRCLo ^ dgus_auchCRCHi[uIndex];
    }

    return (quint16)(uchCRCHi << 8 | uchCRCLo);
}

/**
 * 添加双字节数据.
 *
 * \param data		QByteArray
 * \param value		双字节数据 quint16
 */
void SC::byteArrayFrom(QByteArray &data, quint16 value)
{
    data.append(qint8(value >> 8));
    data.append(qint8(value));
}

/**
 * @brief QByteArray 添加4字节数据
 * @param data
 * @param value
 */
void SC::byteArrayFrom(QByteArray& data, quint32 value)
{
    data.append(qint8(value >> 24 & 0xFF));
    data.append(qint8(value >> 16 & 0xFF));
    data.append(qint8(value >> 8 & 0xFF));
    data.append(qint8(value & 0xFF));
}

bool SC::__bool(quint8 data, quint8 offset)
{
    if ((data & (0x01 << offset)) == (0x01 << offset))
    {
        return true;
    }
    else
    {
        return false;
    }
    //return true;
}


bool SC::__bool(quint16 data, quint8 offset)
{
    if ((data & (0x0001 << offset)) == (0x0001 << offset))
    {
        return true;
    }
    else
    {
        return false;
    }
    //return true;
}

bool SC::__bool(quint32 data, quint8 offset)
{
    if ((data & (0x00000001 << offset)) == (0x00000001 << offset))
    {
        return true;
    }
    else
    {
        return false;
    }
    //return true;
}

/**
 * 解析无符号双字节数据.
 *
 * \param data1
 * \param data2
 * \return
 */
quint16 SC::__uint16(quint8 data1, quint8 data2)
{
    quint16 temp = 0;
    temp = quint16((data1 << 8) | data2);
    return temp;
}

/**
 * 解析有符号双字节数据.
 *
 * \param data1
 * \param data2
 * \return
 */
qint16 SC::___int16(quint8 data1, quint8 data2)
{
    qint16 temp = 0;
    temp = qint16((data1 << 8) | data2);
    return temp;
}

/**
 * 解析无符号4字节数据.
 *
 * \param data1
 * \param data2
 * \param data3
 * \param data4
 * \return
 */
quint32 SC::__uint32(quint8 data1, quint8 data2, quint8 data3, quint8 data4)
{
    quint32 temp = 0;
    temp = quint32((data1 << 24) | (data2 << 16) | (data3 << 8) | data4);
    return temp;
}

/**
 * @brief 字节反序
 * @param data
 */
void SC::__changeData(QByteArray& data)
{
    quint8 tmp;
    int a = data.size() / 2;
    for (int i = 0; i < a; ++i)
    {
        tmp = data[i];
        data[i] = data[data.size() - i - 1];
        data[data.size() - i - 1] = tmp;
    }
}

qint16 SC::__changeint16(quint16 data)
{
    quint8* p = (quint8*)&data;
    quint8 t;
    t = p[0];
    p[0] = p[1];
    p[1] = t;

    return *((qint16*)p);
}


quint16 SC::__changeuint16(quint16 data)
{
    quint8* p = (quint8*)&data;
    quint8 t;
    t = p[0];
    p[0] = p[1];
    p[1] = t;

    return *((quint16*)p);
}

quint32 SC::__changeuint32(quint32 data)
{
    quint8* p = (quint8*)&data;
    quint8 value;

    value = p[0];
    p[0] = p[3];
    p[3] = value;
    value = p[1];
    p[1] = p[2];
    p[2] = value;

    return *((quint32*)p);
}

quint64 SC::__changeuint64(quint64 data)
{
    quint64 temp = 0;
    quint8* psrc = (quint8*)&data;
    quint8* pdst = (quint8*)&temp;

    for (int i = 0; i < 8; i++) {
        pdst[7 - i] = psrc[i];
    }
    return temp;
}

quint32 SC::__changeuintRead32(quint32 data)
{
    quint8* p = (quint8*)&data;
    quint8 value;

    value = p[0];
    p[0] = p[2];
    p[2] = value;
    value = p[1];
    p[1] = p[3];
    p[3] = value;

    return *((quint32*)p);
}

quint32 SC::__changeuintSend32(quint32 data)
{
    quint8* p = (quint8*)&data;
    quint8 value;

    value = p[0];
    p[0] = p[1];
    p[1] = value;
    value = p[2];
    p[2] = p[3];
    p[3] = value;

    return *((quint32*)p);
}

/**
 * @brief 获取编辑框中的数据
 * @param edit
 * @param uint
 * @return
 */
quint16 SC::getLineEditUint16(QLineEdit *edit, float uint)
{
    quint16 value = 0;
    QString str = edit->text();
    if(edit->isHidden() || str.isEmpty()) {
        return 0xffff;
    }
    else {
        value = __changeuint16(str.toFloat() * uint);
    }

    return value;
}

/**
 * @brief 整型链表去重
 * @param list
 * @return
 */
QList<int> SC::getIntListDistinct(QList<int> list)
{
    QList<int> l;
    for (int i = 0; i < list.size(); i++) {
        if(!l.contains(list.at(i))) {
            l.append(list.at(i));
        }
    }
    return l;
}

/**
 * @brief 导出文件
 * @param path  文件路径
 * @param array 文件数据
 * @return
 */
bool SC::exportfile(QString path, QByteArray array)
{
    QStringList list = QString::fromLocal8Bit(array).split("\n");
    // 校验码
    quint64 check = 0;
    for (int i = 0; i < list.size(); i++) {
        QByteArray array = list.at(i).toUtf8();
        for (int j = 0; j < array.size(); j++) {
            check += array.at(j) * checkArray[j % 13];
        }
    }
    QString str = QString::fromUtf8("数据校验=");
    quint8 *temp = (quint8*)&check;
    for (int i = 7; i > -1; i--) {
        str.append(QString("%1").arg(*(temp + i), 2, 16, QChar('0')));
    }
    array.append(str.toUtf8());
    // 导出到文件
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    quint64 length = file.write(QString::fromLocal8Bit(array).toUtf8());
    file.close();
    if (length > 0) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief 记录当前文件路径
 * @param path
 */
void SC::recordCurrentPath(QString path)
{
    int index = path.lastIndexOf("/");
    currentPath = path.remove(index, path.length() - index) + "/";
}
//延时函数ms
void SC::Sleep(int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while(QTime::currentTime()<dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}
//判断文件是否存在
bool SC::IsFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}
//
/**
 * 解析焊接包参数
 * 0x34.
 *
 * \param data
 * \return
 */
UI_PACKAGE_DATA SC::getPackageData(QByteArray data)
{
    UI_PACKAGE_DATA info;

    // 焊接包组名编号
    info.GroupNameNumber = data.at(4);
    // 焊接包包名编号
    info.PackageNameNumber = data.at(5);
    // 激光功率
    info.LaserPower = __uint16(data.at(38), data.at(39));
    // 摆动频率
    info.SwingFrequence = __uint16(data.at(40), data.at(41));
    // 摆动幅度
    info.SwingAmplitude = __uint16(data.at(42), data.at(43));
    // 上升时间
    info.RiseTime = __uint16(data.at(44), data.at(45));
    // 下降时间
    info.DescentTime = __uint16(data.at(46), data.at(47));
    // 气体开启延时
    info.StartDelay = __uint16(data.at(48), data.at(49));
    // 气体关闭延时
    info.StopDelay = __uint16(data.at(50), data.at(51));
    // 脉冲频率
    info.PulseFrequence = __uint16(data.at(52), data.at(53));
    // 脉冲占空比
    info.PulseDuty = __uint16(data.at(54), data.at(55));
    // 点焊时间
    info.SpotWeldingTime = __uint16(data.at(56), data.at(57));
    // 点焊间隔
    info.SpotWeldinginterval = __uint16(data.at(58), data.at(59));
    // 点焊次数
    info.SpotWeldingTimes = __uint16(data.at(60), data.at(61));
    // 焊接方式
    info.LightHandMode = __uint16(data.at(62), data.at(63));
    // 送丝速度
    info.wirefeedSpeed = __uint16(data.at(64), data.at(65));
    // 送丝延时
    info.wirefeedDelay = __uint16(data.at(66), data.at(67));
    // 回丝长度
    info.backwireLength = __uint16(data.at(68), data.at(69));
    // 补丝长度
    info.wirefillingLength = __uint16(data.at(70), data.at(71));
    // 预热时长
    info.preheatTime = __uint16(data.at(72), data.at(73));
    // 预热功率
    info.preheatPower = __uint16(data.at(74), data.at(75));
    // 预热缓升时间
    info.preheatRiseTime = __uint16(data.at(76), data.at(77));
    // 预热缓降时间
    info.preheatDescentTime = __uint16(data.at(78), data.at(79));

    return info;
}


