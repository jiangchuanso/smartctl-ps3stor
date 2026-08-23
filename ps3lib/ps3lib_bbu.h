/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_bbu.h
 * @date    2023.05.24
 * @brief   BBU信息
 */
#ifndef __PS3LIB_BBU_H__
#define __PS3LIB_BBU_H__

#define PS3LIB_MFG_NAME_LEN                (12)    ///< 生产厂商名长度
#define PS3LIB_DEVICE_NAME_LEN             (12)    ///< 设备名称长度
#define PS3LIB_DEVICE_CHEMISTRY_LEN        (5)     ///< 设备化学成分名称长度
#define PS3LIB_MFG_DATA_LEN                (9)     ///< 生产厂商二进制编码信息数据长度
#define PS3LIB_MODULE_VERSION_LEN          (4)     ///< 模块版本名称长度
#define PS3LIB_BAT_VERSION_LEN             (8)     ///< 电池版本号名称长度
#define PS3LIB_PCB_VERSION_LEN             (10)    ///< PCB版本长度
#define PS3LIB_A_WEEKLY_HOURS              (168)   ///< 一周小时数
#define PS3LIB_DAY_TIME_CONVERTION         (24*60*60*1000)
#define PS3LIB_TIME_CONVERTION             (60*60*1000)

/**
 * @brief   BBU电池错误状态结构体
 */
typedef union Ps3LibBbuBatteryState {
    struct {
        U16 absent : 1;          ///< 电池不在位(1:有效)
        U16 overVoltage : 1;     ///< 过压、过充(1:有效)
        U16 overCurrent : 1;     ///< 过流(1:有效)
        U16 overTemperature : 1; ///< 过温(1:有效)
        U16 overLoad : 1;        ///< 过放(1:有效)
        U16 lifeIsToOver : 1;    ///< 寿命即将到达(1:有效)
        U16 unknownErr : 1;      ///< 未知错误(1:有效)
        U16 reserved : 9;        ///< 保留字段
    };
    U16 b;                       ///< 共用体
} Ps3LibBbuBatteryState_u;

/**
 *  @brief   BBU电量计状态结构体
 */
typedef union Ps3LibBbuGgStatus {
    struct {
        U16 error : 4;                   ///< 错误码
        U16 fullyDischarged : 1;         ///< 放空电
        U16 fullyCharged : 1;            ///< 充满电
        U16 discharging : 1;             ///< 放电中
        U16 initialized : 1;             ///< 初始化
        U16 remainingTimeAlarm : 1;      ///< 剩余时间不足报警
        U16 remainingCapacityAlarm : 1;  ///< 剩余电量不足报警
        U16 reserved1 : 1;               ///< 保留字段
        U16 terminateDischargeAlarm : 1; ///< 放电停止报警
        U16 overTemperatureAlarm : 1;    ///< 过温报警
        U16 isSOHGood : 1;               ///< 电池健康状态
        U16 terminateChargeAlarm : 1;    ///< 充电停止报警
        U16 overchargedAlarm : 1;        ///< 过冲报警
    };
    U16 s;                               ///< 共用体
} Ps3LibBbuGgStatus_u;

/**
 * @brief   固件状态结构体
 */
typedef union Ps3LibBbuFwStatus {
    struct {
        U32 packMissing : 1;           ///< 电池丢失
        U32 voltageLow : 1;            ///< 电压过低
        U32 temperatureHigh : 1;       ///< 过温
        U32 chargeStatus : 1;          ///< 充放电状态(0:放电, 1:充电)
        U32 learnCycleRequested : 1;   ///< 校准请求
        U32 learnCycleActive : 1;      ///< 校准中
        U32 learnCycleFailed : 1;      ///< 校准失败
        U32 learnCycleTimeout : 1;     ///< 校准超时
        U32 i2cErrorsDetected : 1;     ///< I2C通信错误
        U32 replacePack : 1;           ///< 更换电池请求
        U32 remainingCapacityLow : 1;  ///< 剩余电量低
        U32 periodicLearnRequired : 1; ///< 周期自校准请求
        U32 transparentLearn : 1;      ///< 透明自校准
        U32 noSpace : 1;               ///< 无cache offload空间
        U32 predictiveFailure : 1;     ///< 电池即将失效
        U32 premiumFeatureReqd : 1;    ///< 掉电保护状态：是否需要最优性能
        U32 microcodeUpdateReqd : 1;   ///< 模块升级需求
        U32 updateActive : 1;          ///< 模块是否正在升级
        U32 nvCacheState : 1;          ///< M.2硬盘cache offload状态
        U32 learnParamSetSupport: 1;   ///< 校准参数是否支持设置(1:支持 0:不支持)
        U32 reserved : 12;             ///< 保留字段
    };
    U32 w;                             ///< 共用体
} Ps3LibBbuFwStatus_u;

