/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_DEBUG.h
 *
 * \latexonly \hypertarget{IO_DEBUG}{} \endlatexonly
 *
 * \brief IO Driver functions for DEBUG utilities.
 *
 *  Provides functions to set debug output pins (LEDs), to interface the UART in an Stdio like
 *  manner (e.g. printf) and to obtain the watchdog's state when running in debug mode.
 *
 *  The pins (LEDs) are only accessible via the JTAG adapter board and thus only with open devices
 *  and connected JTAG adapter board. These pins are only suitable for debugging purposes, like
 *  function runtime measurements, or debug status outputs.

 *  DEBUG-API Usage:
 *    - \ref debug_examples "Examples for DEBUG API functions"
 *
 * \note \anchor debug_stdout_buffering
 *   The standard output line buffered. This means that strings which do not end with a line break
 *   character (<tt>'\\n'</tt>) and are less than 80 characters long will stay in the stdout buffer.
 *   This buffer can be flushed immediately with <tt>fflush(stdout);</tt>
 *
 * \section debug_examples DEBUG Code Examples
 * \brief Examples for using the DEBUG API
 * \subsection debug_pin_examples Example for DEBUG pin setting
 * \code
 *
 *  // set debug pin 0 (LED 0) to high
 *  IO_DEBUG_SetOutputPin(IO_DEBUG_OUTPUT_PIN_0, //pin 0
 *                        TRUE);                 //set high
 *
 *  task()
 *
 *  // set debug pin 0 (LED 0) to low
 *  IO_DEBUG_SetOutputPin(IO_DEBUG_OUTPUT_PIN_0, //pin 0
 *                        FALSE);                //set low
 *
 * \endcode
 *
 * \subsection stdio_examples Example for Stdio usage
 * \code
 *
 *  IO_ErrorType io_error;
 *
 *  // initialize UART for standard I/O interaction
 *  IO_DEBUG_StdioInit();
 *
 *  // output a string using printf
 *  printf("Hello World!\r\n");
 *
 *  printf("This is a line without a line break.");
 *
 *  // flush stdout after printing the last line without a line break
 *  fflush(stdout);
 *
 *  // wait for the UART buffers to be completely emptied
 *  do
 *  {
 *      io_error = IO_DEBUG_GetTxStatus();
 *  } while (io_error != IO_E_OK);
 *
 *  // deinitialize UART for standard I/O interaction
 *  IO_DEBUG_StdioDeInit();
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_DEBUG_H_
#define IO_DEBUG_H_

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

#define IO_DEBUG_OUTPUT_PIN_0       0U  /**< Debug Pin 0 */
#define IO_DEBUG_OUTPUT_PIN_1       1U  /**< Debug Pin 1 */
#define IO_DEBUG_OUTPUT_PIN_2       2U  /**< Debug Pin 2 */

#define IO_DEBUG_WD_NORMAL          0U  /**< Watchdog mode is normal */
#define IO_DEBUG_WD_STATE_PREPARED  1U  /**< Watchdog mode is debug with state "prepared" */
#define IO_DEBUG_WD_STATE_ACTIVE    2U  /**< Watchdog mode is debug with state "active" */
#define IO_DEBUG_WD_STATE_SAFE      3U  /**< Watchdog mode is debug with state "safe" */
#define IO_DEBUG_WD_STATE_UNKNOWN   4U  /**< Watchdog mode is debug with an unknown state */

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Sets a debug output pin (LED).
 *
 * \param pin               Debug output pin, one of:
 *                              - \c #IO_DEBUG_OUTPUT_PIN_0
 *                              - \c #IO_DEBUG_OUTPUT_PIN_1
 *                              - \c #IO_DEBUG_OUTPUT_PIN_2
 * \param value             Output value, one of:
 *                              - \c #TRUE:  High level
 *                              - \c #FALSE: Low level
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CH_CAPABILITY          \c pin parameter is invalid
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_FPGA_NOT_INITIALIZED   the FPGA has not been initialized
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_DEBUG_SetOutputPin(ubyte1 pin,
                                   bool value);

/**********************************************************************************************//**
 *
 * \brief Initializes the UART interface for standard IO usage.
 *
 *  Used settings are: 115.200 baud, 8 databits, 1 stopbit, no parity
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_CHANNEL_BUSY           channel has been initialized before
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_DEBUG_StdioInit(void);

/**********************************************************************************************//**
 *
 * \brief Deinitializes the UART interface for standard IO usage.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED channel has not been initialized
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_DEBUG_StdioDeInit(void);

/**********************************************************************************************//**
 *
 * \brief Checks if the stdout transmission buffers are empty
 *
 * With the help of this function it can be checked if all the parameters of a printf()
 * have been transmitted. This doesn't include the line buffering of stdout -- for details see the
 * \ref debug_stdout_buffering "note about stdout buffering".
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     All the characters printed to stdout have been transmitted.
 * \retval #IO_E_BUSY                   There is at least one character in the UART module which
 *                                      has not been printed out yet.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The UART interface has not been initialized
 *                                      for Stdio usage.
 *
 *************************************************************************************************/
IO_ErrorType IO_DEBUG_GetTxStatus( void );

/**********************************************************************************************//**
 *
 * \brief Returns the current debug state of the watchdog module
 *
 * \param[out] wd_state     Indicates the watchdog debug state
 *                              - \c #IO_DEBUG_WD_NORMAL: Watchdog module runs in normal mode
 *                              - \c #IO_DEBUG_WD_STATE_PREPARED: Watchdog module is in prepared
 *                                   debug state
 *                              - \c #IO_DEBUG_WD_STATE_ACTIVE: Watchdog module is in active debug
 *                                   state
 *                              - \c #IO_DEBUG_WD_STATE_SAFE: Watchdog module is in safe debug
 *                                   state
 *                              - \c #IO_DEBUG_WD_STATE_UNKNOWN: Watchdog module runs in debug mode
 *                                   but the state could not be obtained
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_NULL_POINTER           a NULL pointer has been passed to the function
 * \retval #IO_E_WD_STATUS_INVALID      the watchdog's status information could not be obtained
 *
 *************************************************************************************************/
IO_ErrorType IO_DEBUG_GetWatchdogState(ubyte1 * const wd_state);

#ifdef __cplusplus
}
#endif

#endif /* IO_DEBUG_H_ */
