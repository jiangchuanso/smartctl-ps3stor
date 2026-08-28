/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_pd.h
 * @date    2023.05.26
 * @brief   磁盘信息
 */
#ifndef __PS3LIB_PHY_H__
#define __PS3LIB_PHY_H__

#if defined(__cplusplus)
extern "C" {
#endif

#define PS3LIB_CTRL_MAX_PHY_CNT            (18)
#define PS3LIB_MAX_PD_PER_ENCL             (255)
#define PS3LIB_PHY_DESCRIPTION_LEN         (16)
#define PS3LIB_ID_LIST_MAX_COUNT           (256)   ///< 最大存储控制卡、背板、slot的个数

/**
 * @brief 存放控制卡、背板、slot标识,例如控制卡c1-3,5
 */
typedef struct Ps3libIdList {
    S32 count; ///< -1:all ,c1-3,5 : 4
    U8  pad[4];
    S64 values[PS3LIB_ID_LIST_MAX_COUNT]; ///< c1-3,5 : 1 2 3 5
} Ps3LibIdList_t;

/**
 * @brief PHY连接信息结构体
 */
typedef struct Ps3LibPhyBaseInfo {
    U64     sasAddr;                    ///< 本phy地址
    U64     attachedSasAddr;            ///< 对端sas地址
    U8      phyId;                      ///< phy标识
    U8      attachedPhyId;              ///< 对端phy标识
    U8      connectorId;                ///< phy所属的控制卡端口id
    U8      subConnectorId;             ///< phy所属的hac id
    U32     attachedDeviceType  : 3;    ///< 对端设备类型(enum Ps3LibTopologyDevType_e)
    U32     deviceIsSata        : 1;    ///< 标识SATA设备
    U32     smpInitiator        : 1;    ///< 标识SMP Initiator
    U32     stpInitiator        : 1;    ///< 标识STP Initiator
    U32     sspInitiator        : 1;    ///< 标识SSP Initiator
    U32     smpTarget           : 1;    ///< 标识SMP Target
    U32     stpTarget           : 1;    ///< 标识STP Target
    U32     sspTarget           : 1;    ///< 标识SSP Target
    U32     disabled            : 1;    ///< 标识phy被disable
    U32     sasPortId           : 6;    ///< port号
    U32     sasPortIdValid      : 1;    ///< 标识port有效
    U32     linkSpeed           : 6;    ///< 协商出的phy连接速率
    U32     maxSpeed            : 6;    ///< 用户设置的phy最大速率
    U32     reserved            : 2;
    U8      description[PS3LIB_PHY_DESCRIPTION_LEN];   ///< phy描述
} Ps3LibPhyBaseInfo_t;

/**
 * @brief   PHY错误计数结构体
 */
typedef struct Ps3LibPhyErrCount {
    U32     InvalidDwordCount;              ///< 非法双字计数
    U32     RunningDisparityErrorCount;     ///< 极性偏差错误计数
    U32     LossDwordSynchCount;            ///< 丢失同步计数
    U32     PhyResetProblemCount;           ///< Phy重置计数
} Ps3LibPhyErrCount_t;

/**
 * @brief   PHY列表结构体
 */
typedef struct Ps3LibPhyInfo {
    Ps3LibPhyBaseInfo_t    phyBaseInfo;                ///< phy基本信息
    Ps3LibPhyErrCount_t    phyErrCnts;                 ///< phy错误计数
} Ps3LibPhyInfo_t;

/**
 * @brief   PHY速度枚举
 */
typedef enum Ps3LibSasDeviceSpeed{
    PS3LIB_SAS_DEVICE_SPEED_NO_LIMIT   = 0,   ///< 非法类型
    PS3LIB_SAS_DEVICE_SPEED_1p5G       = 0x8, ///< 1.5Gbit/s
    PS3LIB_SAS_DEVICE_SPEED_3G         = 0x9, ///< 3.0Gbit/s
    PS3LIB_SAS_DEVICE_SPEED_6G         = 0xA, ///< 6.0Gbit/s
    PS3LIB_SAS_DEVICE_SPEED_12G        = 0xB, ///< 12.0Gbit/s
    PS3LIB_SAS_DEVICE_SPEED_22p5G      = 0xC, ///< 22.5Gbit/s
}Ps3LibSasDeviceSpeed_e;

/**
 * @brief   PHY连接设备类型枚举 TODO
 */
typedef enum Ps3LibSasDeviceType{
    PS3LIB_SAS_DEVICE_TYPE_NO_DEVICE           = 0,
    PS3LIB_SAS_DEVICE_TYPE_END_DEVICE          = 1,    ///< 终端设备
    PS3LIB_SAS_DEVICE_TYPE_EDGE_EXPANDER       = 2,    ///< edge expander
    PS3LIB_SAS_DEVICE_TYPE_FANOUT_EXPANDER     = 3,    ///< fanout expander
}Ps3LibSasDeviceType_t;

/**
 * @brief   连接器位置枚举
 */
typedef enum Ps3LibSasConnLocationType{
    PS3LIB_SAS_CONN_LOCATION_UNKNOWN       = 0,    ///< 位置未知
                                                ///< = 1, reserved
    PS3LIB_SAS_CONN_LOCATION_INTERNAL      = 2,    ///< 内部
    PS3LIB_SAS_CONN_LOCATION_EXTERNAL      = 3,    ///< 外部
}Ps3LibSasConnLocationType_e;

/**
 * @brief       获取指定phy的基本信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   phyId: phy索引
 * @param[in]   enclId: phy所属机框号(控制卡会模拟成机框)
 * @param[in]   enclSasAddr: phy所属机框sas地址, 可传入无效码, 表示由接口内部根据enclId查询encl的sas地址
 * @param[out]  phyInfo: phy信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPhyBaseInfoGet(CtrlId_t ctrlId, PhyId_t phyId, EnclId_t enclId, U64 enclSasAddr,
        Ps3LibPhyBaseInfo_t *phyInfo);

/**
 * @brief       设置指定phy的链路速率
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   phyId: phy index
 * @param[in]   enclId: phy所属机框号(控制卡会模拟成机框)
 * @param[in]   enclSasAddr: phy所属机框sas地址, 可传入无效码, 表示由接口内部根据enclId查询encl的sas地址
 * @param[in]   phyLinkspeed: phy的链路速率
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPhyLinkSpeedSet(CtrlId_t ctrlId, PhyId_t phyId, EnclId_t enclId, U64 enclSasAddr, U8 phyLinkspeed);

/**
 * @brief       获取控制卡下phy的列表
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   phyIdListIn: 传入的phy列表, 当传入为空或count为负数时, 向控制卡获取phy列表
 * @param[out]  phyIdListOut: 输出的phy列表
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPhyListFill(CtrlId_t ctrlId, Ps3LibIdList_t *phyIdListOut, const Ps3LibIdList_t *phyIdListIn);

/**
 * @brief       获取指定phy的信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   phyId: phy index
 * @param[in]   enclId: phy所属机框号(控制卡会模拟成机框)
 * @param[in]   enclSasAddr: phy所属机框sas地址, 可传入无效码, 表示由接口内部根据enclId查询encl的sas地址
 * @param[out]  pPhyInfo: phy的信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libPhyInfoGet(CtrlId_t ctrlId, PhyId_t phyId, EnclId_t enclId, U64 enclSasAddr, Ps3LibPhyInfo_t *pPhyInfo);

#if defined(__cplusplus)
}
#endif

#endif
