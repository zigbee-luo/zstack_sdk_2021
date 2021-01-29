/*******************************************************************************
  Filename:       mac.h
  Revised:        $Date: 2015-08-24 12:10:00 -0800 (Mon, 24 Aug 2015) $
  Revision:       $Revision: 42806 $

  Description:    This file contains the data structures and APIs for CC26xx
                  RF Core Firmware Specification for IEEE 802.15.4.

  Copyright 2009-2015 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License"). You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product. Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
*******************************************************************************/

#ifndef MAC_H
#define MAC_H

/*******************************************************************************
 * INCLUDES
 */
#include "hal_types.h"
#include "rf_hal.h"
#include "saddr.h"
#include "mac_autopend.h"

/*******************************************************************************
 * CONSTANTS
 */

#define RAT_BASE                       RFC_RAT_BASE

/* Registers */
#define MAC_RAT_COUNT                  HWREG(RAT_BASE + 0x004)
#define MAC_RAT_ADJ                    HWREG(RAT_BASE + 0x008)

/* MAC channels 27 and 28 for CC26XX */
#define MAC_CHAN_27_CC26XX             185
#define MAC_CHAN_28_CC26XX             190
#define MAC_CHAN_29_CC26XX             195

/*******************************************************************************
 * MACROS
 */

/* Radio Operation Trigger */
#define SET_RFOP_ALT_TRIG_CMD_BIT   BV(4)
#define SET_RFOP_PAST_TRIG_BIT      BV(7)

/* Receive queue entry configuration bit field */
#define SET_RX_AUTO_FLUSH_CRC(x)                                              \
  x |= BV(0)
#define SET_RX_AUTO_FLUSH_FRAME_FILTERING(x)                                  \
  x |= BV(1)
#define SET_RX_INCLUDE_PHY_HDR(x)                                             \
  x |= BV(2)
#define SET_RX_INCLUDE_CRC(x)                                                 \
  x |= BV(3)
#define SET_RX_APPEND_RSSI(x)                                                 \
  x |= BV(4)
#define SET_RX_APPEND_CORR_CRC(x)                                             \
  x |= BV(5)
#define SET_RX_APPEND_SRC_INDEX(x)                                            \
  x |= BV(6)
#define SET_RX_APPEND_TIMESTAMP(x)                                            \
  x |= BV(7)

#define SET_RX_AUTO_FLUSH_CRC_BIT              BV(0)
#define SET_RX_AUTO_FLUSH_FRAME_FILTERING_BIT  BV(1)
#define SET_RX_INCLUDE_PHY_HDR_BIT             BV(2)
#define SET_RX_INCLUDE_CRC_BIT                 BV(3)
#define SET_RX_APPEND_RSSI_BIT                 BV(4)
#define SET_RX_APPEND_CORR_CRC_BIT             BV(5)
#define SET_RX_APPEND_SRC_INDEX_BIT            BV(6)
#define SET_RX_APPEND_TIMESTAMP_BIT            BV(7)

/* CSMA Configuration */
#if defined( CC26XX_PG1 )
#define SET_CSMA_INIT_CW(x, v)                                                \
  x  = (((x) & ~0x3F) | ((v) & 0x3F))
#define SET_CSMA_SLOTTED(x)                                                   \
  x |= BV(6)
#define CLEAR_CSMA_SLOTTED(x)                                                 \
  x &= ~BV(6)
#define SET_CSMA_RX_OFF(x)                                                    \
  x |= BV(7)
#define SET_CSMA_RX_ON(x)                                                     \
  x &= ~BV(7)
#elif defined( CC26XX )
#define SET_CSMA_INIT_CW(x, v)                                                \
  x  = (((x) & ~0x1F) | ((v) & 0x1F))
#define SET_CSMA_SLOTTED(x)                                                   \
  x |= BV(5)
#define CLEAR_CSMA_SLOTTED(x)                                                 \
  x &= ~BV(5)
#define SET_CSMA_RX_OFF_MODE(x, v)                                            \
  x  = (((x) & ~0xC0) | (((v) & 0x03) << 6))
#else /* unknown device */
#error "ERROR: Unknown device!"
#endif /* CC26XX_PG1 */

/* TX options */
#define SET_TX_INCLUDE_PHY_HDR(x)                                             \
  x |= BV(0)
