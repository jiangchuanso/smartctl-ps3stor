/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_passthru.h
 * @date    2023.07.08
 * @brief   直通命令数据接口及接口
 */

#ifndef __PS3LIB_PASSTHRU_H__
#define __PS3LIB_PASSTHRU_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "ps3lib_smp.h"

#define PS3LIB_NVME_MAX_MPT_ERROR_REPLY     48      ///< nvme 厂商自定义数据长度
#define PS3LIB_NVME_MAX_ERR_RSP_LENGTH      32      ///< nvme cq长度 预留一部分
#define PS3LIB_NVME_MAX_CMD_LENGTH          128     ///< nvme sq长度 预留一部分

#define PS3LIB_NVME_SUBMISSIONQ_IO          0       ///< nvme io命令
#define PS3LIB_NVME_SUBMISSIONQ_ADMIN       1       ///< nvme 管理命令

#define PS3LIB_DWORD_LEN                    4   ///< dword长度 4字节
#define PS3LIB_SCSI_MAX_SENSE_LENGTH        32  ///< scsi sense data 最大长度
#define PS3LIB_SCSI_MAX_CDB_LENGTH          16  ///< scsi cdb 最大长度

/**
 * @brief   scsi直通命令目标类型
 */
typedef enum Ps3LibSCSICmdType {
    PS3LIB_CMD_OP_VD_SCSI     = 0,         ///< issue a SCSI CDB to a logical drive
    PS3LIB_CMD_OP_PD_SCSI     = 1,         ///< issue a SCSI CDB to a physical drive
} Ps3LibSCSICmdType_e;

/**
 * @brief   transfer direction flags
 */
typedef enum Ps3LibDir {
    PS3LIB_DIR_NONE        = 0,            ///< no data transfer
    PS3LIB_DIR_WRITE       = 1,            ///< transfer is from host
    PS3LIB_DIR_READ        = 2,            ///< transfer is to host
    PS3LIB_DIR_BOTH        = 3,            ///< transfer is both read and write, or unknown
} Ps3LibDir_e;

/**
 * @brief   scsi直通命令结构体
 */
typedef struct Ps3LibSCSIPassthru {
    U8      cmdType;                        ///< 命令类型 目标为物理盘还是虚拟盘 (enum Ps3LibSCSICmdType_e)
    U8      scsiStatus;                     ///< 返回的SCSI命令状态
    U8      cdbLength;                      ///< SCSI命令的CDB长度
    U8      cmdDir;                         ///< ID方向 enum Ps3LibDir_e
    U8      reserved[4];                    ///< 保留字段
    union {
        struct {
            U8      enclId;                 ///< 机框号
            U8      pad;
            U16     slotId;                 ///< 槽位号
        } target;
        U16     vdId;                       ///< 虚拟盘id
    };                                      ///< 4字节
    U8      cdb[PS3LIB_SCSI_MAX_CDB_LENGTH];    ///< SCSI Command Descriptor Block
    U8      pRequestSenseData[PS3LIB_SCSI_MAX_SENSE_LENGTH];  ///< 返回的sense data
    U32     dataSize;               ///< SCSI命令传输的数据长度
    U8      data[0];                ///< SCSI命令传输的数据 柔性数组 由调用者申请内存
} Ps3LibSCSIPassthru_t;

/**
 * @brief   nvme 直通命令请求结构体
 */
typedef struct Ps3LibNVMeEncapsulationReq {
    U16     targetId;               ///< target Identifier
    U8      dir;                    ///< dir of the command  (Ps3libDir_e)
    U8      submissionQueueType;    ///< 0 - I/O Submission Queue, 1 - Admin Submission Queue
    U8      reserved[12];
    U16     timeout;                ///< timeout in secs
    U16     encapsulatedCmdLength;  ///< submission queue length
    U8      encapsulatedNVMeCmd[PS3LIB_NVME_MAX_CMD_LENGTH];    ///< submission queue
} Ps3LibNVMeEncapsulationReq_t;

/**
 * @brief   nvme 直通命令回复结构体
 */
typedef struct Ps3LibNVMeEncapsulationRsp {
    U8      errorResponseData[PS3LIB_NVME_MAX_ERR_RSP_LENGTH];  ///< completion queue
    U16     errorResponseCount;     ///< completion queue length
    U8      reserved[6];
    U8      encapsulatedMPTErrorResponse[PS3LIB_NVME_MAX_MPT_ERROR_REPLY];  ///< 厂商自定义返回信息 保留字段
} Ps3LibNVMeEncapsulationRsp_t;

/**
 * @brief   SMP命令执行状态
 */
