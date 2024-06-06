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
     * 查询可用焊接包命令.
     */
    enum QUERY_AVAIL_WELD
    {
        QueryFrameHeader_avail             = 0x7E,			// 帧头
        QueryComandCode_avail              = 0x14,            // 查询命令符
        QueryDataSize_avail                = 0x0001,          // 数据位长度1
        QueryDataCode_avail                = 0x0F,			// 数据码
        QueryFrameEnd_avail                = 0xE7,			// 帧尾
    };
    /**
     * 查询当前焊接包命令.
     */
    enum QUERY_CURR_WELD
    {
        QueryFrameHeader_curr             = 0x7E,			// 帧头
        QueryComandCode_curr              = 0x1B,            // 查询命令符
        QueryDataSize_curr                = 0x0001,          // 数据位长度1
        QueryDataCode_curr                = 0x0F,			// 数据码
        QueryFrameEnd_curr                = 0xE7,			// 帧尾
    };
    /**
     * 查询客户焊接包命令.
     */
    enum QUERY_CUS_WELD
    {
        QueryFrameHeader_cus             = 0x7E,			// 帧头
        QueryComandCode_cus              = 0x12,            // 查询命令符
        QueryDataSize_cus                = 0x0002,          // 数据位长度1
        QueryFrameEnd_cus                = 0xE7,			// 帧尾
    };
    /**
     * 设置当前焊接包命令.
     */
    enum SET_CURR_WELD
    {
        SetFrameHeader_curr             = 0x7E,              // 帧头
        SetComandCode_curr              = 0x20,              // 设置命令符
        SetDataSize_curr                = 0x0050,            // 数据位长度80
        SetFrameEnd_curr                = 0xE7,              // 帧尾

    };
    /**
     * 存储当前焊接包命令.
     */
    enum SAVE_CURR_WELD
    {
        SaveFrameHeader_curr             = 0x7E,              // 帧头
        SaveComandCode_curr              = 0x08,              // 设置命令符
        SaveDataSize_curr                = 0x0001,            // 数据位长度1
        SaveDataCode_curr                = 0x0F,              // 数据 厂家F0
        SaveFrameEnd_curr                = 0xE7,              // 帧尾

    };
    /**
     * 回传当前焊接包数据.
     */
    enum BACK_CURR_WELD
    {
        BackFrameHeader_curr              = 0x7E,			// 帧头
        BackComandCode_curr               = 0x34,           // 返回命令符
        BackDataSize_curr                 = 0x003E,         // 数据位长度62
        BackFrameEnd_curr                 = 0xE7,			// 帧尾
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
