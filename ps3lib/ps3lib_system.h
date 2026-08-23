/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_system.h
 * @date    2023.04.25
 * @brief   系统级别数据结构与接口
 */

#ifndef __PS3LIB_SYSTEM_H__
#define __PS3LIB_SYSTEM_H__

#include "ps3lib_phy.h"

#define PS3LIB_OS_INFO_STRING_LEN   (128)           ///< 系统信息字符串长度
#define PS3LIB_VERSION_LEN          (32)            ///< ps3lib库版本
#define PS3LIB_MAX_EVENT_REG_CNT    (32)            ///< 事件日志最大注册数量
#define PS3LIB_ERRNO_LIST_MAX_COUNT (64)  ///< 错误码列表最大数量
#define PS3LIB_BMC_DRIVER_MAX_LEN   (64)  ///< BMC驱动名称最大字符长度
#define PS3LIB_BMC_ENV_I2C            "i2c"       ///< 环境变量 : i2c 通路
#define PS3LIB_BMC_ENV_MCTP_I2C       "mctp_i2c"  ///< 环境变量 : mctp over i2c 通路的
#define PS3LIB_BMC_ENV_MCTP_PCIE      "mctp_pcie" ///< 环境变量 : mctp over pcie 通路
#define PS3LIB_HOST_INFO_RESERVED_1_LEN        (54)    ///< Host Info 保留字段1 54 bytes
#define PS3LIB_HOST_INFO_RESERVED_2_LEN        (192)   ///< Host Info 保留字段2 192 bytes
#define PS3LIB_HOST_INFO_DRIVE_NAME_LEN        (128)   ///< Host Info 驱动名称 128 bytes
#define PS3LIB_HOST_INFO_DRIVE_VERSION_LEN     (128)   ///< Host Info 驱动版本 128 bytes

/**
 * @brief lib库初始化标记
 */
typedef enum Ps3LibInit{
    PS3LIB_IS_DESTORY = 0,    ///< lib库已销毁
    PS3LIB_IS_INIT = 1,       ///< lib库已初始化
    PS3LIB_INIT_NR,
} Ps3LibInit_e;

/**
 * @brief lib库初始化标记
 */
enum {
    PS3LIB_DEFAULT_TYPE     = 0b0,    ///< 默认lib库版本
    PS3LIB_RAID_HBA_TYPE    = 0b10,   ///< RAID-HBA lib库版本
    PS3LIB_EXP_TYPE         = 0b100,  ///< EXP lib库版本
    PS3LIB_SWITCH_TYPE      = 0b1000, ///< SWITCH lib库版本
};

/**
 * @brief lib库运行于OOB模式下通路类型
 */
enum {
    PS3LIB_BMC_CHANNEL_I2C  = 0x0,
    PS3LIB_BMC_CHANNEL_MCTP = 0x1,
    PS3LIB_BMC_CHANNEL_PCIE = 0x2,
    ///< 其它通路类型从此处依次追加
    PS3LIB_BMC_CHANNEL_UNKNOWN = 0xff,
};

/**
 * @brief PCI设备结构体信息
 */
typedef struct Ps3LibPciInfo {
    U16 vendorId;                   ///< PCI设备厂商ID
    U16 deviceId;                   ///< PCI设备ID
    U16 subVendorId;                ///< PCI设备子系统厂商ID
    U16 subDeviceId;                ///< PCI设备子系统设备ID
    U8  busNumber;                  ///< PCI设备bus号
    U8  deviceNumber;               ///< PCI设备device号
    U8  functionNumber;             ///< PCI设备function号
    U8  pad;                        ///< 保留字段
    U32 domainID;
    S8  pciAddr[32];                ///< PCI设备地址
} Ps3LibPciInfo_t;

/**
 * @brief 系统PCI设备结构体
 */
typedef struct Ps3LibSystemPciInfo {
    U32             hostId;     ///< 主机标识符
    Ps3LibPciInfo_t pciInfo;    ///< PCI设备结构体
} Ps3LibSystemPciInfo_t;

/**
 * @brief lib库通用系统时间结构体
 */
typedef struct Ps3LibSystemTime {
    U32 year;        ///< 年
    U32 mon;         ///< 月
    U32 day;         ///< 日
    U32 hour;        ///< 时
    U32 min;         ///< 分
    U32 sec;         ///< 秒
    U32 msec;        ///< 毫秒
} Ps3LibSystemTime_t;

