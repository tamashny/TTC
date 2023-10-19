/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_VOUT.h
 *
 * \latexonly \hypertarget{IO_VOUT}{} \endlatexonly
 *
 * \brief IO Driver functions for voltage outputs.
 *
 *   Contains all service functions for the voltage outputs. Up to 8 channels can be configured:
 *   \c #IO_VOUT_00 .. \c #IO_VOUT_07
 *
 *   The voltage output stage is a push/pull PWM output with a well defined output resistance of
 *   2.58kOhm. This resistance is necessary for operation, low pass filtering and over load
 *   protection.
 *
 * \note
 *   The voltage output is limited to resistive loads to ground with 10kOhm or higher.
 *
 *   The outputs will be activated after setting them via \c IO_VOUT_SetVoltage().
 *
 *   When configuring a voltage output, the associated voltage feedback channel  will also be
 *   configured.
 *
 * \note
 *   Deviations on the power supply are automatically corrected by the module.
 *
 *      VOUT-API Usage:
 *        - \ref vout_examples "Examples for VOUT API functions"
 *
 * \section vout_output_protection VOUT output protection
 *
 * Each voltage output is individually protected against malfunction. Whenever the difference
 * between the measured output (U_feedback) and the configured output calculated by <b>U_diff =
 * output_value / 100 * U_BAT - U_feedback</b> is greater than <b>abs(+/-18V)</b> for at least
 * 100ms, the output protection is enabled latest within 12ms.
 *
 * When entering the protection state, the voltage output has to remain in this state for at least
 * 1s. After this wait time the voltage output can be reenabled via function \c
 * IO_VOUT_ResetProtection(). Note that the number of reenabling operations for a single voltage
 * output is limited to 10. Refer to function \c IO_VOUT_ResetProtection() for more information on
 * how to reenable a voltage output.
 *
 * \section vout_examples VOUT Code Examples
 * \brief Examples for using the VOUT API
 * \subsection vout_init_examples VOUT initialization example:
 * \code
 *      IO_VOUT_Init(IO_VOUT_00);       // setup a voltage output
 * \endcode
 *
 * \subsection vout_task_examples VOUT task function example:
 * \code
 *      ubyte2 voltage = 0;
 *      bool fresh = FALSE;
 *
 *      IO_VOUT_SetVoltage(IO_VOUT_00,
 *                         6200);       // set output voltage to 6200mV
 *
 *      IO_VOUT_GetVoltage(IO_VOUT_00,
 *                         &voltage,
 *                         &fresh);     // get voltage feedback
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_VOUT_H_
#define IO_VOUT_H_

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
 * \brief Setup one voltage output channel
 *
 * \param vout_channel      VOUT channel (\c #IO_VOUT_00 .. \c #IO_VOUT_07)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist or is not available on
 *                                      the used ECU variant
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a VOUT channel or the used ECU
 *                                      variant does not support this function
 * \retval #IO_E_CHANNEL_BUSY           the VOUT output channel or the voltage feedback channel is
 *                                      currently used by another function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_FPGA_NOT_INITIALIZED   the FPGA has not been initialized
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_VOUT_Init(ubyte1 vout_channel);

/**********************************************************************************************//**
 *
 * \brief Deinitializes one voltage output
 *
 * \param vout_channel      VOUT output (\c #IO_VOUT_00 .. \c #IO_VOUT_07)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a VOUT channel
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_VOUT_DeInit(ubyte1 vout_channel);

/**********************************************************************************************//**
 *
 * \brief Sets the output voltage of one voltage output channel
 *
 * \param vout_channel      VOUT channel (\c #IO_VOUT_00 .. \c #IO_VOUT_07)
 * \param output_voltage    Output voltage in mV (0..32000)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_FET_PROT_ACTIVE        the output FET protection is active and has set the output
 *                                      to 0V. It can be reset with \c IO_VOUT_ResetProtection()
 *                                      after the wait time is passed
 * \retval #IO_E_FET_PROT_REENABLE      the output FET protection is ready to be reset with
 *                                      \c IO_VOUT_ResetProtection()
 * \retval #IO_E_FET_PROT_PERMANENT     the output FET is protected permanently because it was
 *                                      already reset more than 10 times. The output will remain at
 *                                      0V
 * \retval #IO_E_INVALID_PARAMETER      the setpoint is out of range
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a VOUT channel
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_VOUT_SetVoltage(ubyte1 vout_channel,
                                ubyte2 output_voltage);

/**********************************************************************************************//**
 *
 * \brief Get the voltage feedback of one voltage output channel
 *
 * \param vout_channel      VOUT channel (\c #IO_VOUT_00 .. \c #IO_VOUT_07)
 * \param[out] voltage      Measured voltage in mV. Range: 0..32000  (0V..32.000V)
 * \param[out] fresh        Indicates if new values are available since the last call.
 *                           - \c #TRUE: Value in "voltage" is valid
 *                           - \c #FALSE: No new value available.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_NULL_POINTER           a NULL pointer has been passed to the function
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a VOUT channel
 * \retval #IO_E_STARTUP                the output is in the startup phase
 * \retval #IO_E_REFERENCE              the reference voltage is out of range
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *   If there is no new measurement value available (for example the function
 *   \c IO_VOUT_GetVoltage() gets called more frequently than the AD sampling) the flag \c fresh
 *   will be set to \c #FALSE.
 *
 *************************************************************************************************/
IO_ErrorType IO_VOUT_GetVoltage(ubyte1 vout_channel,
                                ubyte2 * const voltage,
                                bool * const fresh);

/**********************************************************************************************//**
 *
 * \brief Reset the output protection for a VOUT channel
 *
 * \param vout_channel      VOUT channel (\c #IO_VOUT_00 .. \c #IO_VOUT_07)
 * \param[out] reset_cnt    Protection reset counter. Indicates how often the application already
 *                          reset the protection.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_FET_PROT_NOT_ACTIVE    no output FET protection is active
 * \retval #IO_E_FET_PROT_PERMANENT     the output FET is protected permanently because it was
 *                                      already reset more than 10 times
 * \retval #IO_E_FET_PROT_WAIT          the output FET protection can not be reset, as the wait
 *                                      time of 1s is not already passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a VOUT channel
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \attention
 *   The protection can be reset 10 times, afterwards the output will remain permanently protected
 *
 * \note
 *   - After entering the output protection, a certain time has to pass before the output
 *   protection can be reset:
 *       - 1s for \c #IO_VOUT_00 .. \c #IO_VOUT_07
 *
 * \note
 *    - This function will not set the output back again to the last voltage. After calling this
 *    function the output has to be set to the intended voltage with \c IO_VOUT_SetVoltage()
 *
 * \remarks
 *   If the parameter \c reset_cnt is \c #NULL, the parameter is ignored. The parameter
 *   \c reset_cnt returns the number of resets already performed.
 *
 *************************************************************************************************/
IO_ErrorType IO_VOUT_ResetProtection(ubyte1 vout_channel,
                                     ubyte1 * const reset_cnt);

#ifdef __cplusplus
}
#endif

#endif /* IO_VOUT_H_ */
