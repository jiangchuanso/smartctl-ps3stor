/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_dg.h
 * @date    2023.05.26
 * @brief   dg信息
 */

#ifndef __PS3LIB_DG_H__
#define __PS3LIB_DG_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "ps3lib_pd.h"

#define PS3LIB_WWN_LEN                (24)      ///< WWN长度
#define PS3LIB_SPAN_PER_DG            (8)       ///< 每个dg中的最大span个数
#define PS3LIB_MAX_DEDICATED_SPARE_PER_DG (8)   ///< dg中的最大局部热备盘数量
#define PS3LIB_MAX_VD_PER_DG_RAID     (16)      ///< dg中的最大vd数
#define PS3LIB_MAX_VD_PER_DG          PS3LIB_MAX_VD_PER_DG_RAID
#define PS3LIB_MAX_PD_PER_ARRAY       (32)      ///< 每个array中的最大pd数
#define PS3LIB_MAX_FOREIGN_DG_PER_CTLR  (258)   ///< 每个控制卡上的最大外部dg数
#define PS3LIB_FOREIGN_SECURITY_KEY_LEN (32)    ///< foreign盘密钥长度
#define PS3LIB_VD_NAME_LEN              (16)    ///< vd名长度 15加上1个'\0'
#define PS3LIB_MAX_PD_PER_DG            PS3LIB_MAX_PD_PER_VD_RAID
#define PS3LIB_MAX_PD_PER_VD_RAID       (256 + 2)  ///< 现在不支持超过4K TODO
#define PS3LIB_MAX_DG_NUM              (256)    ///< 最大dg数量
#define PS3LIB_MAX_GLOBAL_SPARE_RAID   (128)    ///< 最大全局热备数量
#define PS3LIB_MAX_VD_PER_FOREIGN_DG_RAID (PS3LIB_MAX_VD_PER_DG_RAID * 2)

/**
 * @brief   migrate方式
 */
typedef enum Ps3LibMigrateType {
    PS3LIB_MIGR_RAID_LEVEL_CHANGE    = 0,       ///< 只改变raid级别
    PS3LIB_MIGR_PD_NUM_ADD           = 1,       ///< 加盘
    PS3LIB_MIGR_PD_NUM_REMOVE        = 2,       ///< 减盘
} Ps3LibMigrateType_e;

/**
 * @brief   dg允许的操作
 */
typedef struct Ps3LibDgAllowedOps{
    U32     addVirtualDrive                     : 1;    ///< 增加vd
    U32     dgDel                               : 1;    ///< 删除DG
    U32     dgModifyDriveGroup                  : 1;    ///< 迁移DG
    U32     hideAllVirtualDrives                : 1;    ///< 是否支持隐藏所有的vd
    U32     unHideAllVirtualDrives              : 1;    ///< 是否支持不隐藏所有的vd
    U32     supMigrateRaid0                     : 1;    ///< 是否支持迁移到Raid0
    U32     supMigrateRaid1                     : 1;    ///< 是否支持迁移到Raid1
    U32     supMigrateRaid5                     : 1;    ///< 是否支持迁移到Raid5
    U32     supMigrateRaid6                     : 1;    ///< 是否支持迁移到Raid6
    U32     supMigrateRaid00                    : 1;    ///< 是否支持迁移到Raid00
    U32     supMigrateRaid10                    : 1;    ///< 是否支持迁移到Raid10
    U32     supMigrateRaid1E                    : 1;    ///< 是否支持迁移到Raid1E
    U32     supMigrateRaid50                    : 1;    ///< 是否支持迁移到Raid50
    U32     supMigrateRaid60                    : 1;    ///< 是否支持迁移到Raid60
    U32     supUnmap                            : 1;    ///< 是否支持Unmap
    U32     pad1                                : 17;   ///< 字段对齐
    ///< 以下字段为上面各个字段的有效性字段，只有对应的有效性字段为true时，才能根据该字段的值判断操作是否支持
    U32     addVirtualDriveValid                : 1;    ///< 增加vd
    U32     dgDelValid                          : 1;    ///< 删除DG
    U32     dgModifyDriveGroupValid             : 1;    ///< 迁移DG
    U32     hideAllVirtualDrivesValid           : 1;    ///< 是否支持隐藏所有的vd
    U32     unHideAllVirtualDrivesValid         : 1;    ///< 是否支持不隐藏所有的vd
    U32     supMigrateRaid0Valid                : 1;    ///< 是否支持迁移到Raid0
    U32     supMigrateRaid1Valid                : 1;    ///< 是否支持迁移到Raid1
    U32     supMigrateRaid5Valid                : 1;    ///< 是否支持迁移到Raid5
    U32     supMigrateRaid6Valid                : 1;    ///< 是否支持迁移到Raid6
    U32     supMigrateRaid00Valid               : 1;    ///< 是否支持迁移到Raid00
    U32     supMigrateRaid10Valid               : 1;    ///< 是否支持迁移到Raid10
    U32     supMigrateRaid1EValid               : 1;    ///< 是否支持迁移到Raid1E
    U32     supMigrateRaid50Valid               : 1;    ///< 是否支持迁移到Raid50
    U32     supMigrateRaid60Valid               : 1;    ///< 是否支持迁移到Raid60
    U32     supUnmapValid                       : 1;    ///< 是否支持Unmap
    U32     pad2                                : 17;   ///< 字段对齐
} Ps3LibDgAllowedOps_t;

