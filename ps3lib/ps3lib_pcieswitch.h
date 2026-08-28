/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_pcieswitch.h
 * @date    2023.04.28
 * @brief   switch信息
 */

#ifndef __PS3LIB_PCIESWITCH_H__
#define __PS3LIB_PCIESWITCH_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h> ///< for bool
#include "ps3lib_event.h"

#define PS3LIB_SW_MAX_BASEINFO_RSP_SIZE      (4)   ///< Switch基础信息最大回复长度
#define PS3LIB_SW_EVENT_DATA_COLLECT_MAX_NUM (256) ///< Switch 补充收集信息数量
#define PS3LIB_SW_EVENT_DESC_64              (64)  ///< 事件描述字符串长度
#define PS3LIB_SW_REVISION_LEVEL_LENGTH      (4)   ///< sw配置文件revisionLevel长度
#define PS3LIB_SW_LTSSM_INFO_MAX_COUNT       (64)  ///< 状态积最大查询次数
#define PS3LIB_SW_STRING_LENTH               (64)  ///< sw版本信息、时间、日志字符串长度
#define PS3LIB_SW_NAME_BUF_LEN               (5)   ///< 线程名称长度，与之前保持一致
#define PS3LIB_SW_PORT_MAX                   (116) ///< pcieswitch Port的最大规格

/**
 *  @brief Switch 基础信息结构体
 */
typedef struct Ps3LibSWBaseInfo {
    U32 dataLen;               ///< 基础信息数据长度
    S8  data[0];               ///< 基础信息数据
} Ps3LibSWBaseInfo_s;

/**
 * @brief   厂商分区字段长度
 */
enum {
    PS3LIB_SW_MFG_PART_NUMBER_LEN   = 16, ///< 序列号长度
    PS3LIB_SW_MFG_WWN_LEN           = 16, ///< 卡标识符长度
    PS3LIB_SW_MFG_REVISION_LEN      = 4,  ///< 反工版本号长度
    PS3LIB_SW_MFG_OEM_STR_LEN       = 64, ///< oem字段长度
};

/**
 * @brief   厂商分区保存的生产字段
 * @note    字符串可能不包含终止符'\0'
 */
typedef struct Ps3LibSwMfgInfo {
    U8  partNumber[PS3LIB_SW_MFG_PART_NUMBER_LEN];     ///< part number 16-byte ASCII string
    U8  serialNumber[PS3LIB_SW_MFG_PART_NUMBER_LEN];   ///< 序列号
    U64 sasAddrBase;                                    ///< Not Support
    U32 mfgDate;                                        ///< U32 时间戳
    U8  revision[PS3LIB_SW_MFG_REVISION_LEN];          ///< 版本号
    U8  wwn[PS3LIB_SW_MFG_WWN_LEN];                    ///< 卡的唯一标识符
    U32 reworkDate;                                     ///< 返工时间 时间戳
    U8  pad[4];                                         ///< 保留字段
    U8  extra1[PS3LIB_SW_MFG_OEM_STR_LEN];             ///< oem字段
    U8  extra2[PS3LIB_SW_MFG_OEM_STR_LEN];             ///< oem字段
} Ps3LibSwMfgInfo_t;

/**
 *  @brief Switch 事件设备类型
 */
typedef enum Ps3LibSWEventDevType
{ 
    PS3LIB_SW_EVT_TYPE_UNKNOW   = 0,   ///< 事件类型未知
    PS3LIB_SW_EVT_TYPE_EXPANDER = 1,   ///< 事件类型为Switch
    PS3LIB_SW_EVT_TYPE_SWITCH   = 2,   ///< 事件类型为Switch
    PS3LIB_SW_EVT_TYPE_NR,
}Ps3LibSWEventDevType_e; 

/**
 *  @brief Switch 事件类型
 */
typedef enum Ps3LibSWEventType
{ 
    PS3LIB_SW_EVT_LOCAL           = 1, ///< local 事件类型
    PS3LIB_SW_EVT_PD_COUNT_LOCAL  = 2, ///< pd count 事件类型
    PS3LIB_SW_EVT_CTRL_INFO_LOCAL = 3, ///< ctrl info 事件类型
    PS3LIB_SW_EVT_SWITCH_LOCAL       = 4, ///< Switch info 事件类型
    PS3LIB_SW_EVT_SWITCH_PHY_LOCAL   = 5, ///< Switch phy info 事件类型
    PS3LIB_SW_EVT_SWITCH_CONN_LOCAL  = 6, ///< Switch connect info 事件类型
    PS3LIB_SW_EVT_SWITCH_INFO_LOCAL  = 7, ///< Switch ctrl info 事件类型
    PS3LIB_SW_EVT_MAX_TYPE_LOCAL  = 8, ///< Switch 所有事件类型
    PS3LIB_SW_EVT_TYPE_MAX,
}Ps3LibSWEventType_e; 

/**
 *  @brief Switch 事件级别
 */
typedef enum Ps3LibSWEventLevel
{
    PS3LIB_SW_EVT_CLASS_UNKNOWN   = 0b0000,    ///< 未知级别事件
    PS3LIB_SW_EVT_CLASS_INFO      = 0b0001,    ///< Info 级别事件
    PS3LIB_SW_EVT_CLASS_WARNING   = 0b0010,    ///< Warning 级别事件
    PS3LIB_SW_EVT_CLASS_CRITICAL  = 0b0100,    ///< Critical 级别事件
    PS3LIB_SW_EVT_CLASS_FATAL     = 0b1000,    ///< Fatal 级别事件
    PS3LIB_SW_EVT_CLASS_MAX,
} Ps3LibSWEventLevel_e;

