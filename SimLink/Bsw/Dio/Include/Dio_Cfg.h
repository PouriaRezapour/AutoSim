/**
 * @file    Dio_Cfg.h
 * @author  Ali Rezapour (Pouria)
 * @date    2025-07-08
 * @version 0.2.6
 * @brief   AUTOSAR DIO Driver – pre-compile configuration.
 *
 * @details
 * Generated configuration header for the STM32H753 target.
 * Contains symbolic names for channels, ports, and channel groups
 * as required by [SRS_Dio_12355] / [SWS_Dio_00026].
 *
 * Configuration variant: VARIANT-PRE-COMPILE
 * AUTOSAR CP R24-11
 *
 * @note  Adapt the symbolic names and IDs to match your board schematic.
 *        The channel ID encoding used here is:
 *            ChannelId = (PortIndex * 16U) + PinNumber
 *        where PortIndex: A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8,
 *                         J=9, K=10
 *        Port A  – channels  0 .. 15
 *        Port B  – channels 16 .. 31
 *        Port C  – channels 32 .. 47
 *        Port D  – channels 48 .. 63
 *        Port E  – channels 64 .. 79
 *        Port F  – channels 80 .. 95
 *        Port G  – channels 96 .. 111
 *        Port H  – channels 112 .. 127
 *        Port I  – channels 128 .. 143
 *        Port J  – channels 144 .. 159
 *        Port K  – channels 160 .. 175
 *
 * @par Revision History:
 * |---------|------------|------------------|--------------------------------------|
 * | Version | Date       | Author           | Description                          |
 * |---------|------------|------------------|--------------------------------------|
 * | 0.2.6   | 2025-07-08 | A.Rezapour       | Initial Release                      |
 * |---------|------------|------------------|--------------------------------------|
 */

#ifndef DIO_CFG_H
#define DIO_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Module Version ──────────────────────────────────────────────────────── */

/* Must match Dio.h */
#define DIO_CFG_AR_MAJOR_VERSION    (4U)
#define DIO_CFG_AR_MINOR_VERSION    (7U)
#define DIO_CFG_AR_PATCH_VERSION    (0U)
#define DIO_CFG_SW_MAJOR_VERSION    (1U)
#define DIO_CFG_SW_MINOR_VERSION    (0U)
#define DIO_CFG_SW_PATCH_VERSION    (0U)

/* ─── General Switches ────────────────────────────────────────────────────── */

/* [ECUC_Dio_00142 / ECUC_Dio_00153 / ECUC_Dio_00155 / ECUC_Dio_00143] */

/** Development error detection ON/OFF [SWS_Dio_00074] */
#define DIO_DEV_ERROR_DETECT        (STD_ON)

/** Include Dio_FlipChannel() [SWS_Dio_00191] */
#define DIO_FLIP_CHANNEL_API        (STD_ON)

/** Include Dio_MaskedWritePort() [SWS_Dio_00200] */
#define DIO_MASKED_WRITE_PORT_API   (STD_ON)

/** Include Dio_GetVersionInfo() [SWS_Dio_00139] */
#define DIO_VERSION_INFO_API        (STD_ON)

/* ─── Port Symbolic Names ─────────────────────────────────────────────────── */

/* [SWS_Dio_00020] DioPortId values mapped to the STM32H753 GPIO peripheral index. */
#define DioConf_DioPort_PORTA   ((Dio_PortType)0U)   /**< GPIOA */
#define DioConf_DioPort_PORTB   ((Dio_PortType)1U)   /**< GPIOB */
#define DioConf_DioPort_PORTC   ((Dio_PortType)2U)   /**< GPIOC */
#define DioConf_DioPort_PORTD   ((Dio_PortType)3U)   /**< GPIOD */
#define DioConf_DioPort_PORTE   ((Dio_PortType)4U)   /**< GPIOE */
#define DioConf_DioPort_PORTF   ((Dio_PortType)5U)   /**< GPIOF */
#define DioConf_DioPort_PORTG   ((Dio_PortType)6U)   /**< GPIOG */
#define DioConf_DioPort_PORTH   ((Dio_PortType)7U)   /**< GPIOH */
#define DioConf_DioPort_PORTI   ((Dio_PortType)8U)   /**< GPIOI */
#define DioConf_DioPort_PORTJ   ((Dio_PortType)9U)   /**< GPIOJ */
#define DioConf_DioPort_PORTK   ((Dio_PortType)10U)  /**< GPIOK */

/* ─── Channel Symbolic Names ─────────────────────────────────────────────── */

/* [SWS_Dio_00017] Encoding: ChannelId = (PortIndex * 16) + PinNumber */

