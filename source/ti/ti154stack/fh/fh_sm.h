/******************************************************************************

 @file fh_sm.h

 @brief TIMAC 2.0 FH finite state machine API

 Group: WCS LPC
 Target Device: cc13x2_26x2

 ******************************************************************************
 
 Copyright (c) 2016-2021, Texas Instruments Incorporated
 All rights reserved.

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

 ******************************************************************************
 
 
 *****************************************************************************/

/******************************************************************************
 Includes
 *****************************************************************************/

#ifndef FH_SM_H
#define FH_SM_H

/* ------------------------------------------------------------------------------------------------
 *                                            Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "hal_types.h"

/* ------------------------------------------------------------------------------------------------
 *                                            Defines
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                           Typedefs
 * ------------------------------------------------------------------------------------------------
 */

typedef enum __fh_state
{
  FH_ST_HOP             = 0,
  FH_ST_TX              = 1,
  FH_ST_RX              = 2,
  FH_ST_ASYNC           = 3,
  FH_ST_EDFE            = 4,

  FH_ST_NUM_STATES
} FH_state_t;


typedef enum __fh_event
{

  FH_EVT_UC_TIMER       = 0,
  FH_EVT_BC_TIMER       = 1,
  FH_EVT_RX_START       = 2,
  FH_EVT_RX_DONE        = 3,
  FH_EVT_TX_START       = 4,
  FH_EVT_TX_DONE        = 5,
  FH_EVT_ASYNC_START    = 6,
  FH_EVT_ASYNC_DONE     = 7,
  FH_EVT_EDFE_START     = 8,
  FH_EVT_EDFE_DONE      = 9,

  FH_EVT_NUM_EVENTS
} FH_smEvent_t;

#define FHFSM_MAX_DEBUG_EVENTS				(32)

/* FH State Machine debugging */
typedef struct
{
    uint16_t state_evt[FHFSM_MAX_DEBUG_EVENTS];
    uint8_t  idx;
} FHFSM_debugBuffer_t;

/* State transition function */
typedef FHAPI_status (*FSM_eventActivation_t)(uint16_t currState_p, uint16_t event, void* data_p);

//typedef void (*FSM_action_t)(void);
typedef void (*FSM_action_t)(void* data_p);
/* Action cell */
typedef  struct __attribute__((__packed__))
{
  uint8_t                nextState;      /* next state */
  FSM_action_t          actionFunc_p;   /**< action function */
}FSM_actionCell_t;

/* Matrix type */
typedef FSM_actionCell_t*       FSM_matrix_t;

/* General FSM structure */
typedef struct
{
  const FSM_actionCell_t  *stateEventMatrix; /* State and Event matrix */
  uint8_t                  maxStates;        /* Max Number of states in the matrix */
  uint8_t                  maxEvents;        /* Max Number of events in the matrix */
  uint8_t                  previousState;    /* Previous state */
  uint8_t                  currentState;     /* Current state */
  FSM_eventActivation_t   transitionFunc_p; /* State transition function */

}FSM_stateMachine_t;

/* ------------------------------------------------------------------------------------------------
 *                                           Global Externs
 * ------------------------------------------------------------------------------------------------
 */

/* ------------------------------------------------------------------------------------------------
 *                                           Prototypes
 * ------------------------------------------------------------------------------------------------
 */
FHAPI_status FHSM_event(FSM_stateMachine_t *fsm_p,
                        uint16_t event, void *data_p);

FHAPI_status FSM_transaction_debug(uint16_t currState,
                                   uint16_t event, void *pdata);

#endif