/**
 *  @brief Switch 事件日志补充收集信息结构体key-value
 */
typedef struct Ps3LibSWEventDataCollectionKV {
    char key[PS3LIB_SW_EVENT_DATA_COLLECT_MAX_NUM];   ///< 补充信息-key
    char val[PS3LIB_SW_EVENT_DATA_COLLECT_MAX_NUM];   ///< 补充信息-value
} Ps3LibSWEventDataCollectionKV_s;

/**
 *  @brief Switch 事件日志补充收集信息结构体
 */
typedef struct Ps3LibSWEventDataCollection {
    struct Ps3LibSWEventDataCollectionKV kv[PS3LIB_SW_EVENT_DATA_COLLECT_MAX_NUM]; ///< 事件日志补充收集信息数组
} Ps3LibSWEventDataCollection_s;

/**
 *  @brief Switch 事件回复信息结构体
 */
typedef struct Ps3LibSWCmdEventInfo {
    U32                  uniqueId;         ///< 订阅ID
    U32                  seqNum;           ///< 日志序号
    U32                  timeStampBySec;   ///< 秒级时间戳 since 1970
    U16                  evtCode;          ///< 事件代号 0-4095
    U16                  pad;              ///< 保留字段
    Ps3LibSWEventType_e  eventType;        ///< 事件类型
    Ps3LibSWEventLevel_e evtLevel;         ///< 事件级别
    S8                   eventDesc[PS3LIB_SW_EVENT_DESC_64];    ///< 事件描述
    S32                  kvCount;          ///< Switch补充信息数量
    Ps3LibSWEventDataCollection_s  collection;     ///< Switch补充信息
} Ps3LibSWCmdEventInfo_t;

/**
 *  @brief Switch 事件订阅结构体
 */
typedef struct Ps3LibSWEventSubscribe {
    CtrlId_t                devlId;      ///< 设备地址 低八位为bus 高八位为address
    Ps3LibSWEventDevType_e  devType;     ///< 设备类型
    Ps3LibSWEventType_e     evtType;     ///< 事件类型 PS3LIB_EVT_MAX_TYPE_LOCAL 表示获取所有
    Ps3LibSWEventLevel_e    evtLevel;    ///< 事件级别 PS3LIB_EVT_CLASS_MAX 表示获取所有
    Ps3Errno (*getEvtInfo)(Ps3LibSWCmdEventInfo_t *evtInfo);  ///< 获取事件日志回调
} Ps3LibSWEventSubscribe_t;

/**
 *  @brief Switch Port Type
 */
typedef enum Ps3LibSWPortType {
    PS3LIB_SW_PCIE_PORT_TYPE_UP = 0,    ///< up 端口
    PS3LIB_SW_PCIE_PORT_TYPE_DP,        ///< dp 端口
    PS3LIB_SW_PCIE_PORT_TYPE_EXP_NTB0,  ///< ntb0 端口
    PS3LIB_SW_PCIE_PORT_TYPE_EXP_NTB1,  ///< ntb1 端口
    PS3LIB_SW_PCIE_PORT_TYPE_MAX,
} Ps3LibSWPortType_e;

typedef enum Ps3LibSWPortSpeed{
    PS3LIB_SW_PCIE_PORT_SPEED_GEN1  = 1,  ///< GEN1 2.5GT/s
    PS3LIB_SW_PCIE_PORT_SPEED_GEN2,  ///< GEN2 5.0GT/s
    PS3LIB_SW_PCIE_PORT_SPEED_GEN3,  ///< GEN3 8.0GT/s
    PS3LIB_SW_PCIE_PORT_SPEED_GEN4,  ///< GEN4 16.0GT/s
    PS3LIB_SW_PCIE_PORT_SPEED_GEN5,  ///< GEN5 32.0GT/s
    PS3LIB_SW_PCIE_PORT_SPEED_GEN6,  ///< GEN6 64.0GT/s
    PS3LIB_SW_PCIE_PORT_SPEED_UNKNOWN,
} Ps3LibSWPortSpeed_e;

typedef enum Ps3LibSWPortWidth{
    PS3LIB_SW_PCIE_PORT_WIDTH_X1  = 1,  ///< X1
    PS3LIB_SW_PCIE_PORT_WIDTH_X2  = 2,  ///< X2
    PS3LIB_SW_PCIE_PORT_WIDTH_X4  = 4,  ///< X3
    PS3LIB_SW_PCIE_PORT_WIDTH_X8  = 8,  ///< X8
    PS3LIB_SW_PCIE_PORT_WIDTH_X16 = 16, ///< X16
    PS3LIB_SW_PCIE_PORT_WIDTH_UNKNOWN,
} Ps3LibSWPortWidth_e;

/**
 *  @brief Switch Port Link Stat
 */
typedef enum Ps3LibSWPortLinkStat {
    PS3LIB_SW_PCIE_PORT_LINK_STAT_DOWM = 0,  ///< link down 状态
    PS3LIB_SW_PCIE_PORT_LINK_STAT_UP,        ///< link up 状态
    PS3LIB_SW_PCIE_PORT_LINK_STAT_NA,
} Ps3LibSWPortLinkStat_e;