#define SET_TX_INCLUDE_CRC(x)                                                 \
  x |= BV(1)

/* RX frame filtering configuration bit field */
#define SET_FRAME_FILTER_ON(x)                                                \
  x |= BV(0)
#define SET_FRAME_FILTER_OFF(x)                                               \
  x &= ~BV(0)
#define SET_AUTO_ACK_ENABLE(x)                                                \
  x |= BV(2)
#define SET_AUTO_PEND_ENABLE(x)                                               \
  x |= BV(4)
#define SET_DEFAULT_PEND_ON(x)                                                \
  x |= BV(5)
#define SET_DEFAULT_PEND_OFF(x)                                               \
  x &= ~BV(5)
#define SET_DATA_REQ_AUTO_PEND(x)                                             \
  x |= BV(6)
#define SET_FRAME_FILTER_PAN_COORDINATOR(x, v)                                \
  x |= ((x) & ~BV(7) | ((v) ? BV(7) : 0))
#define SET_FRAME_FILTER_MAX_VERSION(x, v)                                    \
  x |= (((x) & ~0x0300) | (((v) & 0x03) << 8))
#define SET_STRICT_ACK_LEN(x)                                                 \
  x |= BV(15)

#define SET_FRAME_FILTER_ON_BIT               BV(0)
#define SET_AUTO_ACK_ENABLE_BIT               BV(2)
#define SET_AUTO_PEND_ENABLE_BIT              BV(4)
#define SET_DEFAULT_PEND_ON_BIT               BV(5)
#define SET_DATA_REQ_AUTO_PEND_BIT            BV(6)
#define SET_FRAME_FILTER_PAN_COORDINATOR_BIT  BV(7)
#define SET_FRAME_FILTER_MAX_VERSION_ONE      (1<<8)
#define SET_FRAME_FILTER_MAX_VERSION_TWO      (2<<8)
#define SET_FRAME_FILTER_MAX_VERSION_THREE    (3<<8)
#define SET_STRICT_ACK_LEN_BIT                BV(15)

/* RX frame type filtering bit field */
#define SET_ACCEPT_BEACON(x)                                                  \
  x |= BV(0)
#define SET_ACCEPT_DATA(x)                                                    \
  x |= BV(1)
#define SET_ACCEPT_ACK(x)                                                     \
  x |= BV(2)
#define SET_ACCEPT_MAC_CMD(x)                                                 \
  x |= BV(3)
#define SET_ACCEPT_FT5(x)                                                     \
  x |= BV(5)

#define SET_ACCEPT_BEACON_BIT   BV(0)
#define SET_ACCEPT_DATA_BIT     BV(1)
#define SET_ACCEPT_ACK_BIT      BV(2)
#define SET_ACCEPT_MAC_CMD_BIT  BV(3)
#define SET_ACCEPT_FT5_BIT      BV(5)

/* CCA Configuration bit field */
#define SET_ENABLE_ENERGY_SCAN_SOURCE(x)                                      \
  x |= BV(0)
#define SET_ENABLE_CORRELATOR_SOURCE(x)                                       \
  x |= BV(1)
#define SET_ENABLE_SYNC_FOUND_SOURCE(x)                                       \
  x |= BV(2)
#define SET_CORRELATOR_OPERATOR(x)                                            \
  x |= BV(3)
#define CLEAR_CORRELATOR_OPERATOR(x)                                          \
  x &= ~BV(3)
#define SET_SYNC_OPERATOR(x)                                                  \
  x |= BV(4)
#define CLEAR_SYNC_OPERATOR(x)                                                \
  x &= ~BV(4)
#define SET_CORRELATOR_THRESHOLD(x, v)                                        \
  x |= (((x) & 0x60) | (((v) & 0x03) << 5))

#define SET_ENABLE_ENERGY_SCAN_SOURCE_BIT  BV(0)
#define SET_ENABLE_CORRELATOR_SOURCE_BIT   BV(1)
#define SET_ENABLE_SYNC_FOUND_SOURCE_BIT   BV(2)
#define SET_CORRELATOR_OPERATOR_BIT        BV(3)
#define SET_SYNC_OPERATOR_BIT              BV(4)
#define SET_CORRELATOR_THRESHOLD_ONE       (1<<5)
#define SET_CORRELATOR_THRESHOLD_TWO       (2<<5)
#define SET_CORRELATOR_THRESHOLD_THREE     (3<<5)


