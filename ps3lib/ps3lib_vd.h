/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_vd.h
 * @date    2023.05.15
 * @brief   磁盘信息
 */
#ifndef __PS3LIB_VD_H__
#define __PS3LIB_VD_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "ps3lib_ctrl.h"
#include "ps3lib_pd.h"
#include "ps3lib_dg.h"
#include "ps3lib_phy.h"

#define PS3LIB_MAX_VD_NUM                  (256)    ///< ctrl支持的最大vd数量
#define PS3LIB_VD_NAAID_LEN                (16)     ///< NAAId(Network Address Authority)长度
#define PS3LIB_MAX_VD_PASSWD_LEN           (32)     ///< vd密码最大长度
#define PS3LIB_RAID1_MIN_PD_CNT            (2)

/**
 * @brief   VD encryption枚举
 */
typedef enum Ps3LibVdEncryption {
    PS3LIB_VD_ENCRYPTION_TYPE_NONE = 0,      ///< 没有加密
    PS3LIB_VD_ENCRYPTION_TYPE_FDE  = 1,      ///< fde驱动加密
    PS3LIB_VD_ENCRYPTION_TYPE_CTRL = 2,      ///< 控制器加密
} Ps3LibVdEncryption_e;

/**
 * @brief   VD模拟类型枚举
 */
typedef enum Ps3LibEmulationType{
    PS3LIB_EMULATION_TYPE_DEFAULT      = 0,
    PS3LIB_EMULATION_TYPE_DISABLE      = 1,
    PS3LIB_EMULATION_TYPE_512E_FORCE   = 2,
}Ps3LibEmulationType_t;

/**
 * @brief   VD Unmap枚举
 */
typedef enum Ps3LibUnmapStatus {
    PS3LIB_UNMAP_DISABLE = 0,   ///< disable
    PS3LIB_UNMAP_ENABLE,        ///< enable
    PS3LIB_UNMAP_NA,        ///< enable
} Ps3LibUnmapStatus_e;

/**
 * @brief   VD基本信息
 */
typedef struct Ps3LibVdBaseInfo{
    U16     vdId;                       ///< VD target ID
    U16     dgId;                       ///< 所属磁盘组ID
    U16     devId :12;                  ///< 所属设备ID
    U16     channelId :4;               ///< 所属通道ID
    U8      raidLevel;                  ///< raid级别(enum Ps3LibRaidLevel_e)
    U8      vdstate;                    ///< VD状态
    U8      accessPolicy;               ///< VD访问策略(enum Ps3LibVdAccessPolicy_t)
    U8      isHidden;                   ///< 0-not hidden, 1-hidden
    U8      isConsist;                  ///< 一致性检验
    U8      isLocked;                   ///< vd是否锁定
    U8      configReadPolicy;           ///< 配置读策略
    U8      configWritePolicy;          ///< 配置写策略
    U8      configIoPolicy;             ///< Not Support
    U8      currentReadPolicy;          ///< 当前读策略
    U8      currentWritePolicy;         ///< 当前写策略
    U8      currentIoPolicy;            ///< Not Support
    U8      pad[6];
    U64     vdSize;                     ///< VD 大小
    U8      vdName[PS3LIB_VD_NAME_LEN]; ///< VD名称(字符串可能不包含终止符'\0')
    U8      isCcScheduled;              ///< 是否定期进行一致性检查
    U8      pad1[7];
    U32     logicalSectorSize;          ///< 逻辑扇区大小(B)
    U32     physicalSectorSize;         ///< 物理扇区大小(B)
    U8      reserved[16];
}Ps3LibVdBaseInfo_t;                       ///< 576Bytes

typedef struct Ps3LibVdSpanBlock {
    U64     startBlock;                 ///< span中pd的起始块位置
    U64     blockSize;                  ///< span中pd所占block大小
    U16     arrayIndex;                 ///< 字段不明确
    U8      pad[6];
} Ps3LibVdSpanBlock_t;

typedef struct Ps3LibVdSpan {
    U8                spanCount;
    U8                pad[7];
    Ps3LibVdSpanBlock_t  block[8];
} Ps3LibVdSpan_t;

/**
 * @brief   VD属性
 */
typedef struct Ps3LibVdProperties{
    U8          configReadPolicy;           ///< 配置读策略
    U8          configWritePolicy;          ///< 配置写策略
    U8          configIoPolicy;             ///< 配置IO策略
    U8          currentReadPolicy;          ///< 当前读策略
    U8          currentWritePolicy;         ///< 当前写策略
    U8          currentIoPolicy;            ///< 当前IO策略
    U8          accessPolicy;               ///< 访问策略
    U8          isHidden;                   ///< 0-not hidden, 1-hidden
    U8          pdCache;                    ///< 硬盘缓存策略(enum Ps3LibPdCache_t)
    U8          autoBgi;                    ///< 后台初始化使能
    U8          emulationType;              ///< 模拟类型(0-default,1-enable512esupport)
    U8          cacheBypassIOSize;          ///< Cachebypass大小 (enum Ps3LibCacheBypassIoSize_e)
    U8          cacheBypassMode;            ///< Cachebypass类型   (enum Ps3LibCacheBypassMode_e)
    U8          unmapEnable;                ///< unmap
    U8          isBootDrive;                ///< 是否启动盘
    U8          SSCWriteBuferMode;          ///< not support
    U32         createTime;                 ///< VD创建时间戳
    U8          pad[4];
    U8          scsiNAAId[PS3LIB_VD_NAAID_LEN];    ///< NAAId(Network Address Authority)(scsi功能字段，暂时保留)
    U8          name[PS3LIB_VD_NAME_LEN];          ///< VD名称,15 + '\0'
    Ps3LibVdSpan_t span;                    ///< vd中span相关信息
    U8          encryptionStatus;           ///< 加密状态 0-off 1-on
    U8          certifyUpdate;              ///< 认证状态更新开关 0-on 1-off
    U8          reserved[30];               ///< 保留字段
}Ps3LibVdProperties_t;     ///< 96Bytes

/**
 * @brief   VD set属性
 */
