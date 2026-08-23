/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_pcieswitch.h
 * @date    2023.04.28
 * @brief   switch信息
 */

#ifndef __PS3LIB_PCIESWITCH_H__
#define __PS3LIB_PCIESWITCH_H__

#define PS3LIB_SW_MAX_BASEINFO_RSP_SIZE (4)          ///< Switch基础信息最大回复长度
#define PS3LIB_SW_EVENT_DATA_COLLECT_MAX_NUM (256)  ///< Switch 补充收集信息数量
#define PS3LIB_SW_EVENT_DESC_64              (64)   ///< 事件描述字符串长度

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
    U16                     devlId;      ///< 设备地址 低八位为bus 高八位为address
    U8                      pad[2];      ///< 保留字段
    Ps3LibSWEventDevType_e  devType;     ///< 设备类型
    Ps3LibSWEventType_e     evtType;     ///< 事件类型 PS3LIB_EVT_MAX_TYPE_LOCAL 表示获取所有
    Ps3LibSWEventLevel_e    evtLevel;    ///< 事件级别 PS3LIB_EVT_CLASS_MAX 表示获取所有
    Ps3Errno (*getEvtInfo)(Ps3LibSWCmdEventInfo_t *evtInfo);  ///< 获取事件日志回调
} Ps3LibSWEventSubscribe_t;

/**
 * @brief       获取 Switch 基础信息
 * @param[in]   devId:     控制卡标识符
 * @param[in]   addr:      请求地址
 *              (0x74=EXP_MCU_MANU, 0x9b=Manufacturer information, 0x98=Wheather to support upgrade
 *              0x96=Version, 0xa0=Temperature0, 0xa1=Temperature1)
 * @param[out]  pRespInfo: 用户接受回复信息结构体
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSWGetBaseInfo(U16 devId, U8 addr, Ps3LibSWBaseInfo_s *pRespInfo);

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
Ps3Errno ps3libSwGetMfgInfo(U16 devId, Ps3LibSwMfgInfo_t *pMfgInfo);

#endif
