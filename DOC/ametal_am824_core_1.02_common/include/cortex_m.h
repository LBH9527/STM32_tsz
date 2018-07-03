
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
 * \brief �رձ���CPU�ж�
 *
 * \attention
 * - �رձ���CPU�жϵ�ʱ��һ��Ҫ��
 * - ������ am_int_cpu_unlock() ��������ʹ��
 *
 * \return �жϹر���Ϣ
 *
 * \par ʾ����
 * \code
 * uint32_t key;
 * 
 * key = am_int_cpu_lock();
 * // do something.
 * am_int_cpu_unlock(key);
 * \endcode
 *
 * \par Ƕ��ʹ��ʾ����
 * \code 
 * uint32_t key0, key1;
 *
 * key0 = am_int_cpu_lock();  // �ж����Ϲر�
 * key1 = am_int_cpu_lock();  // �жϱ��ֹر�
 * 
 * am_int_cpu_unlock(key1);   // �жϱ��ֹر�
 * am_int_cpu_unlock(key0);   // �ж����ϴ�
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
 * \brief �򿪱���CPU�ж�
 *
 * \attention
 *  ������ am_int_cpu_lock() ��������ʹ��
 *
 * \param[in] key : �жϹر���Ϣ
 * \return    ��
 */
__attribute__((always_inline))
static void am_int_cpu_unlock(cortex_int_state_t state)
{
    if (!state) {
        __enable_irq();
    }
}
/**
 * \brief ʹ���ж�
 * \param[in] inum  : �жϺ�
 * \retval    AM_OK : �����ɹ�
 */
int am_int_enable(int inum);

/**
 * \brief �����ж�
 * \param[in] inum  : �жϺ�
 * \retval    AM_OK : �����ɹ�
 */
int am_int_disable(int inum);



extern void SystemReset(void);

#endif
