/**
 * @file    NamingConvention.c
 * @brief   C Naming Convention Reference Guide
 *
 * This file demonstrates the standard naming conventions used across
 * the codebase. Each section contains a comment explaining the rule
 * followed by a concrete example.
 *
 * Convention Summary:
 *  - File Names      : PascalCase with optional [Prefix_]Name[_Suffix]
 *  - Functions       : PascalCase with optional [Prefix_]Name[_Suffix]
 *  - Local Variables : camelCase
 *  - Global Variables: camelCase with g_ prefix
 *  - Static Variables: camelCase with s_ prefix
 *  - Parameters      : camelCase
 *  - Constants       : UPPER_SNAKE_CASE
 *  - Macros          : UPPER_SNAKE_CASE
 *  - Typedefs        : PascalCase with _t suffix
 *  - Pointers        : camelCase with p prefix
 *  - Booleans/Flags  : camelCase with is/has/can/should prefix
 *  - Arrays          : camelCase, plural or with Array/Buffer suffix
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* =========================================================================
 * SECTION 1 — FILE NAMES
 * Rule   : PascalCase, optional prefix and/or suffix separated by underscores.
 * Format : [Prefix_]Name[_Suffix].c / .h
 * Examples (as filenames, not code):
 *   Eth_UdpServer.c
 *   Spi_Tranceive.h
 *   Dio_Status.c
 * ========================================================================= */


/* =========================================================================
 * SECTION 2 — CONSTANTS
 * Rule   : UPPER_SNAKE_CASE — all uppercase letters, words separated by '_'.
 * Use for: #define constants and const global values.
 * ========================================================================= */
#define MAX_BUFFER_SIZE     256U
#define DEFAULT_TIMEOUT     1000U
#define MIN_THRESHOLD       10U


/* =========================================================================
 * SECTION 3 — MACROS (FUNCTION-LIKE)
 * Rule   : UPPER_SNAKE_CASE — same as constants.
 *          Parameters are typically single uppercase letters or short names.
 * ========================================================================= */
#define ENABLE_FEATURE          (1U)
#define CALCULATE_CHECKSUM(x)   ((x) ^ 0xFFU)

/* =========================================================================
 * SECTION 4 — TYPEDEFS (STRUCTS / UNIONS / ENUMS)
 * Rule   : PascalCase with _t suffix.
 * ========================================================================= */

/* Struct typedef example */
typedef struct
{
    uint16_t id;
    uint8_t  rawValue;
    bool     isActive;
} SensorData_t;

/* Another struct typedef example */
typedef struct
{
    uint8_t  address;
    uint16_t baudRate;
    bool     isEnabled;
} ConfigParam_t;

/* Enum typedef example */
typedef enum
{
    SYSTEM_STATE_IDLE    = 0U,
    SYSTEM_STATE_RUNNING = 1U,
    SYSTEM_STATE_ERROR   = 2U
} SystemState_t;


/* =========================================================================
 * SECTION 5 — GLOBAL VARIABLES
 * Rule   : camelCase with g_ prefix.
 *          Visible across multiple translation units (extern linkage).
 * ========================================================================= */
SystemState_t g_systemState    = SYSTEM_STATE_IDLE;
ConfigParam_t g_deviceConfig   = {0U, 9600U, false};
uint32_t      g_errorCounter   = 0U;


/* =========================================================================
 * SECTION 6 — STATIC (MODULE-LEVEL) VARIABLES
 * Rule   : camelCase with s_ prefix.
 *          File-scoped — not accessible outside this translation unit.
 * ========================================================================= */
static uint32_t s_moduleCounter   = 0U;
static bool     s_initializeFlag  = false;


/* =========================================================================
 * SECTION 7 — FUNCTION DECLARATIONS
 * Rule   : PascalCase with optional [Prefix_]Name[_Suffix].
 *          Prefix usually indicates the module (e.g., Com_, Spi_, Dio_).
 * ========================================================================= */
void     Com_SendMessage(uint8_t* pData, uint16_t configValue);
uint8_t  Spi_Read_SensorData(uint8_t inputData);
void     Dio_WriteChannel(uint8_t channelId, bool isHigh);


/* =========================================================================
 * SECTION 8 — FUNCTION DEFINITIONS
 *
 * Inside functions, the following local naming rules apply:
 *
 *  Local Variables : camelCase
 *  Parameters      : camelCase
 *  Pointers        : camelCase with p prefix
 *  Booleans/Flags  : camelCase with is / has / can / should prefix
 *  Arrays          : camelCase, plural or with Array / Buffer suffix
 * ========================================================================= */

/**
 * @brief  Reads sensor data over SPI and returns a processed value.
 *
 * @param  inputData  Raw byte to transmit on the SPI bus. (camelCase parameter)
 * @return Processed sensor byte.
 */
uint8_t Spi_Read_SensorData(uint8_t inputData)
{
    /* --- Local Variables (camelCase) --- */
    uint8_t  sensorValue  = 0U;
    uint16_t bufferIndex  = 0U;

    /* --- Boolean / Flag Variables (camelCase + is/has/can/should prefix) --- */
    bool isInitialized = false;
    bool hasError      = false;
    bool canTransmit   = true;
    bool shouldReset   = false;

    /* --- Arrays (camelCase, plural or Array/Buffer suffix) --- */
    uint8_t  sensorValues[MAX_BUFFER_SIZE];
    uint8_t  configArray[8U];
    uint8_t  dataBuffer[MAX_BUFFER_SIZE];

    /* --- Pointers (camelCase with p prefix) --- */
    uint8_t*      pData       = dataBuffer;
    SensorData_t* pSensorInfo = NULL;
    void*         pBuffer     = NULL;

    /* Suppress unused-variable warnings in this demo context */
    (void)bufferIndex;
    (void)hasError;
    (void)canTransmit;
    (void)shouldReset;
    (void)configArray;
    (void)sensorValues;
    (void)pSensorInfo;
    (void)pBuffer;

    /* Example logic using the conventions above */
    isInitialized = s_initializeFlag;

    if (isInitialized && (inputData > (uint8_t)MIN_THRESHOLD))
    {
        sensorValue = CALCULATE_CHECKSUM(inputData);
        *pData      = sensorValue;
        s_moduleCounter++;
    }

    return sensorValue;
}

/**
 * @brief  Sends a message over the COM layer.
 *
 * @param  pData        Pointer to the data buffer to transmit.  (p prefix)
 * @param  configValue  Configuration word for the transmission. (camelCase)
 */
void Com_SendMessage(uint8_t* pData, uint16_t configValue)
{
    bool isReadyToSend = (pData != NULL) && (configValue != 0U);

    if (isReadyToSend)
    {
        g_errorCounter = 0U;
        /* Transmission logic would go here */
    }
    else
    {
        g_errorCounter++;
        g_systemState = SYSTEM_STATE_ERROR;
    }
}

/**
 * @brief  Writes a logic level to a DIO channel.
 *
 * @param  channelId  Target channel identifier. (camelCase)
 * @param  isHigh     true = logic HIGH, false = logic LOW. (is prefix)
 */
void Dio_WriteChannel(uint8_t channelId, bool isHigh)
{
    uint8_t outputValue = isHigh ? 1U : 0U;

    /* Write outputValue to channelId register (hardware abstracted) */
    (void)channelId;
    (void)outputValue;
}
