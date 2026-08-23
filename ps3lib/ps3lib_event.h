/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_event.h
 * @date    2023.05.22
 * @brief   磁盘信息
 */

#ifndef __PS3LIB_EVENT_H__
#define __PS3LIB_EVENT_H__

#define PS3LIB_MAX_VD_NAME_BYTES             (16)   ///< 最大允许VD名称长度
#define PS3LIB_CTRL_AUTOCONFIG_EVTDATA_SIZE  (8)    ///< 事件数据大小
#define PS3LIB_BBM_ERRTBL_NAME_LEN           (6)    ///< 坏块表类型名称长度
#define PS3LIB_EVT_DESC_MAX_LEN              (4096) ///< 事件日志描述最大长度
#define PS3LIB_FGI_MODE_LEN                  (5)
#define PS3LIB_EVT_LOG_INFO_MAX_SIZE         (116)
#define PS3LIB_EXP_EVENT_DATA_COLLECT_MAX_NUM (256) ///< 补充收集信息数量
#define PS3LIB_MAX_EVENT_REG_CNT              (32)   ///< 最大允许订阅数

/**
 * @brief   事件日志读取位置
 */
enum {
    PS3LIB_EVT_LOG_OLDEST,        ///< 从最旧日志读取
    PS3LIB_EVT_LOG_LATEST,        ///< 从最新日志读取
    PS3LIB_EVT_LOG_LAST_CLEAR,    ///< 从最近清除日志读取
    PS3LIB_EVT_LOG_LAST_REBOOT,   ///< 从最近重启日志读取
    PS3LIB_EVT_LOG_LAST_SHUTDOWN, ///< 从最近关机日志读取
    PS3LIB_EVT_LOG_FATAL_OLDEST,  ///< 从最旧FATAL日志读取
    PS3LIB_EVT_LOG_FATAL_LATEST,  ///< 从最新FATAL日志读取
    PS3LIB_EVT_LOG_LAST_MAX,
};

/**
 *  @brief 事件级别
 */
typedef enum Ps3LibEpEventLevel
{
    PS3LIB_EVT_CLASS_UNKNOWN   = 0b0000,    ///< 未知级别事件
    PS3LIB_EVT_CLASS_DEBUG     = 0b0011,    ///< debug 级别事件
    PS3LIB_EVT_CLASS_PROCESS   = 0b0101,    ///< process 级别事件
    PS3LIB_EVT_CLASS_INFO      = 0b0001,    ///< Info 级别事件
    PS3LIB_EVT_CLASS_WARNING   = 0b0010,    ///< Warning 级别事件
    PS3LIB_EVT_CLASS_CRITICAL  = 0b0100,    ///< Critical 级别事件
    PS3LIB_EVT_CLASS_FATAL     = 0b1000,    ///< Fatal 级别事件
    PS3LIB_EVT_CLASS_MAX,
} Ps3LibEpEventLevel_e;

/**
 *  @brief 事件local
 */
enum
{ 
    PS3LIB_CTRL_EVT_SAS_INFO_LOCAL = 1,
    PS3LIB_CTRL_EVT_PD_COUNT_LOCAL = 2,
    PS3LIB_CTRL_EVT_VD_COUNT_LOCAL = 3,
    PS3LIB_CTRL_EVT_CTRL_INFO_LOCAL = 4,
    PS3LIB_CTRL_EVT_PD_ATTR_LOCAL = 5,
    PS3LIB_CTRL_EVT_VD_ATTR_LOCAL = 6,
    PS3LIB_CTRL_EVT_DG_INFO_LOCAL = 7,
    PS3LIB_CTRL_EVT_BBU_INFO_LOCAL = 8,
    PS3LIB_CTRL_EVT_CONFIG_LOCAL = 9,
    PS3LIB_CTRL_EVT_IO_INFO_LOCAL = 10,
    PS3LIB_CTRL_EVT_UKEY_INFO_LOCAL = 11,
    PS3LIB_CTRL_EVT_HWR_INFO_LOCAL = 12,
    PS3LIB_CTRL_EVT_ALARM_INFO_LOCAL = 13,
    PS3LIB_CTRL_EVT_ECC_INFO_LOCAL = 14,
    PS3LIB_CTRL_EVT_UPGRADE_INFO_LOCAL = 15,
    PS3LIB_CTRL_EVT_TEMP_INFO_LOCAL = 16,
    PS3LIB_CTRL_EVT_PD_ATTR_EXTEND_LOCAL = 17,
    PS3LIB_CTRL_EVT_DEFAULT_UNUSED_LOCAL,
    PS3LIB_CTRL_EVT_MAX_TYPE_LOCAL,
}; 

/**
 * @brief 事件日志读取结构
 */
typedef struct Ps3LibEvtLogRdEntry {
    U32 loopCnt;        ///< 轮数
    U32 seqNum;         ///< 日志序号
    U32 offset;         ///< 日志在存储介质中的偏移
    U32 timeStampBySec; ///< 从日志头信息中获取
    U32 size;           ///< 该日志大小
} Ps3LibEvtLogRdEntry_s;

