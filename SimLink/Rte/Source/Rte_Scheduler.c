/**
 * @file    Rte_Scheduler.c
 * @author  Ali Rezapour (Pouria)
 * @date    2025-06-07
 * @version 0.2.3
 * @brief   RTE Scheduler — maps OS tasks to SWC runnables.
 *
 * @details
 * Central dispatch point between the OS cyclic task layer and the SWC
 * runnable functions. Only Rte_*OsIf.h headers are permitted here. Each
 * scheduler function corresponds to one OS task period and calls the
 * runnables assigned to that rate. Empty scheduler slots are intentional
 * placeholders for future SWC additions.
 * The module now uses the project's standard types defined in Std_Types.h.
 *
 * @par Revision History:
 * |---------|------------|------------------|-------------------------------------|
 * | Version | Date       | Author           | Description                         |
 * |---------|------------|------------------|-------------------------------------|
 * | 0.1.0   | 2025-05-04 | A.Rezapour       | Initial release                     |
 * | 0.1.2   | 2025-05-18 | A.Rezapour       | Applied new file template           |
 * | 0.2.0   | 2025-05-23 | A.Rezapour       | Added Can_Test_Tx_10ms to 10ms slot |
 * | 0.2.1   | 2025-05-26 | A.Rezapour       | Moved CAN test to DevTst framework  |
 * | 0.2.3   | 2025-06-07 | A.Rezapour       | Integrate Std_Types.h               |
 * | 0.2.5   | 2025-07-01 | A.Rezapour       | Added VlvSigObs and CmdSigMgr 50ms  |
 * |         |            |                  | runnables                           |
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | New DevTst 100ms runnable which has |
 * |         |            |                  | the DioFlip test                    |
 * |---------|------------|------------------|-------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"
#include "Rte_Scheduler.h"

#include "Rte_Hmi.h"
#include "Rte_CmdSigMgr.h"
#include "Rte_VlvSigObs.h"
#include "Rte_Eth.h"
#include "Rte_DevTst.h"

/* ─── Public Functions ────────────────────────────────────────────────────── */

void Rte_Scheduler_Init(void)
{
	Rte_Hmi_Init();
    Rte_Eth_Init();
    Rte_DevTst_Init();
}

void Rte_Scheduler_1ms(void)
{
}

void Rte_Scheduler_5ms(void)
{
}

void Rte_Scheduler_10ms(void)
{
	Rte_Runnable_Hmi_10ms();
    Rte_Runnable_DevTst_10ms();
}

void Rte_Scheduler_20ms(void)
{
}

void Rte_Scheduler_50ms(void)
{
    Rte_Runnable_VlvSigObs_50ms();
    Rte_Runnable_CmdSigMgr_50ms();

}

void Rte_Scheduler_100ms(void)
{
	Rte_Runnable_DevTst_100ms();
}

void Rte_Scheduler_Background(void)
{
}

void Rte_Scheduler_Eth(void)
{
    Rte_Runnable_Eth();
}
