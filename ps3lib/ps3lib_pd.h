/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_pd.h
 * @date    2023.04.25
 * @brief   磁盘信息
 */

#ifndef __PS3LIB_PD_H__
#define __PS3LIB_PD_H__

#define PS3LIB_MAX_PD_PER_CONTROLLER_HBA   (1024 + 2)   ///< HBA的最大物理盘控制卡数量
#define PS3LIB_MAX_PD_PER_CONTROLLER       (PS3LIB_MAX_PD_PER_CONTROLLER_HBA)  ///< 最大物理盘控制卡数量
#define PS3LIB_MAX_VD_PER_CTRL_RAID        (128 + 4)   ///< RAID的最大虚拟盘控制卡数量
#define PS3LIB_MAX_PD_NUM                  (1026)      ///< ctrl支持的最大pd数量
#define PS3LIB_MAX_DG_PER_DEDICATED_SPARE  (8)         ///< 单个DG中允许的最大局部热备数
#define PS3LIB_BGT_FGI_MODE_DATA_MAX_LEN   (16)        ///< pd前台初始化数据最大长度
#define PS3LIB_MANUFACTURE_ID_LEN          (16)  ///< 硬盘生产商ID的最大长度
#define PS3LIB_PD_SERIAL_NUMBER_LEN        (24)  ///< 硬盘序列号的最大长度
#define PS3LIB_MODEL_NUMBER_LEN            (40)  ///< 硬盘型号的长度
#define PS3LIB_FW_REVISION_LEN             (8)   ///< 硬盘固件版本的长度
#define PS3LIB_MAX_PORT_NUM                (2)      ///< 最大端口数量
#define PS3LIB_MAX_CONNECTOR_NUM           (2)      ///< 最大连接数量
#define PS3LIB_PD_CONNECTID_MAX_CNT        (5)
#define PS3LIB_VPD_PAGE_LEN                (64)     ///< vpd page 长度
#define PS3LIB_INQUIRY_DATA_LEN            (96)     ///< Inquiry 数据长度
#define PS3LIB_INQUIRY_DATAEXT_LEN         (32)     ///< Inquiry 96字节之后的额外长度
#define PS3LIB_MAX_PD_NUM_EXT              (2048)   ///< 预留足够的pd盘个数
#define PS3LIB_SCSI_CDB_LEN                (32)     ///< 透传scis CDB长度
#define PS3LIB_SCSI_SENSE_BUFFER_LEN       (96)     ///< 透传scis sense buffer长度
#define PS3LIB_SCSI_WR_CODE                (0x3b)   ///< 透传scsi wr code 值
#define PS3LIB_SECTOR_SZIE_512             (512)    ///< sectorsize为512B
#define PS3LIB_SECTOR_SZIE_4K              (4096)   ///< sectorsize为4096B
#define PS3LIB_PD_PORT_NUM_SAS             (2)      ///< SAS盘port数量
#define PS3LIB_PD_PORT_NUM_SATA            (1)      ///< SATA盘port数量
#define PS3LIB_PD_PORT_NUM_NVME            (1)      ///< NVME盘port数量
#define PS3LIB_MAX_PROFILE_COUNT           (24)     ///< 最大profile个数

/**
 * @brief       pd位置信息结构体
 */
typedef struct Ps3LibPdPosition {
    U8  enclId;    ///< 背板标识符
    U8  pad;       ///< 保留字段
    U16 slotId;    ///< 槽位标识符
} Ps3LibPdPosition_t;

/**
 * @brief       pd position列表结构体
 */
typedef struct Ps3LibPdList{
    U32                 pdCount;    ///< pd数量
    Ps3LibPdPosition_t  pdPosition[PS3LIB_MAX_PD_NUM];    ///< pd位置列表
} Ps3LibPdList_s;

/**
 * @brief       pd devId列表结构体
 */
typedef struct Ps3LibPdDevIdList{
    U32     pdCount;    ///< pd数量
    U16     pdDevId[PS3LIB_MAX_PD_NUM];    ///< pd devId列表
} Ps3LibPdDevIdList_s;

/**
 * @brief   硬盘后台任务进度信息
 */
typedef struct Ps3libProgress{
    U32     progressPercent;    ///< 进度百分比
    U32     remainSecs;         ///< 预计剩余时间
}Ps3LibProgress_t;

/**
 * @brief   connectId结构体
 */
typedef struct Ps3LibPdConnect {
    U8 connectId;
    U8 subconnectId;
} Ps3LibPdConnect_t;

/**
 * @brief   硬盘port信息
 */
typedef struct Ps3LibPdPortInfo{
    U8  physicalSpeed[PS3LIB_MAX_PORT_NUM];    ///< 物理速率
    U8  logicalSpeed[PS3LIB_MAX_PORT_NUM];     ///< 协商或者配置的链路速率
    U8  active[PS3LIB_MAX_PORT_NUM];           ///< 端口是否active(not_support)
    U8  pad[2];
    union{
        U64 sasAddr[PS3LIB_MAX_PORT_NUM];      ///< 硬盘SAS地址
        U64 wwid;                           ///< nvme地址
        U64 sataStpAddr;                    ///< SATA盘的STP桥地址
    };
}Ps3LibPdPortInfo_t;

/**
 * @brief   硬盘path信息
 */
typedef struct Ps3LibPdPathInfo {
    U8      isPathBroken    : 4;                            ///< not_support
    U8      pad             : 3;                            ///< 预留
    U8      widePortCapable : 1;                            ///< not_support
    U8      connectorType;                                  ///< not_support
    U8      connectedPortBitmap[PS3LIB_MAX_CONNECTOR_NUM];  ///< not_support
    U8      connectorIndex[PS3LIB_MAX_CONNECTOR_NUM];       ///< not_support
    U8      pad1[2];
    Ps3LibPdPortInfo_t       portInfo;                       ///< 硬盘port信息
    U8      connectCnt;
    Ps3LibPdConnect_t        connect[PS3LIB_PD_CONNECTID_MAX_CNT]; ///< 拼接port需要connectID
    U8      pad2[5];
} Ps3LibPdPathInfo_t;

/**
 * @brief   设置pd状态信息结构体
 */
typedef struct Ps3LibPdSetState{
    U8     state;       ///< 硬盘状态
    U8     forceFlag;   ///< 强制标记
}Ps3LibPdSetState_t;

/**
 * @brief   
 */