/**
 * @brief 事件日志长久保存信息
 */
typedef struct Ps3LibEvtLogRdInfo {
    Ps3LibEvtLogRdEntry_s persistInfo[PS3LIB_EVT_LOG_LAST_MAX]; ///< 事件日志读取结构
} Ps3LibEvtLogRdInfo_s;

/**
 * @brief 事件日志持久化信息
 */
typedef struct Ps3LibEvtPersistInfo {
    U32               regionSz[2];   ///< 0 为normal区域大小，1为fatal级别区域大小
    Ps3LibEvtLogRdInfo_s persist;    ///< 持久化信息
} Ps3LibEvtPersistInfo_s;

/**
 * @brief 事件日志头
 */
typedef struct Ps3LibEvtLogHeader {
    U32 magic;          ///< 魔数校验头
    U32 seqNum;         ///< 事件日志序列号
    U32 size    : 8;    ///< 日志长度,包括日志头和尾部事件长度
    U32 funcType  : 2;  ///< 事件所属function (enum Ps3LibCtrlFuncType_e)
    U32 conFlag : 1;    ///< 驱动加载状态 (0:未加载, 1:已加载)
    U32 evtCode : 12;   ///< 事件代号 0-4095
    U32 level : 4;      ///< 事件日志级别 (INFO WARNING CRITICAL FATAL)
    U32 type : 5;       ///< 事件所属类别 bit 0-15
    U32 timeStampBySec; ///< 秒级时间戳 since 1970
} Ps3LibEvtLogHeader_s;

#pragma pack(1) ///< 结构体1字节方式对齐

/**
 * @brief pd属性信息
 */
typedef struct Ps3LibPdAttrInfo {
    U32 checkSum    : 8;         ///< pd引用计数
    U32 enclosureId : 8;         ///< enclID
    U32 phyId       : 8;         ///< 盘prev状态
    U32 evtVersion  : 8;         ///< 识别event版本
    U16 phyDiskID;               ///< 物理盘标识ID
    U16 softChan    : 4;         ///< 通道号
    U16 devID       : 12;        ///< 设备编号
    U16 slotId;                  ///< 槽位号
    U16 oldState    : 4;         ///< 盘prev状态
    U16 newState    : 4;         ///< 盘当前状态
    U16 isEnclPd    : 1;         ///< 是否enclusorePD
    U16 pad1        : 7;         ///< 预留
    U16 arrayId     : 8;         ///< 盘子组ID
    U16 rowId       : 8;         ///< 子组内序号
    U16 prevState   : 8;         ///< 盘prev状态
    U16 curState    : 8;         ///< 盘当前状态    
    U64 sasAddr;                 ///< sas Address
}Ps3LibPdAttrInfo_s;

/**
 * @brief 热备盘信息
 */
typedef struct Ps3LibSparePdInfo {
    Ps3LibPdAttrInfo_s baseInfo;
    U8 dedicatedDgCnt;    ///< 局部热备关联DG个数
    U8 reserved[3];       ///< 保留字段
    U16 dedicatedDgId[8]; ///< 局部热备盘最多关联8个DG
}Ps3LibSparePdInfo_s;

/**
 * @brief vd属性信息
 */
typedef struct Ps3LibVdAttrInfo {
    U32 magicNum;                   ///< 校验魔数
    U16 virtDiskID;                 ///< 虚拟盘标识符
    U16 softChan    : 4,            ///< 通道号
        devID       : 12;           ///< 设备编号
    U16 diskGrpId;                  ///< DG标识符
    U16 locked      : 1,            ///< VD认证标记(0:认证,1:未认证),未认证VD对OS不可见
        pad         : 15;           ///< 保留位域
}Ps3LibVdAttrInfo_s;

typedef struct Ps3LibDiskPFCfgModifyEvtInfo {
    U8 modifyCfgDataType;
    U8 funcIsEnable:1;          ///< 修改后的硬盘预失效是否使能
    U8 pad : 7;                 ///< 保留字段
    U16 preFailPollTimeMin;     ///< 修改后的硬盘预失效例测时间
}Ps3LibDiskPFCfgModifyEvtInfo_s;

/**
 * @brief VD可以修改的基本配置
 */
