/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_smp.h
 * @date    2023.06.25
 * @brief   smp协议相关数据结构 参考SAS Protocol Layer 1. SMP functions
 */

#ifndef __PS3LIB_SMP_H__
#define __PS3LIB_SMP_H__

#pragma pack(1)

#define PS3LIB_SMP_MAX_RESPONSE_BUFFER_LEN (1032)
///< Length of Response buffer (1024 bytes of data + 4 bytes header + 4 bytes CRC)
#define PS3LIB_SMP_MAX_RESPONSE_BUFFER_LEN_OOB (76)

#define PS3LIB_SMP_REQUEST_FRAME_TYPE                     0x40  ///< request frame type
#define PS3LIB_SMP_RESPONSE_FRAME_TYPE                    0x41  ///< response frame type
#define PS3LIB_SMP_FUNCTION_CODE_REPORT_GENERAL           0x00  ///< report general function
#define PS3LIB_SMP_FUNCTION_CODE_DISCOVER                 0x10  ///< discover function
#define PS3LIB_SMP_FUNCTION_CODE_REPORT_ROUTE_INFO        0x13  ///< report route info function
#define PS3LIB_SMP_FUNCTION_CODE_CONFIGURE_ROUTE_INFO     0X90  ///< configure route info function
#define PS3LIB_SMP_FUNCTION_CODE_REPORT_MANUFACTURER_INFO 0x01  ///< report manufacturer function
#define PS3LIB_SMP_FUNCTION_CODE_REPORT_PHY_ERROR_LOG     0x11  ///< report phy error log function
#define PS3LIB_SMP_FUNCTION_CODE_REPORT_PHY_SATA          0x12  ///< report phy sata function
#define PS3LIB_SMP_FUNCTION_CODE_REQUEST_PHY_CONTROL      0x91  ///< phy control function
#define PS3LIB_SMP_FUNCTION_CODE_READ_GPIO_REGISTER       0x02  ///< read gpio register function
#define PS3LIB_SMP_FUNCTION_CODE_WRITE_GPIO_REGISTER      0x82  ///< write gpio register function

#define PS3LIB_SMP_FN_REPORT_GENERAL_RESP_LEN       76      ///< report general rsp len (header + data + crc)
#define PS3LIB_SMP_FN_REPORT_MANUFACTURER_RESP_LEN  64      ///< report manufacturer rsp len (header + data + crc)
#define PS3LIB_SMP_FN_REPORT_PHY_ERR_LOG_RESP_LEN   32      ///< report phy err log rsp len (header + data + crc)
#define PS3LIB_SMP_FN_REPORT_PHY_EVENT_RESP_LEN     (1020 + 4 + 4)  ///< report phy evt rsp len (header + data + crc)
#define PS3LIB_SMP_FN_REPORT_PHY_SATA_RESP_LEN      72      ///< report phy sata rsp len (header + data + crc)
#define PS3LIB_REP_ROUTE_INFO_RESP_LEN              44      ///< report route info rsp len (header + data + crc)
#define PS3LIB_SMP_FN_DISCOVER_RESP_LEN             124     ///< discover rsp len (header + data + crc)
#define PS3LIB_SMP_REP_COMMON_LEN                   8       ///< header + crc

#define PS3LIB_SMP_REQ_LEN_DISCOVER                 2       ///< discover request length (dwords)
#define PS3LIB_SMP_REQ_LEN_PHY_ERR_LOG              2       ///< report phy err log request length (dwords)
#define PS3LIB_SMP_REQ_LEN_PHY_SATA                 2       ///< report phy sata request length (dwords)
#define PS3LIB_SMP_REQ_LEN_REP_ROUTE_INFO           2       ///< report route info request length (dwords)
#define PS3LIB_SMP_REQ_LEN_CONF_ROUTE_INFO          9       ///< configure route info request length (dwords)
#define PS3LIB_SMP_REQ_LEN_PHY_CTRL                 9       ///< phy control request length (dwords)

