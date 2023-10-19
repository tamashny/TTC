/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_EEPROM.h
 *
 * \latexonly \hypertarget{IO_EEPROM}{} \endlatexonly
 *
 * \brief IO Driver functions for external EEPROM/FRAM.
 *
 *  The EEPROM module allows writing to and reading from the external SPI EEPROM or FRAM
 *  (depending on the product variant).
 *
 *  All functions are strictly non-blocking (i.e. they return immediately).
 *  The EEPROM/FRAM operations, including the communication on the SPI bus, can be lengthy
 *  and for this reason they are not performed in the API functions but rather "in the background":
 *  The API calls (\c IO_EEPROM_Read() or \c IO_EEPROM_Write()) initiate the reading/writing, after
 *  which the \c IO_EEPROM_GetStatus() function can be called to see if the operation finished
 *  or is still ongoing.
 *
 *  The background processing of the read or write operations takes place in a periodic (1 ms) interrupt.
 *  This happens without the user application's intervention and guarantees the minimum read and write
 *  data transfer rates (see below). If these minimum rates are not sufficient, they can be substantially increased
 *  by the user application calling \c IO_EEPROM_GetStatus() in a polling loop.
 *  Other than returning the status of the EEPROM/FRAM driver, this function performs the processing
 *  of the read/write operations. These operations consist of a sequence of SPI commands that take typ. several us.
 *  When the \c IO_EEPROM_GetStatus() is polled, it will advance to the next stage as soon as the previous one
 *  is finished without waiting for the next 1 ms period.
 *
 * \attention
 *  In situations where interrupts are disabled, such as in the error or notification callbacks,
 *  the polling method is necessary.
 *
 * \section eeprom_speed Speed of the EEPROM/FRAM Operations
 *
 *  Due to internal limitations, the size of the SPI communication buffer is limited to 64 B.
 *  This is also the effective page size for the EEPROM/FRAM.
 *
 * \note
 *  The communication buffer size has impact on the performance of the read/write operations
 *  but it does not limit the size of the data that can be read of written by the API functions.
 *  The API functions will process the read/write requests over multiple pages if necessary.
 *
 * \subsection eeprom_speed_interrupt Read and write speeds of the background processing
 *
 *  The maximum duration of <b>reading</b> an EEPROM/FRAM page, when relying on the background interrupt, is 3 ms.
 *  The full page buffer is used when the data start address and size is aligned to 64 B.
 *  When this is fulfilled, the EEPROM driver guarantees the minimum read data rate of 21333 B/s.
 *
 *  <b>Writing</b> requires min. 4 ms per page.
 *  The EEPROM needs additional 5 ms for the internal programming cycle (max), i.e. the maximum duration for writing
 *  one page is 9 ms. With the data aligned to 64 B, the minimum write data rate for the EEPROM is 7111 B/s.
 *  The FRAM adds no additional waiting time for its internal programming. The maximum write time of one page is 5 ms
 *  and the minimum write data rate is 12800 B/s.
 *
 *  \subsection eeprom_speed_polling Read and write speeds with the IO_EEPROM_GetStatus() polling
 *
 *  The values given below assume that the minimum read/written data size is 64 B. The alignment is not required.
 *
 *  With the polling method, the speed of reading the EEPROM or FRAM is limited by the bandwidth of the SPI bus.
 *  The <b>read</b> data rate that can be achieved is 267000 B/s and higher.
 *
 *  The <b>write</b> speed achievable for the FRAM is 247000 B/s.
 *  The EEPROM write speed cannot be significantly improved by the polling because it is limited by the internal
 *  5 ms programming delay (max), i.e. to 12800 B/s.
 *
 *  The data rates for the polling method are not guaranteed because they depend on the frequency of the polling.
 *  The desired polling period is 1.6 us and shorter: If this is fulfilled, the specified data rates will surely be
 *  exceeded. In practice, however, the polling frequency depends on various CPU load conditions
 *  (such as the background interrupts) that are difficult to control.
 *  The given data rates are typical values verified by measurements in a test application. They should be met with
 *  a high degree of confidence in the majority of customer applications.
 *
 * \section eep_examples EEPROM Code Examples
 * \brief Examples for using the EEPROM API
 * \subsection eeprom_init_example Example for EEPROM/FRAM initialization
 * \details EEPROM needs to be initialized after \c IO_Driver_Init().
 * \code
 *      IO_ErrorType io_error;
 *
 *      // initialize driver
 *      io_error = IO_Driver_Init(NULL);
 *
 *      // initialize EEPROM module
 *      io_error = IO_EEPROM_Init();
 * \endcode
 *
 * \subsection eeprom_write_example Example for EEPROM/FRAM write
 * \code
 *      ubyte1 data[6] = {0, 1, 2, 3, 4, 5};
 *
 *      // check if EEPROM is busy
 *      if (IO_EEPROM_GetStatus() == IO_E_OK)
 *      {
 *          // if not busy write data
 *          io_error = IO_EEPROM_Write(0,      // offset
 *                                     6,      // length
 *                                     data);  // data buffer
 *      }
 *
 *      // Write is complete when IO_EEPROM_GetStatus() returns IO_E_OK again.
 * \endcode
 *
 * \subsection eeprom_read_example Example for EEPROM/FRAM read
 * \code
 *      ubyte1 data[2000] = {0};
 *
 *      // check if EEPROM is busy
 *      if (IO_EEPROM_GetStatus() == IO_E_OK)
 *      {
 *          // if not busy start reading
 *          io_error = IO_EEPROM_Read(0,       // offset
 *                                    2000,    // length
 *                                    data);   // data buffer
 *      }
 *
 *      // Data is not yet available!
 *      // Data is available when IO_EEPROM_GetStatus() returns IO_E_OK again.
 * \endcode
 *
 * \subsection eeprom_poll_example Polling the EEPROM/FRAM status
 * \code
 *      while (IO_EEPROM_GetStatus() == IO_E_BUSY)
 *      {
 *          // The user application can do its internal processing here
 *          // but decreasing the frequency of the polling will decrease the attained read/write speed.
 *      }
 *
 *      if (IO_EEPROM_GetStatus() == IO_E_OK)
 *      {
 *          // The previous read or write operation completed successfully.
 *      }
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_EEPROM_H_
#define IO_EEPROM_H_

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
 * F U N C T I O N S
 *
 *************************************************************************************************/