/* ----- Port A ----- */
#define DioConf_DioChannel_PA0    ((Dio_ChannelType)0U)
#define DioConf_DioChannel_PA1    ((Dio_ChannelType)1U)
#define DioConf_DioChannel_PA2    ((Dio_ChannelType)2U)
#define DioConf_DioChannel_PA3    ((Dio_ChannelType)3U)
#define DioConf_DioChannel_PA4    ((Dio_ChannelType)4U)
#define DioConf_DioChannel_PA5    ((Dio_ChannelType)5U)
#define DioConf_DioChannel_PA6    ((Dio_ChannelType)6U)
#define DioConf_DioChannel_PA7    ((Dio_ChannelType)7U)
#define DioConf_DioChannel_PA8    ((Dio_ChannelType)8U)
#define DioConf_DioChannel_PA9    ((Dio_ChannelType)9U)
#define DioConf_DioChannel_PA10   ((Dio_ChannelType)10U)
#define DioConf_DioChannel_PA11   ((Dio_ChannelType)11U)
#define DioConf_DioChannel_PA12   ((Dio_ChannelType)12U)
#define DioConf_DioChannel_PA13   ((Dio_ChannelType)13U)
#define DioConf_DioChannel_PA14   ((Dio_ChannelType)14U)
#define DioConf_DioChannel_PA15   ((Dio_ChannelType)15U)

/* ----- Port B ----- */
#define DioConf_DioChannel_PB0    ((Dio_ChannelType)16U)
#define DioConf_DioChannel_PB1    ((Dio_ChannelType)17U)
#define DioConf_DioChannel_PB2    ((Dio_ChannelType)18U)
#define DioConf_DioChannel_PB3    ((Dio_ChannelType)19U)
#define DioConf_DioChannel_PB4    ((Dio_ChannelType)20U)
#define DioConf_DioChannel_PB5    ((Dio_ChannelType)21U)
#define DioConf_DioChannel_PB6    ((Dio_ChannelType)22U)
#define DioConf_DioChannel_PB7    ((Dio_ChannelType)23U)
#define DioConf_DioChannel_PB8    ((Dio_ChannelType)24U)
#define DioConf_DioChannel_PB9    ((Dio_ChannelType)25U)
#define DioConf_DioChannel_PB10   ((Dio_ChannelType)26U)
#define DioConf_DioChannel_PB11   ((Dio_ChannelType)27U)
#define DioConf_DioChannel_PB12   ((Dio_ChannelType)28U)
#define DioConf_DioChannel_PB13   ((Dio_ChannelType)29U)
#define DioConf_DioChannel_PB14   ((Dio_ChannelType)30U)
#define DioConf_DioChannel_PB15   ((Dio_ChannelType)31U)

/* ----- Port C ----- */
#define DioConf_DioChannel_PC0    ((Dio_ChannelType)32U)
#define DioConf_DioChannel_PC1    ((Dio_ChannelType)33U)
#define DioConf_DioChannel_PC2    ((Dio_ChannelType)34U)
#define DioConf_DioChannel_PC3    ((Dio_ChannelType)35U)
#define DioConf_DioChannel_PC4    ((Dio_ChannelType)36U)
#define DioConf_DioChannel_PC5    ((Dio_ChannelType)37U)
#define DioConf_DioChannel_PC6    ((Dio_ChannelType)38U)
#define DioConf_DioChannel_PC7    ((Dio_ChannelType)39U)
#define DioConf_DioChannel_PC8    ((Dio_ChannelType)40U)
#define DioConf_DioChannel_PC9    ((Dio_ChannelType)41U)
#define DioConf_DioChannel_PC10   ((Dio_ChannelType)42U)
#define DioConf_DioChannel_PC11   ((Dio_ChannelType)43U)
#define DioConf_DioChannel_PC12   ((Dio_ChannelType)44U)
#define DioConf_DioChannel_PC13   ((Dio_ChannelType)45U)
#define DioConf_DioChannel_PC14   ((Dio_ChannelType)46U)
#define DioConf_DioChannel_PC15   ((Dio_ChannelType)47U)

/* ----- Port D ----- */
#define DioConf_DioChannel_PD0    ((Dio_ChannelType)48U)
#define DioConf_DioChannel_PD1    ((Dio_ChannelType)49U)
#define DioConf_DioChannel_PD2    ((Dio_ChannelType)50U)
#define DioConf_DioChannel_PD3    ((Dio_ChannelType)51U)
#define DioConf_DioChannel_PD4    ((Dio_ChannelType)52U)
#define DioConf_DioChannel_PD5    ((Dio_ChannelType)53U)
#define DioConf_DioChannel_PD6    ((Dio_ChannelType)54U)
#define DioConf_DioChannel_PD7    ((Dio_ChannelType)55U)
#define DioConf_DioChannel_PD8    ((Dio_ChannelType)56U)
#define DioConf_DioChannel_PD9    ((Dio_ChannelType)57U)
#define DioConf_DioChannel_PD10   ((Dio_ChannelType)58U)
#define DioConf_DioChannel_PD11   ((Dio_ChannelType)59U)
#define DioConf_DioChannel_PD12   ((Dio_ChannelType)60U)
#define DioConf_DioChannel_PD13   ((Dio_ChannelType)61U)
#define DioConf_DioChannel_PD14   ((Dio_ChannelType)62U)
#define DioConf_DioChannel_PD15   ((Dio_ChannelType)63U)

