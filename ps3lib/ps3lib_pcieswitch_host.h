/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_pcieswitch_host.h
 * @date    2025.10.27
 * @brief   switch 主机侧相关接口
 */
#ifndef __PS3LIB_PCIESWITCH_HOST_H__
#define __PS3LIB_PCIESWITCH_HOST_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "ps3lib_ctrl.h"
#include "ps3lib_update.h"
#include "ps3lib_pcieswitch.h"

#define PS3LIB_PCIESW_PORT_MAX                  (116)   ///< pcieswitch Port的最大规格
#define PS3LIB_SW_REVISION_LEVEL_LENGTH         (4)     ///< sw配置文件revisionLevel长度
#define PS3LIB_MAX_PSW_CNT                      (128)   ///< pcie switch最大个数为128
#define PS3LIB_FILE_PATH_NAME                   (1024)  ///< 文件路径字符长度
#define PS3LIB_VERSION_STRING_LENTH             (64)    ///< sw版本信息和时间字符串长度
#define PS3LIB_PRODUCT_NAME_LEN                 (80)    ///< 产品名称长度

/**
 * @brief pcieswitch Port当前信息
 */
typedef struct Ps3LibCtrlPcieSWPortInfo{
    U32 portNum;
    U8  maxWidth;
    U8  maxSpeed;
    U8  grpNum;
    U8  reserved[1];
} Ps3LibCtrlPcieSWPortInfo_t;

/**
 * @brief PcieSWInfo信息结构体
 */
typedef struct Ps3LibCtrlPcieSWInfo{
    S8   nvVendorID[PS3LIB_PRODUCT_NAME_LEN];      ///< Nvdata VendorID
    S8   productID[PS3LIB_PRODUCT_NAME_LEN];       ///< product ID
    S8   platFormID[PS3LIB_PRODUCT_NAME_LEN];      ///< platForm ID
    S8   fwVersion[PS3LIB_FW_VERSION_LEN];         ///< 固件包版本
    S8   buildDate[PS3LIB_FW_VERSION_LEN];         ///< 固件包编译时间
    S8   bootVersion[PS3LIB_FW_VERSION_LEN];       ///< bootloader版本号

    U16 switchID;     ///< swich ID
    U16 tempValid:1;  ///< CHIP sensor是否在位
    S16 tempValue:15; ///< CHIP 温度(单位：摄氏度)
    U16 chipType;     ///< 来自固件设备的芯片类型[0: 48+4, 1:96+8]
    U16 chipID;       ///< 芯片id
    U16 vendorId;
    U16 deviceId;
    U16 subVendorId;
    U16 subDeviceId;
    Ps3LibCtrlPcieSWPortInfo_t portInfo[PS3LIB_PCIESW_PORT_MAX]; ///< 端口信息
    U16 chipTypeSub;
    U16 reserved;
    U8  revisionLevel[PS3LIB_SW_REVISION_LEVEL_LENGTH]; ///< 配置文件字段(Ascii码, 不包含终止符'\0', 例如"00a1")
    U8  reserved1[60];
} Ps3LibCtrlPcieSWInfo_t;

/**
 * @brief 所有版本信息
 */