typedef struct Ps3LibVdBaseSetting {
    U32 accessPolicy            :2,  ///< VD的访问权限(0:RW, 1:RO, 2:block, 3:remove access)
        hidden                  :1,  ///< 对操作系统不可见(0:可见, 1:不可见)
        defaultWriteCachePolicy :2,  ///< 用户配置写策略(0:WT, 1:WB, 2:AWB)
        currentWriteCachePolicy :1,  ///< 运行写策略(0:WT, 1:WB)
        defaultReadCachePolicy  :1,  ///< 用户配置读策略(0:NoRA, 1:RA)
        currentReadCachePolicy  :1,  ///< 运行读策略(0:NoRA, 1:RA)
        diskCachePolicy         :2,  ///< 硬盘缓存策略(0:default, 1:enable, 2:disable)
        ioPolicy                :1,  ///< 用户配置VD是否支持直发IO(0:Cached, 1:Direct)
        noBgi                   :1,  ///< 不自动bgi标记(0:非使能, 1:使能)
        emulationType           :2,  ///< 0:default, 1:None 512Phy, 2:Force 4096phy
        unmap                   :1,  ///< unmap使能开关(0:不支持, 1:支持umap)
        cbSize                  :2,  ///< 0:64KB(default), 1:128KB, 2:256KB
        cbMode                  :3,  ///< 0:intelligent, 1:standard, 2:custom1, 3:custom2, 4:custom3, 7:disable
        encryption              :1,  ///< 加密
        rebootNoVerify          :1,  ///< 本卡加密VD重启时认证(0:需要(默认), 1:不需要)
        rsv                     :10; ///< 保留字段
    U8  vdName[PS3LIB_MAX_VD_NAME_BYTES]; ///< VD名称
    U64 size;                        ///< VD容量(单位sector)
}Ps3LibVdBaseSetting_s;

/**
 * @brief vd特性信息
 */
typedef struct Ps3LibVdPropertiesInfo {
    Ps3LibVdAttrInfo_s baseInfo;        ///< vd属性信息
    Ps3LibVdBaseSetting_s oldSetting;   ///< old VD可以修改的基本配置
    Ps3LibVdBaseSetting_s newSetting;   ///< new VD可以修改的基本配置
}Ps3LibVdPropertiesInfo_s;

/**
 * @brief vd状态变更信息
 */
typedef struct Ps3LibVdStateChangeInfo {
    Ps3LibVdAttrInfo_s baseInfo;        ///< VD属性信息
    U8 oldVdState;                      ///< old VD状态
    U8 newVdState;                      ///< new VD状态
    U8 reserved[2];                     ///< 保留字段
}Ps3LibVdStateChangeInfo_s;

/**
 * @brief vd创建事件信息
 */
typedef struct Ps3LibVdCreateEvtInfo {
    Ps3LibVdAttrInfo_s baseInfo;        ///< vd属性信息
    Ps3LibVdBaseSetting_s setting;      ///< vd可以修改的基本配置
}Ps3LibVdCreateEvtInfo_s;

/**
 * @brief ctrl属性信息
 */
typedef struct Ps3LibCtrlAttrInfo {
    U32 supportUnevenSpans  : 1; ///< 是否支持异形span
    U32 supportJbodSecure   : 1; ///< 是否支持Jbod盘加密
    U32 supportCrashDump    : 1; ///< 是否支持crashdump
    U32 supportNvmePassthru : 1; ///< 是否支持nvmePassthru
    U32 supportDirectCmd    : 1; ///< 是否支持直发命令
    U32 supportAcceleration : 1; ///< 是否支持fastPath
    U32 supportNcq          : 1; ///< 是否支持 NCQ
    U32 reserved1           : 25; ///< 保留字段
    U32 reserved2[1];             ///< 保留字段
    U64 oldSysTime;
    U64 newSysTIme;
    U64 monoSysTime;             ///< 系统单调时间
    U32 newSysTimeYear;          ///< 年
    U32 newSysTimeMon;           ///< 月
    U32 newSysTimeDay;           ///< 日
    U32 newSysTimeHour;          ///< 时
    U32 newSysTimeMin;           ///< 分
    U32 newSysTimeSec;           ///< 秒
    U64 cfgNum;
    U8  *pValue;        ///< 值所在buffer
    U32 len;            ///< buffer 长度
}Ps3LibCtrlAttrInfo_s;

/**
 * @brief 控制卡启动与退出元数据结构
 */
typedef struct Ps3LibCtrlRebootInfo {
    U16 ctrlBootMode; ///< 控制卡启动的模式(控制卡退出填退出前的启动的模式)
    U16 ctrlShutDownReason;///< 控制卡shutDown原因(控制卡启动不填这个字段)
                           ///< 0:进程coredump
                           ///< 1:进程被kill
                           ///< 2:进程退出exit(驱动下发软复位)
                           ///< 3:进程退出 驱动下发硬复位
                           ///< 4:iocReboot
    U32 ctrlBootCount;     ///< 控制卡重启计数(控制卡退出填退出前的重启计数)
}Ps3LibCtrlRebootInfo_s;

/**
 * @brief dg属性信息
 */
typedef struct Ps3LibDgAttrInfo {
    U16 dgId;                ///< DG ID
    U16 reserved[3];
}Ps3LibDgAttrInfo_s;

/**
 * @brief Expander信息
 */
typedef struct Ps3LibExpanderInfo {
    U8 EnclId;   ///< expander 槽位id
    U8 reserved[7];
}Ps3LibExpanderInfo_s;

/**
 * @brief 事件发布信息
 */