/**
 * \brief The Macro maintains backwards compatibility for applications using the \c IO_EEPROM_Process()
 *  from I/O Driver 3.1.
 */
#define IO_EEPROM_Process   IO_EEPROM_GetStatus

/**********************************************************************************************//**
 *
 * \brief Initialization of the EEPROM driver.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine.
 * \retval #IO_E_INVALID_CHANNEL_ID     EEPROM/FRAM is not supported by the used ECU variant.
 * \retval #IO_E_INVALID_PIN_CONFIG     Error in the ECU variant configuration data.
 * \retval #IO_E_INVALID_VARIANT        The detected ECU variant is not supported.
 * \retval #IO_E_CHANNEL_BUSY           Module has been initialized before.
 * \retval #IO_E_EEPROM_SPI             SPI bus was not correctly initialized.
 * \retval #IO_E_DRIVER_NOT_INITIALIZED The IO_Driver_Init function has not been called.
 *
 **************************************************************************************************
 *
 * \remarks
 *   The EEPROM driver can only be initialized once.
 *   Prior to re-initialization, the \c IO_EEPROM_DeInit() function must be called.
 *
 *************************************************************************************************/
IO_ErrorType IO_EEPROM_Init (void);

/**********************************************************************************************//**
 *
 * \brief Deinitializes the EEPROM driver.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The module is not initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_EEPROM_DeInit (void);

/**********************************************************************************************//**
 *
 * \brief Read data from the EEPROM/FRAM
 *
 *   This function triggers the read operation.
 *
 *   The read operation is processed in the background; its state can be polled using
 *   the \c IO_EEPROM_GetStatus() function.
 *
 *   The read data is available in the \c data buffer after the read operation finishes.
 *
 * \param offset            EEPROM/FRAM memory offset (0..65535 for EEPROM, 0..32767 for FRAM)
 * \param length            Length of data to be read (1..65536 for EEPROM, 1..32768 for FRAM)
 * \param[out] data         Pointer to the address where the data shall be stored to
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine.
 * \retval #IO_E_BUSY                   EEPROM module is still busy.
 * \retval #IO_E_INVALID_PARAMETER      Length is zero.
 * \retval #IO_E_EEPROM_RANGE           Invalid address offset or range.
 * \retval #IO_E_NULL_POINTER           A NULL pointer has been passed.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The module is not initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_EEPROM_Read (ubyte2 offset,
                             ubyte4 length,
                             ubyte1 *const data);

/**********************************************************************************************//**
 *
 * \brief Write data to the EEPROM/FRAM
 *
 *   The function triggers a write operation.
 *
 *   The write operation is processed in the background; its state can be polled using
 *   the \c IO_EEPROM_GetStatus() function.
 *
 *   The data to be written must be available in the \c data buffer for the whole duration of the the write operation!
 *
 * \param offset            EEPROM/FRAM memory offset (0..65535 for EEPROM, 0..32767 for FRAM)
 * \param length            Length of the data to be written (1..65536 for EEPROM, 1..32768 for FRAM)
 * \param[in] data          Pointer to the data to be written
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine.
 * \retval #IO_E_BUSY                   EEPROM module is still busy.
 * \retval #IO_E_INVALID_PARAMETER      Length is zero.
 * \retval #IO_E_EEPROM_RANGE           Invalid address offset or range.
 * \retval #IO_E_NULL_POINTER           A NULL pointer has been passed.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The module is not initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_EEPROM_Write (ubyte2 offset,
                              ubyte4 length,
                              const ubyte1 *const data);

/**********************************************************************************************//**
 *
 * \brief Returns the status of the EEPROM driver and triggers the processing of the read and write operations
 *
 *   It can be used to determine whether the EEPROM/FRAM is idle or if a read or write operation is ongoing.
 *   After an unsuccessful read or write operation, \c IO_EEPROM_GetStatus() returns the error code
 *   and clears it.
 *
 *   This function also triggers the internal processing of the read/write operations.
 *   This feature can be used to speed up the read/write operations
 *   (see \ref eeprom_speed "Speed of the EEPROM/FRAM Operations").
 *
 * \remarks
 *   Periodic polling using this function is necessary to trigger the processing of the read/write operations
 *   in situations where interrupts are not available, such as within the error or notification callbacks.
 *
 * \remarks
 *   If used from the error/notification callback, the nature of the failure will effect whether the
 *   EEPROM is usable. In these cases it is important to include a timeout on any operations.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine, driver is idle.
 * \retval #IO_E_BUSY                   A read or a write operation is ongoing, driver is busy.
 * \retval #IO_E_UNKNOWN                An internal error has occurred.
 * \retval #IO_E_EEPROM_SPI             An internal SPI error has occurred when communicating with the EEPROM/FRAM.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The module is not initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_EEPROM_GetStatus (void);

#ifdef __cplusplus
}
#endif

#endif /* IO_EEPROM_H_ */
