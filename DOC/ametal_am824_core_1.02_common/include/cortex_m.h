
#ifndef CORTEX_M_H
#define CORTEX_M_H

#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx.h"

typedef int cortex_int_state_t;

__attribute__((always_inline))
static cortex_int_state_t cortex_int_get_and_disable(void)
{
    cortex_int_state_t state;
    state = __disable_irq();
    return state;
}

__attribute__((always_inline))
static void cortex_int_restore(cortex_int_state_t state)
{
    if (!state) {
        __enable_irq();
    }
}

__attribute__((always_inline))
static bool cortex_in_isr(void)
{
    return (__get_xPSR() & 0x1F) != 0;
}

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
__attribute__((always_inline))
static cortex_int_state_t am_int_cpu_lock(void)
{
    cortex_int_state_t state;
    state = __disable_irq();
    return state;
}
/**
 * \brief 打开本地CPU中断
 *
 * \attention
 *  必须与 am_int_cpu_lock() 函数搭配使用
 *
 * \param[in] key : 中断关闭信息
 * \return    无
 */
__attribute__((always_inline))
static void am_int_cpu_unlock(cortex_int_state_t state)
{
    if (!state) {
        __enable_irq();
    }
}
/**
 * \brief 使能中断
 * \param[in] inum  : 中断号
 * \retval    AM_OK : 操作成功
 */
int am_int_enable(int inum);

/**
 * \brief 禁能中断
 * \param[in] inum  : 中断号
 * \retval    AM_OK : 操作成功
 */
int am_int_disable(int inum);



extern void SystemReset(void);

#endif
