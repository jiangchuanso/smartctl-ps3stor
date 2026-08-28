/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_encl.h
 * @date    2023.05.24
 * @brief   背板级别数据结构与接口
 */
#ifndef __PS3LIB_ENCL_H__
#define __PS3LIB_ENCL_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "ps3lib_ctrl.h"
#include "ps3lib_phy.h"

#define PS3LIB_MAX_ENCL_NUM                (256)    ///< 最大背板数量
#define PS3LIB_ENCL_SAS_COUNT  PS3LIB_CTRL_SAS_COUNT    ///< 控制卡sas数量
#define PS3LIB_ENCL_CONNECTID_MAX_CNT      (5)      ///< 背板最大连接数
#define PS3LIB_ENCL_SERIAL_NUM_LEN         (24)     ///< 背板序列号长度
#define PS3LIB_ENCL_VENDOR_ID_LEN          (9)      ///< 背板厂商Id长度
#define PS3LIB_ENCL_PRODUCT_ID_LEN         (17)     ///< 背板设备型号Id长度
#define PS3LIB_ENCL_PRODUCT_REV_LEN        (8)      ///< 背板版本号长度
#define PS3LIB_VENDOR_SPECIFIC_LEN         (20)     ///< 背板供应商信息长度
#define PS3LIB_ENCL_FRU_PN_LEN             (32)     ///< FRU Part Number 长度
#define PS3LIB_ENCL_MAX_PHY_NUM            (48)     ///< phy数量
#define PS3LIB_ENCL_MAX_PORT_NUM           (2)      ///< 连接器数量
#define PS3LIB_ENCL_CONNECTOR_NAME_LEN     (16)     ///< 连接器名称长度
#define PS3LIB_MAX_ENCL_STATUS_LEN         (4096)   ///< 背板状态字符串长度
#define PS3LIB_CTRL_SIMU_ENCL_ID           (0)      ///< 模拟背板Id
#define PS3LIB_MAX_ENCL_ELEMENT_COUNT      (1024)   ///< 最大element数量
#define PS3LIB_MAX_ENCL_ELEMENT_TYPE_NUM   (32)     ///< encl元器件种类32种，目前枚举种类已有28个

/**
 * @brief   encl元件类型枚举
 */
typedef enum Ps3LibSesElmtTypeCode {
    PS3LIB_SES_ELMT_UNKNOWN                        = 0x00,  ///< 未知
    PS3LIB_SES_ELMT_DEVICE                         = 0x01,  ///< Device
    PS3LIB_SES_ELMT_POWER_SUPPLY                   = 0x02,  ///< Power Supply
    PS3LIB_SES_ELMT_COOLING                        = 0x03,  ///< Fan
    PS3LIB_SES_ELMT_TEMPERATURE_SENSOR             = 0x04,  ///< Temperature Sensor
    PS3LIB_SES_ELMT_DOOR_LOCK                      = 0x05,  ///< Door Lock
    PS3LIB_SES_ELMT_AUDIBLE_ALARM                  = 0x06,  ///< Alarm
    PS3LIB_SES_ELMT_SES_CONTROLLER_ELECTRONICS     = 0x07,  ///< Controller Electronics (SIM)
    PS3LIB_SES_ELMT_SCC_CONTROLLER_ELECTRONICS     = 0x08,  ///< Controller Electronics (SCC)
    PS3LIB_SES_ELMT_NONVOLATILE_CACHE              = 0x09,  ///< Non-volatile Cache
    PS3LIB_SES_ELMT_INVALID_OPERATION_REASON       = 0x0A,  ///< Invalid Operation Reason
    PS3LIB_SES_ELMT_UNINTERRUPTABLE_POWER_SUPPLY   = 0x0B,  ///< Uninterruptable Power Supply
    PS3LIB_SES_ELMT_DISPLAY                        = 0x0C,  ///< Display
    PS3LIB_SES_ELMT_KEY_PAD_ENTRY                  = 0x0D,  ///< Key Pad Entry
    PS3LIB_SES_ELMT_ENCLOSURE                      = 0x0E,  ///< Enclosure
    PS3LIB_SES_ELMT_SCSI_TRANSCEIVER_PORT          = 0x0F,  ///< SCSI Transceiver Port
    PS3LIB_SES_ELMT_LANGUAGE                       = 0x10,  ///< Language
    PS3LIB_SES_ELMT_COMMUNICATION_PORT             = 0x11,  ///< Communication Port
    PS3LIB_SES_ELMT_VOLTAGE_SENSOR                 = 0x12,  ///< Voltage Sensor
    PS3LIB_SES_ELMT_CURRENT_SENSOR                 = 0x13,  ///< Current Sensor
    PS3LIB_SES_ELMT_SCSI_TARGET_PORT               = 0x14,  ///< SCSI Target Port
    PS3LIB_SES_ELMT_SCSI_INITIATOR_PORT            = 0x15,  ///< SCSI Initiator Port
    PS3LIB_SES_ELMT_SIMPLE_SUB_ENCLOSURE           = 0x16,  ///< Sub Enclosure
    PS3LIB_SES_ELMT_ARRAY_DEVICE                   = 0x17,  ///< Slot Device
    PS3LIB_SES_ELMT_SAS_EXPANDER                   = 0x18,  ///< SAS EXPANDER
    PS3LIB_SES_ELMT_SAS_CONNECTOR                  = 0x19,  ///< SAS CONNECTOR
    PS3LIB_SES_ELMT_VENDOR_SPECIFIC_START_01       = 0x80,  ///< Vendor Specific
    PS3LIB_SES_ELMT_VENDOR_SPECIFIC_END_01         = 0xFF,  ///< Vendor Specific End
} Ps3LibSesElmtTypeCode_t;

