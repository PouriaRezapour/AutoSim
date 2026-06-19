/**
 * @file    Platform_Types.h
 * @author  Ali Rezapour
 * @date    2025-06-13
 * @version 0.2.4
 * @brief   Platform and compiler dependent type definitions.
 *
 * @details
 * Provides AUTOSAR-compliant integer, float, and boolean type definitions
 * Includes CPU type, bit order, and byte order configuration for a 32-bit
 * little-endian platform.
 *
 * @par Revision History:
 * |---------|------------|------------------|---------------------|
 * | Version | Date       | Author           | Description         |
 * |---------|------------|------------------|---------------------|
 * | 0.2.4   | 2025-06-13 | Ali Rezapour     | Initial version     |
 * |---------|------------|------------------|---------------------|
 */

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/* ─── Version Information ─────────────────────────────────────────────────── */

/* Software version */
#define PLATFORM_TYPES_SW_MAJOR_VERSION  0
#define PLATFORM_TYPES_SW_MINOR_VERSION  2
#define PLATFORM_TYPES_SW_PATCH_VERSION  4

/* ─── Macros & Constants ──────────────────────────────────────────────────── */

/* CPU register type width */
#define CPU_TYPE_8        8
#define CPU_TYPE_16       16
#define CPU_TYPE_32       32
#define CPU_TYPE_64       64

/* Bit order definitions */
#define MSB_FIRST         0                 /* Big endian bit ordering       */
#define LSB_FIRST         1                 /* Little endian bit ordering    */

/* Byte order definitions */
#define HIGH_BYTE_FIRST   0                 /* Big endian byte ordering      */
#define LOW_BYTE_FIRST    1                 /* Little endian byte ordering   */

/* Platform type and endianness configuration */
#define CPU_TYPE            CPU_TYPE_32
#define CPU_BIT_ORDER       LSB_FIRST
#define CPU_BYTE_ORDER      LOW_BYTE_FIRST

/* Interrupt mode definitions */
#define MCAL_ISR_TYPE_TOOLCHAIN  1
#define MCAL_ISR_TYPE_OS         2
#define MCAL_ISR_TYPE_NONE       3

/* ─── Type Definitions ────────────────────────────────────────────────────── */

/* AUTOSAR integer data types */
typedef signed char         sint8;          /*        -128 .. +127           */
typedef unsigned char       uint8;          /*           0 .. 255            */
typedef signed short        sint16;         /*      -32768 .. +32767         */
typedef unsigned short      uint16;         /*           0 .. 65535          */
typedef signed long         sint32;         /* -2147483648 .. +2147483647    */
typedef unsigned long       uint32;         /*           0 .. 4294967295     */
typedef signed long long    sint64;         /* 63 bit + 1 bit sign           */
typedef unsigned long long  uint64;         /* 64 bit                        */

/* Least-width integer types */
typedef unsigned long       uint8_least;    /* At least 8 bit                */
typedef unsigned long       uint16_least;   /* At least 16 bit               */
typedef unsigned long       uint32_least;   /* At least 32 bit               */
typedef signed long         sint8_least;    /* At least 7 bit + 1 bit sign   */
typedef signed long         sint16_least;   /* At least 15 bit + 1 bit sign  */
typedef signed long         sint32_least;   /* At least 31 bit + 1 bit sign  */

/* Floating point types */
typedef float               float32;
typedef double              float64;

/* Boolean type */
typedef _Bool               boolean;

#ifndef true
  #define true   1
#endif
#ifndef TRUE
  #define TRUE   true
#endif

#ifndef false
  #define false  0
#endif
#ifndef FALSE
  #define FALSE  false
#endif

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/* No public functions defined in this header. */

#endif /* PLATFORM_TYPES_H */
