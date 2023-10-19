/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_FLASH.h
 *
 * \brief IO Driver functions for handling the external flash
 *
 * The flash driver allows reading from, writing to and erasing the external flash chip.
 *
 * \section flash_blocks Blocks in the Flash Chip
 *
 * The flash chip can be written to and read from any flash offset, but it can only be erased in
 * blocks.
 *
 * The block dimensions are fixed and given by the flash chip. They can vary between Flash chips
 * or even within the same flash chip.
 *
 * \subsection flash_blocks_hw3 Product Version 1.03
 *
 * The blocks have uneven sizes depending on the offset (internal address in the flash memory):
 *  * Offsets from 0x000000 to 0x00FFFF (the first 8 blocks): Block size is 0x2000 (8 KiB).
 *  * Offsets from 0x010000 to 0x7F0000 (remaining blocks): Block size is 0x10000 (64 KiB).
 *
 * The whole memory is partitioned as follows:
 *
 * \verbatim[table]
 * =================== ============== ==============
 * Beginning Offset     End Offset     Block Size
 * ------------------- -------------- --------------
 * 0x000000             0x001FFF       0x2000
 * ------------------- -------------- --------------
 * 0x002000             0x003FFF       0x2000
 * ------------------- -------------- --------------
 * ...                  ...            ...
 * ------------------- -------------- --------------
 * 0x00E000             0x00FFFF       0x2000
 * ------------------- -------------- --------------
 * 0x010000             0x01FFFF       0x10000
 * ------------------- -------------- --------------
 * 0x020000             0x02FFFF       0x10000
 * ------------------- -------------- --------------
 * ...                  ...            ...
 * ------------------- -------------- --------------
 * 0x7F0000             0x7FFFFF       0x10000
 * ------------------- -------------- --------------
 * \endverbatim
 *
 * This product version does not support multiple flash banks.
 *
 * \subsection flash_blocks_hw6 Product Version 1.08
 *
 * The block size is uniform for the whole memory: 0x20000 (128 KiB).
 *
 * \section flash_banks Flash Banks
 *
 * Product variants with flash memory larger than #IO_FLASH_BYTE_SIZE have the flash memory divided into banks.
 * Flash operations than depend on \c offset are limited to the selected flash bank.
 * The \c offset parameter is therefore the offset within the selected flash bank.
 * The internal address in the flash memory is calculated as
 * <tt>address = bank * IO_FLASH_BYTE_SIZE + offset</tt>.
 *
 * The selected flash bank also determines the part of the flash memory
 * that is visible directly in the CPU address space (starting at address 0x64000000).
 *
 * This is also the reason that the MPU protection, if enabled, acts on the currently selected flash bank 
 * and is not synchronized with the bank switching. If such synchronization is required 
 * (i.e. if different flash banks use different MPU protection), it has to be done manually
 * by enabling and disabling the User MPU region together with switching the flash bank.
 *
 * Product variants with flash memory size equal to #IO_FLASH_BYTE_SIZE do not require bank switching.
 * All the bank-dependent functions will act as if bank 0 was selected.
 *
 * \section flash_speed Speed of Flash Operations
 *
 * The communication with the flash chip is handled in a cyclic manner in the background, because
 * flash operations take multiple cycles. Exactly one cycle is executed in every millisecond. Every
 * user operation is triggered by calling the respective function. A status function provides
 * information about the result of the last operation.
 *
 *  \subsection flash_speed_read Speed of a read operation
 *
 *  In every cycle at most 256 bytes will be copied from the external flash to the internal memory.
 *  The number of cycles required for the whole read operation to complete is <tt>ceil
 *  (length / 256)</tt>.
 *
 *  It means the read operation will take more than one cycle to be completed if \c length is
 *  greater than 256, and is most effective when \c length is a multiple of 256 bytes.
 *
 *  \subsection flash_speed_write Speed of a write operation
 *
 *  A write operation is handled in two parts: first it ensures the destination area is blank,
 *  and then copies the data to the flash area. Blank check is done 256 bytes per cycle, so it
 *  takes <tt>ceil(length / 256)</tt> cycles to  check \c length bytes of data.
 *
 *  In every cycle at most 32 bytes will be copied from the internal memory to the external flash.
 *  If the area in the flash crosses a 32-byte boundary, the operation takes one cycle more. Thus
 *  the number of cycles required for the whole write operation to complete is maximum <tt>ceil
 *  (length / 256) + ceil(length / 32) + 2</tt>.
 *
 *  It means a write operation will always take more than one cycle to be completed.
 *  Writing to flash is most efficient when \c length is a multiple of 256 bytes,
 *  and when the range does not cross a 32-byte boundary, for example:
 *   * for \c length = 1 it will take 3 cycles,
 *   * for \c length = 2, \c offset = 31 it will take 4 cycles (range crosses a 32-byte boundary),
 *   * for \c length = 32, \c offset = 0 it will take 3 cycles,
 *   * for \c length = 256, \c offset = 1024 it will take 10 cycles.
 *   * for \c length = 256, \c offset = 1025 it will take 11 cycles (range crosses a 32-byte
 *     boundary).
 *   * for \c length = 257, \c offset = 1024 it will take 12 cycles (range crosses a 32-byte
 *     boundary and \c length > 256).
 *
 * \section flash_examples Flash Code Examples
 * \brief Examples for using the flash API
 *
 * \subsection flash_init_example Example for flash initialization
 *
 * The external flash can be initialized only after \c IO_Driver_Init(). The function
 * \c IO_FLASH_Init() needs to be called before any other flash API function.
 *
 * \code
 *      IO_ErrorType local_ret;
 *
 *      // Initialize HY-TTC 500 driver
 *      local_ret = IO_Driver_Init(NULL);
 *
 *      if (local_ret == IO_E_OK)
 *      {
 *          // Initialize Flash driver
 *          local_ret = IO_FLASH_Init();
 *      }
 *
 *      if (local_ret == IO_E_OK)
 *      {
 *          // Flash driver is initialized successfully, act accordingly
 *      }
 *      else
 *      {
 *          // HY-TTC 500 driver or flash driver was not initialized successfully,
 *          // act accordingly
 *      }
 * \endcode
 *
 * \subsection flash_status_example Example for checking the status of the driver
 *
 * After starting a read, write or erase operation, the flash driver is busy until that operation
 * completes.
 *
 * The status of the driver can be checked by calling periodically the \c IO_FLASH_GetStatus()
 * function. If the driver is already initialized, it returns if the driver is busy (i.e. the last
 * operation is being performed) or idle. If the driver is idle (i.e. the last operation has
 * ended), the return value indicates whether the last operation succeeded, or if not, what the
 * error was.
 *
 * Non-blocking flash application example:
 *
 * \code
 *     IO_RTC_StartTime(&timestamp);
 *
 *     while (1)
 *     {
 *         // task begins
 *         IO_Driver_TaskBegin();
 *
 *         // the IO_FLASH_Read() function is called only on the first cycle
 *         if (CycleCounter == 0)
 *         {
 *             io_error = IO_FLASH_Read(offset, length, dataRead);
 *             printf("IO_FLASH_Read() error code: %d\r\n", io_error);
 *             if (IO_E_OK != io_error)
 *             {
 *                 ProcessEnd = TRUE;
 *             }
 *         }
 *
 *         io_error = IO_FLASH_GetStatus();
 *
 *         if (io_error == IO_E_BUSY)
 *         {
 *             // do nothing, chip is busy
 *         }
 *         else if (io_error == IO_E_OK)
 *         {
 *             ProcessEnd = TRUE;
 *             printf("read finished successfully\r\n");
 *         }
 *         else
 *         {
 *             ProcessEnd = TRUE;
 *             printf("read finished with error code: %d\r\n", io_error);
 *         }
 *
 *         // task ends
 *         IO_Driver_TaskEnd();
 *
 *         CycleCounter += 1;
 *
 *         while (IO_RTC_GetTimeUS(timestamp) < 10000 );   // wait until 10ms have passed
 *         timestamp += 10000;                             // increase time stamp by cycle time
 *     }
 * \endcode
 *
 * Blocking flash application example:
 *
 * \code
 *     // starting chip erase
 *     io_error = IO_FLASH_BlockErase(blockOffset_u4);
 *
 *     if (io_error != IO_E_OK)
 *     {
 *         / an error occurred when trying to erase the block
 *         printf("IO_FLASH_BlockErase returned %d\r\n", io_error);
 *     }
 *
 *     // wait for flash driver to become ready again
 *     do
 *     {
 *         io_error = IO_FLASH_GetStatus();
 *     } while (io_error == IO_E_BUSY);
 *
 *     // check last status value
 *     if (io_error == IO_E_OK)
 *     {
 *         // block was erased successfully
 *         printf("Block was erased successfully\r\n");
 *     }
 *     else
 *     {
 *         // an error occurred when trying to erase the block
 *         printf("Block erase finished with error code: %d\r\n", io_error);
 *     }
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_FLASH_H_
#define IO_FLASH_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *
 * I N C L U D E S
 *
 *************************************************************************************************/