typedef struct Ps3LibIdGroupState {
    union {
        U16                 deviceId;      ///< 设备devId
        Ps3LibPdPosition_t  pdPosition;    ///< pd位置
    };
} Ps3LibIdGroupState_t;

/**
 * @brief   多盘同时设置pd状态结构体
 */
typedef struct Ps3LibPdAllSetState {
    Ps3LibPdSetState_t   pdState;
    U8                   pad[2];
    S32                  count;
    U8                   type;
    U8                   pad1[3];
    Ps3LibIdGroupState_t values[PS3LIB_MAX_PD_NUM];
} Ps3LibPdAllSetState_t;

/**
 * @brief   DG中盘的位置
 */
typedef struct Ps3LibPdPositionInDG{
    DgId_t  dgId;           ///< dg ID
    U16     arrayIdx;       ///< 磁盘子组
    U16     rowIdx;         ///< 盘在磁盘子组中的index
} Ps3LibPdPositionInDG_s;

/**
 * @brief   硬盘后台任务信息,因为后台任务互斥,所以只需要一个Ps3LibProgress_t
 */
typedef struct Ps3LibPdProgressInfo{
    U8      rbld;       ///< 重建
    U8      patrol;     ///< 巡读
    U8      clear;      ///< 初始化
    U8      moveBack;   ///< 回拷
    U8      erase;      ///< 擦除
    U8      locate;     ///< 定位(enum Ps3LibProgressStatus_e 运行|未运行)
    U8      sanitize;   ///< sanitize
    U8      pad;
    U16     moveBackPartnerId;  ///< not_support    回拷时对端硬盘ID
    U8      pad1[2];
    Ps3LibProgress_t   rbldProgress;       ///< 重建状态
    Ps3LibProgress_t   patrolProgress;     ///< 巡读状态
    Ps3LibProgress_t   clearProgress;      ///< 初始化状态
    Ps3LibProgress_t   moveBackProgress;   ///< 回拷状态
    Ps3LibProgress_t   eraseProgress;      ///< 擦除状态
    Ps3LibProgress_t   sanitizeProgress;   ///< 擦除状态
    struct {
        U8      pdm;        ///< PDM
        U8      pad2[7];     ///< 保留字段
        Ps3LibProgress_t   pdmProgress;        ///< PDM状态
    } ext;      ///< v200及后续版本支持
} Ps3LibPdProgressInfo_t;

/**
 * @brief   pd init 结构体
 */
typedef struct Ps3LibPdInitInfo{
    U8  modeDataLen;    ///< 模式数据长度, 不超过BGT_FGI_MODE_DATA_MAX_LEN, 且为2的n次幂
    U8  modeData[PS3LIB_BGT_FGI_MODE_DATA_MAX_LEN];    ///< 模式数据
}Ps3LibPdInitInfo_s;

typedef struct Ps3LibPdEraseInfo{
    U8  type;
    U8  patternA; ///< 暂不支持
    U8  patternB; ///< 暂不支持
} Ps3LibPdEraseInfo_t;

/**
 * @brief   热备盘信息
 */
typedef struct Ps3LibPdHotSpareInfo{
    U32     dgNum;          ///< 当前热备提供给几个DG使用
    DgId_t  dgId[PS3LIB_MAX_DG_PER_DEDICATED_SPARE];       ///< 对应的磁盘组编号
} Ps3LibPdHotSpareInfo_t;

/**
 * @brief   硬盘SMART信息
 */
typedef struct Ps3LibPdSmartInfo{
    U32     powerOnTime;                    ///< 开机时间(小时)
    U32     bbmErrCountSupported    : 1;    ///< 是否支持坏块管理计数
    U32     bbmErrCount             : 31;   ///< 坏块管理计数
    U32     mediaErrCount;                  ///< 介质错误计数
    U32     otherErrCount;                  ///< 其他错误计数
    U32     predFailCount;                  ///< 预失败计数
    U32     shieldCount;                    ///< 硬盘Failed之前的诊断次数 
    U8      temperature;                    ///< 温度(摄氏度)
    U8      pad[3];
    U32     lastPredFailEventSeqNum;        ///< 上一个预失败事件日志序列号
    U32     shieldDiagCompletionTime;       ///< 上一次诊断完成时间 已废弃
    U8      badBlockTableFull;              ///< 坏块表
    U8      ssdPercentUsed;                 ///< SSD使用百分比
    U16     ssdLifeRemainingInDays;         ///< SSD剩余寿命(days)
    U8      smartAlmFlaggedByDrive;         ///< 是否smart告警 已废弃
    U8      pad1[3];
    U16     driveErrCount;                  ///< 硬盘错误计数
    U16     slotErrCount;                   ///< 槽位错误计数
    U8      WCE;                            ///< 写缓存使能
    U8      pad2[3];
} Ps3LibPdSmartInfo_t;

/**
 * @brief   硬盘基本信息
 */
typedef struct Ps3LibPdBaseInfo {
    U16     deviceId;               ///< 标识符
    U16     scsiDevId :12;          ///< 暂时不用，与元数据对齐
    U16     channelId :4;           ///< 所属通道ID
    U8      enclId;                 ///< 机箱号
    U8      piEligible;             ///< 是否支持PI
    U16     slotId;                 ///< 槽位号
    U8      pdState;                ///< PD状态(enum Ps3LibPdState_t)
    U8      sanitize : 1;           ///< 是否正在执行sanitize
    U8      pad : 7;
    U16     dgIndex;                ///< 硬盘组索引
    U8      dgCount;                ///< 该热备盘所属磁盘组数量(仅在该盘为hot spare时生效)
    U8      pfaFlag;                ///< 预失败标记
    U8      pad2[2];
    U16     dgId[PS3LIB_MAX_DG_PER_DEDICATED_SPARE];           ///< 该热备盘所属磁盘组列表(仅在该盘为hot spare时生效)
    U8      manufactureID[PS3LIB_MANUFACTURE_ID_LEN];      ///< 生产商
    U8      serialNumber[PS3LIB_PD_SERIAL_NUMBER_LEN];     ///< 序列号
    U8      modelNumber[PS3LIB_MODEL_NUMBER_LEN];          ///< 型号
    U8      firmwareRevision[PS3LIB_FW_REVISION_LEN];      ///< 固件版本
    U8      pad3[8];
    U64     WWN;                    ///< 全球唯一名称
    U64     rawSize;                ///< 原始大小 逻辑扇区大小个数
    U64     coercedSize;            ///< 对齐大小
    U64     nonCoercedSize;          ///< 非对齐大小
    U8      interfaceType;          ///< 接口类型(enum Ps3LibPdInterface_e)
    U8      mediaType;              ///< 介质类型(enum Ps3LibPdMediaType_e)
    U16     physicalSectorSize;     ///< 物理扇区大小 emulatedBlockSize
    U16     logicalSectorSize;      ///< 逻辑扇区大小 userDataBlockSize
    U8      sedEnable;              ///< 是否自加密硬盘
    U8      piType;                 ///< protection info类型
    U8      spinState;              ///< 当前起转状态
    U8      nvmeLaneWidth;          ///< NVME盘链路带宽
    U8      pad4[6];
    struct {
        U8      pdTask;                 ///< 硬盘当前正在执行的操作 enum Ps3LibPdTask_e
        U8      pad5[7];
    } ext;      ///< 2.x支持的字段
} Ps3LibPdBaseInfo_t;

