/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_expander.h
 * @date    2023.04.28
 * @brief   expander信息
 */

#ifndef __PS3LIB_EXPANDER_H__
#define __PS3LIB_EXPANDER_H__

#include "ps3lib_event.h"

#define PS3LIB_EXP_MAX_PHYSICAL_PHYS_NUM       (48)    ///< EXP最大phy个数

#define PS3LIB_EXP_PDINFO_MAX_LEN (sizeof(Ps3LibExpBatchPdInfo_s) + \
    (PS3LIB_EXP_MAX_PHYSICAL_PHYS_NUM * PS3LIB_EXP_PD_INFO_SIZE)) 
///< 硬盘数据需要的最大内存，回传结构体+ (exp最大phy个数*单个硬盘信息大小)

#define PS3LIB_EXP_PD_INFO_SIZE sizeof(Ps3LibExpPdInfo_s)    ///< Ps3LibExpPdInfo_s 结构体大小宏定义

enum {
    PS3LIB_EXP_PD_VENDOR_ID_LEN        = 16,    ///< inquiry standard格式中，厂商ID域的长度
    PS3LIB_EXP_PD_PRODUCT_ID_LEN       = 40,    ///< inquiry standard格式中，产品ID域的长度
    PS3LIB_EXP_PD_PRODUCT_REVISION_LEN = 8,     ///< inquiry standard格式中，产品版本域的长度
    PS3LIB_EXP_PD_UNIT_SERIAL_NUM_LEN  = 20,    ///< inquiry standard格式中，硬盘的SN号长度
};

#define PS3LIB_EP_MAX_BASEINFO_RSP_SIZE (4)          ///< Expander基础信息最大回复长度
#define PS3LIB_EXP_EVENT_DESC_64              (64)   ///< 事件描述字符串长度

/**
 * @brief zone功能开关状态
 */
typedef enum Ps3LibExpZoneEnable {
    PS3LIB_EXP_ZONE_DISABLE = 0x0, ///< zone功能关闭
    PS3LIB_EXP_ZONE_ENABLE  = 0x1, ///< zone功能开启
    PS3LIB_EXP_ZONE_NR,
} Ps3LibExpZoneEnable_e;

/**
 * @brief Discover 返回字段邻接设备类型定义(SPL-5协议9.4.4章节)
 */
typedef enum Ps3LibExpPhyAttachedType {
    PS3LIB_EXP_SMP_DISCOVER_ATTACHED_DEVICE_TYPE_NO_DEVICES_ATTACHED   = 0x0,    ///< 无邻接设备
    PS3LIB_EXP_SMP_DISCOVER_ATTACHED_DEVICE_TYPE_END_DEVICE_ONLY       = 0x1,    ///< 终端设备
    PS3LIB_EXP_SMP_DISCOVER_ATTACHED_DEVICE_TYPE_EDGE_EXPANDER         = 0x2,    ///< 边沿扩展器
    PS3LIB_EXP_SMP_DISCOVER_ATTACHED_DEVICE_TYPE_FANOUT_EXPANDER       = 0x3,    ///< 扇出扩展器
    PS3LIB_EXP_SMP_DISCOVER_ATTACHED_DEVICE_TYPE_NR,
} Ps3LibExpPhyAttachedType_e;

/**
 * @brief Discover 返回字段协商物理链路速率定义(SPL-5协议9.4.4章节)
 */
typedef enum Ps3LibExpPhySpeed {
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_UNKNOWN_RATE         = 0x0,    ///< 未知速率
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_DISABLED                     = 0x1,    ///< 未启用
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_SPEED_NEG_FAILED     = 0x2,    ///< 速率获取故障
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_IN_SATA_SPINUP_HOLD  = 0x3,    ///< sata 起转保持
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_PORT_SELECTOR        = 0x4,    ///< 端口选择
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_RESET_IN_PROGRESS    = 0x5,    ///< 正在重置
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_UNSUPPORTED_PHY_ATTACHED = 0x6,    ///< 不支持邻接设备
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_AT_1_5_GBPS          = 0x8,    ///< 速率为 1.5Gbps
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_AT_3_0_GBPS          = 0x9,    ///< 速率为 3.0Gbps
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_AT_6_0_GBPS          = 0xa,    ///< 速率为 6.0Gbps
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_AT_12_0_GBPS         = 0xb,    ///< 速率为 12.0Gbps
    PS3LIB_EXP_SMP_NEG_PHYS_LINK_RATE_PHY_ENABLED_NR,
} Ps3LibExpPhySpeed_e;

/**
 *  @brief 设备类型，HDD、SSD
 */
typedef enum Ps3LibExpPdMediaType {
    PS3LIB_EXP_PD_DEVICE_TYPE_UNKNOWN = 0,        ///< 设备类型未知
    PS3LIB_EXP_PD_DEVICE_TYPE_HDD,                ///< HDD类型
    PS3LIB_EXP_PD_DEVICE_TYPE_SSD,                ///< SSD类型
    PS3LIB_EXP_PD_DEVICE_TYPE_NR,
} Ps3LibExpPdMediaType_e;

