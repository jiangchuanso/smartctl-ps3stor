/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_type.h
 * @date    2023.04.25
 * @brief   基本数据类型定义
 */
#ifndef __PS3LIB_TYPE_H__
#define __PS3LIB_TYPE_H__

#if defined(__cplusplus)
extern "C" {
#endif

typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef unsigned long long  U64;

typedef char                S8;
typedef short               S16;
typedef int                 S32;
typedef long long           S64;

typedef U32 CtrlId_t;
typedef S32 Ps3Errno;
typedef U8 EnclId_t;
typedef U16 VdId_t;
typedef U16 PdId_t;
typedef U16 SlotId_t;
typedef U16 DgId_t;
typedef U8 PhyId_t;
typedef U8 LaneId_t;
typedef U32 ExpId_t;
typedef U8  FuncId_t;

#define PS3LIB_INVALID_CODE_U8     0XFF
#define PS3LIB_INVALID_CODE_U16    0XFFFF
#define PS3LIB_INVALID_CODE_U32    0XFFFFFFFF
#define PS3LIB_INVALID_CODE_U64    0XFFFFFFFFFFFFFFFF
#define PS3LIB_INVALID_CODE_S8     0X7F
#define PS3LIB_INVALID_CODE_S16    0X7FFF
#define PS3LIB_INVALID_CODE_S32    0X7FFFFFFF
#define PS3LIB_INVALID_CODE_S64    0X7FFFFFFFFFFFFFFF
#define PS3LIB_INVALID_CODE_PTR    (NULL)

#if defined(__cplusplus)
}
#endif

#endif