/**
 * @brief  操作系统信息
 */
typedef struct Ps3LibOsInfo {
    S8 osName[PS3LIB_OS_INFO_STRING_LEN];        ///< 操作系统名字
    S8 osVersion[PS3LIB_OS_INFO_STRING_LEN];     ///< 操作系统版本
    S8 driverName[PS3LIB_OS_INFO_STRING_LEN];    ///< 驱动名字
    S8 driverVersion[PS3LIB_OS_INFO_STRING_LEN]; ///< 驱动版本
    S8 hostName[PS3LIB_OS_INFO_STRING_LEN];      ///< 主机名字
    S8 machine[PS3LIB_OS_INFO_STRING_LEN];       ///< 状态机
} Ps3LibOsInfo_t;

/**
 * @brief  系统信息
 */
typedef struct Ps3LibSystemInfo {
    U8              libVersion[PS3LIB_VERSION_LEN]; ///< lib库版本
    Ps3LibOsInfo_t  systemInfo;                     ///< 系统信息
} Ps3LibSystemInfo_t;

/**
 * @brief  主机信息
 */
typedef struct Ps3LibHostInfo {
    U64  pciBus;
    U8   pciDev;
    U8   pciFunc;
    U8   reserved1[PS3LIB_HOST_INFO_RESERVED_1_LEN];         ///< 保留字段1 54 bytes
    S8   driverName[PS3LIB_HOST_INFO_DRIVE_NAME_LEN];        ///< 驱动名称 128 bytes
    S8   driverVersion[PS3LIB_HOST_INFO_DRIVE_VERSION_LEN];  ///< 驱动版本 128 bytes
    S8   reserved2[PS3LIB_HOST_INFO_RESERVED_2_LEN];         ///< 保留字段2 192bytes
} Ps3LibHostInfo_t; ///< 512[8+1+1+54+128+128+192]Btyes

typedef struct Ps3LibErrno{
    S8        key[32];    ///< 错误码字符串索引，如"ccrate"
    Ps3Errno  value;      ///< 错误码值
} Ps3LibErrno_t;

typedef struct Ps3LibErrList{
    U8             count;      ///< 错误码列表数量
    U8             pad[3];     ///< 保留字段
    Ps3LibErrno_t  list[PS3LIB_ERRNO_LIST_MAX_COUNT];   ///< 错误码列表
} Ps3LibErrList_t;

typedef enum Ps3LibServType {
    PS3LIB_SERV_LIB    = 0,    ///< 默认值
    PS3LIB_SERV_CLI,
    PS3LIB_SERV_LXA,
    PS3LIB_SERV_BMC,
    PS3LIB_SERV_UEFI,
    PS3LIB_SERV_NTB,
    PS3LIB_SERV_FACTORY,
} Ps3LibServType_e;

typedef enum OutputType {
    OUTPUT_NORMAL,
    OUTPUT_JSON,
    OUTPUT_TREE,
    OUTPUT_DUMP,
    OUTPUT_MENU,
} OutputType_e;

typedef enum Ps3LogLevel {
    PS3_LOG_LEVEL_ALWAYS,
    PS3_LOG_LEVEL_FATAL,
    PS3_LOG_LEVEL_ERROR,
    PS3_LOG_LEVEL_WARN,
    PS3_LOG_LEVEL_INFO,
    PS3_LOG_LEVEL_DEBUG,
    PS3_LOG_LEVEL_TRACE,
} Ps3LogLevel_t;

typedef enum Ps3LogMode {
    PS3_LOG_MODE_SYNC,      ///< 将缓存中的日志下盘，并不再缓存日志
    PS3_LOG_MODE_CACHE,     ///< 将日志存入缓存中, 不下盘
} Ps3LogMode_t;

typedef enum Ps3LibOobChannelType {
    PS3LIB_OOB_CHANNEL_I2C = 0,         ///< I2C
    PS3LIB_OOB_CHANNEL_MCTP_I2C = 1,    ///< MCTP_I2C
    PS3LIB_OOB_CHANNEL_MCTP_PCIE = 2,   ///< MCTP_PCIE
    PS3LIB_OOB_CHANNEL_NR,
} Ps3LibOobChannelType_e;