/**
 *  @brief pd phy 信息
 */
typedef struct Ps3LibExpPhyInfo {
    U8      slotId;             ///< 硬盘槽位号
    U8      phyId;              ///< 硬盘phy id
    U32     speed;              ///< 硬盘链接速率 ExpPhySpeed_e
    U32     zoneEnable;         ///< zone功能是否开启 ExpZoneEnable_e
    U8      zoneGroup;          ///< phy所在的zone group 0-255
    U64     attachedSasAddr;    ///< phy对端的sas address
    U8      attachedPhy;        ///< 对端的phy
    U32     attachedType;       ///< phy对应的类型 ExpPhyAttachedType_e
    U8      attachedSmpi:1,     ///< 对端phy是否为smpi
            attachedStpi:1,     ///< 对端phy是否为stpi
            attachedSspi:1,     ///< 对端phy是否为sspi
            attachedSmpt:1,     ///< 对端phy是否为smpt
            attachedStpt:1,     ///< 对端phy是否为stpt
            attachedSspt:1,     ///< 对端phy是否为sspt
            sataDevice:1,       ///< 对端phy是否为sata device
            reserved:1;
    U8      pad[3];
} Ps3LibExpPhyInfo_s;

/**
 *  @brief pd 基础信息
 */
typedef struct Ps3LibExpPdBaseInfo {
    U8      present;            ///< 硬盘在位
    U8      pad[3];             ///< 保留字段
    U64     WWN;                ///< 硬盘的World Wide Name
    U64     sasAddr;            ///< 硬盘的sas address
    U64     totalSize;          ///< 硬盘容量，单位：字节 totalSize=rawSize*sectorSize
    U64     rawSize;            ///< 硬盘容量，单位：扇区
    U16     physicalSectorSize; ///< 硬盘物理扇区大小
    U16     sectorSize;         ///< 硬盘逻辑扇区大小
    U8      manufacureId[PS3LIB_EXP_PD_VENDOR_ID_LEN];         ///< 硬盘的厂商ID
    U8      modelNumber[PS3LIB_EXP_PD_PRODUCT_ID_LEN];         ///< 硬盘的产品ID
    U8      fwRevision[PS3LIB_EXP_PD_PRODUCT_REVISION_LEN];    ///< 硬盘的固件版本
    U8      serialNumber[PS3LIB_EXP_PD_UNIT_SERIAL_NUM_LEN];   ///< 硬盘的SN号
    U32     mediaType;          ///< 硬盘的媒介类型 Ps3LibExpPdMediaType_e
} Ps3LibExpPdBaseInfo_s;

/**
 *  @brief pd smart信息
 */
typedef struct Ps3LibExpPdSmartInfo {
    U8      temperature;        ///< 硬盘当前温度
    U8      pad[3];             ///< 保留字段
} Ps3LibExpPdSmartInfo_s;

/**
 *  @brief pd 信息
 */
typedef struct Ps3LibExpPdInfo {
    Ps3LibExpPhyInfo_s        phyInfo;        ///< 硬盘所在phy相关信息
    Ps3LibExpPdBaseInfo_s     pdBaseInfo;     ///< 硬盘基本信息
    Ps3LibExpPdSmartInfo_s    pdSmartInfo;    ///< 硬盘smart信息
} Ps3LibExpPdInfo_s;

/**
 *  @brief 批量硬盘信息
 */
typedef struct Ps3LibExpBatchPdInfo {
    U8              pdCnt;      ///< 在位硬盘个数
    U32             dataLen;    ///< 在位硬盘信息数据长度
    U8              pdData[0];  ///< 所有在位硬盘信息
} Ps3LibExpBatchPdInfo_s;

/**
 *  @brief Expander 基础信息结构体
 */
typedef struct Ps3LibBaseInfo {
    U32 dataLen;               ///< 基础信息数据长度
    S8  data[0];               ///< 基础信息数据
} Ps3LibBaseInfo_s;

/**
 *  @brief Expander 事件设备类型
 */
typedef enum Ps3LibEpEventDevType
{ 
    PS3LIB_EVT_TYPE_UNKNOW   = 0,   ///< 事件类型未知
    PS3LIB_EVT_TYPE_EXPANDER = 1,   ///< 事件类型为Expander
    PS3LIB_EVT_TYPE_SWITCH   = 2,   ///< 事件类型为Switch
    PS3LIB_EVT_TYPE_NR,
}Ps3LibEpEventDevType_e; 

/**
 *  @brief Expander 事件类型
 */
typedef enum Ps3LibEpEventType
{ 
    PS3LIB_EVT_LOCAL           = 1, ///< local 事件类型
    PS3LIB_EVT_PD_COUNT_LOCAL  = 2, ///< pd count 事件类型
    PS3LIB_EVT_CTRL_INFO_LOCAL = 3, ///< ctrl info 事件类型
    PS3LIB_EVT_EXP_LOCAL       = 4, ///< expander info 事件类型
    PS3LIB_EVT_EXP_PHY_LOCAL   = 5, ///< expander phy info 事件类型
    PS3LIB_EVT_EXP_CONN_LOCAL  = 6, ///< expander connect info 事件类型
    PS3LIB_EVT_EXP_INFO_LOCAL  = 7, ///< ctrl info 事件类型
    PS3LIB_EVT_MAX_TYPE_LOCAL  = 8, ///< expander 所有事件类型
    PS3LIB_EVT_TYPE_MAX,
}Ps3LibEpEventType_e; 

