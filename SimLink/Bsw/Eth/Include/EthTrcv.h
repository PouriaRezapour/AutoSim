/**
 * @file    EthTrcv.h
 * @author  Ali Rezapour
 * @date    2025-05-10
 * @version 0.1.0
 * @brief   Function prototypes and definitions for the LAN8742 PHY transceiver driver.
 *
 * @details
 * This header exposes the full register map, bit-field definitions, status
 * codes, interrupt flags, I/O context types, and public API for the LAN8742
 * 10/100 Ethernet PHY. The driver communicates with the PHY exclusively
 * through an abstract I/O context (EthTrcv_IOCtx_t), making it independent
 * of any particular MDIO bus implementation.
 *
 * @par Revision History:
 * |---------|------------|---------------|--------------------------------------|
 * | Version | Date       | Author        | Description                          |
 * |---------|------------|---------------|--------------------------------------|
 * | 0.1.0   | 2025-05-10 | Ali Rezapour  | Initial release                      |
 * |---------|------------|---------------|--------------------------------------|
 */

#ifndef ETHTRCV_H
#define ETHTRCV_H

#ifdef __cplusplus
extern "C" {
#endif

/* ─── Includes ────────────────────────────────────────────────────────────── */

#include "Std_Types.h"

/* ─── Register Map ────────────────────────────────────────────────────────── */

#define LAN8742_BCR      ((uint16)0x0000U)  /**< Basic Control Register            */
#define LAN8742_BSR      ((uint16)0x0001U)  /**< Basic Status Register             */
#define LAN8742_PHYI1R   ((uint16)0x0002U)  /**< PHY Identifier 1 Register         */
#define LAN8742_PHYI2R   ((uint16)0x0003U)  /**< PHY Identifier 2 Register         */
#define LAN8742_ANAR     ((uint16)0x0004U)  /**< Auto-Negotiation Advertisement    */
#define LAN8742_ANLPAR   ((uint16)0x0005U)  /**< Auto-Neg Link Partner Ability     */
#define LAN8742_ANER     ((uint16)0x0006U)  /**< Auto-Negotiation Expansion        */
#define LAN8742_ANNPTR   ((uint16)0x0007U)  /**< Auto-Neg Next Page TX             */
#define LAN8742_ANNPRR   ((uint16)0x0008U)  /**< Auto-Neg Next Page RX             */
#define LAN8742_MMDACR   ((uint16)0x000DU)  /**< MMD Access Control Register       */
#define LAN8742_MMDAADR  ((uint16)0x000EU)  /**< MMD Access Address/Data Register  */
#define LAN8742_ENCTR    ((uint16)0x0010U)  /**< EDPD NLP/Crossover Timer Control  */
#define LAN8742_MCSR     ((uint16)0x0011U)  /**< Mode Control/Status Register      */
#define LAN8742_SMR      ((uint16)0x0012U)  /**< Special Modes Register            */
#define LAN8742_TPDCR    ((uint16)0x0018U)  /**< TDR Patterns/Delay Control        */
#define LAN8742_TCSR     ((uint16)0x0019U)  /**< TDR Control/Status Register       */
#define LAN8742_SECR     ((uint16)0x001AU)  /**< Symbol Error Counter Register     */
#define LAN8742_SCSIR    ((uint16)0x001BU)  /**< Special Control/Status Indication */
#define LAN8742_CLR      ((uint16)0x001CU)  /**< Cable Length Register             */
#define LAN8742_ISFR     ((uint16)0x001DU)  /**< Interrupt Source Flag Register    */
#define LAN8742_IMR      ((uint16)0x001EU)  /**< Interrupt Mask Register           */
#define LAN8742_PHYSCSR  ((uint16)0x001FU)  /**< PHY Special Control/Status        */

/* ─── BCR Bit Definitions ─────────────────────────────────────────────────── */

#define LAN8742_BCR_SOFT_RESET         ((uint16)0x8000U)  /**< Software reset              */
#define LAN8742_BCR_LOOPBACK           ((uint16)0x4000U)  /**< Loopback enable             */
#define LAN8742_BCR_SPEED_SELECT       ((uint16)0x2000U)  /**< Speed select (1 = 100 Mb/s) */
#define LAN8742_BCR_AUTONEGO_EN        ((uint16)0x1000U)  /**< Auto-negotiation enable     */
#define LAN8742_BCR_POWER_DOWN         ((uint16)0x0800U)  /**< Power-down mode             */
#define LAN8742_BCR_ISOLATE            ((uint16)0x0400U)  /**< Electrically isolate PHY    */
#define LAN8742_BCR_RESTART_AUTONEGO   ((uint16)0x0200U)  /**< Restart auto-negotiation    */
#define LAN8742_BCR_DUPLEX_MODE        ((uint16)0x0100U)  /**< Duplex mode (1 = full)      */

/* ─── BSR Bit Definitions ─────────────────────────────────────────────────── */

#define LAN8742_BSR_100BASE_T4         ((uint16)0x8000U)  /**< 100BASE-T4 capable          */
#define LAN8742_BSR_100BASE_TX_FD      ((uint16)0x4000U)  /**< 100BASE-TX full-duplex      */
#define LAN8742_BSR_100BASE_TX_HD      ((uint16)0x2000U)  /**< 100BASE-TX half-duplex      */
#define LAN8742_BSR_10BASE_T_FD        ((uint16)0x1000U)  /**< 10BASE-T full-duplex        */
#define LAN8742_BSR_10BASE_T_HD        ((uint16)0x0800U)  /**< 10BASE-T half-duplex        */
#define LAN8742_BSR_100BASE_T2_FD      ((uint16)0x0400U)  /**< 100BASE-T2 full-duplex      */
#define LAN8742_BSR_100BASE_T2_HD      ((uint16)0x0200U)  /**< 100BASE-T2 half-duplex      */
#define LAN8742_BSR_EXTENDED_STATUS    ((uint16)0x0100U)  /**< Extended status available   */
#define LAN8742_BSR_AUTONEGO_CPLT      ((uint16)0x0020U)  /**< Auto-negotiation complete   */
#define LAN8742_BSR_REMOTE_FAULT       ((uint16)0x0010U)  /**< Remote fault detected       */
#define LAN8742_BSR_AUTONEGO_ABILITY   ((uint16)0x0008U)  /**< Auto-negotiation capable    */
#define LAN8742_BSR_LINK_STATUS        ((uint16)0x0004U)  /**< Link up                     */
#define LAN8742_BSR_JABBER_DETECT      ((uint16)0x0002U)  /**< Jabber condition detected   */
#define LAN8742_BSR_EXTENDED_CAP       ((uint16)0x0001U)  /**< Extended capability present */

/* ─── PHYI1R Bit Definitions ─────────────────────────────────────────────── */

#define LAN8742_PHYI1R_OUI_3_18        ((uint16)0xFFFFU)  /**< OUI bits [3:18]             */

/* ─── PHYI2R Bit Definitions ─────────────────────────────────────────────── */

#define LAN8742_PHYI2R_OUI_19_24       ((uint16)0xFC00U)  /**< OUI bits [19:24]            */
#define LAN8742_PHYI2R_MODEL_NBR       ((uint16)0x03F0U)  /**< Device model number         */
#define LAN8742_PHYI2R_REVISION_NBR    ((uint16)0x000FU)  /**< Device revision number      */

/* ─── ANAR Bit Definitions ───────────────────────────────────────────────── */

#define LAN8742_ANAR_NEXT_PAGE              ((uint16)0x8000U)  /**< Next page capable           */
#define LAN8742_ANAR_REMOTE_FAULT           ((uint16)0x2000U)  /**< Remote fault                */
#define LAN8742_ANAR_PAUSE_OPERATION        ((uint16)0x0C00U)  /**< Pause operation mask        */
#define LAN8742_ANAR_PO_NOPAUSE            ((uint16)0x0000U)  /**< No pause                    */
#define LAN8742_ANAR_PO_SYMMETRIC_PAUSE    ((uint16)0x0400U)  /**< Symmetric pause             */
#define LAN8742_ANAR_PO_ASYMMETRIC_PAUSE   ((uint16)0x0800U)  /**< Asymmetric pause            */
#define LAN8742_ANAR_PO_ADVERTISE_SUPPORT  ((uint16)0x0C00U)  /**< Advertise pause support     */
#define LAN8742_ANAR_100BASE_TX_FD         ((uint16)0x0100U)  /**< Advertise 100BASE-TX FD     */
#define LAN8742_ANAR_100BASE_TX            ((uint16)0x0080U)  /**< Advertise 100BASE-TX HD     */
#define LAN8742_ANAR_10BASE_T_FD           ((uint16)0x0040U)  /**< Advertise 10BASE-T FD       */
#define LAN8742_ANAR_10BASE_T              ((uint16)0x0020U)  /**< Advertise 10BASE-T HD       */
#define LAN8742_ANAR_SELECTOR_FIELD        ((uint16)0x000FU)  /**< Protocol selector field     */

/* ─── ANLPAR Bit Definitions ─────────────────────────────────────────────── */

#define LAN8742_ANLPAR_NEXT_PAGE            ((uint16)0x8000U)  /**< Link partner next page       */
#define LAN8742_ANLPAR_REMOTE_FAULT         ((uint16)0x2000U)  /**< Link partner remote fault    */
#define LAN8742_ANLPAR_PAUSE_OPERATION      ((uint16)0x0C00U)  /**< Link partner pause mask      */
#define LAN8742_ANLPAR_PO_NOPAUSE           ((uint16)0x0000U)  /**< No pause                     */
#define LAN8742_ANLPAR_PO_SYMMETRIC_PAUSE   ((uint16)0x0400U)  /**< Symmetric pause              */
#define LAN8742_ANLPAR_PO_ASYMMETRIC_PAUSE  ((uint16)0x0800U)  /**< Asymmetric pause             */
#define LAN8742_ANLPAR_PO_ADVERTISE_SUPPORT ((uint16)0x0C00U)  /**< Advertise pause support      */
#define LAN8742_ANLPAR_100BASE_TX_FD        ((uint16)0x0100U)  /**< LP supports 100BASE-TX FD   */
#define LAN8742_ANLPAR_100BASE_TX           ((uint16)0x0080U)  /**< LP supports 100BASE-TX HD   */
#define LAN8742_ANLPAR_10BASE_T_FD          ((uint16)0x0040U)  /**< LP supports 10BASE-T FD     */
#define LAN8742_ANLPAR_10BASE_T             ((uint16)0x0020U)  /**< LP supports 10BASE-T HD     */
#define LAN8742_ANLPAR_SELECTOR_FIELD       ((uint16)0x000FU)  /**< LP protocol selector field  */

/* ─── ANER Bit Definitions ───────────────────────────────────────────────── */

#define LAN8742_ANER_RX_NP_LOCATION_ABLE    ((uint16)0x0040U)  /**< RX next-page location able  */
#define LAN8742_ANER_RX_NP_STORAGE_LOCATION ((uint16)0x0020U)  /**< RX next-page storage loc.   */
#define LAN8742_ANER_PARALLEL_DETECT_FAULT  ((uint16)0x0010U)  /**< Parallel detection fault    */
#define LAN8742_ANER_LP_NP_ABLE             ((uint16)0x0008U)  /**< Link partner next-page able */
#define LAN8742_ANER_NP_ABLE                ((uint16)0x0004U)  /**< Local next-page ability     */
#define LAN8742_ANER_PAGE_RECEIVED          ((uint16)0x0002U)  /**< New page received           */
#define LAN8742_ANER_LP_AUTONEG_ABLE        ((uint16)0x0001U)  /**< LP auto-neg capable         */

/* ─── ANNPTR Bit Definitions ─────────────────────────────────────────────── */

#define LAN8742_ANNPTR_NEXT_PAGE         ((uint16)0x8000U)  /**< Next page indication        */
#define LAN8742_ANNPTR_MESSAGE_PAGE      ((uint16)0x2000U)  /**< Message page                */
#define LAN8742_ANNPTR_ACK2              ((uint16)0x1000U)  /**< Acknowledge 2               */
#define LAN8742_ANNPTR_TOGGLE            ((uint16)0x0800U)  /**< Toggle bit                  */
#define LAN8742_ANNPTR_MESSAGGE_CODE     ((uint16)0x07FFU)  /**< Message/unformatted code    */

/* ─── ANNPRR Bit Definitions ─────────────────────────────────────────────── */

#define LAN8742_ANNPRR_NEXT_PAGE         ((uint16)0x8000U)  /**< Next page indication        */
#define LAN8742_ANNPRR_ACK               ((uint16)0x4000U)  /**< Acknowledge                 */
#define LAN8742_ANNPRR_MESSAGE_PAGE      ((uint16)0x2000U)  /**< Message page                */
#define LAN8742_ANNPRR_ACK2              ((uint16)0x1000U)  /**< Acknowledge 2               */
#define LAN8742_ANNPRR_TOGGLE            ((uint16)0x0800U)  /**< Toggle bit                  */
#define LAN8742_ANNPRR_MESSAGGE_CODE     ((uint16)0x07FFU)  /**< Message/unformatted code    */

/* ─── MMDACR Bit Definitions ─────────────────────────────────────────────── */

#define LAN8742_MMDACR_MMD_FUNCTION       ((uint16)0xC000U)  /**< MMD function select mask   */
#define LAN8742_MMDACR_MMD_FUNCTION_ADDR  ((uint16)0x0000U)  /**< MMD function: address      */
#define LAN8742_MMDACR_MMD_FUNCTION_DATA  ((uint16)0x4000U)  /**< MMD function: data         */
#define LAN8742_MMDACR_MMD_DEV_ADDR       ((uint16)0x001FU)  /**< MMD device address field   */

/* ─── ENCTR Bit Definitions ──────────────────────────────────────────────── */

#define LAN8742_ENCTR_TX_ENABLE             ((uint16)0x8000U)  /**< EDPD TX NLP enable           */
#define LAN8742_ENCTR_TX_TIMER              ((uint16)0x6000U)  /**< TX NLP timer mask            */
#define LAN8742_ENCTR_TX_TIMER_1S           ((uint16)0x0000U)  /**< TX timer 1 s                 */
#define LAN8742_ENCTR_TX_TIMER_768MS        ((uint16)0x2000U)  /**< TX timer 768 ms              */
#define LAN8742_ENCTR_TX_TIMER_512MS        ((uint16)0x4000U)  /**< TX timer 512 ms              */
#define LAN8742_ENCTR_TX_TIMER_265MS        ((uint16)0x6000U)  /**< TX timer 265 ms              */
#define LAN8742_ENCTR_RX_ENABLE             ((uint16)0x1000U)  /**< EDPD RX NLP enable           */
#define LAN8742_ENCTR_RX_MAX_INTERVAL       ((uint16)0x0C00U)  /**< RX max interval mask         */
#define LAN8742_ENCTR_RX_MAX_INTERVAL_64MS  ((uint16)0x0000U)  /**< RX max interval 64 ms        */
#define LAN8742_ENCTR_RX_MAX_INTERVAL_256MS ((uint16)0x0400U)  /**< RX max interval 256 ms       */
#define LAN8742_ENCTR_RX_MAX_INTERVAL_512MS ((uint16)0x0800U)  /**< RX max interval 512 ms       */
#define LAN8742_ENCTR_RX_MAX_INTERVAL_1S    ((uint16)0x0C00U)  /**< RX max interval 1 s          */
#define LAN8742_ENCTR_EX_CROSS_OVER         ((uint16)0x0002U)  /**< Extended crossover enable    */
#define LAN8742_ENCTR_EX_MANUAL_CROSS_OVER  ((uint16)0x0001U)  /**< Manual crossover enable      */

/* ─── MCSR Bit Definitions ───────────────────────────────────────────────── */

#define LAN8742_MCSR_EDPWRDOWN   ((uint16)0x2000U)  /**< Energy Detect power-down     */
#define LAN8742_MCSR_FARLOOPBACK ((uint16)0x0200U)  /**< Far end loopback mode        */
#define LAN8742_MCSR_ALTINT      ((uint16)0x0040U)  /**< Alternate interrupt mode     */
#define LAN8742_MCSR_ENERGYON    ((uint16)0x0002U)  /**< Energy detected on line      */

/* ─── SMR Bit Definitions ────────────────────────────────────────────────── */

#define LAN8742_SMR_MODE       ((uint16)0x00E0U)  /**< PHY operating mode field     */
#define LAN8742_SMR_PHY_ADDR   ((uint16)0x001FU)  /**< PHY MDIO address field       */

/* ─── TPDCR Bit Definitions ──────────────────────────────────────────────── */

#define LAN8742_TPDCR_DELAY_IN           ((uint16)0x8000U)  /**< TDR delay in enable      */
#define LAN8742_TPDCR_LINE_BREAK_COUNTER ((uint16)0x7000U)  /**< Line break counter       */
#define LAN8742_TPDCR_PATTERN_HIGH       ((uint16)0x0FC0U)  /**< TDR pattern high field   */
#define LAN8742_TPDCR_PATTERN_LOW        ((uint16)0x003FU)  /**< TDR pattern low field    */

/* ─── TCSR Bit Definitions ───────────────────────────────────────────────── */

#define LAN8742_TCSR_TDR_ENABLE            ((uint16)0x8000U)  /**< TDR enable                  */
#define LAN8742_TCSR_TDR_AD_FILTER_ENABLE  ((uint16)0x4000U)  /**< TDR A/D filter enable       */
#define LAN8742_TCSR_TDR_CH_CABLE_TYPE     ((uint16)0x0600U)  /**< TDR channel cable type mask */
#define LAN8742_TCSR_TDR_CH_CABLE_DEFAULT  ((uint16)0x0000U)  /**< Cable type: default         */
#define LAN8742_TCSR_TDR_CH_CABLE_SHORTED  ((uint16)0x0200U)  /**< Cable type: shorted         */
#define LAN8742_TCSR_TDR_CH_CABLE_OPEN     ((uint16)0x0400U)  /**< Cable type: open            */
#define LAN8742_TCSR_TDR_CH_CABLE_MATCH    ((uint16)0x0600U)  /**< Cable type: impedance match */
#define LAN8742_TCSR_TDR_CH_STATUS         ((uint16)0x0100U)  /**< TDR channel status          */
#define LAN8742_TCSR_TDR_CH_LENGTH         ((uint16)0x00FFU)  /**< TDR channel length          */

/* ─── SCSIR Bit Definitions ──────────────────────────────────────────────── */

#define LAN8742_SCSIR_AUTO_MDIX_ENABLE ((uint16)0x8000U)  /**< Auto-MDIX enable            */
#define LAN8742_SCSIR_CHANNEL_SELECT   ((uint16)0x2000U)  /**< MDI/MDI-X channel select    */
#define LAN8742_SCSIR_SQE_DISABLE      ((uint16)0x0800U)  /**< SQE test disable            */
#define LAN8742_SCSIR_XPOLALITY        ((uint16)0x0010U)  /**< Polarity reversal detected  */

/* ─── CLR Bit Definitions ────────────────────────────────────────────────── */

#define LAN8742_CLR_CABLE_LENGTH  ((uint16)0xF000U)  /**< Estimated cable length      */

/* ─── IMR / ISFR Bit Definitions ─────────────────────────────────────────── */

#define LAN8742_INT_8  ((uint16)0x0100U)  /**< Interrupt source 8          */
#define LAN8742_INT_7  ((uint16)0x0080U)  /**< Interrupt source 7          */
#define LAN8742_INT_6  ((uint16)0x0040U)  /**< Interrupt source 6          */
#define LAN8742_INT_5  ((uint16)0x0020U)  /**< Interrupt source 5          */
#define LAN8742_INT_4  ((uint16)0x0010U)  /**< Interrupt source 4          */
#define LAN8742_INT_3  ((uint16)0x0008U)  /**< Interrupt source 3          */
#define LAN8742_INT_2  ((uint16)0x0004U)  /**< Interrupt source 2          */
#define LAN8742_INT_1  ((uint16)0x0002U)  /**< Interrupt source 1          */

/* ─── PHYSCSR Bit Definitions ────────────────────────────────────────────── */

#define LAN8742_PHYSCSR_AUTONEGO_DONE ((uint16)0x1000U)  /**< Auto-negotiation done       */
#define LAN8742_PHYSCSR_HCDSPEEDMASK  ((uint16)0x001CU)  /**< HCD speed indication mask   */
#define LAN8742_PHYSCSR_10BT_HD       ((uint16)0x0004U)  /**< 10BASE-T half-duplex        */
#define LAN8742_PHYSCSR_10BT_FD       ((uint16)0x0014U)  /**< 10BASE-T full-duplex        */
#define LAN8742_PHYSCSR_100BTX_HD     ((uint16)0x0008U)  /**< 100BASE-TX half-duplex      */
#define LAN8742_PHYSCSR_100BTX_FD     ((uint16)0x0018U)  /**< 100BASE-TX full-duplex      */

/* ─── Status Codes ────────────────────────────────────────────────────────── */

#define LAN8742_STATUS_READ_ERROR           ((int32_t)-5)  /**< Register read failed        */
#define LAN8742_STATUS_WRITE_ERROR          ((int32_t)-4)  /**< Register write failed       */
#define LAN8742_STATUS_ADDRESS_ERROR        ((int32_t)-3)  /**< PHY address not found       */
#define LAN8742_STATUS_RESET_TIMEOUT        ((int32_t)-2)  /**< Soft-reset timed out        */
#define LAN8742_STATUS_ERROR                ((int32_t)-1)  /**< Generic error               */
#define LAN8742_STATUS_OK                   ((int32_t) 0)  /**< Operation successful        */
#define LAN8742_STATUS_LINK_DOWN            ((int32_t) 1)  /**< Link is down                */
#define LAN8742_STATUS_100MBITS_FULLDUPLEX  ((int32_t) 2)  /**< 100 Mb/s full-duplex        */
#define LAN8742_STATUS_100MBITS_HALFDUPLEX  ((int32_t) 3)  /**< 100 Mb/s half-duplex        */
#define LAN8742_STATUS_10MBITS_FULLDUPLEX   ((int32_t) 4)  /**< 10 Mb/s full-duplex         */
#define LAN8742_STATUS_10MBITS_HALFDUPLEX   ((int32_t) 5)  /**< 10 Mb/s half-duplex         */
#define LAN8742_STATUS_AUTONEGO_NOTDONE     ((int32_t) 6)  /**< Auto-negotiation in progress*/

/* ─── Interrupt Flag Aliases ─────────────────────────────────────────────── */

#define LAN8742_WOL_IT                       LAN8742_INT_8  /**< Wake-on-LAN interrupt            */
#define LAN8742_ENERGYON_IT                  LAN8742_INT_7  /**< Energy-on interrupt              */
#define LAN8742_AUTONEGO_COMPLETE_IT         LAN8742_INT_6  /**< Auto-negotiation complete        */
#define LAN8742_REMOTE_FAULT_IT              LAN8742_INT_5  /**< Remote fault interrupt           */
#define LAN8742_LINK_DOWN_IT                 LAN8742_INT_4  /**< Link-down interrupt              */
#define LAN8742_AUTONEGO_LP_ACK_IT           LAN8742_INT_3  /**< Auto-neg LP acknowledge          */
#define LAN8742_PARALLEL_DETECTION_FAULT_IT  LAN8742_INT_2  /**< Parallel detection fault         */
#define LAN8742_AUTONEGO_PAGE_RECEIVED_IT    LAN8742_INT_1  /**< Auto-neg next-page received      */

/* ─── Type Definitions ────────────────────────────────────────────────────── */

typedef int32_t (*lan8742_Init_Func)     (void);
typedef int32_t (*lan8742_DeInit_Func)   (void);
typedef int32_t (*lan8742_ReadReg_Func)  (uint32, uint32, uint32 *);
typedef int32_t (*lan8742_WriteReg_Func) (uint32, uint32, uint32);
typedef int32_t (*lan8742_GetTick_Func)  (void);

/**
 * @brief  I/O function context passed to the driver to abstract the MDIO bus.
 *         All function pointers except Init and DeInit are mandatory.
 */
typedef struct
{
    lan8742_Init_Func      Init;
    lan8742_DeInit_Func    DeInit;
    lan8742_WriteReg_Func  WriteReg;
    lan8742_ReadReg_Func   ReadReg;
    lan8742_GetTick_Func   GetTick;
} lan8742_IOCtx_t;

/**
 * @brief  PHY device object. One instance per physical LAN8742 on the bus.
 */
typedef struct
{
    uint32         DevAddr;         /**< Resolved MDIO device address      */
    uint32         Is_Initialized;  /**< Initialisation flag               */
    lan8742_IOCtx_t  IO;              /**< Bound I/O function context        */
    void            *pData;           /**< Optional user data pointer        */
} lan8742_Object_t;

/* ─── Public Function Prototypes ──────────────────────────────────────────── */

/**
 * @brief  Bind I/O functions to a device object.
 * @param  pObj    Pointer to the LAN8742 device object.
 * @param  ioctx   Pointer to the I/O context holding bus functions.
 * @return LAN8742_STATUS_OK if all mandatory pointers are present,
 *         LAN8742_STATUS_ERROR if any mandatory pointer is NULL.
 */
int32_t LAN8742_RegisterBusIO(lan8742_Object_t *pObj, lan8742_IOCtx_t *ioctx);

/**
 * @brief  Initialize the LAN8742 and discover its MDIO address.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK on success,
 *         LAN8742_STATUS_ADDRESS_ERROR if no device is found,
 *         LAN8742_STATUS_READ_ERROR if a register read fails.
 */
int32_t LAN8742_Init(lan8742_Object_t *pObj);

/**
 * @brief  De-initialize the LAN8742 and release hardware resources.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK on success, LAN8742_STATUS_ERROR on failure.
 */
int32_t LAN8742_DeInit(lan8742_Object_t *pObj);

/**
 * @brief  Clear the power-down bit in the BCR to wake the PHY.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK, LAN8742_STATUS_READ_ERROR, or
 *         LAN8742_STATUS_WRITE_ERROR.
 */
int32_t LAN8742_DisablePowerDownMode(lan8742_Object_t *pObj);

/**
 * @brief  Set the power-down bit in the BCR to put the PHY to sleep.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK, LAN8742_STATUS_READ_ERROR, or
 *         LAN8742_STATUS_WRITE_ERROR.
 */
int32_t LAN8742_EnablePowerDownMode(lan8742_Object_t *pObj);

/**
 * @brief  Enable and restart the auto-negotiation process.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK, LAN8742_STATUS_READ_ERROR, or
 *         LAN8742_STATUS_WRITE_ERROR.
 */
int32_t LAN8742_StartAutoNego(lan8742_Object_t *pObj);

/**
 * @brief  Read the current link state from the PHY registers.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return One of: LAN8742_STATUS_LINK_DOWN, LAN8742_STATUS_AUTONEGO_NOTDONE,
 *         LAN8742_STATUS_100MBITS_FULLDUPLEX, LAN8742_STATUS_100MBITS_HALFDUPLEX,
 *         LAN8742_STATUS_10MBITS_FULLDUPLEX, LAN8742_STATUS_10MBITS_HALFDUPLEX,
 *         LAN8742_STATUS_READ_ERROR, or LAN8742_STATUS_WRITE_ERROR.
 */
int32_t LAN8742_GetLinkState(lan8742_Object_t *pObj);

/**
 * @brief  Force a specific link speed and duplex mode via the BCR.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  LinkState  Desired link state; one of:
 *                    LAN8742_STATUS_100MBITS_FULLDUPLEX,
 *                    LAN8742_STATUS_100MBITS_HALFDUPLEX,
 *                    LAN8742_STATUS_10MBITS_FULLDUPLEX, or
 *                    LAN8742_STATUS_10MBITS_HALFDUPLEX.
 * @return LAN8742_STATUS_OK, LAN8742_STATUS_ERROR (bad parameter),
 *         LAN8742_STATUS_READ_ERROR, or LAN8742_STATUS_WRITE_ERROR.
 */
int32_t LAN8742_SetLinkState(lan8742_Object_t *pObj, uint32 LinkState);

/**
 * @brief  Enable MAC-level loopback mode in the BCR.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK, LAN8742_STATUS_READ_ERROR, or
 *         LAN8742_STATUS_WRITE_ERROR.
 */
int32_t LAN8742_EnableLoopbackMode(lan8742_Object_t *pObj);

/**
 * @brief  Disable MAC-level loopback mode in the BCR.
 * @param  pObj  Pointer to the LAN8742 device object.
 * @return LAN8742_STATUS_OK, LAN8742_STATUS_READ_ERROR, or
 *         LAN8742_STATUS_WRITE_ERROR.
 */
int32_t LAN8742_DisableLoopbackMode(lan8742_Object_t *pObj);

/**
 * @brief  Unmask one or more interrupt sources in the IMR.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  Interrupt  Bitmask of LAN8742_xxx_IT flags to enable.
 * @return LAN8742_STATUS_OK, LAN8742_STATUS_READ_ERROR, or
 *         LAN8742_STATUS_WRITE_ERROR.
 */
int32_t LAN8742_EnableIT(lan8742_Object_t *pObj, uint32 Interrupt);

/**
 * @brief  Mask one or more interrupt sources in the IMR.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  Interrupt  Bitmask of LAN8742_xxx_IT flags to disable.
 * @return LAN8742_STATUS_OK, LAN8742_STATUS_READ_ERROR, or
 *         LAN8742_STATUS_WRITE_ERROR.
 */
int32_t LAN8742_DisableIT(lan8742_Object_t *pObj, uint32 Interrupt);

/**
 * @brief  Clear pending interrupt flags by reading the ISFR.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  Interrupt  Bitmask of LAN8742_xxx_IT flags to clear.
 * @return LAN8742_STATUS_OK or LAN8742_STATUS_READ_ERROR.
 */
int32_t LAN8742_ClearIT(lan8742_Object_t *pObj, uint32 Interrupt);

/**
 * @brief  Check whether a specific interrupt flag is set in the ISFR.
 * @param  pObj       Pointer to the LAN8742 device object.
 * @param  Interrupt  Bitmask of LAN8742_xxx_IT flags to test.
 * @return 1 if the flag is set, 0 if clear,
 *         LAN8742_STATUS_READ_ERROR if the register cannot be read.
 */
int32_t LAN8742_GetITStatus(lan8742_Object_t *pObj, uint32 Interrupt);

#ifdef __cplusplus
}
#endif

#endif /* ETHTRCV_H */
