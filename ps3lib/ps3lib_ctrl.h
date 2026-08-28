/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_ctrl.h
 * @date    2023.04.25
 * @brief   控制卡相关数据结构与接口
 */
#ifndef __PS3LIB_CTRL_H__
#define __PS3LIB_CTRL_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "ps3lib_system.h"
#include "ps3lib_bbu.h"
#include "ps3lib_phy.h"

#define PS3LIB_MAX_PD_NUM                  (1026)   ///< ctrl支持的最大pd数量
#define PS3LIB_MAX_VD_PER_CTRL_RAID      (128 + 4)  ///< 最大虚拟盘数量
#define PS3LIB_VPD_PAGE_LEN                   (64)  ///< vpd page 长度
#define PS3LIB_MAX_SECURITY_KEY_LEN           (32)  ///< 最大秘钥长度
#define PS3LIB_MAX_SECURITY_PASSPHRASE_LEN    (32)  ///< 最大口令长度
#define PS3LIB_SECURITY_PASSPHRASE_ENCODE_LEN (32)  ///< 口令长度
#define PS3LIB_MAX_SECURITY_KEYID_LEN         (64)  ///< 最大keyId长度
#define PS3LIB_MIN_SECURITY_KEY_LEN           (8)   ///< 最小秘钥长度
#define PS3LIB_MIN_SECURITY_PASSPHRASE_LEN    (8)   ///< 最小口令长度
#define PS3LIB_MIN_SECURITY_KEYID_LEN         (1)   ///< 最小keyId长度
#define PS3LIB_FW_VERSION_LEN                 (32)  ///< 固件版本长度
#define PS3LIB_PRODUCT_NAME_LEN               (80)  ///< 产品名长度
#define PS3LIB_CTRL_SAS_COUNT                 (3 + 13)    ///< 预留
#define PS3LIB_BIOS_VERSION_LEN               (32)  ///< BIOS版本长度
#define PS3LIB_MAX_CTRL_COUNT                 (128)  ///< 最大控制卡数量由32变更为256
#define PS3LIB_ECC_TYPE                       (2)   ///< ECC类型
#define PS3LIB_ECC_TYPE_EX                    (3)   ///< ECC类型
#define PS3LIB_BATTERY_FRU                    (17)  ///< 不同配置组的最大个数
#define PS3LIB_MAX_SNAP_COUNT                 (32)    ///< 最大snapshot个数
#define PS3LIB_SIZE_PER_SNAP_RD               (31 * 1024) ///< 每次读取snapshot数据大小 31k 给各种头留出空间
#define PS3LIB_ECC_OCM                        (0)   ///< ocm ecc计数索引值
#define PS3LIB_ECC_DDR                        (1)   ///< ocm ddr cpu计数索引值
#define PS3LIB_ECC_CPU                        (2)   ///< ocm ddr cpu计数索引值
#define PS3LIB_CTRL_CC_PR_PAD_LEN             (15)  ///< 一致性校验下发mode结构体占位符
#define PS3LIB_TOPOLOGY_BUFFER_SIZE           (32 * 1024) ///< 32K
#define PS3LIB_EXP_FRAG_LEN                   (4096)  ///< exp升级单包最大长度
#define PS3LIB_TERMLOG_INFO_MAX_LEN           (1024 * 16) ///< 单次请求最大数据长度
#define PS3LIB_MAX_ENCL_PER_CONTROLLER        (255)   ///< exp最多64个
#define PS3LIB_CTRL_TEMPSENSOR_COUNT_MAX      (10)    ///< 控制卡的温度传感器的最大个数
#define PS3LIB_CTRL_PERM_RULES_COUNT          (32)    ///< 权限规则个数

/**
 * @brief   后台任务进度信息
 */
typedef struct Ps3libProgress{
    U32     progressPercent;    ///< 进度百分比
    U32     remainSecs;         ///< 预计剩余时间
} Ps3LibProgress_t;

typedef enum Ps3LibIdGroupType {
    PS3LIB_ID_GROUP_TYPE_UNKNOWN    = 0,  ///< 无效的类型
    PS3LIB_ID_GROUP_TYPE_DEVICE_ID,       ///< Device Id
    PS3LIB_ID_GROUP_TYPE_PD_POSITION,     ///< Enclosure Id + slot Id
    PS3LIB_ID_GROUP_TYPE_VD_ID,           ///< VD Id
    PS3LIB_ID_GROUP_TYPE_BBU_ID,          ///< BBU Id
    PS3LIB_ID_GROUP_TYPE_DG_ID,           ///< Disk Droup Id
    PS3LIB_ID_GROUP_TYPE_PHY_ID,          ///< Phy Id
    PS3LIB_ID_GROUP_TYPE_LANE_ID,         ///< Lane Id
    PS3LIB_ID_GROUP_TYPE_ASO_ID,          ///< ASO Id
    PS3LIB_ID_GROUP_TYPE_ENCL_ID,         ///< Enclosure Id
    PS3LIB_ID_GROUP_TYPE_FRGN_ID,         ///< Foreign configuration Id
} Ps3LibIdGroupType_e;

/**
 * @brief pd位置信息结构体
 */
typedef struct Ps3LibPdPosition {
    U8  enclId;    ///< 背板标识符
    U8  pad;       ///< 保留字段
    U16 slotId;    ///< 槽位标识符
} Ps3LibPdPosition_t;

/**
 * @brief phy位置的结构体
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
    U8      type;                      ///< 盘标识(物理盘、虚拟盘等) Ps3LibIdGroupType_e
    U8      pad[7];
    union {
      U16                 deviceId;    ///< 包括encl slot
      Ps3LibPdPosition_t  pdPosition;  ///< 存放背板ID和slotId
      U16                 vdId;        ///< vd ID
      U16                 dgId;        ///< dg ID
      Ps3LibPhyPosition_t phyPosition; ///< 存放机箱号phy信息
      U16                 laneId;      ///< lane ID
    };
} Ps3LibIdGroup_t;

/**
 * @brief    控制卡口令模式
*/
typedef enum Ps3LibPassphraseMode {
    PS3LIB_PASSPHRASE_MODIFY_NOT_EMPTY = 0,    ///< 修改口令，口令不为空
    PS3LIB_PASSPHRASE_MODIFY_EMPTY     = 1,    ///< 修改口令，口令为空
    PS3LIB_PASSPHRASE_NOT_MODIFY       = 2,    ///< 不修改口令
} Ps3LibPassphraseMode_e;

/**
 * @brief    控制卡加密模式
*/
typedef enum Ps3LibSecurityMode {
    PS3LIB_SECURITY_SET    = 0,    ///< 首次设置加密参数，开启控制卡加密
    PS3LIB_SECURITY_MODIFY = 1,    ///< 修改控制卡加密参数
} Ps3LibSecurityMode_e;

/**
 * @brief    控制卡加密级别
*/
typedef enum Ps3LibPassLevel {
    PS3LIB_PASS_LEVEL_LOW  = 0, ///< 简单密码
    PS3LIB_PASS_LEVEL_HIGH = 1, ///< 复杂密码
    PS3LIB_PASS_LEVEL_NR   = 2, ///< 预留级别
} Ps3LibPassLevel_e;

/**
 * @brief    控制卡加密信息结构体
*/
typedef struct Ps3LibCtrlSecurityInfo{
    U8                  key[PS3LIB_MAX_SECURITY_KEY_LEN];                      ///< securityKey秘钥
    U8                  oldKey[PS3LIB_MAX_SECURITY_PASSPHRASE_LEN];            ///< oldsecuritykey旧秘钥
    U8                  passPhrase[PS3LIB_SECURITY_PASSPHRASE_ENCODE_LEN];     ///< passphrase密码
    U8                  keyId[PS3LIB_MAX_SECURITY_KEYID_LEN + 1];                  ///< keyId 加密ID
    U8                  pad[3];
    Ps3LibPassphraseMode_e passPhraseMode; ///< 控制卡口令模式
    Ps3LibSecurityMode_e   securityMode;   ///< 控制卡加密模式
    Ps3LibPassLevel_e      passLevel;      ///< 控制卡加密级别
} Ps3LibCtrlSecurityInfo_t;

/**
 * @brief   ps3lib库运行模式
 */
typedef enum Ps3LibRunningMode {
    PS3LIB_RUNNING_UNKNOWN_MODE = 0x00,            ///< ps3lib库运行状态未知
    PS3LIB_RUNNING_IOCTL_MODE   = 0x01,            ///< ps3lib库运行于带内模式
    PS3LIB_RUNNING_OOB_MODE     = 0x02,            ///< ps3lib库运行于OOB模式
    PS3LIB_RUNNING_NR           = 0xFF,
} Ps3LibRunningMode_e;

/**
 * @brief   带外信息
 */
typedef struct Ps3LibOobCl{        ///< I2C
    U8 channel;                ///< channel where controller is connected
    U8 deviceAddress;          ///< device address where controller is connected
    U8 reDiscover       :1;    ///< 保留字段
    U8 funcType         :3;    ///< funcType类型(0:单func,1:双func)
    U8 reserved         :4;    ///< 保留字段
    U8 libType;                ///< lib库类型
}Ps3LibOobCl_t;

/**
 * @brief   带外信息
 */
typedef struct Ps3LibOobMctpCtrl_t{    ///< MCTP BDF手动填充形式
    U8 device;                ///< channel where controller is connected
    U8 bus;                   ///< device address where controller is connected
    U8 reDiscover       :1;   ///< 保留字段
    U8 func             :3;   ///< funcType类型(0:单func,1:双func)
    U8 reserved         :4;   ///< 保留字段
    U8 libType;               ///< lib库类型
} Ps3LibOobMctpCtrl_t;

/**
 * @brief   带外信息
 */
typedef struct Ps3LibOobMctpCtrlEID{    ///< MCTP EID主动上报形式
    U8 EID;                   ///< EID
    U8 reserved1;             ///< 保留字段1
    U8 reDiscover       :1;   ///< 保留字段
    U8 func             :3;   ///< funcType类型(0:单func,1:双func)
    U8 reserved2        :4;   ///< 保留字段
    U8 libType;               ///< lib库类型
} Ps3LibOobMctpCtrlEID_t;

/**
 * @brief   控制卡清单
 */
typedef struct Ps3LibCtrlList{
    U16     count;
    U8      flags;      ///< values as defined above; set to PS3LIB_RUNNING_OOB_MODE for OOB
    U8      reserved;
    union
    {
        U32     ctrlId[PS3LIB_MAX_CTRL_COUNT];                   ///< OS下使用
        Ps3LibOobCl_t oobClist[PS3LIB_MAX_CTRL_COUNT];           ///< OOB模式下I2C或I2C_MCTP使用
        Ps3LibOobMctpCtrl_t oobClistMctp[PS3LIB_MAX_CTRL_COUNT]; ///< OOB模式下MCTP_PCIE, BDF手动填充下发使用
        Ps3LibOobMctpCtrlEID_t oobClistMctpEID[PS3LIB_MAX_CTRL_COUNT]; ///< OOB模式下MCTP_PCIE, EID主动上报
    };
}Ps3LibCtrlList_t;

/**
 * @brief   Endpoint List
 */
typedef enum Ps3LibMpciDisCFmt
{
    PS3LIB_MPCI_DISCFMT_PCI_CONFIG_SPACE = 0x00,
    PS3LIB_MPCI_DISCFMT_NONE = 0xFF,
} Ps3LibMpciDisCFmt_e;

/**
 * @brief   MPCI EID Info
 * @warning 除保留字段,禁止修改内存排布
 * @note    如有需要, 可使用union
 * @note    与驱动库交互结构体, 1字节对齐, 禁止修改
 */
typedef struct Ps3LibMpciEIDInfo{
    U8                  EID;
    U8                  status;    ///< 用于控制卡状态标记, 当前不支持
    U16                 pciAddr;   ///< b/d/f
    Ps3LibMpciDisCFmt_e format;    ///< union指代
    U8                  reserved[2];
    union {
        struct {
            U16 deviceID;
            U16 vendorID;
            U16 subDeviceID;
            U16 subVendorID;
            U32 reserved2;
        } pciConfigSpace;
    };
}__attribute__((packed))Ps3LibMpciEIDInfo_t; ///< 1字节对齐, 22字节

typedef struct Ps3LibMpciEIDInfoList{
    U16                  count;
    U16                  reserved;
    Ps3LibMpciEIDInfo_t  EIDInfo[PS3LIB_MAX_CTRL_COUNT];
}__attribute__((packed))Ps3LibMpciEIDInfoList_s; ///< 16+16+22*128

/**
 * @brief   巡读状态枚举
 */
typedef enum Ps3LibPrStatusItems {
    PS3LIB_PR_STATE_ACTIVE = 0x01,            ///< 巡读激活
    PS3LIB_PR_STATE_PAUSED  = 0x02,           ///< 巡读终止
    PS3LIB_PR_STATE_ABORTED  = 0x04,          ///< 巡读暂停
    PS3LIB_PR_STATE_INTERAL_PAUSED   = 0x08,  ///< 巡读暂停间隔
    PS3LIB_PR_STATE_NR,
} Ps3LibPrStatusItems_t;

/**
 * @brief   巡读状态
 */
typedef struct Ps3LibPRStatus {
    U32 numIteration;   ///< 巡读循环次数
    U8  state;          ///< 巡读状态
    U8  numPdDone;      ///< 废弃
    U8  numPdDoneMsb;   ///< 在当前周期中完成巡读的物理盘msb的数量
    U8  pad[1];
    U16 numPdDoneEx;    ///< 在当前周期中完成巡读的物理盘的数量
    U8  reserved[6];
} Ps3LibPRStatus_t;

typedef struct Ps3LibExcludeIdList {
    U8      excludeLdCount;     ///< 数组excludeLD的成员个数
    U8      pad;
    U16     excludedLD[PS3LIB_MAX_VD_PER_CTRL_RAID];  ///< 从巡读中排除的虚拟盘列表
} Ps3LibExcludeIdList_t;

/**
 * @brief   巡读特性
 */
typedef struct Ps3LibPRProperties{
    U8      opMode;             ///< 当前巡读模式
    U8      maxPD;              ///< 最大物理盘数量
    U8      options;            ///< options that define the behavior of scheduled PR(not support)
    U8      includessdsFlag;    ///< on|off|onlymixed
    U8      uncfgareasFlag;     ///< on|off
    U8      modeType;           ///< auto|manual(not support)
    U8      pad1[2];
    U32     nextExec;           ///< 下次执行时间
    U32     execFreq;           ///< 执行频率
    U32     clearFreq;          ///< 擦除频率(not support)
    U32     prNumIteration;     ///< 已完成的pr轮数
    Ps3LibExcludeIdList_t excludeVd; ///< not support
    U8      pad2[2];
} Ps3LibPRProperties_t;

/**
 * @brief  控制卡类型
 */
