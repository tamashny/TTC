/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_DOWNLOAD.h
 *
 * \latexonly \hypertarget{IO_DOWNLOAD}{} \endlatexonly
 *
 * \brief IO Driver functions for handling Ethernet download requests
 *
 * Because an Ethernet link establishment can take up to 3 seconds, any Ethernet download request
 * from the TTC-Downloader cannot be checked during power up (as it is the case for CAN). To make
 * it possible anyway, the DOWNLOAD module can be used for. It's being initialized with
 * \c IO_DOWNLOAD_Init().
 *
 * The DOWLNLOAD module automatically checks for TTC-Downloader requests on the Ethernet interface.
 * This request consists of a handshake between the TTC-Downloader and the ECU. To check, if any
 * request is pending, \c IO_DOWNLOAD_CheckRequest() can be used.
 *
 * After a detected request, the application has a time of 3 seconds to launch the download mode.
 * Within this time, the application needs to shut down and all non-volatile memories need to be
 * stored. With a call to \c IO_DOWNLOAD_Launch(), the ECU restarts in Ethernet download mode. If
 * the download mode is not launched during this time, the request becomes invalid and a new
 * request is necessary.
 *
 * The configuration for setting up the download capability is taken from the APDB. There,
 * the fields
 *    - TargetIPAddress
 *    - SubnetMask
 *    - DLMulticastIPAddress
 *
 * are used. If an enforcement to default settings has been detected during startup, the
 * bootloader's default settings will be applied.
 *
 * DOWNLOAD-API Usage:
 *   - \ref download_examples "Examples for DOWNLOAD API functions"
 *
 * \section download_examples DOWNLOAD Code Examples
 * \brief Examples for using the DOWNLOAD API
 * \code
 *      // initialize Ethernet interface for handling download requests
 *      IO_DOWNLOAD_Init();
 *
 *      // application cycle
 *      while (1)
 *      {
 *          ...
 *
 *          // check, if a download request is pending
 *          if (IO_DOWNLOAD_CheckRequest() == IO_E_OK)
 *          {
 *              // shut down application
 *              ...
 *
 *              // save memories
 *              ...
 *
 *              // launch ECU in download mode
 *              IO_DOWNLOAD_Launch();
 *          }
 *
 *          ...
 *      }
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_DOWNLOAD_H_
#define IO_DOWNLOAD_H_

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

/**********************************************************************************************//**
 *
 * \brief Initialization of the Ethernet download communication driver.
 *
 * The function
 *    - Initializes the Ethernet communications interface
 *    - Sets up all internal modules for automatic download request handling

 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the used ECU variant does not support this function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not
 *                                      been called before
 * \retval #IO_E_CHANNEL_BUSY           channel has been initialized before
 *
 *************************************************************************************************/
IO_ErrorType IO_DOWNLOAD_Init(void);

/**********************************************************************************************//**
 *
 * \brief Checks, if an Ethernet download request is currently pending
 *
 * The function returns the information, if a download request is currently pending with respect of
 * the timeout limits. Once the timing exceeds the limits, the function will return
 * \c #IO_E_DOWNLOAD_NO_REQ.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     a download request is pending
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the driver hasn't been initialized before
 * \retval #IO_E_DOWNLOAD_NO_REQ        no download request is pending
 *
 *************************************************************************************************/
IO_ErrorType IO_DOWNLOAD_CheckRequest(void);

/**********************************************************************************************//**
 *
 * \brief Restarts the ECU in Ethernet download mode
 *
 * As a precondition, a download request must be pending. The function restarts the ECU. On
 * success, the function will NOT return. Instead, the ECU enters the bootloader and activates
 * Ethernet download mode there.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine (theoretically, see above)
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the driver hasn't been initialized before
 * \retval #IO_E_DOWNLOAD_NO_REQ        no download request is pending
 *
 *************************************************************************************************/
IO_ErrorType IO_DOWNLOAD_Launch(void);

#ifdef __cplusplus
}
#endif

#endif /* IO_DOWNLOAD_H_ */