/**
 * @brief   SMP Report General Request Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPRequestReportGeneral {
    U8 smpFrameType;            ///< smp frame type
    U8 function;                ///< function type
    U8 allocatedResponseLength; ///< 为response申请的内存 单位DWORD
    U8 requestLength;           ///< 为request申请的内存 单位DWORD
} Ps3LibSMPRequestReportGeneral_t;

/**
 * @brief   SMP Report General Response Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPResponseReportGeneral {
    U8  smpFrameType;
    U8  function;
    U8  functionResult;
    U8  responseLength;
    U16 expChangeCount;
    U16 expRouteIndex;
    U8  reserved1;
    U8  numPhy;
    struct {
        U8 configurableRouteTable : 1;
        U8 configTag : 1;
        U8 reserved : 6;
    } configBitMap;
    U8 reserved2[17];
} Ps3LibSMPResponseReportGeneral_t;

/**
 * @brief   SMP Report Manufacturer Request Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPRequestReportManufacturerInfo {
    U8 smpFrameType;
    U8 function;
    U8 allocatedResponseLength; ///< 为response申请的内存 单位DWORD
    U8 requestLength;           ///< 为request申请的内存 单位DWORD
} Ps3LibSMPRequestReportManufacturerInfo_t;

/**
 * @brief   SMP Report Manufacturer Response Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPResponseReportManufacturerInfo {
    U8  smpFrameType;
    U8  function;
    U8  functionResult;
    U8  responseLength;
    U8  reserved[8];
    U8  vendorID[8];
    U8  productID[16];
    U32 productRevision;
    U8  vendorSpecificInfo[20];
} Ps3LibSMPResponseReportManufacturerInfo_t;

/**
 * @brief   SMP Discover Request Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPRequestDiscover {
    U8 smpFrameType;
    U8 function;
    U8 allocatedResponseLength; ///< 为response申请的内存 单位DWORD
    U8 requestLength;           ///< 为request申请的内存 单位DWORD
    U8 ignored[5];
    U8 phyID;
    U8 ignored1;
    U8 reserved;
} Ps3LibSMPRequestDiscover_t;

/**
 * @brief   SMP Discover Response Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPResponseDiscover {
    U8 smpFrameType;
    U8 function;
    U8 functionResult;
    U8 responseLength;
    U8 ignored[5];
    U8 phyID;
    U8 reserved[2];
    struct {
        U8 ignored1 : 4;
        U8 attachedDeviceType : 4;
    } attachedDevType;
    struct {
        U8 linkRate : 4;
        U8 reserved : 4;
    } physicalLinkRate;
    struct {
        U8 SATAHost : 1;
        U8 SMPInitiator : 1;
        U8 STPInitiator : 1;
        U8 SSPInitiator : 1;
        U8 reserved : 4;
    } attachedInitiatorBitMap;
    struct {
        U8 SATADevice : 1;
        U8 SMPTarget : 1;
        U8 STPTarget : 1;
        U8 SSPTarget : 1;
        U8 attachedDeviceType : 3;
        U8 SATAPortSelector : 1;
    } attachedTargetBitMap;
    U64 SASAddress;
    U64 attachedSASAddress;
    U8  attachedPhyId;
    U8  reserved2[7];
    struct {
        U8 minHardwarePhysicalLinkRate : 4;
        U8 minProgrammedPhysicalLinkRate : 4;
    } minimumPhysicalLinkRate;
    struct {
        U8 maxHardwarePhysicalLinkRate : 4;
        U8 maxProgrammedPhysicalLinkRate : 4;
    } maximumPhysicalLinkRate;
    U8 phyChangeCount;
    struct {
        U8 partialPathwayTimeoutValue : 4;
        U8 reserved : 3;
        U8 virtualPhy : 1;
    } timeoutBitMap;
    struct {
        U8 routingAttribute : 4;
        U8 reserved : 4;
    } routingAttribute;
    U8 reserved4[5];
    U8 vendorSpecificInfo[2];
} Ps3LibSMPResponseDiscover_t;

/**
 * @brief   SMP Report Phy Error Log Request Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPRequestReportPhyErrLog {
    U8 smpFrameType;
    U8 function;
    U8 allocatedResponseLength; ///< 为response申请的内存 单位DWORD
    U8 requestLength;           ///< 为request申请的内存 单位DWORD
    U8 reserved[5];
    U8 phyID;
    U8 reserved2[2];
} Ps3LibSMPRequestReportPhyErrLog_t;

/**
 * @brief   SMP Report Phy Error Log Response Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPResponseReportPhyErrLog {
    U8  smpFrameType;
    U8  function;
    U8  functionResult;
    U8  responseLength; ///< 回复的数据长度 单位DWORD
    U8  ignored[5];
    U8  phyID;
    U8  reserved[2];
    U32 invalidDwordCount;
    U32 runningDisparityErrorCount;
    U32 lossDwordSyncCount;
    U32 phyResetProblemCount;
} Ps3LibSMPResponseReportPhyErrLog_t;

/**
 * @brief   SMP Report Phy SATA Request Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPRequestReportPhySATA {
    U8 smpFrameType;
    U8 function;
    U8 allocatedResponseLength; ///< 为response申请的内存 单位DWORD
    U8 requestLength;           ///< 为request申请的内存 单位DWORD
    U8 ignored[5];
    U8 phyID;
    U8 reserved[2];
} Ps3LibSMPRequestReportPhySATA_t;

/**
 * @brief   SMP Report Phy SATA Response Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPResponseReportPhySATA {
    U8 smpFrameType;
    U8 function;
    U8 functionResult;
    U8 responseLength; ///< 回复的数据长度 单位DWORD
    U8 ignored[5];
    U8 phyID;
    U8 reserved;
    struct {
        U8 affiliationValid : 1;
        U8 affiliationSupported : 1;
        U8 reserved : 6;
    } affiliationBitMap;
    U8  reserved1[4];
    U64 stpSASAddr;
    U8  regDeviceHostFIS[20];
    U8  reserved2[4];
    U64 affiliatedSTPinitiatorSASAddr;
} Ps3LibSMPResponseReportPhySATA_t;

/**
 * @brief   SMP Report Route Info Request Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPRequestReportRouteInfo {
    U8  smpFrameType;
    U8  function;
    U8  allocatedResponseLength; ///< 为response申请的内存 单位DWORD
    U8  requestLength;           ///< 为request申请的内存 单位DWORD
    U8  reserved[2];
    U16 expanderRouteIndex;
    U8  reserved1;
    U8  phyID;
    U8  ignored;
    U8  reserved2;
} Ps3LibSMPRequestReportRouteInfo_t;

/**
 * @brief   SMP Report Route Info Response Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPResponseReportRouteInfo {
    U8  smpFrameType;
    U8  function;
    U8  functionResult;
    U8  responseLength; ///< 回复的数据长度 单位DWORD
    U8  reserved[2];
    U16 expanderRouteIndex;
    U8  reserved1;
    U8  phyID;
    U8  ignored[2];
    struct {
        U8 reserved : 7;
        U8 isDisabled : 1;
    } expanderRouteEntry;
    U8  ignored1[3];
    U64 routedSASAddr;
    U8  ignored2[16];
} Ps3LibSMPResponseReportRouteInfo_t;

/**
 * @brief   SMP Configure Route Info Request Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPRequestConfigureRouteInfo {
    U8  smpFrameType;
    U8  function;
    U8  allocatedResponseLength; ///< 为response申请的内存 单位DWORD
    U8  requestLength;           ///< 为request申请的内存 单位DWORD
    U8  reserved[2];
    U16 expanderRouteIndex;
    U8  reserved1;
    U8  phyID;
    U8  reserved2[2];
    struct {
        U8 reserved : 7;
        U8 isDisabled : 1;
    } expanderRouteEntry;
    U8  ignored[3];
    U64 routedSASAddr;
    U8  ignored2[16];
} Ps3LibSMPRequestConfigureRouteInfo_t;

/**
 * @brief   SMP Configure Route Info Response Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPResponseConfigureRouteInfo {
    U8 smpFrameType;
    U8 function;
    U8 functionResult;
    U8 responseLength; ///< 回复的数据长度 单位DWORD
} Ps3LibSMPResponseConfigureRouteInfo_t;

/**
 * @brief   SMP Phy Control Request Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPRequestPhyControl {
    U8  smpFrameType;
    U8  function;
    U8  allocatedResponseLength; ///< 为response申请的内存 单位DWORD
    U8  requestLength;           ///< 为request申请的内存 单位DWORD
    U8  reserved[2];
    U16 expanderRouteIndex;
    U8  reserved1;
    U8  phyID;
    U8  phyOperation;
    struct {
        U8 updateTimeoutValue : 1;
        U8 reserved : 7;
    } updateTimeoutValueBitMap;
    U8 ignored1[20];
    struct {
        U8 ignored : 4;
        U8 progMinPhysicalLinkRate : 4;
    } progMinPhysicalLinkRate;
    struct {
        U8 ignored : 4;
        U8 progMaxPhysicalLinkRate : 4;
    } progMaxPhysicalLinkRate;
    U8 ignored2[2];
    struct {
        U8 partialPathwayTimeoutValue : 4;
        U8 reserved : 4;
    } partialPathwayTimeout;
    U8 reserved2[3];
} Ps3LibSMPRequestPhyControl_t;

/**
 * @brief   SMP Phy Control Response Structures (字段含义参考SAS Protocol Layer 1. SMP functions)
 */
typedef struct Ps3LibSMPResponsePhyControl {
    U8 smpFrameType;
    U8 function;
    U8 functionResult;
    U8 responseLength; ///< 回复的数据长度 单位DWORD
} Ps3LibSMPResponsePhyControl_t;

#pragma pack()
#endif