typedef struct Ps3LibVdSetProperties{
    U8          readPolicy;                 ///< 配置读策略
    U8          writePolicy;                ///< 配置写策略
    U8          ioPolicy;                   ///< 配置IO策略
    U8          pdCache;                    ///< 硬盘缓存策略(enum Ps3LibPdCache_t)
    U8          accessPolicy;               ///< 访问策略
    U8          autoBgi;                    ///< 后台初始化使能
    S8          name[PS3LIB_VD_NAME_LEN];   ///< VD名称,15 + '\0'
    U16         vdId;                       ///< VD标识号
    U8          reserved[2];               ///< 保留字段
} Ps3LibVdSetProperties_t;

/**
 * @brief   VD参数
 */
typedef struct Ps3LibVdParameters{
    U8      PRL;                        ///< DDF - primay raid level
    U8      RLQ;                        ///< DDF - raid level qaulifier
    U8      SRL;                        ///< DDF - secondary raid level
    U8      padEx;                      ///< 原sectorSize,废弃
    U32     stripSize;                  ///< 条带大小(单位:sector)
    U8      state;                      ///< VD状态(enum Ps3LibVdState_e)
    U8      initState;                  ///< 前台初始化状态(0=no init, 1=quick init, 2=full init)
    U8      spanNum;                    ///< 磁盘组中span个数
    U8      pdPerSpan;                  ///< 各个span中pd个数
    U16     dgId;                       ///< 所属dg编号
    U16     pdDeviceId[PS3LIB_MAX_PD_PER_DG];  ///< 组成VD的物理盘ID列表
    U8      isConsistent;               ///< 条带是否一致
    U8      isCcScheduled;              ///< 是否定期进行一致性检查
    U8      encryptionType;             ///< 加密模式 (暂时不支持,保留)
    U8      isCachePinned;              ///< 是否含有 pinned cache
    U8      isReady;                    ///< 是否准备好接受操作系统的请求(保留，暂时不实现)
    U8      unmapEnable;                ///< unmap
    U8      badBlocksExist;             ///< 是否存在坏块(被更正的坏块也属于存在坏块)
    U8      pad;                        ///< 保留 不做功能
    U8      accessPolicyStatus;         ///< 访问策略状态(enum Ps3LibVdAccessPolicyStatus_e)
    U8      currentAccessPolicy;        ///< 当前访问策略(enum Ps3LibVdAccessPolicy_t)
    U8      fastPathWbEnabled;          ///< 数据io通路功能点, 0-disable,1-enable
    U8      vdSmartEnable;              ///< 是否支持逻辑盘Smart, 0-not support,1-support
    U8      hasEmulatedPD;              ///< 是否包含逻辑扇区强制转换的磁盘
    U8      reserved[33];
}Ps3LibVdParameters_t;     ///< 48Bytes

/**
 * @brief   VD允许的操作
 */