/**
 * @brief   硬盘允许的操作
 */
typedef struct Ps3LibPdAllowedOps{   ///< 与Ps3LibCtrlGetAdapter中的部分可共用
    U32     makeOnline              : 1;    ///< 支持设置为online状态
    U32     makeOffline             : 1;    ///< 支持设置为offline状态
    U32     makeFailed              : 1;    ///< 支持设置为failed状态
    U32     makeReady               : 1;    ///< 支持设置为ready状态
    U32     makeSpare               : 1;    ///< 支持设置为spare状态
    U32     removeSpare             : 1;    ///< 支持移除热备盘
    U32     replaceMissing          : 1;    ///< 支持热备替换
    U32     markMissing             : 1;    ///< 支持标记missing盘
    U32     startRebuild            : 1;    ///< 支持重建
    U32     stopRebuild             : 1;    ///< 支持停止重建
    U32     locate                  : 1;    ///< 支持硬盘定位
    U32     pdClear                 : 1;    ///< 支持硬盘初始化
    U32     foreignImportNotAllowed : 1;    ///< 不允许外部盘导入
    U32     startCopyBack           : 1;    ///< 支持回拷
    U32     stopCopyBack            : 1;    ///< 支持停止回拷
    U32     fwDownloadNotAllowed    : 1;    ///< 不允许固件下载
    U32     makeSystem              : 1;    ///< 支持设置为系统盘
    U32     T10PowerModeSupported   : 1;    ///< not_support    支持T10电源模式
    U32     suspendRebuild          : 1;    ///< 支持暂停重建
    U32     resumeRebuild           : 1;    ///< 支持恢复重建
    U32     suspendCopyback         : 1;    ///< 支持暂停回拷
    U32     resumeCopyback          : 1;    ///< 支持恢复回拷
    U32     startSecureErase        : 1;    ///< not_support    支持非SED盘的安全擦除
    U32     stopSecureErase         : 1;    ///< not_support    支持非SED盘的停止安全擦除
    U32     SMARTSupported          : 1;    ///< not_support    支持SMART
    U32     prepareForRemoval       : 1;    ///< 支持硬盘待移除状态
    U32     supportFDE              : 1;    ///< not_support    支持FDE
    U32     supportSED              : 1;    ///< not_support    支持SED
    U32     supportPI               : 1;    ///< not_support    支持PI
    U32     supportSelfTest         : 1;    ///< not_support    支持自检
    U32     makeJbod                : 1;    ///< 支持设置jbod
    U32     undoPrepareForRemoval   : 1;    ///< 支持撤销准备删除
    U32     replaceDrive            : 1;    ///< 支持替换盘
    U32     jbodErase               : 1;    ///< 支持擦除jbod
    U32     assignGlobalHotspare    : 1;    ///< 支持设置为全局热备
    U32     removeGlobalHotspare    : 1;    ///< 支持移除全局热备
    U32     assignDedicatedHotspare : 1;    ///< 支持设置为局部热备
    U32     removeDedicatedHotspare : 1;    ///< 支持移除局部热备
    U32     startInit               : 1;    ///< 支持硬盘开始初始化         ps3gui 显示clear drive
    U32     stopInit                : 1;    ///< 支持硬盘停止初始化
    U32     startErase              : 1;    ///< 支持开始擦除
    U32     stopErase               : 1;    ///< 支持停止擦除
    U32     startPdm                : 1;    ///< 开始pdm
    U32     abortPdm                : 1;    ///< 取消pdm
    U32     pausePdm                : 1;    ///< 暂停pdm
    U32     resumePdm               : 1;    ///< 恢复pdm
    U32     sanitize                : 1;    ///< 是否支持sanitize
    U32     supportCreateVd         : 1;    ///< 该pd是否支持创建vd
    U32     isReplacementDrive      : 1;    ///< 是否可以作为被回拷的盘
    U32     foreignDel              : 1;    ///< 支持删除Foreign盘
    U32     foreignImport           : 1;    ///< 支持Foreign导入
    U32     foreignImportPreview    : 1;    ///< 支持Foreign导入状态预览
    U32     fwDownloadAllowed       : 1;    ///< 允许固件下载
    U32     pad                     : 11;   ///< 补齐字段
    ///< 以下字段为上面各个字段的有效性字段，只有对应的有效性字段为true时，才能根据该字段的值判断操作是否支持
    U32     makeOnlineValid              : 1;    ///< 支持设置为online状态
    U32     makeOfflineValid             : 1;    ///< 支持设置为offline状态
    U32     makeFailedValid              : 1;    ///< 支持设置为failed状态
    U32     makeReadyValid               : 1;    ///< 支持设置为ready状态
    U32     makeSpareValid               : 1;    ///< 支持设置为spare状态
    U32     removeSpareValid             : 1;    ///< 支持移除热备盘
    U32     replaceMissingValid          : 1;    ///< 支持热备替换
    U32     markMissingValid             : 1;    ///< 支持标记missing盘
    U32     startRebuildValid            : 1;    ///< 支持重建
    U32     stopRebuildValid             : 1;    ///< 支持停止重建
    U32     locateValid                  : 1;    ///< 支持硬盘定位
    U32     pdClearValid                 : 1;    ///< 支持硬盘初始化
    U32     foreignImportNotAllowedValid : 1;    ///< 不允许外部盘导入
    U32     startCopyBackValid           : 1;    ///< 支持回拷
    U32     stopCopyBackValid            : 1;    ///< 支持停止回拷
    U32     fwDownloadNotAllowedValid    : 1;    ///< 不允许固件下载
    U32     makeSystemValid              : 1;    ///< 支持设置为系统盘
    U32     T10PowerModeSupportedValid   : 1;    ///< 支持T10电源模式
    U32     suspendRebuildValid          : 1;    ///< 支持暂停重建
    U32     resumeRebuildValid           : 1;    ///< 支持恢复重建
    U32     suspendCopybackValid         : 1;    ///< 支持暂停回拷
    U32     resumeCopybackValid          : 1;    ///< 支持恢复回拷
    U32     startSecureEraseValid        : 1;    ///< 支持非SED盘的安全擦除
    U32     stopSecureEraseValid         : 1;    ///< 支持非SED盘的停止安全擦除
    U32     SMARTSupportedValid          : 1;    ///< 支持SMART
    U32     prepareForRemovalValid       : 1;    ///< 支持硬盘待移除状态
    U32     supportFDEValid              : 1;    ///< 支持FDE
    U32     supportSEDValid              : 1;    ///< 支持SED
    U32     supportPIValid               : 1;    ///< 支持PI
    U32     supportSelfTestValid         : 1;    ///< 支持自检
    U32     makeJbodValid                : 1;    ///< 支持设置jbod
    U32     undoPrepareForRemovalValid   : 1;    ///< 支持撤销准备删除
    U32     replaceDriveValid            : 1;    ///< 支持替换盘
    U32     jbodEraseValid               : 1;    ///< 支持擦除jbod
    U32     assignGlobalHotspareValid    : 1;    ///< 支持设置为全局热备
    U32     removeGlobalHotspareValid    : 1;    ///< 支持移除全局热备
    U32     assignDedicatedHotspareValid : 1;    ///< 支持设置为局部热备
    U32     removeDedicatedHotspareValid : 1;    ///< 支持移除局部热备
    U32     startInitValid               : 1;    ///< 支持硬盘开始初始化         ps3gui 显示clear drive
    U32     stopInitValid                : 1;    ///< 支持硬盘停止初始化
    U32     startEraseValid              : 1;    ///< 支持开始擦除
    U32     stopEraseValid               : 1;    ///< 支持停止擦除
    U32     startPdmValid                : 1;    ///< 开始pdm
    U32     abortPdmValid                : 1;    ///< 取消pdm
    U32     pausePdmValid                : 1;    ///< 暂停pdm
    U32     resumePdmValid               : 1;    ///< 恢复pdm
    U32     sanitizeValid                : 1;    ///< 是否支持sanitize
    U32     supportCreateVdValid         : 1;    ///< 该pd是否支持创建vd
    U32     isReplacementDriveValid      : 1;    ///< 是否可以作为被回拷的盘
    U32     foreignDelValid              : 1;    ///< 支持删除Foreign盘
    U32     foreignImportValid           : 1;    ///< 支持Foreign导入
    U32     foreignImportPreviewValid    : 1;    ///< 支持Foreign导入状态预览
    U32     fwDownloadAllowedValid       : 1;    ///< 允许固件下载
    U32     padValid                     : 11;   ///< 补齐字段
}Ps3LibPdAllowedOps_t;