/**
 *  @brief Switch 时钟模式枚举
 */
typedef enum Ps3LibSWUpClockMode {
    PS3LIB_SW_CLOCK_MODE_UNSUPPORT = 0,  ///< 时钟模式不支持
    PS3LIB_SW_CLOCK_MODE_LOCAL,          ///< 本地时钟模式
    PS3LIB_SW_CLOCK_MODE_HOST,           ///< 服务器时钟模式
} Ps3LibSWClockMode_e;

/**
 *  @brief Switch 更新状态枚举
 */
typedef enum Ps3LibSWTimeUpdateState {
    PS3LIB_SW_TIME_UPDATE_SUCCESS = 0,      ///< 时间更新成功
    PS3LIB_SW_TIME_UPDATE_NOT_ACTIVATED,    ///< 驱动已更新时间，本次更新未生效
    PS3LIB_SW_TIME_UPDATE_FAILED,           ///< 时间更新失败
} Ps3LibSWTimeUpdateState_e;

/**
 *  @brief Switch 端口状态机枚举
 */
typedef enum Ps3LibSWPortLtssmState {
    PS3LIB_SW_LTSSM_S_DETECT_QUIET                   = 0x00,
    PS3LIB_SW_LTSSM_S_DETECT_ACT                     = 0x01,
    PS3LIB_SW_LTSSM_S_POLL_ACTIVE                    = 0x02,
    PS3LIB_SW_LTSSM_S_POLL_COMPLIANCE                = 0x03,
    PS3LIB_SW_LTSSM_S_POLL_CONFIG                    = 0x04,
    PS3LIB_SW_LTSSM_S_PRE_DETECT_QUIET               = 0x05,
    PS3LIB_SW_LTSSM_S_DETECT_WAIT                    = 0x06,
    PS3LIB_SW_LTSSM_S_CFG_LINKWD_START               = 0x07,
    PS3LIB_SW_LTSSM_S_CFG_LINKWD_ACEPT               = 0x08,
    PS3LIB_SW_LTSSM_S_CFG_LANENUM_WAI                = 0x09,
    PS3LIB_SW_LTSSM_S_CFG_LANENUM_ACEPT              = 0x0A,
    PS3LIB_SW_LTSSM_S_CFG_COMPLETE                   = 0x0B,
    PS3LIB_SW_LTSSM_S_CFG_IDLE                       = 0x0C,
    PS3LIB_SW_LTSSM_S_RCVRY_LOCK                     = 0x0D,
    PS3LIB_SW_LTSSM_S_RCVRY_SPEED                    = 0x0E,
    PS3LIB_SW_LTSSM_S_RCVRY_RCVRCFG                  = 0x0F,
    PS3LIB_SW_LTSSM_S_RCVRY_IDLE                     = 0x10,
    PS3LIB_SW_LTSSM_S_L0                             = 0x11,
    PS3LIB_SW_LTSSM_S_L0S                            = 0x12,
    PS3LIB_SW_LTSSM_S_L123_SEND_EIDLE                = 0x13,
    PS3LIB_SW_LTSSM_S_L1_IDLE                        = 0x14,
    PS3LIB_SW_LTSSM_S_L2_IDLE                        = 0x15,
    PS3LIB_SW_LTSSM_S_L2_WAKE                        = 0x16,
    PS3LIB_SW_LTSSM_S_DISABLED_ENTRY                 = 0x17,
    PS3LIB_SW_LTSSM_S_DISABLED_IDLE                  = 0x18,
    PS3LIB_SW_LTSSM_S_DISABLED                       = 0x19,
    PS3LIB_SW_LTSSM_S_LPBK_ENTRY                     = 0x1A,
    PS3LIB_SW_LTSSM_S_LPBK_ACTIVE                    = 0x1B,
    PS3LIB_SW_LTSSM_S_LPBK_EXIT                      = 0x1C,
    PS3LIB_SW_LTSSM_S_LPBK_EXIT_TIMEOUT              = 0x1D,
    PS3LIB_SW_LTSSM_S_HOT_RESET_ENTRY                = 0x1E,
    PS3LIB_SW_LTSSM_S_HOT_RESET                      = 0x1F,
    PS3LIB_SW_LTSSM_S_RCVRY_EQ0                      = 0x20,
    PS3LIB_SW_LTSSM_S_RCVRY_EQ1                      = 0x21,
    PS3LIB_SW_LTSSM_S_RCVRY_EQ2                      = 0x22,
    PS3LIB_SW_LTSSM_S_RCVRY_EQ3                      = 0x23,
    PS3LIB_SW_LTSSM_S_NA,
} Ps3LibSWPortLtssmState_e;

/**
 *  @brief Switch 错误计数
 */
typedef struct Ps3LibSWErrorCounters {
    U32    receiverErrors;              ///< receiver error 计数
    U32    badTLPErrors;                ///< bad TLP 计数
    U32    badDLLPErrors;               ///< bad DLLP 计数
    U32    replayNumberRolloverErrors;  ///< replay_num rollover 计数
    U32    replayTimeoutErrors;         ///< replay timer timeout 计数
    U32    linkDownCount;               ///< link down 计数
    U32    pad[2];                      ///< 保留字段
} Ps3LibSWErrorCounters_s;

/**
 *  @brief Switch Port Prop 请求结构体
 */
typedef struct Ps3LibSWPortPropParam {
    U32    portNum;        ///< port 号
    U32    reserved[3];    ///< 保留字段
} Ps3LibSWPortPropParam_s;