typedef struct Ps3LibPcieSWVersion {
    S8 vendorID[PS3LIB_PRODUCT_NAME_LEN];          ///< vendor Name
    S8 productID[PS3LIB_PRODUCT_NAME_LEN];         ///< product Name
    S8 platFormID[PS3LIB_PRODUCT_NAME_LEN];        ///< platFormId
    U8 locationID;                                 ///< switch location id, 根据多套sbr对应的gpio获取
    U8 activeUboot;                                ///< Uboot当前分区，0：ub区，0：FW0，1：FW1oot0，1：uboot1
    U8 activeFw;                                   ///< FW当前分
    U8 pad1[5];                                    ///< 对齐
    S8 uBoot0Ver[PS3LIB_VERSION_STRING_LENTH];     ///< uboot0 分区版本号
    S8 uBoot0Build[PS3LIB_VERSION_STRING_LENTH];   ///< uboot0 分区版本编译时间
    S8 uBoot1Ver[PS3LIB_VERSION_STRING_LENTH];     ///< uboot1 分区版本号
    S8 uBoot1Build[PS3LIB_VERSION_STRING_LENTH];   ///< uboot1 分区版本编译时间
    S8 fw0Ver[PS3LIB_VERSION_STRING_LENTH];        ///< fw0 分区版本号
    S8 fw0Build[PS3LIB_VERSION_STRING_LENTH];      ///< fw0 分区版本编译时间
    S8 fw1Ver[PS3LIB_VERSION_STRING_LENTH];        ///< fw1 分区版本号
    S8 fw1Build[PS3LIB_VERSION_STRING_LENTH];      ///< fw1 分区版本编译时间
    S8 activeUbootVer[PS3LIB_VERSION_STRING_LENTH];///< 实际运行的uboot版本，内存获取
    S8 activeFwVer[PS3LIB_VERSION_STRING_LENTH];   ///< 实际运行的fw版本，内存获取
    U32 reserved[4];                               ///< reserved
} Ps3LibPcieSWVersion_t;

/**
 * @brief 获取端口号的性能统计参数
 */
typedef struct Ps3LibSWPortPerfParamH {
    U32 portNum;        ///< 端口号
    U32 ms;             ///< 延时
    U32 it;             ///< 迭代次数 Iterations
} Ps3LibSWPortPerfParamH_t;

/**
 * @brief 获取端口号的性能统计信息
 */
typedef struct Ps3LibSWPortPerfInfoH {
    U32 portNum;          ///< 端口号
    U32 inMsgCnt;         ///< 输入的报文数
    U32 outMsgCnt;        ///< 输t出的报文数
} Ps3LibSWPortPerfInfoH_t;

/**
 * @brief 获取端口号类型
 */
typedef struct Ps3LibSWPortPropParamH {
    U32 portNum;          ///< 端口 号
    U32 type;             ///< 端口 类型
} Ps3LibSWPortPropParamH_t;

/**
 * @brief 通过PORT号获取的属性
 */
typedef struct Ps3LibSWPortNumPropInfo {
    U32 portNum;          ///< 端口 号
    U32 type;             ///< 端口 类型
    U32 clock;            ///< 硬件时钟
    U32 mrr;              ///< MRR
    U32 mps;              ///< MPS
    U32 maxMps;           ///< max MPS
    U8  linkStat;         ///< link state
    U8  width;            ///< 建链宽度
    U8  maxWidth;         ///< 最大宽度
    U8  speed;            ///< 建链速率
    U8  maxSpeed;         ///< 最大速率
    U8  nppStat;          ///< 端口NPP状态
    U8  reserved[2];
} Ps3LibSWPortNumPropInfo_t;

/**
 * @brief 获得指定类型的所有Port的属性
 */
typedef struct Ps3LibSWPortTypePropInfo {
    U32 portNum;            ///< 端口 号
    U32 minorBus;           ///< 次总线号
    U32 bus;                ///< 总线号
    U32 slot;               ///< slot号
    U32 devNum;
    U8  linkStat;
    U8  width;
    U8  maxWidth;
    U8  speed;
    U8  maxSpeed;
    U8  pad[3];
} Ps3LibSWPortTypePropInfo_t;

/**
 * @brief 获取port属性
 */
typedef union Ps3LibSWPortPropInfoH {
    Ps3LibSWPortNumPropInfo_t    portProbByNum;
    Ps3LibSWPortTypePropInfo_t   portProbByType[PS3LIB_PCIESW_PORT_MAX];
} Ps3LibSWPortPropInfoH_t;

/**
 * @brief PcieSW 端口链路状态信息
 */
typedef struct Ps3LibSWPortLinkStatH {
    U32 portNum;
    U16 dieiID;
    U16 groupID;
    U8 portID;
    U8 linkStat;
    U8 reserved;
    U8 pad[1];
} Ps3LibSWPortLinkStatH_t;