typedef struct Ps3LibCfgAttrInfo {
    U16   profileId;
    U8 reserved[2];
}Ps3LibCfgAttrInfo_s;

typedef struct Ps3LibCfgAutoConfig {
    S8   cfgName[PS3LIB_CTRL_AUTOCONFIG_EVTDATA_SIZE];
}Ps3LibCfgAutoConfig_s;

/**
 * @brief 后台任务rebuild/moveback事件发布信息
 */
typedef struct Ps3LibBgtRebuildInfo {
    U16 newPDFlatId;      ///< 物理盘标识ID
    U16 newEnclosureId;   ///< encl ID
    U16 newSlotId;        ///< 槽位号
    U16 oldPDFlatId;      ///< 物理盘标识ID
    U16 oldEnclosureId;   ///< encl ID
    U16 oldSlotId;        ///< 槽位号        
    U16 virtDiskId;       ///< [2字节]VD ID
    U16 devId;            ///< 设备ID
    U32 remainSecs;       ///< 预计剩余时间（秒）
    U64 errorPba;         ///< 不可修复坏块pba    
    U64 errorLba;         ///< 不可修复坏块Lba
    U8  progressPercent;  ///< 进度百分比
    U8  rebuildRate;      ///< 重建率
    U8  enableMoveback;   ///< 回迁开关
    U8  autoRebuild;      ///< 重建开关
    U8  eghs;             ///< ready/spare/smart紧急热备开关    
    U8  enablePdm;                   ///< PDM开关，缺省开启
    U8  pdmSupportReadyPd;           ///< 是否支持目标ready盘做PDM,默认不开启,手动启动不受此开关控制
    U8  reserved[1];
    U32 pdmTimerInterval;            ///< PDM检测周期
} Ps3LibBgtRebuildInfo_s;

/**
 * @brief 后台任务FGI/BGI事件发布信息
 */
typedef struct Ps3LibBgtInitEvtInfo {
    U32 aliveSec;       ///< [4字节]剩余时间
    U32 progressRate;   ///< [4字节]进度百分比
    U16 dgId;           ///< [2字节]DG ID
    U16 virtDiskId;     ///< [2字节]VD ID
    U16 pdFlatId;       ///< [2字节]PD ID
    U16 enclosureId;    ///< [2字节]enc ID
    U16 slotId;         ///< [2字节]槽位号
    U8  cpuRate;        ///< [1字节]后台任务CPU占用率
    S8  mode[PS3LIB_FGI_MODE_LEN]; ///< [5字节]"FULL" / "FAST"
    U64 mediumErrLba;   ///< [8字节]
    U64 mediumErrPba;   ///< [8字节]
    U16 MediumErrPdFlatId;///< [2字节]
    U16 softChan : 4;   ///< [2字节]通道号
    U16 devID    : 12;  ///< [0字节]设备编号
} Ps3LibBgtInitEvtInfo_s;

/**
 * @brief 后台任务SE事件发布信息
 */
typedef struct Ps3LibBgtEraseEvtInfo {
    U32 aliveSec;       ///< [4字节]剩余时间
    U32 progressRate;   ///< [4字节]进度百分比
    U16 dgId;           ///< [2字节]DG ID
    U16 virtDiskId;     ///< [2字节]VD ID
    U16 pdFlatId;       ///< [2字节]PD ID
    U16 enclosureId;    ///< [2字节]enc ID
    U16 slotId;         ///< [2字节]槽位号
    U16 devId;          ///< [2字节]对外显示的VD号
} Ps3LibBgtEraseEvtInfo_s;

typedef struct Ps3LibBgtCcEvtInfo {
    U32 aliveSecs;       ///< 剩余时间，秒为单位
    U8  progressPercent; ///< 进度百分比
    U8  ccRate;
    U8  mode;
    U8  resered;
    U16 virtDiskID;      ///< MediumErr使用
    U16 diskGroupID;
    U32 inconsistStrip;
    U16 devId;
    U16 faultDiskID;
    U16 enclosureId;
    U16 slotId;
    U64 pdErrLba;
    U64 vdErrLba;
}Ps3LibBgtCcEvtInfo_s;

typedef struct Ps3LibBgtPrEvtInfo {
    U32 aliveSecs;       ///< 剩余时间，秒为单位
    U16 enclosureId;     ///< enc ID
    U16 slotId;          ///< 槽位号
    U8  progressPercent; ///< 进度百分比
    U8  prRate;
    U16 virtDiskID;      ///< MediumErr使用
    U16 pdFlatId;
    U16 diskGroupID;
    U16 dgStatus;
    U8 reserved[2];
    U64 errLba;
}Ps3LibBgtPrEvtInfo_s;

/**
 * @brief phy接入异常告警信息
 */
typedef struct Ps3LibPhyEvtInfo {
    U32 enclosureId:8,   ///< encl ID
        slotId:8,        ///< 槽位号
        phyId:8,         ///< phyId
        reserved:8;
} Ps3LibPhyEvtInfo_s;