/**
 *  @brief Expander 事件回复信息结构体
 */
typedef struct Ps3LibEpCmdEventInfo {
    U32                  uniqueId;         ///< 订阅ID
    U32                  seqNum;           ///< 日志序号
    U32                  timeStampBySec;   ///< 秒级时间戳 since 1970
    U16                  evtCode;          ///< 事件代号 0-4095
    U16                  pad;              ///< 保留字段
    Ps3LibEpEventType_e  eventType;        ///< 事件类型
    Ps3LibEpEventLevel_e evtLevel;         ///< 事件级别
    S8                   eventDesc[PS3LIB_EXP_EVENT_DESC_64];    ///< 事件描述
    S32                  kvCount;          ///< expander补充信息数量
    Ps3LibEventDataCollection_s  collection;     ///< expander补充信息
} Ps3LibEpCmdEventInfo_t;

/**
 *  @brief Expander 事件订阅结构体
 */
typedef struct Ps3LibEpEventSubscribe {
    U16                     devlId;      ///< 设备地址 低八位为bus 高八位为address
    U8                      pad[2];      ///< 保留字段
    Ps3LibEpEventDevType_e  devType;     ///< 设备类型
    Ps3LibEpEventType_e     evtType;     ///< 事件类型 PS3LIB_EVT_MAX_TYPE_LOCAL 表示获取所有
    Ps3LibEpEventLevel_e    evtLevel;    ///< 事件级别 PS3LIB_EVT_CLASS_MAX 表示获取所有
    Ps3Errno (*getEvtInfo)(Ps3LibEpCmdEventInfo_t *evtInfo);  ///< 获取事件日志回调
} Ps3LibEpEventSubscribe_t;

/**
 * @brief   厂商分区字段长度
 */
enum {
    PS3LIB_EXP_MFG_PART_NUMBER_LEN   = 16, ///< 序列号长度
    PS3LIB_EXP_MFG_WWN_LEN           = 16, ///< 卡标识符长度
    PS3LIB_EXP_MFG_REVISION_LEN      = 4,  ///< 反工版本号长度
    PS3LIB_EXP_MFG_OEM_STR_LEN       = 64, ///< oem字段长度
};

/**
 * @brief   厂商分区保存的生产字段
 */
typedef struct Ps3LibExpMfgInfo {
    U8  partNumber[PS3LIB_EXP_MFG_PART_NUMBER_LEN];     ///< part number 16-byte ASCII string
    U8  serialNumber[PS3LIB_EXP_MFG_PART_NUMBER_LEN];   ///< 序列号
    U64 sasAddrBase;                                    ///< Not Support
    U32 mfgDate;                                        ///< U32 时间戳
    U8  revision[PS3LIB_EXP_MFG_REVISION_LEN];          ///< 版本号
    U8  wwn[PS3LIB_EXP_MFG_WWN_LEN];                    ///< 卡的唯一标识符
    U32 reworkDate;                                     ///< 返工时间 时间戳
    U8  pad[4];                                         ///< 保留字段
    U8  extra1[PS3LIB_EXP_MFG_OEM_STR_LEN];             ///< oem字段
    U8  extra2[PS3LIB_EXP_MFG_OEM_STR_LEN];             ///< oem字段
} Ps3LibExpMfgInfo_t;

/**
 * @brief       获取Expander下pd信息
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pResp: 用户接受回复的批量pd信息
 * @param[out]  pRespLen: 用户接受回复信息的实际长度
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libEpGetPdInfo(U16 devId, Ps3LibExpBatchPdInfo_s *pResp, U32 *pRespLen);

/**
 * @brief       获取 Expander 基础信息
 * @param[in]   devId:     控制卡标识符
 * @param[in]   addr:      请求地址
 *              (0x74=EXP_MCU_MANU, 0x9b=Manufacturer information, 0x98=Wheather to support upgrade
 *              0x96=Version, 0xa0=Temperature0, 0xa1=Temperature1)
 * @param[out]  pRespInfo: 用户接受回复信息结构体
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libEpGetBaseInfo(U16 devId, U8 addr, Ps3LibBaseInfo_s *pRespInfo);

/**
 * @brief       Expander事件日志订阅
 * @param[in]   pEventSub: 事件日志订阅结构体
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libEpEventReg(Ps3LibEpEventSubscribe_t *pEventSub);

/**
 * @brief       Expander事件日志取消订阅
 * @param[in]   uniqueId: 已订阅Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libEpEventUnReg(U32 uniqueId);

/**
 * @brief       获取Expander下mfg信息
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pMfgInfo: 生产信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libEpGetMfgInfo(U16 devId, Ps3LibExpMfgInfo_t *pMfgInfo);

#endif