/**
 * @brief  所有端口链路状态信息结构体
 */
typedef struct Ps3LibSWAllPortLinkInfoH{
    Ps3LibSWPortLinkStatH_t portLinkStat[PS3LIB_PCIESW_PORT_MAX]; ///< 端口信息
} Ps3LibSWAllPortLinkInfoH_t;

/**
 * @brief  获取所有的端口的建链信息参数
 */
typedef struct Ps3LibSWAllPortLinkParamH {
    U32 type;
} Ps3LibSWAllPortLinkParamH_t;

/**
 * @brief 配置PCIe Switch的端口 reconfig
 */
typedef struct Ps3LibSWPortReconfig {
    U16 grpid;  ///< switch GROUP的ID号
    U16 mode;   ///< switch 配置的模式
    U32 reservd;
} Ps3LibSWPortReconfig_t;

/**
 * @brief PcieSWHostView 端口信息结构体
 */
typedef struct Ps3LibSWPortHostView {
    U32 portNum;
    U16 devID;
    U16 venID;
    U8  bus;
    U8  devNum;
    U8  function;
    U8  linkStat;
    U8  maxWidth;
    U8  width;
    U8  maxSpeed;
    U8  speed;
    U32 mrr;
    U32 mps;
} Ps3LibSWPortHostView_t;

/**
 * @brief PcieSWHostView 信息结构体
 */
typedef struct Ps3LibSWHostViewH {
    Ps3LibSWPortHostView_t portHostView[PS3LIB_PCIESW_PORT_MAX]; ///< 端口信息
} Ps3LibSWHostViewH_t;

/**
 * @brief 查询PCIe Switch的lane fom info查询参数
 */
typedef struct Ps3LibLaneInfoParam {
    U16 groupID;    ///< switch group的ID号
    U8 pad[6];      ///< 对齐
} Ps3LibLaneInfoParam_s;

/**
 * @brief PCIe Switch的单个lane fom info的结果
 */
typedef struct Ps3LibLaneInfoSingle {
    U8 laneID;          ///< switch group的ID号
    U8 fom;             ///< fom值
    U8 reserved[14];    ///< 预留
} Ps3LibLaneInfoSingle_s;

/**
 * @brief PCIe Switch的lane fom info的查询结果
 */
typedef struct Ps3LibLaneInfoRsp {
    U16 groupID;                         ///< switch group的ID号
    U8 laneCnt;                          ///< lane的数量
    U8 pad[5];                           ///< 对齐
    Ps3LibLaneInfoSingle_s laneInfo[32]; ///< 预留
} Ps3LibLaneInfoRsp_s;

/**
 * @brief 查询PCIe Switch的lane clock参数
 */
typedef struct Ps3LibLaneClockPara {
    U16 groupID;    ///< switch group的ID号
    U8 laneID;      ///< switch lane的ID号
    U8 pad[5];      ///< 预留
} Ps3LibLaneClockPara_s;

/**
 * @brief 查询PCIe Switch的lane clock结果
 */
typedef struct Ps3LibLaneClockInfo {
    U16 groupID;   ///< switch group的ID号
    U8 laneID;     ///< switch lane的ID号
    U32 clock;     ///< 单位MHz
    U8 pad[9];     ///< 预留
} Ps3LibLaneClockInfo_s;

/**
 * @brief switch NPP mode 枚举
 */
typedef enum Ps3LibSWNPPModeE {
    PS3LIB_SWITCH_NPP_MODE_OFF = 0,
    PS3LIB_SWITCH_NPP_MODE_ON,
    PS3LIB_SWITCH_NPP_MODE_NR,
} Ps3LibSWNPPModeE_e;

/**
 * @brief switch NPPMode交互结构体
 */