typedef enum Ps3LibCtrlType {
    PS3LIB_CTRL_TYPE_UNKNOWN   = 0,
    PS3LIB_CTRL_TYPE_RAID      = 0b0001, ///< 控制卡类型RAID
    PS3LIB_CTRL_TYPE_HBA       = 0b0010, ///< 控制卡类型HBA
    PS3LIB_CTRL_TYPE_SWITCH    = 0b0100, ///< 控制卡类型SWITCH
} Ps3LibCtrlType_e;

/**
 * @brief   一致性校验规划状态
 */
typedef struct ps3LibCCScheduleStatus {
    U32     numIteration;   ///< 一致性校验迭代完成数量
    U8      state;          ///< 一致性校验当前状态
    U8      numLdDone;      ///< 完成一致性校验的逻辑盘的数量
    U8      numLdDoneMsb;   ///< 完成一致性校验的最高有效为逻辑盘的数量
    U8      reserved[9];
} Ps3LibCCScheduleStatus_t;

/**
 * @brief   一致性校验MODE状态
 */
enum {
    PS3LIB_CTRL_CC_MODE_SEQ          = 0x01,        ///< 相继模式
    PS3LIB_CTRL_CC_MODE_CONCURRENT   = 0x02,        ///< 同时模式
    PS3LIB_CTRL_CC_MODE_DISABLED     = 0x00,        ///< 未使能
    PS3LIB_CTRL_CC_MODE_NR           = 0xFF,        ///< 无效模式
};

/**
 * @brief 巡读mode状态
 * 
 */
typedef enum Ps3LibPrModeStatus{
    PS3LIB_CTRL_PR_MODE_AUTO         = 0x01,        ///< 自动模式
    PS3LIB_CTRL_PR_MODE_MANUAL       = 0x02,        ///< 手动模式
    PS3LIB_CTRL_PR_MODE_DISABLED     = 0x00,        ///< 未使能
    PS3LIB_CTRL_PR_MODE_NR           = 0xFF,        ///< 无效模式
}Ps3LibPrModeStatus_e;

/**
 * @brief   警报是否开启信息枚举
 */
typedef enum Ps3LibCtrlAlarm {
    PS3LIB_CTRL_HW_ALARM_OFF      = 0,
    PS3LIB_CTRL_HW_ALARM_ON     = 1,
    PS3LIB_CTRL_HW_ALARM_NR,
}Ps3LibCtrlAlarm_e;

typedef enum Ps3LibCtrlOthersStatus {
    PS3LIB_CTRL_STATUS_YES      = 0,
    PS3LIB_CTRL_STATUS_NO       = 1,
    PS3LIB_CTRL_OTHERS_STATUS_NR,
}Ps3LibCtrlOthersStatus_e;

/**
 * @brief   控制卡模式枚举
 */
typedef enum Ps3LibCtrlMode {
    PS3LIB_CTRL_MODE_RAID      = 0,
    PS3LIB_CTRL_MODE_HBA       = 1,
    PS3LIB_CTRL_MODE_NR,
}Ps3LibCtrlMode_e;

/**
 * @brief   ehs能力是否支持
 */
typedef enum Ps3LibEhsTypeGet{
    PS3LIB_EHS_SUPPORT_NO      = 0,    ///< 不支持ehs
    PS3LIB_EHS_SUPPORT_YES     = 1,    ///< 支持
    PS3LIB_EHS_SUPPORT_UNKNOW  = 2,    ///< 未知
}Ps3LibEhsTypeGet_t;

/**
 * @brief   支持硬盘驱动类型枚举
 */
typedef enum Ps3LibCtrlSupportDrive {
    PS3LIB_CTRL_TYPE_DRIVE_SAS       = 0,
    PS3LIB_CTRL_TYPE_DRIVE_SATA      = 1,
    PS3LIB_CTRL_TYPE_DRIVE_NVME      = 2,
    PS3LIB_CTRL_TYPE_DRIVE_NR,
}Ps3LibCtrlSupportDrive_e;

/**
 * @brief   硬盘驱动是否允许信息枚举
 */
typedef enum Ps3LibCtrlHardDisk {
    PS3LIB_CTRL_HARD_DRIVE_ALLOWED    = 0,
    PS3LIB_CTRL_HARD_DRIVE_ALLOW      = 1,
    PS3LIB_CTRL_HARD_DRIVE_NR,
} Ps3LibCtrlHardDisk_e;

typedef enum Ps3LibCtrlCoercion {
    PS3LIB_CTRL_COERCION_NONE      = 0,
    PS3LIB_CTRL_COERCION_128M,
    PS3LIB_CTRL_COERCION_1G,
    PS3LIB_CTRL_COERCION_NR,
}Ps3LibCtrlCoercion_e;

/**
 * @brief   一致性校验规划属性
 */
typedef struct Ps3LibCCProperties {
    U8      opMode;             ///< 当前巡读模式 PS3LIB_CTRL_CC_MODE_NR
    U8      maxLds;             ///< 最大逻辑盘数量
    U8      reserved;
    U8      pad1[5];
    U64     curLdMap;           ///< 最后一次擦除后完成一致性校验的设备的bitmap
    U64     lastLdMap;          ///< 最后一次擦除前完成一致性校验的设备的bitmap
    U32     nextExec;           ///< 下次执行时间
    U32     execFreq;           ///< 执行频率
    U32     clearFreq;          ///< 擦除频率
    U8      pad2[4];
    Ps3LibExcludeIdList_t excludeVd;
    U8      pad3[6];
} Ps3LibCCProperties_t;

/**
 * @brief   一致性校验下发mode
 */
typedef struct Ps3LibCcMode {
    U32 mode;                ///< cc 执行模式 PS3LIB_CTRL_CC_MODE_SEQ
    U8  isHasStartTime;      ///< 用户是否输入了starttime，输入starttime为1，未输入starttime为0
    U8  pad[PS3LIB_CTRL_CC_PR_PAD_LEN];
} Ps3LibCcMode_t;

/**
 * @brief   控制卡属性信息结构体
 */
typedef struct Ps3LibCtrlPropInfo {
    U32 rebuildRate;         ///< 重建速率
    U32 prRate;              ///< 巡读速率
    U32 bgiRate;             ///< bgi速率
    U32 ccRate;              ///< 一致性校验速率
    
    U32 migrationRate;       ///< 迁移速率
    U32 maxDrive;            ///< Max Drives to Spin Up at One Time. 注:只支持查询,不支持设置.
    U32 delayAmong;          ///< Delay Among Spin Up Groups (sec)
    U32 preFailPollInt;      ///< 预失败轮询间隔 单位:小时 +

    U32 eccBucketSize;         ///< ECC Bucket Size
    U32 eccBucketLeakRate;         ///< ECC Bucket Leak Rate (in minutes)
    U32 flushTime;           ///< 缓存刷写时间 单位:秒
    U32 cacheInterval;

    U32 pad1[12];            ///< 保留字段

    U8  autoRebuild;         ///< 自动重建开关
    U8  coercionMode;        ///< Drive Coercion Mode, 驱动模式drivemode
    U8  mainPdFailHistory;   ///< 维护磁盘故障信息开关
    U8  batteryWarning;      ///< 电池告警

    U8  spinDownHS;          ///< 仅支持查询, 不支持批量属性设置,可通过ps3libCtrlSetPsState进行设置
    U8  spinupDriveCount;    ///< Max Drives to Spin Up at One Time
    U16 spinDownTime;

    U8  useGSpareForEmergency;
    U8  useReadyForEmergency;
    U8  spinupDelay;
    U8  abortCCOnError;      ///< 禁止检查不一致

    U8  enableJbod;          ///< 使能JBOD
    U8  enablSnapshot;       ///< 使能备份
    U8  alarmState;          ///< 蜂鸣器状态
    U8  exposeEnclosure;     ///< 背板可见

    U8  moveBack;            ///< 回拷状态
    U8  ncq;                 ///< 不支持设置, 仅支持查询.若填充，自动忽略.
    U8  allowBoot;           ///< Allow Boot with Preserved Cache
    U8  prCorrectUnconfiguredAreas;

    U8  spinDownReady;       ///< 仅支持查询, 不支持批量属性设置,可通过ps3libCtrlSetPsState进行设置
    U8  deviceReportingOrder; ///< 上报盘顺序，0-VD后在JBOD之后，1-JBOD在VD之前
    U8  pad2[10];            ///< 保留字段
} Ps3LibCtrlPropInfo_t;

/**
 * @brief   ctrl允许的操作
 */
typedef struct Ps3LibCtrlAllowedOps{
    U32 foreignConfiguration                : 1;    ///< 有外盘可以导入时为true
    U32 foreignImport                       : 1;    ///< 是否支持外盘导入
    U32 foreignClear                        : 1;    ///< 是否支持清除
    U32 advancedConfiguration               : 1;    ///< 是否允许创建vd
    U32 supportR0                           : 1;    ///< 是否支持创建R0
    U32 supportR1                           : 1;    ///< 是否支持创建R1
    U32 supportR1E                          : 1;    ///< 是否支持创建R1E
    U32 supportR5                           : 1;    ///< 是否支持创建R5
    U32 supportR6                           : 1;    ///< 是否支持创建R6
    U32 supportR00                          : 1;    ///< 是否支持创建R00
    U32 supportR10                          : 1;    ///< 是否支持创建R10
    U32 supportR50                          : 1;    ///< 是否支持创建R50
    U32 supportR60                          : 1;    ///< 是否支持创建r60
    U32 profileManagement                   : 1;    ///< 能够show出Profile时，即为true
    U32 enableScsiUnmap                     : 1;    ///< 能否启动scsi unmap,启用unmap ； ctrl级别
    U32 disableScsiUnmap                    : 1;    ///< 能否关闭scsi unmap,禁用unmap ； ctrl级别
    U32 downloadSnapshotLog                 : 1;    ///< snapshot状态是on时显示
    U32 generateSnapshot                    : 1;    ///< snapshot状态是on时显示
    U32 clearSnapshot                       : 1;    ///< snapshot状态是on时显示
    U32 enableDriveSecurity                 : 1;    ///< 能否启动加密
    U32 disableDriveSecurity                : 1;    ///< 能否禁用加密
    U32 changeDriveSecurity                 : 1;    ///< 能否修改加密
    U32 manageSASStorageLinkSpeed           : 1;    ///< 存在 phys in a SAS port
    U32 managePCIeStorageInterface          : 1;    ///< 有NVME盘时为true
    U32 startPatrolRead                     : 1;    ///< 当pr的mode为manual或automatic时
    U32 stopPatrolRead                      : 1;    ///< pr开始之后
    U32 suspendPatrolRead                   : 1;    ///< 暂停
    U32 resumePatrolRead                    : 1;    ///< 恢复
    U32 supportAutoRaid0                    : 1;    ///< 是否有可设为Raid0的ready盘
    U32 supportDiscardPreservedCache        : 1;    ///< 是否支持删除脏数据
    U32 supportDownloadTTYLog               : 1;    ///< 是否支持下载termlog
    U32 enableDriveGuard                    : 1;    ///< 是否支持启动硬盘预失败
    U32 disableDriveGuard                   : 1;    ///< 是否支持关闭硬盘预失败
    U32 autoconfigManagement                : 1;    ///< 是否支持百分比
    U32 rebuildRate                         : 1;    ///< 是否支持百分比
    U32 prRate                              : 1;    ///< 是否支持百分比
    U32 bgiRate                             : 1;    ///< 是否支持百分比
    U32 ccRate                              : 1;    ///< 是否支持百分比
    U32 migrateRate                         : 1;    ///< 是否支持百分比
    U32 asoShow                             : 1;    ///< 是否支持百分比
    U32 dgAdvancedConfiguration             : 1;    ///< 是否允许DG添加vd
    U32 supportDgAddR0                      : 1;    ///< 是否支持DG创建R0
    U32 supportDgAddR1                      : 1;    ///< 是否支持DG创建R1
    U32 supportDgAddR1E                     : 1;    ///< 是否支持DG创建R1E
    U32 supportDgAddR5                      : 1;    ///< 是否支持DG创建R5
    U32 supportDgAddR6                      : 1;    ///< 是否支持DG创建R6
    U32 supportDgAddR00                     : 1;    ///< 是否支持DG创建R00
    U32 supportDgAddR10                     : 1;    ///< 是否支持DG创建R10
    U32 supportDgAddR50                     : 1;    ///< 是否支持DG创建R50
    U32 supportDgAddR60                     : 1;    ///< 是否支持DG创建R60
    U32 pad                                 : 14;   ///< 字段对齐
    ///< 以下字段为上面各个字段的有效性字段，只有对应的有效性字段为true时，才能根据该字段的值判断操作是否支持
    U32 foreignConfigurationValid                : 1;    ///< 有外盘可以导入时为true
    U32 foreignImportValid                       : 1;    ///< 是否支持外盘导入
    U32 foreignClearValid                        : 1;    ///< 是否支持清除
    U32 advancedConfigurationValid               : 1;    ///< 是否允许创建vd
    U32 supportR0Valid                           : 1;    ///< 是否支持创建R0
    U32 supportR1Valid                           : 1;    ///< 是否支持创建R1
    U32 supportR1EValid                          : 1;    ///< 是否支持创建R1E
    U32 supportR5Valid                           : 1;    ///< 是否支持创建R5
    U32 supportR6Valid                           : 1;    ///< 是否支持创建R6
    U32 supportR00Valid                          : 1;    ///< 是否支持创建R00
    U32 supportR10Valid                          : 1;    ///< 是否支持创建R10
    U32 supportR50Valid                          : 1;    ///< 是否支持创建R50
    U32 supportR60Valid                          : 1;    ///< 是否支持创建r60
    U32 profileManagementValid                   : 1;    ///< 能够show出Profile时，即为true
    U32 enableScsiUnmapValid                     : 1;    ///< 能否启动scsi unmap,启用unmap ； ctrl级别
    U32 disableScsiUnmapValid                    : 1;    ///< 能否关闭scsi unmap,禁用unmap ； ctrl级别
    U32 downloadSnapshotLogValid                 : 1;    ///< snapshot状态是on时显示
    U32 generateSnapshotValid                    : 1;    ///< snapshot状态是on时显示
    U32 clearSnapshotValid                       : 1;    ///< snapshot状态是on时显示
    U32 enableDriveSecurityValid                 : 1;    ///< 能否启动加密
    U32 disableDriveSecurityValid                : 1;    ///< 能否禁用加密
    U32 changeDriveSecurityValid                 : 1;    ///< 能否修改加密
    U32 manageSASStorageLinkSpeedValid           : 1;    ///< 存在 phys in a SAS port
    U32 managePCIeStorageInterfaceValid          : 1;    ///< 有NVME盘时为true
    U32 startPatrolReadValid                     : 1;    ///< 当pr的mode为manual或automatic时
    U32 stopPatrolReadValid                      : 1;    ///< pr开始之后
    U32 suspendPatrolReadValid                   : 1;    ///< 暂停
    U32 resumePatrolReadValid                    : 1;    ///< 恢复
    U32 supportAutoRaid0Valid                    : 1;    ///< 是否有可设为Raid0的ready盘
    U32 supportDiscardPreservedCacheValid        : 1;    ///< 是否支持删除脏数据
    U32 supportDownloadTTYLogValid               : 1;    ///< 是否支持下载termlog
    U32 enableDriveGuardValid                    : 1;    ///< 是否支持启动硬盘预失败
    U32 disableDriveGuardValid                   : 1;    ///< 是否支持关闭硬盘预失败
    U32 autoconfigManagementValid                : 1;    ///< 是否支持autoConfig
    U32 rebuildRateValid                         : 1;    ///< 是否支持百分比
    U32 prRateValid                              : 1;    ///< 是否支持百分比
    U32 bgiRateValid                             : 1;    ///< 是否支持百分比
    U32 ccRateValid                              : 1;    ///< 是否支持百分比
    U32 migrateRateValid                         : 1;    ///< 是否支持百分比
    U32 asoShowValid                             : 1;    ///< 是否支持百分比
    U32 dgAdvancedConfigurationValid             : 1;    ///< 是否允许DG添加vd
    U32 supportDgAddR0Valid                      : 1;    ///< 是否支持DG中创建R0
    U32 supportDgAddR1Valid                      : 1;    ///< 是否支持DG创建R1
    U32 supportDgAddR1EValid                     : 1;    ///< 是否支持DG创建R1E
    U32 supportDgAddR5Valid                      : 1;    ///< 是否支持DG创建R5
    U32 supportDgAddR6Valid                      : 1;    ///< 是否支持DG创建R6
    U32 supportDgAddR00Valid                     : 1;    ///< 是否支持DG创建R00
    U32 supportDgAddR10Valid                     : 1;    ///< 是否支持DG创建R10
    U32 supportDgAddR50Valid                     : 1;    ///< 是否支持DG创建R50
    U32 supportDgAddR60Valid                     : 1;    ///< 是否支持DG创建R60
    U32 padValid                                 : 14;   ///< 字段对齐
} Ps3LibCtrlAllowedOps_t;