/**
 * @brief 前端坏块处理模块事件发布信息
 */
typedef struct Ps3LibVdBbmEvtInfo {
    U64 lba;            ///< 添加到坏块表的lba
    U64 pba;            ///< 添加到坏块表的lba对应的pba
    U16 lbaLen;         ///< 添加到坏块表的lba长度
    U16 dgId;           ///< DG ID
    U16 virtDiskId;     ///< VD ID
    U16 percentErrTbl;  ///< 坏块表已占用百分比
    U16 devId;          ///< 设备ID
    S8  errTblName[PS3LIB_BBM_ERRTBL_NAME_LEN]; ///< 坏块表类型名称
    U16 pdFlatId;       ///< PD ID
    U16 enclosureId;    ///< PD enc ID
    U16 slotId;         ///< 槽位号
    U16 reserved;
} Ps3LibVdBbmEvtInfo_s;

typedef struct Ps3LibRwDdtEvtInfo {
    U16 virtDiskID;      ///< vdId
    U16 diskGroupID;     ///< dgId
    U32 vdLen;           ///< io长度
    U64 vdLba;           ///< io起始LBA
} Ps3LibRwDdtEvtInfo_s;

typedef struct Ps3LibFlushEvtInfo {
    U16 opcode;         ///< 操作 1:加标记 0:清标记
    U16 dgId;           ///< DG ID
    U16 virtDiskId;     ///< VD ID
    U16 minVdId;        ///< 不使用，该值为0
    U16 devId;          ///< 设备ID
    U8  reserved[6];
    U64 vdIdMap[3];     ///< 用于OCR事件，用MAP方式保存VDID数组 (128+4个bit)，内部vdId，从1开始
} Ps3LibFlushEvtInfo_s;

typedef struct Ps3LibMigrationInfo {
    U8  migrRate;
    U8  resv;
    U16 diskGroupID;
    U32 percent;
    U32 aliveSec;
    U16 currVdId;
    U8 reserved[2];
} Ps3LibMigrationInfo_s;

/**
 * @brief BBM删RCT的批量事件
 */
typedef struct Ps3LibVdBbmBatchEvtInfo
{
    U32 count;
    Ps3LibVdBbmEvtInfo_s vdBbmEvtInfo[0];
} Ps3LibVdBbmBatchEvtInfo_s;

/**
 * @brief vd批量事件
 */
typedef struct Ps3LibVdBatchEvtInfo
{
    U32 count;
    Ps3LibVdAttrInfo_s vdInfo[0];
}Ps3LibVdBatchEvtInfo_s;

/**
 * @brief pd批量事件
 */
typedef struct Ps3LibPdBatchEvtInfo
{
    U32 count;
    Ps3LibPdAttrInfo_s pdInfo[0];
}Ps3LibPdBatchEvtInfo_s;

/**
 * @brief ctrl批量事件
 */
typedef struct Ps3LibCtrlBatchEvtInfo
{
    U32 count;
    Ps3LibCtrlAttrInfo_s ctrlInfo[0];
}Ps3LibCtrlBatchEvtInfo_s;

/**
 * @brief 批量事件结构体模板，所有批量事件结构体均需按照此格式定义
 */
typedef struct Ps3libBatchEvtInfoCommon
{
    U32 count;
    S8 batchInfo[0];
}Ps3libBatchEvtInfoCommon_s;

/**
 * @brief BBU模块事件发布信息
 */
typedef struct Ps3LibBbuEvtInfo {
    U8  absent : 1;                       ///< 1：缺少bbu 0：bbu存在
    U8  overTemp : 1;                     ///< 1：温度过高 0：温度正常
    U8  overVol : 1;                      ///< 1：过压    0：电压正常
    U8  overCur : 1;                      ///< 1：过流 0：电流正常
    U8  overLoad : 1;                     ///< 1：过放 0：正常
    U8  lifeisOver : 1;                   ///< 1：寿命到达 0：正常
    U8  reserved : 2;
    U8  status;                           ///< 1：bbu可用 0：bbu不可用
    U8  chargeStatus;                     ///< 1：放电 0：充电
    U8  learnStage;                       ///< 校准阶段
    S16  batTemperature;                   ///< 温度(单位：C)
    U16  batVoltage;                       ///< 电压(单位：mV)
    S16  batCurrent;                       ///< 电流(单位：mA)
    U8  reserved1[2];
} Ps3LibBbuEvtInfo_s;

/**
 * @brief ukey模块事件发布信息
 */
typedef struct Ps3LibUkeyEvtInfo {
    U8  ukeyStatus;                    ///< ukey状态
    U8  reserved[3];
} Ps3LibUkeyEvtInfo_s;

/**
 * @brief expander事件信息
 */
typedef struct Ps3LibExpEvtInfo {
    U64 expanderSasAddr; ///< expander SAS地址
    U64 attachedSasAddr; ///< 对端SAS地址
    U8  phyId[8];        ///< phy id
} Ps3LibExpEvtInfo_s;