/**
 * @brief   生产日期结构体
 */
typedef union Ps3LibBbuDate {
    struct {
        U32 day : 5;      ///< 日
        U32 month : 4;    ///< 月
        U32 year : 16;    ///< 年
        U32 reserved : 7; ///< 保留字段
    };
    U32 w;                ///< 共用体
} Ps3LibBbuDate_t;

/**
 * @brief   BBU状态结构体
 */
typedef struct Ps3LibBbuStatus {
    U8                   batteryType;           ///< 电池类型
    U8                   bbuState;              ///< BBU状态
    U16                  ggType;                ///< 电量计类型
    U16                  voltage;               ///< 电压
    S16                  current;               ///< 电流
    S16                  temperature;           ///< 温度
    Ps3LibBbuBatteryState_u batteryState;       ///< 电池状态
    Ps3LibBbuGgStatus_u  ggStatus;              ///< 电量计状态
    U8                   pad[2];                ///< 保留字段
    Ps3LibBbuFwStatus_u  fwStatus;              ///< 固件状态
    Ps3LibBbuDate_t      mgrBbuDate;            ///< 生产日期
    U8                   bbuMode;               ///< BBU模式
    U8                   vpdSupported : 1;      ///< 是否支持VPD
    U8                   reserved : 7;          ///< 保留字段
    U8                   reserved1[10];         ///< 保留字段
} Ps3LibBbuStatus_t;

/**
 * @brief   BBU电量信息结构体
 */
typedef struct Ps3LibBbuCapacityInfo{
    U16     relativeStateOfCharge;      ///< 相对容量
    U16     absoluteStateOfCharge;      ///< 绝对容量
    U16     remainingCapacity;          ///< 剩余容量
    U16     fullChargeCapacity;         ///< 充满电的容量
    U16     runTimeToEmpty;             ///< 剩余可用时间（MIN）
    U16     averageTimeToEmpty;         ///< 平均放空电时间（MIN）
    U16     averageTimeToFull;          ///< 平均充满电时间（MIN）
    U16     cycleCount;                 ///< 充放电周期次数
    U16     maxError;                   ///< 最大错误数
    U16     remainingCapacityAlarm;     ///< 剩余容量报警阈值
    U16     remainingTimeAlarm;         ///< 剩余时间报警阈值
    U8      reserved[26];               ///< 保留字段
} Ps3LibBbuCapacityInfo_t;

/**
 * @brief   BBU设计信息结构体
 */
