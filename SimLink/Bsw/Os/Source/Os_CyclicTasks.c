/**
 * @file    Os_CyclicTasks.c
 * @author  A.Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Cyclic RTOS task implementations.
 *
 * @details
 * Contains the implementation of all periodic RTOS tasks (1 ms, 5 ms, 10 ms,
 * 20 ms, 50 ms, 100 ms, background, and Ethernet). Each task uses
 * osDelayUntil for drift-free periodic execution and delegates to the
 * corresponding RTE scheduler function. Tasks are conditionally compiled
 * based on OS_TASK_xxx_ENABLED flags defined in Os_TasksConfig.h.
 *
 * @par Revision History:
 * |---------|------------|------------------|------------------------------------------|
 * | Version | Date       | Author           | Description                              |
 * |---------|------------|------------------|------------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                          |
 * | 0.1.1   | 2025-05-17 | A.Rezapour       | Replace osDelay with osDelayUntil for    |
 * |         |            |                  | drift-free periodic execution            |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template                |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the project's        |
 * |         |            |                  | standard types defined in Std_Types.h.   |
 * |---------|------------|------------------|------------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Os_CyclicTasks.h"
#include "Os_TasksConfig.h"
#include "Std_Types.h"
#include "cmsis_os.h"
#include "Rte_Scheduler.h"

/* ─── Thread Handle Definitions ──────────────────────────────────────────── */

#if (OS_TASK_ETH_ENABLED == 1)
osThreadId Os_EthThreadHandle = NULL;
#endif

#if (OS_TASK_1MS_ENABLED == 1)
osThreadId Os_Task1msThreadHandle = NULL;
#endif

#if (OS_TASK_5MS_ENABLED == 1)
osThreadId Os_Task5msThreadHandle = NULL;
#endif

#if (OS_TASK_10MS_ENABLED == 1)
osThreadId Os_Task10msThreadHandle = NULL;
#endif

#if (OS_TASK_20MS_ENABLED == 1)
osThreadId Os_Task20msThreadHandle = NULL;
#endif

#if (OS_TASK_50MS_ENABLED == 1)
osThreadId Os_Task50msThreadHandle = NULL;
#endif

#if (OS_TASK_100MS_ENABLED == 1)
osThreadId Os_Task100msThreadHandle = NULL;
#endif

#if (OS_TASK_BACKGROUND_ENABLED == 1)
osThreadId Os_TaskBackgroundThreadHandle = NULL;
#endif

/* ─── Public Functions ────────────────────────────────────────────────────── */

#if (OS_TASK_ETH_ENABLED == 1)
void Os_Task_Eth(void const *argument)
{
    (void)argument;
    Rte_Scheduler_Eth();
}
#endif

#if (OS_TASK_BACKGROUND_ENABLED == 1)
void Os_Task_Background(void const *argument)
{
    (void)argument;
    uint32 previousWakeTime = osKernelSysTick();

    for (;;)
    {
        Rte_Scheduler_Background();
        osDelayUntil(&previousWakeTime, OS_TASK_BACKGROUND_DELAY);
    }
}
#endif

#if (OS_TASK_1MS_ENABLED == 1)
void Os_Task_1ms(void const *argument)
{
    (void)argument;
    uint32 previousWakeTime = osKernelSysTick();

    for (;;)
    {
        Rte_Scheduler_1ms();
        osDelayUntil(&previousWakeTime, OS_TASK_1MS_DELAY);
    }
}
#endif

#if (OS_TASK_5MS_ENABLED == 1)
void Os_Task_5ms(void const *argument)
{
    (void)argument;
    uint32 previousWakeTime = osKernelSysTick();

    for (;;)
    {
        Rte_Scheduler_5ms();
        osDelayUntil(&previousWakeTime, OS_TASK_5MS_DELAY);
    }
}
#endif

#if (OS_TASK_10MS_ENABLED == 1)
void Os_Task_10ms(void const *argument)
{
    (void)argument;
    uint32 previousWakeTime = osKernelSysTick();

    for (;;)
    {
        Rte_Scheduler_10ms();
        osDelayUntil(&previousWakeTime, OS_TASK_10MS_DELAY);
    }
}
#endif

#if (OS_TASK_20MS_ENABLED == 1)
void Os_Task_20ms(void const *argument)
{
    (void)argument;
    uint32 previousWakeTime = osKernelSysTick();

    for (;;)
    {
        Rte_Scheduler_20ms();
        osDelayUntil(&previousWakeTime, OS_TASK_20MS_DELAY);
    }
}
#endif

#if (OS_TASK_50MS_ENABLED == 1)
void Os_Task_50ms(void const *argument)
{
    (void)argument;
    uint32 previousWakeTime = osKernelSysTick();

    for (;;)
    {
        Rte_Scheduler_50ms();
        osDelayUntil(&previousWakeTime, OS_TASK_50MS_DELAY);
    }
}
#endif

#if (OS_TASK_100MS_ENABLED == 1)
void Os_Task_100ms(void const *argument)
{
    (void)argument;
    uint32 previousWakeTime = osKernelSysTick();

    for (;;)
    {
        Rte_Scheduler_100ms();
        osDelayUntil(&previousWakeTime, OS_TASK_100MS_DELAY);
    }
}
#endif