/**
 * @brief   硬盘连接信息
 */
typedef struct Ps3LibPdConnectInfo{
    U16     deviceId;           ///< 设备号
    U16     enclDeviceId;       ///< 连接的encl设备号
    U8      enclIndex;          ///< not_support    连接的encl设备索引
    U8      enclPosition;       ///< 连接的encl级联深度
    U16     slotId;             ///< not_support
    U8      locationType;       ///< not_support    接口类型(enum Ps3LibSasConnLocationType_e)
    U8      pad[7];
    Ps3LibPdPathInfo_t pathInfo;   ///< 硬盘path信息
} Ps3LibPdConnectInfo_t;

/**
 * @brief   硬盘元数据信息(仅在该盘为online或hot spare时生效)
 */
typedef struct Ps3LibPdMetaInfo{
    U16     dgIndex;        ///< 硬盘组索引
    U8      spanIndex;      ///< 硬盘子组索引(仅在该盘为online时生效)
    U8      rowIndex;       ///< 硬盘在子组内的编号(仅在该盘为online时生效)
    U8      spareType;      ///< 热备类型(0:未知,1:局部热备,2:全局热备)
    U8      revertible;     ///< 热备盘可逆
    U8      dgCount;        ///< 该热备盘所属磁盘组数量(仅在该盘为hot spare时生效)
    U8      pad;
    U16     dgId[PS3LIB_MAX_DG_PER_DEDICATED_SPARE];        ///< 该热备盘所属磁盘组列表(仅在该盘为hot spare时生效)
} Ps3LibPdMetaInfo_t;

/**
 * @brief   硬盘扩展信息
 */
typedef struct Ps3LibPdExtInfo{
    U32     piCapble            : 1;    ///< not_support    支持PI 同supportPI
    U32     piType              : 3;    ///< not_support    protection info类型(enum Ps3LibPIType_e)
    U32     piFormatted         : 1;    ///< not_support    1=drive is formatted for PI information, 0=no PI data
    U32     piEligible          : 1;    ///< 1=drive can be used for PD Enable LD
    U32     NCQ                 : 1;    ///< SATA盘NCQ使能
    U32     commissionedSpare   : 1;    ///< 产品确认删除  被使用的热备盘
    U32     emergencySpare      : 1;    ///< 产品确认删除     是否允许此盘做为紧急热备盘
    U32     ineligibleForSSCD   : 1;    ///< not_support
    U32     fdeType             : 3;    ///< not_support    FDE类型(enum Ps3LibFdeType_t)
    U32     fdeCapable          : 1;    ///< not_support    支持FDE,full disk encryption
    U32     fdeEnabled          : 1;    ///< not_support    FDE使能
    U32     powerState          : 2;    ///< not_support    电源状态
    U32     readyForRemoval     : 1;    ///< not_support    可移除
    U32     sedCapable          : 1;    ///< not_support    支持自加密
    U32     sedEnable           : 1;    ///< not_support    自加密使能
    U32     multiPath           : 1;    ///< not_support    多路径寻址
    U32     sanitizeType        : 5;    ///< Bit 0 - CRYPTO ERASE; 1 - OVERWRITE; 2 - BLOCK_ERASE;
                                        ///< Bit 3 - FREEZE LOCK; 4 - ANTI-FREEZE LOCK; All 0 - not support sanitize
    U32     isSanitizeTypeValid : 1;    ///< 表示sanitizeType字段是否有效
    U32     reserved            : 5;
    U8      unmapCapable;               ///< 支持unmap或trim功能
    U8      unmapCapableForLDs;         ///< 支持LD的unmap
    U8      pad[2];
    U8      vpdPage83[PS3LIB_VPD_PAGE_LEN];        ///< not_support    VPD page 83
    U8      vpdPage83Ext[PS3LIB_VPD_PAGE_LEN];     ///< not_support    与vpdPage83结合使用
    U8      inquiryData[PS3LIB_INQUIRY_DATA_LEN];  ///< inquiryData
    U8      secureEnable;                       ///< not_support    secure使能状态
    U8      secureEraseCapable;                 ///< not_support    是否具有加密擦除能力
    U8      locked;                             ///< not_support    硬盘是否locked
    U8      needsEKM;                           ///< not_support    是否需要扩展密钥管理
    U8      certified;                          ///< not_support    硬盘是否经过认证
    U8      inquiryExt[PS3LIB_INQUIRY_DATAEXT_LEN];   ///< inquiryData 额外数据
    U8      pad1[1];
}Ps3LibPdExtInfo_t;