/**
 * @brief   span信息
 */
typedef struct Ps3LibSpanInfo{
    U16         spanIdx;            ///< span序号
    U16         dgId;               ///< array reference (index)
    U8          raidLevel;          ///< raid级别
    U8          spanState;          ///< span状态
    U8          isBackTaskActive;   ///< 是否有后台任务正在执行,后台任务模块表示,VD在执行后台任务时,所有span都执行
    U8          piEnable;           ///< 保留字段
    U8          sedEnable;          ///< 保留字段
    U8          psPolicy;           ///< 保留字段
    U8          isTransportReady;   ///< 保留字段
    U8          pad[5];             ///< 保留字段
    U64         sectorNum;          ///< span大小(单位sector)
    U16         pdCount;            ///< 物理盘个数,根据span index和row index来获取raidDisk
    U8          reserved[6];
}Ps3LibSpanInfo_t;     ///< 32Bytes

/**
 * @brief   盘信息结构体
 */
typedef struct Ps3LibRaidDisk{
    U16         enclosureId;        ///< 背板号
    U16         slotId;             ///< 槽位号
    U8          state;              ///< 盘状态
    U8          pfaFlag;            ///< 预失败标记
    U8          pad[2];             ///< 保留字段
    U64         configurableSector; ///< totalSize - 128MB
    U16         pdFlatId;           ///< device ID
    U8          isBackTaskActive;   ///< PD是否正在执行PD后台任务
    U8          piEnable;           ///< pi是否支持
    U8          sedEnable;          ///< 保留字段
    U8          PowerSavingOption;  ///< 保留字段
    U8          isTransportReady;   ///< 保留字段
    U8          pad2[9];
}Ps3LibRaidDisk_t;

typedef struct Ps3LibVdBrifeInfo {
    U64             vdSize;                     ///< vd大小, 单位512B
    U8              vdName[PS3LIB_VD_NAME_LEN]; ///< vd名称(字符串可能不包含终止符'\0')
    U16             vdId;                       ///< vdId
    U8              pad[2];
    U32             attr;                       ///< 保存meta中的vd属性
    U8              pad1[8];
}Ps3LibVdBrifeInfo_t;

typedef struct Ps3LibCfgfileVdInfo{
    U8              readPolicy;    ///< mgrRcmCachePolicy2Cli
    U8              writePolicy;   ///< mgrRcmCachePolicy2Cli
    U8              ioPolicy;      ///< mgrRcmCachePolicy2Cli
    U8              accessPolicy;  ///< mgrRcmCachePolicy2Cli
    U8              unmapFlag;     ///< getVirtualDiskInfo
    U8              pad[3];        ///< 保留字段
    U32             stripSize;     ///< getVirtualDiskInfo
    U32             sectorSize;    ///< getVirtualDiskInfo
    U32             createTime;    ///< getVirtualDiskInfo
    U8              emulationType; ///< getVirtualDiskInfo
    U8              forceFlag;     ///< 元数据暂时没用该参数,确定需求
    U8              cbSize;        ///< getVirtualDiskInfo
    U8              cbMode;        ///< getVirtualDiskInfo
    U8              sedFlag;                ///< not_support 保留字段
    U8              piFlag;                 ///< not_support 保留字段
    U8              dsMode;                 ///< not_support 保留字段
    U8              cacheVdFlag;            ///< not_support 保留字段
    U8              exclusiveAccessFlag;    ///< not_support 保留字段
    U8              pad2[3];                ///< 保留字段
    U64             startBlock;             ///< VD起始位置,单位512B getVirtualDiskInfo
    U64             extentSize;             ///< 起始位置处的可用extent大小,仅在DG上新增VD时有效,单位block extentSize
    Ps3LibVdBrifeInfo_t   vdCreateInfo;
}Ps3LibCfgfileVdInfo_t;