/**
 * @brief   厂商分区字段长度
 */
enum {
    PS3LIB_MFG_PART_NUMBER_LEN   = 16, ///< 序列号长度
    PS3LIB_MFG_WWN_LEN           = 16, ///< 卡标识符长度
    PS3LIB_MFG_REVISION_LEN      = 4,  ///< 反工版本号长度
    PS3LIB_MFG_OEM_STR_LEN       = 64, ///< oem字段长度
};

/**
 * @brief  从控制器获取的信息
 * @note   补充ctrl show相关结构体
 */
typedef struct Ps3LibCtrlGetInfo{
    U32  mgfDate;                                  ///< 生产制造日期
    U8   pad[4];
    S8   fwVersion[PS3LIB_FW_VERSION_LEN];         ///< 固件包版本(注意:可能不包含终止符'\0')
    S8   packageBuild[PS3LIB_FW_VERSION_LEN];      ///< 固件包编译版本(注意:可能不包含终止符'\0')
    S8   buildDate[PS3LIB_FW_VERSION_LEN];         ///< 固件包编译时间(注意:可能不包含终止符'\0')
    U64  controllerTime;                           ///< 当前控制器时间戳
    S8   biosVersion[PS3LIB_BIOS_VERSION_LEN];     ///< BIOS版本(注意:可能不包含终止符'\0')
    S8   nvVersion[PS3LIB_FW_VERSION_LEN];         ///< nvdata版本(注意:可能不包含终止符'\0')
    U32  domainID;                                 ///< 保留字段
    U8   operationMode;                            ///< MglCtrlOperationMode_e 盘的接入模式
    U8   powerMode;                                ///< Ps3LibCtrlInPowerMode_e 芯片当前运行的功耗模式
    U8   pad2[2];                                  ///< 保留字段
    S8   productName[PS3LIB_PRODUCT_NAME_LEN];     ///< 产品名称(注意:可能不包含终止符'\0')
    U8   serialNo[PS3LIB_MFG_PART_NUMBER_LEN];     ///< 序列号(注意:可能不包含终止符'\0')
    U64  sasAddr[PS3LIB_CTRL_SAS_COUNT];           ///< sas地址
    U32  sasActiveMap : PS3LIB_CTRL_SAS_COUNT;     ///< sas接口是否active的bitmap
                                                   ///< ((sasActiveMap >> i) & 1)表示第i个sas接口是否active
    U32  pad3 : (32 - PS3LIB_CTRL_SAS_COUNT);
    U32  reworkdate;                               ///< 反工时间
    U8   reworkNo[PS3LIB_MFG_REVISION_LEN];        ///< 反工版本号(注意:可能不包含终止符'\0')
    U32  controllerId;                             ///< 控制器id, oob模式下不支持获取
    S8   boardName[PS3LIB_PRODUCT_NAME_LEN];       ///< 板卡名称(注意:可能不包含终止符'\0')
    S8   boardAssembly[PS3LIB_PRODUCT_NAME_LEN];   ///< 板卡制造商(注意:可能不包含终止符'\0')
    S8   boardTraceNumber[PS3LIB_PRODUCT_NAME_LEN-16-16];///< 板卡追踪码(注意:可能不包含终止符'\0')
    S8   brandName[16];                            ///< 品牌名称 (注意:可能不包含终止符'\0')
    S8   chipName[16];                             ///< chipName (注意:可能不包含终止符'\0')
    U16  vendorId;                                 ///< PCI设备厂商ID
    U16  deviceId;                                 ///< PCI设备设备ID
    U16  subVendorId;                              ///< PCI设备子系统厂商ID
    U16  subDeviceId;                              ///< PCI设备子系统设备ID
}Ps3LibCtrlGetInfo_t;

/**
 * @brief   电池状态枚举
 */
typedef enum Ps3LibBbuBatStatus {
    PS3LIB_BBU_BATTERY_FAILED       = 0,   ///< 电池失效
    PS3LIB_BBU_BATTERY_LOWCAPACITY  = 1,   ///< 电量不足
    PS3LIB_BBU_BATTERY_DEGRADE      = 2,   ///< 电量降级状态
    PS3LIB_BBU_BATTERY_OPTIMAL      = 3,   ///< 最优状态，电量充足
    PS3LIB_BBU_BATTERY_LEARNING     = 4,   ///< 校准状态
    PS3LIB_BBU_BATTERY_ABSENT       = 5,   ///< bbu不在位
    PS3LIB_BBU_BATTERY_NR
}Ps3LibBbuBatStatus_e;

/**
 * @brief   ctrl health 信息
 */
typedef struct Ps3LibCtrlOverview {
    U8                  portCnt;        ///< port 个数
    U8                  pad[3];
    Ps3LibBbuBatStatus_e bbuState;      ///< bbu状态
    U8                  prMode;         ///< Scheduled Patrol Read
    U8                  psReadyStatus;
    U8                  psHSStatus;
    U8                  ehs;            ///< Emergency Spare Drive
    U8                  healthState;
    U8                  pad1[1];
    U16                 pdOptCnt;       ///< 适配BMC pd optimal 个数
    U8                  reserved[4];    
    U32                 pdCnt;          ///< pd个数
    U32                 dgCnt;          ///< dg个数
    U32                 vdCnt;          ///< vd个数
    U32                 pdNotOptCnt;    ///< pd not optimal个数
    U32                 vdNotOptCnt;    ///< dg not optimal个数
    U32                 dgNotOptCnt;    ///< vd not optimal个数
    U32                 vdOffCnt;       ///< vd offline状态
    U32                 vdDegCnt;       ///< vd 降级个数
    U32                 vdParDegCnt;    ///< vd部分降级个数
    U32                 asoCnts;        ///< 高级功能个数
    S8                  model[PS3LIB_PRODUCT_NAME_LEN];     ///< 产品名称(注意:可能不包含终止符'\0')
    Ps3LibPdPosition_t  pdNotOpt[PS3LIB_MAX_PD_NUM];   ///< pd not optimal
    U8                  pad2[4];
    U32                 pdPreFailCnt;      ///< pd预测失败个数
    U32                 pdFailCnt;         ///< pd失败个数
    U32                 pdReadyCnt;        ///< pd ready状态个数
    U32                 pdUbadCnt;         ///< pd ubad状态个数
    U64                 totalCapacitySize; ///< pd总容量
    U64                 usedCapacitySize;  ///< 组成dg的pd的总容量
} Ps3LibCtrlOverview_s;

/**
 * @brief  V100版本与片内交互使用
 */
typedef struct Ps3LibProfileParam {
    U16 profileId;             ///< 配置文件ID
    U8  personalityMode;       ///< 配置文件的有效模式
    U8  reserved0;
    U16 maxPdCount;            ///< 最大物理盘数量
    U16 maxVdCount;            ///< 最大虚拟盘数量
    U16 maxNvmeCount;          ///< 最大NVME数量
    U32   isDflt:1;
    U32   isCurr:1;
    U32   isOptm:1;
    U32   isPend:1;
    U32   isNvmeOnlyProfile:1; ///< 配置文件是否只支持NVME设备
    U32   isWbSup:1;
    U32   isCmp:1;
    U32   reserved1:25;
}__attribute__((packed))Ps3LibProfileParam_t;

/**
 * @brief   开关特性2
 */
typedef struct Ps3LibOnOffProperties2 {
    U16 enableLargeIOSupport                : 1;    ///< 禁用大型ios支持
    U16 enableLargeQD                       : 1;    ///< 大型QD被禁用
    U16 SESVPDAssociationTypeInMultiPathCfg : 1;    ///< 仅适用于ses设备
    U16 activeFWImageSelect                 : 1;    ///< 控制器从位于主闪存区域的FW影像启动
    U16 enableSnapShot                      : 1;    ///< 快照功能已启用
    U16 disableScsiUnmap                    : 1;    ///< 使能unmap
    U16 reserved                            : 10;
} Ps3LibOnOffProperties2_t;

/**
 * @brief   开关特性
 */
typedef struct Ps3LibOnOffProperties {
    U32     moveBackDisabled            : 1;    ///< 禁用回写
    U32     SMARTerEnabled              : 1;    ///< 使能SMARTer
    U32     prCorrectUnconfiguredAreas  : 1;    ///< 巡读期间错误检查
    U32     useFdeOnly                  : 1;    ///< 允许基于FDE驱动器的加密
    U32     disableNCQ                  : 1;    ///< 去使能NCQ
    U32     SSDSMARTerEnabled           : 1;    ///< 将SMART拷贝到SSD
    U32     SSDPatrolReadEnabled        : 1;    ///< SSD巡读使能
    U32     enableSpinDownUnconfigured  : 1;    ///< 降速未配置的驱动器
    U32     autoEnhancedImport          : 1;    ///< 自动导入外部配置
    U32     enableSecretKeyControl      : 1;    ///< 允许锁定秘钥更新
    U32     disableOnlineCtrlReset      : 1;    ///< 允许在线控制器重置
    U32     allowBootWithPinnedCache    : 1;    ///< 允许控制器使用固定缓存启动
    U32     disableSpinDownHS           : 1;    ///< 不降速热备盘
    U32     enableJBOD                  : 1;    ///< 使能JBOD
    U32     disableCacheBypass          : 1;    ///< 禁用缓存绕过性能改进功能
    U32     useDiskActivityForLocate    : 1;    ///< 驱动切换激活LED
    U32     enablePI                    : 1;    ///< 使能PI
    U32     preventPIImport             : 1;    ///< 阻止SCSI DIF保护的逻辑硬盘
    U32     useGlobalSparesForEmergency : 1;    ///< 紧急情况下使用配件
    U32     useUnconfGoodForEmergency   : 1;    ///< 紧急情况下使用未经处理的驱动器
    U32     useEmergencySparesforSMARTer: 1;    ///< 使用SMARTer的应急备件
    U32     forceSGPIOForQuadOnly       : 1;    ///< 仅对四个驱动器强制每个端口的SGPIO状态
    U32     enableConfigAutoBalance     : 1;    ///< 使能配置自动平衡
    U32     failPDOnSMARTErr            : 1;    ///< 使达到智能故障阈值的pd失败
    U32     enableWriteVerifyDuringCacheFlush  : 1; ///< FW在缓存刷新操作期间使用写验证
    U32     disableImmediateIO          : 1;    ///< 禁止旧版IO
    U32     disableT10RebuildAssist     : 1;    ///< 禁用T10重建
    U32     ignore64ldRestriction       : 1;    ///< 使用新的ddf格式支持超过64LD
    U32     enableSwZone                : 1;    ///< 启用软件域
    U32     limitMaxRateSATA3G          : 1;    ///< 直连的协商链路速率
    U32     driveActivityLED            : 1;    ///< 显示所用驱动器
    U32     disableSESMonitoring        : 1;    ///< 禁用控制器上的ses监视

    union {
        U8      autoSnapVDSpace;    ///< 快照中的快照元数据和用户数据的快照存储库
        U8      viewSpace;          ///< 不推荐使用的快照可写视图容量
        Ps3LibOnOffProperties2_t CtrlInfoOnOff;
    };
    U8 pad[2];
} Ps3LibOnOffProperties_t;

/**
 * @brief   请求下载的termlog日志占最大容量的百分比
 */
typedef enum Ps3LibCtrlGetTermlogCapMaxQtr {
    PS3LIB_CTRL_GET_TERMLOG_QTR_ONE       = 1,  ///< 最大容量的1/4
    PS3LIB_CTRL_GET_TERMLOG_QTR_TWO       = 2,  ///< 最大容量的2/4,即1/2
    PS3LIB_CTRL_GET_TERMLOG_QTR_THREE     = 3,  ///< 最大容量的3/4
    PS3LIB_CTRL_GET_TERMLOG_QTR_FOUR      = 4,  ///< 最大容量的4/4，即最大容量
    PS3LIB_CTRL_GET_TERMLOG_QTR_NR,
}Ps3LibCtrlGetTermlogCapMaxQtr_e;

/*
 * brief termlog get请求结构体
 */
typedef struct Ps3LibCtrlTermLogReq {
    U8  beginFlag;   ///< 日志开始标志
    U8  capMax;   ///< 请求下载的termlog占最大容量的百分比 枚举Ps3LibCtrlGetTermlogCapMaxQtr; 0和全F，片内按照默认值
    U8  pad[2];
    S32 sessionId;   ///< 会话ID
    U32 dmaLength;   ///< 申请大小
} Ps3LibCtrlTermLogReq_t;

/*
 * brief termlog信息结构体
 */
typedef struct Ps3LibCtrlTermLogInfo {
    U32 magic;                               ///< 魔数
    U8  logEndFlag;                          ///< 日志结束标志
    U8  reserved;                            ///< 保证内存对齐
    U16 filelength;                          ///< 日志文件大小
    S32 sessionId;                           ///< 会话ID
    S8  data[0];                             ///< 日志文件内容
} Ps3LibCtrlTermLogInfo_t;

typedef struct Ps3LibSupportDriveInfo {
    U8  suppDriveNum;             ///< 支持驱动数量
    U8  suppDriveList[5];         ///< 支持驱动列表
} Ps3LibSupportDriveInfo_s;

typedef struct Ps3LibRaidTypeInfo {
    U8  suppRaidTypeNum;          ///< 支持raid类型数量
    U8  suppRaidTypeList[9];      ///< 支持raid类型列表
} Ps3LibRaidTypeInfo_s;

