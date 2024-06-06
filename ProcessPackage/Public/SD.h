#ifndef SD_H
#define SD_H

#include "DT.h"



#pragma pack(1)
/**
 * 设置当前焊接包参数数据结构	0x20.
 */
struct SET_PACKAGE_DATA
{
    quint8	GroupNameNumber;			// 焊接包组名编号
    quint8	PackageNameNumber;			// 焊接包包名编号
    quint8  name1[16];
    quint8  name2[16];
    quint16 LaserPower;					// 激光功率
    quint16 SwingFrequence;				// 摆动频率
    quint16 SwingAmplitude;				// 摆动幅度
    quint16 RiseTime;					// 功率缓升时间
    quint16 DescentTime;				// 功率缓降时间
    quint16 StartDelay;					// 气体开启延时
    quint16 StopDelay;					// 气体关闭延时
    quint16 PulseFrequence;				// 脉冲频率
    quint16 PulseDuty;					// 脉冲占空比
    quint16 SpotWeldingTime;			// 点焊出光时间
    quint16 SpotWeldinginterval;		// 点焊间隔时间
    quint16 SpotWeldingTimes;			// 点焊次数
    quint16 LightHandMode;				// 焊接方式
    quint16 wirefeedSpeed;              // 送丝速度
    quint16 wirefeedDelay;              // 送丝延时
    quint16 backwireLength;             // 回丝长度
    quint16 wirefillingLength;          // 补丝长度
    quint16 preheatTime;                // 预热时长
    quint16 preheatPower;               // 预热功率
    quint16 preheatRiseTime;            // 预热缓升时间
    quint16 preheatDescentTime;         // 预热缓降时间
    quint16 CSWLICOP;                   // 连续点焊出光间隔截至功率
    quint16 Reserve;                    // 预留

    SET_PACKAGE_DATA() {
        memset(this, 0, sizeof(SET_PACKAGE_DATA));
    }
    void keep() {
        memset(this, 0xFF, sizeof(SET_PACKAGE_DATA));
    }
};
/**
 * 焊接包编号数据结构.
 */
struct PACKAGE_NUMBER
{
    quint8 GroupNumber;		// 组编号
    quint8 PackageNumber;	// 包编号

    PACKAGE_NUMBER() {
        memset(this, 0, sizeof(PACKAGE_NUMBER));
    }
};
/**
 * 界面-焊接包参数结构体-0x34.
 */
struct UI_PACKAGE_DATA : SET_PACKAGE_DATA
{

};
struct BACK_DATA_CURR_WELD : SET_PACKAGE_DATA
{

};

#pragma pack()

#endif // SD_H