/**
 * @brief   encl元件状态枚举
 */
typedef enum Ps3LibEnclElementStatusType{
    PS3LIB_ENCL_ELEM_STATUS_UNSUPPORTED        = 0x00,  ///< 0x0 Element is unsupported. new for SES spec.
    PS3LIB_ENCL_ELEM_STATUS_OK                 = 0x01,  ///< 0x1 Element is installed,no error conditions are known.
    PS3LIB_ENCL_ELEM_STATUS_CRITICAL           = 0x02,  ///< 0x2 Critical condition is detected.
    PS3LIB_ENCL_ELEM_STATUS_NON_CRITICAL       = 0x03,  ///< 0x3 Noncritical condition is detected.
    PS3LIB_ENCL_ELEM_STATUS_UNRECOVERABLE      = 0x04,  ///< 0x4 Unrecoverable condition is detected.
    PS3LIB_ENCL_ELEM_STATUS_NOT_INSTALLED      = 0x05,  ///< 0x5 Element is not installed in enclosure.
    PS3LIB_ENCL_ELEM_STATUS_UNKNOWN            = 0x06,  ///< 0x6 Sensor has failed or element status is not available.
    PS3LIB_ENCL_ELEM_STATUS_NOT_AVAILABLE      = 0x07,  ///< 0x7 Element installed, no known errors
    PS3LIB_ENCL_ELEM_STATUS_NO_ACCESS_ALLOWED  = 0x08,  ///< 0x8 The enclosure is expected to respond
}Ps3LibEnclElementStatusType_t;

/**
 * @brief   encl Fan Speed枚举
 */
typedef enum Ps3LibFanSpeed {
    PS3LIB_FAN_SPEED_STOP           = 0,    ///< fan speed stop
    PS3LIB_FAN_SPEED_LOW            = 1,    ///< fan speed low
    PS3LIB_FAN_SPEED_SECOND_LOW     = 2,    ///< fan speed second low
    PS3LIB_FAN_SPEED_THIRD_LOW      = 3,    ///< fan speed third low
    PS3LIB_FAN_SPEED_MEDIUM         = 4,    ///< fan speed MEDIUM
    PS3LIB_FAN_SPEED_THIRD_HIGH     = 5,    ///< fan speed third high
    PS3LIB_FAN_SPEED_SECOND_HIGH    = 6,    ///< fan speed second high
    PS3LIB_FAN_SPEED_HIGH           = 7,    ///< fan speed high
} Ps3LibFanSpeed_e;

/**
 * @brief   encl设备协议类型枚举
 */