/*******************************************************************************
 * CONSTANTS
 */

/* Mailbox 15.4 Immediate Commands */
#define CMD_IEEE_MOD_CCA               0x2001
#define CMD_IEEE_MOD_FILT              0x2002
#define CMD_IEEE_MOD_SRC_MATCH         0x2003
#define CMD_IEEE_ABORT_FG              0x2401
#define CMD_IEEE_STOP_FG               0x2402
#define CMD_IEEE_CCA_REQ               0x2403

/* Mailbox 15.4 Direct Commands */

/* Mailbox 15.4 Radio Commands - Background */
#define CMD_IEEE_RX                    0x2801
#define CMD_IEEE_ED_SCAN               0x2802

/* Mailbox 15.4 Radio Commands - Foreground */
#define CMD_IEEE_TX                    0x2C01
#define CMD_IEEE_CSMA                  0x2C02
#define CMD_IEEE_RX_ACK                0x2C03
#define CMD_IEEE_ABORT_BG              0x2C04
#define CMD_IEEE_SETUP                 0x2C05

/* Common Radio Operation Commands - Foreground */
#define CMD_FG_NOP                     0x0C01
#define CMD_FG_COUNT                   0x0C0B
#define CMD_FG_SCH_IMM                 0x0C10
#define CMD_FG_COUNT_BRANCH            0x0C12
#define CMD_FG_PATTERN_CHECK           0x0C13

/* IEEE Radio Operation Command Status */
#define MACSTAT_IDLE                   RFSTAT_IDLE
#define MACSTAT_PENDING                RFSTAT_PENDING
#define MACSTAT_ACTIVE                 RFSTAT_ACTIVE
#define MACSTAT_SUSPENDED              0x2001

#define MACSTAT_IEEE_DONE_OK           0x2400  /* result True  */
#define MACSTAT_IEEE_DONE_BUSY         0x2401  /* result False */
#define MACSTAT_IEEE_DONE_STOPPED      0x2402  /* result False */
#define MACSTAT_IEEE_DONE_ACK          0x2403  /* result True  */
#define MACSTAT_IEEE_DONE_ACKPEND      0x2404  /* result True  */
#define MACSTAT_IEEE_DONE_TIMEOUT      0x2405  /* result False */
#define MACSTAT_IEEE_DONE_BGEND        0x2406  /* result Abort */
#define MACSTAT_IEEE_DONE_ABORT        0x2407  /* result Abort */

#define MACSTAT_ERROR_WRONG_BG         0x0806  /* result Abort */
#define MACSTAT_ERROR_PAR              0x2800  /* result Abort */
#define MACSTAT_ERROR_NO_SETUP         0x2801  /* result Abort */
#define MACSTAT_ERROR_NO_FS            0x2802  /* result Abort */
#define MACSTAT_ERROR_SYNTH_PROG       0x2803  /* result Abort */
#define MACSTAT_ERROR_RXOVF            0x2804  /* result Abort */
#define MACSTAT_ERROR_TXUNF            0x2805  /* result Abort */

/* Radio Timer (RAT) Times (in 250ns ticks) */

/*******************************************************************************
 * TYPEDEFS
 */

/* MAC Radio Operation Receive Queue Configuration
 * |     7     |     6    |    5    |  4   |  3  |   2     |         1         |         0           |
 * | Timestamp | SrcIndex | CorrCRC | RSSI | CRC | PhyHdr  | AutoFlush Ignored | AutoFlush CRC Error |
 */
typedef uint8  rxQConfig_t;

/* MAC Frame Filter Configuration
 * |         15        |    14..13     |      12..10       |     9..8      |
 * | ACK Length Filter | Modify Filter | FCF Reserved Mask | Max Frame Ver |
 * |
 * |     7     |       6       |      5       |     4     |     3       |    2     |   1         |      0       |
 * | PAN Coord | Pend Data Req | Default Pend | Auto Pend | Slotted ACK | Auto ACK | Filter Stop | Frame Filter |
 */
typedef uint16  frmFilterOpt_t;

