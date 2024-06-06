#pragma once

#include "QtGlobal"

#define PRODUCT_MODEL_ID	"Weiding"		//产品型号
#define VERSION_M			1				//主版本
#define VERSION_S			0				//子版本
#define VERSION_R			0				//修正版本

#define UPDATE_TIME_YEAR	2024
#define UPDATE_TIME_MONTH	5
#define UPDATE_TIME_DAY		20

#define BACK_DATA_SIZE_WELD            62		// 回传数据长度(焊接包)
#define TIMER_TIMEROUT                 3000	    // 连接超时时间
#define CONNECT_STATUS_TIMEROUT        1000	    // 通信超时时间（定时器）




/**
 * 枚举数据命名空间.
 */
namespace DT
{

    enum FRAME
    {
        Header	= 0x7E,			// 帧头 0x7E
        End		= 0xE7			// 帧尾 0xE7
    };

    /**
     * 界面切换.
     */
    enum CHANGED_PAGE
    {
        // 子界面
        PAGE_DEPLOY             = 0,	// 配置界面
        PAGE_PRO_PAR			= 1,	// 参数界面

    };

    /**
     * 查询命令类型.
     */
    enum QUERY_COMMAND
    {
        Queryrun				= 0x10,			// 运行参数查询
        Queryadapt				= 0x11,			// 适应性参数查询
        Queryclientpackage		= 0x12,			// 客户焊接包查询
        Queryfactorypackage		= 0x13,			// 厂家焊接包查询
        Queryuseingpackage		= 0x14,			// 可用焊接包查询
        Querylaserrunlog		= 0x15,			// 激光器运行日志查询
        Querylasercontrollog	= 0x16,			// 激光器操作日志查询
        Querygalvanometer		= 0x17,			// 振镜控制系统参数查询
        Querywirefeeder			= 0x18,			// 送丝机参数查询
        Querycoolrun			= 0x19,			// 冷却系统运行参数查询
        Querycooladapt			= 0x1A,			// 冷却系统适应性参数查询
        Querycurpackage			= 0x1B,			// 当前焊接包参数查询
        Querymakeinfo			= 0x1C,			// 制造信息查询
        Querybluetooth			= 0x1D,			// 蓝牙名称查询
        Querylanguage			= 0x1E,			// 主机当前语言查询
        Querycustom				= 0x1F,			// 自定义参数查询
        Queryfrequencyconversion= 0x52			// 变频驱动板参数查询
    };
    /**
     * 设置命令类型.
     */
    enum SET_COMMAND
    {
        Setpackage			= 0x20,			// 当前焊接包参数设置
        Setadapt			= 0x21,			// 适应性参数设置
        Setrtc				= 0x22,			// RTC时间设置
        Setduetime			= 0x23,			// 到期时间设置
        Setpowerdata		= 0x24,			// 功率等设置
        Setbluetoolth		= 0x25,			// 蓝牙设置
        Setfocus			= 0x26,			// 振镜系统焦点设置
        Setcoolsystem		= 0x27,			// 冷却系统适应性参数设置
        Setmakeinfo			= 0x29,			// 制造信息设置
        Setfactorypackage	= 0x2A,			// 厂家预设焊接包参数设置
        Setfanspeed			= 0x2B,			// 手动模式风扇转速设置
        Setlanguage			= 0x2C,			// 主机语言设置
        Setgunlength		= 0x2D,			// 焊枪枪杆长度设置
        Setcompressorfreq	= 0x2E,			// 压缩机频率设置
        Setvalvestep        = 0x2F,			// 主辅阀步数设置
    };
    /**
     * 回传命令类型.
     */
    enum BACK_COMMAND
    {
        BackHeader              = 0x7E,          // 返回帧头
        Backresults             = 0x30,			// 操作结果回传
        Backrun                 = 0x31,			// 运行参数回传
        Backadapt               = 0x32,			// 适应性参数回传
        Backpackagenumber       = 0x33,			// 焊接包可用编号回传
        Backpackagedata         = 0x34,			// 焊接包参数回传
        Backlaserrunlog         = 0x35,			// 激光器运行日志回传
        Backlasercontrollog     = 0x36,			// 激光器操作日志回传
        Backgalvanometer        = 0x37,			// 振镜系统参数回传
        Backwirefeeder          = 0x38,			// 送丝机参数回传
        Backcoolsystenrun       = 0x39,			// 冷却系统运行参数回传
        Backcoolsystemadapt     = 0x3A,			// 冷却系统适应性参数回传
        Backmakeinfo            = 0x3B,			// 制造信息回传
        Backbluetooth           = 0x3C,			// 蓝牙名称回传
        Backcustom              = 0x3E,			// 自定义参数回传
        Backlanguage            = 0x3F,			// 主机语言回传
        Backfrequencyconversion	= 0x72,			// 变频驱动板参数回传
    };

};