#include "IO_Driver.h"

/**************************************************************************************************
 *
 * D E F I N I T I O N S
 *
 *************************************************************************************************/
/**
 * Defines the size of the flash bank. \n
 * (8 MiB = 8,388,608 bytes)
 */
#define IO_FLASH_BYTE_SIZE          0x800000U

/* Defined in order to maintain backwards compatibility */
/**
 * \brief The Macro maintains backwards compatibility for applications using \c IO_FLASH_SetBusy()
 *  from I/O Driver 3.1.
 */
#define IO_FLASH_SetBusy            IO_FLASH_Suspend

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Initializes the flash driver
 *
 * For code examples see the section \ref flash_init_example "Example for flash initialization".
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine
 * \retval #IO_E_FLASH_WRONG_DEVICE_ID  The flash chip did not return the expected device ID
 * \retval #IO_E_INVALID_CHANNEL_ID     The flash module is not available on the used ECU variant
 * \retval #IO_E_CHANNEL_BUSY           Module has been initialized before
 * \retval #IO_E_DRIVER_NOT_INITIALIZED The common driver init function has not been called before
 * \retval #IO_E_OPTION_NOT_SUPPORTED   The type of the flash chip is not supported
 * \retval #IO_E_INTERNAL_CSM           Internal error
 * \retval #IO_E_BUSY                   Driver is busy, last operation is still ongoing
 * \retval #IO_E_FLASH_OP_FAILED        Driver is idle, the last operation has failed
 *
 **************************************************************************************************
 *
 * \remarks
 *    - If the flash driver needs to be re-initialized, the function \c IO_FLASH_DeInit() has to be called first.
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_Init(void);