/**
 *  @brief Switch Port Prop 回复结构体
 */
typedef struct Ps3LibSWPortPropInfo {
    U32    portNum;                ///< port 号
    U32    portType;               ///< (enum Ps3LibSWPortType_e)
    U32    maxReadRequestSize;     ///< MRR
    U32    maxPayloadSize;         ///< MPS
    U32    maxPayloadSizeSupport;  ///< 能够支持的最大payload size
    U32    busPrimary;             ///< bus号
    U32    busSecondary;           ///< 次bus号
    U32    reversed[5];            ///< 保留字段
    U8     linkStat;               ///< (enum Ps3LibSWPortLinkStat_e) 建链状态
    U8     linkWidth;              ///< (enum Ps3LibSWPortWidth_e) 建链宽度
    U8     maxLinkWidth;           ///< (enum Ps3LibSWPortWidth_e) 最大宽度
    U8     linkSpeed;              ///< (enum Ps3LibSWPortSpeed_e) 建链速率
    U8     maxLinkSpeed;           ///< (enum Ps3LibSWPortSpeed_e) 最大速率
    U8     clockMode;              ///< (enum Ps3LibSWClockMode_e) 时钟模式
    U8     pad[10];                ///< 保留字段
    Ps3LibSWErrorCounters_s errCounters;    ///< 错误计数
} Ps3LibSWPortPropInfo_s;

/**
 *  @brief Switch Port Perf 请求结构体
 *  @note  建议查询间隔与查询次数乘积不超过30秒
 */
typedef struct Ps3LibSWPortPerfParam {
    U32    portNum;           ///< port 号
    U32    elapsedTimeMs;     ///< 查询间隔, 建议范围0-6000ms
    U32    iterationTimes;    ///< 查询次数, 建议范围1-30次
    U32    reserved;          ///< 保留字段
} Ps3LibSWPortPerfParam_s;

/**
 *  @brief Switch Port Perf 回复结构体
 */
typedef struct Ps3LibSWPortPerfInfo {
    U32    portNum;        ///< port 号
    U32    inMsgCnt;       ///< 输入报文数
    U32    outMsgCnt;      ///< 输出报文数
    U8     isValid;        ///< 当前时刻性能数据是否有效
    U8     pad[3];         ///< 保留字段
    U64    timeStamp;      ///< ms级时间戳
    U32    reserved[2];    ///< 保留字段
} Ps3LibSWPortPerfInfo_s;

/**
 *  @brief Switch 版本信息获取结构体
 */
typedef struct Ps3LibSWFwVersionInfo {
    U8     revisionLevel[PS3LIB_SW_REVISION_LEVEL_LENGTH]; ///< 配置文件字段(Ascii码, 不包含终止符'\0', 例如"00a1")
    U32    bootloaderVersion;       ///< bootloader版本
    U8     fwVersion[32];            ///< 固件版本
    U32    reserved[6];            ///< 保留字段
} Ps3LibSWFwVersionInfo_s;

/**
 *  @brief Switch 更新时间戳请求结构体
 */
typedef struct Ps3LibSWUpdateTimeReq {
    U64    timeStamp;     ///< ms级时间戳
    U8     updateMode;    ///< 时间戳更新模式(0-Not Force, 1-Force)
    U8     reserved[7];   ///< 保留字段
} Ps3LibSWUpdateTimeReq_s;

/**
 *  @brief Switch 更新时间戳回复结构体
 */
typedef struct Ps3LibSWUpdateTimeResp {
    U8    state;          ///< 更新结果(enum Ps3LibSWTimeUpdateState_e)
    U8    reserved[3];    ///< 保留字段
} Ps3LibSWUpdateTimeResp_s;

/**
 *  @brief Switch 状态积请求参数
 */
typedef struct Ps3LibSWPortLtssmPara {
    U32    portNum;       ///< port 号
    U8     count;         ///< 查询次数1-64
    U8     reserved[11];   ///< 保留字段
} Ps3LibSWPortLtssmPara_s;

/**
 *  @brief Switch 状态机信息
 */
typedef struct Ps3LibSWPortLtssmInfo {
    U8     index;           ///< 寄存器索引1-64
    U8     pad[3];          ///< 保留字段
    U8     ltssmStateOld;   ///< 上一状态机(enum Ps3LibSWPortLtssmState_e)
    U8     ltssmStateNew;   ///< 当前状态机(enum Ps3LibSWPortLtssmState_e)
    U8     ltssmSpeedOld;   ///< 状态积速率(enum Ps3LibSWPortSpeed_e)
    U8     ltssmWidthOld;   ///< 状态积宽度(enum Ps3LibSWPortWidth_e)
    U32    ltssmTraceHight; ///< LTSSMTRACE 高8位
    U32    ltssmTraceLow;   ///< LTSSMTRACE 低8位
    U32    reserved[4];     ///< 保留字段
} Ps3LibSWPortLtssmInfo_s;

/**
 *  @brief Switch 状态积信息回复数据
 */
typedef struct Ps3LibSWPortLtssmInfoResp {
    U8 count;               ///< 回复数据个数
    U8 reserved[7];         ///< 保留字段
    Ps3LibSWPortLtssmInfo_s ltssmInfoList[PS3LIB_SW_LTSSM_INFO_MAX_COUNT];  ///< 状态积信息列表
} Ps3LibSWPortLtssmInfoResp_s;

