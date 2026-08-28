/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_update.h
 * @date    2023.04.25
 * @brief   升级信息
 */
#ifndef __PS3LIB_UPDATE_H__
#define __PS3LIB_UPDATE_H__

#if defined(__cplusplus)
extern "C" {
#endif

#define PS3LIB_FILE_PATH_NAME              (1024)              ///< 文件路径字符长度

/**
 * @brief   升级固件类型
 */
typedef enum Ps3LibUpdateFwType{
    PS3LIB_UPDATE_FWTYPE_INVALID               = 0,

    ///< 单分区操作枚举
    PS3LIB_UPDATE_FWTYPE_NORMAL_BASE        = 1, ///< 不区分raid/hba的基
    PS3LIB_UPDATE_FWTYPE_UBOOT,                ///< uboot/bootloader
    PS3LIB_UPDATE_FWTYPE_UBOOT_BACKUP,         ///< uboot/bootloader 备份
    PS3LIB_UPDATE_FWTYPE_FW,                   ///< firmware
    PS3LIB_UPDATE_FWTYPE_FW_BACKUP,            ///< firmware 备份
    PS3LIB_UPDATE_FWTYPE_NVDATA,               ///< NVDATA
    PS3LIB_UPDATE_FWTYPE_NVDATA_BACKUP,        ///< NVDATA 备份
    PS3LIB_UPDATE_FWTYPE_OPTIONROM,            ///< OptionROM
    PS3LIB_UPDATE_FWTYPE_OPTIONROM_BACKUP,     ///< OptionROM 备份
    PS3LIB_UPDATE_FWTYPE_CPLD_UPD,             ///< CPLD UPD         ///< 不支持升级
    PS3LIB_UPDATE_FWTYPE_EVTLOG,               ///< EventLog         ///< 不支持升级
    PS3LIB_UPDATE_FWTYPE_EVTLOG_VITAL,         ///< EventLog 重要日志    ///< 不支持升级
    PS3LIB_UPDATE_FWTYPE_MCU_SAS,              ///< backend:SAS MCU
    PS3LIB_UPDATE_FWTYPE_MCU_SAS_BACKUP,       ///< backend:SAS MCU 备份
    PS3LIB_UPDATE_FWTYPE_USER_DEFAULT,         ///< 制造商数据
    PS3LIB_UPDATE_FWTYPE_USER_DEFAULT_BACKUP,  ///< 制造商数据 备份
    PS3LIB_UPDATE_FWTYPE_NORMAL_NR  = 17,

    ///< RAID单分区操作枚举
    PS3LIB_UPDATE_FWTYPE_RAID_BASE,            ///< 暂定RAID公用类型的基
    PS3LIB_UPDATE_FWTYPE_OS,                   ///< OS镜像(仅RAID)
    PS3LIB_UPDATE_FWTYPE_OS_BACKUP,            ///< OS镜像 备份(仅RAID)
    PS3LIB_UPDATE_FWTYPE_RAMDISK,              ///< OS文件系统(仅RAID)
    PS3LIB_UPDATE_FWTYPE_RAMDISK_BACKUP,       ///< OS文件系统 备份(仅RAID)
    PS3LIB_UPDATE_FWTYPE_DTB,                  ///< 设备树(仅RAID)
    PS3LIB_UPDATE_FWTYPE_DTB_BACKUP,           ///< 设备树 备份(仅RAID)
    PS3LIB_UPDATE_FWTYPE_MCU_APU0,             ///< frontend:APU0 MCU(仅RAID)
    PS3LIB_UPDATE_FWTYPE_MCU_APU0_BACKUP,      ///< frontend:APU0 MCU 备份(仅RAID)
    PS3LIB_UPDATE_FWTYPE_MCU_APU1,             ///< frontend:APU1 MCU(仅RAID)
    PS3LIB_UPDATE_FWTYPE_MCU_APU1_BACKUP,      ///< frontend:APU1 MCU 备份(仅RAID)
    PS3LIB_UPDATE_FWTYPE_RAID_NR = 29,

    ///< HBA单分区操作枚举
    PS3LIB_UPDATE_FWTYPE_HBA_BASE,             ///< 暂定HBA公用类型的基
    PS3LIB_UPDATE_FWTYPE_COREDUMP,             ///< coredump(仅HBA)    ///< 不支持升级
    PS3LIB_UPDATE_FWTYPE_TERMLOG,              ///< TermLog0(仅HBA)    ///< 不支持升级
    PS3LIB_UPDATE_FWTYPE_TERMLOG1,             ///< TermLog1(仅HBA)    ///< 不支持升级
    PS3LIB_UPDATE_FWTYPE_HBA_NR = 34,

    PS3LIB_UPDATE_FWTYPE_OTHER_BASE,           ///< 暂定其他类型的基
    PS3LIB_UPDATE_FWTYPE_FW_PACKAGE,           ///< 固件大包
    PS3LIB_UPDATE_FWTYPE_BOOTROM,              ///< preos
    PS3LIB_UPDATE_FWTYPE_ATF,                  ///< ATF
    PS3LIB_UPDATE_FWTYPE_ATF_BACKUP,           ///< ATF备份
    PS3LIB_UPDATE_FWTYPE_PATCH,                ///< PATCH
    PS3LIB_UPDATE_FWTYPE_PATCH_BACKUP,         ///< PATCH备份
    PS3LIB_UPDATE_FWTYPE_SBR,                  ///< SBR
    PS3LIB_UPDATE_FWTYPE_SBR_BACKUP,           ///< SBR备份
    PS3LIB_UPDATE_FWTYPE_SPL,                  ///< SPL
    PS3LIB_UPDATE_FWTYPE_SPL_BACKUP,           ///< SPL备份
    PS3LIB_UPDATE_FWTYPE_OEM,                  ///< OEM
    PS3LIB_UPDATE_FWTYPE_SPD,                  ///< SPD
    PS3LIB_UPDATE_FWTYPE_VPD,                  ///< VPD
    PS3LIB_UPDATE_FWTYPE_BIOS,                 ///< BIOS
    PS3LIB_UPDATE_FWTYPE_MPB,                  ///< MPB
    PS3LIB_UPDATE_FWTYPE_FLASH,                ///< FLASH
    PS3LIB_UPDATE_FWTYPE_CONFIG,               ///< 配置信息
    PS3LIB_UPDATE_FWTYPE_FW_PACKAGE_BACKUP,    ///< 备份分区固件大包
    PS3LIB_UPDATE_FWTYPE_BBU_VPD,              ///< bbu vpd 54
    PS3LIB_UPDATE_FWTYPE_BOOTROM_BACKUP,       ///< preos备份
    PS3LIB_UPDATE_FWTYPE_PUBLICKEY,            ///< publickey
    PS3LIB_UPDATE_FWTYPE_DIAGTOOL,             ///< diagtool

    PS3LIB_UPDATE_FWTYPE_SBR1,                 ///< SBR1 
    PS3LIB_UPDATE_FWTYPE_SBR2,                 ///< SBR2 
    PS3LIB_UPDATE_FWTYPE_SBR3,                 ///< SBR3 
    
    PS3LIB_UPDATE_FWTYPE_OTHER_NR = 61,

    PS3LIB_UPDATE_FWTYPE_ALL,
    PS3LIB_UPDATE_FWTYPE_MAX
}Ps3LibUpdateFwType_e;

/**
 * @brief   固件升级设备类型
 */
typedef enum Ps3LibUpdateDevType {
    PS3LIB_UPDATE_DEVTYPE_INVALID        = 0,
    PS3LIB_UPDATE_DEVTYPE_CONTROLLER    = 1,   ///< 控制器
    PS3LIB_UPDATE_DEVTYPE_EXPANDER      = 2,   ///< expander
    PS3LIB_UPDATE_DEVTYPE_PCIESWITCH    = 3,   ///< pcie switch
    PS3LIB_UPDATE_DEVTYPE_PD            = 4,   ///< 硬盘
    PS3LIB_UPDATE_DEVTYPE_BBU           = 5,   ///< BBU
    PS3LIB_UPDATE_DEVTYPE_CPLD          = 6,   ///< CPLD
    PS3LIB_UPDATE_DEVTYPE_SBR           = 7,   ///< EEPROM-SBR
    PS3LIB_UPDATE_DEVTYPE_SSD           = 8,   ///< BGA SSD
    PS3LIB_UPDATE_DEVTYPE_SPD           = 9,   ///< EEPROM-SPD
    PS3LIB_UPDATE_DEVTYPE_FILE          = 10,  ///< 文件
    PS3LIB_UPDATE_DEVTYPE_SPL           = 11,  ///< 负责装载操作系统到RAM中
    PS3LIB_UPDATE_DEVTYPE_PSOC          = 12,  ///< PSOC
    PS3LIB_UPDATE_DEVTYPE_BIOS          = 13,  ///< BIOS
    PS3LIB_UPDATE_DEVTYPE_PKEY          = 14,  ///< 固件分区
    PS3LIB_UPDATE_DEVTYPE_CONFIG        = 15,  ///< 配置
    PS3LIB_UPDATE_DEVTYPE_DIAGTOOL      = 16,  ///< diagtool
    PS3LIB_UPDATE_DEVTYPE_MAX,
} Ps3LibUpdateDevType_e;

/**
 * @brief   固件升级open阶段参数结构体
 */
typedef struct Ps3LibUpdateOpenCmd {
    U32 devType;              ///<  设备类型 PS3LIB_UPDATE_DEVTYPE_MAX
    U32 fwType;               ///<  固件类型 PS3LIB_UPDATE_FWTYPE_MAX
    S8  fileName[PS3LIB_FILE_PATH_NAME];        ///<  文件名称
    U32 encID;                ///<  enclosure号
    U32 slotID;               ///<  槽位号
    U64 UUID;                 ///<  主机id +pid +主机消  序列号=UUID用户唯一
    U32 fragNum;              ///<  分片数量
    U32 fragLen;              ///<  分片长度
    U32 fwLen;                ///<  固件总长度
    U32 noSignChk : 1;        ///<  nosigncheck
    U32 noVerChk  : 1;        ///<  noversion
    U32 force     : 1;        ///<  是否强制
    U32 all       : 1;        ///<  全部烧写或全部擦除标记
    U32 backUp    : 1;        ///<  是否需要备份
    U32 isFwhead  : 1;        ///<  是否需要片外构建fwhead(1:需要; 0:不需要)
    U32 noReset   : 1;        ///<  不需要重启
    U32 forceHcb  : 1;        ///<  磁盘是否强制执行
    U32 resetNow  : 1;        ///<  是否立即重启
    U32 forceClose: 1;        ///<  强制执行是否关闭
    U32 isPacket  : 1;        ///<  是否打包
    U32 innerPad1 : 1;        ///<  内部预留使用,非用户预留
    U32 reserved  : 20;
} Ps3LibUpdateOpenCmd_t;

/**
 * @brief   固件升级
 */
typedef struct Ps3LibDownloadInfo {
    U8 *pData;                                                            ///< 数据指针
    S8 *fileName;                                                         ///< not support
    void (*progressFunc)(void *arg, const S8 *name, S64 done, S64 total, const S8 *left); ///< 进度回调
    S8  *fileData;                                                        ///<  文件内容
    S32 fileSize;                                                         ///<  文件长度
    void *arg;                                                            ///< 私有数据
} Ps3LibDownloadInfo_t;

/**
 * @brief       Expander 带外升级命令
 * @param[in]   devId: 控制卡标识符
 * @param[in]   downloadInfo: 固件升级结构体
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libEpUpdate(CtrlId_t devId, Ps3LibDownloadInfo_t *downloadInfo);

/**
 * @brief       Switch 带外升级命令
 * @param[in]   devId: 控制卡标识符
 * @param[in]   downloadInfo: 固件升级结构体
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSWUpdate(CtrlId_t devId, Ps3LibDownloadInfo_t *downloadInfo);

/**
 * @brief      Ctrl 升级命令
 * @param[in]  ctrlId: 控制卡标识符
 * @param[in]  downloadInfo: 用户填充的升级信息
 * @note       升级流程为不可中断流程, 启动后便不再响应非本流程功能
 * @note       升级接口执行结果返回后, 则恢复响应其他功能
 * @warning    若强行中断升级流程, 存在不可预期风险
 * @warning    强行中断后, 若其他接口无法正常交互, 需调用ps3libThreadAbortProcess()结束不可中断流程
 * @return     PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCtrlUpdateDownLoad(CtrlId_t ctrlId, Ps3LibDownloadInfo_t *downloadInfo);

#if defined(__cplusplus)
}
#endif

#endif