typedef enum Ps3LibEnclProtocolType{
    PS3LIB_ENCL_PROTOCOL_TYPE_UNKNOWN  = 0,    ///< 未知
    PS3LIB_ENCL_PROTOCOL_TYPE_SES      = 1,    ///< SES
    PS3LIB_ENCL_PROTOCOL_TYPE_SGPIO    = 2,    ///< SGPIO
    PS3LIB_ENCL_PROTOCOL_TYPE_SAFTE    = 3,    ///< SAFTE
    PS3LIB_ENCL_PROTOCOL_TYPE_SC       = 4,    ///< SC
    PS3LIB_ENCL_PROTOCOL_TYPE_NVME     = 5,    ///< NVME Switch
}Ps3LibEnclProtocolType_e;

/**
 * @brief   encl设备类型枚举
 */
typedef enum Ps3LibEnclDeviceType{
    PS3LIB_ENCL_DEVICE_TYPE_UNKNOWN    = 0,    ///< 未知
    PS3LIB_ENCL_DEVICE_TYPE_BACKPLANE  = 1,    ///< 背板
    PS3LIB_ENCL_DEVICE_TYPE_ENCLOSURE  = 2,    ///< 机箱
}Ps3LibEnclDeviceType_e;

/**
 * @brief   encl列表
 */
typedef struct Ps3LibEnclList{
    U16     count;                              ///< encl个数
    U8      enclId[PS3LIB_MAX_ENCL_NUM];        ///< encl设备号柔性数组
}Ps3LibEnclList_t;

typedef struct Ps3LibElmtTypeHeader{
    U8      elmtType;                       ///< element类型
    U8      elmtNum;                        ///< 该类型的element的个数  最大元器件数量为16(按协议文档)
    U8      subEnclID;                      ///< 次enclosure ID
    U8      rsvd;                           ///< 预留
} Ps3LibElmtTypeHeader_s;

typedef struct Ps3LibElmtStatInfo {
    Ps3LibElmtTypeHeader_s     header;      ///< element信息
    U32                     status[0];      ///< enclosure status dp(02)页中 elemt 对应的 status 描述信息
} Ps3LibElmtStatInfo_s;

typedef struct Ps3LibEnclStatMsgEx {
    U32                    typeHeaderCnt;   ///< 元素数量 枚举Ps3LibSesElmtTypeCode，目前是28种。
    ///< 每个info距Ps3LibEnclStatMsgEx_s头的偏移，32时type的个数保证够用
    U32                   elmtOffset[PS3LIB_MAX_ENCL_ELEMENT_TYPE_NUM];
    Ps3LibElmtStatInfo_s         info[0];   ///< 元素信息
} Ps3LibEnclStatMsgEx_s;

/**
 * @brief   encl中slot和pdflatId的映射关系
 */
typedef struct Ps3LibPdSlotMap {
    U16 slotId;     ///< 槽位Id
    U16 pdFlatId;   ///< pdFlat索引
} Ps3LibPdSlotMap_t;

/**
 * @brief   connectId结构体
 */
typedef struct Ps3LibEnclConnect {
    U8 connectId;       ///< 连接器ID
    U8 subconnectId;    ///< 副连接器ID
} Ps3LibEnclConnect_t;

/**
 * @brief   encl信息结构体,供上层使用
 */
typedef struct Ps3LibEnclBasicInfo {
    U8      enclId;             ///< 背板ID
    U8      deviceType;         ///< 设备类型
    U8      protocolType;       ///< 协议类型
    U8      state;              ///< 状态
    U8      enclPosition;       ///< 级联深度
    U8      ctrlPhyCnt;         ///< 接在卡上的phy个数
    U8      enclPhyCnt;         ///< encl上的phy个数,id为index
    U8      slotOffset;         ///< 槽位号起始偏移
    U8      FRUPartNum;         ///< 废弃字段
    U8      zoningMode;         ///< 分区模式
    U16     devId :12;          ///< not support
    U16     channelId :4;       ///< 所属通道ID
    U16     pdCount;            ///< 硬盘数
    U16     deviceId;           ///< 背板ID
    U16     partnerDeviceId;    ///< 双主控情况下,另一个encl的Id
    U8      phyId[PS3LIB_CTRL_MAX_PHY_CNT];            ///< encl接在卡上的phyId
    U32     sasActiveMap : PS3LIB_ENCL_SAS_COUNT;      ///< sas接口是否使能的bitmap
    U32     pad : (32 - PS3LIB_ENCL_SAS_COUNT);        ///< 预留
    U64     enclSasAddr[PS3LIB_ENCL_SAS_COUNT-1];   ///< enclosure logical identifier (框的sas地址)
    U64     enclSesSasAddr;                         ///< 框虚拟成ses盘的sas地址
    U8      connectCnt;                                ///< 连接器数量
    Ps3LibEnclConnect_t    connect[PS3LIB_ENCL_CONNECTID_MAX_CNT]; ///< connect的connectId和subconnectId
    U8      pad2[5];                                    ///< 预留
    Ps3LibPdSlotMap_t  pdMap[PS3LIB_MAX_PD_PER_ENCL];   ///< 硬盘ID列表
    U8      pad3[4];                                    ///< 预留  
} Ps3LibEnclBasicInfo_t;

