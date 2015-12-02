/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// flash.c - Driver for the flash Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup flash_api
//! @{
//
//*****************************************************************************

#include "inc/hw_regaccess.h"
#include "inc/hw_memmap.h"

#ifdef DRIVERLIB_LEGACY_MODE

#ifdef __MSP430_HAS_FLASH__
#include "flash.h"

#include <assert.h>

//*****************************************************************************
//
//! \brief Erase a single segment of the flash memory.
//!
//! For devices like MSP430i204x, if the specified segment is the information
//! flash segment, the FLASH_unlockInfo API must be called prior to calling
//! this API.
//!
//! \param baseAddress is the base address of the FLASH module.
//! \param flash_ptr is the pointer into the flash segment to be erased
//!
//! \return None
//
//*****************************************************************************
void FLASH_segmentErase (uint16_t baseAddress, uint8_t *flash_ptr)
{
    //Clear Lock bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY;

    //Set Erase bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY + ERASE;

    //Dummy write to erase Flash seg
    *flash_ptr = 0;

    //test busy
    while (HWREG8(baseAddress + OFS_FCTL3) & BUSY) ;

    //Clear ERASE bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY;

    //Set LOCK bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY + LOCK;
}

//*****************************************************************************
//
//! \brief Erase a single bank of the flash memory.
//!
//! This function erases a single bank of the flash memory.  This API will
//! erase the entire flash if device contains only one flash bank.
//!
//! \param baseAddress is the base address of the FLASH module.
//! \param flash_ptr is a pointer into the bank to be erased
//!
//! \return None
//
//*****************************************************************************
void FLASH_bankErase (uint16_t baseAddress, uint8_t *flash_ptr)
{
    //Clear Lock bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY;
    
    while (HWREG8(baseAddress + OFS_FCTL3) & BUSY) ;

    //Set MERAS bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY + MERAS;

    //Dummy write to erase Flash seg
    *flash_ptr = 0;

    //test busy
    while (HWREG8(baseAddress + OFS_FCTL3) & BUSY) ;

    //Clear MERAS bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY;

    //Set LOCK bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY + LOCK;
}

//*****************************************************************************
//
//! \brief Erase all flash memory.
//!
//! This function erases all the flash memory banks. For devices like
//! MSP430i204x, this API erases main memory and information flash memory if
//! the FLASH_unlockInfo API was previously executed (otherwise the information
//! flash is not erased). Also note that erasing information flash memory in
//! the MSP430i204x impacts the TLV calibration constants located at the
//! information memory.
//!
//! \param baseAddress is the base address of the FLASH module.
//! \param flash_ptr is a pointer into the bank to be erased
//!
//! \return None
//
//*****************************************************************************
void FLASH_massErase (uint16_t baseAddress, uint8_t *flash_ptr)
{
    //Clear Lock bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY;
    
    while (HWREG8(baseAddress + OFS_FCTL3) & BUSY) ;

    //Set MERAS bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY + MERAS + ERASE;

    //Dummy write to erase Flash seg
    *flash_ptr = 0;

    //test busy
    while (HWREG8(baseAddress + OFS_FCTL3) & BUSY) ;

    //Clear MERAS bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY;

    //Set LOCK bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY + LOCK;
}

//*****************************************************************************
//
//! \brief Erase check of the flash memory
//!
//! This function checks bytes in flash memory to make sure that they are in an
//! erased state (are set to 0xFF).
//!
//! \param baseAddress is the base address of the FLASH module.
//! \param flash_ptr is the pointer to the starting location of the erase check
//! \param numberOfBytes is the number of bytes to be checked
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************
bool FLASH_eraseCheck (uint16_t baseAddress,
    uint8_t *flash_ptr,
    uint16_t numberOfBytes
    )
{
    uint16_t i;

    for (i = 0; i < numberOfBytes; i++)
    {
        //was erasing successfull?
        if ((*(flash_ptr + i)) != 0xFF){
            return ( STATUS_FAIL) ;
        }
    }
    return ( STATUS_SUCCESS) ;
}