typedef struct Ps3LibSWNPPMode {
    U8 mode;            ///< NPP mode ,其值为 Ps3LibSWNPPModeE_e
    U8 pad[7];          ///< 对齐
} Ps3LibSWNPPMode_t;

/**
 * @brief 每个端口的error counters的结构体
 */
typedef struct Ps3LibPcieSWPortErrCounters{
	U32 portNum;            ///< 端口号
    U32 receiverErrors;     ///< PCI Express® Base Specification Revision 6.0 7.8.4.5 Correctable Error Status Register
    U32 badTLPErrors;       ///< Bad TLP Status
    U32 badDLLPErrors;      ///< Bad DLLP Status
    U32 replayNumberRolloverErrors; ///< REPLY_NUM Rollover Status
    U32 replayTimeoutErrors;        ///< Replay Timer Timeout Status
    U32 linkDownCount;          ///< link down 计数
    U32 reserved[3];
} Ps3LibPcieSWPortErrCounters_t;

/**
 * @brief 全部端口的error counters的结构体
 */
typedef struct Ps3LibPcieSWErrCounters{
    U32 portCnt;                                    ///< sw的port数量
    U32 pad;                                        ///< 对齐
	Ps3LibPcieSWPortErrCounters_t errCounters[0];   ///< 每个port对应的error counters
} Ps3LibPcieSWErrCounters_t;

/**
 * @brief   switch insight mode 枚举
 */
typedef enum Ps3LibSwInsightMode {
    PS3LIB_SWITCH_INSIGHT_MODE_OFF = 0,
    PS3LIB_SWITCH_INSIGHT_MODE_ON,
    PS3LIB_SWITCH_INSIGHT_MODE_NR,
} Ps3LibSwInsightMode_e;

/*
 * brief switch insightmode交互结构体
 */
typedef struct Ps3LibSwitchInsightMode {
    Ps3LibSwInsightMode_e mode;
} Ps3LibSwitchInsightMode_t;

/**
 *  @brief Switch Port Type主机侧
 */
typedef enum Ps3LibSwHostPortType {
    PS3LIB_SWITCH_HOST_PORT_TYPE_DP = 1,    ///< dp 端口
    PS3LIB_SWITCH_HOST_PORT_TYPE_UP,        ///< up 端口
    PS3LIB_SWITCH_HOST_PORT_TYPE_NT,        ///< nt 端口
    PS3LIB_SWITCH_HOST_PORT_TYPE_UNKNOWN,
} Ps3LibSwHostPortType_e;

/**
 * @brief 启动分区枚举值
 * 
 */
typedef enum Ps3LibCtrlEnablePosition {
    PS3LIB_CTRL_ENABLEPOSITION_FW0  = 0,
    PS3LIB_CTRL_ENABLEPOSITION_FW1,      
    PS3LIB_CTRL_ENABLEPOSITION_NR,     
} Ps3LibCtrlEnablePosition_e;

typedef struct Ps3LibEnablePosition {
    U32 activefw;   ///< 分区值 
    U8  pad[64];
} Ps3LibEnablePosition_t;

/**
 * @brief   固件擦除设备类型
 */
typedef enum Ps3LibUpdateEraseType{
    PS3LIB_UPDATE_ERASE_INVALID       = 0,
    PS3LIB_UPDATE_ERASE_SINGLE,
    PS3LIB_UPDATE_ERASE_ALL,
    PS3LIB_UPDATE_ERASE_MAX,
} Ps3LibUpdateEraseType_e;

/**
 * @brief  flash擦除信息
 */
typedef struct Ps3LibSWFlashErase {
    Ps3LibUpdateDevType_e devType;      ///< 设备类型
    Ps3LibUpdateFwType_e fwType;        ///< image类型
    Ps3LibUpdateEraseType_e erashType;  ///< 擦除flash分区(全都/单个)
    S8 fileName[PS3LIB_FILE_PATH_NAME]; ///< Bootloader image file
    U8 excludemfg;                      ///< 是否保留制造商数据
    U8 pad[3];                          ///< 对齐
} Ps3LibSWFlashErase_t;