/*
 * brief 容量信息结构体
 */
typedef struct Ps3LibCtrlCapability {
    Ps3LibSupportDriveInfo_s  supportDrives;
    Ps3LibRaidTypeInfo_s  levelSupport;
    U8  enableJbod;            ///< 使能JBOD
    U8  mixEnclosure;          ///< 最小背板数量
    U8  mixSasSataVd;          ///< 最小sas sata虚拟盘数量
    U8  mixSasSataNvmeVd;      ///< 最小sas sata vnme虚拟盘数量
    U8  mixSsdVd;              ///< 最小固态虚拟盘数量
    U8  sasDisable;            ///< 去使能sas
    U8  pad[2];
    U32 maxArmsVd;             ///< 最大arms虚拟盘数量
    U32 maxSpans;              ///< 最大spans
    U32 maxArrays;             ///< 最多配置
    U32 maxVdPerarray;         ///< 最大虚拟盘阵列
    U32 maxNumber;             ///< 最大数量
    U32 maxParallel;           ///< 最大平行数量
    U32 maxSgeCount;           ///< 最大SGE数量
    U32 pad3;
    U32 maxStrips;             ///< 最大方式
    U32 maxConfig;             ///< 最大配置
    U32 maxTransport;          ///< 最大变化量
    U8  enableSnapshot;        ///< 使能备份
    U8  enableScsi;            ///< 使能SCSI
    U8  fdeDrive;              ///< FDE驱动
    U8  pad2;
    U32 minStripsize;          ///< 最小条带大小
    U32 maxStripSize;          ///< 最大条带大小
    U32 maxChainedEnclNum;     ///< 最大通道背板数量
    U32 maxChainedEnclDep;     ///< 最大通道背板深度
    U32 maxDataSize;           ///< 最大数据容量
} Ps3LibCtrlCapability_t;

/*
* 硬件配置信息结构体
*/
typedef struct Ps3LibCtrlHwCfg {
    U8  batteryFRU[PS3LIB_MFG_DATA_LEN];   ///< FRU电池状态(注意:可能不包含终止符'\0')
    U8  pad[3];
    U32 frontPort;        ///< not support
    U32 backPort;         ///< not support
    U8  bbu;              ///< bbu是否在位
    U8  alarm;            ///< 告警
    U8  serail;           ///< serail
    U8  pad2;
    S8  chipRevision[PS3LIB_MFG_DATA_LEN]; ///< 芯片版本(注意:可能不包含终止符'\0')
    S8  pad3;
    U16 nvramsize;        ///< nvram大小
    U16 flashsize;        ///< 内存大小
    U16 boardMemory;      ///< 内存板
    U8  pad4[4];
    U32 cacheVault;       ///< bbu内存大小
    U8  tpm;              ///< not support
    U8  upgradekey;       ///< 升级秘钥
    U8  temSenROC;        ///< ship温度 是否在位
    U8  temSenController; ///< top温度 是否在位
    U8  upgradeCPLD;      ///< 是否支持CPLD升级
    U8  temSenBoard;      ///< 板卡入风口挡板温度sensor是否在位 
    S16 boardTemp;        ///< 板卡风口挡板温度
    U32 sizeCache;        ///< not support
    U32 sizeFw;           ///< FW大小
    S32 rocTemp;          ///< CHIP 温度
} Ps3LibCtrlHwCfg_t;

/**
 * @brief   固件镜像名字
 */
typedef struct Ps3LibCtrlGetImages {
    S8  pendingImages[PS3LIB_FW_VERSION_LEN]; ///< 镜像名称(注意:可能不包含终止符'\0')
} Ps3LibCtrlGetImages_t;

/**
 * @brief   电池状态枚举
 */
typedef enum Ps3LibCtrlStatus {
    PS3LIB_CTRL_STATUS_HEALTHY   = 0, ///< 健康
    PS3LIB_CTRL_STATUS_WARNING   = 1, ///< 告警
    PS3LIB_CTRL_STATUS_FAILED    = 2, ///< 失败
    PS3LIB_CTRL_STATUS_NR,
} Ps3LibCtrlStatus_e;

/**
 * @brief   控制卡状态信息
 */
typedef struct Ps3LibCtrlGetStatus {
    Ps3LibCtrlStatus_e   ctrlStatus;
    U16  correctableErrors[PS3LIB_ECC_TYPE];    ///< 废弃
    U16  uncorrectableErrors[PS3LIB_ECC_TYPE];  ///< 废弃
    U16  eccBucketCount[PS3LIB_ECC_TYPE];       ///< 废弃
    U8   offlineVdCachePreserved;               ///< offline状态的vd是否存在保留的cache
    U8   bbuStatus;                             ///< bbu状态
    U8   batteryState;                          ///< 电池状态
    U8   downloadProgress;                      ///< 硬盘升级进行中
    U8   suppPdFwDownload;                      ///< 控制卡允许进行硬盘升级
    U8   lockKey;                               ///< not support
    U8   faillockKey;                           ///< not support
    U8   lockKeyBootup;                         ///< not support
    U8   lockkeyNBackup;                        ///< not support
    U8   biosNDetected;                         ///< not support
    U8   ctrlReboot;                            ///< not support
    U8   rollBackOperation;                     ///< not support
    U8   pfkInNvram;                            ///< not support
    U8   ctrlShutdown;                          ///< not support
    U8   ctrlBootSafeMode;                      ///< not support
    U8   sscPolicy;                             ///< not support
    U8   fans;                                  ///< not support
    U8   reserved[3];
    Ps3LibCtrlGetImages_t images;
    U32  correctableErrorsEx[PS3LIB_ECC_TYPE_EX];   ///<  3 cpu cache correctable errors
    U32  uncorrectableErrorsEx[PS3LIB_ECC_TYPE_EX]; ///<  3 cpu cache uncorrectable errors
    U32  eccBucketCountEx[PS3LIB_ECC_TYPE_EX];      ///<  3 cpu cache ecc bucket count
    U8   onboardFlashStatus;                        ///< 0:fault 1:pass 其他:N/A
    U8   pad[3];
} Ps3LibCtrlGetStatus_t;

/**
 * @brief  控制卡驱动信息
 */
typedef struct Ps3LibBootdriveInfo {
    U8        hasBootDrive        :1; ///< 是否有驱动
    U8        isPD                :1; ///< 是否为PD
    U8        reserved_9          :6;
    U8        enclID  ;               ///< 背板ID
    U16       slotID  ;               ///< slot ID
    U16       vdID    ;               ///< VD ID
    U8        pad[2];
}__attribute__((packed)) Ps3LibBootdriveInfo_t;

/**
 * @brief  从控制器获取的版本号信息
 * @note   字符串可能不包含终止符'\0'
 */
typedef struct Ps3LibCtrlGetVersion {
    S8  packageBuild[PS3LIB_FW_VERSION_LEN];   ///< 固件包编译版本
    S8  firmVersion[PS3LIB_FW_VERSION_LEN];    ///< 固件版本
    S8  cpldVersion[PS3LIB_FW_VERSION_LEN];    ///< cpld版本
    S8  biosVersion[PS3LIB_FW_VERSION_LEN];    ///< bios版本
    S8  hiiVersion[PS3LIB_FW_VERSION_LEN];     ///< hii版本
    S8  prebootVersion[PS3LIB_FW_VERSION_LEN]; ///< preBoot版本
    S8  nvVersion[PS3LIB_FW_VERSION_LEN];      ///< nv版本
    S8  bootVersion[PS3LIB_FW_VERSION_LEN];    ///< boot版本
    S8  sbrVersion[PS3LIB_FW_VERSION_LEN];     ///< sbr版本
    S8  splVersion[PS3LIB_FW_VERSION_LEN];     ///< spl版本
} Ps3LibCtrlGetVersion_t;

/*
 * brief 支持适配器操作结构体
 */
typedef struct Ps3LibCtrlGetAdapter {
    U32       suppRebuildRate     :1; ///< 控制卡告警
    U32       suppCCRate          :1; ///< 支持CC速率
    U32       suppBGIRate         :1; ///< 支持BGI速率
    U32       suppMigrateRate     :1; ///< 支持Migrate速率
    U32       suppPRRate          :1; ///< 支持巡读速率
    U32       alarmCtrl           :1; ///< 控制卡告警
    U32       bbu                 :1; ///< bbu
    U32       spanning            :1; ///< spanning
    U32       dedHotSpare         :1; ///< 无效热备
    U32       revHotSpare         :1; ///< 保留热备
    U32       foreignConfig       :1; ///< 允许外部配置导入
    U32       selfDiagnostic      :1; ///< 控制器支持自我诊断
    U32       allowMixedRedundancyOnArr:1; ///< 允许同一阵列具有不同VD
    U32       globalHotSpares     :1; ///< 全局热备盘
    U32       suppScsiPassThru    :1; ///< 支持pass thru
    U32       suppSmpPassThru     :1; ///< 允许SMP
    U32       suppStpPassThru     :1; ///< 允许STP
    U32       suppMoreThan8Phys   :1; ///< API仅支持八个phy
    U32       fwEventGmtTime      :1; ///< not support
    U32       suppEnhancedForeign :1; ///< 支持增强对外输入能力
    U32       suppEnclosureEnum   :1; ///< 支持背板
    U32       suppAllowedOps      :1; ///< 支持允许操作
    U32       abortCConErr        :1; ///< 终止错误并继续
    U32       suppMultipath       :1; ///< 支持多路径和负载平衡
    U32       suppOddEvenDrvCnt   :1; ///< 支持odd和even
    U32       suppSecurity        :1; ///< 支持安全模式
    U32       suppConfigPageModel :1; ///< 使用配置文件维护现有数据
    U32       suppOceWithoutAddDrvs:1; ///< 支持oce而不添加驱动器
    U32       suppEkm             :1; ///< 支持EKM模式
    U32       suppSnapshot        :1; ///< 支持snapshot
    U32       suppPfk             :1; ///< 支持PFK
    U32       suppPi              :1; ///< 支持PI
    U32       suppVdBbm           :1; ///< 支持BBM
    U32       ssdWritedisk        :1; ///< 支持读SSD硬盘
    U32       suspendResume       :1; ///< 支持恢复挂起
    U32       emergency           :1; ///< 应急备用驱动器
    U32       linkspeed           :1; ///< 接口速率
    U32       pfkchange           :1; ///< not support
    U32       suppPerfTuning      :1; ///< not support
    U32       patrolRead          :1; ///< 支持巡读
    U32       timeSchedule        :1; ///< 时间表
    U32       resetNow            :1; ///< 立即重启
    U32       emulatedDrives      :1; ///< not support
    U32       headlessMode        :1; ///< fw支持无头模式
    U32       dedhotspareLimited  :1; ///< 热备盘可以用于一个或多个阵列
    U32       pointTimeProgress   :1; ///< 支持时间进度查询
    U32       extendedVd          :1; ///< 支持扩展VD
    U32       suppUnevenSpan      :1; ///< 支持不均衡跨度
    U32       suppConfigAutoBalance:1; ///< 配置支持自动负载平衡
    U32       maintenanceMode     :1; ///< 支持维护模式
    U32       diagnosticMode      :1; ///< 诊断模式
    U32       extEnclosure        :1; ///< 支持外部存储模块
    U32       sesmonitoring       :1; ///< not support
    U32       securityjbod        :1; ///< not support
    U32       forceFlash          :1; ///< 强制擦除
    U32       clearCtrlEvent      :1; ///< 清楚控制卡事件
    U32       saveCtrlEvent       :1; ///< 保存控制卡事件
    U32       scsiUnmap           :1; ///< 虚拟盘scsi支持取消映射
    U32       disableImmediateIO  :1; ///< 禁止旧版IO
    U32       largeIoSupport      :1; ///< 支持大型IO
    U32       drvActLedSet        :1; ///< 驱动器支持点灯设置
    U32       flushWriteVerify    :1; ///< 支持刷新写验证
    U32       cpldUpdate          :1; ///< 支持cpld升级
    U32       forceTo512e         :1; ///< not_support
    U32       discardCacheDurVdDel:1; ///< 支持在vd删除过程中丢弃缓存
    U32       jbodWriteCache      :1; ///< not_support
    U32       largeQd             :1; ///< 支持大型QD
    U32       ctrlExtended        :1; ///< 支持控制卡信息扩展
    U32       aesEncryption       :1; ///< not_support
    U32       encryptionMfc       :1; ///< not_support
    U32       snapShot            :1; ///< not_support
    U32       forcePersonality    :1; ///< not_support
    U32       dualFwImage         :1; ///< 支持双fw镜像
    U32       psocUpdate          :1; ///< not_support
    U32       secureBoot          :1; ///< 支持安全启动
    U32       debugQueue          :1; ///< not_support
    U32       leastLatency        :1; ///< not_support
    U32       onDemandSnapshot    :1; ///< not_support
    U32       clearSnapshot       :1; ///< not_support
    U32       fwTriggerSnapshot   :1; ///< not_support
    U32       phyCurrentspeed     :1; ///< not_support
    U32       laneCurrentspeed    :1; ///< not_support
    U32       nvmeWidth           :1; ///< not_support
    U32       laneDeviceType      :1; ///< not_support
    U32       extendDrvPerfMoitor :1; ///< not_support
    U32       nvmeRepair          :1; ///< 支持NVME恢复
    U32       supportJbod         :1; ///< 支持JBOD
    U32       onlinePfkChange     :1; ///< not support
    U32       diagnosticResults   :1; ///< not support
    U32       suppVdPiType1       :1; ///< not support
    U32       suppVdPiType2       :1; ///< not support
    U32       suppVdPiType3       :1; ///< not support
    U32       suppLaneDevType     :1; ///< not support
    U32       extCtrlProp         :1; ///< not support
    U32       smartPollInterval   :1; ///< not support
    U32       pad_2               :1;
    U32       forceOnline         :1; ///< 强制上线
    U32       forceOffline        :1; ///< 强制下线
    U32       forceRebuild        :1; ///< 强制重执行
    U32       suppForceFailed     :1; ///< not support
    U32       suppForceGoodBad    :1; ///< not support
    U32       suppMissingReplace  :1; ///< not support
    U32       suppClear           :1; ///< 支持擦除
    U32       suppPdLocate        :1; ///< 支持物理盘区域设置
    U32       powerState          :1; ///< 支持电源状态
    U32       setPowerStateForCfg :1; ///< fw要求应用程序在更改配置之前设置电源状态
    U32       t10PowerState       :1; ///< T10能耗状态
    U32       temperature         :1; ///< 物理盘支持温度检测
    U32       ncq                 :1; ///< 启用sas驱动器中的本机命令队列
    U32       maxRateSata         :1; ///< 支持limitMaxRateSATA3G字段
    U32       degradedMedia       :1; ///< 支持PD降级检测
    U32       parallelFwUpdate    :1; ///< fw支持并行pd微码更新
    U32       driveCrypto         :1; ///< not support
    U32       ssdWearGauge        :1; ///< FW支持SSD驱动器的磨损计量
    U32       pad_3               :14;
    U32       readPolicy          :1; ///< 读策略可以被改变
    U32       writePolicy         :1; ///< 写策略可以被改变
    U32       ioPolicy            :1; ///< IO策略可以被改变
    U32       accessPolicy        :1; ///< 访问策略可以被改变
    U32       diskCachePolicy     :1; ///< 硬盘缓存策略可以被改变
    U32       migrate             :1; ///< DG迁移
    U32       suppVdLocate        :1; ///< 支持VD区域定位
    U32       supportCC           :1; ///< 支持一致性校验
    U32       allowCtrlEncryption :1; ///< 支持控制器加密
    U32       enableVdbbm         :1; ///< 使能虚拟盘BBM
    U32       supportFastPath     :1; ///< 支持fastpath
    U32       performanceMetrics  :1; ///< 支持基于虚拟机的IO性能指标
    U32       powerSavings        :1; ///< 支持已配置驱动器的节能
    U32       powerMaxWithCache   :1; ///< 支持带缓存写入的节能
    U32       breakmirror         :1; ///< 支持打断镜像的操作
    U32       sscWriteBack        :1; ///< ssc支持写缓存
    U32       sscAssociation      :1; ///< 支持关联SSC
    U32       vdHide              :1; ///< 支持隐藏虚拟盘
    U32       vdCachebypass       :1; ///< 支持VD缓存
    U32       vdDiscardCacheDurVdDel:1; ///< 在VD删除期间支持VD丢弃缓存
    U32       vdScsiUnmap         :1; ///< 支持虚拟盘scsi取消映射
    U32       pad_4               :11;
} Ps3LibCtrlGetAdapter_t;