/* MAC Frame Types Filtering
 * |      7       |      6       |      5       |      4       |     3       |   2     |    1     |     0      |
 * | FT7 Reserved | FT6 Reserved | FT5 Reserved | FT4 Reserved | FT3 MAC Cmd | FT2 ACK | FT1 Data | FT0 Beacon |
 */
typedef uint8  frmTypes_t;

/* MAC CCA Source Configuration
 * |    7     |      5..6      |      4        |     3         |     2      |     1      |        0        |
 * | Reserved | Corr Threshold | Sync operator | Corr Operator | Sync Found | Correlator | CCA Energy Scan |
 */
typedef uint8  ccaOpt_t;

/* MAC CSMA Configuration for CC26XX PG1
 * |   7   |    6    |  5..0  |
 * | RxOff | Slotted | InitCW |
 *
 * MAC CSMA Configuration for CC26XX PG2
 * |    7..6     |    5    |  4..0  |
 * | Rx Off Mode | Slotted | InitCW |
 */
typedef uint8  csmaConfig_t;

/* MAC Tx Option
 * |        7..3        |    2     |  1  |    0    |
 * | Payload Length MSB | Reserved | CRC | PHY HDR |
 */
typedef uint8  txOption_t;

/* MAC Sourch Matching Option
 * |   7..3   |      2     |      1      |   0    |
 * | Reserved | Entry Type | Source Pend | Enable |
 */
typedef uint8  srcMatchOpt_t;

/* MAC CCA Info
 * |    7     |     6    |   5..4   |    3..2    |    1..0   |
 * | Reserved | CCA Sync | CCA Corr | CCA Energy | CCA State |
 */
typedef uint8 ccaInfo_t;

/* MAC Reveived Correlation/CRC Results
 * |     7     |   6    | 5..0 |
 * | CRC Error | Ignore | Corr |
 */
typedef uint8 corrCrc_t;

/*
** MAC Radio Commands
**
** R  = System CPU reads; Radio CPU will not read.
** W  = System CPU writes; Radio CPU reads but will not modify.
** RW = System CPU writes initially; Radio CPU reads and may modify.
*/

/*
** MAC Radio Operation Commands
*/

/*
** MAC Output Command Structures
*/

/* RX command */
PACKED_TYPEDEF_STRUCT
{
  uint8             nTxAck;            /* RW: num ACK frames transmitted */
  uint8             nRxBeacon;         /* RW: num beacon frames received */
  uint8             nRxData;           /* RW: num data frames received */
  uint8             nRxAck;            /* RW: num ACK frames received */
  uint8             nRxMacCmd;         /* RW: num MAC command frames received */
  uint8             nRxReserved;       /* RW: num reserved type frames received */
  uint8             nRxNok;            /* RW: num frames with CRC error received */
  uint8             nRxIgnored;        /* RW: num ignored frames received */
  uint8             nRxBufFull;        /* RW: num frames discarded due to buffer full */
  uint8             lastRssi;          /* R:  RSSI of the last received frame */
  uint8             maxRssi;           /* R:  Highest RSSI observed in the operation */
  uint8             reserved;          /* X:  Reserved */
  uint32            beaconTimeStamp;   /* R:  Time stamp of the last received beacon frame */
} output_t;

/*
** MAC Input Command Structures
*/

/* Short address entry structure */
PACKED_TYPEDEF_STRUCT
{
  uint16            shortAddr;         /* RW: Short address of the entry */
  uint16            panID;             /* RW: PAN ID of the entry */
} shortAddrEntry_t;

/* Extended address list structure */
PACKED_TYPEDEF_STRUCT
{
  uint32            srcMatchEn;        /* RW: Word with enable bits for each extAddrEntry */
  uint32            srcPendEn;         /* RW: Word with pending data bits for each extAddrEntry */
  sAddrExt_t        extAddrEntry[MAC_SRCMATCH_EXT_MAX_NUM_ENTRIES];
} extAddrList_t;

/* Short address list structure */
PACKED_TYPEDEF_STRUCT
{
  uint32            srcMatchEn;        /* RW: Word with enable bits for each shortAddrEntry */
  uint32            srcPendEn;         /* RW: Word with pending data bits for each shortAddrEntry */
  shortAddrEntry_t  shortAddrEntry[MAC_SRCMATCH_SHORT_MAX_NUM_ENTRIES];
} shortAddrList_t;