/**
 * @brief   硬盘信息
 */
typedef struct Ps3LibPdInfo{
    Ps3LibPdBaseInfo_t         pdBaseInfo;     ///< 硬盘基本信息
    Ps3LibPdProgressInfo_t     pdProgress;     ///< 硬盘后台任务信息
    Ps3LibPdAllowedOps_t       pdAllowedOps;   ///< 硬盘允许的操作
    U8                         pad1[4];
    Ps3LibPdConnectInfo_t      pdConnect;      ///< 硬盘连接信息
    Ps3LibPdMetaInfo_t         pdMetaInfo;     ///< 硬盘元数据信息
    Ps3LibPdSmartInfo_t        pdSmartInfo;    ///< 硬盘SMART信息
    Ps3LibPdExtInfo_t          pdExtInfo;      ///< 硬盘扩展信息
    U8                         pad2[4];
} Ps3LibPdInfo_t;

typedef struct Ps3LibPdDwldInfo {
    CtrlId_t            ctrlId;                             ///< 控制卡id
    U8                  mode;                               ///< 升级模式
    U8                  parallel;                           ///< 串行/并行升级 只有mode7支持并行
    U8                  chunkSize;                          ///< 分片大小 单位KB 默认32K
    U8                  offline;                            ///< 是否离线模式
    U8                  activateNow;                        ///< 是否立即激活
    U8                  pad[1];    
    U32                 imageSize;                          ///< 文件大小
    U32                 pdIdx;                              ///< 当前处理的盘序号
    U32                 pdCnt;                              ///< 要升级的盘个数
    Ps3LibPdPosition_t  idList[PS3LIB_MAX_PD_NUM_EXT];      ///< 需要升级的硬盘列表
    PdId_t              devIdList[PS3LIB_MAX_PD_NUM_EXT];   ///< devId列表
    U8                  devTypeList[PS3LIB_MAX_PD_NUM_EXT]; ///< devType列表
} Ps3LibPdDwldInfo_t;

/**
 * @brief 存放机箱号等信息
 */
typedef struct Ps3LibPhyPosition {
    U64 enclSasAddr;     ///< phy所属机框sas地址
    U64 phySasAddr;      ///< phy的sas地址,
    U8  enclId;          ///< 机箱号
    U8  phyId;           ///< phy唯一ID
    U8  pad[6];
} Ps3LibPhyPosition_t;

/**
 * @brief 存放盘信息
 */
typedef struct Ps3LibIdGroup {
    U8      type;                      ///< 盘标识(物理盘、虚拟盘等)
    U8      pad[7];
    union{
      U16                 deviceId;    ///< 包括/ex/sx /ex /sx
      Ps3LibPdPosition_t  pdPosition;  ///< 存放背板ID和slotId
      U16                 vdId;        ///< vd ID
      U16                 dgId;        ///< dg ID
      Ps3LibPhyPosition_t phyPosition; ///< 存放机箱号phy信息
      U16                 laneId;      ///< lane ID
    };
} Ps3LibIdGroup_t;

/**
 * @brief scsi请求cdb信息
 */
typedef struct Ps3LibScsiCdbInfo{
    U8   cdbLen;                   ///< cdb数据长度
    U8   cdb[PS3LIB_SCSI_CDB_LEN]; ///< cdb数据
} Ps3LibScsiCdbInfo_t;

/**
 * @brief scsi执行结果信息
 */
typedef struct Ps3LibScsiRetInfo {
    U8   status;                                   ///< scsi执行结果
    U8   pad[3];
    U32  xferCnt;
    U8   senseData[PS3LIB_SCSI_SENSE_BUFFER_LEN];  ///< 透传的scis数据
} Ps3LibScsiRetInfo_t;

/*
 * 硬盘升级准备命令更新需要的参数
 */
typedef struct Ps3LibPdFwDwldParam {
    U16   cdbCount;                         ///< pd升级文件分片个数
    U8    updateTime;                       ///< pd升级所需要的时间最大为120s
    U8    options;                          ///< 保留字段
    U8    parallel;                         ///< 是否支持并行升级
    U8    forceParallel;                    ///< 保留字段
    U8    pad[2];
    U32   imageSize;                        ///< 升级文件大小
    U8    pdFwDownloadMap[PS3LIB_MAX_PD_NUM_EXT/8];    ///< 升级ID列表
} Ps3LibPdFwDwldParam_t;

/**
 * @brief   硬盘write cache状态
 */
typedef struct Ps3LibPdWriteCache {
    U8      writeCacheStatus;        ///< 硬盘write cache状态 0-off 1-on
    U8      pad[3];                  ///< 字节对齐
} Ps3LibPdWriteCache_t;

/**
 * @brief   硬盘write cache相关状态
 */
typedef enum Ps3LibWriteCacheState {
    PS3LIB_WRITE_CACHE_OFF     = 0,
    PS3LIB_WRITE_CACHE_ON      = 1,
    PS3LIB_WRITE_CACHE_NR      = 2,
} Ps3LibWriteCacheState_e;

/**
 * @brief   硬盘状态枚举
 */