/**********************************************************************************************//**
 *
 * \brief Deinitializes the flash driver
 *
 * This puts the external flash chip into the hardware reset state.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the module is not initialized
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_DeInit(void);

/**********************************************************************************************//**
 *
 * \brief Reads data from the external flash
 *
 *   Data is read from the offset specified by the \c offset parameter in currently selected flash bank and written
 *   to the memory location pointed to by the \c data parameter.
 *   The number of bytes to be read is denoted by the \c length parameter.
 *   The function only triggers a read operation, data is never returned instantly.
 *
 *   After calling \c IO_FLASH_Read(), the \c IO_FLASH_GetStatus() function should be called
 *   periodically to check if the read operation has finished.
 *
 * \param offset            Flash memory offset (0 .. (#IO_FLASH_BYTE_SIZE - 1))
 * \param length            Length of data to be read, in bytes (1 .. #IO_FLASH_BYTE_SIZE)
 * \param[out] data         Pointer to data in the internal memory.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Read operation started fine
 * \retval #IO_E_BUSY                   Last operation of flash module is still in progress
 * \retval #IO_E_INVALID_PARAMETER      Invalid address offset or length
 * \retval #IO_E_NULL_POINTER           Parameter \c data is a NULL pointer
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The flash module has not been initialized
 * \retval #IO_E_FLASH_SUSPENDED        The Flash is in a suspended state
 *
 **************************************************************************************************
 *
 * \note
 *    - <tt>(offset + length)</tt> must not be greater than #IO_FLASH_BYTE_SIZE.
 *
 * \note
 *    - The buffer passed via parameter \c data must be valid throughout the whole read
 *      procedure (e.g. not located on the stack).
 *
 * \note
 *    - A read operation to external flash must not be requested while the external flash chip
 *      is busy performing a write, block erase or chip erase operation.
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_Read(ubyte4 offset,
                           ubyte4 length,
                           ubyte1 * const data);

/**********************************************************************************************//**
 *
 * \brief Writes data to the external flash
 *
 *   Data is written from the memory location pointed to by the \c data parameter to the flash offset
 *   specified by the \c offset parameter in currently selected flash bank.
 *   The number of bytes to be written is denoted by the \c length parameter.
 *   The function only triggers a write operation, data is not written instantly.
 *
 *   Before the write operation begins, the destination flash memory range is checked to be in
 *   erased state. The write operation continues only if all bytes are found to be blank, i.e.
 *   their value is 0xFF.
 *
 *   After calling \c IO_FLASH_Write(), the \c IO_FLASH_GetStatus() function should be called
 *   periodically to check if the write operation has finished.
 *
 * \param offset            Flash memory offset (0 .. (#IO_FLASH_BYTE_SIZE - 1))
 * \param length            Length of data to be written, in bytes (1 .. #IO_FLASH_BYTE_SIZE)
 * \param[in] data          Pointer to data in the internal memory.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Write operation started fine
 * \retval #IO_E_BUSY                   Last operation of flash module is still in progress
 * \retval #IO_E_INVALID_PARAMETER      Invalid address offset or length
 * \retval #IO_E_NULL_POINTER           Parameter \c data is a NULL pointer
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The flash module has not been initialized
 * \retval #IO_E_FLASH_SUSPENDED        The Flash is in a suspended state
 *
 **************************************************************************************************
 *
 * \note
 *    - <tt>(offset + length)</tt> must not be greater than #IO_FLASH_BYTE_SIZE.
 *
 * \note
 *    - The buffer passed via parameter \c data must be valid throughout the whole write
 *      procedure (e.g. not located on the stack).
 *
 * \note
 *    - Constant data that has been linked to the external flash must not be erased. Otherwise
 *      the application CRC will be invalidated and thus the bootloader will not start the
 *      application anymore.
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_Write(ubyte4 offset,
                            ubyte4 length,
                            const ubyte1 * const data);

/**********************************************************************************************//**
 *
 * \brief Erases one block in the external flash
 *
 *   The flash memory block which begins at the offset specified by the \c offset parameter
 *   in currently selected flash bank will be erased.
 *   The function only triggers a block erase operation, it doesn't happen instantly.
 *
 *   The \c offset parameter must point exactly to the beginning of a block. For a description of
 *   the size of blocks please see the \ref flash_blocks "Blocks in the Flash Chip" section.
 *
 *   After calling \c IO_FLASH_BlockErase(), the \c IO_FLASH_GetStatus() function should be
 *   called periodically to check if the block erase operation has finished.
 *
 *   If a block erase operation is interrupted by a power-down or a call to \c IO_FLASH_DeInit(),
 *   the result is undefined.
 *
 * \param   offset          Flash memory offset (beginning of block)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Block erase operation started fine
 * \retval #IO_E_BUSY                   Last operation of flash module is still in progress
 * \retval #IO_E_INVALID_PARAMETER      Invalid block offset
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The flash module has not been initialized
 * \retval #IO_E_FLASH_SUSPENDED        The Flash is in a suspended state
 *
 **************************************************************************************************
 *
 * \note
 *    - Constant data that has been linked to the external flash must not be erased. Otherwise
 *      the application CRC will be invalidated and thus the bootloader will not start the
 *      application anymore.
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_BlockErase(ubyte4 offset);

/**********************************************************************************************//**
 *
 * \brief Erases the whole external flash chip
 *
 *   The whole external flash chip will be erased, regardless of the selected flash bank.
 *   This function only triggers a chip erase operation, it doesn't happen instantly.
 *
 *   After calling \c IO_FLASH_ChipErase(), the \c IO_FLASH_GetStatus() function should be
 *   called periodically to check if the chip erase operation has finished.
 *
 *   If a chip erase operation is interrupted by a power-down or a call to \c IO_FLASH_DeInit(),
 *   the result is undefined.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Chip erase operation started fine
 * \retval #IO_E_BUSY                   Last operation of flash module is still in progress
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The flash module has not been initialized
 * \retval #IO_E_FLASH_SUSPENDED        The Flash is in a suspended state
 *
 **************************************************************************************************
 *
 * \note
 *    - Constant data that has been linked to the external flash must not be erased. Otherwise
 *      the application CRC will be invalidated and thus the bootloader will not start the
 *      application anymore.
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_ChipErase(void);

/**********************************************************************************************//**
 *
 * \brief Returns the status of the last flash operation
 *
 *   This function returns the result of the last operation accepted by the flash driver.
 *
 *   The driver is idle if the return value is not equal to #IO_E_BUSY. (If the last operation did
 *   not finish successfully, the flash chip and the internal states are reset automatically.)
 *
 *   For code examples see the section
 *   \ref flash_status_example "Example for checking the status of the driver".
 *
 * \return #IO_ErrorType
 * \retval #IO_E_BUSY                   Driver is busy, last operation is still ongoing
 * \retval #IO_E_OK                     Driver is idle, last operation finished successfully
 * \retval #IO_E_FLASH_OP_TIMEOUT       Driver is idle, the last operation exceeded the allowed
 *                                      time
 * \retval #IO_E_FLASH_OP_FAILED        Driver is idle, the last operation has failed
 * \retval #IO_E_FLASH_BLANK_CHECK_FAILED Driver is idle, during the last write operation a
 *                                      non-blank byte was found
 * \retval #IO_E_UNKNOWN                Driver is idle, during the last operation an internal or
 *                                      unknown error occurred
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The flash module has not been initialized
 * \retval #IO_E_FLASH_SUSPENDED        The Flash is in a suspended state
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_GetStatus(void);

/**********************************************************************************************//**
 *
 * \brief Sets the active flash bank
 *
 *   The Flash memory is broken into banks of size #IO_FLASH_BYTE_SIZE. This function sets the active flash bank.
 *
 *   See \ref flash_banks "Flash Banks" for details.
 *
 * \param Bank              Requested flash bank
 *
 * \return #IO_ErrorType
 * \retval #IO_E_BUSY                   Driver is busy, last operation is still ongoing
 * \retval #IO_E_OK                     Driver is idle, last operation finished successfully
 * \retval #IO_E_INVALID_PARAMETER      Invalid flash bank requested
 * \retval #IO_E_UNKNOWN                Driver is idle, during the last operation an internal or
 *                                      unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_BankSelect(ubyte1 Bank);

/**********************************************************************************************//**
 *
 * \brief Gets the active flash bank
 *
 *   The flash memory is broken into banks of size #IO_FLASH_BYTE_SIZE. This function returns the active flash bank.
 *
 *   See \ref flash_banks "Flash Banks" for details.
 *
 * \param Bank              Currently active flash bank
 *
 * \return #IO_ErrorType
 * \retval #IO_E_BUSY                   Driver is busy, last operation is still ongoing
 * \retval #IO_E_OK                     Driver is idle, last operation finished successfully
 * \retval #IO_E_INVALID_PARAMETER      Invalid flash bank requested
 * \retval #IO_E_UNKNOWN                Driver is idle, during the last operation an internal or
 *                                      unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_GetBank(ubyte1* Bank);

/**********************************************************************************************//**
 *
 * \brief Suspends (or Resumes) the API access to the external flash
 *
 *   The external flash memory can be accessed using the API functions in this module (read, write and erase access)
 *   but it is also mapped directly to the CPU address space
 *   (starting at address 0x64000000, only the active bank), which allows direct read access.
 *   To get correct and consistent data, the direct read access should not be used together with the API functions
 *   (specifically not with erasing and writing). 
 *   The function \c IO_FLASH_Suspend() allows internal suspending of the API functions to ensure
 *   that they cannot interfere with the direct read access.
 * 
 *   Another feature of this function is to suspend an ongoing Block Erase. If a Block Erase is in progress when the 
 *   function \c IO_FLASH_Suspend() is called the erase operation will be suspended until the flash module is resumed. 
 *   While in a suspended block erase state any data can be read from the external flash except the block which is being
 *   erased.
 *
 * \note
 *   The following operations can not be executed while the flash is in a suspended state: 
 *   \c IO_FLASH_BlockErase(), \c IO_FLASH_ChipErase(), \c IO_FLASH_Read(), and \c IO_FLASH_Write().
 *   Switching flash banks is not suspended.
 * 
 * \note
 *   If the application changes the flash bank while in a suspended state, upon resuming future operations will take 
 *   place on the new flash bank.\n
 *   If a Block Erase was suspended with \c IO_FLASH_Suspend() and during the suspend the flash bank was changed, upon 
 *   resuming the Block Erase will complete successfully on the originally intended Block (regardless of flash bank). 
 *   After this future operations will take place on the new flash bank.
 *
 * \param  suspend             #TRUE = flash API use suspended, #FALSE = flash API use resumed
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Operation successful
 * \retval #IO_E_BUSY                   Last operation of flash module is still in progress
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The flash module has not been initialized
 * \retval #IO_E_FLASH_SUSPENDED        The Flash is already in a suspended state
 *
 *************************************************************************************************/
IO_ErrorType IO_FLASH_Suspend(bool suspend);

#ifdef __cplusplus
}
#endif

#endif /* IO_FLASH_H_ */
