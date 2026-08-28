/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_rdebug.h
 * @date    2023.06.19
 * @brief   远程debug
 */

#ifndef __PS3LIB_RDEBUG_H__
#define __PS3LIB_RDEBUG_H__

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief         启动远程Debug功能
 * @param[in]     ctrlId: 控制卡标识符
 * @param[out]    pBuffer: 保存获取到的启动时串口输出的buffer，返回给调用者 
 * @param[in/out] pBufferLen: 输入pBuffer长度（防止获取数据超出buffer长度），返回获取到的串口输出的长度
 * @return        PS3_ERRNO_SUCCESS: 成功
 */  
S32 ps3libRemoteDebugStart(CtrlId_t ctrlId, U8* pBuffer, U16* pBufferLen);

/** 
 * @brief      将用户输入发送给CPLD 
 * @param[in]  ctrlId: 控制卡标识符
 * @param[in]  pBuffer: 用户输入数据的buffer 
 * @param[in]  bufferLen: 用户输入数据的长度 
 * @return     PS3_ERRNO_SUCCESS: 成功
 */
S32 ps3libRemoteDebugSend(CtrlId_t ctrlId, U8* pBuffer, U16 bufferLen);

/** 
 * @brief         从CPLD获取控制卡的串口输出，范围是从上次获取到本次获取间的log 
 * @param[in]     ctrlId: 控制卡标识符
 * @param[out]    pBuffer: 保存获取到的串口输出的buffer，返回给调用者 
 * @param[in/out] pBufferLen: 输入pBuffer长度（防止获取数据超出buffer长度），返回获取到的串口输出的长度
 * @return        PS3_ERRNO_SUCCESS: 成功
 */
S32 ps3libRemoteDebugReceive(CtrlId_t ctrlId, U8* pBuffer, U16* pBufferLen);

/** 
 * @brief      停止远程Debug功能 
 * @param[in]  ctrlId: 控制卡标识符
 * @return     PS3_ERRNO_SUCCESS: 成功
 */
S32 ps3libRemoteDebugStop(CtrlId_t ctrlId);

#if defined(__cplusplus)
}
#endif

#endif