/**
 * @brief 控制卡休眠状态
 */
typedef enum Ps3LibCtrlSleepState {
    PS3LIB_CTRL_STATE_UNKNOWN,
    PS3LIB_CTRL_STATE_WAKE,   ///< 唤醒
    PS3LIB_CTRL_STATE_SLEEP,  ///< 休眠
} Ps3LibCtrlSleepState_e;

/**
 * @brief 获取控制卡keyid结构体
 * 
 */
typedef struct Ps3LibCtrlSecurityId
{
    U8 keyId[PS3LIB_MAX_SECURITY_KEYID_LEN+1];    ///< 加密的keyid
} Ps3LibCtrlSecurityId_t;

/**
 * @brief   SnapShot特性结构体
 */
typedef struct  Ps3LibSnapShotProp {
    U8       snapShotState;             ///< snapshot状态 0-off 1-on
    U8       offloadNum;                ///< 保存的snapshot个数
    U8       triggerMinNumSecBeforeOcr; ///< 驱动程序需要出发快照的最小数量
    U8       isReadOnly;                ///< 0-readonly 1-rw
    U8       disableOffloadNum;         ///< not support
    U8       disablePtb;                ///< 不使能PTB
    U8       reserved1[10];
} Ps3LibSnapShotProp_t;

/**
 * @brief   SnapShot信息头
 */
typedef  struct Ps3LibSnapShotHdr {
    U8  snapShotId;                 ///< snapShot ID
    U8  reserved1[7];
    U32 snapShotTime;               ///< snapShot时间
    U8  snapShotTriggerType;        ///< snapshot触发类型(Ps3LibSnapshotDataFormat_e)
    U8  snapShotDataFormat;         ///< snapShot数据内容(Ps3LibSnapshotTriggerType_e)
    U8  reserved2[10];
    U32 snapShotSizeBytes;          ///< napShot大小
} Ps3LibSnapShotHdr_t;

/**
 * @brief   SnapShot信息
 */
typedef struct Ps3LibSnapShotInfo {
    U8                  countOfSnapInfo;    ///< snap信息条目技术
    U8                  reserved[7];
    Ps3LibSnapShotHdr_t snapInfo[0];        ///< snapshot头部信息
} Ps3LibSnapShotInfo_t;

/**
 * @brief   SnapShot数据请求结构体
 */
typedef struct Ps3LibSnapShotDataReq {
    U8      snapShotId;             ///< snapShot ID
    U8      reserved[7];
    U32     actualOffset;           ///< 返回数据偏移量
} Ps3LibSnapShotDataReq_t;

/**
 * @brief   SnapShot数据回复结构体
 */
typedef struct Ps3LibSnapShotDataRsp {
    U8      snapShotId;             ///< snapShot ID
    U8      reserved[7];
    U32     actualOffset;           ///< 返回数据偏移量
    U32     actualByteCount;        ///< 返回字节数
    U8      data[0];                ///< 返回buffer的大小
} Ps3LibSnapShotDataRsp_t;

/**
 * @brief   bbu等属性是否在位信息
 */
typedef enum Ps3LibCtrlBbuHw {
    PS3LIB_CTRL_HW_ABSENT       = 0, ///< bbu不在位
    PS3LIB_CTRL_HW_PRESENT      = 1, ///< bbu在位
    PS3LIB_CTRL_HW_NR,
} Ps3LibCtrlBbuHw_e;

/*
 * brief 策略信息结构体
 */
typedef struct Ps3LibCtrlPolicy {
    U8  pad0[4];
    U32 preFailPollIntDefault; ///< 预失败轮询间隔默认值 单位:分钟
    U32 inthroCom;           ///< not support
    U32 rebuildRate;         ///< 重建率
    U32 rebuildRateDefault;  ///< 默认重建率
    U32 prrate;              ///< 巡读率
    U32 prrateDefault;       ///< 默认巡读率
    U32 bgirate;             ///< bgi rate
    U32 bgirateDefault;      ///< 默认 bgi rate
    U32 ccRate;              ///< cc rate
    U32 ccRateDefault;       ///< 默认cc rate
    U32 migrationRate;       ///< migratio rate
    U32 migrationRateDefault; ///< 默认migratio rate
    U32 cacheInterval;
    U32 flushTime;            ///< 缓存刷写时间 单位:秒
    U8  drivemode;            ///< 驱动模式
    U8  autorebuild;
    U8  batteryWarning;       ///< 电池告警
    U8  pad;
    U32 eccBuckSize;          ///< not support
    U32 eccBuckleak;          ///< not support
    U8  restoreInsert;        ///< not support
    U8  exposedevice;         ///< not support
    U8  mainPdFailHistory;      ///< 维护磁盘故障信息开关
    U8  reoder;               ///< not support
    U8  autodetect;           ///< not support
    U8  loadBalance;          ///< not support
    U8  securityAss;          ///< not support
    U8  disableReset;
    U8  enterpriseCapability; ///< not support
    U8  bootAgent;            ///< not support
    U8  configured;           ///< not support
    U8  pad2;
    U32 preFailPollInt;      ///< 预失败轮询间隔 单位:分钟
} Ps3LibCtrlPolicy_t;

/*
 * brief 启动信息结构体
 */
typedef struct Ps3LibCtrlBoot {
    U8  biosvds;
    U8  stopBiosError; ///< not support
    U8  pad[2];
    U32 delayPost;     ///< not support
    U8  spinMode;      ///< not support
    U8  pad1;
    U8  enableBios;    ///< not support
    U8  pad2;
    U32 maxDrive;
    U32 maxNumDirect;
    U32 delayAmong;
    U8  allowBoot;
    U8  pad3[3];
    U8  enablePreBoot;   ///< not support
    U8  pad4[7];
} Ps3LibCtrlBoot_t;

/*
 * brief 高可用性信息结构体
 */
typedef struct Ps3LibCtrlHighAvail {
    U8  topology;      ///< 拓扑类型
    U8  clusterPer;    ///< 允许集群
    U8  clusterActive; ///< 允许激活
    U8  pad;
} Ps3LibCtrlHighAvail_t;

/*
 * brief 默认信息结构体
 */
typedef struct Ps3LibCtrlDefaults {
    U32 phyPolarity;      ///< not support
    U32 phyPolaritysplit; ///< not support
    U32 stripSize;        ///< not support
    U8  writePolicy;      ///< 写策略
    U8  readPolicy;       ///< 读策略
    U8  cacheBBU;         ///< not support
    U8  cachedIo;         ///< not support, 返回全FF
    U32 defaultSpin;      ///< 默认旋转
    U8  coercionNode;
    U8  restorehot;        ///< not support
    U8  exposeEnclosure;   ///< not support
    U8  maintainPd;

    U8  zeroBase;          ///< 零基
    U8  disablePunc;
    U8  enableldBbm;       ///< 使能VD BBM
    U8  disableHii;        ///< 去使能HII

    U8  uncertified;         ///< not support
    U8  smartmode;           ///< not support
    U8  enableled;           ///< not support
    U8  leddrive;            ///< not support

    U8  dirtyled;            ///< not support
    U8  enableCrash;         ///< not support
    U8  disablectrlreset;
    U8  poweroption;

    U8  ttylog;               ///< tty log
    U8  autoimport;           ///< 自动导出
    U8  breakmirror;          ///< not support
    U8  maxChainedEnclosures; ///< 最大连锁机柜
    U32 timetaken;            ///< not support
    U8  linkSpeed;            ///< 连接速度
    U8  pad2[3];
    Ps3LibCtrlHighAvail_t highAvail;
} Ps3LibCtrlDefaults_t;

/*
 * brief 调度任务信息结构体
 */
typedef struct Ps3LibCtrlSchedule {
    U32 ccReocurrence;    ///< 控制卡启动定时cc的延时时间
    U32 nextCCLaunch;     ///< 控制卡下次cc开始时间
    U32 prReocurrence;    ///< 控制卡巡读持续时间
    U32 nextPrLaunch;     ///< 控制卡下次巡读开始时间
    U64 batteryLearn;      ///< 巡读周期
    U64 nextBatteryLearn;  ///< 下次巡读周期
    S8  oemId[PS3LIB_BATTERY_FRU];
    U8  pad[7];
} Ps3LibCtrlSchedule_t;

typedef struct Ps3LibCtrlSecureBoot {
    U8  secureBootEnabled; ///< 安全模式下启动控制器
    U8  softSecureMode;    ///< 软安全模式
    U8  hardSecureMode;    ///< 硬安全模式
    U8  updatePending;     ///< 更新挂起
    U32 secureKeySlots;    ///< 暂未实现
} Ps3LibCtrlSecureBoot_t;

/*
 * brief ukey在位状态信息
 */
typedef enum Ps3LibUkeyCliStatus {
    PS3LIB_UKEY_DEGRADE_STATUS,           ///< ukey降级状态码
    PS3LIB_UKEY_INSERT_STATUS,            ///< ukey在位状态码
} Ps3LibUkeyCliStatus_e;

/*
 * brief Ukey raid类型
 */
typedef enum Ps3LibUkeyRaidType {
    PS3LIB_UKEY_BIT_RAID0_1 = 0,
    PS3LIB_UKEY_BIT_RAID5   = 1,
    PS3LIB_UKEY_BIT_RAID6   = 2,
    PS3LIB_UKEY_BIT_RAID_UNLIMIT = 0XFE,
    PS3LIB_UKEY_RAID_BIT_NR = 0XFFFFFFFF
} Ps3LibUkeyRaidType_e;

/*
 * brief Ukey SM4标记
 */
typedef enum Ps3LibUkeySM4Flag {
    PS3LIB_UKEY_BIT_SM4_OFF  = 0,
    PS3LIB_UKEY_BIT_SM4_ON   = 1,
    PS3LIB_UKEY_SM4_BIT_NR   = 0XFFFFFFFF
} Ps3LibUkeySM4Flag_e;

/*
 * brief ukey可支持的高级功能表
 */
typedef struct Ps3LibUkeyAsoBitTable {
    Ps3LibUkeyRaidType_e     raidType;   ///< RAID类型：0/1、5、6
    U32                      pdNumber;   ///< PD个数 全F代表不限制
    U32                      vdNumber;   ///< VD个数 全F代表不限制
    Ps3LibUkeySM4Flag_e      sm4Flag;    ///< 是否支持SM4，支持为1，不支持为0
} Ps3LibUkeyAsoBitTable_s;

/*
 * brief ukey使用期限
 */
typedef enum Ps3LibUkeyCliTimeRemain {
    PS3LIB_UKEY_TIME_UNLIMITED,            ///< unlimited 状态
    PS3LIB_UKEY_TIME_DEACTIVE_ON_SYSTEM_REBOOT,  ///< 重启失效
} Ps3LibUkeyCliTimeRemain_e;

/*
 * brief ukey模式
 */
typedef enum Ps3LibUkeyCliSecureMode {
    PS3LIB_UKEY_MODE_NOT_SECURED,          ///< 不加密状态
    PS3LIB_UKEY_MODE_SECURED,              ///< 加密状态
} Ps3LibUkeyCliSecureMode_e;

/*
 * brief Ukey Aso 上下文信息
 */
typedef struct Ps3LibUkeyAsoContxt {
    U32                         keyStatus;         ///< Ps3LibUkeyCliStatus_e
    Ps3LibUkeyAsoBitTable_s     asoCfg;            ///< ukey可支持的高级功能表
    U32                         timeRemain;        ///< Ps3LibUkeyCliTimeRemain_e
    U32                         secureMode;        ///< Ps3LibUkeyCliSecureMode_e
} Ps3LibUkeyAsoContxt_s;

/**
 * @brief   get snapshot命令参数结构体
 */
typedef struct Ps3LibSnapGetPrv {
    S32               bOndemand;                     ///< 是否通知片内生成snapshot
    S32                snapCnt;                       ///< 需要导出的snap个数  -1表示需要查询列表 导出所有snapshot
    U8                 snapId[PS3LIB_MAX_SNAP_COUNT]; ///< 需要导出的snapshot id
    const S8 *         fileName;                      ///< 导出后的文件名 null表示需要cli生成文件名
    U32                snapInfoSize;                  ///< snapInfo 大小
    U8                 pad[4];
    Ps3LibSnapShotInfo_t *pSnapInfo;                  ///< 查询的snap列表
} Ps3LibSnapGetPrv_t;

/**
 * @brief   SnapShot产生方式格式
 */
typedef enum Ps3LibSnapshotTriggerType {
    PS3LIB_SNAPSHOT_FW_UNSPECIFIED = 0,    ///< snapshot未说明的类型
    PS3LIB_SNAPSHOT_FW_TRIGGERED   = 1,    ///< snapshot类型fw triggered
    PS3LIB_SNAPSHOT_ONDEMAND       = 2,    ///< snapshot类型on-demand
} Ps3LibSnapshotTriggerType_e;

/**
 * @brief   SnapShot文件格式
 */
typedef enum Ps3LibSnapshotDataFormat {
    PS3LIB_SNAPSHOT_ZIPPED                  = 0,    ///< snapshot zipped
    PS3LIB_SNAPSHOT_TEXT                    = 1,    ///< snapshot text
    PS3LIB_SNAPSHOT_NONE                    = 2,    ///< snapshot none
} Ps3LibSnapshotDataFormat_e;

/**
 * @brief   厂商分区保存的生产字段
 * @note    字符串可能不包含终止符'\0'
 */
