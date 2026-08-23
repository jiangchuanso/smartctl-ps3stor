/**
 * Copyright (C), 2020,., Ltd.
 *
 * @file        ps3_ses.h
 * @date        2021.01.10
 * @brief       ses解析
 */

#ifndef __PS3_SES_H__
#define __PS3_SES_H__

#include "ps3_types.h"

#ifdef __cplusplus
        extern "C"{
#endif

///< diagnostic page code
enum {
    SES_SUPPORTED_DPC           = 0x0,
    SES_CONFIGURATION_DPC       = 0x1,
    SES_ENC_CONTROL_DPC         = 0x2,
    SES_ENC_STATUS_DPC          = 0x2,
    SES_HELP_TEXT_DPC           = 0x3,
    SES_STRING_DPC              = 0x4,
    SES_THRESHOLD_DPC           = 0x5,
    SES_ARRAY_CONTROL_DPC       = 0x6,   ///< obsolete, last seen ses-r08b.pdf
    SES_ARRAY_STATUS_DPC        = 0x6,   ///< obsolete
    SES_ELEM_DESC_DPC           = 0x7,
    SES_SHORT_ENC_STATUS_DPC    = 0x8,
    SES_ENC_BUSY_DPC            = 0x09,
    SES_ADD_ELEM_STATUS_DPC     = 0xa,   ///< Additional Element Status dpage code
    SES_SUBENC_HELP_TEXT_DPC    = 0xb,
    SES_SUBENC_STRING_DPC       = 0xc,
    SES_SUPPORTED_SES_DPC       = 0xd,   ///< should be 0x1 <= dpc <= 0x2f
    SES_DOWNLOAD_MICROCODE_DPC  = 0xe,
    SES_SUBENC_NICKNAME_DPC     = 0xf,
    SES_PROTOCOL_SPEC_DPC       = 0x3f,
    SES_TRANS_ADDR_DPC          = 0x40,
    SES_DEV_STATUS_DPC          = 0x41,
    SES_REBUILD_ASSIST_DPC      = 0x42,
    SES_ALL_DPC                 = 0xff,
};

///< Element Type codes
enum {
    SES_UNSPECIFIED_ETC     = 0x0,
    SES_DEVICE_ETC          = 0x1,
    SES_POWER_SUPPLY_ETC    = 0x2,
    SES_COOLING_ETC         = 0x3,
    SES_TEMPERATURE_ETC     = 0x4,
    SES_DOOR_ETC            = 0x5,    ///< prior to ses3r05 was DOOR_LOCK_ETC
    SES_AUD_ALARM_ETC       = 0x6,
    SES_ENC_SCELECTR_ETC    = 0x7,    ///< Enclosure services controller electronics
    SES_SCC_CELECTR_ETC     = 0x8,    ///<  SCC: SCSI Controller Commands (e.g. RAID
                                  ///< controller). SCC Controller Elecronics
    SES_NV_CACHE_ETC        = 0x9,
    SES_INV_OP_REASON_ETC   = 0xa,
    SES_UI_POWER_SUPPLY_ETC = 0xb,
    SES_DISPLAY_ETC         = 0xc,
    SES_KEY_PAD_ETC         = 0xd,
    SES_ENCLOSURE_ETC       = 0xe,
    SES_SCSI_PORT_TRAN_ETC  = 0xf,
    SES_LANGUAGE_ETC        = 0x10,
    SES_COMM_PORT_ETC       = 0x11,
    SES_VOLT_SENSOR_ETC     = 0x12,
    SES_CURR_SENSOR_ETC     = 0x13,
    SES_SCSI_TPORT_ETC      = 0x14,
    SES_SCSI_IPORT_ETC      = 0x15,
    SES_SIMPLE_SUBENC_ETC   = 0x16,
    SES_ARRAY_DEV_ETC       = 0x17,
    SES_SAS_EXPANDER_ETC    = 0x18,
    SES_SAS_CONNECTOR_ETC   = 0x19,
    SES_LAST_ETC            = SES_SAS_CONNECTOR_ETC,      ///<  adjust as necessary
    SES_NUM_ETC             = (SES_LAST_ETC + 1),
};

///< Transport protocol identifiers or just Protocol identifiers
enum {
    SES_TPROTO_FCP    = 0,
    SES_TPROTO_SPI    = 1,
    SES_TPROTO_SSA    = 2,
    SES_TPROTO_1394   = 3,
    SES_TPROTO_SRP    = 4,
    SES_TPROTO_ISCSI  = 5,
    SES_TPROTO_SAS    = 6,
    SES_TPROTO_ADT    = 7,
    SES_TPROTO_ATA    = 8,
    SES_TPROTO_UAS    = 9,   ///< USB attached SCSI
    SES_TPROTO_SOP    = 0xa, ///< SCSI over PCIe
    SES_TPROTO_PCIE   = 0xb, ///< includes NVMe
    SES_TPROTO_NONE   = 0xf,
};

///< Supported Diagnostic Page[0x00] format/ Supported SES Diagnostic Pages Diagnostic Page[0x0d]
typedef struct SesSuppDP{
    U8 pageCode;
    U8 rsvd;
    U8 pageLen[2];      ///< 大端字节序，使用时需转为U16
    U8 suppList[0];    ///< support page list
} SesSuppDP_s;

///< Configuration Diagnostic Page[0x01] 的 enclosure descriptor format
typedef struct SesEnclDesc{
    U8      enclSerProcNum      : 3;        ///< num of enclosure services processes
    U8      rsvd1               : 1;
    U8      relEnclSerProcID    : 3;        ///< relative enclosure services process id
    U8      rsvd2               : 1;

    U8      subEnclID;
    U8      tdhNum;                         ///< number of type descriptor headers
    U8      enclDescLen;
    U8      logicalID[8];
    U8      vendorID[8];
    U8      productID[16];
    U8      revisionLevel[4];
    U8      vendorSpecInfo[0];              ///< vendor specfic enclosure information
} SesEnclDesc_s;

///< Configuration Diagnostic Page[0x01] 的 type descriptor header format
typedef struct SesTypeDescHdr{
    U8      elmtType;                       ///< element类型
    U8      elmtNum;                        ///< 该类型的element的个数
    U8      subEnclID;                      ///< 次enclosure ID
    U8      typeDescTextLen;                ///< 类型描述文本长度
} SesTypeDescHdr_s;

///< Configuration Diagnostic Page[0x01] format
typedef struct SesConfigDP{
    U8 pageCode;
    U8 secSubenclNum;
    U8 pageLen[2];      ///< 大端字节序，使用时需转为U16
    U8 genCode[4];
    U8 descList[0]; ///< enclosure descriptor list + type descriptor header list + type descriptor list
} SesConfigDP_s;

///< Unspecified element type specific status information format
typedef struct SesElmtUnspecStat {
    U8 rsvd[3];
} SesElmtUnspecStat_s;

///< device element type specific status information format
typedef struct SesElmtDevStat {
    U8 slotAddr;

    U8 report           : 1;
    U8 ident            : 1;
    U8 rmv              : 1;
    U8 readyToInsert    : 1;
    U8 enclsrBypB       : 1;
    U8 enclsrBypA       : 1;
    U8 notRmv           : 1;
    U8 appClientBypA    : 1;

    U8 devBypB          : 1;
    U8 devBypA          : 1;
    U8 bypB             : 1;
    U8 bypA             : 1;
    U8 devOff           : 1;
    U8 faultRqst        : 1;
    U8 faultSensed      : 1;
    U8 appClientBypB    : 1;
} SesElmtDevStat_s;

///< ArrayDevice element type specific status information format
typedef struct SesElmtArrayDevStat {
    U8 rrAbort          : 1;
    U8 rebuildRemap     : 1;
    U8 inFailedArray    : 1;
    U8 inCritArray      : 1;
    U8 consCheck        : 1;
    U8 hotSpare         : 1;
    U8 rsvdDev          : 1;
    U8 ok               : 1;

    U8 report           : 1;
    U8 ident            : 1;
    U8 rmv              : 1;
    U8 readyToInsert    : 1;
    U8 enclsrBypB       : 1;
    U8 enclsrBypA       : 1;
    U8 doNotRmv         : 1;
    U8 clientBypA       : 1;

    U8 devBypB          : 1;
    U8 devBypA          : 1;
    U8 bypB             : 1;
    U8 bypA             : 1;
    U8 devOff           : 1;
    U8 faultRqstd       : 1;
    U8 faultSensed      : 1;
    U8 clientBypB       : 1;
}  SesElmtArrayDevStat_s;

///< power supply element type specific status information format
typedef struct SesElmtPowerSupplyStat {
    U8 rsvd1       : 6;
    U8 doNotRmv    : 1;
    U8 ident       : 1;

    U8 rsvd2       : 1;
    U8 dcOverCurr  : 1;
    U8 dcUnderVolt : 1;
    U8 dcOverVolt  : 1;
    U8 rsvd3       : 4;

    U8 dcFail      : 1;
    U8 acFail      : 1;
    U8 tempWarn    : 1;
    U8 overtmpFail : 1;
    U8 off         : 1;
    U8 rqstedOn    : 1;
    U8 fail        : 1;
    U8 hotSwap     : 1;
} SesElmtPowerSupplyStat_s;

///< cooling element type specific status information format
typedef struct SesElmtCoolingStat {
    U8 actualFanSpeedHigh    : 3;
    U8 fanSpeedFactor        : 3; ///< SES3 此处为保留字段
    U8 doNotRmv              : 1;
    U8 ident                 : 1;

    U8 actualFanSpeedLow;

    U8 actualSpeedCode       : 3;
    U8 rsvd2                 : 1;
    U8 off                   : 1;
    U8 rqstedOn              : 1;
    U8 fail                  : 1;
    U8 hotSwap               : 1;
} SesElmtCoolingStat_s;

///< Temperature element type specific status information format
typedef struct SesElmtTempSensorStat {
    U8 rsvd1                : 6;
    U8 fail                 : 1;
    U8 ident                : 1;

    U8 temperature;

    U8 utWarning            : 1;
    U8 utFailure            : 1;
    U8 otWarning            : 1;
    U8 otFailure            : 1;
    U8 rsvd2                : 4;
} SesElmtTempSensorStat_s;

///< door element type specific status information format
typedef struct SesElmtDoorStat {
    U8 rsvd1  : 6;
    U8 fail   : 1;
    U8 ident  : 1;

    U8 rsvd2;

    U8 unLock : 1;
    U8 open   : 1;
    U8 rsvd3  : 6;
} SesElmtDoorStat_s;

///< audible alarm element type specific status information format
typedef struct SesElmtAudiAlarmStat {
    U8 rsvd1    : 6;
    U8 fail     : 1;
    U8 ident    : 1;

    U8 rsvd2;

    U8 unRecov  : 1;
    U8 crit     : 1;
    U8 nonCrit  : 1;
    U8 info     : 1;
    U8 remind   : 1;
    U8 rsvd3    : 1;
    U8 muted    : 1;
    U8 rqstMute : 1;
} SesElmtAudiAlarmStat_s;

///< enclousure services controller electronics element type specific status information format
typedef struct SesElmtEnclServCtrlElectStat {
    U8 rsvd1      : 4;
    U8 rmv        : 1;
    U8 doNotRmv   : 1;
    U8 fail       : 1;
    U8 ident      : 1;

    U8 report     : 1;
    U8 rsvd2      : 7;

    U8 rsvd3      : 7;
    U8 hotSwap    : 1;
} SesElmtEnclServCtrlElectStat_s;

///< scc controller electronics element type specific status information format
typedef struct SesElmtSccCtrlElectStat {
    U8 rsvd1      : 6;
    U8 fail       : 1;
    U8 ident      : 1;

    U8 report     : 1;
    U8 rsvd2      : 7;

    U8 rsvd3;
} SesElmtSccCtrlElectStat_s;

///< non volatile chache element type specific status information format
typedef struct SesElmtNvCacheStat {
    U8 sizeMulti      : 2;
    U8 rsvd1          : 4;
    U8 fail           : 1;
    U8 ident          : 1;

    U8 nvCacheSize[2];      ///< 大端字节序，使用时需转为U16
} SesElmtNvCacheStat_s;

///< 通用 Invalid operation reason element type specific status information format
typedef struct SesInvopStat {
    U8 rsvd0       : 1;
    U8 rsvd1       : 5;
    U8 invopType   : 2;

    U8 rsvd2;
    U8 rsvd3;
} SesInvopStat_s;

///< invop 为 00 时, Invalid operation reason element type specific status information format
typedef struct SesInvop00Stat {
    U8 pageNotSupp : 1;
    U8 rsvd1       : 5;
    U8 invopType   : 2;

    U8 rsvd2;
    U8 rsvd3;
} SesInvop00Stat_s;

///< invop 为 01 时, Invalid operation reason element type specific status information format
typedef struct SesInvop01Stat {
    U8 bitNum    : 3;
    U8 rsvd      : 3;
    U8 invopType : 2;

    U8 byteOffset[2];       ///< 大端字节序，使用时需转为U16
} SesInvop01Stat_s;

///< invop 为 10 时, Invalid operation reason element type specific status information format
typedef struct SesInvop10Stat {
    U8 rsvd1       : 6;
    U8 invopType   : 2;

    U8 rsvd2;
    U8 rsvd3;
} SesInvop10Stat_s;

///< invop 为 11 时, Invalid operation reason element type specific status information format
typedef struct SesInvop11Stat {
    U8 vendorSpecByte1  : 6;
    U8 invopType        : 2;

    U8 vendorSpecByte2;
    U8 vendorSpecByte3;
} SesInvop11Stat_s;

///< Invalid operation reason element type specific status information format
typedef union SesElmtInvOpReasonStat {
    SesInvopStat_s   invopstat;
    SesInvop00Stat_s invop00stat;
    SesInvop01Stat_s invop01stat;
    SesInvop10Stat_s invop10stat;
    SesInvop11Stat_s invop11stat;
} SesElmtInvOpReasonStat_u;

///< uninterruptible power supply element type specific status information format
typedef struct SesElmtUnintrPowerSuppStat {
    U8 batteryStatus;

    U8 intfFail    : 1;
    U8 warn        : 1;
    U8 upsFail     : 1;
    U8 dcFail      : 1;
    U8 acFail      : 1;
    U8 acQual      : 1;
    U8 acHi        : 1;
    U8 acLo        : 1;

    U8 bpf         : 1;
    U8 battFail    : 1;
    U8 rsvd        : 3;
    U8 doNotRmv    : 1;
    U8 fail        : 1;
    U8 ident       : 1;
} SesElmtUnintrPowerSuppStat_s;

///< display element type specific status information format
typedef struct SesElmtDisplayStat {
    U8 dispModeStat  : 2;
    U8 rsvd          : 4;
    U8 fail          : 1;
    U8 ident         : 1;

    U8 dispCharStat[2];        ///< 大端字节序，使用时需转为U16
} SesElmtDisplayStat_s;

///< key pad entry element type specific status information format
typedef struct SesElmtKeyPadStat {
    U8 rsvd1          : 6;
    U8 fail           : 1;
    U8 ident          : 1;

    U8 rsvd2;
    U8 rsvd3;
} SesElmtKeyPadStat_s;

///< enclosure element type specific status information format
typedef struct SesElmtEnclStat {
    U8 logFull             : 1;     ///< 日志分区是否已满
    U8 rsvd                : 6;
    U8 ident               : 1;

    U8 warnIndi            : 1;
    U8 failIndi            : 1;
    U8 timeUnitPowCycle    : 6;

    U8 warnRqst            : 1;
    U8 failRqst            : 1;
    U8 rqstPowOffDur       : 6;
} SesElmtEnclStat_s;

///< scsi port/transceiver element type specific status information format
typedef struct SesElmtScsiPortTranStat {
    U8 rsvd1               : 6;
    U8 fail                : 1;
    U8 ident               : 1;

    U8 report              : 1;
    U8 rsvd2               : 7;

    U8 xmitFail            : 1;
    U8 lol                 : 1;
    U8 rsvd3               : 2;
    U8 disable             : 1;
    U8 rsvd4               : 3;
} SesElmtScsiPortTranStat_s;

///< language element type specific status information format
typedef struct SesElmtLangStat {
    U8 rsvd1               : 7;
    U8 ident               : 1;

    U8 langCode[2];         ///< 大端字节序，使用时需转为U16
} SesElmtLangStat_s;

///< communication port element type specific status information format
typedef struct SesElmtCommPortStat {
    U8 rsvd1               : 6;
    U8 fail                : 1;
    U8 ident               : 1;

    U8 rsvd2;

    U8 disabled            : 1;
    U8 rsvd3               : 7;
}  SesElmtCommPortStat_s;

///< voltage sensor element type specific status information format
typedef struct SesElmtVoltSensorStat {
    U8 critUnder     : 1;
    U8 critOver      : 1;
    U8 warnUnder     : 1;
    U8 warnOver      : 1;
    U8 rsvd          : 2;
    U8 fail          : 1;
    U8 ident         : 1;

    U8 voltage[2];          ///< 大端字节序，使用时需转为U16
}  SesElmtVoltSensorStat_s;

///< current sensor element type specific status information format
typedef struct SesElmtCurrSensorStat {
    U8 rsvd1         : 1;
    U8 critOver      : 1;
    U8 rsvd2          : 1;
    U8 warnOver      : 1;
    U8 rsvd3         : 2;
    U8 fail          : 1;
    U8 ident         : 1;

    U8 current[2];          ///< 大端字节序，使用时需转为U16
}  SesElmtCurrSensorStat_s;

///< scsi target port element type specific status information format
typedef struct SesElmtScsiTargetPortStat {
    U8 rsvd1         : 6;
    U8 fail          : 1;
    U8 ident         : 1;

    U8 report        : 1;
    U8 rsvd2         : 7;

    U8 enabled       : 1;
    U8 rsvd3         : 7;
}  SesElmtScsiTargetPortStat_s;

///< scsi initiator port element type specific status information format
typedef struct SesElmtScsiInitPortStat {
    U8 rsvd1         : 6;
    U8 fail          : 1;
    U8 ident         : 1;

    U8 report        : 1;
    U8 rsvd2         : 7;

    U8 enabled       : 1;
    U8 rsvd3         : 7;
}  SesElmtScsiInitPortStat_s;

///< simple subenclosure element type specific status information format
typedef struct SesElmtSimSubenclStat {
    U8 rsvd1         : 6;
    U8 fail          : 1;
    U8 ident         : 1;

    U8 rsvd2;
    U8 shortEnclStat;
} SesElmtSimSubenclStat_s;

///< sas expander element type specific status information format
typedef struct SesElmtSasExpanderStat {
    U8 rsvd1         : 6;
    U8 fail          : 1;
    U8 ident         : 1;

    U8 rsvd2;
    U8 rsvd3;
} SesElmtSasExpanderStat_s;

///< sas connector element type specific status information format
typedef struct SesElmtSasConnStat {
    U8 connType      : 7;
    U8 ident         : 1;

    U8 connPhyLink;

    U8 rsvd1         : 5;
    U8 oc            : 1;
    U8 fail          : 1;
    U8 mated         : 1;
}  SesElmtSasConnStat_s;

///< Enclosure Status Diagnostic Page[0x02] 的 Element type specific Status information format
typedef union SesElmtTypeStatInfo {
    SesElmtUnspecStat_s            unspecStat;
    SesElmtDevStat_s               devStat;
    SesElmtArrayDevStat_s          arrDevStat;
    SesElmtPowerSupplyStat_s       powerSupplyStat;
    SesElmtCoolingStat_s           coolingStat;
    SesElmtTempSensorStat_s        tempSensorStat;
    SesElmtDoorStat_s              doorStat;
    SesElmtAudiAlarmStat_s         audiAlarmStat;
    SesElmtEnclServCtrlElectStat_s enclServCtrlElectStat;
    SesElmtSccCtrlElectStat_s      sccCtrlElectStat;
    SesElmtNvCacheStat_s           nvCacheStat;
    SesElmtInvOpReasonStat_u       invOpReasonStat;
    SesElmtUnintrPowerSuppStat_s   unintrPowerSuppStat;
    SesElmtDisplayStat_s           displayStat;
    SesElmtKeyPadStat_s            keyPadStat;
    SesElmtEnclStat_s              enclStat;
    SesElmtScsiPortTranStat_s      scsiPortTranStat;
    SesElmtLangStat_s              langStat;
    SesElmtCommPortStat_s          commPortStat;
    SesElmtVoltSensorStat_s        voltSensorStat;
    SesElmtCurrSensorStat_s        currSensorStat;
    SesElmtScsiTargetPortStat_s    scsiTPortStat;
    SesElmtScsiInitPortStat_s      scsiIPortStat;
    SesElmtSimSubenclStat_s        simSubenclStat;
    SesElmtSasExpanderStat_s       sasExpStat;
    SesElmtSasConnStat_s           sasConnStat;
}SesElmtTypeStatInfo_u;

///< Enclosure Status Diagnostic Page[0x02] 对于每一个 element 的 common status
typedef struct SesElmtCmmnStat {
    U8 statCode     : 4;
    U8 swap         : 1;
    U8 disabled     : 1;
    U8 prdfail      : 1;
    U8 rsvd         : 1;
} SesElmtCmmnStat_s;

///< Enclosure Status Diagnostic Page[0x02] 的 status element format
typedef struct SesStatElmt {
    SesElmtCmmnStat_s stat;         ///< 通用 status data;
    SesElmtTypeStatInfo_u statInfo; ///< 每一个 element 类型所对应的 status format
} SesStatElmt_s;

///< Enclosure Status Diagnostic Page[0x02] format
typedef struct SesEnclStatDP{
    U8 pageCode;

    U8 unRecov  : 1;
    U8 crit     : 1;
    U8 nonCrit  : 1;
    U8 info     : 1;
    U8 invop    : 1;
    U8 rsvd     : 3;

    U8 pageLen[2];               ///< 大端字节序，使用时需转为U16
    U8 genCode[4];
    SesStatElmt_s statusList[0]; ///< status desccriptor list
} SesEnclStatDP_s;

///< Help text diagnostic page[0x03] format
typedef struct SesHelpTextDP{
    U8 pageCode;
    U8 obsolete;
    U8 pageLen[2];      ///< 大端字节序，使用时需转为U16
    U8 helpText[0];     ///< primary subenclosure help text
} SesHelpTextDP_s;

///< string in/out diagnostic page[0x04] format
typedef struct SesStringDP {
    U8 pageCode;
    U8 obsolete;
    U8 pageLen[2];       ///< 大端字节序，使用时需转为U16
    U8 strData[0];    ///< primary subenclosure string in/out data
} SesStringDP_s;

///< threshold in diagnostic page[0x05] 的 threshold status element format
typedef struct SesThrStatElmt {
    U8 highCritThr;     ///< high critical threshold
    U8 highWarnThr;     ///< high warning  threshold
    U8 lowWarnThr;      ///< low  warning  threshold
    U8 lowCritThr;      ///< low  critical threshold
} SesThrStatElmt_s;

///< threshold in diagnostic page[0x05] format
typedef struct SesThrInDP {
    U8 pageCode;

    U8 rsvd1   : 4;
    U8 invop   : 1;
    U8 rsvd2   : 3;

    U8 pageLen[2];            ///< 大端字节序，使用时需转为U16
    U8 genCode[4];
    SesThrStatElmt_s thrStatDescList[0];    ///< threshold status descriptor list
} SesThrInDP_s;

///< element descriptor diagnostic page[0x07] 的 overall descriptor format and element descriptor format
typedef struct SesElmtDesc {
    U8 rsvd[2];
    U8 descLen[2];  ///< 大端字节序，使用时需转为U16
    U8 desc[0];     ///< descriptor
} SesElmtDesc_s;

///< element descriptor diagnostic page[0x07] format
typedef struct SesElmtDescDP {
    U8            pageCode;
    U8            rsvd;
    U8            pageLen[2];         ///< 大端字节序，使用时需转为U16
    U8            genCode[4];
    SesElmtDesc_s elmtDescList[0];    ///< element descriptor by type list
} SesElmtDescDP_s;

///< sShort enclosure status diagnostic page[0x08] format
typedef struct SesShortEnclStatDP {
    U8 pageCode;
    U8 status;       ///< short enclosure status
    U8 pageLen[2];   ///< 大端字节序，使用时需转为U16
} SesShortEnclStatDP_s;

///< Enclosure Busy diagnostic page[0x09] format
typedef struct SesEnclBusyDP {
    U8 pageCode;

    U8 busy     : 1;
    U8 venSpec  : 7; ///< vendor specific

    U8 pageLen[2];   ///< 大端字节序，使用时需转为U16
} SesEnclBusyDP_s;

///< additional elment status diagnostic page[0x0a] 的 FCP 协议对应的 port descriptor format
typedef struct SesFcPortDesc { ///< 16 bytes
    U8 portLoopPos;       ///< port loop position
    U8 bypassReason;
    U8 rsvd[2];
    U8 portRqstHardAddr;  ///< port requested hard address
    U8 nPortId[3];        ///< n_port identifier
    U8 nPortName[8];      ///< n_port name
} SesFcPortDesc_s;

///< additional elment status diagnostic page[0x0a] 的 eip 位为 0 时 的 FCP 协议
///< 对应的 descriptor protocol specific information
typedef struct SesFcSpecInfoEip0 {
    U8 portNum;
    U8 rsvd1;
    U8 nodeName[8];
    SesFcPortDesc_s portDescList[0]; ///< port descriptor
} SesFcSpecInfoEip0_s;

///< additional elment status diagnostic page[0x0a] 的 eip 位为 1 时 的 FCP 协议
///< 对应的 descriptor protocol specific information
typedef struct SesFcSpecInfoEip1 {
    U8 portNum;
    U8 rsvd1[2];
    U8 devSlotNum;
    U8 nodeName[8];
    SesFcPortDesc_s portDescList[0]; ///< port descriptor
} SesFcSpecInfoEip1_s;

///< additional elment status diagnostic page[0x0a] 的 SAS 协议 中 device slot element
///< 和 array device slot elment(已弃用) 类型 对应的 port descriptor format
typedef struct SesSasDevElmtPhyDesc {
    U8 rsvd1       : 4;
    U8 devType     : 3;  ///< device type
    U8 rsvd2       : 1;

    U8 rsvd3;

    U8 rsvd4       : 1;
    U8 smpIPort    : 1;  ///< smp initiator port
    U8 stpIPort    : 1;  ///< stp initiator port
    U8 sspIPort    : 1;  ///< ssp initiator port
    U8 rsvd5       : 4;

    U8 sataDev     : 1;
    U8 smpTPort    : 1;
    U8 stpTPort    : 1;
    U8 sspTPort    : 1;
    U8 rsvd6       : 3;
    U8 sataPortSle : 1; ///< stat port selector

    U8 attchSasAddr[8]; ///< attached sas address
    U8 sasAddr[8];      ///< sas address
    U8 phyID;           ///< phy identifier
    U8 rsvd7[6];
} SesSasDevElmtPhyDesc_s;

///< additional elment status diagnostic page[0x0a] 的 SAS 协议 的
///< descriptor protocol specific information 的通用format
typedef struct SesSasSpecInfoComm {
    U8       descTypeSpec1;

    U8       descTypeSpec2 : 6;
    U8       descType      : 2;

    U8       descTypeSpec3[0];
} SesSasSpecInfoComm_s;

///< additional elment status diagnostic page[0x0a] 的
///< additonal element status descriptor protocol specific information for device slot
///< element and array device slot elements for sas with the EIP bit set to zero
typedef struct SesSasDevElmtSpecInfoEip0 {
    U8                     phyDescNum;

    U8                     notAllPhys : 1;
    U8                     rsvd       : 5;
    U8                     descType   : 2;

    SesSasDevElmtPhyDesc_s phyDescList[0]; ///< phy descriptor list for device slot element
                                           ///< and array device slot elements
} SesSasDevElmtSpecInfoEip0_s;

///< additional elment status diagnostic page[0x0a] 的
///< additonal element status descriptor protocol specific information for device slot
///< element and array device slot elements for sas with the EIP bit set to one
typedef struct SesSasDevElmtSpecInfoEip1 {
    U8                     phyDescNum;

    U8                     notAllPhys : 1;
    U8                     rsvd1      : 5;
    U8                     descType   : 2;

    U8                     rsvd2;
    U8                     devSlotNum;
    SesSasDevElmtPhyDesc_s phyDescList[0]; ///< phy descriptor list for device slot element
                                           ///< and array device slot elements
} SesSasDevElmtSpecInfoEip1_s;

///< additional elment status diagnostic page[0x0a] 的 SAS 协议 中
///< sas expander element 对应的 port descriptor format
typedef struct SesExpPhyDesc {
    U8 connElmtIdx;    ///< connector element index
    U8 otherElmtIdx;   ///< other element index
} SesExpPhyDesc_s;

///< additional elment status diagnostic page[0x0a] 的 additonal element status
///< descriptor protocol specific information for sas expander elements
typedef struct SesSasExpElmtSpecInfo {
    U8 expPhyDescNum;

    U8 rsvd1    : 6;
    U8 descType : 2;

    U8 rsvd2[2];
    U8 sasAddr[8];

    SesExpPhyDesc_s expPhyDescList[0];
} SesSasExpElmtSpecInfo_s;

///< additional elment status diagnostic page[0x0a] 的 SAS 协议中 scsi initiator port
///< scsi target port, and enclosure services controller electronics element 对应的 port descriptor format
typedef struct SesSseElmtPhyDesc {
    U8 phyID;        ///< phy identifier
    U8 rsvd;
    U8 connElmtIdx;   ///< connector element index
    U8 otherElmtIdx;  ///< other element index
    U8 sasAddr[8];   ///< sas address
} SesSseElmtPhyDesc_s;

///< additional elment status diagnostic page[0x0a] 的 additonal element status descriptor protocol
///< specific information for scsi initiator port scsi target port, and enclosure services controller
///< electronics elements for sas
typedef struct SesSasSseElmtSpecInfo {
    U8                  phyDescNum;

    U8                  rsvd1    : 6;
    U8                  descType : 2;

    U8                  rsvd2[2];
    SesSseElmtPhyDesc_s ssePhyDescList[0];
} SesSasSseElmtSpecInfo_s;

///< additional elment status diagnostic page[0x0a] 的 PCIE 协议中 nvme 对应的 port descriptor format
typedef struct SesNvmePortDesc_s {
    U8 cidValid : 1;
    U8 bdfValid : 1;
    U8 psnValid : 1;
    U8 rsvd1    : 5;

    U8 ctrlID[2];     ///< controller id 注意此字段为小端字节序
    U8 rsvd2;
    U8 busNum;        ///< bus number

    U8 funcNum : 3;   ///< function number
    U8 devNum  : 5;   ///< device number

    U8 phySlotNumLow; ///< 注意 physical slot number字段为小端字节序

    U8 phySlotNumhigh : 5;
    U8 rsvd3          : 3;
} SesNvmePortDesc_s;

///< additional elment status diagnostic page[0x0a] 的 PCIE 协议中 nvme 对应的
///< additonal element status descriptor protocol specific information
typedef struct SesPcieNvmeSpecInfo {
    U8 portNum;                            ///< number of ports

    U8 notAllPort     : 1;                 ///< not all ports
    U8 rsvd1          : 4;
    U8 pcieProtoType  : 3;                 ///< pcie protocol type

    U8 rsvd2;
    U8 devSlotNum;                         ///< device slot number
    U8 rsvd3[2];
    U8 pcieVenID[2];                       ///< pcie vendor id 注意：此字段为小端字节序
    U8 serialNum[20];                      ///< serial number 大端字节序
    U8 modelNum[40];                       ///< model number 大端字节序
    SesNvmePortDesc_s nvmePortDescList[0]; ///< nvme port descriptor list
} SesPcieNvmeSpecInfo_s;

///< additional elment status diagnostic page[0x0a] 的 protocol specific info for Fibre Channnel、SAS 、PCI
typedef union SesProtoSpecInfo {
    SesFcSpecInfoEip0_s           fcSpecInfoE0;
    SesFcSpecInfoEip1_s           fcSpecInfoE1;

    SesSasSpecInfoComm_s          sasSpecInfoComm;
    ///< specific info for device slot and array device slot elements for sas
    SesSasDevElmtSpecInfoEip0_s   sasDeSpecInfoE0;
    SesSasDevElmtSpecInfoEip1_s   sasDeSpecInfoE1;
    SesSasExpElmtSpecInfo_s       sasExpElmtSpecInfo; ///< specific info for expander elements for sas
    ///< scsi initiator port, scsi target port, and enclosure services controller electronics elements for sas
    SesSasSseElmtSpecInfo_s       sasSseElmtSpecInfo;

    SesPcieNvmeSpecInfo_s         pcieNvmeSpecInfo;
} SesProtoSpecInfo_u;

///< additional elment status diagnostic page[0x0a] 的 additional elment status descriptor 的公共头
typedef struct SesAesDescComm{
    U8 protoId  : 4;
    U8 eip      : 1;
    U8 rsvd1    : 2;
    U8 invalid  : 1;
    U8 aesDescLen;  ///< additional element status descriptor length
} SesAesDescComm_s;

///< additional elment status diagnostic page[0x0a] 的 eip 为 1 时 的 additional elment status descriptor format
typedef struct SesAesDescEip1{
    SesAesDescComm_s   aesDesc;

    U8                 eiioe    : 2;
    U8                 rsvd2    : 6;

    U8                 elmtIdx;          ///< element index
    SesProtoSpecInfo_u protoSpecInfo[0]; ///< protocol specific information
} SesAesDescEip1_s;

///< additional elment status diagnostic page[0x0a] 的 eip 为 0 时 的 additional elment status descriptor format
typedef struct SesAesDescEip0{
    SesAesDescComm_s   aesDesc;
    SesProtoSpecInfo_u protoSpecInfo[0]; ///< protocol specific information
} SesAesDescEip0_s;

///< additional elment status diagnostic page[0x0a] 的 additional elment status descriptor format
typedef union SesAesDesc {
    SesAesDescComm_s aesDesc;
    SesAesDescEip0_s aesDescEip0;
    SesAesDescEip1_s aesDescEip1;
} SesAesDesc_u;

///< additional elment status diagnostic page[0x0a] format
typedef struct SesAddElmtStatDP{
    U8                pageCode;
    U8                rsvd;
    U8                pageLen[2];     ///< 大端字节序，使用时需转为U16
    U8                genCode[4];
    SesAesDesc_u      aesDescList[0]; ///< additional element status descriptor list
} SesAddElmtStatDP_s;

///< Subenclosure help text diagnostic page[0x0b] 的 subenclosure help text entry format
typedef struct SesSubHelpEntry{
    U8  rsvd;
    U8  subenclID;
    U8  helpTextLen[2]; ///< 大端字节序，使用时需转为U16
    U8  helpText[0];
} SesSubHelpEntry_s;

///< Subenclosure help text diagnostic page[0x0b] format
typedef struct SesSubenclHelpDP{
    U8                pageCode;
    U8                secSubenclNum;
    U8                pageLen[2];  ///< 大端字节序，使用时需转为U16
    U8                genCode[4];
    SesSubHelpEntry_s helpList[0]; ///< subenclosure help text list
} SesSubenclHelpDP_s;

///< subenclosure string in diagnostic page[0x0c] 的 subenclosure string in format
typedef struct SesSubStrInData {
    U8  rsvd;
    U8  subenclID;
    U8  subStrInLen[2]; ///< 大端字节序，使用时需转为U16
    U8  subStrInData[0];
} SesSubStrInData_s;

///< subenclosure string in diagnostic page[0x0c] format
typedef struct SesSubenclStrInDP {
    U8                 pageCode;
    U8                 secSubenclNum;
    U8                 pageLen[2];  ///< 大端字节序，使用时需转为U16
    U8                 genCode[4];
    SesSubStrInData_s  subStrInDataList[0]; ///< subenclosure string in data list
} SesSubenclStrInDP_s;

///< download microcode status diagnostic page[0x0e] 的 download microcode status descriptor format
typedef struct SesDownMicroStatDesc {
    U8  rsvd1;
    U8  subID;                    ///< subenclosure indentifier
    U8  subDownMicroStat;         ///< subenclosure download microcode status
    U8  subDownMicroAddStat;      ///< subenclosure download microcode addtional status
    U8  subDownMicroMaxSize[4];   ///< 大端字节序，使用时需转为U32
    U8  rsvd2[3];
    U8  subDownMicroExpBufID;     ///< subenclosure download microcode expected buffer id
    U8  subDownMicroExpBufOff[4]; ///< subenclosure download microcode expected buffer offset
} SesDownMicroStatDesc_s;

///< download microcode status diagnostic page[0x0e] format
typedef struct SesDownMicroStatDP {
    U8                      pageCode;
    U8                      secSubenclNum;
    U8                      pageLen[2];     ///< 大端字节序，使用时需转为U16
    U8                      genCode[4];
    SesDownMicroStatDesc_s  dmsDescList[0]; ///< download microcode status descriptor list
} SesDownMicroStatDP_s;

///< subenclosure nickname status diagnostic page[0x0f] 的 subenclosure nickname status descriptor format
typedef struct SesSubNickStatDesc {
    U8                      rsvd1;
    U8                      subId;             ///< subenclosure identifier
    U8                      subNickStat;       ///< subenclosure enclosure nickname status
    U8                      subNickAddStat;    ///< subenclosure enclosure nickname additional status
    U8                      rsvd2[2];
    U8                      subNickLanCode[2]; ///< subenclosure nickname laguage code, two ascii characters
    U8                      subNick[32];       ///< subenclosure nickname
} SesSubNickStatDesc_s;

///< subenclosure nickname status diagnostic page[0x0f] format
typedef struct SesSubNickStatDP {
    U8                      pageCode;
    U8                      secSubenclNum;
    U8                      pageLen[2];     ///< 大端字节序，使用时需转为U16
    U8                      genCode[4];
    SesSubNickStatDesc_s    snsDescList[0]; ///< subenclosure nickname status descriptor list
} SesSubNickStatDP_s;

#ifdef __cplusplus
}
#endif

#endif