/**
 *  @brief Switch cli命令是否启用枚举
 */
typedef enum Ps3LibSWCmdMaskState { 
    PS3LIB_SW_CMD_MASK_ENABLE   = 0,   ///< 禁用
    PS3LIB_SW_CMD_MASK_DISABLE  = 1,   ///< 启用
    PS3LIB_SW_CMD_MASK_NR,
} Ps3LibSWCmdMaskState_e;

/**
 *  @brief Switch 禁用cli命令请求参数
 */
typedef struct Ps3LibSWCmdMaskPara {
    U32    opcode;          ///< 命令字
    U8     state;           ///< 是否禁用 0：禁用，1：启用
    U8     pad[3];          ///< 对齐
    U64    reserved;        ///< 预留
} Ps3LibSWCmdMaskPara_s;

/**
 *  @brief Switch 更新cmd mask 回复结构体
 */
typedef struct Ps3LibSWUpdateCmdMaskRsp {
    U8    state;          ///< 更新结果
    U8    reserved[7];    ///< 预留
} Ps3LibSWUpdateCmdMaskRsp_s;

/**
 *  @brief Switch 资源数量
 */
typedef struct Ps3LibSWResourceInfo {
    U16 sensorCnt;       ///< 传感器数量
    U16 portCnt;         ///< port数量
    U32 threadCnt;       ///< 线程数量
    U8 flashTableCnt;    ///< flash表数量
    U8 reserved[7];      ///< 预留
} Ps3LibSWResourceInfo_s;

/**
 *  @brief Switch 日期日志
 */
typedef struct Ps3LibSWDateTimeInfo {
    U64 time;               ///< 日期时间
    U64 upTime;             ///< 运行时间
    U64 reserverd[2];       ///< 预留
} Ps3LibSWDateTimeInfo_s;

/**
 *  @brief Switch 获取minicom信息，保存为bin文件
 */
typedef struct Ps3LibSWMinicoreInfo {
    S8 coreData[64*1024];
} Ps3LibSWMinicoreInfo_s;

/**
 * @brief   片内内存使用信息
 */
typedef struct Ps3LibSWHeapInformation {
    U64 number;
    U64 largest;
    U64 total;
} Ps3LibSWHeapInformation_s;

/**
 * @brief   内存状态信息
 */
typedef struct Ps3LibSWHeapStatistics {
    U64 lifetimeAllocated;
    U64 lifetimeFreed;
    U64 size;
    U64 freeSize;
    U64 minFreeSize;
    U32 freeBlocks;
    U32 maxFreeBlocks;
    U32 usedBlocks;
    U32 maxSearch;
    U32 searches;
    U32 allocs;
    U32 failedAllocs;
    U32 frees;
    U32 resizes;
    U8  pad[4];
} Ps3LibSWHeapStatistics_s;

/**
 * @brief   内存使用结构体
 */
typedef struct Ps3LibSWHeapInformationBlock {
    Ps3LibSWHeapInformation_s free;
    Ps3LibSWHeapInformation_s used;
    Ps3LibSWHeapStatistics_s stats;
} Ps3LibSWHeapInformationBlock_s;

/**
 * @brief  片内上传数据结构
 */
typedef struct Ps3LibSWMemoryInfo {
    bool isUnified;                          ///< os资源管理对象内存和通用内存是否统一管理
    U8 pad[7];                               ///< 对齐
    Ps3LibSWHeapInformationBlock_s heapInfo; ///< 内存信息
} Ps3LibSWMemoryInfo_s;

/**
 * @brief sw 片内对象数量请求结构体(公用)，使用U32
 */
typedef struct Ps3LibSWObjectCountReq {
    U32 count;       ///< 片内对象数量
    U8 pad[4];       ///< 预留
} Ps3LibSWObjectCountReq_s;

/**
 * @brief PcieSW flash分区信息
 * @note  迁移的结构体，名称不变
 */
typedef struct Ps3LibPcieSWFlashRegionInfo{
    U8 regionId;        ///< 分区id
    S8 regionType[20];  ///< 分区名称，每个占用20字节
    U8 reserved[3];     ///< 预留
    U32 regionOffset;   ///< 分区偏移
    U32 regionSize;     ///< 分区大小Size（KB）
} Ps3LibPcieSWFlashRegionInfo_t;

/**
 * @brief PcieSW flash分区整体信息
 * @note  迁移的结构体，名称不变
 */
typedef struct Ps3LibPcieSWFlashTblInfo{
    U32 norflashSize;   ///< flash size
    U32 startAddr;      ///< flash 起始地址
    U32 regionCnt;      ///< region分区数量
    U32 pad;            ///< 对齐
    Ps3LibPcieSWFlashRegionInfo_t flashRegionInfo[0];   ///< flash分区数组
} Ps3LibPcieSWFlashTblInfo_t;

/**
 * @brief PcieSW 端口信息
 */
typedef struct Ps3LibSWPortPropAllInfo {
    U32 portCnt;        ///< port数量
    U32 pad;            ///< 对齐
    Ps3LibSWPortPropInfo_s   portPropInfo[0];   ///< 端口信息
} Ps3LibSWPortPropAllInfo_s ;

/**
 * @brief HostView信息结构体单个
 */