typedef struct Ps3LibMfgInfo {
    U8  partNumber[PS3LIB_MFG_PART_NUMBER_LEN];     ///< part number 16-byte ASCII string
    U8  serialNumber[PS3LIB_MFG_PART_NUMBER_LEN];   ///< 序列号
    U64 sasAddrBase;                                ///< sas地址
    U32 mfgDate;                                    ///< U32 时间戳
    U8  revision[PS3LIB_MFG_REVISION_LEN];          ///< 版本号
    U8  wwn[PS3LIB_MFG_WWN_LEN];                    ///< 卡的唯一标识符
    U32 reworkDate;                                 ///< 返工时间 时间戳
    U8  pad[4];
    U8  extra1[PS3LIB_MFG_OEM_STR_LEN];             ///< oem字段
    U8  extra2[PS3LIB_MFG_OEM_STR_LEN];             ///< oem字段
} Ps3LibMfgInfo_t;

/**
 * @brief   autoconfig配置信息
 */
typedef struct Ps3LibAutoConfigSet {
    U8 mode;            ///< 0:none,1:R0,2:JBOD
    U8 immediate;       ///< 只有当mode为R0时生效. 0:不立即生效,1:立即生效;
    U8 updateCfg;       ///< 是否更新配置文件, 0:不更新, 1:更新;
    U8 pad[5];             ///< 字节对齐
} Ps3LibAutoConfigSet_t;

/**
 * @brief   autoconfig回显信息
 */
typedef struct Ps3LibAutoConfigShow {
    U8 mode;                    ///< 0:none;1:R0;2:JBOD
    U8 supportedAutoConfig;     ///< 按位的位置 0:none;1:R0;2:JBOD ，剩余填0
                                ///< 每个位置上 0:表示不支持，1:表示支持
                                ///< 例如00000101，表示支持none,不支持R0,支持JBOD
    U8 supportedR0Immediate;    ///< 0:不支持;1:支持
    U8 pad;                     ///< 字节对齐
} Ps3LibAutoConfigShow_t;

typedef enum Ps3LibCacheStatus{
    PS3LIB_CACHETEST_RANDOM             = 1,        ///< 随机数
    PS3LIB_CACHETEST_CHECKERBOARD       = 2,
    PS3LIB_CACHETEST_BITFLIP            = 3,
    PS3LIB_CACHETEST_WALKBITS1          = 4,
    PS3LIB_CACHETEST_WALKBITS0          = 5,
    PS3LIB_CACHETEST_NR                 = 0xFF,     ///< 无效模式
}Ps3LibCacheStatus_e;

typedef struct Ps3LibCacheTestType{
    U8  cacheTestType;
    U8  pad[3];
}Ps3LibCacheTestType_t;

typedef struct Ps3LibCacheTestRspSize {
    U32 cacheSize;
    U32 memorySize;
}Ps3LibCacheTestRspSize_t;

/**
 * @brief autoconfig模式枚举
 */
typedef enum Ps3LibCtrlAutoConfigMode {
    PS3LIB_AUTOCONFIG_MODE_NONE = 0,
    PS3LIB_AUTOCONFIG_MODE_R0   = 1,
    PS3LIB_AUTOCONFIG_MODE_JBOD = 2,
} Ps3LibCtrlAutoConfigMode_e;

/**
 * @brief autoconfig immediate枚举
 */
enum {
    PS3LIB_AUTOCONFIG_SUPPORT_R0_IMMEDIATE_NO = 0,
    PS3LIB_AUTOCONFIG_SUPPORT_R0_IMMEDIATE_YES = 1,
};

/*
 * @brief 定义 PCI 链路速率枚举
 */
typedef enum Ps3LibPciLinkSpeed {
    PS3LIB_PCI_LINK_SPEED_DEFAULT       = 0,    ///< Max supported by hardware
    PS3LIB_PCI_LINK_SPEED_GEN_1         = 1,    ///< Gen1  2.5GT/s
    PS3LIB_PCI_LINK_SPEED_GEN_2         = 2,    ///< Gen2  5GT/s
    PS3LIB_PCI_LINK_SPEED_GEN_3         = 3,    ///< Gen3  8GT/s
    PS3LIB_PCI_LINK_SPEED_GEN_4         = 4,    ///< Gen4  16GT/s
    PS3LIB_PCI_LINK_SPEED_GEN_5         = 5,    ///< Gen5  32GT/s
} Ps3LibPciLinkSpeed_e;

/**
 * @brief  控制卡主机接口信息
 */
typedef struct Ps3LibCtrlInterfaceInfo {
    struct {
        U8      SAS_3G      : 1;
        U8      iSCSI       : 1;
        U8      PCIe        : 1;
        U8      PCIx        : 1;
        U8      SRIOV       : 1;
        U8      pad1        : 3;    ///< 保留位域
        U8      maxPciLinkSpeed;    ///< 最大链路速率 enum Ps3LibPciLinkSpeed_e
        U8      curPciLinkSpeed;    ///< 当前链路速率 enum Ps3LibPciLinkSpeed_e
        U8      pad2[5];            ///< 保留字段
    } hostInterface;

    struct {
        U8      SPI         : 1;
        U8      SAS_3G      : 1;
        U8      SATA_1_5G   : 1;
        U8      SATA_3G     : 1;
        U8      SAS_6G      : 1;
        U8      SAS_12G     : 1;
        U8      SATA_6G     : 1;
        U8      pad         : 1;    ///< 保留位域
        U8      maxPciLinkSpeed;    ///< 最大链路速率 enum Ps3LibPciLinkSpeed_e
        U8      pad1;               ///< 保留字段
        U8      portCount;          ///< 下联phy列表数量
        U8      pad2[4];            ///< 保留字段
    } deviceInterface;
} Ps3LibCtrlInterfaceInfo_t;

/**
 * @brief   控制卡下pd和vd数量
 */
typedef struct Ps3LibCtrlDriveInfo {
    U16 pdCount;
    U16 vdCount;
} Ps3LibCtrlDriveInfo_t;

/**
 * @brief   控制卡信息结构体
 */
typedef struct Ps3LibCtrlInfo {
    Ps3LibCtrlGetInfo_t      baseInfo;              ///< 基础信息
    Ps3LibCtrlGetVersion_t   versionInfo;           ///< 版本信息
    Ps3LibCtrlInterfaceInfo_t interfaceInfo;       ///< 控制卡接口信息
    Ps3LibCtrlGetStatus_t    statusInfo;            ///< 状态信息
    Ps3LibCtrlGetAdapter_t   adapterInfo;           ///< 控制卡允许的操作
    Ps3LibCtrlHwCfg_t        hwCfgInfo;             ///< 硬件配置信息
    Ps3LibCtrlPolicy_t       policyInfo;            ///< 策略信息
    Ps3LibCtrlBoot_t         bootInfo;              ///< 启动信息
    Ps3LibCtrlDefaults_t     defaultsInfo;          ///< 默认配置信息
    Ps3LibCtrlCapability_t   capability;            ///< 容量信息
    Ps3LibCtrlPropInfo_t     propInfo;              ///< 控制卡属性信息
    Ps3LibCtrlDriveInfo_t    driveInfo;             ///< 控制卡下pd和vd信息
    U8                       pad[4];
} Ps3LibCtrlInfo_t;

typedef struct Ps3LibCtrlPrMode
{
    U32 mode;               ///< PR 执行模式,取值参考 PS3LIB_CTRL_PR_MODE_NR
    U8 isHasStartTime;      ///< 用户是否输入了starttime，输入starttime为1，未输入starttime为0
    U8 pad[PS3LIB_CTRL_CC_PR_PAD_LEN];
} Ps3LibCtrlPrMode_t;

/**
 * @brief   lib库版本
 */
typedef struct Ps3LibVersion {
    S8 libVersion[PS3LIB_VERSION_LEN]; ///< lib库版本(注意:字符串可能不包含终止符'\0')
} Ps3LibVersion_s;

/**
 * @brief ps设置结构体
 */
typedef struct Ps3LibCtrlPsInfo
{
    U8  state;          ///< 省电模式开关
    U8  type;           ///< 省电模式停转的盘类型
    U16 spindownTime;   ///< 省点模式下盘不工作多久会停转 min
} Ps3LibCtrlPsInfo_s;

/**
 * @brief   ctrl PS Type 枚举
 */
typedef enum Ps3LibCtrlPSType {
    PS3LIB_CTRL_PS_TYPE_READY = 0x01,
    PS3LIB_CTRL_PS_TYPE_HS    = 0x02,
    PS3LIB_CTRL_PS_TYPE_ALL   = 0x03,
    PS3LIB_CTRL_PS_TYPE_NR    = 0xFF,
} Ps3LibCtrlPSType_t;

/**
 * @brief   拓扑设备类型
 */
typedef enum Ps3LibTopologyDevType {
    PS3LIB_NO_DEVICE = 0,   ///< 无设备
    PS3LIB_END_DEVICE,      ///< 终端设备
    PS3LIB_EDGE_EXPANDER,   ///< edge expander
    PS3LIB_FANOUT_EXPANDER, ///< fanout expander
    PS3LIB_PCIE_SWITCH,      ///< switch
    PS3LIB_IOC,             ///< 控制卡
} Ps3LibTopologyDevType_e;

/**
 * @brief   sas设备类型详细信息
 */
typedef union {
    struct {
        U32 SSPInitiator : 1; ///< ssp initiator
        U32 STPInitiator : 1; ///< stp initiator
        U32 SMPInitiator : 1; ///< smp initiator
        U32 SSPTarget : 1;    ///< ssp target
        U32 STPTarget : 1;    ///< stp target
        U32 SMPTarget : 1;    ///< smp target
        U32 SATADevice : 1;   ///< sata设备
        U32 reserved : 25;
    };
    U32 deviceWord;
} Ps3LibTopologyDevBitMap_u;

/**
 * @brief   phy/lane详细信息
 */
typedef struct Ps3LibTopologyPhy {
    U8  attachedDeviceType; ///< 对端设备类型
    U8  attachedPhyId;      ///< 对端phyId/laneId
    U16 attachedDeviceOffset; ///< phy/lane连接的设备节点结构体在整个拓扑信息总的内存偏移量
    Ps3LibTopologyDevBitMap_u  deviceBitMap;
    U64 attachedSasAddr;       ///< 对端sas地址
    U8  disable : 1;
    U8  reserved1 : 7;
    U8  reserved[7];
} Ps3LibTopologyPhy_s;

/**
 * @brief   控制卡拓扑信息根节点 即控制卡节点
 */
typedef struct Ps3LibTopologyRootNode {
    U8                  deviceType; ///< 控制卡的设备类型 即IOC
    U8                  reserved;
    U16                 totalSize; ///< 整个控制卡拓扑信息的数据大小
    U8                  reserved1[3];
    U8                  numPhy;                           ///< 控制卡的phy/lane个数
    Ps3LibTopologyPhy_s phyList[PS3LIB_CTRL_MAX_PHY_CNT]; ///< 控制卡的phy/lane列表
} Ps3LibTopologyRootNode_s;

/**
 * @brief   控制卡拓扑信息子节点 即Expander或Switch节点
 */
typedef struct Ps3LibTopologyExpNode {
    U8                        enclId;   ///< 设备ID
    U8                        deviceType; ///< expander/switch的设备类型
    U8                        numPhy;     ///< expander的phy个数 或switch的lane个数
    U8                        reserved[1];
    Ps3LibTopologyDevBitMap_u deviceBitMap; ///< 详细设备类型信息位图 设备为switch时无效
    U64                       sasAddr;      ///< expander的sas地址 或switch的wwid
    Ps3LibTopologyPhy_s       phyList[0];   ///< expander的phy列表 或switch的lane列表
} Ps3LibTopologyExpNode_s;

/**
 * @brief   终端设备的phy/lane信息
 */
typedef struct Ps3LibTopologyEndDevPhy {
    U64 sasAddr; ///< 终端设备的phy sas地址 或lane wwid
    U32 reserved[2];
} Ps3LibTopologyEndDevPhy_s;

/**
 * @brief   控制卡拓扑信息叶子节点 即硬盘节点
 */
typedef struct Ps3LibTopologyEndDevNode {
    U8                        deviceType;   ///< 终端设备的设备类型
    U8                        numPhy;       ///< phy个数
    U16                       deviceId;     ///< 设备id
    Ps3LibTopologyDevBitMap_u deviceBitMap; ///< 详细设备类型信息位图
    U32                       reserved[2];
    Ps3LibTopologyEndDevPhy_s phyList[2]; ///< phy列表
} Ps3LibTopologyEndDevNode_s;

/**
 * @brief  与固件交互使用 profile信息
 */
typedef struct Ps3LibProfileInfo {
    U16   profileId;
    U8    personalityMode;
    U8    reserved0;
    U16   maxPdCount;
    U16   maxVdCount;
    U16   maxNvmeCount;
    U16   isDflt:1;
    U16   isCurr:1;
    U16   isOptm:1;
    U16   isPend:1;
    U16   isNvmeOnlyProfile:1;
    U16   isWbSup:1;
    U16   isCmp:1;
    U16   reserved1:9;
} Ps3LibProfileInfo_t;

/**
 * @brief  与固件交互使用 profile信息列表
 */
typedef struct Ps3LibProfileList {
    U32                   count;
    Ps3LibProfileInfo_t   profileList[0];
} Ps3LibProfileList_t;

/**
 * @brief   the Function Type Enum, 控制卡内部funcType
 */
typedef enum Ps3LibCtrlFuncType {
    PS3LIB_CTRL_FUNC_X2   = 0,
    PS3LIB_CTRL_FUNC_X16  = 1,
    PS3LIB_CTRL_FUNC_NR   = 2,
} Ps3LibCtrlFuncType_e;

/**
 * @brief 元数据导入结果
 * 1个response代表一种类型的导入结果
 * 
 */
typedef struct Ps3LibCtrlImportResponse{
    U32 type; ///< 类型
    U32 ret;  ///< 返回值
} Ps3LibCtrlImportResponse_s;

/**
 * @brief 元数据导入的总结果
 * 元数据导入根据兼容性 v100只导入控制卡配置
 *                    v200导入控制卡配置和元数据                      
 */
typedef struct Ps3LibCtrlImportResult {
    U32 count;  ///< 数量
    Ps3LibCtrlImportResponse_s data[0]; ///< 结果内容柔性数组
} Ps3LibCtrlImportResult_s;

/**
 * brief 元数据导入功能类型
*/
typedef enum Ps3LibImportType {
    PS3LIB_IMPORT_TYPE_META = 0, ///< 元数据导入
    PS3LIB_IMPORT_TYPE_CFG,      ///< 控制卡配置导入
    PS3LIB_IMPORT_TYPE_NR,       
} Ps3LibImportType_e;

///< 控制卡动态信息
typedef struct Ps3LibCtrlDynamicInfo {
   U16 countOfPdCreateVd;    ///< 当前控制卡下可创建VD的PD个数
   U16 pad[3];
   U8  reserved[88];
} Ps3LibCtrlDynamicInfo_s;

/**
 * @brief 功率数据
 * 
 */
