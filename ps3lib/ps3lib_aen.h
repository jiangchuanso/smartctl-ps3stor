/**
 * Copyright (C), 2023,  .
 *
 * @file    ps3lib_event.h
 * @date    2023.05.22
 * @brief   事件日志
 */

#ifndef __PS3LIB_AEN_H__
#define __PS3LIB_AEN_H__

/**
 * @brief   事件上报回调
 */
typedef int (*pAenCbFunc)(Ps3LibEvtLogEntry_s *pEventDetail);

/**
 * @brief   批量事件上报回调
 */
typedef int (*pAenBatchCbFunc)(Ps3LibEvtLogList_s *pEvtLogList);

/**
 * @brief   事件日志注册信息
 */
typedef struct Ps3LibRegAenInfo {
    CtrlId_t ctrlId;    ///< 控制卡Id
    U8       pad[2];
    U32      level;     ///< 注册级别
    U32      locale;    ///< 注册模式
    U32      seqNum;    ///< 订阅序列号
} Ps3LibRegAenInfo_s;

/**
 * @brief    事件日志注册输入信息
 */
typedef struct Ps3LibRegAenInput {
    U32             count;          ///< 注册数量
    Ps3LibRegAenInfo_s regAenInfo[PS3LIB_MAX_CTRL_COUNT];    ///< 事件日志注册信息
    pAenCbFunc      pFunc;          ///< 事件单独上报回调 有则调用
    pAenBatchCbFunc pFuncBatch;     ///< 事件批量上报回调 有则调用
} Ps3LibRegAenInput_s;

/**
 * @brief    事件日志注册回复信息
 */
typedef struct Ps3LibRegAenOutput {
    U32 regCount;                         ///< 注册数量
    U32 regCtrlId[PS3LIB_MAX_CTRL_COUNT]; ///< 注册的控制卡Id
    U32 uniqueId;                         ///< 本次注册唯一Id
} Ps3LibRegAenOutput_s;

/**
 * @brief       事件日志注册
 * @param[in]   input:   事件日志注册输入信息
 * @param[out]  output:  事件日志注册回复信息
 * @return      PS3_ERRNO_SUCCESS: 成功；其他: 失败
 * @note        
 */
Ps3Errno ps3libRegAen(Ps3LibRegAenInput_s *input, Ps3LibRegAenOutput_s *output);

/**
 * @brief    事件日志取消注册
 * @param[in]   uniqueId:   注册唯一Id标识
 */
Ps3Errno ps3libUnregAen(U32 uniqueId);

#endif
