
#ifndef CORTEX_M_H
#define CORTEX_M_H

#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx.h"


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
uint32_t am_int_cpu_lock(void);

/**
 * \brief �򿪱���CPU�ж�
 *
 * \attention
 *  ������ am_int_cpu_lock() ��������ʹ��
 *
 * \param[in] key : �жϹر���Ϣ
 * \return    ��
 */
void am_int_cpu_unlock(uint32_t key);



extern void SystemReset(void);

#endif