/* ----- Port E ----- */
#define DioConf_DioChannel_PE0    ((Dio_ChannelType)64U)
#define DioConf_DioChannel_PE1    ((Dio_ChannelType)65U)
#define DioConf_DioChannel_PE2    ((Dio_ChannelType)66U)
#define DioConf_DioChannel_PE3    ((Dio_ChannelType)67U)
#define DioConf_DioChannel_PE4    ((Dio_ChannelType)68U)
#define DioConf_DioChannel_PE5    ((Dio_ChannelType)69U)
#define DioConf_DioChannel_PE6    ((Dio_ChannelType)70U)
#define DioConf_DioChannel_PE7    ((Dio_ChannelType)71U)
#define DioConf_DioChannel_PE8    ((Dio_ChannelType)72U)
#define DioConf_DioChannel_PE9    ((Dio_ChannelType)73U)
#define DioConf_DioChannel_PE10   ((Dio_ChannelType)74U)
#define DioConf_DioChannel_PE11   ((Dio_ChannelType)75U)
#define DioConf_DioChannel_PE12   ((Dio_ChannelType)76U)
#define DioConf_DioChannel_PE13   ((Dio_ChannelType)77U)
#define DioConf_DioChannel_PE14   ((Dio_ChannelType)78U)
#define DioConf_DioChannel_PE15   ((Dio_ChannelType)79U)

/* ----- Port F ----- */
#define DioConf_DioChannel_PF0    ((Dio_ChannelType)80U)
#define DioConf_DioChannel_PF1    ((Dio_ChannelType)81U)
#define DioConf_DioChannel_PF2    ((Dio_ChannelType)82U)
#define DioConf_DioChannel_PF3    ((Dio_ChannelType)83U)
#define DioConf_DioChannel_PF4    ((Dio_ChannelType)84U)
#define DioConf_DioChannel_PF5    ((Dio_ChannelType)85U)
#define DioConf_DioChannel_PF6    ((Dio_ChannelType)86U)
#define DioConf_DioChannel_PF7    ((Dio_ChannelType)87U)
#define DioConf_DioChannel_PF8    ((Dio_ChannelType)88U)
#define DioConf_DioChannel_PF9    ((Dio_ChannelType)89U)
#define DioConf_DioChannel_PF10   ((Dio_ChannelType)90U)
#define DioConf_DioChannel_PF11   ((Dio_ChannelType)91U)
#define DioConf_DioChannel_PF12   ((Dio_ChannelType)92U)
#define DioConf_DioChannel_PF13   ((Dio_ChannelType)93U)
#define DioConf_DioChannel_PF14   ((Dio_ChannelType)94U)
#define DioConf_DioChannel_PF15   ((Dio_ChannelType)95U)

/* ----- Port G ----- */
#define DioConf_DioChannel_PG0    ((Dio_ChannelType)96U)
#define DioConf_DioChannel_PG1    ((Dio_ChannelType)97U)
#define DioConf_DioChannel_PG2    ((Dio_ChannelType)98U)
#define DioConf_DioChannel_PG3    ((Dio_ChannelType)99U)
#define DioConf_DioChannel_PG4    ((Dio_ChannelType)100U)
#define DioConf_DioChannel_PG5    ((Dio_ChannelType)101U)
#define DioConf_DioChannel_PG6    ((Dio_ChannelType)102U)
#define DioConf_DioChannel_PG7    ((Dio_ChannelType)103U)
#define DioConf_DioChannel_PG8    ((Dio_ChannelType)104U)
#define DioConf_DioChannel_PG9    ((Dio_ChannelType)105U)
#define DioConf_DioChannel_PG10   ((Dio_ChannelType)106U)
#define DioConf_DioChannel_PG11   ((Dio_ChannelType)107U)
#define DioConf_DioChannel_PG12   ((Dio_ChannelType)108U)
#define DioConf_DioChannel_PG13   ((Dio_ChannelType)109U)
#define DioConf_DioChannel_PG14   ((Dio_ChannelType)110U)
#define DioConf_DioChannel_PG15   ((Dio_ChannelType)111U)

/* Ports H..K follow the same pattern (PortIndex * 16 + PinNumber);
 * add definitions here as pins on those ports are brought into use. */

/* ─── Configuration Limits ───────────────────────────────────────────────── */

/* Used for DET channel/port range validation. */

/** Total number of STM32H753 GPIO ports (A..K = 11) */
#define DIO_NUM_PORTS           (11U)

/** Pins per port */
#define DIO_PINS_PER_PORT       (16U)

/** Maximum valid ChannelId (port K, pin 15) */
#define DIO_MAX_CHANNEL_ID      ((Dio_ChannelType)((DIO_NUM_PORTS * DIO_PINS_PER_PORT) - 1U))

/** Maximum valid PortId */
#define DIO_MAX_PORT_ID         ((Dio_PortType)(DIO_NUM_PORTS - 1U))

#ifdef __cplusplus
}
#endif

#endif /* DIO_CFG_H */