typedef struct Ps3LibSWHostView{
    U32 portNum;
    U16 devID;
    U16 venID;
    U8  bus;
    U8  devNum;
    U8  function;
    U8  linkStat;
    U8  maxWidth;
    U8  width;
    U8  maxSpeed;
    U8  speed;
    U32 mrr;
    U32 mps;
} Ps3LibSWHostView_s;

/**
 * @brief HostView信息结构体(全部)
 */
typedef struct Ps3LibSWHostViewInfo {
    U32 portCnt;                        ///< port数量
    U32 pad;                            ///< 对齐
    Ps3LibSWHostView_s hostviewInfo[0]; ///< hostview信息
} Ps3LibSWHostViewInfo_s;

/**
 * @brief   片内线程属性信息
 * @note    查询片内线程信息
 */
typedef struct Ps3LibSWThreadInfo{
    U32 taskID;                                 ///< 线程ID
    S8  taskName[PS3LIB_SW_NAME_BUF_LEN];       ///< 4+1 线程名称    
    S8  pad1[3];    
    S8  schedulerName[PS3LIB_SW_NAME_BUF_LEN];  ///< 4+1线程所属调度器名称
    S8  pad2[3];                
    U32 taskPriority;                   ///< 线程优先级(如果不是通过osp接口创建的任务，优先级无效)    
    U32 taskState;                      ///< 线程当前状态，参考osp_task.h的OSP_STATES_xxx        
    U32 events;                         ///< 线程收到的事件       
    U64 stackLow;                       ///< 线程堆栈的低地址    
    U64 stackHigh;                      ///< 线程堆栈的高地址    
    U64 stackCurr;                      ///< 线程当前栈
    U32 maxStackUse;                    ///< 线程栈历史最大开销
    U32 cpuUseSec;                      ///< 线程执行时间统计(seconds)，ospCliResetCpuUsage重新开始统计    
    U32 cpuUseNanosec;                  ///< 线程执行时间统计(nanoseconds)，ospCliResetCpuUsage重新开始统计
    U32 cpuUseIval;                     ///< 线程执行时间占比(整数部分)，ospCliResetCpuUsage重新开始统计
    U32 cpuUseFval;                     ///< 线程执行时间占比(小数部分)，ospCliResetCpuUsage重新开始统计   
    U8  pad3[4];
} Ps3LibSWThreadInfo_s;

/**
 * @brief   片内线程属性信息
 * @note    查询片内线程信息
 */
typedef struct Ps3LibSWThreadAllInfo{
    U32 taskCnt;                            ///< 片内线程数量
    U32 pad;                                ///< 对齐
    Ps3LibSWThreadInfo_s pThreadInfo[0];    ///< 片内各个线程统计信息
} Ps3LibSWThreadAllInfo_s;

/**
 * @brief sensor type 枚举
 */
typedef enum Ps3LibSwSensorType {
    PS3LIB_SWITCH_SENSOR_TEMPERATURE = 0,   ///< 温度传感器类型
    PS3LIB_SWITCH_SENSOR_VOLTAGE,           ///< 电压传感器
    PS3LIB_SWITCH_SENSOR_NR,
} Ps3LibSwSensorType_e;

/**
 * @brief 传感器相关结构体 单个
 */
typedef struct Ps3LibSWSensorInfoSingle {
    S16 value;                  ///< 传感器的值
    U8  sensorType;             ///< 器件类型，无效值为0xFF, 使用
    U8  pad1[13];                ///< 预留
} Ps3LibSWSensorInfoSingle_t;

/**
 * @brief 传感器相关结构体
 */
typedef struct Ps3LibSWSensorInfo {
    U16 sensorNum;                          ///< 传感器数量
    U16 pad0[7];                            ///< 预留
    Ps3LibSWSensorInfoSingle_t sensor[0];   ///< sensor提供的信息
} Ps3LibSWSensorInfo_t;

/**
 * @brief termlog get请求结构体
 */
typedef struct Ps3LibSWTermLogReq {
    U8  beginFlag;   ///< 日志开始标志
    U8  capMax;      ///< 请求下载的termlog占最大容量的百分比 枚举Ps3LibCtrlGetTermlogCapMaxQtr; 0和全F，片内按照默认值
    U8  pad[2];
    S32 sessionId;   ///< 会话ID
    U32 dmaLength;   ///< 申请大小
} Ps3LibSWTermLogReq_t;

/**
 * @brief termlog信息结构体
 */
typedef struct Ps3LibSWTermLogInfo {
    U32 magic;                               ///< 魔数
    U8  logEndFlag;                          ///< 日志结束标志
    U8  reserved;                            ///< 保证内存对齐
    U16 filelength;                          ///< 日志文件大小
    S32 sessionId;                           ///< 会话ID
    S8  data[0];                             ///< 日志文件内容
} Ps3LibSWTermLogInfo_t;

/**
 * @brief SW oob读取事件日志请求结构体
 */
typedef struct Ps3LibSWEventReq {
    U32 reqSize;   ///< 请求数据大小
    U32 reqOffset; ///< 事件日志读取偏移
    U32 reqLocal;  ///< 读取日志分区标识
} Ps3LibSWEventReq_s;

/**
 * @brief SW oob读取事件日志回复数据
 */
typedef struct Ps3LibSWEventDetailList {
    Ps3LibEvtPersistInfo_s  persistInfo;
    U32                     rspSize;        ///< 应答事件大小
    U16                     evtHeaderLen;   ///< 事件日志头长度 sizeof(MglEvtLogHeader_s)
    U16                     evtMaxLen;      ///< 事件日志最大长度 sizeof(MglEvtLogEntry_s)
    S8                      rspInfo[0];     ///< 应答数据
} Ps3LibSWEventDetailList_s;