typedef struct Ps3LibVdAllowedOps{
    U32     startFGI                    : 1; ///< 开始前台初始化
    U32     stopFGI                     : 1; ///< 停止前台初始化
    U32     startCC                     : 1; ///< 开始一致性校验
    U32     stopCC                      : 1; ///< 停止一致性校验
    U32     startRecon                  : 1; ///< not_support 允许开始重建
    U32     deleteVd                    : 1; ///< 删除VD,片内赋值。需上层自行判断是否有分区
                                             ///< 假如拿到false，自行判断vd包含分区，则不能删vd，只能强制forceDeleteVD
    U32     secure                      : 1; ///< 加密VD,创建vd时，加密VD, raid支持加密；hba不支持
                                             ///< 如果vd 是加密的,secure为true; 如果这个vd没有加密，secure为 false.
    U32     SSCCacheable                : 1; ///< not_support cache保留字段
    U32     suspendCCBGI                : 1; ///< not_support 暂停一致性校验和后台初始化
    U32     resumeCCBGI                 : 1; ///< not_support 恢复一致性校验和后台初始化
    U32     suspendRecon                : 1; ///< not_support 暂停重建
    U32     resumeRecon                 : 1; ///< not_support 恢复重建
    U32     startSecureEraseNonSED      : 1; ///< not_support 对于自加密的VD的安全擦除(保留字段)
    U32     stopSecureEraseNonSED       : 1; ///< not_support 停止VD安全擦除
    U32     forceDeleteVD               : 1; ///< 强制删除VD，片内赋值
    U32     fullFGDisallow              : 1; ///< 是否允许全盘前台初始化
    U32     scsiUnmap                   : 1; ///< not_support 1=LD unmap/trim is allowed
    U32     hideLD                      : 1; ///< 隐藏VD(配置互斥)
    U32     unhideLD                    : 1; ///< 暴露VD(配置互斥)
    U32     setTransportReady           : 1; ///< not_support 1= LD can be set as transport ready(暂不支持)
    U32     clearTransportReady         : 1; ///< not_support 1= Clear transport ready status for the LD(暂不支持)
    U32     disableLDSetProperty        : 1; ///< not_support 1= LD set property not allowed(某些状态下不允许修改VD属性)
    U32     breakMirror                 : 1; ///< not_support 保留字段
    U32     joinMirror                  : 1; ///< not_support 保留字段
    U32     enableHAShare               : 1; ///< not_support 集群相关,保留字段
    U32     disassociatedSSC            : 1; ///< not_support Disassociation from cache pool allowed,保留字段
    U32     atomicityChangeable         : 1; ///< not_support atomicity can be changed from Enable to Disable 
                                             ///< or from Disable to Enable
    U32     startBgi                    : 1; ///< not_support 能否后台初始化
    U32     dataProtection              : 1; ///< not_support 能否进行数据加密
    U32     expandVd                    : 1; ///< 能否扩展
    U32     enableScsiUnmap             : 1; ///< 能否启动scsi unmap
    U32     disableScsiUnmap            : 1; ///< 能否关闭scsi unmap
    U32     migrateDg                   : 1; ///< 能否dg迁移
    U32     suspendCC                   : 1; ///< 能否暂停一致性校验
    U32     resumeCC                    : 1; ///< 能否恢复一致性校验
    U32     suspendBGI                  : 1; ///< 能否暂停后台初始化
    U32     resumeBGI                   : 1; ///< 能否恢复后台初始化
    U32     stopBGI                     : 1; ///< 能否停止后台初始化
    U32     modifyProperties            : 1; ///< 能否进行属性修改
    U32     startErase                  : 1; ///< 能否开始擦除
    U32     stopErase                   : 1; ///< 能否停止擦除
    U32     makeBootDrive               : 1; ///< 支持设置为bootdrive
    U32     accessPolicy                : 1; ///< 是否可修改accessPolicy策略，目前发现隐藏的vd不可修改
    U32     ioPolicy                    : 1; ///< 是否可修改ioPolicy策略，VD IO 直发使能
    U32     secureOff                   : 1; ///< 关闭加密 ； vd处于加密状态时为true
    U32     certifyUpdate               : 1; ///< 重启后，是否保持之前的加密状态。vd处于加密状态时为true
    U32     certifyNotUpdate            : 1; ///< 重启后，是否不保持之前的加密状态。
    U32     changePassWd                : 1; ///< 修改加密秘钥
    U32     pdCache                     : 1; ///< IO是否写回到盘上cache
    U32     emulation                   : 1; ///< 512e盘模拟类型
    U32     cbsize                      : 1; ///< 数据直通传输大小和模式
    U32     name                        : 1; ///< 设置名字
    U32     autobgi                     : 1; ///< 自动启动bgi开关
    U32     rdcache                     : 1; ///< 预读
    U32     wrcache                     : 1; ///< 回写策略
    U32     unlock                      : 1; ///< 是否可解锁
    U32     locate                      : 1; ///< 是否支持定位
    U32     reserved                    : 7; ///< 补齐字段
    
    ///< 以下字段为上面各个字段的有效性字段，只有对应的有效性字段为true时，才能根据该字段的值判断操作是否支持
    U32     startFGIValid               : 1; ///< 开始前台初始化
    U32     stopFGIValid                : 1; ///< 停止前台初始化
    U32     startCCValid                : 1; ///< 开始一致性校验
    U32     stopCCValid                 : 1; ///< 停止一致性校验
    U32     startReconValid             : 1; ///< 允许开始重建
    U32     deleteVdValid               : 1; ///< 删除VD(暂时一直允许删除VD，可以不实现)
    U32     secureValid                 : 1; ///< 加密VD,保留字段
    U32     SSCCacheableValid           : 1; ///< cache保留字段
    U32     suspendCCBGIValid           : 1; ///< 暂停一致性校验和后台初始化
    U32     resumeCCBGIValid            : 1; ///< 恢复一致性校验和后台初始化
    U32     suspendReconValid           : 1; ///< 暂停重建
    U32     resumeReconValid            : 1; ///< 恢复重建
    U32     startSecureEraseNonSEDValid : 1; ///< 对于自加密的VD的安全擦除(保留字段)
    U32     stopSecureEraseNonSEDValid  : 1; ///< 停止VD安全擦除
    U32     forceDeleteVDValid          : 1; ///< 强制删除VD(暂时一直允许删除VD，可以不实现)
    U32     fullFGDisallowValid         : 1; ///< 是否允许全盘前台初始化
    U32     scsiUnmapValid              : 1; ///< 1=LD unmap/trim is allowed
    U32     hideLDValid                 : 1; ///< 隐藏VD(配置互斥)
    U32     unhideLDValid               : 1; ///< 暴露VD(配置互斥)
    U32     setTransportReadyValid      : 1; ///< 1= LD can be set as transport ready(暂不支持)
    U32     clearTransportReadyValid    : 1; ///< 1= Clear transport ready status for the LD(暂不支持)
    U32     disableLDSetPropertyValid   : 1; ///< 1= LD set property not allowed(某些状态下不允许修改VD属性)
    U32     breakMirrorValid            : 1; ///< 保留字段
    U32     joinMirrorValid             : 1; ///< 保留字段
    U32     enableHAShareValid          : 1; ///< 集群相关,保留字段
    U32     disassociatedSSCValid       : 1; ///< Disassociation from cache pool allowed,保留字段
    U32     atomicityChangeableValid    : 1; ///< atomicity can be changed
    U32     startBgiValid               : 1; ///< 能否后台初始化
    U32     dataProtectionValid         : 1; ///< 能否进行数据加密
    U32     expandVdValid               : 1; ///< 能否扩展
    U32     enableScsiUnmapValid        : 1; ///< 能否启动scsi unmap
    U32     disableScsiUnmapValid       : 1; ///< 能否关闭scsi unmap
    U32     migrateDgValid              : 1; ///< 能否dg迁移
    U32     suspendCCValid              : 1; ///< 能否暂停一致性校验
    U32     resumeCCValid               : 1; ///< 能否恢复一致性校验
    U32     suspendBGIValid             : 1; ///< 能否暂停后台初始化
    U32     resumeBGIValid              : 1; ///< 能否恢复后台初始化
    U32     stopBGIValid                : 1; ///< 能否停止后台初始化
    U32     modifyPropertiesValid       : 1; ///< 能否进行属性修改
    U32     startEraseValid             : 1; ///< 能否开始擦除
    U32     stopEraseValid              : 1; ///< 能否停止擦除
    U32     makeBootDriveValid          : 1; ///< 支持设置为bootdrive
    U32     accessPolicyValid           : 1; ///< 是否可修改accessPolicy策略，目前发现隐藏的vd不可修改
    U32     ioPolicyValid               : 1; ///< 是否可修改ioPolicy策略，VD IO 直发使能
    U32     secureOffValid              : 1; ///< 关闭加密 ； vd处于加密状态时为true
    U32     certifyUpdateValid          : 1; ///< 重启后，是否保持之前的加密状态。vd处于加密状态时为true
    U32     certifyNotUpdateValid       : 1; ///< 重启后，是否不保持之前的加密状态。
    U32     changePassWdValid           : 1; ///< 修改加密秘钥
    U32     pdCacheValid                : 1; ///< IO是否写回到盘上cache
    U32     emulationValid              : 1; ///< 512e盘模拟类型
    U32     cbsizeValid                 : 1; ///< 数据直通传输大小和模式
    U32     nameValid                   : 1; ///< 设置名字
    U32     autobgiValid                : 1; ///< 自动启动bgi开关
    U32     rdcacheValid                : 1; ///< 预读
    U32     wrcacheValid                : 1; ///< 回写策略
    U32     unlockValid                 : 1; ///< 是否可解锁
    U32     locateValid                 : 1; ///< 是否支持定位bit有效
    U32     reservedValid               : 7; ///< 补齐字段
}Ps3LibVdAllowedOps_t;     ///< 8Bytes