/**
 * @brief ecc模块事件发布信息
 */
typedef struct Ps3LibEccEvtInfo {
    U32 eccErrObj;
    U32 eccSingleBitCntInc;
    U8  eccErrCntThreshold;
    U8  pad[3];
    U16 eccClearPeriod;
    U8  eccType;
    U8  eccErrSubObj;
    U64 eccMutilErrAddr;
    U32 eccEvtVersion;
} Ps3LibEccEvtInfo_s;

/**
 * @brief temp模块事件发布信息
 */
typedef struct Ps3LibTempEvtInfo {
    U32 tempType;
    S32 tempErrThreshold[4];
    S32 temperature;
} Ps3LibTempEvtInfo_s;

typedef struct Ps3LibIoCmdType {
    U8 cmdType;
    U8 rsv[9];
} Ps3LibIoCmdType_s;

/**
 * @brief reset done事件发布信息
 */
typedef struct Ps3LibDeviceResetEvtInfo {
    U16 enclosureId; ///< enc ID
    U16 slotId;      ///< 槽位号
    U16 phyDiskID;   ///< 物理盘标识ID
    U16 resetType;
    U64 sasAddress;
} Ps3LibDeviceResetEvtInfo_s;

/**
 * @brief sense data事件发布信息
 */
typedef struct Ps3LibSenseDataEvtInfo {
    U16 enclosureId;   ///< enc ID
    U16 slotId;        ///< 槽位号
    U16 phyDiskID;     ///< 物理盘标识ID
    union {
        U8 cdb[10];   ///< 只发布10个字节
        Ps3LibIoCmdType_s ioCmdType;
    };
    U8 ioFormat;
    U8 palErr;
    U8 dataPre;
    U8 scsiStatus;
    U8 skStatus;
    U8 sk;
    U8 asc;
    U8 ascq;
    U64 path;
} Ps3LibSenseDataEvtInfo_s;

typedef struct Ps3LibPdDownloadInfo {
    U32 downloadMode; ///< MODE_E MODE_F MODE_7
    S32 isSuccess;
    U16 phyDiskID;               ///< 物理盘标识ID
    U16 softChan    : 4;        ///< 通道号
    U16 devID       : 12;        ///< 设备编号
    U16 enclosureId;   ///< enc ID
    U16 slotId;        ///< 槽位号
}Ps3LibPdDownloadInfo_s;

typedef struct Ps3LibSanitizeEvtInfo {
    Ps3LibPdAttrInfo_s baseInfo;
    U32 aliveSecs;       ///< 剩余时间，秒为单位
    U8  progressPercent; ///< 进度百分比
    U8  pad[3]; ///< 进度百分比
}Ps3LibSanitizeEvtInfo_s;

typedef struct Ps3LibFormatEvtInfo {
    Ps3LibPdAttrInfo_s baseInfo;
    U32 aliveSecs;       ///< 剩余时间，秒为单位
    U8  progressPercent; ///< 进度百分比
    U8  pad[3]; ///< 预留
}Ps3LibFormatEvtInfo_s;

typedef struct Ps3LibSnapshotEvtInfo {
    U8  snapCount;  ///< 快照计数
    U8  pad[3];     ///< 预留
}Ps3LibSnapshotEvtInfo_s;

typedef struct Ps3LibPdPreFailInfo {
    U32 checkSum    : 8;        ///< pd引用计数
    U32 oldState    : 4;        ///< 盘prev状态
    U32 newState    : 4;        ///< 盘当前状态
    U32 diskType    : 4;        ///< 盘类型
    U32 pad         : 12;
    U16 phyDiskID;               ///< 物理盘标识ID
    U16 softChan    : 4;        ///< 通道号
    U16 devID       : 12;        ///< 设备编号
    U16 enclosureId;            ///< enc ID
    U16 slotId;                 ///< 槽位号
    U32 historyErrBitMap;     ///< 每个bit表示一类错误，dmSataSmartType_e,dmSasSmartType_e,dmNvmeSmartType_e
    U32 errBitMap;     ///< 每个bit表示一类错误，dmSataSmartType_e,dmSasSmartType_e,dmNvmeSmartType_e
    S8  vendor[8];
    S8  diskSerialNum[24];
}Ps3LibPdPreFailInfo_s;

typedef struct Ps3LibNvDataInvaildInfo {
    U32  nvDataIDBitMap[16];    ///< 失效的nvDataID对应的bitMap,最大dump 512个nvDataID
    U16  bitmapSize;            ///< bitmap的大小
    U16  invaildCount;          ///< 无效的nvData个数
} Ps3LibNvDataInvaildInfo_s;

/**
 * @brief pcie oem事件信息
 */
typedef struct Ps3LibOemInfo {
    S8  oemData[PS3LIB_EVT_LOG_INFO_MAX_SIZE];
} Ps3LibOemInfo_s;

