/**************************************************************************************************
  Filename:       gpd_memory.c
  Revised:        $Date: 2014-10-06 15:42:43 -0700 (Mon, 06 Oct 2014) $
  Revision:       $Revision: 40449 $

  Description:    Green Power device non volatile memory functions.


  Copyright 2005-2014 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
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
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "gpd_common.h"
#include "gpd_memory.h"
#include "gpd.h"
#include "api_mac.h"
#include "mac_api.h"
#include "zcomdef.h"
#include "nvintf.h"
#include "stdlib.h"
#include "string.h"
#include "rom_jt_154.h"


#define GP_NV_EX_LEGACY           0x0000

/*********************************************************************
 * MACROS
 */
/*

*/
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
#ifdef NV_RESTORE
static NVINTF_nvFuncts_t *nvGpFps;
#endif

#ifdef NV_RESTORE
/*******************************************************************************
 * @fn          gp_appNvInit
 *
 * @brief       Initialize the application
 *
 * @param       pfnNV - pointer to the NV functions
 *
 * @return      none
 */

void gp_appNvInit(NVINTF_nvFuncts_t *pfnNV)
{
  nvGpFps = pfnNV;
#if (SEQUENCE_NUMBER_CAP == TRUE)
  uint8_t secNum = 0;

  if(gp_nv_item_init(GP_NV_SEQUENCE_NUMBER, sizeof(uint8_t), &secNum) == SUCCESS)
  {
    gp_nv_read(GP_NV_SEQUENCE_NUMBER, 0, 1, &secNum);
  }
  ApiMac_mlmeSetReqUint8(ApiMac_attribute_dsn, secNum);
#endif
#if (GP_SECURITY_LEVEL == GP_SECURITY_LVL_4FC_4MIC) || (GP_SECURITY_LEVEL == GP_SECURITY_LVL_4FC_4MIC_ENCRYPT)
  uint32_t secFrameCounter = 0;
  if(gp_nv_item_init(GP_NV_SEC_FRAME_COUNTER, sizeof(secFrameCounter), &secFrameCounter) == SUCCESS)
  {
    gp_nv_read(GP_NV_SEC_FRAME_COUNTER, 0, sizeof(secFrameCounter), &secFrameCounter);
    gpd_setSecurityFrameCounter(secFrameCounter);
  }
#endif
}
#endif

/*********************************************************************
 * @fn      gp_memcpy
 *
 * @brief
 *
 *   Generic memory copy.
 *
 *   Note: This function differs from the standard OsalPort_memcpy(), since
 *         it returns the pointer to the next destination uint8_t. The
 *         standard OsalPort_memcpy() returns the original destination address.
 *
 * @param   dst - destination address
 * @param   src - source address
 * @param   len - number of bytes to copy
 *
 * @return  pointer to end of destination buffer
 */
void *gp_memcpy( void *dst,const void GENERIC *src, unsigned int len )
    {
      uint8_t *pDst;
      const uint8_t GENERIC *pSrc;

      pSrc = src;
      pDst = dst;

      while ( len-- )
        *pDst++ = *pSrc++;

      return ( pDst );
    }

#ifdef NV_RESTORE
/******************************************************************************
 * @fn      gp_nv_item_init
 *
 * @brief   If the NV item does not already exist, it is created and
 *          initialized with the data passed to the function, if any.
 *          This function must be called before calling gp_nv_read() or
 *          gp_nv_write().
 *
 * @param   id  - Valid NV item Id.
 * @param   len - Item length.
 * @param  *buf - Pointer to item initalization data. Set to NULL if none.
 *
 * @return  NV_ITEM_UNINIT - Id did not exist and was created successfully.
 *          SUCCESS       - Id already existed, no action taken.
 *          NV_OPER_FAILED - Failure to find or create Id.
 */
uint8_t gp_nv_item_init( uint16_t id, uint16_t len, void *buf )
{
    if( (*nvGpFps).createItem )
    {
        uint32_t nvLen = 0;
        NVINTF_itemID_t nvId;
        nvId.systemID = NVINTF_SYSID_ZSTACK;
        nvId.itemID = (uint16_t)GP_NV_EX_LEGACY;
        nvId.subID = (uint16_t)id;
        if( (*nvGpFps).getItemLen )
        {
            nvLen = (*nvGpFps).getItemLen( nvId );
        }

        if ( nvLen == len )
        {
          // Already exists and length is good
          return ( SUCCESS );
        }

        if ( (*nvGpFps).createItem( nvId, len, buf ) == NVINTF_FAILURE )
        {
          // Operation failed
          return ( NV_OPER_FAILED );
        }
    }
    // NV was created
    return ( NV_ITEM_UNINIT );
}

