#pragma once

#include "QtGlobal"

#define PRODUCT_MODEL_ID	"MCB_5"			//产品型号
#define VERSION_M			1				//主版本
#define VERSION_S			0				//子版本
#define VERSION_R			0				//修正版本

#define UPDATE_TIME_YEAR	2024
#define UPDATE_TIME_MONTH	4
#define UPDATE_TIME_DAY		8

#define BACK_DATA_SIZE_M			   31		// 回传数据长度
#define BACK_DATA_SIZE_S			   14		// 回传数据长度
#define TIMER_TIMEROUT                 3000	    // 连接超时时间
#define CONNECT_STATUS_TIMEROUT        1000	    // 通信超时时间（定时器）


/**
 * 枚举数据命名空间.
 */
namespace DT
{

    /**
     * 界面切换类型.
     */
    enum CHANGED_PAGE
    {
        // 子界面
        PAGE_S_NET_CONNECT		= 0,	// 网络连接界面
        PAGE_S_MCB_TEST			= 1,	// 主控测试界面
        PAGE_S_MCB_SYN			= 2,	// 综合测试界面
        PAGE_S_SYS_INFO         = 3,    // 系统信息界面
    };
    /**
     * 开始测试类型.
     */
    enum START_TEST
    {
        StartFrameHeader             = 0x7E,			// 帧头
        StartComandCode              = 0xAB,            // 查询命令符
        StartDataSize                = 0x0001,          // 数据位长度2（8）
        StartDataCode                = 0x0F,			// 数据码
        StartFrameEnd                = 0xE7,			// 帧尾

    };
    /**
     * 查询命令类型.
     */
    enum QUERY_COMMAND_MAIN
    {
        QueryFrameHeader             = 0x7E,			// 帧头
        QueryComandCode              = 0xAA,            // 查询命令符
        QueryDataSize                = 0x0002,          // 数据位长度2（8）
        QueryDataDisplay             = 0x00,			// 数据码1
        QueryDataDigital             = 0x00,			// 数据码2
        QueryFrameEnd                = 0xE7,			// 帧尾

    };
    /**
     * 查询命令类型.
     */
    enum QUERY_COMMAND_SYN
    {
        QueryFrameHeader_syn             = 0x7E,			// 帧头
        QueryComandCode_syn              = 0xBB,            // 查询命令符
        QueryDataSize_syn                = 0x0001,          // 数据位长度
        QueryDataCode_syn                = 0x0F,			// 数据码 0f
        QueryFrameEnd_syn                = 0xE7,			// 帧尾

    };
    /**
     * 回传命令类型.
     */
    enum BACK_COMMAND_MAIN
    {
        BackFrameHeaderM              = 0x7E,			// 帧头
        BackComandCodeM               = 0xAA,           // 返回命令符
        BackDataSizeM                 = 0x0019,         // 数据位长度25（31）
        BackFrameEndM                 = 0xE7,			// 帧尾
    };
    /**
     * 回传命令类型.
     */
    enum BACK_COMMAND_SYN
    {
        BackFrameHeaderS              = 0x7E,			// 帧头
        BackComandCodeS               = 0xBB,           // 返回命令符
        BackDataSizeS                 = 0x0008,         // 数据位长度25（31）
        BackFrameEndS                 = 0xE7,			// 帧尾
    };



};
