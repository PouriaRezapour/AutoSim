/**
 * @file    Os.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   Operating System abstraction layer implementation.
 *
 * @details
 * Implements the main OS lifecycle functions: RTE scheduler initialisation,
 * conditional creation of all enabled periodic and event-driven RTOS tasks,
 * and final handover to the CMSIS-RTOS kernel. Os_Run() does not return
 * after osKernelStart() is called.
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------|
 * | Version | Date       | Author           | Description                    |
 * |---------|------------|------------------|--------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template      |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | The module now uses the        |
 * |         |            |                  | project's standard types       |
 * |         |            |                  | defined in Std_Types.h.        |
 * |---------|------------|------------------|--------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Os.h"
#include "Os_TasksCfg.h"
#include "Os_CyclicTasks.h"
#include "Rte_Scheduler.h"
#include "Std_Types.h"
#include "cmsis_os.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

void Os_TasksCreate(void)
{
    /* Ethernet task */
    #if (OS_TASK_ETH_ENABLED == 1)
        osThreadDef(Eth, Os_Task_Eth, OS_TASK_ETH_PRIORITY, 0, OS_TASK_ETH_STACK);
        Os_EthThreadHandle = osThreadCreate(osThread(Eth), NULL);
    #endif

    /* Background task */
    #if (OS_TASK_BACKGROUND_ENABLED == 1)
        osThreadDef(Task_Background, Os_Task_Background, OS_TASK_BACKGROUND_PRIORITY, 0, OS_TASK_BACKGROUND_STACK);
        Os_TaskBackgroundThreadHandle = osThreadCreate(osThread(Task_Background), NULL);
    #endif

    /* 1 ms task */
    #if (OS_TASK_1MS_ENABLED == 1)
        osThreadDef(Task_1ms, Os_Task_1ms, OS_TASK_1MS_PRIORITY, 0, OS_TASK_1MS_STACK);
        Os_Task1msThreadHandle = osThreadCreate(osThread(Task_1ms), NULL);
    #endif

    /* 5 ms task */
    #if (OS_TASK_5MS_ENABLED == 1)
        osThreadDef(Task_5ms, Os_Task_5ms, OS_TASK_5MS_PRIORITY, 0, OS_TASK_5MS_STACK);
        Os_Task5msThreadHandle = osThreadCreate(osThread(Task_5ms), NULL);
    #endif

    /* 10 ms task */
    #if (OS_TASK_10MS_ENABLED == 1)
        osThreadDef(Task_10ms, Os_Task_10ms, OS_TASK_10MS_PRIORITY, 0, OS_TASK_10MS_STACK);
        Os_Task10msThreadHandle = osThreadCreate(osThread(Task_10ms), NULL);
    #endif

    /* 20 ms task */
    #if (OS_TASK_20MS_ENABLED == 1)
        osThreadDef(Task_20ms, Os_Task_20ms, OS_TASK_20MS_PRIORITY, 0, OS_TASK_20MS_STACK);
        Os_Task20msThreadHandle = osThreadCreate(osThread(Task_20ms), NULL);
    #endif

    /* 50 ms task */
    #if (OS_TASK_50MS_ENABLED == 1)
        osThreadDef(Task_50ms, Os_Task_50ms, OS_TASK_50MS_PRIORITY, 0, OS_TASK_50MS_STACK);
        Os_Task50msThreadHandle = osThreadCreate(osThread(Task_50ms), NULL);
    #endif

    /* 100 ms task */
    #if (OS_TASK_100MS_ENABLED == 1)
        osThreadDef(Task_100ms, Os_Task_100ms, OS_TASK_100MS_PRIORITY, 0, OS_TASK_100MS_STACK);
        Os_Task100msThreadHandle = osThreadCreate(osThread(Task_100ms), NULL);
    #endif
}

void Os_Run(void)
{
    /* Initialize all RTE schedulers */
    Rte_Scheduler_Init();

    /* Create all enabled application tasks */
    Os_TasksCreate();

    /* Start RTOS scheduler — does not return */
    osKernelStart();
}