/**
 * @brief pcieswitch 背板事件信息
 */
typedef struct Ps3LibBplaneEvtInfo {
    S8  bplaneData[PS3LIB_EVT_LOG_INFO_MAX_SIZE];
} Ps3LibBplaneEvtInfo_s;

typedef union Ps3LibReportEvtData
{
    Ps3LibPdAttrInfo_s    pdInfo;
    Ps3LibSparePdInfo_s   sparePdInfo;
    Ps3LibVdAttrInfo_s    vdInfo;
    Ps3LibVdPropertiesInfo_s vdChange;
    Ps3LibVdStateChangeInfo_s vdStateChangeInfo;
    Ps3LibVdCreateEvtInfo_s  vdCreate;
    Ps3LibCtrlAttrInfo_s  ctrlInfo;
    Ps3LibCtrlRebootInfo_s  ctrlRebootInfo;
    Ps3LibDgAttrInfo_s    dgInfo;
    Ps3LibExpanderInfo_s  expanderInfo;
    Ps3LibCfgAttrInfo_s   cfgInfo;
    Ps3LibCfgAutoConfig_s   autoConfigInfo;

    Ps3LibBgtRebuildInfo_s bgtRebuildInfo;
    Ps3LibBgtInitEvtInfo_s bgtInitEvtInfo;
    Ps3LibBgtEraseEvtInfo_s bgtEraseEvtInfo;
    Ps3LibBgtCcEvtInfo_s   bgtCcInfo;

    Ps3LibBgtPrEvtInfo_s   bgtPrInfo;

    Ps3LibPhyEvtInfo_s     phyInfo;

    Ps3LibVdBbmEvtInfo_s  vdBbmEvtInfo;
    Ps3LibRwDdtEvtInfo_s  dataVdInfo;

    Ps3LibFlushEvtInfo_s   flushEvtInfo;

    Ps3LibMigrationInfo_s  bgtMigrInfo;

    Ps3LibVdBatchEvtInfo_s   batchVdInfo;
    Ps3LibPdBatchEvtInfo_s   batchPdInfo;
    Ps3LibCtrlBatchEvtInfo_s batchCtrlInfo;
    Ps3LibVdBbmBatchEvtInfo_s batchBbmInfo;
    Ps3libBatchEvtInfoCommon_s *pBatchCommonInfo;
    Ps3LibBbuEvtInfo_s        bbuEvtInfo;
    Ps3LibUkeyEvtInfo_s       ukeyInfo;

    Ps3LibExpEvtInfo_s        expEvtInfo;
    Ps3LibOemInfo_s           oemEvtInfo;
    Ps3LibBplaneEvtInfo_s     bplaneEvtInfo;
    Ps3LibEccEvtInfo_s        eccEvtInfo;
    Ps3LibTempEvtInfo_s       tempEvtInfo;
    Ps3LibDeviceResetEvtInfo_s deviceResetEvtInfo;
    Ps3LibSenseDataEvtInfo_s  senseDataEvtInfo;
    Ps3LibPdDownloadInfo_s    pdDldEvtInfo;
    Ps3LibSanitizeEvtInfo_s   sanitizeInfo;
    Ps3LibFormatEvtInfo_s     formatInfo;
    Ps3LibSnapshotEvtInfo_s   snapShotInfo;
    Ps3LibPdPreFailInfo_s     pdPrefailInfo;
    Ps3LibDiskPFCfgModifyEvtInfo_s  diskPFCfgModifyEvtInfo;
    Ps3LibNvDataInvaildInfo_s nvDataInvaildInfo;

    ///< 用来计算 device ID 联合低64位
    U64 value;

    ///< 用来声明发布事件内容最大值，不做任何使用
    U8 data[PS3LIB_EVT_LOG_INFO_MAX_SIZE];
}Ps3LibReportEvtData_u;
#pragma pack()

/**
 * @brief   事件日志
 */
typedef struct Ps3LibEvtLogEntry {
    U32                seqNum;
    Ps3LibEvtLogHeader_s  head;    ///< 日志消息头
    Ps3LibReportEvtData_u evtInfo; ///< 日志消息体,事件描述信息
    CtrlId_t           ctrlId;     ///< 事件所属控制卡id 非控制卡事件时为全F
    U8                 pad[4];
    CtrlId_t           regCtrlId;  ///< 订阅的控制卡id 只在事件上报时关注
    U32                registerId; ///< 订阅的uniqueId 只在事件上报时关注
} Ps3LibEvtLogEntry_s;

/**
 * @brief   事件日志列表
 */
typedef struct Ps3LibEvtLogList {
    U32                   count;        ///< 事件日志数量
    Ps3LibEvtLogEntry_s   evtEntry[0];  ///< 事件日志柔性数组
} Ps3LibEvtLogList_s;

/**
 * @brief   事件日志中的错误数据结构体
 */