typedef struct Ps3LibRealtimePower {
    U32   rtp;         ///< 功率(单位mW)
    U8    pad[68];     ///< 占位8字节对齐
} Ps3LibRealtimePower_s;

///< 温度传感器温度类型
enum {
    PS3LIB_CTRL_TEMPSENSOR_TUNE_TYPE = 0,           ///<  板卡整体虚拟温度类型
    PS3LIB_CTRL_TEMPSENSOR_BOARD_TYPE,              ///<  Board温度类型
    PS3LIB_CTRL_TEMPSENSOR_CHIP_TYPE,               ///<  芯片温度类型
    PS3LIB_CTRL_TEMPSENSOR_ONF_TYPE,                ///<  ONF温度类型
    PS3LIB_CTRL_TEMPSENSOR_BBU_TYPE,                ///<  BBU温度类型
    PS3LIB_CTRL_TEMPSENSOR_MAX_TYPE,                ///<  板卡上温度sensor最大类型数量
};

///< 板卡上调温温度信息和所有温度sensor的信息
typedef struct Ps3LibCtrlHwTempSensorInfo {
    S16 tuneTempCur;                ///<  板卡整体温度,单位:摄氏度
    S16 tuneTempThreshold;          ///<  板卡整体调节阈值,单位:摄氏度
    U8  tuneTempType;               ///<  当前整体温度信息来自的器件类型 参考 PS3LIB_CTRL_TEMPSENSOR_MAX_TYPE
    U8  pad;
    U16 sensorNum;                ///< 温度传感器数量，表示下面的tempSensor的前几个为有效信息
    struct {
        S16 temperature;                        ///< 温度传感器当前温度,单位:摄氏度
        S16 temperatureThreshold;               ///< 器件温度阈值,单位:摄氏度
        U8  temperatureSensorType;              ///< 器件类型，无效值为0xFF
        U8  pad[3];
    } tempSensor[PS3LIB_CTRL_TEMPSENSOR_COUNT_MAX];  ///< 温度sensor提供的温度信息
} Ps3LibCtrlHwTempSensorInfo_t;

/**
 * @brief  powersave相关属性
 */
typedef struct Ps3LibPsInfo {
    U8        psSpnDwnReady;
    U8        psSpnDwnHS;
    U16       psSpinDownTime;
    U8        reserved[4];
} Ps3LibPsInfo_t;

/**
 * brief 控制卡接入盘的模式
*/
typedef enum Ps3LibCtrlOperationMode {
    PS3LIB_CTRL_OPERATION_MODE_RAID   = 0x00,   ///< 0:RAID模式(有RAID配置的盘按VD规则接入)
    PS3LIB_CTRL_OPERATION_MODE_PTH    = 0x01,   ///< 1:PTH模式(直通接入)
    PS3LIB_CTRL_OPERATION_MODE_NR,
} Ps3LibCtrlOperationMode_e;

/**
 * brief 权限规则结构体
*/
typedef struct Ps3LibCtrlPermRulesList {
    U8    count;        ///< 权限规则个数
    U8    pad[3];       ///< 保留字段
    U32   ruleList[PS3LIB_CTRL_PERM_RULES_COUNT]; ///< 权限规则列表
} Ps3LibCtrlPermiRulesList_t;

/**
 *@brief 权限规则枚举
 *@note  权限管理命令下发时, 将枚举填充至规则列表, 即可恢复或禁用(OS)对应功能权限
 *@note  权限管理命令查询时, 可查询当前权限管理情况
 */
typedef enum Ps3LibCtrlPermRules {
    PS3LIB_CTRL_PERM_DOWNLOAD_IOC_ENABLE  = 0x0B0A0000, ///< 恢复IOC默认升级权限/当前IOC升级权限不受限制
    PS3LIB_CTRL_PERM_DOWNLOAD_IOC_DISABLE = 0x0B0A00FF, ///< 禁用IOC升级权限
    PS3LIB_CTRL_PERM_DOWNLOAD_EXP_ENABLE  = 0x040A0000, ///< 恢复EXP默认升级权限/当前EXP升级权限不受限制
    PS3LIB_CTRL_PERM_DOWNLOAD_EXP_DISABLE = 0x040A00FF, ///< 禁用EXP升级权限
} Ps3LibCtrlPermRules_e;

/**
 * brief 权限规则设置接口
*/
Ps3Errno ps3libCtrlPermRulesSet(CtrlId_t ctrlId, Ps3LibCtrlPermiRulesList_t *pRules);

/**
 * brief 权限规则获取接口
*/
Ps3Errno ps3libCtrlPermRulesGet(CtrlId_t ctrlId, Ps3LibCtrlPermiRulesList_t *pRules);

/**
 * @brief        设置控制卡Securitykey
 * @param[in]    ctrlId: 控制卡Id
 * @param[out]   securityInfo: 加密信息结构体指针
 * @return       PS3_ERRNO_SUCCESS: 成功;其他: 失败
 */
Ps3Errno ps3libCtrlSecuritySet(CtrlId_t ctrlId, Ps3LibCtrlSecurityInfo_t *securityInfo);

/**
 * @brief      删除控制卡security配置
 * @param[in]  ctrlId: 控制卡Id
 * @return     PS3_ERRNO_SUCCESS: 成功;其他: 失败
 */
Ps3Errno ps3libCtrlSecurityDelete(CtrlId_t ctrlId);

/**
 * @brief      初始化 ps3lib库
 * @param[in]  ctrlList 控制卡列表
 * @return PS3_ERRNO_SUCCESS: 成功；其他: 失败
 */
Ps3Errno ps3libInit(Ps3LibCtrlList_t *ctrlList);

/**
 * @brief      销毁 Ps3Lib库
 * @return PS3_ERRNO_SUCCESS: 成功；其他: 失败
 */
Ps3Errno ps3libDestory();

/**
 * @brief        获取控制卡允许的操作
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   ctrlAllowOps: 控制卡允许的操作
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlAllowedOpsGet(CtrlId_t ctrlId, Ps3LibCtrlAllowedOps_t *ctrlAllowOps);

/**
 * @brief        设置控制卡属性信息
 * @param[in]    ctrlId:   控制卡标识符
 * @param[in]    ctrlProp: 控制卡特性信息结构体指针
 * @param[out]   errList:  输出错误码列表
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlPropInfoSet(CtrlId_t ctrlId, Ps3LibCtrlPropInfo_t *ctrlProp, Ps3LibErrList_t *errList);

/**
 * @brief        查询控制卡Alarm状态
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   alarmState: U8*指针
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlAlarmGet(CtrlId_t ctrlId, U8 *alarmState);

/**
 * @brief        使能控制卡蜂鸣器
 * @param[in]    ctrlId: 控制卡标识符
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlAlarmEnable(CtrlId_t ctrlId);

/**
 * @brief        控制卡恢复出厂设置
 * @param[in]    ctrlId: 控制卡标识符
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlFactoryDefaultsSet(CtrlId_t ctrlId);

/**
 * @brief        使控制卡蜂鸣器无效
 * @param[in]    ctrlId: 控制卡标识符
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlAlarmDisable(CtrlId_t ctrlId);

/**
 * @brief       silence控制卡的蜂鸣器
 * @param[in]   ctrlId: 控制卡标识符
 * @note        silence alarm需要保持蜂鸣器原来的状态
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlAlarmSilence(CtrlId_t ctrlId);

/**
 * @brief       获取PRStatus信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  prStatus: Ps3LibPRStatus_t结构体指针
 * @note        只能获取到state numPdDone numIteration三个参数值
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlPRStatusInfoGet(CtrlId_t ctrlId, Ps3LibPRStatus_t *prStatus);

/**
 * @brief        获取PR Prop信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   prProp: Ps3LibPRProperties_t结构体指针
 * @note         只能获取到opMode maxPD nextExec execFreq includessdsFlag uncfgareasFlag六个参数值
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlPRPropGet(CtrlId_t ctrlId, Ps3LibPRProperties_t *prProp);

/**
 * @brief        设置PR Prop信息
 * @param[in]    ctrlId : 控制卡标识符
 * @param[in]    prProp : Ps3LibPRProperties_t结构体指针
 * @note         参考stortest工具只有opMode maxPD execFreq可供设置
 * @warning      opMode   : 0-disabled
                            1-auto
                            2-manual
                 maxPD    : 1-64
                 execFreq :0-65535, 小时
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlPRPropSet(CtrlId_t ctrlId, Ps3LibPRProperties_t *prProp);

/**
 * @brief       控制卡开始巡读
 * @param[in]   ctrlId : 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlPrStart(CtrlId_t ctrlId);

/**
 * @brief       控制卡停止巡读
 * @param[in]   ctrlId : 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlPrAbort(CtrlId_t ctrlId);

/**
 * @brief  获取控制卡类型
 * @param[in]  ctrlId: 控制卡ID
 * @param[out] ctrlType: 控制卡类型
 * @return Ps3Errno
 */
Ps3Errno ps3libCtrlTypeGet(CtrlId_t ctrlId, Ps3LibCtrlType_e *ctrlType);

/**
 * @brief          填充ctrl列表
 * @param[in/out]  ctrlList: 控制卡列表
 * @note   OOB模式需提前填充
 * @return Ps3Errno
 */
Ps3Errno ps3libCtrlListGet(Ps3LibCtrlList_t *ctrlList);

/**
 * @brief  获取 ctrl 的个数
 * @param[out]  ctrlCount: 控制卡个数
 * @return Ps3Errno
 */
Ps3Errno ps3libCtrlCountGet(U32 *ctrlCount);

/**
 * @brief        显示控制卡bootdrive开关状态
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   info:  控制卡驱动信息
 * @return       Ps3Errno
 */
Ps3Errno ps3libCtrlBootdriveInfoGet(CtrlId_t ctrlId, Ps3LibBootdriveInfo_t *info);

/**
 * @brief        获取控制卡健康状态
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   ctrlOverview: 控制卡健康状态信息
 * @return       Ps3Errno
 */
Ps3Errno ps3libCtrlOverviewGet(CtrlId_t ctrlId, Ps3LibCtrlOverview_s *ctrlOverview);

/**
 * @brief       获取控制卡拓扑信息
 * @param[in]   ctrlId : 控制卡标识符
 * @param[out]  pTopoInfo : 控制卡拓扑信息结构体
 * @param[in]   topologySize : 控制卡拓扑信息大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlTopologyInfoGet(CtrlId_t ctrlId, Ps3LibTopologyRootNode_s *pTopology, U32 topologySize);

/**
 * @brief       获取lib库版本
 * @param[out]  libVersion : lib库版本号
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVersionGet(Ps3LibVersion_s *libVersion);

/**
 * @brief       获取控制卡信息
 * @param[in]   ctrlId :   控制卡标识符
 * @param[in]   ctrlInfo : 控制卡信息结构体指针
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlInfoGet(CtrlId_t ctrlId, Ps3LibCtrlInfo_t *ctrlInfo);

/**
 * @brief       控制卡暂停巡读
 * @param[in]   ctrlId : 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlPrPause(CtrlId_t ctrlId);

/**
 * @brief       控制卡恢复巡读
 * @param[in]   ctrlId : 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlPrResume(CtrlId_t ctrlId);

/**
 * @brief       删除控制卡配置
 * @param[in]   ctrlId,    控制卡标识符
 * @param[in]   forceFlag, 是否强制删除
 * @note        OOB模式下, forceFlag=0时, 禁止清除配置
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlConfigDelete(CtrlId_t ctrlId, U8 forceFlag);

/**
 * @brief        重启控制卡
 * @param[in]    ctrlId: 控制卡标识符
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlIocRestart(CtrlId_t ctrlId);

/**
 * @brief       设置控制卡profileID
 * @param[in]   ctrlId, 控制卡标识符
 * @param[in]   profileId, 配置文件ID
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlProfileIdSet(CtrlId_t ctrlId, U16 profileId);

/**
 * @brief        获取CCStatus信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   ccStatus: 一致性校验状态信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlCCStatusInfoGet(CtrlId_t ctrlId, Ps3LibCCScheduleStatus_t *ccStatus);

/**
 * @brief       设置CC Prop信息
 * @param[in]   ctrlId : 控制卡标识符
 * @param[in]   ccProp : 一致性校验属性信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlCCPropSet(CtrlId_t ctrlId, Ps3LibCCProperties_t *ccProp);

/**
 * @brief        获取控制卡properties信息
 * @param[in]    ctrlId:   控制卡标识符
 * @param[out]   ctrlProp: 控制卡特性信息结构体指针
 * @note         获取termlog流程为不可中断流程, 启动后便不再响应非本流程功能
 * @note         获取termlog接口执行结果返回后, 则恢复响应其他功能
 * @warning      强行中断后, 若其他接口无法正常交互, 需调用ps3libThreadAbortProcess()结束不可中断流程
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlPropInfoGet(CtrlId_t ctrlId, Ps3LibCtrlPropInfo_t *ctrlProp);

/**
 * @brief       获取termlog内容
 * @param[in]   ctrlId:      制卡标识符
 * @param[out]  termLogReq:  termlog请求结构体
 * @param[out]  termlogInfo: termlog请求结构体
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libTermlogGet(CtrlId_t ctrlId, Ps3LibCtrlTermLogReq_t *termLogReq, Ps3LibCtrlTermLogInfo_t *termlogInfo,
        S32 termlogInfoSize);

/**
 * @brief       获取控制卡信息
 * @param[in]   ctrlId:   控制卡标识符
 * @param[in]   info: 控制卡信息结构体指针
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlBaseInfoGet(CtrlId_t ctrlId, Ps3LibCtrlGetInfo_t *info);

/**
 * @brief        控制卡能力集信息获取
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   capability: 控制卡能力集信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlCapabilityGet(CtrlId_t ctrlId, Ps3LibCtrlCapability_t *capability);

/**
 * @brief        控制卡硬件配置信息获取
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   hwcfg指针: 硬件配置信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlHwCfgInfoGet(CtrlId_t ctrlId, Ps3LibCtrlHwCfg_t *hwcfg);

/**
 * @brief        显示控制卡时间
 * @param[in]    ctrlId,   控制卡标识符
 * @param[out]   tmpInfo,  time U64指针,毫秒级
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlTimeGet(CtrlId_t ctrlId, U64 *tmpInfo);

/**
 * @brief        控制卡状态信息获取
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   ctrlstatus: 控制卡状态信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlStatusInfoGet(CtrlId_t ctrlId, Ps3LibCtrlGetStatus_t *ctrlstatus);

/**
 * @brief       判断OCR开关是否打开
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   ocrState: OCR开关状态
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlOcrGet(CtrlId_t ctrlId, U8 *ocrState);

/**
 * @brief        查询重建率
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   rebuildRat : U8指针
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlRebuildRateGet(CtrlId_t ctrlId, U8 *rebuildRate);

/**
 * @brief        设置重建任务资源占有率
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    rebuildRate : 重建率, 范围0-100
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlRebuildRateSet(CtrlId_t ctrlId, U8 rebuildRate);

/**
 * @brief       获取版本号信息接口
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  info: ctrlVersion结构指针
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlVersionInfoGet(CtrlId_t ctrlId, Ps3LibCtrlGetVersion_t *ctrlVersion);

/**
 * @brief        获取控制卡adapter信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   pAdapterOps: 控制卡能力集
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlAdapterGet(CtrlId_t ctrlId, Ps3LibCtrlGetAdapter_t *pAdapterOps);

/**
 * @brief        获取控制卡加密的keyid
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   keyid: 加密Id
 * @return       Ps3Errno
 */