typedef struct Ps3LibBbuDesignInfo {
    U64          cacheVaultFlashSize;                          ///< Cache大小(bytes)
    Ps3LibBbuDate_t mgrBbuDate;                                ///< 生产日期
    U16          designCapacity;                               ///< 设计容量
    U16          designVoltage;                                ///< 设计电压
    U16          specificationInfo;                            ///< 规格信息
    U16          serialNumber;                                 ///< 序列号
    U16          packStatConfiguration;                        ///< 电池组配置
    U8           mfgName[PS3LIB_MFG_NAME_LEN];                 ///< 生产厂商
    U8           deviceName[PS3LIB_DEVICE_NAME_LEN];           ///< 设备名称
    U8           deviceChemistry[PS3LIB_DEVICE_CHEMISTRY_LEN]; ///< 设备化学成分
    U8           mfgData[PS3LIB_MFG_DATA_LEN];                 ///< 生产厂商二进制编码信息
    U8           bbuMode;                                      ///< BBU模式
    U8           transparentLearn;                             ///< 透明学习
    U8           moduleVersion[PS3LIB_MODULE_VERSION_LEN];     ///< 模块版本
    U8           appData;                                      ///< Populated based on BBU HW
    U8           bbuPcbAssmNo[PS3LIB_PCB_VERSION_LEN];         ///< PCB封装号
    U8           bbuPcbVersionNo[PS3LIB_PCB_VERSION_LEN];      ///< PCB版本号
    U8           bbuPackAssmNo[PS3LIB_PCB_VERSION_LEN];        ///< 电池组封装号
    U8           bbuBatVersionNo[PS3LIB_BAT_VERSION_LEN];      ///< 电池版本号
    U8           pad[3];                                       ///< 保留字段
    Ps3LibBbuDate_t bbuBatDate;                                ///< 电池/超级电容生产日期
    U16          bbuSerialNo;                                  ///< 电池/超级电容序列号
    U8           reserved[6];                                  ///< 保留字段
} Ps3LibBbuDesignInfo_t;

/**
 * @brief   BBU属性信息
 */
typedef struct Ps3LibBbuProperties {
    U64 autoLearnPeriod;         ///< 自校准周期(秒,只读,不可设置)
    U64 nextLearnTime;           ///< 下次校准时间戳
    U64 learnDelayInterval;      ///< 校准启动延时时间(小时,不超过168小时)
    U64 lastSuccessfulLearnTime; ///< 上次成功校准时间戳
    U8 learnStage;               ///< 校准阶段
    U8 autoLearnMode;            ///< 自校准模式(0:Auto, 1:Disabled)
    U8 bbuMode;                  ///< BBU模式
    U8 reserved[13];             ///< 保留字段
} Ps3LibBbuProperties_t;

/**
 * @brief   电量计睡眠状态枚举
 */
 typedef enum Ps3LibBbuPowerModes {
    PS3LIB_BBU_POWERMODE_NORMAL  = 0, ///< 电量计正常模式
    PS3LIB_BBU_POWERMODE_SLEEP = 1, ///< 电量计睡眠模式
 } Ps3LibBbuPowerModes_e;

/**
 * @brief   bbu是否在位枚举
 */
typedef enum Ps3LibBbuState {
    PS3LIB_BBU_STATE_INVALID   = 0, ///< 非法状态
    PS3LIB_BBU_STATE_PRESENT   = 1, ///< bbu在位
    PS3LIB_BBU_STATE_ABSENT    = 2, ///< bbu不在位
} Ps3LibBbuState_e;

/**
 * @brief   校准状态信息结构体
 */
typedef struct Ps3LibBbuLearnStatus {
    U64 autoLearnPeriod;           ///< 自动校准周期(秒,只读,不可设置)
    U64 lastSuccessfulLearnTime;   ///< 上次成功校准时间
    U64 nextLearnTime;             ///< 下次校准时间
    U64 learnDelayInterval;        ///< 校准启动延时(小时,不超过168小时)
    U8  learnStage;                ///< 校准阶段
    U8  autoLearnMode;             ///< 自动校准模式(0:auto, 1:Disabled, 2:Warn)
    U8  transparentLearn : 1;      ///< 透明校准
    U8  periodicLearnRequired : 1; ///< 周期校准请求
    U8  learnCycleTimeout : 1;     ///< 校准超时
    U8  learnCycleFailed : 1;      ///< 校准失败(1:失败)
    U8  learnCycleActive : 1;      ///< 正在校准
    U8  learnCycleRequested : 1;   ///< 校准请求
    U8  reserved : 2;              ///< 保留字段
    U8  reserved1[5];              ///< 保留字段
} Ps3LibBbuLearnStatus_t;

/**
 * @brief   自校准模式枚举
 */
typedef enum Ps3LibBbuAutoLearnMode {
    PS3LIB_BBU_AUTOLEARN_AUTO     = 0,   ///< 周期自校准开启
    PS3LIB_BBU_AUTOLEARN_DISABLED = 1,   ///< 周期自校准关闭
    PS3LIB_BBU_AUTOLEARN_NR
} Ps3LibBbuAotuLearnMode_e;

