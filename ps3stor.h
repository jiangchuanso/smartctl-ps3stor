/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3stor.h
 * @date    2024.04.25
 * @brief   header files and macros
 */

#ifndef __PS3STOR_H__
#define __PS3STOR_H__

#include "ps3lib_type.h"
#include "ps3lib_ctrl.h" //todo : create ps3stor.h
#include "ps3lib_passthru.h"

#define PS3STOR_ERRNO_SUCCESS           (0)
#define PS3STOR_MIN(x,y)                ((x) < (y) ? (x) : (y))
#define PS3STOR_SCSI_STATUS_UNDERRUN    (140)

//#define PS3STOR_SUPPORT_DEV_SDX   //todo : ps3libSystemPciInfoGet return with hostid 

#define PS3STOR_INVALID_CTRL_ID             PS3LIB_INVALID_CODE_U16
#define PS3STOR_INVALID_ENCL_ID             PS3LIB_INVALID_CODE_U8
#define PS3STOR_INVALID_SLOT_ID             PS3LIB_INVALID_CODE_U16
#define PS3STOR_PD_INTERFACE_TYPE_UNKNOWN   PS3LIB_PD_INTERFACE_TYPE_UNKNOWN
#define PS3STOR_PD_INTERFACE_TYPE_SAS       PS3LIB_PD_INTERFACE_TYPE_SAS
#define PS3STOR_PD_INTERFACE_TYPE_SATA      PS3LIB_PD_INTERFACE_TYPE_SATA
#define PS3STOR_PD_INTERFACE_TYPE_NVME      PS3LIB_PD_INTERFACE_TYPE_NVME
#define PS3STOR_NVME_SUBMISSIONQ_ADMIN      PS3LIB_NVME_SUBMISSIONQ_ADMIN
#define PS3STOR_NVME_SUBMISSIONQ_IO         PS3LIB_NVME_SUBMISSIONQ_IO

typedef CtrlId_t                        ctrl_id_t;
typedef EnclId_t                        encl_id_t;
typedef SlotId_t                        slot_id_t;
typedef Ps3Errno                        ps3stor_errno;
typedef Ps3LibPdBaseInfo_t              ps3stor_pd_baseinfo_t;
typedef Ps3LibSCSIPassthru_t            ps3stor_scsi_passthru_t;
typedef Ps3LibPdDevIdList_s             ps3stor_devid_list_t;
typedef Ps3LibSystemPciInfo_t           ps3stor_system_pcieinfo_t;
typedef Ps3LibCtrlList_t                ps3stor_ctrl_list_t;
typedef Ps3LibPdInterface_e             ps3stor_pd_interface_e;
typedef Ps3LibNVMeEncapsulationReq_t    ps3stor_nvme_encapsution_req_t;
typedef Ps3LibNVMeEncapsulationRsp_t    ps3stor_nvme_encapsution_rsp_t;

/**
 * @brief   nvme 请求 PRP结构
 */
typedef struct ps3stor_prp_entry {
    union {
        struct {
            unsigned pbao_low    :  32;   ///< Page Base Address and Offset - NOTE: Bits 0-1 Reserved
            unsigned pbao_hi     :  32;   ///< Page Base Address and Offset
        } bits;
        uint64_t qword;
    } u;
} ps3stor_prp_entry_t;

/**
 * @brief   nvme cmd ： 64 bytes
 */
typedef struct ps3stor_nvme_cmd {
    unsigned                cdw0;             //03:00
    unsigned                nsid;             //07:04
    uint64_t                reserved_8_15;    //15:08 reserved
    uint64_t                mptr;             //23:16 metadata pointer
    ps3stor_prp_entry_t     prp1;             //31:24 PRP Entry 2
    ps3stor_prp_entry_t     prp2;             //39:32 PRP Entry 1
    unsigned                cdw10;            //43：40
    unsigned                cdw11;            //47：44
    unsigned                cdw12;            //51：48
    unsigned                cdw13;            //55：52
    unsigned                cdw14;            //59：56
    unsigned                cdw15;            //63：60
} ps3stor_nvme_cmd_t;

#endif