typedef struct Ps3LibLogConfig {
    OutputType_e    outputType;
    int             page;
    const char *    logFile;
    Ps3LogLevel_t   logLevel;
    int             logRotateCount;
    int             logRotateSize;
    U8              logOverwrite; ///< 日志文件覆盖写，目前未实现，后续考虑删除
    U8              pad[3];
} Ps3LibLogConfig_t;

typedef struct Ps3LibOobConfig {
    S8    i2cPath[PS3LIB_BMC_DRIVER_MAX_LEN];
    S8    mctpI2cPath[PS3LIB_BMC_DRIVER_MAX_LEN];
    S8    mctpPCIePath[PS3LIB_BMC_DRIVER_MAX_LEN];
    U32   setCmdTimeout;    ///< 设置命令超时时间,单位(s)
    U32   getCmdTimeout;    ///< 获取命令超时时间,单位(s)
    U32   downloadTimeout;  ///< 升级命令超时时间,单位(s),设置为0时使用默认值
    U32   oobChannelType;   ///< oob驱动类型(enum Ps3LibOobChannelType_e)
} Ps3LibOobConfig_t;

typedef struct Ps3LibConfigFile {
    Ps3LibLogConfig_t logConfig;
    Ps3LibOobConfig_t oobConfig;
} Ps3LibConfigFile_t;

/**
 * @brief  lib库运行通路类型
 */
typedef enum Ps3LibChannelType {
    PS3LIB_CHANNEL_TYPE_UNKNOWN = 0,
    PS3LIB_CHANNEL_TYPE_IOCTL   = 1,    ///< Ioctl通路
    PS3LIB_CHANNEL_TYPE_PIPE    = 2,    ///< pipe通路
    PS3LIB_CHANNEL_TYPE_BMC     = 3,    ///< BMC通路
    PS3LIB_CHANNEL_TYPE_UEFI    = 4,    ///< UEFI通路
    PS3LIB_CHANNEL_TYPE_NR,
} Ps3LibChannelType_e;

/**
 * @brief        获取控制卡PCIe信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   pciInfo: 获取到的PCIe信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSystemPciInfoGet(CtrlId_t ctrlId, Ps3LibSystemPciInfo_t *pciInfo);

/**
 * @brief        查询系统信息
 * @param[out]   info: 系统信息
 * @note         OOB下不支持
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSystemInfoGet(CtrlId_t ctrlId, Ps3LibSystemInfo_t *info);

/**
 * @brief  填充 ctrl 的 IdList 结构
 * @param  ctrlIdList    输出参数
 * @return Ps3Errno
 * @note   仅支持带内模式
 */
 
Ps3Errno ps3libFillCtrlList(Ps3LibIdList_t *ctrlIdList);

/**
 * @brief        设置Lib库参数
 * @param[in]    config: 日志配置
 * @return       Ps3Errno
 */
Ps3Errno ps3libLibraryParamSet(Ps3LibLogConfig_t config);

/**
 * @brief        获取Lib库参数
 * @param[in]    config: 日志配置
 * @return       Ps3Errno
 */
Ps3Errno ps3libLibraryParamGet(Ps3LibLogConfig_t *config);

/**
 * @brief        配置文件初始化
 * @param[in]    config: 日志配置
 * @return       Ps3Errno
 */
Ps3Errno ps3libConfigFileInit(Ps3LibConfigFile_t *config);

/**
 * @brief        配置初始化
 * @param[in]    config: 日志配置
 * @return       Ps3Errno
 */
void ps3libLibraryParamInit(Ps3LibConfigFile_t *config);

/**
 * @brief    仅供test独立编译日志模块使用
 */
void ps3libLogF(Ps3LogLevel_t level, const char *file, int line, const char *func, const char *format, ...);

/**
 * @brief      获取当前lib库运行模式
 * @param[out] ChannelType: 当前lib库运行模式
 * @return     Ps3Errno
 */
U8 ps3libChannelTypeGet();

/**
 * @brief        获取主机信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[out]   hostInfo: 主机信息
 * @return       Ps3Errno
 */
Ps3Errno ps3libHostInfoGet(CtrlId_t ctrlId, Ps3LibHostInfo_t *hostInfo);

/**
 * @brief      设置当前lib库日志模式
 * @param[in] mode: 当前lib库日志模式
 */
void ps3libLogModeSet(Ps3LogMode_t mode);

/**
 * @brief      获取当前lib库日志模式
 * @param[out] mode: 当前lib库日志模式
 */
void ps3libLogModeGet(Ps3LogMode_t* mode);

#endif
