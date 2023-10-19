/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_LIN.h
 *
 * \latexonly \hypertarget{IO_LIN}{} \endlatexonly
 *
 * \brief IO Driver functions for LIN communication.
 *
 *  The LIN driver uses the LIN module of the TMS570 CPU.
 *
 *  The interface is compliant to LIN 2.1 and supports baudrates up to 20 Kbit/s. As the LIN is
 *  only specified for 12V systems, it operates always at 12V, also in a 24V system.
 *
 *  The LIN module automatically generates the header when initiating a read or write transfer. The
 *  LIN supports LIN 2.0 checksum. The LIN also detects short circuits on the bus.
 *
 *  LIN-API Usage:
 *    - \ref lin_examples "Examples for LIN API functions"
 *
 * \section lin_examples LIN Code Examples
 * \brief Examples for using the LIN API
 * \subsection lin_init_examples Example for LIN initialization
 * \code
 *
 *      // init LIN master
 *      IO_LIN_Init(20000,            //20 kBits/s
 *                  IO_LIN_ENHANCED); //enhanced checksum
 *
 * \endcode
 *
 * \subsection lin_task_examples Example for LIN task function call
 * \code
 *      IO_LIN_DATA_FRAME lin_frame_tx;
 *      IO_LIN_DATA_FRAME lin_frame_rx;
 *
 *      // wait for LIN Bus to be ready
 *      while (IO_LIN_GetStatus() == IO_E_BUSY);
 *
 *      // set id
 *      lin_frame_tx.id = 0x23; // id 0x23
 *
 *      // assemble data
 *      lin_frame_tx.data[0] = 0x12;
 *      lin_frame_tx.data[1] = 0x25;
 *      lin_frame_tx.data[2] = 0xAC;
 *      lin_frame_tx.data[3] = 0xFE;
 *
 *      // set length of data
 *      lin_frame_tx.length = 4; // 4 bytes
 *
 *      // write LIN frame
 *      IO_LIN_Write(&lin_frame_tx);
 *
 *      // wait for LIN Bus to be ready
 *      while (IO_LIN_GetStatus() == IO_E_BUSY);
 *
 *      // set id
 *      lin_frame_rx.id = 0x23; // id 0x23
 *
 *      // set length of data
 *      lin_frame_rx.length = 4; // 4 bytes
 *
 *      // read a LIN frame
 *      IO_LIN_Read(&lin_frame_rx);
 *
 *      // wait for LIN Read to be complete
 *      while (IO_LIN_GetStatus() == IO_E_BUSY);
 *
 *      // check if read was successful
 *      if (IO_LIN_GetStatus() == IO_E_OK)
 *      {
 *          // process received data
 *      }
 *      else
 *      {
 *          // error
 *      }
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_LIN_H_
#define IO_LIN_H_

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
 * \name Checksum type
 *
 * Defines the LIN checksum type
 *
 * @{
 */
#define IO_LIN_CLASSIC             0U  /**< Classic checksum */
#define IO_LIN_ENHANCED            1U  /**< Enhanced LIN 2.0 checksum */
/** @} */

/**
 * \name Baudrate configuration
 *
 * Defines the minimum and maximum possible baudrate
 *
 * @{
 */
#define IO_LIN_BAUDRATE_MIN     1000U  /**< Minimum LIN baudrate */
#define IO_LIN_BAUDRATE_MAX    20000U  /**< Maximum LIN baudrate */
/** @} */

/**
 *
 * \brief LIN data frame
 *
 *  Stores a data frame for the LIN communication.
 *
 */
typedef struct io_lin_data_frame_
{
    ubyte1 id;          /**< Frame identifier (0..63)                  */
    ubyte1 length;      /**< Number of bytes to be read/written (1..8) */
    ubyte1 data[8];     /**< data buffer                               */
} IO_LIN_DATA_FRAME;

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Initialization of the LIN communication driver.
 *
 *   The function
 *      - Enables the module and sets the LIN to master mode
 *      - Automatically sets up the bit timing for the given baudrate
 *
 * \param baudrate          Baud rate in bit/s (1000..20000)
 * \param checksum_type     Checksum type, one of:
 *                              - \c #IO_LIN_CLASSIC,
 *                              - \c #IO_LIN_ENHANCED
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     LIN is not supported on the used ECU variant
 * \retval #IO_E_INVALID_PARAMETER      invalid parameter has been passed
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_CHANNEL_BUSY           channel has been initialized before
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_LIN_Init(ubyte2 baudrate,
                         ubyte1 checksum_type);

/**********************************************************************************************//**
 *
 * \brief Reads a LIN frame with the given id and of the given length.
 *
 *   Transmits the LIN Header. After this function returns, the user has to check with
 *   IO_LIN_GetStatus() for receive complete.
 *
 * \param[in,out] frame     Pointer to LIN frame structure. ID and length must be set before. The
 *                          received frame will be stored in the data part.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_BUSY                   the channel is busy, no new data
 *                                      received
 * \retval #IO_E_INVALID_PARAMETER      invalid parameter has been passed
 * \retval #IO_E_NULL_POINTER           null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the channel was not configured
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_LIN_Read(IO_LIN_DATA_FRAME * const frame);

/**********************************************************************************************//**
 *
 * \brief Transmit a LIN frame with the given id and of the given length.
 *
 * \param[in] frame         Pointer to LIN frame structure. ID and length and data parts must be
 *                          provided.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_BUSY                   the channel is busy, no new data
 *                                      received
 * \retval #IO_E_INVALID_PARAMETER      invalid parameter has been passed
 * \retval #IO_E_NULL_POINTER           null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the channel was not configured
 *
 *************************************************************************************************/
IO_ErrorType IO_LIN_Write(const IO_LIN_DATA_FRAME * const frame);

/**********************************************************************************************//**
 *
 * \brief Returns the status of the LIN channel.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_BUSY                   the channel is busy
 * \retval #IO_E_LIN_TIMEOUT            LIN timeout detected, if frame duration
 *                                      exceeded the maximum frame time
 * \retval #IO_E_LIN_PARITY             parity error detected during receiving
 * \retval #IO_E_LIN_OVERRUN            unread data was overwritten
 * \retval #IO_E_LIN_FRAMING            an expected stop bit is not found
 * \retval #IO_E_LIN_NO_RESPONSE        no response from the slave
 * \retval #IO_E_LIN_INCONSISTENT_SYNCH_FIELD inconsistent synch field error was detected
 * \retval #IO_E_LIN_CHECKSUM           a checksum error occurred
 * \retval #IO_E_LIN_PHYSICAL_BUS       a physical bus error was detected
 * \retval #IO_E_LIN_BIT                a bit error was detected
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the channel was not configured
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_LIN_GetStatus(void);

/**********************************************************************************************//**
 *
 * \brief Deinitializes the LIN communication driver.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the channel was not configured
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_LIN_DeInit(void);

#ifdef __cplusplus
}
#endif

#endif /* IO_LIN_H_ */