/**
 * @brief   DG空闲空间结构体
 */
typedef struct Ps3LibFreeSpace{
    U16         afterVd;            ///< 在哪个VD后面
    U8          pad[6];             ///< 保留字段
    U64         vdSize;             ///< 可创建的VD大小(单位sector)
    U64         startBlock;         ///< 起始位置
    U64         extentSize;         ///< extent大小(单位sector)
}Ps3LibFreeSpace_t;

/**
 * @brief   DG剩余空间结构体
 */
typedef struct Ps3LibDgFreeSpace{
    U16     dgId;                   ///< DG ID
    U16     freeSpaceNum;           ///< 空闲空间个数
    U8      pad[4];                 ///< 保留字段
    Ps3LibFreeSpace_t  freeSpace[PS3LIB_MAX_VD_PER_DG + 1];  ///< 最多16+1块空闲空间
}Ps3LibDgFreeSpace_t;

/**
 * @brief   DG信息
 */
typedef struct Ps3LibDgInfo{
    U32             dgInfoSize;             ///< DG大小
    U16             dgId;                   ///< DG序号
    U8              raidLevel;              ///< raid级别
    U8              state;                  ///< DG状态 (enum Ps3LibVdState_e)
    U8              isBackTaskActive;       ///< 是否有后台任务正在执行
    U8              pdcache;                ///< 物理盘缓存策略
    U8              isFreeSpace;            ///< 是否有空闲空间
    U8              piEnable;               ///< 保留字段
    U8              sedEnable;              ///< 保留字段
    U8              psPolicy;               ///< 保留字段
    U8              isTransportReady;       ///< 保留字段
    U8              pad[1];                 ///< 保留字段
    U64             raidDiskCfgBlocks;      ///< 各成员盘加入DG的数据空间block数
    U64             totalBlocks;            ///< 总block数
    U64             usedBlocks;             ///< 已经使用的block数
    U8              dgWWN[PS3LIB_WWN_LEN];  ///< DG唯一标识,保留字段
    U16             vdCount;                ///< vd数量
    U16             pdNum;                  ///< pd数量
    U8              spanNum;                ///< span数量
    U8              localHotSpareNum;       ///< 局部热备数量
    U8              prSwitch;               ///< 巡读开关是否打卡
    U8              pad2;
    U16             vdId[PS3LIB_MAX_VD_PER_CTRL_RAID];
    Ps3LibSpanInfo_t       spanInfo[PS3LIB_SPAN_PER_DG];          ///< 248Bytes
    Ps3LibRaidDisk_t       spares[PS3LIB_MAX_DEDICATED_SPARE_PER_DG];     ///< 热备盘
    Ps3LibCfgfileVdInfo_t  vdInfo[PS3LIB_MAX_VD_PER_DG];                  ///< 虚拟盘
    Ps3LibRaidDisk_t       drives[PS3LIB_MAX_PD_PER_DG];                  ///< 成员盘
    Ps3LibDgFreeSpace_t    freeInfo;                       ///< 磁盘组空闲空间
}Ps3LibDgInfo_t;

/**
 * @brief       vd列表信息结构体
 */
typedef struct Ps3LibDgList{
    U32             dgCount;    ///< dg数量
    DgId_t          dgId[PS3LIB_MAX_DG_NUM];    ///< dgId
} Ps3LibDgList_s;

/**
 * @brief   LIB对外的DG信息结构体
 */
typedef struct Ps3LibGlobalSpares {
    U16              count;
    U8               pad[6];
    Ps3LibRaidDisk_t globalSpare[PS3LIB_MAX_GLOBAL_SPARE_RAID];
} Ps3LibGlobalSpares_t;

/**
 * @brief   Dg重建入参结构体, 供上层使用
 */