/******************************************************************************
 * @fn      gp_nv_read_ex
 *
 * @brief   Read data from NV. This function can be used to read an entire item from NV or
 *          an element of an item by indexing into the item with an offset.
 *          Read data is copied into *buf.
 *
 * @param   id     - Valid NV item Id.
 * @param   ndx - Index offset into item
 * @param   len    - Length of data to read.
 * @param   *buf  - Data is read into this buffer.
 *
 * @return  SUCCESS if NV data was copied to the parameter 'buf'.
 *          Otherwise, NV_OPER_FAILED for failure.
 */
uint8_t gp_nv_read( uint16_t id, uint16_t ndx, uint16_t len, void *buf )
{
  if ( (*nvGpFps).readItem )
  {
    NVINTF_itemID_t nvId;

    nvId.systemID = NVINTF_SYSID_ZSTACK;
    nvId.itemID = (uint16_t)GP_NV_EX_LEGACY;
    nvId.subID = (uint16_t)id;
    if ( (*nvGpFps).readItem( nvId, ndx, len, buf ) == NVINTF_FAILURE )
    {
      return ( 0x0A );
    }
    else
    {
      return ( SUCCESS );
    }
  }
 else
  {
    return (NV_OPER_FAILED);
  }

}

/******************************************************************************
 * @fn      gp_nv_write
 *
 * @brief   Write a data item to NV. Function can write an entire item to NV or
 *          an element of an item by indexing into the item with an offset.
 *
 * @param   id  - Valid NV item Id.
 * @param   len - Length of data to write.
 * @param  *buf - Data to write.
 *
 * @return  SUCCESS if successful, NV_ITEM_UNINIT if item did not
 *          exist in NV and offset is non-zero, NV_OPER_FAILED if failure.
 */
uint8_t gp_nv_write( uint16_t id, uint16_t len, void *buf )
{
  uint8_t rtrn = SUCCESS;

  if ( (*nvGpFps).writeItem )
  {
    uint32_t nvLen = 0;
    NVINTF_itemID_t nvId;

    nvId.systemID = NVINTF_SYSID_ZSTACK;
    nvId.itemID = (uint16_t)GP_NV_EX_LEGACY;//ZCD_NV_EX_LEGACY;
    nvId.subID = (uint16_t)id;

    if ( (*nvGpFps).getItemLen )
    {
      nvLen = (*nvGpFps).getItemLen( nvId );
    }

    if ( nvLen > 0 )
    {
      if ( (*nvGpFps).writeItem( nvId, len, buf ) == NVINTF_FAILURE )
      {
        rtrn = NV_OPER_FAILED;
      }
    }
    else
    {
      rtrn = NV_ITEM_UNINIT;
    }
  }

  return rtrn;
}
/******************************************************************************
 * @fn      gp_nv_delete
 *
 * @brief   Delete item from NV. This function will fail if the length
 *          parameter does not match the length of the item in NV.
 *
 * @param   id  - Valid NV item Id.
 * @param   len - Length of item to delete.
 *
 * @return  SUCCESS if item was deleted,
 *          NV_ITEM_UNINIT if item did not exist in NV,
 *          NV_BAD_ITEM_LEN if length parameter not correct,
 *          NV_OPER_FAILED if attempted deletion failed.
 */
uint8_t gp_nv_delete( uint16_t id, uint16_t len )
{
  uint8_t ret = SUCCESS;

  if ( (*nvGpFps).deleteItem )
  {
    uint32_t nvLen = 0;
    NVINTF_itemID_t nvId;

    nvId.systemID = NVINTF_SYSID_ZSTACK;
    nvId.itemID = (uint16_t)GP_NV_EX_LEGACY;
    nvId.subID = (uint16_t)id;
    if ( (*nvGpFps).getItemLen )
    {
      nvLen = (*nvGpFps).getItemLen( nvId );
    }

    if ( nvLen == 0 )
    {
      ret = NV_ITEM_UNINIT;
    }
    else if ( nvLen != len )
    {
      ret = NV_BAD_ITEM_LEN;
    }
    else if ( (*nvGpFps).deleteItem( nvId ) == NVINTF_FAILURE )
    {
      ret = NV_OPER_FAILED;
    }
  }

  return ( ret );
}
/******************************************************************************
 * @fn      gp_nv_item_len
 *
 * @brief   Get the data length of the item stored in NV memory.
 *
 * @param   id  - Valid NV item Id.
 *
 * @return  Item length, if found; zero otherwise.
 */
uint16_t gp_nv_item_len(  uint16_t subId )
{
  uint16_t nvLen = 0;

  if ( (*nvGpFps).getItemLen )
  {
    NVINTF_itemID_t nvId;

    nvId.systemID = NVINTF_SYSID_ZSTACK;
    nvId.itemID = (uint16_t)GP_NV_EX_LEGACY;
    nvId.subID = (uint16_t)subId;

    nvLen = (*nvGpFps).getItemLen( nvId );
  }
  return ( nvLen);
}
#endif
