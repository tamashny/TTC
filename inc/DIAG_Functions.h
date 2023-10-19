/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 *
 * \file DIAG_Functions.h
 *
 * \latexonly \hypertarget{DIAG_Functions}{} \endlatexonly
 *
 * \brief Auxiliary functions for watchdog handling and Flash/RAM/CfgFlash correctable errors
 * monitoring.
 *
 * Provides the interface to the watchdog infrastructure and Flash/RAM/CfgFlash correctable errors
 * monitoring.
 *
 *************************************************************************************************/

#ifndef DIAG_FUNCTIONS_H_
#define DIAG_FUNCTIONS_H_

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
 * \brief Status function for diagnostic state machine and watchdog states
 *
 * Returns the current status as well as the error codes of the diagnostic state machine and
 * the watchdog CPU.
 *
 * \param[out] diag_state       current state of the diagnostic state machine
 * \param[out] watchdog_state   current state of the watchdog CPU
 * \param[out] diag_error       error codes of the diagnostic state machine
 * \param[out] watchdog_error   error codes of the watchdog CPU
 * \param[out] error_count      watchdog CPU device error counter
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  everything fine
 * \retval #IO_E_UNKNOWN             an unknown error occurred
 * \retval #IO_E_NULL_POINTER        null pointer has been passed
 *
 * \note
 *      When debug key in the APDB is set, the error count will be by 1 higher than not in debug
 *      mode, because of the initial reset of the debugger.
 *
 *************************************************************************************************/
IO_ErrorType DIAG_Status(ubyte1 * diag_state,
                         ubyte1 * watchdog_state,
                         DIAG_ERRORCODE * diag_error,
                         DIAG_ERRORCODE * watchdog_error,
                         ubyte1 * error_count);

/**********************************************************************************************//**
 *
 * \brief Allows an application driven safe state
 *
 * The safe state is activated immediately.
 * When resets are configured in the I/O Driver safety configuration
 * (field \c reset_behavior in #IO_DRIVER_SAFETY_CONF), the ECU will reset after 22 ms.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                  Everything fine.
 *
 *************************************************************************************************/
IO_ErrorType DIAG_EnterSafestate(void);

/**********************************************************************************************//**
 *
 * \brief Reads the number of correctable ECC errors in the internal flash
 *
 * This function returns the number of correctable ECC errors in the flash since start-up.
 *
 * If there is an overflow in the 16 bit internal register in the CPU, the \c overflow parameter is
 * set to #TRUE and \c err_cnt is set to 65,535.
 *
 * \param[out] err_cnt      Returns the correctable ECC error counter value of the flash.
 * \param[out] overflow     Returns the counter overflow status. This parameter is optional (it's
 *                          not set if \c #NULL).\n
 *                          Possible values are:
 *                              - #FALSE: No overflow occurred.
 *                              - #TRUE: An overflow occurred in the counter. The actual number of
 *                                errors is anything larger than 65,535.
 *
 * \retval #IO_E_OK             Operation executed successfully.
 * \retval #IO_E_NULL_POINTER   The \c err_cnt parameter is a NULL pointer.
 *
 *************************************************************************************************/
IO_ErrorType DIAG_GetFlashErrors(ubyte2 * err_cnt,
                                 bool * overflow);

/**********************************************************************************************//**
 *
 * \brief Reads the number of correctable ECC errors in the configuration flash
 *
 * This function returns the number of correctable ECC errors in the configuration flash since
 * start-up.
 *
 * If there is an overflow in the 16 bit internal register in the CPU,  the \c overflow parameter
 * is set to #TRUE and \c err_cnt is set to 65,535.
 *
 * \param[out] err_cnt      Returns the correctable ECC error counter value of the configuration
 *                          flash.
 * \param[out] overflow     Returns the counter overflow status. This parameter is optional (it's
 *                          not set if \c #NULL).\n
 *                          Possible values are:
 *                              - #FALSE: No overflow occurred.
 *                              - #TRUE: An overflow occurred in the counter. The actual number of
 *                                errors is anything larger than 65,535.
 *
 * \retval #IO_E_OK             Operation executed successfully.
 * \retval #IO_E_NULL_POINTER   The \c err_cnt parameter is a NULL pointer.
 *
 *************************************************************************************************/
IO_ErrorType DIAG_GetCfgFlashErrors(ubyte2 * err_cnt,
                                    bool * overflow);

/**********************************************************************************************//**
 *
 * \brief Reads the number of correctable ECC errors in bank 0 of the internal RAM
 *
 * This function returns the number of correctable ECC errors in bank 0 of the internal RAM since
 * start-up.
 *
 * If there is an overflow in the 16 bit internal register in the CPU, the \c overflow parameter is
 * set to #TRUE and \c err_cnt is set to 65,535.
 *
 * \param[out] err_cnt      Returns the correctable ECC error counter value of bank 0 of the
 *                          internal RAM.
 * \param[out] overflow     Returns the counter overflow status. This parameter is optional (it's
 *                          not set if \c #NULL).\n
 *                          Possible values are:
 *                              - #FALSE: No overflow occurred.
 *                              - #TRUE: An overflow occurred in the counter. The actual number of
 *                                errors is anything larger than 65,535.
 *
 * \retval #IO_E_OK             Operation executed successfully.
 * \retval #IO_E_NULL_POINTER   The \c err_cnt parameter is a NULL pointer.
 *
 *************************************************************************************************/
IO_ErrorType DIAG_GetRamB0Errors(ubyte2 * err_cnt,
                                 bool * overflow);

/**********************************************************************************************//**
 *
 * \brief Reads the number of correctable ECC errors in bank 1 of the internal RAM
 *
 * This function returns the number of correctable ECC errors in bank 1 of the internal RAM since
 * start-up.
 *
 * If there is an overflow in the 16 bit internal register in the CPU, the \c overflow parameter is
 * set to #TRUE and \c err_cnt is set to 65,535.
 *
 * \param[out] err_cnt      Returns the correctable ECC error counter value of bank 1 of the
 *                          internal RAM.
 * \param[out] overflow     Returns the counter overflow status. This parameter is optional (it's
 *                          not set if \c #NULL).\n
 *                          Possible values are:
 *                              - #FALSE: No overflow occurred.
 *                              - #TRUE: An overflow occurred in the counter. The actual number of
 *                                errors is anything larger than 65,535.
 *
 * \retval #IO_E_OK             Operation executed successfully.
 * \retval #IO_E_NULL_POINTER   The \c err_cnt parameter is a NULL pointer.
 *
 *************************************************************************************************/
IO_ErrorType DIAG_GetRamB1Errors(ubyte2 * err_cnt,
                                 bool * overflow);

#ifdef __cplusplus
}
#endif

#endif /* DIAG_FUNCTIONS_H_ */
