/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_PVG.h
 *
 * \latexonly \hypertarget{IO_PVG}{} \endlatexonly
 *
 * \brief IO Driver functions for PVG channels.
 *
 *   Contains all service functions for the PVG (Proportional Valve Group) outputs. Up to 8
 *   channels can be configured: \c #IO_PVG_00 .. \c #IO_PVG_07
 *
 *   PVG valves are active valves with integrated electronic control. The input impedance is
 *   defined with 12kOhm to 50% Ubat (2 * 24kOhm to Ubat and GND).
 *
 *   The PVG output stage is a push/pull PWM output with a well defined output resistance of
 *   2.58kOhm. This resistance is necessary for operation, low pass filtering and over load
 *   protection.
 *
 * \note
 *   The correction of the PVG impedance is directly performed inside the module. Therefore a PVG
 *   valve is needed for correct operation of this outputs.
 *
 *   When configuring a PVG output, the associated voltage feedback channel will also be
 *   configured.
 *
 *      PVG-API Usage:
 *        - \ref pvg_examples "Examples for PVG API functions"
 *
 * \section pvg_output_protection PVG output protection
 *
 * Each PVG output is individually protected against malfunction. Whenever the difference between
 * the measured output (U_feedback) and the configured output calculated by <b>U_diff =
 * output_value / 100 * U_BAT - U_feedback</b> is greater than <b>abs(+/-18V)</b> for at least
 * 100ms, the output protection is enabled latest within 12ms.
 *
 * When entering the protection state, the PVG output has to remain in this state for at least
 * 1s. After this wait time the PVG output can be reenabled via function \c
 * IO_PVG_ResetProtection(). Note that the number of reenabling operations for a single PVG
 * output is limited to 10. Refer to function \c IO_PVG_ResetProtection() for more information on
 * how to reenable a PVG output.
 *
 * \section pvg_examples PVG Code Examples
 * \brief Examples for using the PVG API
 * \subsection pvg_init_examples PVG initialization example:
 * \code
 *     // Setup a PVG output with output value of 50%
 *     IO_PVG_Init(IO_PVG_00,
 *                 5000);           // output value is 5000 = 50%
 * \endcode
 *
 * \subsection pvg_task_examples PVG task examples:
 * \code
 *     IO_PVG_SetOutput(IO_PVG_00,
 *                      6200);      // set duty cycle to 62%
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_PVG_H_
#define IO_PVG_H_

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
 * \brief Setup one PVG channel
 *
 * \param pvg_channel       PVG channel (\c #IO_PVG_00 .. \c #IO_PVG_07)
 * \param output_value      Output value with which the PVG-channel will be initialized in
 *                          percent * 100 ( 1000..9000 )
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist or is not available on
 *                                      the used ECU variant
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PVG channel or the used ECU
 *                                      variant does not support this function
 * \retval #IO_E_INVALID_PARAMETER      parameter is out of range
 * \retval #IO_E_CHANNEL_BUSY           the PVG output channel or the voltage feedback channel is
 *                                      currently used by another function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_FPGA_NOT_INITIALIZED   the FPGA has not been initialized
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_PVG_Init(ubyte1 pvg_channel,
                         ubyte2 output_value);

/**********************************************************************************************//**
 *
 * \brief Deinitializes one PVG output
 *
 * \param pvg_channel       PVG output (\c #IO_PVG_00 .. \c #IO_PVG_07)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PVG channel
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_PVG_DeInit(ubyte1 pvg_channel);

/**********************************************************************************************//**
 *
 * \brief Sets the output value of one PVG channel
 *
 * \param pvg_channel       PVG channel (\c #IO_PVG_00 .. \c #IO_PVG_07)
 * \param output_value      Output value in percent * 100 (1000..9000)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_FET_PROT_ACTIVE        the output FET protection is active and has set the output
 *                                      to 0%. It can be reset with \c IO_PVG_ResetProtection()
 *                                      after the wait time is passed
 * \retval #IO_E_FET_PROT_REENABLE      the output FET protection is ready to be reset with
 *                                      \c IO_PVG_ResetProtection()
 * \retval #IO_E_FET_PROT_PERMANENT     the output FET is protected permanently because it was
 *                                      already reset more than 10 times. The output will remain at
 *                                      0%
 * \retval #IO_E_INVALID_PARAMETER      the setpoint is out of range
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PVG channel
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_PVG_SetOutput(ubyte1 pvg_channel,
                              ubyte2 output_value);

/**********************************************************************************************//**
 *
 * \brief Get the voltage feedback of one PVG channel
 *
 * \param pvg_channel       PVG channel (\c #IO_PVG_00 .. \c #IO_PVG_07)
 * \param[out] voltage      Measured voltage in mV. Range: 0..32000  (0V..32.000V)
 * \param[out] fresh        Indicates if new values are available since the last call.
 *                              - \c #TRUE: Value in "voltage" is valid
 *                              - \c #FALSE: No new value available.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_NULL_POINTER           a NULL pointer has been passed to the function
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PVG channel
 * \retval #IO_E_STARTUP                the output is in the startup phase
 * \retval #IO_E_REFERENCE              the reference voltage is out of range
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *   If there is no new measurement value available (for example the function
 *   \c IO_PVG_GetVoltage() gets called more frequently than the AD sampling) the flag \c fresh
 *   will be set to \c #FALSE.
 *
 *************************************************************************************************/
IO_ErrorType IO_PVG_GetVoltage(ubyte1 pvg_channel,
                               ubyte2 * const voltage,
                               bool * const fresh);

/**********************************************************************************************//**
 *
 * \brief Reset the output protection for a PVG channel
 *
 * \param pvg_channel       PVG channel (\c #IO_PVG_00 .. \c #IO_PVG_07)
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
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PVG channel
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \attention
 *   The protection can be reset 10 times, afterwards the output will remain permanently
 *   protected
 *
 * \note
 *    - After entering the output protection, a certain time has to pass before the output
 *    protection can be reset:
 *       - 1s for \c #IO_PVG_00 .. \c #IO_PVG_07
 *
 * \note
 *    - This function will not set the output back again to the last setpoint. After calling this
 *    function the output has to be set to the intended level with \c IO_PVG_SetOutput()
 *
 * \remarks
 *   If the parameter \c reset_cnt is \c #NULL, the parameter is ignored. The parameter
 *   \c reset_cnt returns the number of resets already performed.
 *
 *************************************************************************************************/
IO_ErrorType IO_PVG_ResetProtection(ubyte1 pvg_channel,
                                    ubyte1 * const reset_cnt);

#ifdef __cplusplus
}
#endif

#endif /* IO_PVG_H_ */