/* IEEE 802.15.4 Rx Command */
PACKED_TYPEDEF_STRUCT
{
  rfOpCmd_t         rfOpCmd;           /* radio command common structure */
  uint8             channel;           /* W:  channel to tune to */
  rxQConfig_t          rxCfg;             /* W:  Configuration bits */
  dataEntryQ_t     *pRXQ;              /* W:  ptr to Rx queue */
  output_t         *pOutput;           /* W:  ptr to result structure */
  frmFilterOpt_t    filterOpt;         /* RW: frame filter option */
  frmTypes_t        frmTypes;          /* RW: frame types to receive */
  ccaOpt_t          ccaOpt;            /* RW: CCA options */
  uint8             ccaRssiThreshold;  /* RW: RSSI threshold for CCA */
  uint8             reserved0;         /* X:  Reserved */
  uint8             numExtEntries;     /* W:  num extended address entries */
  uint8             numShortEntries;   /* W:  num short adress entries */
  extAddrList_t    *pExtEntryList;     /* W:  ptr to extended address entry list */
  shortAddrList_t  *pShortEntryList;   /* W:  ptr to short address entry list */
  sAddrExt_t        localExtAddr;      /* W:  local externded address */
  uint16            localShortAddr;    /* W:  local short address */
  uint16            localPanID;        /* W:  local PAN ID */
  uint8             reserved1[3];      /* X:  Reserved byte 52..54 */
  uint8             endTrigger;        /* W:  end trigger for RX operation */
  uint32            endTime;           /* W:  time for end trigger */
} rxCmd_t;

/* IEEE 802.15.4 Energy Detect Scan Command */
PACKED_TYPEDEF_STRUCT
{
  rfOpCmd_t         rfOpCmd;           /* radio command common structure */
  uint8             channel;           /* W:  channel to tune to */
  ccaOpt_t          ccaOpt;            /* RW: CCA options */
  uint8             ccaRssiThreshold;  /* RW: RSSI threshold for CCA */
  uint8             reserved;          /* X:  Reserved */
  uint8             maxRssi;           /* R:  max RSSI recorded during ED scan */
  uint8             endTrigger;        /* W:  end trigger for RX operation */
  uint32            endTime;           /* W:  time for end trigger */
} edScanCmd_t;

/* edCmd and rxCmd share the same memory */
PACKED_TYPEDEF_UNION
{
  rxCmd_t      rxCmd;  /* Receive Command */
  edScanCmd_t  edCmd;  /* Energy Detection Command */
} rxEdScan_t;

/* IEEE 802.15.4 CSMA-CA Command */
PACKED_TYPEDEF_STRUCT
{
  rfOpCmd_t         rfOpCmd;           /* radio command common structure */
  uint16            randomState;       /* RW: pseudo-random generator state */
  uint8             macMaxBE;          /* W:  macMaxEB parameter */
  uint8             macMaxCSMABackoffs;/* W:  macMaxCSMABackoffs parameter */
  csmaConfig_t      csmaConfig;        /* W:  CSMA configuration */
  uint8             NB;                /* RW: NB parameter */
  uint8             BE;                /* RW: BE parameter */
  uint8             remPeriods;        /* RW: remaining periods from a paused backoff countdown */
  uint8             lastRssi;          /* R:  RSSI measured at last CCA operation */
  uint8             endTrigger;        /* W:  end trigger for CSMA-CA operation */
  uint32            lastTimeStamp;     /* R:  time of last CCA operation */
  uint32            endTime;           /* W:  time for end trigger */
} csmaCaCmd_t;

/* FG Pttern Check Command */
typedef rfOpCmd_patternCk_t FGPatternCheckCmd_t;

/* IEEE 802.15.4 Tx Command */
PACKED_TYPEDEF_STRUCT
{
  rfOpCmd_t         rfOpCmd;           /* radio command common structure */
  txOption_t        txOption;          /* W:  Tx option */
  uint8             payloadLen;        /* W:  payload length */
  uint8            *pPayload;          /* W:  ptr to payload buffer */
  uint32            timeStamp;         /* R:  time stamp of the transmitted frame */
} txCmd_t;