typedef enum Ps3LibPdState {
    PS3LIB_PD_STATE_UNKNOWN = 0,
    PS3LIB_PD_STATE_READY,              ///< UGOOD
    PS3LIB_PD_STATE_UBAD,               ///< Unonfigured bad
    PS3LIB_PD_STATE_DSPARE,             ///< 局部热备
    PS3LIB_PD_STATE_GSPARE,             ///< 全局热备
    PS3LIB_PD_STATE_OFFLINE,            ///< 离线， 用户配置
    PS3LIB_PD_STATE_ONLINE,             ///< 在线
    PS3LIB_PD_STATE_MISSING,            ///< DG成员盘不在位
    PS3LIB_PD_STATE_FAILED,             ///< 硬盘故障
    PS3LIB_PD_STATE_REBUILD,            ///< rebuild中
    PS3LIB_PD_STATE_REPLACE,            ///< 回迁移中
    PS3LIB_PD_STATE_FOREIGN,            ///< 有元数据，还没有加载
    PS3LIB_PD_STATE_JBOD,               ///< JOBD盘
    PS3LIB_PD_STATE_UNSUPPORT,          ///< 超系统硬盘规格的盘
    PS3LIB_PD_STATE_PDM,                ///< 预先数据迁移
    PS3LIB_PD_STATE_CFSHLD,             ///< vd成员盘异常修复中
    PS3LIB_PD_STATE_HSPSHLD,            ///< 热备盘异常修复中
    PS3LIB_PD_STATE_READY_UNSUPPORT,    ///< UGOOD unsupport
    PS3LIB_PD_STATE_UBAD_UNSUPPORT,     ///< UBAD unsupport
    PS3LIB_PD_STATE_NR,
} Ps3LibPdState_t;

///< 硬盘起转相关状态
typedef enum Ps3LibDiskSpinUpState {
    PS3LIB_DISK_SPIN_INVALID = 0,
    PS3LIB_DISK_SPIN_UP      = 1,
    PS3LIB_DISK_SPIN_TRANS   = 2,
    PS3LIB_DISK_SPIN_DOWN    = 3,
} Ps3LibDiskSpinUpState_e;

/**
 * @brief   硬盘接口类型枚举
 */
typedef enum Ps3LibPdInterface{
    PS3LIB_PD_INTERFACE_TYPE_UNKNOWN  = 0,    ///< unknown
    PS3LIB_PD_INTERFACE_TYPE_SAS      = 1,    ///< SAS
    PS3LIB_PD_INTERFACE_TYPE_SATA     = 2,    ///< SATA
    PS3LIB_PD_INTERFACE_TYPE_SES      = 3,    ///< SAS
    PS3LIB_PD_INTERFACE_TYPE_NVME     = 4,    ///< NVME
    PS3LIB_PD_INTERFACE_TYPE_NR       = 5,
}Ps3LibPdInterface_e;

/**
 * @brief   wce状态枚举
 */
typedef enum Ps3LibPdWceType{
    PS3LIB_PD_WCE_TYPE_OFF = 0,
    PS3LIB_PD_WCE_TYPE_ON,
    PS3LIB_PD_WCE_TYPE_NA,
}Ps3LibPdWceType_e;

typedef enum Ps3LibIdGroupType{
    PS3LIB_ID_GROUP_TYPE_UNKNOWN       = 0,
    PS3LIB_ID_GROUP_TYPE_DEVICE_ID        ,
    PS3LIB_ID_GROUP_TYPE_PD_POSITION      ,
    PS3LIB_ID_GROUP_TYPE_VD_ID            ,
    PS3LIB_ID_GROUP_TYPE_BBU_ID           ,
    PS3LIB_ID_GROUP_TYPE_DG_ID            ,
    PS3LIB_ID_GROUP_TYPE_PHY_ID           ,
    PS3LIB_ID_GROUP_TYPE_LANE_ID          ,
    PS3LIB_ID_GROUP_TYPE_ASO_ID           ,
    PS3LIB_ID_GROUP_TYPE_ENCL_ID          ,
}Ps3LibIdGroupType_e;

/**
 * @brief   硬盘后台任务状态
 */
typedef enum Ps3LibProgressStatus{
    PS3LIB_PROGRESS_RUNNING          = 0x01,         ///< 运行
    PS3LIB_PROGRESS_PAUSE            = 0x02,         ///< 暂停
    PS3LIB_PROGRESS_NOT_RUNNING      = 0x04,         ///< 未运行
    PS3LIB_PROGRESS_INTERNAL_PAUSE   = 0x08,         ///< 后台任务内部暂停
    PS3LIB_PROGRESS_WAIT_FOR_RUNNING = 0x10,         ///< HBA卡被阻塞的任务，等待运行
}Ps3LibProgressStatus_e;

/**
 * @brief   硬盘安全擦除类型枚举
 */
typedef enum Ps3LibPdEraseType{
    PS3LIB_PD_ERASE_TYPE_SIMPLE        = 0,    ///< simple
    PS3LIB_PD_ERASE_TYPE_NORMAL        = 1,    ///< normal
    PS3LIB_PD_ERASE_TYPE_THOROUGH      = 2,    ///< thorough
    PS3LIB_PD_ERASE_TYPE_STANDARD      = 3,    ///< standard
    PS3LIB_PD_ERASE_TYPE_THREE_PASS    = 4,    ///< threepass
    PS3LIB_PD_ERASE_TYPE_CRYPTO        = 5,    ///< crypto
}Ps3LibPdEraseType_t;

/**
 * @brief   硬盘介质类型枚举
 */
typedef enum Ps3LibPdMediaType{
    PS3LIB_PD_MEDIA_TYPE_UNKNOWN       = 0,    ///< unknown
    PS3LIB_PD_MEDIA_TYPE_ROTATIONAL    = 1,    ///< HDD
    PS3LIB_PD_MEDIA_TYPE_SSD           = 2,    ///< SSD
    PS3LIB_PD_MEDIA_TYPE_SSM_FLASH     = 3,    ///< Solid state flash module
}Ps3LibPdMediaType_e;

/**
 * @brief   硬盘设备速度枚举
 */
typedef enum Ps3LibPdDeviceSpeed{
    PS3LIB_PD_SPEED_LINKDOWN           = 0,        ///< disable

    PS3LIB_PD_SPEED_PCIE_2_5GT         = 0x1,     ///< 2.5GT/s
    PS3LIB_PD_SPEED_PCIE_5GT           = 0x2,     ///< 5GT/s
    PS3LIB_PD_SPEED_PCIE_8GT           = 0x3,     ///< 8GT/s
    PS3LIB_PD_SPEED_PCIE_16GT          = 0x4,     ///< 16GT/s

    PS3LIB_PD_SPEED_1p5G               = 0x8,        ///< 1.5Gb/s - SATA 150
    PS3LIB_PD_SPEED_3G                 = 0x9,        ///< 3.0Gb/s
    PS3LIB_PD_SPEED_6G                 = 0xA,        ///< 6.0Gb/s
    PS3LIB_PD_SPEED_12G                = 0xB,        ///< 12.0Gb/s
    PS3LIB_PD_SPEED_22p5G              = 0xC,        ///< 22.5Gb/s
}Ps3LibPdDeviceSpeed_e;