/**
 * @brief   VD后台任务信息
 */
typedef struct Ps3LibVdProgressInfo{
    U8     cc;             ///< 一致性校验(enum Ps3LibProgressStatus_e)
    U8     bgi;            ///< 后台初始化
    U8     fgi;            ///< 前台初始化
    U8     erase;          ///< 擦除
    U8     locate;         ///< not_support    定位(运行|未运行)
    U8     disassociate;   ///< not_support    保留,不做功能
    U8     pad[2];         ///< 4字节对齐

    Ps3LibProgress_t   ccProgress;     ///< 一致性校验状态
    Ps3LibProgress_t   bgiProgress;    ///< 后台初始化状态
    Ps3LibProgress_t   fgiProgress;    ///< 前台初始化状态
    Ps3LibProgress_t   eraseProgress;  ///< 擦除状态
}Ps3LibVdProgressInfo_t;       ///< 24 + (8-520)Bytes

/**
 * @brief   VD所占空间信息(VD在每块盘上占据的空间叫做extent，多个extent之间一致)
 */
typedef struct Ps3LibVdExtentInfo{
    U16     vdId;                       ///< VD标识号
    U8      pad[6];
    U64     startSector;                 ///< VD起始位置 单位：Sector
    U64     extentSize;                  ///< VD extent 大小 单位：Sector
}Ps3LibVdExtentInfo_t;     ///< 12Bytes

/**
 * @brief   VD信息
 */
typedef struct Ps3LibVdInfo{
    U8      logicalVPDPage83[PS3LIB_VPD_PAGE_LEN]; ///< Firmware generated VPD page for the LD
    Ps3LibVdBaseInfo_t     vdBaseInfo;     ///< VD基本信息
    Ps3LibVdProperties_t   vdProp;
    Ps3LibVdParameters_t   vdParam;
    Ps3LibVdAllowedOps_t   vdAllowedOps;
    Ps3LibVdProgressInfo_t vdProgress;
    Ps3LibVdExtentInfo_t   vdExtentInfo;
}Ps3LibVdInfo_t;       ///< 8 + 64 + 576 + 96 + 48 + 4 + 24 + (8-520) + 12 = (840-1352)Bytes

/**
 * @brief   vd添加返回结构体
 */
typedef struct Ps3LibVdAddRsp {
    DgId_t  dgId;
    U16     vdCount;
    VdId_t  vdId[PS3LIB_MAX_VD_PER_DG];
} Ps3LibVdAddRsp_t;

/**
 * @brief       vd列表信息结构体
 */
typedef struct Ps3LibVdList{
    U32             vdCount;    ///< vd数量
    VdId_t          vdId[PS3LIB_MAX_VD_NUM];    ///< vdId
} Ps3LibVdList_s;

/**
 * @brief       vd中的pd列表
 */
typedef struct Ps3LibPdInVd{
    U32 count;
    U16 deviceId[PS3LIB_MAX_PD_PER_DG];
} Ps3LibPdInVd_t;

/**
 * @brief   VD当前可扩容信息(OCE, 不进行数据搬移)
 */
typedef struct Ps3LibVdExpansion{
    U16     vdId;           ///< vdId
    U8      supportOce;     ///< 是否支持在线扩容, 0-不支持, 1-支持
    U8      status;         ///< 未使用, 保留字段
    U8      pad[4];         ///< 保留字段
    U64     vdSize;         ///< vd大小(单位512B)
    U64     noDgExpSize;    ///< 使用dg内剩余空间进行扩容, 可扩容大小
    U64     dgExpSize;      ///< 更换Dg成员盘对DG进行扩容, 可扩容大小
}Ps3LibVdExpansion_t;       ///< 32Bytes

/**
 * @brief   VD扩容请求(OCE, 不进行数据搬移)
 */
typedef struct Ps3LibVdExpandReq{
    U8      expDg;          ///< 是否对dg进行扩容
    U8      pad[7];
    U64     size;           ///< 想要扩容的大小(单位512B)
} Ps3LibVdExpandReq_t;

/**
 * @brief   VD扩容回复(OCE, 不进行数据搬移)
 */
typedef struct Ps3LibVdExpandRsp {
    U8      status;         ///< 扩容状态, 含义未知, 保留字段
    U8      freePercent;    ///< 剩余空间百分比
    U8      rsv[6];
    U64     vdSize;         ///< vd大小
    U64     freeSize;       ///< 剩余空间大小
    U64     ReqSize;        ///< 想要扩容的大小
    U64     absUserSize;    ///< 实际上扩容的大小
    U64     newSize;        ///< 扩容之后vd的大小
    U64     noDgExpSize;    ///< dg内扩容的大小
} Ps3LibVdExpandRsp_t;

/**
 * @brief       vd隐藏标记
 */
typedef enum Ps3LibVdHiddenFlag {
    PS3LIB_UNHIDDEN_VD_FLAG = 0,    ///< 不隐藏vd
    PS3LIB_HIDDEN_VD_FLAG = 1,      ///< 隐藏vd
    PS3LIB_UNHIDDEN_VD_NR,
} Ps3LibVdHiddenFlag_e;

/**
 * @brief       vd初始化模式
 */
typedef enum Ps3LibVdInitMode {
    PS3LIB_VD_FULL_INIT_MODE = 0,     ///< vd全盘初始化
    PS3LIB_VD_FAST_INIT_MODE = 1,     ///< vd快速初始化
    PS3LIB_VD_INIT_MODE_NR,
} Ps3LibVdInitMode_e;

/**
 * @brief       vd擦除结构体
 */
typedef struct Ps3LibVdStartErase{
    U8  type;
    U8  patternA;
    U8  patternB;
    U8  delAfterErase;
    U8  force;              ///< 是否强制开始擦除
    U8  pad[3];
} Ps3LibVdStartErase_t;

/**
 * @brief   VD Erase Type 枚举
 */