//*****************************************************************************
//
//! \brief Write data into the flash memory in byte format, pass by reference
//!
//! This function writes a byte array of size count into flash memory. Assumes
//! the flash memory is already erased and unlocked. FLASH_segmentErase can be
//! used to erase a segment.
//!
//! \param baseAddress is the base address of the FLASH module.
//! \param data_ptr is the pointer to the data to be written
//! \param flash_ptr is the pointer into which to write the data
//! \param count number of times to write the value
//!
//! \return None
//
//*****************************************************************************
void FLASH_write8 (uint16_t baseAddress,
    uint8_t *data_ptr,
    uint8_t *flash_ptr,
    uint16_t count
    )
{
    //Clear Lock bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY;

    //Enable byte/word write mode
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY + WRT;

    while (count > 0)
    {
        //test busy
        while (HWREG8(baseAddress + OFS_FCTL3) & BUSY) ;

        //Write to Flash
        *flash_ptr++ = *data_ptr++;
        count--;
    }

    //Clear WRT bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY;

    //Set LOCK bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY + LOCK;
}

//*****************************************************************************
//
//! \brief Write data into the flash memory in 16-bit word format, pass by
//! reference
//!
//! This function writes a 16-bit word array of size count into flash memory.
//! Assumes the flash memory is already erased and unlocked. FLASH_segmentErase
//! can be used to erase a segment.
//!
//! \param baseAddress is the base address of the FLASH module.
//! \param data_ptr is the pointer to the data to be written
//! \param flash_ptr is the pointer into which to write the data
//! \param count number of times to write the value
//!
//! \return None
//
//*****************************************************************************
void FLASH_write16 (uint16_t baseAddress,
    uint16_t *data_ptr,
    uint16_t *flash_ptr,
    uint16_t count
    )
{
    //Clear Lock bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY;

    //Enable byte/word write mode
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY + WRT;

    while (count > 0)
    {
        //test busy
        while (HWREG8(baseAddress + OFS_FCTL3) & BUSY) ;

        //Write to Flash
        *flash_ptr++ = *data_ptr++;
        count--;
    }

    //Clear WRT bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY;

    //Set LOCK bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY + LOCK;
}

//*****************************************************************************
//
//! \brief Write data into the flash memory in 32-bit word format, pass by
//! reference
//!
//! This function writes a 32-bit array of size count into flash memory.
//! Assumes the flash memory is already erased and unlocked. FLASH_segmentErase
//! can be used to erase a segment.
//!
//! \param baseAddress is the base address of the FLASH module.
//! \param data_ptr is the pointer to the data to be written
//! \param flash_ptr is the pointer into which to write the data
//! \param count number of times to write the value
//!
//! \return None
//
//*****************************************************************************
void FLASH_write32 (uint16_t baseAddress,
    uint32_t *data_ptr,
    uint32_t *flash_ptr,
    uint16_t count
    )
{
    //Clear Lock bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY;

    //Enable long-word write
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY + BLKWRT;

    while (count > 0)
    {
        //test busy
        while (HWREG8(baseAddress + OFS_FCTL3) & BUSY) ;

        //Write to Flash
        *flash_ptr++ = *data_ptr++;

        count--;
    }

    //Clear BLKWRT bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY;

    //Set LOCK bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY + LOCK;
}

//*****************************************************************************
//
//! \brief Write data into the flash memory in 32-bit word format, pass by
//! value
//!
//! This function writes a 32-bit data value into flash memory, count times.
//! Assumes the flash memory is already erased and unlocked. FLASH_segmentErase
//! can be used to erase a segment.
//!
//! \param baseAddress is the base address of the FLASH module.
//! \param value value to fill memory with
//! \param flash_ptr is the pointer into which to write the data
//! \param count number of times to write the value
//!
//! \return None
//
//*****************************************************************************
void FLASH_memoryFill32 (uint16_t baseAddress,
    uint32_t value,
    uint32_t *flash_ptr,
    uint16_t count
    )
{
    //Clear Lock bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY;

    //Enable long-word write
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY + BLKWRT;

    //test busy
    while (count > 0)
    {
        while ((HWREG8(baseAddress + OFS_FCTL3)) & BUSY) ;

        //Write to Flash
        *flash_ptr++ = value;

        count--;
    }

    //Clear BLKWRT bit
    HWREG16(baseAddress + OFS_FCTL1) = FWKEY;

    //Set LOCK bit
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY + LOCK;
}

