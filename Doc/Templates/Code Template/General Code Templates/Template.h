/**
 * @file    Template.h
 * @author  Your Name
 * @date    20xx-xx-xx
 * @version x.x.x
 * @brief   Brief description of the module.
 *
 * @details
 * Detailed description (optional):
 * Explain the purpose of this header, what it exposes,
 * and any important usage notes.
 *
 * @par Revision History:
 * |---------|------------|------------|--------------------------------------|
 * | Version | Date       | Author     | Description                          |
 * |---------|------------|------------|--------------------------------------|
 * | x.x.x   | 20xx-xx-xx | Your Name  | Initial release                      |
 * |---------|------------|------------|--------------------------------------|
 */

#ifndef TEMPLATE_H
#define TEMPLATE_H

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include <stdint.h>
#include <stddef.h>

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

#define MODULE_VERSION  "1.0.0"
#define BUFFER_SIZE     256

/* ─── Type Definitions ────────────────────────────────────────────────────── */

typedef enum {
    STATUS_OK    = 0,
    STATUS_ERROR = -1
} Status_t;

typedef struct {
    uint32_t id;
    char     name[BUFFER_SIZE];
    Status_t status;
} MyStruct_t;

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Initialize the module.
 * @return STATUS_OK on success, STATUS_ERROR on failure.
 */
Status_t Module_Init(void);

/**
 * @brief  Process the given data structure.
 * @param  pData  Pointer to a MyStruct_t instance.
 * @return STATUS_OK on success, STATUS_ERROR on failure.
 */
Status_t Module_Process(MyStruct_t *pData);

/**
 * @brief  Clean up and release resources.
 */
void Module_Cleanup(void);

#endif /* TEMPLATE_H */