typedef enum Ps3LibVdEraseType{
    PS3LIB_VD_ERASE_TYPE_SIMPLE      = 0,
    PS3LIB_VD_ERASE_TYPE_NORMAL      = 1,
    PS3LIB_VD_ERASE_TYPE_THOROUGH    = 2,
    PS3LIB_VD_ERASE_TYPE_STANDARD    = 3,
}Ps3LibVdEraseType_t;

/**
 * @brief   VD读策略枚举
 */
typedef enum Ps3LibVdReadCache {
    PS3LIB_VD_CACHE_READ_POLICY_RA     = 0,        ///< Read Ahead 预读
    PS3LIB_VD_CACHE_READ_POLICY_NORA   = 1,        ///< No Read Ahead
    PS3LIB_VD_CACHE_READ_POLICY_INVALID = 2,       ///< 无效值
    PS3LIB_VD_CACHE_READ_POLICY_NR,
} Ps3LibVdReadCache_e;

/**
 * @brief   VD写策略枚举
 */
typedef enum Ps3LibVdWriteCache {
    PS3LIB_VD_CACHE_WRITE_POLICY_WB = 0,   ///< Write Back 回写
    PS3LIB_VD_CACHE_WRITE_POLICY_WT,       ///< Write Through
    PS3LIB_VD_CACHE_WRITE_POLICY_AWB,      ///< Always Write Back
    PS3LIB_VD_CACHE_WRITE_POLICY_UNKNOWN,
} Ps3LibVdWriteCache_e;

/**
 * @brief   VD硬盘缓存策略枚举
 */
typedef enum Ps3LibPdCache{
    PS3LIB_PD_CACHE_AUTO       = 0,  ///< auto
    PS3LIB_PD_CACHE_ON         = 1,  ///< on
    PS3LIB_PD_CACHE_OFF        = 2,  ///< off
    PS3LIB_PD_CACHE_DEF        = 3,  ///< default
}Ps3LibPdCache_t;

/**
 * @brief   VD访问策略枚举
 */
typedef enum Ps3LibVdAccessPolicy{
    PS3LIB_VD_ACCESS_READ_WRITE                = 0,
    PS3LIB_VD_ACCESS_READ_ONLY                 = 1,
    PS3LIB_VD_ACCESS_BLOCKED                   = 2,
    PS3LIB_VD_ACCESS_REMOVE_ACCESS             = 3,
    PS3LIB_VD_ACCESS_TRANSPORT_READY           = 4,    ///< 保留
    PS3LIB_VD_ACCESS_TRANSPORT_UNKNOWN         = 5,
}Ps3LibVdAccessPolicy_t;

/**
 * @brief   VD硬盘缓存自动后台初始化枚举
 */
typedef enum Ps3LibPdAutobgi{
    PS3LIB_PD_AUTOBGI_OFF        = 0,  ///< off
    PS3LIB_PD_AUTOBGI_ON         = 1,  ///< on
    PS3LIB_PD_AUTOBGI_DEFAULT    = 2,  ///< default
}Ps3LibPdAutobgi_e;

/**
 * @brief   VD状态枚举
 */
typedef enum Ps3LibVdState{
    PS3LIB_VD_STATE_UNKNOWN             = 0,   ///< 脱机维护或故障状态,数据不可用
    PS3LIB_VD_STATE_OFFLINE,                   ///< 脱机维护或故障状态,数据不可用
    PS3LIB_VD_STATE_OPTIMAL,                   ///< 状态良好,所有物理盘均online
    PS3LIB_VD_STATE_PARTIAL_DEGRADED,          ///< 部分降级状态,数据未丢失
    PS3LIB_VD_STATE_DEGRADED,                  ///< 降级状态,数据未丢失
}Ps3LibVdState_e;

/**
 * @brief   VD条带大小枚举 单位：KB
 */
typedef enum Ps3LibStripSize{
    PS3LIB_STRIP_SIZE_64   = 64,
    PS3LIB_STRIP_SIZE_128  = 128,
    PS3LIB_STRIP_SIZE_256  = 256,
    PS3LIB_STRIP_SIZE_512  = 512,
    PS3LIB_STRIP_SIZE_1024 = 1024,
}Ps3LibStripSize_e;

/**
 * @brief   VD条最大的IO大小 单位：KB
 */
enum {
    PS3LIB_VD_MAX_IO_SIZE_64   = 64,
    PS3LIB_VD_MAX_IO_SIZE_128  = 128,
    PS3LIB_VD_MAX_IO_SIZE_256  = 256,
    PS3LIB_VD_MAX_IO_SIZE_512  = 512,
    PS3LIB_VD_MAX_IO_SIZE_1024 = 1024,
    PS3LIB_VD_MAX_IO_SIZE_NR,
};

/**
 * @brief   VD创建基本信息
 */
typedef struct Ps3LibVdCreateInfo{
    U64             vdSize;                  ///< vd大小, 单位512B
    U8              vdName[PS3LIB_VD_NAME_LEN];         ///< vd名称(注意:字符串可能不包含终止符'\0')
    S8              vdPasswd[PS3LIB_MAX_VD_PASSWD_LEN]; ///< vd密码(注意:字符串可能不包含终止符'\0')
    U8              certifyUpdate;
    U8              pad[7];
}Ps3LibVdCreateInfo_t;     ///< 24Bytes

/**
 * @brief   VD创建结构体
 */