/**
 * @brief   硬盘固件升级模式
 */
typedef enum Ps3LibPdDwldMode{
    PS3LIB_PD_DWLD_MODE_7 = 0x7,     ///< mode 7
    PS3LIB_PD_DWLD_MODE_E = 0xE,     ///< mode E
    PS3LIB_PD_DWLD_MODE_F = 0xF,     ///< mode F
} Ps3LibPdDwldMode_e;

/**
 * @brief   PI类型枚举
 */
typedef enum Ps3LibPIType{
    PS3LIB_PROTECTION_INFO_TYPE_NONE   = 0,    ///< No SCSI protection type used
    PS3LIB_PROTECTION_INFO_TYPE_1      = 1,    ///< SCSI PI Type 1
    PS3LIB_PROTECTION_INFO_TYPE_2      = 2,    ///< SCSI PI Type 2
    PS3LIB_PROTECTION_INFO_TYPE_3      = 3,    ///< SCSI PI Type 3
    PS3LIB_PROTECTION_INFO_TYPE_RES_4  = 4,    ///< Reserved SCSI PI Type 4
    PS3LIB_PROTECTION_INFO_TYPE_RES_5  = 5,    ///< Reserved SCSI PI Type 5
    PS3LIB_PROTECTION_INFO_TYPE_RES_6  = 6,    ///< Reserved SCSI PI Type 6
    PS3LIB_PROTECTION_INFO_TYPE_RES_7  = 7,    ///< Reserved SCSI PI Type 7
}Ps3LibPIType_e;

/**
 * @brief   硬盘任务枚举
 */
typedef enum Ps3LibPdTask {
    PS3LIB_PD_TASK_NONE            = 0,    ///< 无硬盘任务
    PS3LIB_PD_TASK_SANITIZING      = 1,    ///< 正在执行sanitize
    PS3LIB_PD_TASK_FORMATTING      = 2,    ///< 正在执行format
    PS3LIB_PD_TASK_FORMAT_FAILED   = 3,    ///< format失败
    PS3LIB_PD_TASK_SENDDIAG        = 4,    ///< 自检
} Ps3LibPdTask_e;

/**
 * @brief   FDE类型枚举
 */
typedef enum Ps3LibFdeType{
    PS3LIB_PD_FDE_TYPE_NONE        = 0,    ///< PD does not support TCG (non-FDE PD)
    PS3LIB_PD_FDE_TYPE_ENTERPRISE  = 1,    ///< PD supports TCG Enterprise SSC
    PS3LIB_PD_FDE_TYPE_OPAL        = 2,    ///< PD supports TCG OPAL SSC
}Ps3LibFdeType_t;

/**
 * @brief   热备盘类型枚举
 */
typedef enum Ps3LibSpareType{
    PS3LIB_SPARE_TYPE_UNKNOWN     = 0,    ///< 未知
    PS3LIB_SPARE_TYPE_DEDICATED   = 1,    ///< 局部热备盘
    PS3LIB_SPARE_TYPE_GLOBAL      = 2,    ///< 全局热备盘
}Ps3LibSpareType_e;

/**
 * @brief   硬盘后台任务是否自动开启
 */
typedef enum Ps3LibPropertiesAutoStatus{
    PS3LIB_PROPERTIES_NOT_RUNNING = 0,        ///< 关闭自动开启
    PS3LIB_PROPERTIES_RUNNING     = 1,        ///< 自动开启
}Ps3LibPropertiesAutoStatus_t;

/**
 * @brief       获取指定物理盘smart信息
 * @param[in]   ctrlId:    控制卡标识符
 * @param[in]   deviceId:  物理盘Id
 * @param[out]  smartInfo: 物理盘smart信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdSmartInfoGet(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdSmartInfo_t *smartInfo);

/**
 * @brief       开始物理盘擦除
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[in]   eraseInfo: 擦除参数
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdEraseStart(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdEraseInfo_t *eraseInfo);

/**
 * @brief       停止物理盘擦除
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdEraseAbort(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       获取物理盘基础信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[out]  info: 物理盘基础信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdBaseInfoGetByDevId(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdBaseInfo_t *info);

/**
 * @brief       获取物理盘允许的操作
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[out]  info: pd允许的操作
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdAllowedOpsGet(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdAllowedOps_t *info);

/**
 * @brief       将指定物理盘插入dg组中
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[in]   pdPosInDG: 物理盘插入磁盘组中的位置
 * @param[in]   forceFlag: 盘包含分区表时是否进行此操作(1:强制, 0:非强制)
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdInsertToDg(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdPositionInDG_s *pdPosInDG, U8 forceFlag);

/**
 * @brief       获取指定物理盘的后台任务进度信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[out]  progInfo: 硬盘后台任务信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdProgressInfoGet(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdProgressInfo_t *progInfo);

/**
 * @brief       设置指定物理盘点灯状态
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdLocateStartByDevId(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       终止磁盘点灯
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdLocateAbort(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       将指定物理盘添加为全局热备盘
 * @param[in]   ctrlId:    控制卡标识符
 * @param[in]   deviceId:  物理盘Id
 * @param[in]   forceFlag: 盘包含分区表时仍然进行此操作(1:强制, 0:非强制)
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdGHotSpareAdd(CtrlId_t ctrlId, U16 deviceId, U8 forceFlag);

/**
 * @brief       将指定物理盘添加为局部热备盘, 可指定磁盘组个数和磁盘组Id
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[in]   forceFlag: 盘包含分区表时仍然进行此操作(1:强制, 0:非强制)
 * @param[in]   hotSpareInfo: 热备盘信息, 包括磁盘组个数和磁盘组Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdDHotSpareAdd(CtrlId_t ctrlId, U16 deviceId, U8 forceFlag, Ps3LibPdHotSpareInfo_t *hotSpareInfo);

/**
 * @brief       获取指定控制卡下pd数量与位置信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  pdList: pd列表信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdListGetByCtrlId(CtrlId_t ctrlId, Ps3LibPdList_s *pdList);

/**
 * @brief       获取指定控制卡下pd数量与物理盘Id
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  pdDevIdList: pd列表信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdDevIdListGetByCtrlId(CtrlId_t ctrlId, Ps3LibPdDevIdList_s *pdDevIdList);

/**
 * @brief       批量设置物理盘状态
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   info:   批量设置的物理盘状态信息
 */