typedef struct Ps3LibMigrateReq{
    U8                  operation;                  ///< 重构操作, enum Ps3LibRestructionOps_e
    U8                  newRaidLevel;               ///< 新raid级别
    U8                  mode;                       ///< 迁移模式, 0-vd大小等比改变, 1-vd大小不变
    U8                  pdPerArray;                 ///< 不支持复合raid迁移，此参数已废弃
    U16                 pdCount;                    ///< 新增/删除的pd个数
    U8                  withddr;                    ///< 迁移时使用ddf保存冲突区域进行数据提速(0:不提速, 1-提速)
    U8                  pad;                        ///< 保留字段
    Ps3LibPdPosition_t  pdPosi[PS3LIB_MAX_PD_PER_VD_RAID];      ///< 新增/删除的PD列表
}Ps3LibMigrateReq_t;

/**
 * @brief   VD后台任务信息
 */
typedef struct Ps3LibDgProgressInfo{
    U8     migrate;    ///< 迁移(enum Ps3LibProgressStatus_e)
    U8     pad[3];     ///< 4字节对齐
    Ps3LibProgress_t   migrateProgress;     ///< 迁移状态
    Ps3LibMigrateReq_t pMigrateReq;         ///< dg重建信息结构体
}Ps3LibDgProgressInfo_t;

/**
 * @brief   外部盘DG列表信息结构体
 */
typedef struct Ps3LibForeignDgList{
    S32         hasFrgnCfg;     ///< 是否存在foreign配置
    U32         frgnDgCount;    ///< foreign Dg的个数
    U16         frgnDgId[PS3LIB_MAX_FOREIGN_DG_PER_CTLR];  ///< foreign Dg列表
}Ps3LibForeignDgList_t;

/**
 * @brief   foreign盘导入参数
 */
typedef struct Ps3LibForeignImportArg {
    U8      pfaForce;       ///< PFA标记的盘需要指定该参数才可以导入(1:force)
    U8      isSec;          ///< 是否输入了密钥(0:无密钥, 1: 有密钥)
    U8      pad[2];
    U8      securityKey[PS3LIB_FOREIGN_SECURITY_KEY_LEN];    ///< 密钥
}Ps3LibForeignImportArg_t;

/**
 * @brief 外部配置VD信息
 */
typedef struct Ps3LibForeignVdInfo{
    U16     dgId;                   ///< DG序号
    U16     vdId;                   ///< VD序号
    U8      pad[4];                 ///< 保留字段
    U64     sectorNum;              ///< VD大小(单位sector)
    U8      raidLevel;              ///< raidLevel
    U8      pad2[7];                ///< 保留字段
    U8      vdName[PS3LIB_VD_NAME_LEN];    ///< VD名称(字符串可能不包含终止符'\0')
}Ps3LibForeignVdInfo_t;

/**
 * @brief 外部配置VD列表
 */
typedef struct Ps3LibForeignVdInfoList{
    U16                     count;
    U8                      pad[6];
    Ps3LibForeignVdInfo_t   vdInfoArray[PS3LIB_MAX_VD_PER_FOREIGN_DG_RAID];///< VDInfo数组
}Ps3LibForeignVdInfoList_t;

/**
 * @brief   DG信息
 */
typedef struct Ps3LibDgBaseInfo{
    U32             dgInfoSize;             ///< dg info size大小
    U16             dgId;                   ///< DG序号
    U8              raidLevel;              ///< raid级别
    U8              state;                  ///< DG状态 (enum Ps3LibVdState_e)
    U8              isBackTaskActive;       ///< 是否有后台任务正在执行
    U8              pdcache;                ///< 物理盘缓存策略
    U8              isFreeSpace;            ///< 是否有空闲空间
    U8              piEnable;               ///< 保留字段
    U8              sedEnable;              ///< 保留字段
    U8              psPolicy;               ///< 保留字段
    U8              isTransportReady;       ///< 保留字段
    U8              pad[1];                 ///< 保留字段
    U64             raidDiskCfgBlocks;      ///< 各成员盘加入DG的数据空间block数
    U64             totalBlocks;            ///< 总block数
    U64             usedBlocks;             ///< 已经使用的block数
    U8              dgWWN[PS3LIB_WWN_LEN];  ///< DG唯一标识,保留字段
    U16             vdCount;                ///< DG内VD数量
    U16             pdNum;                  ///< DG内PD数量
    U8              spanNum;                ///< DG内span数量
    U8              localHotSpareNum;       ///< DG内局部热备数量
    U8              prSwitch;               ///< 巡读开关标记(0:关闭, 1:开启)
    U8              pad2;                   ///< 保留字段
    U16             vdId[PS3LIB_MAX_VD_PER_CTRL_RAID];        ///< DG内VD标识符列表
    Ps3LibSpanInfo_t   spanInfo[PS3LIB_SPAN_PER_DG];          ///< DG内span信息列表
    Ps3LibRaidDisk_t   spares[PS3LIB_MAX_DEDICATED_SPARE_PER_DG];     ///< DG内spare信息列表
    Ps3LibCfgfileVdInfo_t   vdInfo[PS3LIB_MAX_VD_PER_DG];     ///< DG内VD信息列表
    Ps3LibRaidDisk_t   drives[0];                  ///< 成员盘
}Ps3LibDgBaseInfo_t;