typedef struct Ps3LibVdCreate{
    U8              raidLevel;              ///< raid级别
    U8              spanCount;              ///< span数量
    U8              pdPerSpan;              ///< 每个span的pd个数
    U8              pdCache;                ///< pd缓存策略
    U8              readPolicy;             ///< 读策略
    U8              writePolicy;            ///< 写策略
    U8              ioPolicy;               ///< Not Support 该策略已废弃,需填充0xFF忽略
    U8              accessPolicy;           ///< 访问策略
    U8              unmapFlag;        
    U8              pad1[3];
    U32             stripSize;              ///< 单位:sector    
    U32             sectorSize;             ///< 0xffffffff表示用户未指定该参数     
    U8              emulationType;          ///< vd模拟类型
    U8              forceFlag;              ///< 强制创建标志位
    U8              cbSize;                 ///< cache bypass io吞吐量
    U8              cbMode;                 ///< VD cache bypass 模式    
    U8              sedFlag;                ///< 保留字段
    U8              piFlag;                 ///< 数据是否加密
    U8              dsMode;                 ///< 保留字段
    U8              cacheVdFlag;            ///< 保留字段        
    U8              exclusiveAccessFlag;    ///< 保留字段
    U8              pad2[1];
    U16             afterVd;                ///< VD起始位置    
    U16             driveCount;             ///< 成员盘数量    
    U8              spareCount;             ///< 热备盘数量 字段保留,不使用    
    U8              pad3[1]; 
    U16             vdCount;                ///< 创建的VD数量
    U8              pad4[2];    
    Ps3LibVdCreateInfo_t   vdCreateInfo[PS3LIB_MAX_VD_PER_DG];      ///< vd创建基本信息
    Ps3LibPdPosition_t     drivesPosition[PS3LIB_MAX_PD_PER_DG];    ///< 成员盘
    Ps3LibPdPosition_t     sparesPosition[PS3LIB_MAX_DEDICATED_SPARE_PER_DG];    ///< 热备盘
} Ps3LibVdCreate_t;

/**
 * @brief   VD成员盘坏块管理信息(错误表)
 */
typedef struct Ps3LibPdErrTbl {
    U16     pdId;                   ///< PD id
    U16     pdCorrectedNum;         ///< 可能被更正的坏块数量
    U16     pdUncorrectedNum;       ///< 肯定不能被更正的坏块数量
    U8      reserved[2];
} Ps3LibPdErrTbl_t;

/**
 * @brief   VD坏块管理信息(错误表)
 */
typedef struct Ps3LibVdBadBlockInfo {
    U16     vdCorrectedNum;                     ///< 可能被更正的坏块数量
    U16     vdUncorrectedNum;                   ///< 肯定不能被更正的坏块数量
    U16     pdInfoCount;                        ///< 对应VD有坏块的成员盘数量
    U8      pad[2];
    U8      vdName[PS3LIB_VD_NAME_LEN];                ///< VD名称(注意:字符串可能不包含终止符'\0')
    Ps3LibPdErrTbl_t pdList[PS3LIB_MAX_PD_PER_VD_RAID];   ///< 坏块成员盘列表
} Ps3LibVdBadBlockInfo_t;

/**
 * @brief   VD列表
 */
typedef struct Ps3LibDirtyVdList {
    U32 count; ///< -1:all
    U16 vdId[PS3LIB_MAX_VD_PER_CTRL_RAID]; ///< VD标识符
} Ps3LibDirtyVdList_t;

/**
 * @brief   VD初始化结构体
 */
typedef struct Ps3LibVdStartInit{
    U8  mode;
    U8  isForce;
    U8  modeDataLen;    ///< 模式数据长度, 不超过BGT_FGI_MODE_DATA_MAX_LEN, 且为2的n次幂
    U8  modeData[PS3LIB_BGT_FGI_MODE_DATA_MAX_LEN];    ///< 模式数据
}Ps3LibVdStartInit_t;

/**
 * @brief   VD访问策略状态枚举
 */
typedef enum Ps3LibRaidLevel{
    PS3LIB_VD_RAID_LEVEL_0     = 0x00,         ///< raid0
    PS3LIB_VD_RAID_LEVEL_1     = 0x01,         ///< raid1
    PS3LIB_VD_RAID_LEVEL_5     = 0x05,         ///< raid5
    PS3LIB_VD_RAID_LEVEL_6     = 0x06,         ///< raid6
    PS3LIB_VD_RAID_LEVEL_JBOD  = 0x0A,         ///< JBOD
    PS3LIB_VD_RAID_LEVEL_10    = 0x10,         ///< raid10
    PS3LIB_VD_RAID_LEVEL_1E    = 0x11,         ///< raid11
    PS3LIB_VD_RAID_LEVEL_00    = 0x20,         ///< raid00
    PS3LIB_VD_RAID_LEVEL_50    = 0x50,         ///< raid50
    PS3LIB_VD_RAID_LEVEL_60    = 0x60,         ///< raid60
    PS3LIB_VD_RAID_LEVEL_UNKNOWN    = 0xff,    ///< unknown
}Ps3LibRaidLevel_e;

/**
 * @brief   VD重构操作类型枚举
 */
typedef enum Ps3LibRestructionOps{
    PS3LIB_RECON_OP_NONE    = 0,                ///< 只改变raid级别
    PS3LIB_RECON_OP_INSERT  = 1,                ///< 加盘
    PS3LIB_RECON_OP_DELETE  = 2,                ///< 删除盘,保留字段,不支持
} Ps3LibRestructionOps_e;

/**
 * @brief   VD访问策略状态枚举
 */
typedef enum Ps3LibVdAccessPolicyStatus{
    PS3LIB_VD_ACCESS_STATUS_USER               = 1,    ///< policy set by user
    PS3LIB_VD_ACCESS_STATUS_SSC_OFFLINE        = 2,    ///< secondary cache is offline, some data is unavailable
    PS3LIB_VD_ACCESS_STATUS_PEER_UNAVAILABLE   = 3,    ///< peer controller in HA domain is offline,
} Ps3LibVdAccessPolicyStatus_e;

///< vd加密状态
typedef enum Ps3LibVdEncryptionStatus {
    PS3LIB_VD_ENCRYPTION_STATUS_OFF = 0, ///< vd加密状态 未开启加密
    PS3LIB_VD_ENCRYPTION_STATUS_ON  = 1, ///< vd加密状态 开启加密
    PS3LIB_VD_ENCRYPTION_STATUS_NR  = 2,
} Ps3LibVdEncryptionStatus_e;

///< 密码认证状态 off为主机重启后保持重启前的密码认证状态;on为不保持，重新加密
typedef enum Ps3LibVdCertifyUpdate {
    PS3LIB_VD_CERTIFY_UPDATE_ON  = 0, ///< certify on
    PS3LIB_VD_CERTIFY_UPDATE_OFF = 1, ///< certify off
    PS3LIB_VD_CERTIFY_UPDATE_NR  = 2,
} Ps3LibVdCertifyUpdate_e;

