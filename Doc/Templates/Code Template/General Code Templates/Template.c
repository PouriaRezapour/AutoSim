/**
 * @file    Template.c
 * @author  Your Name
 * @date    20xx-xx-xx
 * @version x.x.x
 * @brief   Implementation of the template module.
 *
 * @details
 * Detailed description (optional):
 * Describe implementation choices, dependencies, or
 * anything useful for a maintainer reading this file.
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | x.x.x   | 20xx-xx-xx | Your Name  | Initial release                      |
 * |---------|------------|------------|--------------------------------------|
 */

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Template.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── Private Macros ──────────────────────────────────────────────────────── */

#define LOG(fmt, ...)  fprintf(stderr, "[template] " fmt "\n", ##__VA_ARGS__)

/* ─── Private Types ───────────────────────────────────────────────────────── */

typedef struct {
    int  isInitialised;
    /* add internal state here */
} ModuleState_t;

/* ─── Private Variables ───────────────────────────────────────────────────── */

static ModuleState_t s_state = { .isInitialised = 0 };

/* ─── Private Function Prototypes ────────────────────────────────────────── */

static Status_t ValidateData(const MyStruct_t *pData);

/* ─── Public Functions ────────────────────────────────────────────────────── */

Status_t Module_Init(void)
{
    if (s_state.isInitialised) {
        LOG("Already initialised");
        return STATUS_OK;
    }

    /* TODO: perform initialisation */

    s_state.isInitialised = 1;
    LOG("Initialised successfully");
    return STATUS_OK;
}

Status_t Module_Process(MyStruct_t *pData)
{
    if (!s_state.isInitialised) {
        LOG("Module not initialised");
        return STATUS_ERROR;
    }

    if (ValidateData(pData) != STATUS_OK) {
        return STATUS_ERROR;
    }

    /* TODO: implement processing logic */

    return STATUS_OK;
}

void Module_Cleanup(void)
{
    if (!s_state.isInitialised) {
        return;
    }

    /* TODO: release resources */

    s_state.isInitialised = 0;
    LOG("Cleaned up");
}

/* ─── Private Functions ───────────────────────────────────────────────────── */

/**
 * @brief  Validate that the data pointer and its fields are sane.
 */
static Status_t ValidateData(const MyStruct_t *pData)
{
    if (pData == NULL) {
        LOG("Null data pointer");
        return STATUS_ERROR;
    }

    /* TODO: add field-level validation */

    return STATUS_OK;
}