/* IEEE 802.15.4 Receive ACK Command */
PACKED_TYPEDEF_STRUCT
{
  rfOpCmd_t         rfOpCmd;           /* radio command common structure */
  uint8             seqNum;            /* W:  sequence number expected */
  uint8             endTrigger;        /* W:  end trigger for giving up ACK recption */
  uint32            endTime;           /* W:  time for end trigger */
} rxAckCmd_t;

/* IEEE 802.15.4 Update Radio Settings Command */
typedef rfOpCmd_RadioSetup_t updateRadioSettingsCmd_t;

/*
** MAC Radio Immediate Commands
*/

/* Modify CCA Command */
PACKED_TYPEDEF_STRUCT
{
  uint16            cmdNum;            /* W:  radio command number */
  ccaOpt_t          newCcaOpt;         /* W:  new CCA option for the running background operation */
  uint8             newCcaRssiThr;     /* W:  new CCA RSSI threshold */
} macModifyCcaCmd_t;

/* Modify Frame Filtering Command */
PACKED_TYPEDEF_STRUCT
{
  uint16            cmdNum;            /* W:  radio command number */
  frmFilterOpt_t    newFrmFiltOpt;     /* W:  new frame filtering option for the running background operation */
  uint8             newFrmTypes;       /* W:  new frame types */
} macModifyFrmFiltCmd_t;

/* Enable Disable Source Matching Entry Command */
PACKED_TYPEDEF_STRUCT
{
  uint16            cmdNum;            /* W:  radio command number */
  srcMatchOpt_t     srcMatchOpt;       /* W:  source matching options */
  uint8             entryNo;           /* W:  Index of entry to enable or disable */
} srcMatchCmd_t;

/* Request CCA State Command */
PACKED_TYPEDEF_STRUCT
{
  uint16            cmdNum;            /* W:  radio command number */
  uint8             currRssi;          /* R:  current RSSI */
  uint8             maxRssi;           /* R:  maxmum RSSI on the channel since Rx starts */
  ccaInfo_t         ccaInfo;           /* R:  CCA info */
} requestCcaStateCmd_t;

/*******************************************************************************
 * LOCAL VARIABLES
 */

/*******************************************************************************
 * GLOBAL VARIABLES
 */

/* CSMA/CA Connand */
extern csmaCaCmd_t macCsmaCaCmd;

/* Transmit Command */
extern txCmd_t macTxCmd;

/* Receive ACK Command */
extern rxAckCmd_t macRxAckCmd;

/* Receive and Energy Detect Scan commands share the same memory */
extern rxEdScan_t macRxEdScan;

/* MAC RAT channel A */
extern uint8 macRatChanA;

/* MAC RAT channel B */
extern uint8 macRatChanB;

/* MAC Reveive output buffer */
extern output_t macRxOutput;

/* MAC RF front end configuration. Use a pointer to void since
 * the format changes from package to package.
 */
extern void *macRadioConfig;

/*******************************************************************************
 * APIs
 */
extern void macSetupRadio( void );
extern void macSetCcaRssiThreshold( uint8 ccaRssiThreshold );
extern void macSetupReceiveCmd( void );
extern void macSendReceiveCmd( void );
extern void macSendEdCmd( void );
extern void macSetupCsmaCaTxCmd( bool );
extern void macSetupSlottedTxCmd( void );
extern void macSetupGreenPowerTxCmd( void );
extern void macSetupRxAckCmd( void );
extern void macSetupEdCmd( void );
extern void macSetupRxAckSeqNo( uint8 seqNo );
extern void macStopCmd( void );
extern void macPowerDownCmd( void );
extern uint32 macStopRAT( void );
extern void macSyncStartRAT( uint32 ratval );
extern void macRxQueueFlushCmd ( void );
extern void macWriteRfRegCmd( uint16 addr, uint32 value );
extern void macFreqSynthCmd( uint8 channel );
extern uint8 macGetCurrRssi( void );
extern void macSetupRfHal( void );
extern void macSetupRATChanCompare( uint8 ratChan, uint32 compareTime );
extern void macSetTxPowerVal( uint16 txPowerVal );
extern void macSetupMailbox( void );
extern void macRatChanCBack_A ( void );
extern void macRatChanCBack_B ( void );
extern void macSetUserConfig( void *arg );
extern void macCheckCommandDone(rfOpCmd_t *rfOpCmd);

/*******************************************************************************
 */

#endif /* MAC_H */