/**
 * @brief   背板数据
 * @note    字符串可能不包含终止符'\0'
 */
typedef struct Ps3LibEnclInquiryData {
    S8      serialNumber[PS3LIB_ENCL_SERIAL_NUM_LEN];          ///< 序列号
    S8      vendorID[PS3LIB_ENCL_VENDOR_ID_LEN];               ///< 厂商
    S8      productID[PS3LIB_ENCL_PRODUCT_ID_LEN];             ///< 设备型号
    S8      productRevLevel[PS3LIB_ENCL_PRODUCT_REV_LEN];      ///< 版本号
    S8      vendorSpecificInfo[PS3LIB_VENDOR_SPECIFIC_LEN];    ///< 供应商特定信息
    S8      FRUPartNumber[PS3LIB_ENCL_FRU_PN_LEN];             ///< FRU Part Number
    S8      pad[2];                                            ///< 预留
} Ps3LibEnclInquiryData_t;

/**
 * @brief   encl Connector信息(从设备管理获取)
 */
typedef struct Ps3LibEnclConnector {
    U8      location;                                       ///< 连接的位置(1:unknown,2:internal,3:external)
    U8      phyCnts;                                        ///< phy的个数
    U8      phyId[PS3LIB_ENCL_MAX_PHY_NUM];                 ///< phyId数组
    U8      connectName[PS3LIB_ENCL_CONNECTOR_NAME_LEN];    ///< 连接器名称(注意:字符串可能不包含终止符'\0')
} Ps3LibEnclConnector_t;

/**
 * @brief   encl connector列表(encl在根上的连接)
 */
typedef struct Ps3LibEnclConnectorList{
    U8                     connectorCnts;                            ///< 连接器数量
    Ps3LibEnclConnector_t  enclConnector[PS3LIB_ENCL_MAX_PORT_NUM];  ///< 连接器信息
} Ps3LibEnclConnectorList_t;

/**
 * @brief   encl全部信息
 */
typedef struct Ps3LibEnclInfo {
    Ps3LibEnclBasicInfo_t          enclBaseInfo;      ///< encl信息结构体,供上层使用
    Ps3LibEnclInquiryData_t        enclInquiryData;   ///< encl Inquiry Data(从设备管理获取)
    Ps3LibEnclConnectorList_t      enclConnectorList; ///< encl connector列表(encl在根上的连接)
    U8 pad[11];                                        ///< 预留
    union {
        Ps3LibEnclStatMsgEx_s      enclStatMsg;                         ///< 机框所有元件信息
        U8                         buff[PS3LIB_MAX_ENCL_STATUS_LEN];    ///< 机框状态buffer
    };
} Ps3LibEnclInfoEx_t;

/**
 * @brief   encl各部件数量
 */
typedef struct Ps3LibEnclElmtCnt {
    U32 optimalCnt;
    U32 criticalCnt;
    U32 nonCriticalCnt;
    U32 unRecoverableCnt;
    U32 notInstalCnt;
    U32 notAvailCnt;
    U32 unknownCnt;
} Ps3LibEnclElmtCnt_t;

/**
 * @brief       获取控制卡下机框列表
 * @param[in]   ctrlId: 控制卡标识符
 * @param[out]  enclList: 输出的encl列表
 * @return      PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libEnclListGetByCtrlId(CtrlId_t ctrlId, Ps3LibEnclList_t *enclList);

/**
 * @brief       获取指定机框的元件状态信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   enclId: 机框号
 * @param[in]   len: 外部为status申请的内存大小
 * @param[out]  status: 柔型数组, 机框的元件状态信息
 * @return      PS3_ERRNO_SUCCESS: 成功
*/
Ps3Errno ps3libEnclStatusGet(CtrlId_t ctrlId, EnclId_t enclId, Ps3LibEnclStatMsgEx_s *status, U32 len);