/**
 * @brief   VD cache bypass io吞吐量枚举
 */
typedef enum Ps3LibCacheBypassIoSize{
    PS3LIB_CACHEBYPASS_IOSIZE_64K     = 0,
    PS3LIB_CACHEBYPASS_IOSIZE_128K    = 1,
    PS3LIB_CACHEBYPASS_IOSIZE_256K    = 2,
}Ps3LibCacheBypassIoSize_e;

/**
 * @brief   VD cache bypass 模式枚举
 */
typedef enum Ps3LibCacheBypassMode{
    PS3LIB_CACHEBYPASS_ENABLE_INTELLIGENT      = 0,  ///< Involves streaming detection
    PS3LIB_CACHEBYPASS_ENABLE_STANDARD         = 1,  ///< Will bypass the IO if its size
    PS3LIB_CACHEBYPASS_ENABLE_CUSTOM_MODE_1    = 2,  ///< Custom bypass mode. Not implemented currently
    PS3LIB_CACHEBYPASS_ENABLE_CUSTOM_MODE_2    = 3,  ///< Custom bypass mode. Not implemented currently
    PS3LIB_CACHEBYPASS_ENABLE_CUSTOM_MODE_3    = 4,  ///< Custom bypass mode. Not implemented currently
    PS3LIB_CACHEBYPASS_ENABLE_RESERVED_MODE_1  = 5,  ///< Reserved for future use
    PS3LIB_CACHEBYPASS_ENABLE_RESERVED_MODE_2  = 6,  ///< Reserved for future use
    PS3LIB_CACHEBYPASS_DISABLE                 = 7,  ///< Disable cache bypass
}Ps3LibCacheBypassMode_e;

/**
 * @brief       获取指定控制卡下vd列表
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  vdList: vd列表
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdListGetByCtrlId(CtrlId_t ctrlId, Ps3LibVdList_s *vdList);

/**
 * @brief       获取vd列表下所有vd信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdList: vd列表
 * @param[out]  vdInfo: 用户接受回复的批量vd信息
 * @param[out]  pErrList:   批量查询时上报的vd错误码列表
 * @note        vdInfo需由调用者根据vd的数量申请内存并释放
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdInfoGetByVdList(CtrlId_t ctrlId, Ps3LibVdList_s *vdList, Ps3LibVdInfo_t *vdInfo, Ps3Errno *pErrList);

/**
 * @brief        获取vd基本信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId: vd标识符
 * @param[out]   vdBaseInfo: vd基本信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdBaseInfoGet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdBaseInfo_t *vdBaseInfo);

/**
 * @brief        获取vd所有信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId:   vd标识符
 * @param[out]   vdInfo: vd信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdInfoGet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdInfo_t *vdInfo);

/**
 * @brief        获取vd属性信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId: vd标识符
 * @param[out]   vdPropInfo: vd属性信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdPropertiesGet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdProperties_t *vdPropInfo);

/**
 * @brief        通过vdId获取pd列表
 * @param[in]    ctrlId:     控制卡标识符
 * @param[in]    vdId:       vd标识符
 * @param[out]   pdInVdList: vd中包含的pd列表
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPdListGetByVdId(CtrlId_t ctrlId, VdId_t vdId, Ps3LibPdInVd_t *pdInVdList);

/**
 * @brief        获取vd允许的操作
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId:   vd标识符
 * @param[out]   vdAllowedOps: vd允许的操作
 * @return       PS3_ERRNO_SUCCESS: 成功; 其他: 失败
 */
Ps3Errno ps3libVdAllowedOpsGet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdAllowedOps_t *vdAllowedOps);

 /**
 * @brief       进行删除vd操作
 * @param[in]   ctrlId:  控制卡标识符
 * @param[in]   vdId:    vd标识符
 * @param[in]   isForce: 强制删除vd标记(0:非强制, 1:强制)
 * @return      PS3_ERRNO_SUCCESS: 成功；其他: 失败
 */
Ps3Errno ps3libVdDelete(CtrlId_t ctrlId, VdId_t vdId, U8 isForce);