typedef enum Ps3LibSMPStat {
    PS3LIB_SMP_STAT_OK = 0x0,            ///< 执行成功
    PS3LIB_SMP_STAT_UNKNOWN,             ///< 默认执行失败
    PS3LIB_SMP_STAT_DEV_NOT_FOUND,       ///< disk 不在，已报out，返回失败
    PS3LIB_SMP_STAT_ILLEGAL,             ///< 命令非法，返回失败
    PS3LIB_SMP_STAT_MEDIUM_ERROR,        ///< 硬盘介质错误，从其他盘读/写，在pRespIU中看LBA
    PS3LIB_SMP_STAT_INTERNAL_HARD_ERROR, ///< HBA卡内部硬件故障，返回失败
    PS3LIB_SMP_STAT_DEV_HARD_ERROR,      ///< 硬盘硬件错误，从其他盘读/写
    PS3LIB_SMP_STAT_BUSY,                ///< 卡内因为资源等问题导致的busy,需要重试
    PS3LIB_SMP_STAT_LINK_ERR,            ///< 硬件链路故障，建议从其他盘读/写
    PS3LIB_SMP_STAT_DISK_BUSY,           ///< 盘busy 需要重试或者踢盘
    PS3LIB_SMP_STAT_UNDERRUN,            ///< 数据少于命令指定数据的长度，需要业务模块判断是否成功，需要带回transLen
    PS3LIB_SMP_STAT_OVERRUN,             ///< 数据多于命令指定数据的长度，表明失败，建议有限次重试
    PS3LIB_SMP_STAT_DIF_GRD_ERROR,       ///< 数据一致性校验GRD失败
    PS3LIB_SMP_STAT_DIF_REF_ERROR,       ///< 数据一致性校验REF失败
    PS3LIB_SMP_STAT_DIF_APP_ERROR,       ///< 数据一致性校验APP失败
    PS3LIB_SMP_STAT_NR,
} Ps3LibSMPStat_e;

/**
 * @brief   SMP直通命令请求数据结构
 */
typedef struct Ps3LibSMPPassthruReq {
    U64         sasAddr;            ///< device SAS address
    U16         timeout;            ///< command timeout (in seconds)
    U8          sasPortNum;         ///< 保留字段 暂不支持
    U8          reserved[5];        ///< 保留字段
    U8          reservedEx[2];      ///< 保留字段
    U16         requestLen;         ///< length of the request buffer
    U32         responseLen;        ///< length of the response buffer
    U8          requestBuffer[0];   ///< data buffer of dynamic size to hold SMP request buffer
} Ps3LibSMPPassthruReq_t;

/**
 * @brief   SMP直通命令回复数据结构
 */
typedef struct Ps3LibSMPPassthruRsp {
    U8          status;                 ///< SMP命令状态 (Ps3LibSMPStat_e)
    U8          reserved[7];            ///< 保留字段
    U8          responseBuffer[0];      ///< data buffer of dynamic size to hold SMP response buffer
} Ps3LibSMPPassthruRsp_t;

/**
 * @brief   SCSI ATA PASSTHROUGH Identify Request Struct
 */
typedef struct Ps3LibATAPassThru16 {
    U8 scsiOpc;
    union {
        U8 option[2];
        struct {
            U8 extend : 1; ///< byte 1
            U8 protocol : 4;
            U8 rsvd2 : 3;
            U8 t_length : 2; ///< byte 2
            U8 byte_block : 1;
            U8 t_dir : 1;
            U8 t_type : 1;
            U8 ck_cond : 1;
            U8 off_line : 2;
        };
    };
    U8 feature1;    ///< byte 3, feature 15:0
    U8 feature0;
    U8 count1;      ///< byte 5, count 15:0
    U8 count0;
    U8 lba3;        ///< byte 7, LBA 47:0
    U8 lba0;
    U8 lba4;
    U8 lba1;
    U8 lba5;
    U8 lba2;
    U8 device;  ///< byte 13, device
    U8 sataCmd; ///< byte 14, command
    U8 ctrl;    ///< Bit2 NACA bit =1, return CHECK CONDITION status
                ///< sense key = ILLEGAL REQUEST and ASC = INVALID FIELD IN CDB
} __attribute__((packed)) Ps3LibATAPassThru16_s;

/**
 * @brief   硬盘上的log E4 信息
 */
typedef struct Ps3LibPdLogE4Info {
    U16  revisonId;
    S8   signature[5];
    U8   type;
    U8   healthLevel;
    ///< Device  Info
    U8   plpCapacitance;
    U16  plpCapacitorVolt;
    U32  ddrCeErrCount;
    U32  ddrUceErrCount;
    U32  sramCeErrCount;
    U32  sramUceErrCount;
    U32  sensorErrCount;
    ///< Temperature Info
    U8   compositeTemp;
    U8   unc;
    U16  uncCnt;
    U8   uc;
    U16  ucCnt;
    U8   minTemp;
    U8   maxTemp;
    U8   socTemp;
    U8   nandTemp;
    ///< Media Info 
    U32  eraseErrCnt;
    U32  programErrCnt;
    U32  ardErrGoodCnt;
    U32  mrdErrGoodCnt;
    U32  totalBadBlkCnt;
    U32  nandMediaErrCnt;
    U32  ardErrFailCnt;
    U32  mrdErrFailCnt;
    U32  raidFailCnt;
    U32  raidGoodCnt;
    U32  reallocSectorCnt;
    U32  curPendingSectorCnt;
    ///< Inf  Info
    U32  sataPhyErrCnt;
    U64  fisCrcErrCnt;
    U32  linkSpeedDownGrade;
    ///< Software Info
    U32  unsafeShutdownCnt;
    U32  availableSpareCnt;
    U32  highLatRcmdttlCnt;
    U32  highLatWcmdttlCnt;
    U32  fatalRst;
    U32  readOnlyMode;
    U32  hsCrcErrCnt;
    ///< Reerved Info
    U32  powerOnHours;
    U32  powerCycleCnt;
    U32  lifetimeUsed;
} __attribute__((packed)) Ps3LibPdLogE4Info_t;