Ps3Errno ps3libPdStateSet(CtrlId_t ctrlId, Ps3LibPdAllSetState_t *info);

/**
 * @brief       开始物理盘重建
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdRebuildStart(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       开始物理盘重建
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdRebuildAbort(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       暂停物理盘重建
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdRebuildPause(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       恢复物理盘重建
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdRebuildResume(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       开始物理盘前台初始化
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[in]   initInfo: 物理盘init信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdInitStart(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdInitInfo_s *initInfo);

/**
 * @brief       停止物理盘前台初始化
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdInitAbort(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       移除指定物理盘作为全局热备盘的功能
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdHotSpareDelete(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       指定物理盘下电
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdSpinUp(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       指定物理盘下电
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdSpinDown(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       开始物理盘回拷
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   srcDeviceId: 要回拷的源物理盘Id
 * @param[in]   desDeviceId: 要回拷的目的物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdMovebackStart(CtrlId_t ctrlId, U16 srcDeviceId, U16 desDeviceId);

/**
 * @brief       停止物理盘回拷
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdMovebackAbort(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       暂停物理盘回拷
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdMovebackPause(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       恢复物理盘回拷
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdMovebackResume(CtrlId_t ctrlId, U16 deviceId);

/**
 * @brief       根据物理盘Id获取物理盘基本信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[out]  info: 物理盘基本信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdInfoGetByDeviceId(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdInfo_t *info);

/**
 * @brief       将指定物理盘设置为启动盘
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[in]   isBootDrive: 物理盘是否设置为启动盘
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdBootDriveSetByDevId(CtrlId_t ctrlId, U16 deviceId, U8 isBootDrive);

/**
 * @brief       将指定物理盘设置为启动盘
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   enclId  机框号
 * @param[in]   slotId  物理盘槽位号
 * @param[in]   isBootDrive: 物理盘是否设置为启动盘(0:取消设置, 1:设置)
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdBootDriveSetByPosition(CtrlId_t ctrlId, EnclId_t enclId, SlotId_t slotId, U8 isBootDrive);

/**
 * @brief       安全擦除指定物理盘
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   deviceId: 物理盘Id
 * @param[in]   isForceFlag: 是否强制安全擦除(0:非强制, 1:强制)
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdSecureErase(CtrlId_t ctrlId, U16 deviceId, U8 isForceFlag);

/**
 * @brief       获取指定物理盘信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   enclId: 物理盘所属机框号
 * @param[in]   slotId: 物理盘槽位号
 * @param[out]  info: 指向接收存放物理盘信息的指针
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdInfoGetByPosition(CtrlId_t ctrlId, EnclId_t enclId, SlotId_t slotId, Ps3LibPdInfo_t *info);

/**
 * @brief       获取pd列表下所有pd信息
 * @param[in]   ctrlId:     控制卡标识符
 * @param[in]   pdList:     pd列表信息
 * @param[out]  pdInfo:     用户接受回复的批量pd信息
 * @note        无
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdInfoGetByPdList(CtrlId_t ctrlId, Ps3LibPdList_s *pdList, Ps3LibPdInfo_t *pdInfo);

/**
 * @brief        获取指定的物理盘的基本信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    enclId: 物理盘所属机框号
 * @param[in]    slotId: 物理盘槽位号
 * @param[out]   baseInfo: 物理盘基本信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdBaseInfoGet(CtrlId_t ctrlId, EnclId_t enclId, SlotId_t slotId, Ps3LibPdBaseInfo_t *baseInfo);

/**
 * @brief      检查控制卡是否支持并行升级和串行升级
 * @param[in]  pdDwldInfo: 硬盘升级信息
 * @return     PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libIsCtrlAndParallelAllowUpdate(Ps3LibPdDwldInfo_t *pdDwldInfo);

/**
 * @brief       针对pd盘下发scsi协议，目前只封装了写命令
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   enclId: 机框号
 * @param[in]   slotId: 物理盘槽位号
 * @param[in]   cdbInfo: scsi命令描述
 * @param[out]  scsiData: scsi resp data
 * @param[out]  scsiLen: scsi resp len
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdScsiPassthru(CtrlId_t ctrlId, Ps3LibIdGroup_t *idGroup, Ps3LibScsiCdbInfo_t *cdbInfo,
        Ps3LibScsiRetInfo_t *scsiRet, void *scsiData, U32 scsiLen);

/**
 * @brief       允许硬盘升级命令
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   enable: 硬盘升级使能
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdFwDwldEnable(CtrlId_t ctrlId, U8 enable);

/**
 * @brief    判断硬盘是否支持升级
 * @param    pdDwldInfo, 硬盘升级内容
 * @return   Ps3Errno
 */
Ps3Errno ps3libPdAllowUpdate(Ps3LibPdDwldInfo_t *pdDwldInfo);

/**
 * @brief       硬盘升级准备命令
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   pParam: 硬盘升级参数信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdFwDwldPrepare(CtrlId_t ctrlId, Ps3LibPdFwDwldParam_t *param);

/**
 * @brief       设置指定物理盘点灯状态
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   enclId: 物理盘所属机框号
 * @param[in]   slotId: 物理盘槽位号
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdLocateStartByPosition(CtrlId_t ctrlId, EnclId_t enclId, SlotId_t slotId);

/**
 * @brief       根据devIdList获取pd列表下所有pd信息
 * @param[in]   ctrlId:     控制卡标识符
 * @param[in]   pdList:     pd列表信息
 * @param[out]  pdInfo:     用户接受回复的批量pd信息
 * @note        无
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdInfoGetByDevIdList(CtrlId_t ctrlId, Ps3LibPdDevIdList_s *pdDevIdList, Ps3LibPdInfo_t *pdInfo);

/**
 * @brief       设置物理盘 write cache status
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   idGroup: id号(encl/slot)
 * @param[in]   pWriteCacheStatus: 物理盘write cache status
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdWriteCacheStatusSet(CtrlId_t ctrlId, 
        Ps3LibIdGroup_t *idGroup, Ps3LibPdWriteCache_t *pWriteCacheStatus);

#endif