//*****************************************************************************
//
//! \brief Check FLASH status to see if it is currently busy erasing or
//! programming
//!
//! This function checks the status register to determine if the flash memory
//! is ready for writing.
//!
//! \param baseAddress is the base address of the FLASH module.
//! \param mask FLASH status to read
//!        Mask value is the logical OR of any of the following:
//!        - \b FLASH_READY_FOR_NEXT_WRITE
//!        - \b FLASH_ACCESS_VIOLATION_INTERRUPT_FLAG
//!        - \b FLASH_PASSWORD_WRITTEN_INCORRECTLY
//!        - \b FLASH_BUSY
//!
//! \return Logical OR of any of the following:
//!         - \b FLASH_READY_FOR_NEXT_WRITE
//!         - \b FLASH_ACCESS_VIOLATION_INTERRUPT_FLAG
//!         - \b FLASH_PASSWORD_WRITTEN_INCORRECTLY
//!         - \b FLASH_BUSY
//!         \n indicating the status of the FLASH
//
//*****************************************************************************
uint8_t FLASH_status (uint16_t baseAddress,
    uint8_t mask
    )
{
    return ((HWREG8(baseAddress + OFS_FCTL3) & mask ));
}

//*****************************************************************************
//
//! \brief Locks the information flash memory segment A
//!
//! This function is typically called after an erase or write operation on the
//! information flash segment is performed by any of the other API functions in
//! order to re-lock the information flash segment.
//!
//! \param baseAddress is the base address of the FLASH module.
//!
//! \return None
//
//*****************************************************************************
void FLASH_lockInfoA (uint16_t baseAddress)
{
    //Disable global interrupts while doing RMW operation on LOCKA bit
    uint16_t gieStatus;
    gieStatus = __get_SR_register() & GIE;          //Store current SR register
    __disable_interrupt();                          //Disable global interrupt

    //Set the LOCKA bit in FCTL3.
    //Since LOCKA toggles when you write a 1 (and writing 0 has no effect),
    //read the register, XOR with LOCKA mask, mask the lower byte
    //and write it back.
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY
        + ((HWREG16(baseAddress + OFS_FCTL3) ^ LOCKA) & 0xFF);

    //Reinstate SR register to restore global interrupt enable status
    __bis_SR_register(gieStatus);
}

//*****************************************************************************
//
//! \brief Unlocks the information flash memory segment A
//!
//! This function must be called before an erase or write operation on the
//! information flash segment is performed by any of the other API functions.
//!
//! \param baseAddress is the base address of the FLASH module.
//!
//! \return None
//
//*****************************************************************************
void FLASH_unlockInfoA (uint16_t baseAddress)
{
    //Disable global interrupts while doing RMW operation on LOCKA bit
    uint16_t gieStatus;
    gieStatus = __get_SR_register() & GIE;          //Store current SR register
    __disable_interrupt();                          //Disable global interrupt

    //Clear the LOCKA bit in FCTL3.
    //Since LOCKA toggles when you write a 1 (and writing 0 has no effect),
    //read the register, mask the lower byte, and write it back.
    HWREG16(baseAddress + OFS_FCTL3) = FWKEY 
        + (HWREG16(baseAddress + OFS_FCTL3) & 0xFF);

    //Reinstate SR register to restore global interrupt enable status
    __bis_SR_register(gieStatus);
}


#endif
#endif
//*****************************************************************************
//
//! Close the doxygen group for flash_api
//! @}
//
//*****************************************************************************