Ps3Errno ps3libCtrlSecurityKeyidGet(CtrlId_t ctrlId, Ps3LibCtrlSecurityId_t *keyid);

/**
 * @brief         验证securitykey是否正确
 * @param[in]     ctrlId: 控制卡标识符
 * @param[in]     securityKey: 秘钥指针
 * @param[in]     len; 秘钥长度
 * @return        Ps3Errno
 */
Ps3Errno ps3libCtrlSecurityCompare(CtrlId_t ctrlId, S8 *securityKey, U8 len);

/**
 * @brief       获取推荐密码
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  securityKey: 秘钥
 * @param[in]   len; 秘钥长度
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlPassphraseSuggestGet(U8 *pBuffer, U8 bufLen);

/**
 * @brief       获取控制卡休眠状态
 * @param[in]   ctrlId: 控制卡ID
 * @param[out]  ctrlStatus: 控制卡休眠状态
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlSleepStatusGet(CtrlId_t ctrlId, U8 *ctrlStatus);

/**
 * @brief      删除snapshot
 * @param[in]  ctrlId: 控制卡标识符
 * @return     PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlSnapShotClear(CtrlId_t ctrlId);

/**
 * @brief        查询控制卡snapshot属性
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   pSnapshotProp: snapshot属性
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlSnapShotPropGet(CtrlId_t ctrlId, Ps3LibSnapShotProp_t *pSnapshotProp);

/**
 * @brief        设置控制卡snapshot属性
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    pSnapshotProp: snapshot属性结构体
 * @return  Ps3Errno
 */
Ps3Errno ps3libCtrlSnapshotPorpSet(CtrlId_t ctrlId, Ps3LibSnapShotProp_t *pSnapshotProp);

/**
 * @brief        查询控制卡snapshot信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   pSnapshotInfo: snapshot信息
 * @param[in]    snapshotInfoSize: 可接收的snapshot信息大小
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlSnapShotInfoGet(CtrlId_t ctrlId, Ps3LibSnapShotInfo_t *pSnapshotInfo, U32 snapshotInfoSize);

/**
 * @brief      生成snapshot
 * @param[in]  ctrlId: 控制卡标识符
 * @param[in]  pSnapshotId: snapshot ID
 * @return     PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlSnapshotOndemand(CtrlId_t ctrlId, U8 *pSnapshotId);

/**
 * @brief       查询控制卡snapshot信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  pSnapshotDataReq: snapshot数据请求结构体
 * @param[out]  pSnapshotDataRsp: snapshot数据回复结构体
 * @param[in]   rspSize: 回复结构体大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlSnapShotDataGet(CtrlId_t ctrlId, Ps3LibSnapShotDataReq_t *pSnapshotDataReq,
        Ps3LibSnapShotDataRsp_t *pSnapshotDataRsp, U32 rspSize);

/**
 * @brief        控制卡策略信息获取
 * @param[in]    ctrlId     : CtrlId_t
 * @param[out]   policy指针  : 策略信息
 * @return       Ps3Errno
 */
Ps3Errno ps3libCtrlPolicyGet(CtrlId_t ctrlId, Ps3LibCtrlPolicy_t *policy);

/**
 * @brief        控制卡策略信息获取
 * @param[in]    ctrlId    : 控制卡标识符
 * @param[out]   boot指针  : 策略信息
 * @return       Ps3Errno
 */
Ps3Errno ps3libCtrlBootInfoGet(CtrlId_t ctrlId, Ps3LibCtrlBoot_t *boot);

/**
 * @brief        获取CC Prop信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   ccProp: 一致性校验结构体指针
 * @note         只能获取到opMode nextExec execFreq excludeVd四个参数
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlCCPropGet(CtrlId_t ctrlId, Ps3LibCCProperties_t *ccProp);

/**
 * @brief        显示控制卡时间
 * @param[in]    ctrlId,     控制卡标识符
 * @param[out]   secureBoot, 控制卡启动信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlSecureBootGet(CtrlId_t ctrlId, Ps3LibCtrlSecureBoot_t *secureBoot);

/**
 * @brief       获取指定控制卡的高级功能软件信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  ukeyAsoInfo: Ukey Aso 上下文信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libAsoInfoGet(CtrlId_t ctrlId, Ps3LibUkeyAsoContxt_s *ukeyAsoInfo);

/**
 * @brief        控制卡默认配置信息获取
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   defaults: 默认信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlDefaultGet(CtrlId_t ctrlId, Ps3LibCtrlDefaults_t *defaults);

/**
 * @brief        调度任务信息获取
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   schedule: 调度任务信息
 * @return       Ps3Errno
 */
Ps3Errno ps3libCtrlScheduleGet(CtrlId_t ctrlId, Ps3LibCtrlSchedule_t *schedule);

/**
 * @brief       设置控制卡CC mode
 * @param[in]   ctrlId :  控制卡标识符
 * @param[in]   pCcMode : Ps3LibCcMode_t枚举
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlCCModeSet(CtrlId_t ctrlId, Ps3LibCcMode_t *pCcMode);

/**
 * @brief       设置控制卡CC excludevd
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   excludeVdList: excludevd指针
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlCCExcludevdSet(CtrlId_t ctrlId, Ps3LibExcludeIdList_t *excludeVdList);

/**
 * @brief       设置控制卡CC delay
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   ccDelay: U32类型，单位是小时，范围是0-65535
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlCCDelaySet(CtrlId_t ctrlId, U32 ccDelay);

/**
 * @brief       设置控制卡CC starttime
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   ccStarttime: U32秒级,不得早于当前时间
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlCCStarttimeSet(CtrlId_t ctrlId, U32 ccStarttime);

/**
 * @brief       设置控制卡PR starttime
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   prStarttime: U32秒级,不得早于当前时间
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlPrStarttimeSet(CtrlId_t ctrlId, U32 prStarttime);

/**
 * @brief     设置控制卡进行解密
 * @param[in] ctrlId: 控制卡标识符
 * @param[in] passwd: 秘钥
 * @return    Ps3Errno
 */
Ps3Errno ps3libCtrlUnlockSet(CtrlId_t ctrlId, const S8 *passwd);

/**
 * @brief     设置控制pr的mode
 * @param[in] ctrlId: 控制卡标识符
 * @param[in] prMode: mode结构体
 * @return    Ps3Errno
 */
Ps3Errno ps3libCtrlPrModeSet(CtrlId_t ctrlId,Ps3LibCtrlPrMode_t *prMode);

/**
 * @brief     设置pr的delay属性
 * @param[in] ctrlId: 控制卡标识符
 * @param[in] prDelay: delay数值
 * @return    Ps3Errno
 */
Ps3Errno ps3libCtrlPrDelaySet(CtrlId_t ctrlId, U32 prDelay);

/**
 * @brief     设置pr的prMaxConcurrentPd
 * @param[in] ctrlId: 控制卡标识符
 * @param[in] prMaxConcurrentPd: prMaxConcurrentPd数值
 * @return    Ps3Errno
 */
Ps3Errno ps3libCtrlPrMaxConcurrentPdSet(CtrlId_t ctrlId, U16 prMaxConcurrentPd);

/**
 * @brief     设置pr的prIncludessds
 * @param[in] ctrlId: 控制卡标识符
 * @param[in] prIncludessds: prIncludessds数值
 * @return    Ps3Errno
 */
Ps3Errno ps3libCtrlPrIncludessdsSet(CtrlId_t ctrlId, U8 prIncludessds);

/**
 * @brief     设置pr的prUncfgareas
 * @param[in] ctrlId: 控制卡标识符
 * @param[in] prUncfgareas: prUncfgareas数值
 * @return    Ps3Errno
 */
Ps3Errno ps3libCtrlPrUncfgareasSet(CtrlId_t ctrlId, U8 prUncfgareas);

/**
 * @brief   设置控制卡snapshot属性
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   pSnapshotProp: snapshot属性结构体
 * @return  Ps3Errno
 */
Ps3Errno ps3libCtrlSnapshotPorpSet(CtrlId_t ctrlId, Ps3LibSnapShotProp_t *pSnapshotProp);

/**
 * @brief       获取所有mfginfo的配置
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  pMfgData: 所有mfgdata配置信息
 * @return      Ps3Errno
 */
Ps3Errno ps3libMfgInfoCfgGet(CtrlId_t ctrlId, Ps3LibMfgInfo_t *pMfgInfo);

/**
 * @brief        查询autoconfig相关属性
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   pAutoConfigShow: autoconfig信息的结构体
 * @return       Ps3Errno
 */
Ps3Errno ps3libCtrlAutoConfigShow(CtrlId_t ctrlId, Ps3LibAutoConfigShow_t *pAutoConfigShow);

/**
 * @brief       设置autoconfig相关属性
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   pAutoConfigSet: autoconfig信息的结构体
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlAutoConfigSet(CtrlId_t ctrlId, Ps3LibAutoConfigSet_t *pAutoConfigSet);

/**
 * @brief       设置控制卡powersave类型和状态
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   psInfo: ps设置结构体信息
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlSetPsState(CtrlId_t ctrlId, Ps3LibCtrlPsInfo_s *psInfo);

/**
 * @brief       设置控制卡powersave时间
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   psInfo: ps设置结构体信息
 * @return      Ps3Errno
 */
Ps3Errno ps3libCtrlSetPsTime(CtrlId_t ctrlId, Ps3LibCtrlPsInfo_s *psInfo);

/**
 * @brief ps3lib校验security key oldkey password复杂度
 * 
 * @param ctrlId 控制卡id
 * @param param 参数
 * @param inLen 参数长度
 * @return Ps3Errno 
 */
Ps3Errno ps3libCtrlCheckSecurityComplexity(CtrlId_t ctrlId, U8 *param,
                                           U32 inLen);

/**
 * @brief ps3lib校验keyid复杂度
 * 
 * @param ctrlId 控制卡id
 * @param param 参数
 * @param inLen 参数长度
 * @return Ps3Errno 
 */
Ps3Errno ps3libCtrlCheckKeyidComplexity(CtrlId_t ctrlId, U8 *param, U32 inLen);

/**
 * @brief      获取控制卡OOB运行模式
 * @param[in]  ctrlId: 控制卡标识符
 * @param[out] mode:   运行模式
 * @return     Ps3Errno
 */
Ps3Errno ps3libCtrlOobModeGet(CtrlId_t ctrlId, U8 *mode);

/**
 * @brief   profile信息列表查询
 * @param[in]   ctrlId: 控制卡Id
 * @param[out]  pProfileList: profile信息列表
 * @param[in]   len: profile信息列表申请的内存大小
 * @return  Ps3Errno 
 */
Ps3Errno ps3libCtrlProfileListGet(CtrlId_t ctrlId, Ps3LibProfileList_t *pProfileList, U32 len);

 /**
 * @brief      获取控制卡配置/元数据
 * @param[in]  ctrlId: 控制卡标识符
 * @param[out] mode:   运行模式
 * @return     Ps3Errno
 */
Ps3Errno ps3libCtrlMetaFileGet(CtrlId_t ctrlId, S8 **fileData, U32 *fileSize);

/**
 * @brief 设置控制卡配置/元数据
 * 
 * @param[in] ctrlId 控制卡标识符
 * @param[in] fileData 文件指针
 * @param[in] fileSize 文件大小
 * @param[out] importResult 导入结果
 * @return Ps3Errno 
 */
Ps3Errno ps3libCtrlMetaFileSet(CtrlId_t ctrlId, S8 *fileData, S32 fileSize, Ps3LibCtrlImportResult_s **importResult);

/**
 * @brief 获取实时功率数据
 * 
 * @param ctrlId 控制卡标识符
 * @param realtimePower 功率数据
 * @return Ps3Errno 
 */
Ps3Errno ps3libCtrlRealtimePowerGet(CtrlId_t ctrlId, Ps3LibRealtimePower_s *realtimePower);

/**
 * @brief 获取控制卡接口信息
 * @param[in]  ctrlId:        控制卡Id
 * @param[out] interfaceInfo: 接口信息
 * @return     Ps3Errno 
 */
Ps3Errno ps3libCtrlInterfaceInfoGet(CtrlId_t ctrlId, Ps3LibCtrlInterfaceInfo_t *interfaceInfo);

/**
 * @brief       持续交互流程主动终止后需进行资源清理
 * @param[in]   ctrlId: 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
void ps3libThreadAbortProcess(CtrlId_t ctrlId);

/**
 * @brief       powersave 信息查询
 * @param[in]   ctrlId: 控制卡Id
 * @param[out]  pPsInfo: powersave 信息
 * @return      PS3_ERRNO_SUCCESS: 成功 
 */
Ps3Errno ps3libCtrlPsInfoGet(CtrlId_t ctrlId, Ps3LibPsInfo_t *pPsInfo);

/**
 * @brief 获取控制卡上所有温度点位的温度信息
 * @param[in]  ctrlId:        控制卡Id
 * @param[out] pSensorInfo:   控制卡上温度sensor的信息
 * @return     Ps3Errno
 */
Ps3Errno ps3libCtrlHwTempSensorInfoGet(CtrlId_t ctrlId,  Ps3LibCtrlHwTempSensorInfo_t *pSensorInfo);

/**
 * @brief        设置上报盘优先策略
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    deviceReportingOrder 0-逻辑盘在JBOD之后,1-逻辑盘先于JBOD盘之前                
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlDeviceReportingOrderSet(CtrlId_t ctrlId, U8 deviceReportingOrder);

/**
 * @brief        获取上报盘优先策略
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]    deviceReportingOrder 0-逻辑盘在JBOD之后,1-逻辑盘先于JBOD盘之前                
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlDeviceReportingOrderGet(CtrlId_t ctrlId, U8 *deviceReportingOrder);

/**
 * @brief        设置 slotNumSource
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    slotNumSource 0-Use the connector element index, 1-Use the device slot number
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlSlotNumSourceSet(CtrlId_t ctrlId, U8 slotNumSource);

/**
 * @brief        获取 slotNumSource
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   slotNumSource 0-Use the connector element index, 1-Use the device slot number
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlSlotNumSourceGet(CtrlId_t ctrlId, U8 *slotNumSource);

/**
 * @brief      通过 MCTP Over PCIe发现PCIe设备列表
 * @param[in]  vendorId:     唯一标识,用于确定产品类型
 * @param[out] pCtrlList:    该vendorId下的控制卡列表
 * @return     Ps3Errno
 */
Ps3Errno ps3libDiscoverMCTPCtrlList(U16 vendorId, Ps3LibCtrlList_t *pCtrlList);

#if defined(__cplusplus)
}
#endif

#endif