/**
 * @brief      vd开始定位
 * @param[in]  ctrlId: 控制卡标识符
 * @param[in]  vdId: vd标识符
 * @return     PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdLocateStart(CtrlId_t ctrlId, VdId_t vdId);

/**
 * @brief      vd停止定位
 * @param[in]  ctrlId: 控制卡标识符
 * @param[in]  vdId: vd标识符
 * @return     PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdLocateAbort(CtrlId_t ctrlId, VdId_t vdId);

/**
 * @brief       设置是否对系统隐藏虚拟盘
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   hiddenFlag: 隐藏标记位(enum Ps3LibVdHiddenFlag_e)
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdHiddenSet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdHiddenFlag_e hiddenFlag);

/**
 * @brief       开始虚拟盘一致性校验任务
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId:   vd标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdCcStart(CtrlId_t ctrlId, VdId_t vdId);

/**
 * @brief       终止虚拟盘一致性校验任务
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId:   vd标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdCcAbort(CtrlId_t ctrlId, VdId_t vdId);

/**
 * @brief       暂停虚拟盘一致性校验任务
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId:   vd标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdCcPause(CtrlId_t ctrlId, VdId_t vdId);

/**
 * @brief       恢复虚拟盘一致性校验任务
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId:   vd标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdCcResume(CtrlId_t ctrlId, VdId_t vdId);

/**
 * @brief       开始虚拟盘前台初始化任务
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   initParam: 初始化参数
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdInitStart(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdStartInit_t *initParam);

/**
 * @brief       停止虚拟盘前台初始化任务
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdInitAbort(CtrlId_t ctrlId, VdId_t vdId);

/**
 * @brief        获取虚拟盘后台任务进度
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId:   vd标识符
 * @param[out]   vdProgressInfo: vd后台任务信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdProgressGet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdProgressInfo_t *vdProgressInfo);

/**
 * @brief       开始虚拟盘擦除任务
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   vdEraseInfo: vd擦除信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdEraseStart(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdStartErase_t *vdEraseInfo);

/**
 * @brief       停止虚拟盘擦除任务
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId:   vd标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdEraseAbort(CtrlId_t ctrlId, VdId_t vdId);

/**
 * @brief        获取虚拟磁盘当前可扩容信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId: vd标识符
 * @param[out]   vdExpansion: vd当前可扩容信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdExpansionInfoGet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdExpansion_t *vdExpansion);

/**
 * @brief        进行vd扩容操作
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId: vd标识符
 * @param[in]    vdExpReq: 扩容请求
 * @param[out]   vdExpRsp: 扩容结果
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdExpand(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdExpandReq_t *vdExpReq, Ps3LibVdExpandRsp_t *vdExpRsp);

/**
 * @brief       设置或更改虚拟盘名称
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   name: 待设置的逻辑盘名称
 * @param[in]   len: 逻辑盘名称长度
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdNameSet(CtrlId_t ctrlId, VdId_t vdId, S8 *name, U32 len);

/**
 * @brief       vd设置读缓存策略
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId:   vd标识符
 * @param[in]   readCache: 读策略
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdReadCacheSet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdReadCache_e readCache);

/**
 * @brief       设置虚拟盘写缓存策略
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   writeCache: 写策略
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdWriteCacheSet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdWriteCache_e writeCache);

/**
 * @brief       设置vd的成员盘缓存策略
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   pdCache: 硬盘缓存策略
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdPdCacheSet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibPdCache_t pdCache);

/**
 * @brief       设置vd访问策略
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   accessPolicy: 访问策略
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdAccessPolicySet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdAccessPolicy_t accessPolicy);

/**
 * @brief       设置逻辑盘自动开启后台初始化
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   autoBgi: 自动执行后台初始化模式
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdAutoBgiSet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibPdAutobgi_e autoBgi);

/**
 * @brief        创建vd
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdCreate: 创建vd所需参数信息
 * @param[out]   vdAddRsp: 新建的vd列表以及所属dg信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdAdd(CtrlId_t ctrlId, Ps3LibVdCreate_t *vdCreate, Ps3LibVdAddRsp_t *vdAddRsp);

/**
 * @brief        获取逻辑盘一致性校验任务进度信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId: vd标识符
 * @param[out]   Ps3LibVdProgressInfo_t: Vd后台任务信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libCcProgressInfoGet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdProgressInfo_t *vdProInfo);

/**
 * @brief        设置虚拟盘属性
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId:   vd标识符
 * @param[in]    pVdSetPro: vd可以设置的属性
 * @return       PS3_ERRNO_SUCCESS: 成功
 * @note Ps3LibVdSetProperties_t 使用时初始化为全f
 */
Ps3Errno ps3libVdPropertiesSet(CtrlId_t ctrlId, VdId_t vdId, Ps3LibVdSetProperties_t *pVdSetPro,
    Ps3LibErrList_t *errList);

/**
 * @brief       设置虚拟盘为系统引导盘
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   isBootDrive: 设置引导盘标记(0:设置, 1:取消设置)
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libVdBootDriveSet(CtrlId_t ctrlId, VdId_t vdId, U8 isBootDrive);

/**
 * @brief       获取vd的PreservedCache状态
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  vdList, 存在脏数据的VD列表
 * @return      Ps3Errno
 */
Ps3Errno ps3libVdPreservedCacheGet(CtrlId_t ctrlId, Ps3LibDirtyVdList_t *vdList);

/**
 * @brief         获取虚拟盘列表
 * @param[in]     ctrlId: 控制卡标识符
 * @param[in]     vdIdListIn: 虚拟盘列表
 * @param[out]    vdIdListOut: 虚拟盘列表
 * @return        PS3_ERRNO_SUCCESS: 成功；其他: 失败
 * @note          vdIdListIn->count为虚拟盘的个数，vdIdListIn->values为虚拟盘的列表，
 *                当vdIdListIn指针为NULL或者vdIdListIn->count为小于0时，获取所有虚拟盘的列表
 *                \n当vdIdListIn不为NULL时，将vdIdListIn指向的数据拷贝到vdIdListOut的地址中
 * @attention     对vdIdListOut->values进行了限制，范围为1-132;对输出的vdIdListOut->values进行了排序，由从小到大;
 */
Ps3Errno ps3libVdListGet(CtrlId_t ctrlId, Ps3LibIdList_t *vdIdListOut, const Ps3LibIdList_t *vdIdListIn);

/**
 * @brief       vd删除preservedcache
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   force: 强制删除标志
 * @return      PS3_ERRNO_SUCCESS: 成功；其他: 失败
 */
Ps3Errno ps3libVdPreservedCacheDel(CtrlId_t ctrlId, VdId_t vdId, U8 *force);

/**
 * @brief        获取虚拟盘坏块信息
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    vdId: vd标识符
 * @param[in]    details: 是否获取details信息，暂未处理
 * @param[out]   badBlockInfo: vd坏块信息结构体指针
 * @return       PS3_ERRNO_SUCCESS: 成功；其他: 失败
 */
Ps3Errno ps3libVdBbmtInfoGet(CtrlId_t ctrlId, VdId_t vdId, U8 *details, Ps3LibVdBadBlockInfo_t *badBlockInfo);

/**
 * @brief       删除虚拟盘坏块信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @return      PS3_ERRNO_SUCCESS: 成功；其他: 失败
 */
Ps3Errno ps3libVdBbmtDel(CtrlId_t ctrlId, VdId_t vdId);

/**
 * @brief       设置VD的最大IO SIZE
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   maxIOsize: 最大IO大小 64 128 256 512 1024
 * @return      PS3_ERRNO_SUCCESS: 成功；其他: 失败
 */
Ps3Errno ps3libVdMaxIOSizeSet(CtrlId_t ctrlId, VdId_t vdId, U32 maxIOsize);

/**
 * @brief       获取VD的最大IO SIZE
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   vdId: vd标识符
 * @param[in]   maxIOsize: 最大IO大小 64 128 256 512 1024
 * @return      PS3_ERRNO_SUCCESS: 成功；其他: 失败
 */
Ps3Errno ps3libVdMaxIOSizeGet(CtrlId_t ctrlId, VdId_t vdId, U32 *maxIoSize);

#if defined(__cplusplus)
}
#endif

#endif