/**
 * @brief   电池充放电状态枚举
 */
typedef enum Ps3LibBbuChargeStatus {
    PS3LIB_BBU_BATTERY_DISCHARGING = 0, ///< 放电中
    PS3LIB_BBU_BATTERY_CHARGING    = 1, ///< 充电中
}Ps3LibBbuChargeStatus_e;

/**
 * @brief   校准阶段枚举
 */
typedef enum Ps3LibBbuLearnStage {
    PS3LIB_BBU_LEARN_NONE        = 0, ///< 非校准状态
    PS3LIB_BBU_LEARN_PRECHARGING = 1, ///< 校准准备阶段，电池充满电
    PS3LIB_BBU_LEARN_DISCHARGING = 2, ///< 放电阶段，校准中
    PS3LIB_BBU_LEARN_COMPLETE    = 3, ///< 放电到空，校准完成，转为充电
    PS3LIB_BBU_LEARN_FAILED      = 4, ///< 校准失败
    PS3LIB_BBU_LEARN_NR
} Ps3LibBbuLearnStage_e;

/**
 * @brief   电池类型枚举
 */
typedef enum Ps3LibBbuType {
    PS3LIB_BBU_TYPE_NO_BATT      = 0,
    PS3LIB_BBU_TYPE_BBU0         = 1,  ///< 锂电池 德赛 基于电量计bq28z610
    PS3LIB_BBU_TYPE_BBU1         = 2,  ///< 锂电池 康舒 基于电量计bq28z610
    PS3LIB_BBU_TYPE_SCAP0        = 3,  ///< 超级电容 中车 基于电量计bq28z610
    PS3LIB_BBU_TYPE_SCAP1        = 4,  ///< 超级电容 江海 基于电量计bq27742-g1
    PS3LIB_BBU_TYPE_NR
}Ps3LibBbuType_e;

/**
 * @brief        获取BBU状态信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   bbuStatus: BBU状态信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuStatusInfoGet(CtrlId_t ctrlId, Ps3LibBbuStatus_t *bbuStatus);

/**
 * @brief        获取BBU电量信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   bbuCapacityInfo: BBU电量信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuCapacityInfoGet(CtrlId_t ctrlId, Ps3LibBbuCapacityInfo_t *bbuCapacityInfo);

/**
 * @brief       BBU开始校准
 * @param[in]   ctrlId: 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuStartLearnSet(CtrlId_t ctrlId);

/**
 * @brief        获取BBU设计信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   bbuDesignInfo: BBU设计信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuDesignInfoGet(CtrlId_t ctrlId, Ps3LibBbuDesignInfo_t *bbuDesignInfo);

/**
 * @brief        获取BBU属性信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   bbuPropInfo: BBU属性信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuPropInfoGet(CtrlId_t ctrlId, Ps3LibBbuProperties_t *bbuPropInfo);

/**
 * @brief       设置BBU校准延长周期
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   learnDelayInterval: 校准延长周期(范围: 0-168)
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuLearnDelaySet(CtrlId_t ctrlId, U8 learnDelayInterval);

/**
 * @brief       设置BBU电量计状态
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   mode:   电量计模式
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuPowermodeSet(CtrlId_t ctrlId, Ps3LibBbuPowerModes_e mode);

/**
 * @brief       设置BBU自动校准模式
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   autoLearnMode: 自动校准模式(0:开启, 1:关闭)
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuAutoLearnModeSet(CtrlId_t ctrlId, U8 autoLearnMode);

/**
 * @brief        查询BBU是否在位
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   bbuState: bbu在位信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuStateGet(CtrlId_t ctrlId, Ps3LibBbuState_e *bbuState);

/**
 * @brief        获取BBU校准状态信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   bbuLearnsSatus: bbu校准状态信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libBbuLearnInfoGet(CtrlId_t ctrlId, Ps3LibBbuLearnStatus_t *bbuLearnsSatus);

#endif