/**
 * @brief       从机框所有元件信息中提取指定元件的信息
 * @param[in]   pSesStatus: 机框所有元件信息
 * @param[in]   etc: 机框元件类型, enum Ps3LibSesElmtTypeCode_t
 * @param[out]  pElmtStatus: 元件状态数组 struct SesStatElmt_s
 * @param[in]   len: pElmtStatus指向内存长度
 * @return      U32: 该类元件个数
 * @warning     pElmtStatus由调用者申请内存，使用完毕由调用者释放。
 *              pElmtStatus大小至少为(elmtCount * sizeof(*pElmtStatus)),elmtCount通过mgmGetSesElmtCount查询
*/
U32 ps3libEnclSesElmtStatusGet(Ps3LibEnclStatMsgEx_s *pSesStatus,
                               Ps3LibSesElmtTypeCode_t etc, U32 *pElmtStatus,
                               U32 len);

/**
 * @brief       从机框所有元件信息中提取指定元件的个数
 * @param[in]   pSesStatus: 机框所有元件信息
 * @param[in]   etc: 机框元件类型, enum Ps3LibSesElmtTypeCode_t
 * @return      U32: 元件个数
 * @warning
*/
U32 ps3libEnclSesElmtCountGet(Ps3LibEnclStatMsgEx_s *pSesStatus, Ps3LibSesElmtTypeCode_t etc);

/**
 * @brief       获取指定机框的基本信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   enclId: 机框号
 * @param[out]  baseInfo: 机框的基本信息
 * @return      PS3_ERRNO_SUCCESS: 成功；其他: 失败
*/
Ps3Errno ps3libEnclBaseInfoGet(CtrlId_t ctrlId, EnclId_t enclId, Ps3LibEnclBasicInfo_t *baseInfo);

/**
 * @brief       获取指定机框的信息
 * @param[in]   ctrlId: 控制卡标识符
 * @param[in]   enclId: 机框号
 * @param[out]  enclInfo: 机框信息
 * @return      PS3_ERRNO_SUCCESS: 成功
*/
Ps3Errno ps3libEnclInfoGet(CtrlId_t ctrlId, EnclId_t enclId, Ps3LibEnclInfoEx_t *enclInfo);

/**
 * @brief        获取encl下所有物理盘槽位列表
 * @param[in]    ctrlId: 控制卡标识符
 * @param[in]    enclId: enclosure标识符
 * @param[in]    pdSlotListIn: 物理盘列表
 * @param[out]   pdSlotListOut: 物理盘列表
 * @return       PS3_ERRNO_SUCCESS: 成功
 * @note         pdSlotListIn->count为encl下物理盘的个数，pdSlotListIn->values为物理盘列表，
 *               当pdSlotListIn指针为NULL或者pdSlotListIn->count为小于0时，获取encl下所有物理盘列表
 *               当pdSlotListIn不为NULL时，将pdSlotListIn指向的数据直接拷贝到pdSlotListOut的地址中
 * @attention    对pdSlotListIn->values进行了限制，范围为0-127，对输出的pdSlotListOut->values进行了排序，由从小到大;
 */
Ps3Errno ps3libPdListGetByEnclId(CtrlId_t ctrlId, EnclId_t enclId, const Ps3LibIdList_t *pdSlotListIn,
        Ps3LibIdList_t *pdSlotListOut);

/**
 * @brief      获取背板信息
 * @param[in]  ctrlId   控制卡标识符
 * @param[in]  idList   背板信息
 * @param[in]  idCount  dg ID的数量
 * @param[out] info     encl全部信息
 * @param[out] errList  查询失败的dg计数
 * @return     PS3_ERRNO_SUCCESS: 成功
 */
Ps3Errno ps3libEnclInfoGetByEnclList(CtrlId_t ctrlId, EnclId_t *idList, U32 idCount, Ps3LibEnclInfoEx_t *info,
        Ps3Errno *errList);
        
#if defined(__cplusplus)
}
#endif

#endif
