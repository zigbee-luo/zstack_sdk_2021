/******************************************************************************
  Filename:       ota_signature.c
  Revised:        $Date: 2014-05-15 12:54:26 -0700 (Thu, 15 May 2014) $
  Revision:       $Revision: 38555 $

  Description:    This file contains code to calculate and verify OTA
                  signatures based on teh MMO AES Hash function.


  Copyright 2010-2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
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
******************************************************************************/

#include "hal_types.h"
#include "ota_common.h"
#include "ota_signature.h"
#include "eccapi.h"

#ifdef _WIN32
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define memset  memset
#define OsalPort_memcpy  OsalPort_memcpy
#define strlen  strlen
#include "aes.h"
#define ssp_HW_KeyInit(a)
#else
#include "rom_jt_154.h"
#include "hal_aes.h"
#include "ssp_hash.h"
#include "zglobals.h"
#include "zcl.h"
#include "osal_nv.h"
#endif

static void OTA_AesHashBlock(uint8_t *pHash, uint8_t *pData);
static void OTA_XorBlock(uint8_t *pHash, uint8_t *pData);

/******************************************************************************
 * @fn      OTA_AesHashBlock
 *
 * @brief   This function performs the AES MMO Hash on a block of data
 *
 * @param   pHash - Pointer to hash
 *          pData - pointer to data
 *
 * @return  none
 */
void OTA_AesHashBlock(uint8_t *pHash, uint8_t *pData)
{
  uint8_t key[OTA_MMO_HASH_SIZE];

  OsalPort_memcpy(key, pHash, OTA_MMO_HASH_SIZE);
  OsalPort_memcpy(pHash, pData, OTA_MMO_HASH_SIZE);
  ssp_HW_KeyInit(key);
  sspAesEncryptHW(key, pHash);
  OTA_XorBlock(pHash, pData);
}

/******************************************************************************
 * @fn      OTA_XorBlock
 *
 * @brief   This function exclusive ORs a block of hash and data and puts the
 *          result into the hash.
 *
 * @param   pHash - Pointer to hash
 *          pData - pointer to data
 *
 * @return  none
 */
void OTA_XorBlock(uint8_t *pHash, uint8_t *pData)
{
  uint8_t i;

  for (i=0; i < OTA_MMO_HASH_SIZE; i++)
  {
    pHash[i] ^= pData[i];
  }
}

/******************************************************************************
 * @fn      OTA_CalculateMmoR3
 *
 * @brief   This function calcualtes a MMO (revision 3) Hash of an OTA Image
 *          The hash must cover the entire image, but the data is received in
 *          smaller blocks.  State information about the hash is passed into
 *          this function with each block of data.
 *
 * @param   pCtrl - The control structure to calculate the MMO AES Hash
 *          pData - A block of data (must be OTA_MMO_HASH_SIZE bytes except for last block)
 *          len - The length of pData (ignored except when lastBlock = TRUE)
 *          lastBlock - Indicates this is the last block of data to be hashed
 *
 * @return  none
 */
void OTA_CalculateMmoR3(OTA_MmoCtrl_t *pCtrl, uint8_t *pData, uint8_t len, uint8_t lastBlock)
{
  if (lastBlock)
  {
    uint32_t m = (pCtrl->length + len) << 3;
    uint8_t ending[OTA_MMO_HASH_SIZE];

    memset(ending, 0, OTA_MMO_HASH_SIZE);

    if ( len >= OTA_MMO_HASH_SIZE )
    {
      len = OTA_MMO_HASH_SIZE - 1;
    }
    if (len)
    {
      OsalPort_memcpy(ending, pData, len);
    }
    ending[len] = 0x80;

    // Different endings are required depending on total length
    if (m < 0x00010000)
    {
      if(len > 13)
      {
        OTA_AesHashBlock(pCtrl->hash, ending);
        memset(ending, 0, OTA_MMO_HASH_SIZE);
      }

      ending[14] = (uint8_t)((m >> 8) &0xFF);
      ending[15] = (uint8_t)(m & 0xFF);

      OTA_AesHashBlock(pCtrl->hash, ending);
    }
    else
    {
      if (len > 9)
      {
        OTA_AesHashBlock(pCtrl->hash, ending);
        memset(ending, 0, OTA_MMO_HASH_SIZE);
      }

      ending[10] = (uint8_t)((m >> 24) & 0xFF);
      ending[11] = (uint8_t)((m >> 16) & 0xFF);
      ending[12] = (uint8_t)((m >> 8) & 0xFF);
      ending[13] = (uint8_t)(m & 0xFF);

      OTA_AesHashBlock(pCtrl->hash, ending);
    }
  }
  else
  {
    OTA_AesHashBlock(pCtrl->hash, pData);
    pCtrl->length += OTA_MMO_HASH_SIZE;
  }
}

/******************************************************************************
 * @fn      OTA_ValidateSignature
 *
 * @brief   This function validates an ECDSA Signature.
 *
 * @param   pHash - The digest created from the OTA Image
 *          pCert - The Signer Certificate
 *          pSig - The signature from the OTA Image
 *          pIEEE - The Signer IEEE
 *
 * @return  none
 */
uint8_t OTA_ValidateSignature(uint8_t *pHash, uint8_t* pCert, uint8_t *pSig, uint8_t *pIEEE)
{
  // silence compiler warnings
  pHash = pHash;
  pCert = pCert;
  pSig = pSig;
  pIEEE = pIEEE;

  return ZSuccess;
}
