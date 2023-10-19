/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_UART.h
 *
 * \latexonly \hypertarget{IO_UART}{} \endlatexonly
 *
 * \brief IO Driver functions for UART communication.
 *
 * The UART driver uses the SCI module of the TMS570 CPU. The interface supports baud rates up to
 * 115.200 bit/s.
 *
 *  UART-API Usage:
 *    - \ref uart_examples "Examples for UART API functions"
 *
 * \section uart_examples UART Code Examples
 * \brief Examples for using the UART API
 * \subsection uart_init_examples Example for UART initialization
 * \code
 *
 *      // init UART driver
 *      IO_UART_Init(115200,                // baud rate
 *                   8,                     // 8 data bits
 *                   IO_UART_PARITY_NONE,   // no parity bit
 *                   1);                    // 1 stop bit
 *
 * \endcode
 *
 * \subsection uart_task_examples Example for UART task function call
 * \code
 *      char tx_data[20] = { '\0' };
 *      char rx_data[20] = { '\0' };
 *      ubyte2 tx_len = 0;
 *      ubyte2 rx_len = 0;
 *
 *      // get number of bytes in the transmit buffer
 *      IO_UART_GetTxStatus(&tx_len);
 *
 *      // check, if there's enough space in the transmit buffer
 *      if ((tx_len + sizeof(tx_data)) < IO_UART_BUFFER_LEN)
 *      {
 *          // transmit some data
 *          IO_UART_Write(tx_data,          // data buffer to be transmitted
 *                        sizeof(tx_data),  // size of the given data buffer
 *                        &tx_len);         // successfully queued data bytes
 *      }
 *
 *      // get number of bytes in the receive buffer
 *      IO_UART_GetRxStatus(&rx_len);
 *
 *      // check, if there's data in the receive buffer
 *      if (rx_len > 0)
 *      {
 *          // try to read some data
 *          IO_UART_Read(rx_data,           // destination data buffer
 *                       sizeof(rx_data),   // size of the given data buffer
 *                       &rx_len);          // number of read data bytes
 *      }
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_UART_H_
#define IO_UART_H_

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
 * \name Baudrate configuration
 *
 * Defines the minimum and maximum possible UART baudrate
 *
 * @{
 */
#define IO_UART_BAUDRATE_MIN     1200U  /**< Minimum UART baudrate */
#define IO_UART_BAUDRATE_MAX   115200U  /**< Maximum UART baudrate */
/** @} */

/**
 * \name UART buffer length
 *
 * Length (in bytes) of the internal buffers for transmission and reception
 *
 * @{
 */
#define IO_UART_BUFFER_LEN         512U  /**< 512 bytes   */
/** @} */

/**
 * \name Parity configuration
 *
 * Defines for the UART parity configuration
 *
 * @{
 */
#define IO_UART_PARITY_NONE        0x0U  /**< no parity   */
#define IO_UART_PARITY_EVEN        0x2U  /**< even parity */
#define IO_UART_PARITY_ODD         0x3U  /**< odd parity  */
/** @} */

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Initialization of the UART module
 *
 *   The UART module can only be initialized after \c IO_Driver_Init().
 *   The function \c IO_UART_Init() needs to be called before any UART function.
 *
 * \param   baudrate        Baud rate in baud/s (1200 ... 115200)
 * \param   dbits           Number of data bits per frame (1 ... 8)
 * \param   par             Parity configuration, one of:
 *                              - \c #IO_UART_PARITY_NONE
 *                              - \c #IO_UART_PARITY_EVEN
 *                              - \c #IO_UART_PARITY_ODD
 * \param   sbits           Number of stop bits per frame (1 .. 2)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      Everything fine.
 * \retval #IO_E_INVALID_CHANNEL_ID      The UART interface is not available on this ECU variant.
 * \retval #IO_E_INVALID_PIN_CONFIG      Error in the ECU variant configuration data.
 * \retval #IO_E_INVALID_VARIANT         The detected ECU variant is not supported.
 * \retval #IO_E_INVALID_PARAMETER       A parameter is out of range.
 * \retval #IO_E_CHANNEL_BUSY            The UART interface is already initialized.
 * \retval #IO_E_DRIVER_NOT_INITIALIZED  The I/O Driver has not been initialized.
 *
 **************************************************************************************************
 *
 * \remarks
 *   Module is initialized only once. To re-initialize the module, the function IO_UART_DeInit()
 *   needs to be called.
 *
 *************************************************************************************************/
IO_ErrorType IO_UART_Init(ubyte4 baudrate,
                          ubyte1 dbits,
                          ubyte1 par,
                          ubyte1 sbits);

/**********************************************************************************************//**
 *
 * \brief Deinitialization of the UART module
 *
 *   The UART module can be initialized again by calling \c IO_UART_Init().
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Channel has not been initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_UART_DeInit(void);

/**********************************************************************************************//**
 *
 * \brief Read data from the serial interface
 *
 *   This function reads the received data from the Rx buffer.
 *   It also reports the errors detected during the reception (IO_E_UART_...) since the last call
 *   to \c IO_UART_GetRxStatus(). The errors are *not cleared* in this function.
 *   See \c IO_UART_GetRxStatus() for more details.
 *
 * \param[out] data         Address where the read data will be stored.
 * \param len               Maximum size of data that can be stored.
 * \param[out] rx_len       Actually read bytes.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine.
 * \retval #IO_E_UART_BUFFER_FULL       The Rx buffer overflowed. The data received after the overflow has been lost.
 * \retval #IO_E_UART_FRAMING           UART framing error has been detected.
 * \retval #IO_E_UART_OVERFLOW          UART hardware reception buffer overrun.
 * \retval #IO_E_UART_PARITY            UART parity check failed.
 * \retval #IO_E_UART_DMA               Error in the Rx DMA data processing. Data has been lost.
 * \retval #IO_E_NULL_POINTER           NULL pointer has been passed.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Channel has not been initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_UART_Read(ubyte1 * const data,
                          ubyte2 len,
                          ubyte2 * const rx_len);

/**********************************************************************************************//**
 *
 * \brief Write data to the serial interface
 *
 *   This function writes the data to the Tx buffer and starts the transmission.
 *
 * \param[in] data          Address to the data to be written.
 * \param len               Number of bytes to be written.
 * \param[out] tx_len       Actually written bytes.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine.
 * \retval #IO_E_UART_BUFFER_FULL       The free space in the Tx buffer is less than \c len.
 *                                      No data is written in this case.
 * \retval #IO_E_NULL_POINTER           NULL pointer has been passed.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Channel has not been initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_UART_Write(const ubyte1 * const data,
                           ubyte2 len,
                           ubyte2 * const tx_len);

/**********************************************************************************************//**
 *
 * \brief Retrieve the status of the reception buffer
 *
 *   This function returns the number of bytes in the Rx buffer and the last error detected during the reception
 *   (IO_E_UART_...) since the last call of this function (i.e., it *reads and clears* the errors).
 *
 * \note
 *   The UART reception stores the last detected error. When multiple errors occurred since the last
 *   call of this function, all except the last one have been discarded.
 *
 * \param[out] rx_len       Number of received data bytes in reception buffer.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine.
 * \retval #IO_E_UART_BUFFER_FULL       The Rx buffer overflowed. The data received after the overflow has been lost.
 * \retval #IO_E_UART_FRAMING           UART framing error has been detected.
 * \retval #IO_E_UART_OVERFLOW          UART hardware reception buffer overrun.
 * \retval #IO_E_UART_PARITY            UART parity check failed.
 * \retval #IO_E_UART_DMA               Error in the Rx DMA data processing. Data has been lost.
 * \retval #IO_E_NULL_POINTER           NULL pointer has been passed.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Channel has not been initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_UART_GetRxStatus(ubyte2 * const rx_len);

/**********************************************************************************************//**
 *
 * \brief Retrieve the status of the transmission buffer
 *
 *
 * \param[out] tx_len       Number of remaining data bytes in transmission buffer.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything fine.
 * \retval #IO_E_NULL_POINTER           NULL pointer has been passed.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED Channel has not been initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_UART_GetTxStatus(ubyte2 * const tx_len);

#ifdef __cplusplus
}
#endif

#endif /* IO_UART_H_ */