/**
 * @brief   DG信息结构体
 */
typedef struct Ps3LibForeignDgInfo{
    Ps3LibForeignVdInfoList_t  frgnVdInfo; ///< foreign Dg中vd的信息
    Ps3LibDgBaseInfo_t             frgnDgInfo; ///< foreign Dg基本信息
}Ps3LibForeignDgInfo_t;

/**
 * @brief       获取控制卡下磁盘组列表
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   dgList: 输出dg列表
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libDgListGet(CtrlId_t ctrlId, Ps3LibDgList_s *dgList);

/**
 * @brief       获取指定磁盘组的信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   dgId:   磁盘组标识符
 * @param[out]  dgInfo: 磁盘组信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libDgInfoGet(CtrlId_t ctrlId, DgId_t dgId, Ps3LibDgInfo_t *dgInfo);

/**
 * @brief       指定磁盘组开始迁移
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   dgId: 磁盘组标识符
 * @param[in]   info: 磁盘组迁移信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 *              PS3LIB_ERRNO_INVALID_PD_STATE: 物理盘状态错误
 *              PS3LIB_ERRNO_DRIVES_NOT_IN_DG: 要移除的物理盘不在dg组内
 */
Ps3Errno ps3libDgMigrateStart(CtrlId_t ctrlId, DgId_t dgId, Ps3LibMigrateReq_t *info);

/**
 * @brief       获取指定磁盘组迁移后台任务信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   dgId: 磁盘组标识符
 * @param[out]  info: 磁盘组迁移进度信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libMigrateProgInfoGet(CtrlId_t ctrlId, DgId_t dgId, Ps3LibDgProgressInfo_t *info);

/**
 * @brief       批量获取磁盘组的信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   dgList: 磁盘组列表
 * @param[out]  dgInfo: 磁盘组信息
 * @param[out]  pErrList:   批量查询时上报的dg错误码列表
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libDgInfoGetByDgList(CtrlId_t ctrlId, Ps3LibDgList_s *dgList, Ps3LibDgInfo_t *dgInfo, Ps3Errno *pErrList);

/**
 * @brief       设置指定磁盘组是否对操作系统隐藏
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   dgId: 磁盘组标识符
 * @param[out]  hiddenFlag: 磁盘组对操作系统隐藏标记(0:隐藏, 1:不隐藏)
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libDgHiddenSet(CtrlId_t ctrlId, DgId_t dgId, U8 hiddenFlag);

/**
 * @brief       导入外部配置
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   importInfo: 外部配置秘钥信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libForeignImport(CtrlId_t ctrlId, Ps3LibForeignImportArg_t *importInfo);

/**
 * @brief       删除外部配置
 * @param[in]   ctrlId: 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libForeignDelete(CtrlId_t ctrlId);

/**
 * @brief       获取外部配置列表
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  foreignDgList: 外部配置列表
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libForeignDgListGet(CtrlId_t ctrlId, Ps3LibForeignDgList_t *foreignDgList);

/**
 * @brief       获取外部配置的dg信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   foreignDgId: 外部配置序号
 * @param[out]  foreignDgInfo: 外部配置的dg信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libForeignDgInfoGet(CtrlId_t ctrlId, U16 foreignDgId, Ps3LibForeignDgInfo_t *foreignDgInfo);

/**
 * @brief        获取Drive Group允许的操作
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    dgId:   dg标识符
 * @param[out]   allowedOps: vd允许的操作
 * @return       PS3_ERRNO_SUCCESS: 成功; 其他: 失败
 */
Ps3Errno ps3libDgAllowedOpsGet(CtrlId_t ctrlId, const DgId_t dgId, Ps3LibDgAllowedOps_t *allowedOps);

/**
 * @brief       获取全局热备盘信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  globalSpares: 全局热备盘信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libGlobalSparesGet(CtrlId_t ctrlId, Ps3LibGlobalSpares_t *globalSpares);

#if defined(__cplusplus)
}
#endif

#endif
