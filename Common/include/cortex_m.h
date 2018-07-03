
#ifndef CORTEX_M_H
#define CORTEX_M_H

#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx.h"


/**
 * \brief 关闭本地CPU中断
 *
 * \attention
 * - 关闭本地CPU中断的时间一定要短
 * - 必须与 am_int_cpu_unlock() 函数搭配使用
 *
 * \return 中断关闭信息
 *
 * \par 示例：
 * \code
 * uint32_t key;
 * 
 * key = am_int_cpu_lock();
 * // do something.
 * am_int_cpu_unlock(key);
 * \endcode
 *
 * \par 嵌套使用示例：
 * \code 
 * uint32_t key0, key1;
 *
 * key0 = am_int_cpu_lock();  // 中断马上关闭
 * key1 = am_int_cpu_lock();  // 中断保持关闭
 * 
 * am_int_cpu_unlock(key1);   // 中断保持关闭
 * am_int_cpu_unlock(key0);   // 中断马上打开
 * \endcode
 */
uint32_t am_int_cpu_lock(void);

/**
 * \brief 打开本地CPU中断
 *
 * \attention
 *  必须与 am_int_cpu_lock() 函数搭配使用
 *
 * \param[in] key : 中断关闭信息
 * \return    无
 */
void am_int_cpu_unlock(uint32_t key);



extern void SystemReset(void);

#endif