/**
 * @brief       获取pcie switch的个数
 * @param[out]  pPswCount: switch的数量
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwCountGet(U32 *pPswCount);

/**
 * @brief       获取switch列表
 * @param[out]  pSwIdList 输出参数
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libFillSwList(Ps3LibIdList_t *pSwIdList);

/**
 * @brief       获取指定switch的基本信息
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pcieSwInfo:pciesw的基本信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwPcieInfoGet(CtrlId_t devId, Ps3LibCtrlPcieSWInfo_t *pPcieSwInfo);

/**
 * @brief       switch 带内升级命令
 * @param[in]   devId:  控制卡标识符
 * @param[out]  pDownloadInfo:升级相关信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwUpdateByHost(CtrlId_t devId, Ps3LibDownloadInfo_t *pDownloadInfo);

/**
 * @brief       switch 复位
 * @param[in]   devId:  控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwIocRestart(CtrlId_t devId);

/**
 * @brief       获取所有mfginfo的配置
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pMfgInfo: 所有mfgdata配置信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwMfgInfoCfgGet(CtrlId_t devId, Ps3LibMfgInfo_t *pMfgInfo);

/**
 * @brief       获取指定控制卡上pciesw的Port Perf属性信息
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pPortParam:Port Perf属性查询信息
 * @param[out]  pPortPerfInfo:Port Perf的基本信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwPortPerfInfoGet(CtrlId_t devId, Ps3LibSWPortPerfParamH_t *pPortParam,
                                      Ps3LibSWPortPerfInfoH_t *pPortPerfInfo);

/**
 * @brief       获取指定控制卡上pciesw的Port TYPE属性信息
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pPortParam: port参数
 * @param[out]  pPciePortPorpInfo:pciesw的基本信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwPortTypePropInfoGet(CtrlId_t devId, Ps3LibSWPortPropParamH_t *pPortParam,
                                          Ps3LibSWPortPropInfoH_t *pPciePortPorpInfo);

/**
 * @brief       获取指定控制卡上pciesw的PortNum 属性信息
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pPortParam: 端口信息
 * @param[out]  pPortPorpInfo:pciesw的基本信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwPortNumPropInfoGet(CtrlId_t devId, Ps3LibSWPortPropParamH_t *pPortParam,
                                         Ps3LibSWPortPropInfoH_t *pPortPorpInfo);

/**
 * @brief       获取指定控制卡上pciesw的Port 建链状态信息
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pLinkParam: 类型参数
 * @param[out]  pAllPortLinkStat:pciesw的建链信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwAllPortLinkStatGet(CtrlId_t devId, Ps3LibSWAllPortLinkParamH_t *pLinkParam,
                                         Ps3LibSWAllPortLinkInfoH_t *pAllPortLinkStat);

/**
 * @brief       设置PCIeSwitch的port reconfig
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pPortReconfig:pciesw的基本信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwPortReconfigSet(CtrlId_t devId, Ps3LibSWPortReconfig_t *pPortReconfig);

/**
 * @brief       查看hostview信息
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pHostView:host view信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwHostViewGet(CtrlId_t devId, Ps3LibSWHostViewH_t *pHostView);

/**
 * @brief        显示pcie switch time
 * @param[in]    pswId: 控制卡标识符
 * @param[out]   pTmpInfo : time U64指针，毫秒级
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwTimeGet(CtrlId_t devId, U64 *pTmpInfo);

/**
 * @brief        设置pcie switch time
 * @param[in]    devId: 控制卡标识符
 * @param[out]   time : time 毫秒级
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwTimeSet(CtrlId_t devId, U64 time);

/**
 * @brief        查询 uptime
 * @param[in]    devId: 控制卡标识符
 * @param[out]   pUptime: U64指针
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwUpTimeGet(CtrlId_t devId, U64 *pUpTime);

/**
 * @brief       删除switch termlog
 * @param[in]   devId: 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwTermLogDel(CtrlId_t devId);

/**
 * @brief       设置mfgInfo的配置
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pComboPhyCfg: combo phy配置字段
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwMfgInfoCfgSet(CtrlId_t devId, Ps3LibSwMfgInfo_t *pMfgInfoCfg);

/**
 * @brief       擦除flash
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pEraseFlashInfo: 指定擦除信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwFlashErase(CtrlId_t devId, Ps3LibSWFlashErase_t *pEraseFlashInfo);

/**
 * @brief       上传命令下发
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pUploadInfo, 用户输入的上载信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwUpdateUpLoad(CtrlId_t devId, Ps3LibDownloadInfo_t *pUploadInfo);

/**
 * @brief       获取termlog内容
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pTermLogReq: termlog查询参数
 * @param[out]  pTermlogInfo: termlog信息
 * @param[out]  termlogInfoSize: termlog允许返回的最大值
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwTermlogGet(CtrlId_t devId, Ps3LibCtrlTermLogReq_t *pTermLogReq,
                            Ps3LibCtrlTermLogInfo_t *pTermlogInfo, S32 termlogInfoSize);

/**
 * @brief       删除pcie switch的事件日志
 * @param[in]   devId: 控制卡标识符
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwEventLogsDel(CtrlId_t devId);

/**
 * @brief       设置启动分区
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pEnablePosition: 启动分区
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwEnablePositionSet(CtrlId_t devId, Ps3LibEnablePosition_t *pEnablePosition);

/**
 * @brief       查询启动分区
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pEnablePosition: 启动分区
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwEnablePositionGet(CtrlId_t devId, Ps3LibEnablePosition_t *pEnablePosition);

/**
 * @brief       设置switch insightmode
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pInsightMode: 眼图模式
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwInsightModeSet(CtrlId_t devId, Ps3LibSwitchInsightMode_t *pInsightMode);

/**
 * @brief       查询switch insightmode
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pInsightMode: 眼图模式
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwInsightModeGet(CtrlId_t devId, Ps3LibSwitchInsightMode_t *pInsightMode);

/**
 * @brief       查询flash分区数量
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pFlashRegionCnt: 分区数量
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwFlashRegionCntGet(CtrlId_t devId, U8 *pFlashRegionCnt);

/**
 * @brief       查询flash分区信息
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pFlashTblInfo: flash分区信息
 * @param[in]   pFlashTblInfoSize: flash分区大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwFlashTblInfoGet(CtrlId_t devId, Ps3LibPcieSWFlashTblInfo_t *pFlashTblInfo,
                                  size_t *pFlashTblInfoSize);

/**
 * @brief       清除端口错误数量
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pPortNum: 端口号
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwErrCountersDel(CtrlId_t devId, U32 *pPortNum);

/**
 * @brief       查询端口数量
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pPortCnt: 端口数量
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwPortCntGet(CtrlId_t devId, U16 *pPortCnt);

/**
 * @brief       查询指定端口错误数量
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pPortNum: port数量
 * @param[out]  pErrCounters: 错误数量信息
 * @param[in]   pErrCountersSize: pPswShowErrCounters的大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwErrCountersShow(CtrlId_t devId, U32 *pPortNum, 
            Ps3LibPcieSWErrCounters_t *pErrCounters, U32 errCountersSize);

/**
 * @brief       查询npp超时时间
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pNPPTime: npp超时时间
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwNPPTimeoutShow(CtrlId_t devId, U32 *pNPPTime);

/**
 * @brief       设置npp超时时间
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pNPPTime: npp超时时间
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwNPPTimeoutSet(CtrlId_t devId, U32 *pNPPTime);

/**
 * @brief       端口重置
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pPortNum: 指定端口
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwPortResetSet(CtrlId_t devId, U32 *pPortNum);

/**
 * @brief       获取ltssm信息
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pLtssmReq: ltssm查询参数
 * @param[out]  pLtssmRsp: ltssm信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwLtssmInfoShow(CtrlId_t devId, Ps3LibSWPortLtssmPara_s *pLtssmReq, 
                                Ps3LibSWPortLtssmInfoResp_s *pLtssmRsp);

/**
 * @brief       获取sw的 sensor cnt
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pSensorCnt: 传感器数量
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwSensorCntGet(CtrlId_t devId, U16 *pSensorCnt);

/**
 * @brief       获取sw的 sensor 信息
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pSensor: 传感器信息
 * @param[out]  pSensorSize: 传感器大小
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwSensorInfoGet(CtrlId_t devId, Ps3LibSWSensorInfo_t *pSensor, size_t *pSensorSize);

/**
 * @brief       设置switch NPP mode
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pNPPMode: npp模式
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwNPPModeSet(CtrlId_t devId, Ps3LibSWNPPMode_t *pNPPMode);

/**
 * @brief       查询switch NPP mode
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pNPPMode：npp模式
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwNPPModeGet(CtrlId_t devId, Ps3LibSWNPPMode_t *pNPPMode);

/**
 * @brief       获取switch中lane clock
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pLaneClockPara, switch中lane clock 参数
 * @param[out]  pLaneClockInfo, switch中lane clock 结果
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwLaneClockInfoGet(CtrlId_t devId, Ps3LibLaneClockPara_s *pLaneClockPara,
                               Ps3LibLaneClockInfo_s *pLaneClockInfo);

/**
 * @brief       查询PCIe Switch的lane fom info查询参数
 * @param[in]   devId: 控制卡标识符
 * @param[in]   pLaneInfoPara: lane info group信息
 * @param[out]  pLaneInfoRsp: lane info信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwLaneInfoGet(CtrlId_t devId, Ps3LibLaneInfoParam_s *pLaneInfoPara,
                               Ps3LibLaneInfoRsp_s *pLaneInfoRsp);

/**
 * @brief        查询日志持久化信息
 * @param[in]    devId: 控制卡标识符
 * @param[out]   pPersistInfo: 日志的信息
 * @return       PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwEvtlogPerGet(CtrlId_t devId, Ps3LibEvtPersistInfo_s *pPersistInfo);

/**
 * @brief       读取控制卡的所有事件日志
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   sinceSeqNum: 读取的事件日志起始序列号
 * @param[out]  ppEvtLog: 读取到的事件日志信息
 * @warning     输出的ppEvtLog需要由调用者调用mgmEventLogDestroy释放
 * @return      PS3_ERRNO_SUCCESS: 成功
 *              PS3LIB_ERRNO_EVENT_BUSY: 事件日志系统正忙, 请稍后获取
 *              PS3LIB_ERRNO_MALLOC_FAILED: 内存申请失败
 *              PS3LIB_ERRNO_WRONG_EVENT_LOG_DATA: 读取到错误的事件日志数据
 *              PS3LIB_ERRNO_WRONG_EVENT_LOG_INFO:
 * 读取到错误的事件日志持久化信息
 */
Ps3Errno ps3libSwEventLogGet(CtrlId_t devId, U32 sinceSeqNum, Ps3LibEvtLog_s **ppEvtLog);

/**
 * @brief       与ps3libSwEventLogGet函数成对使用,释放由ps3libSwEventLogGet生成的事件日志结构体
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   ppEvtLog: 需要释放事件日志信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
void ps3libEventLogDestroy(Ps3LibEvtLog_s *pEvtLog);

/**
 * @brief       获取PCIe Switch address信息
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pPciInfo: 获取到的PCIe信息
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwAddressInfoGet(CtrlId_t devId, Ps3LibSystemPciInfo_t *pPciInfo);

/**
 * @brief       获取switch的版本信息
 * @param[in]   devId: 控制卡标识符
 * @param[out]  pVersion 版本信息 
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libSwVersionInfoShow(CtrlId_t devId, Ps3LibPcieSWVersion_t *pVersion);

#if defined(__cplusplus)
}
#endif

#endif