typedef struct Ps3LibEvtErrDataEntry {
    U32 beforeSeqNum; ///< 错误数据在哪条日志之后
    U32 errDataLen;   ///< 错误数据长度
    U8 *errData;      ///< 指向错误数据
} Ps3LibEvtErrDataEntry_s;

typedef struct Ps3LibEvtLog {
    Ps3LibEvtPersistInfo_s   evtPerInfo;   ///< 持久化信息
    U32                      evtCount;     ///< 事件日志的条数
    U32                      errCount;     ///< 错误数据的条数
    Ps3LibEvtLogEntry_s *    evtEntryList; ///< 指向第一条事件日志
    Ps3LibEvtErrDataEntry_s *errDataList;  ///< 指向第一条错误数据
} Ps3LibEvtLog_s;

/**
 *  @brief Expander 事件日志补充收集信息结构体key-value
 */
typedef struct Ps3LibEventDataCollectionKV {
    char key[PS3LIB_EXP_EVENT_DATA_COLLECT_MAX_NUM];   ///< 补充信息-key
    char val[PS3LIB_EXP_EVENT_DATA_COLLECT_MAX_NUM];   ///< 补充信息-value
} Ps3LibEventDataCollectionKV_s;

/**
 *  @brief Expander 事件日志补充收集信息结构体
 */
typedef struct Ps3LibEventDataCollection {
    struct Ps3LibEventDataCollectionKV kv[PS3LIB_EXP_EVENT_DATA_COLLECT_MAX_NUM]; ///< 事件日志补充收集信息数组
} Ps3LibEventDataCollection_s;

enum {
    PS3LIB_EXPANDER_EVENT_TYPE = 0,       ///< EXPANDER事件类型
    PS3LIB_SWITCH_EVENT_TYPE   = 1,       ///< SWITCH事件类型
    PS3LIB_RAID_HBA_EVENT_TYPE = 0xff,    ///< RAID/HBA事件类型
};

typedef struct Ps3LibEvtPrintFunc{
    S8 const *(*evtCode2Str)(U32 opCode);
    S8 const *(*evtLoca2Str)(U8 locate);
    const S8 *(*getEvtDesc)(Ps3LibEvtLogEntry_s *event, S32 len, S8 *buff, S32 buffLen);
    S32(*getEvtData)
    (Ps3LibEventDataCollection_s *eventDataCollection, Ps3LibEvtLogEntry_s *event, S32 len, S8 *buff, S32 buffLen);
}Ps3LibEvtPrintFunc_t;

/**
 * @brief       事件日志转字符串描述
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   enclId: encl标识符
 * @return      Ps3LibEvtPrintFunc_t
 */
Ps3LibEvtPrintFunc_t *ps3libEventPrintFunc(CtrlId_t ctrlId, U8 eventType);

/**
 * @brief       查询事件日志的持久化信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  evtlogRdInfo: 查询到的事件日志持久化信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlEvtlogPerGet(CtrlId_t ctrlId, Ps3LibEvtPersistInfo_s *evtlogRdInfo);

/**
 * @brief       读取控制卡的所有事件日志
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   sinceSeqNum: 读取的事件日志起始序列号
 * @param[out]  ppEvtLog: 读取到的事件日志信息
 * @warning     输出的ppEvtLog需要由调用者调用ps3libEventLogDestroy释放
 * @return      PS3_ERRNO_SUCCESS: 成功
 *              PS3LIB_ERRNO_EVENT_BUSY: 事件日志系统正忙, 请稍后获取
 *              PS3LIB_ERRNO_MALLOC_FAILED: 内存申请失败
 *              PS3LIB_ERRNO_WRONG_EVENT_LOG_DATA: 读取到错误的事件日志数据
 *              PS3LIB_ERRNO_WRONG_EVENT_LOG_INFO:
 * 读取到错误的事件日志持久化信息
 */
Ps3Errno ps3libEventLogGet(CtrlId_t ctrlId, U32 sinceSeqNum, Ps3LibEvtLog_s **ppEvtLog);

/**
 * @brief       与ps3libEventLogGet函数成对使用,释放由ps3libEventLogGet生成的事件日志结构体
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   ppEvtLog: 需要释放事件日志信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
void ps3libEventLogDestroy(Ps3LibEvtLog_s *pEvtLog);

/**
 * @brief       比较两个事件级别
 * @param[in]   levelA[in], 级别A
 * @param[in]   levelB[in], 级别B
 * @return      返回一个int, 小于0、等于0或大于0, 当levelA小于levelB、levelA等于levelB、levelA大于levelB时
 */
S32 ps3libEvtLevelCompare(U8 levelA, U8 levelB);

/**
 * @brief       删除控制卡的事件日志
 * @param[in]   ctrlId: 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlEventLogsDelete(CtrlId_t ctrlId);

/**
 * @brief       事件订阅Id转控制卡Id
 * @param[in]   uniqueId: 已订阅Id
 * @return      ctrlId
 */
U16 ps3libEventUinqueIdToCtrlId(U32 uniqueId);

#endif