///< 端口的链接状态
typedef enum Ps3LibSWLinkStat {
    PS3LIB_SW_PORT_LINK_STAT_DOWN  = 0,      ///< LINK DOWN
    PS3LIB_SW_PORT_LINK_STAT_UP,             ///< LINK UP
    PS3LIB_SW_PORT_LINK_STAT_NA,             ///< invaild stat
} Ps3LibSWLinkStat_e;

/**
 * @brief PcieSW port linkStat信息
 */
typedef struct Ps3LibSWPortLinkStatEntry {
    U32 portNum;
    U16 dieiID;
    U16 groupID;
    U8 portID;
    U8 linkStat;
    U8 pad[2];
} Ps3LibSWPortLinkStatEntry_s;

/**
 * @brief 所有port linkStat 信息结构体
 */
typedef struct Ps3LibSWAllPortLinkInfo {
    U32 portCnt;            ///< 端口数量
    Ps3LibSWPortLinkStatEntry_s portLinkStat[PS3LIB_SW_PORT_MAX]; ///< 端口信息
} Ps3LibSWAllPortLinkInfo_s;

/**
 * @brief 所有port linkStat 信息结构体
 */
typedef struct Ps3LibSWAllPortLinkParam {
    U32 type;   ///< ture：获取详细； false || 0xffffffff:获取简略
} Ps3LibSWAllPortLinkParam_s;