/**
 * @brief   硬盘上的log E5 信息
 */
typedef struct Ps3LibPdLogE5LevelInfo {
    U8  healthLevel;
    ///< Device Info
    U8  plpCapErrLevel; ///<  PLP电容健康 error level
    U8  plpCapVoltErrLevel; ///< PLP电容电压 error level
    U8  ddrCeErrLevel;
    U8  ddrUceErrLevel;
    U8  sramCeErrLevel;
    U8  sramUceErrLevel;
    U8  sensorErrLevel;
    ///< Temperature Info
    U8  compTempErrLevel;    ///< 综合温度 error level
    U8  criticalCompErrLevel;///< 综合温度超出值的次数
    U8  warningCompErrLevel;
    U8  minTempErrLevel;
    U8  maxTempErrLevel;
    U8  socTempErrLevel;
    U8  nandTempErrLevel;
    ///< Media    Info
    U8  eraseErrLevel;
    U8  programErrLevel;
    U8  ardErrGoodLevel;
    U8  mrdErrGoodLevel;
    U8  totalBadBlkLevel;
    U8  nandMediaErrLevel;
    U8  ardErrFailLevel;
    U8  mrdErrFailLevel;
    U8  raidFailLevel;
    U8  raidGoodLevel;
    U8  reallocSectorLevel;
    U8  curPendingSectorLevel;
    ///< Inf Info
    U8  sataPhyErrLevel;
    U8  fisCrcErrLevel;
    U8  linkSpeedDownGradeLv; ///< 降级 level
    ///< Software Info
    U8  unsafeShutdownLv;
    U8  availableSpareLv;
    U8  highLatRcmdttlLv;
    U8  highLatwcmdttlLv;
    U8  fatalRstLevel;
    U8  readOnlyModeLv;
    U8  hsCrcErrLevel;
    U8  norCritialLogFullErrLv;
    U8  numErrLogEntryErrLv;
    ///< Reerved Info
    U8  powerOnHoursErrLevel; ///< 仅监控
    U8  powerCycleErrLevel;   ///< 仅监控
    U8  lifetimeUsedErrlevel; ///< 95 为等级4,99 为level 2,100为Lv1
} __attribute__((packed)) Ps3LibPdLogE5LevelInfo_t; 

/**
 * @brief           对指定物理盘下发scsi直通命令
 * @param[in]       ctrlId: 控制卡标识符
 * @param[in/out]   pScsiPassthru: 指向scsi直通命令结构体的指针
 * @return          PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSCSIPassthru(CtrlId_t ctrlId, Ps3LibSCSIPassthru_t *pScsiPassthru);

/**
 * @brief       nvme 直通命令
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   pNVMeReq: nvme请求信息
 * @param[out]  pNVMeRsp: nvme回复信息
 * @param[in]   pData: nvme传输的数据
 * @param[in]   dataLen: nvme传输的数据长度
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libNVMePassthru(CtrlId_t ctrlId, Ps3LibNVMeEncapsulationReq_t *pNVMeReq,
        Ps3LibNVMeEncapsulationRsp_t *pNVMeRsp, void *pData, U32 dataLen);

/**
 * @brief       smp直通命令
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   pSmpReq: smp请求信息
 * @param[in]   smpReqSize: smp请求数据大小
 * @param[out]  pSmpRsp: smp回复信息
 * @param[out]  smpRspSize: smp回复信息buffer大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSMPPassthru(CtrlId_t ctrlId, Ps3LibSMPPassthruReq_t *pSmpReq, U32 smpReqSize,
        Ps3LibSMPPassthruRsp_t *pSmpRsp, U32 smpRspSize);

/**
 * @brief       获取PD的log E4信息
 * @param[in]   ctrlId:    控制卡标识符
 * @param[in]   deviceId:  物理盘Id
 * @param[in]   pE4log: 获取PD的log E4信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdLogE4Get(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdLogE4Info_t *pE4log);

/**
 * @brief       获取PD的log E5信息
 * @param[in]   ctrlId:    控制卡标识符
 * @param[in]   deviceId:  物理盘Id
 * @param[in]   pE5log: 获取PD的log E5信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdLogE5Get(CtrlId_t ctrlId, U16 deviceId, Ps3LibPdLogE5LevelInfo_t *pE5log);

#if defined(__cplusplus)
}
#endif

#endif