/**
 * @brief       获取 Switch 基础信息
 * @param[in]   devId:     控制卡标识符
 * @param[in]   addr:      请求地址
 *              (0x74=EXP_MCU_MANU, 0x9b=Manufacturer information, 0x98=Wheather to support upgrade
 *              0x96=Version, 0xa0=Temperature0, 0xa1=Temperature1)
 * @param[out]  pRespInfo: 用户接受回复信息结构体
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSWGetBaseInfo(CtrlId_t devId, U8 addr, Ps3LibSWBaseInfo_s *pRespInfo);

/**
 * @brief       Switch事件日志订阅
 * @param[in]   pEventSub: 事件日志订阅结构体
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSWEventReg(Ps3LibSWEventSubscribe_t *pEventSub);

/**
 * @brief       Switch事件日志取消订阅
 * @param[in]   uniqueId: 已订阅Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSWEventUnReg(U32 uniqueId);

/**
 * @brief       获取Switch下mfg信息
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pMfgInfo: 获取到的生产信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetMfgInfo(CtrlId_t devId, Ps3LibSwMfgInfo_t *pMfgInfo);

/**
 * @brief       获取Switch下端口属性信息
 * @param[in]   devId:       控制卡标识符
 * @param[in]   pPropParam:  属性信息请求参数
 * @param[out]  pPropInfo:   属性信息回复参数
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetPortPropInfo(CtrlId_t devId, Ps3LibSWPortPropParam_s *pPropParam,
    Ps3LibSWPortPropInfo_s *pPropInfo);

/**
 * @brief       获取Switch下端口性能信息
 * @param[in]   devId:       控制卡标识符
 * @param[in]   pPerfParam:  性能信息请求参数
 * @param[out]  pPerfInfo:   性能信息回复参数
 * @param[in]   rspLen:      回复数据长度: sizeof(Ps3LibSWPortPerfInfo_s) * 查询个数
 * @warning     该命令为阻塞接口, 阻塞时间取决于查询次数与查询间隔的乘积
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetPortPerfInfo(CtrlId_t devId, Ps3LibSWPortPerfParam_s *pPerfParam,
    Ps3LibSWPortPerfInfo_s *pPerfInfo, U32 rspLen);

/**
 * @brief       获取Switch版本信息
 * @param[in]   devId:        控制卡标识符
 * @param[out]  pVersionInfo: 版本信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetVersionInfo(CtrlId_t devId, Ps3LibSWFwVersionInfo_s *pVersionInfo);

/**
 * @brief       获取Switch时间戳
 * @param[in]   devId:     控制卡标识符
 * @param[out]  timeStamp: 版本信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetTimeStamp(CtrlId_t devId, U64 *timeStamp);

/**
 * @brief       更新Switch时间戳
 * @param[in]   devId:     控制卡标识符
 * @param[out]  timeStamp: 版本信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwSetTimeStamp(CtrlId_t devId, Ps3LibSWUpdateTimeReq_s *pTimeReq,
    Ps3LibSWUpdateTimeResp_s *pTimeResp);

/**
 * @brief       获取lsttmInfo
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pLtssmParam: 获取ltssm信息请求参数
 * @param[out]  pLtssmInfoResp: ltssm信息回复数据
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetLtssmInfo(CtrlId_t devId, Ps3LibSWPortLtssmPara_s *pLtssmParam,
    Ps3LibSWPortLtssmInfoResp_s *pLtssmInfoResp);

/**
 * @brief       cli命令是否启用设置
 * @param[in]   devId:     控制卡标识符
 * @param[in]   pCmdMask:  cli命令是否启用入参
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwSetCmdMask(CtrlId_t devId, Ps3LibSWCmdMaskPara_s *pCmdMask);

/**
 * @brief       获取sw资源数量
 * @param[in]   devId:     控制卡标识符
 * @param[in]   pResourceInfo:  资源对象的数量
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetResourceInfo(CtrlId_t devId, Ps3LibSWResourceInfo_s *pResourceInfo);

/**
 * @brief       获取sw的minicore信息
 * @param[in]   devId:     控制卡标识符
 * @param[in]   pMinicoreInfo:  minicore信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetMinicoreInfo(CtrlId_t devId, Ps3LibSWMinicoreInfo_s *pMinicoreInfo);

/**
 * @brief       获取sw的日期信息
 * @param[in]   devId:     控制卡标识符
 * @param[in]   pDateTimeInfo:  日期时间信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetDateTimeInfo(CtrlId_t devId, Ps3LibSWDateTimeInfo_s *pDateTimeInfo);

/**
 * @brief       获取sw的内存信息
 * @param[in]   devId:     控制卡标识符
 * @param[in]   pMemoryInfo:  内存信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetMemoryInfo(CtrlId_t devId, Ps3LibSWMemoryInfo_s *pMemoryInfo);

/**
 * @brief       获取sw的flash table info
 * @param       devId:     控制卡标识符
 * @param       pCountReq:  片内对象数量
 * @param       pFlashTblInfo:  FlashTblInfo信息
 * @param       pFalshTblInfoSize:  FlashTblInfo信息的大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetFlashTblInfo(CtrlId_t devId, Ps3LibSWObjectCountReq_s *pCountReq,
                                 Ps3LibPcieSWFlashTblInfo_t *pFlashTblInfo, U32 falshTblInfoSize);

/**
 * @brief       获取sw的port info
 * @param       devId:     控制卡标识符
 * @param       pCountReq:  片内对象数量
 * @param       pPortInfo:  端口属性信息
 * @param       portInfoSize:  端口属性信息的大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetPortInfo(CtrlId_t devId, Ps3LibSWObjectCountReq_s *pCountReq,
                             Ps3LibSWPortPropAllInfo_s *pPortInfo, U32 portInfoSize);

/**
 * @brief       获取sw的hostview info
 * @param[in]   devId:     控制卡标识符
 * @param       pCountReq:  片内对象数量
 * @param[in]   pHostviewInfo:  hostview信息
 * @param[in]   hostviewInfoSize:  hostview信息的大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetHostviewInfo(CtrlId_t devId, Ps3LibSWObjectCountReq_s *pCountReq,
                                 Ps3LibSWHostViewInfo_s *pHostviewInfo, U32 hostviewInfoSize);

/**
 * @brief       获取sw的thread info
 * @param[in]   devId:     控制卡标识符
 * @param       pCountReq:  片内对象数量
 * @param[in]   pThreadInfo:  线程信息
 * @param[in]   threadInfoSize:  线程信息的大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetThreadInfo(CtrlId_t devId, Ps3LibSWObjectCountReq_s *pCountReq,
                               Ps3LibSWThreadAllInfo_s *pThreadInfo, U32 threadInfoSize);

/**
 * @brief       获取sw的sensor info
 * @param[in]   devId:     控制卡标识符
 * @param[in]   pCountReq:  片内对象数量
 * @param[in]   pSensorInfo:  传感器信息
 * @param[in]   sensorInfoSize:  传感器信息的大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetSensorInfo(CtrlId_t devId, Ps3LibSWObjectCountReq_s *pCountReq,
                               Ps3LibSWSensorInfo_t *pSensorInfo, U32 sensorInfoSize);

/**
 * @brief       获取sw的termlog日志
 * @param[in]   devId:     控制卡标识符
 * @param[in]   pTermlogReq:  termlog请求参数
 * @param[in]   pTemlogInfo:  termlog返回结构体
 * @param[in]   pTermlogfoSize:  termlog返回内容的大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetTermlogInfo(CtrlId_t devId, Ps3LibSWTermLogReq_t *pTermlogReq,
                               Ps3LibSWTermLogInfo_t *pTemlogInfo, U32 termlogfoSize);

/**
 * @brief       获取sw的eventLog 持久化信息
 * @param[in]   devId:     控制卡标识符
 * @param[out]  pPersistInfo:  事件日志的持久化信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetPersistLogInfo(CtrlId_t devId, Ps3LibEvtPersistInfo_s *pPersistInfo);

/**
 * @brief       获取sw的eventLog日志
 * @param[in]     devId:     控制卡标识符
 * @param[in]     pEventReq:  事件日志的请求信息
 * @param[in/out] pEventRsp:  事件日志的回复信息
 * @param[in/out] pEventSize: 期望恢复数据的大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwOobGetEventLogInfo(CtrlId_t devId, Ps3LibSWEventReq_s *pEventReq,
                                 Ps3LibSWEventDetailList_s *pEventRsp, U32 eventRspSize);
/**
 * @brief       将sw的日志保存到指定路径
 * @param[in]   devId:     控制卡标识符
 * @param[in]   filePath:  事件日志的请求信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwCollectionAllLogInfo(CtrlId_t devId, S8 *filePath);

/**
 * @brief       获取所有建链的端口的信息
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pLinkParam: 获取link stat信息请求参数
 * @param[out]  pPortLinkInfo: link stat信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwGetPortLinkInfo(CtrlId_t devId, Ps3LibSWAllPortLinkParam_s *pLinkParam,
                                Ps3LibSWAllPortLinkInfo_s *pPortLinkInfo);
#if defined(__cplusplus)
}
#endif

#endif
