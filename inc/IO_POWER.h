/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_POWER.h
 *
 * \latexonly \hypertarget{IO_POWER}{} \endlatexonly
 *
 * \brief IO Driver functions for Power IC control
 *
 *  The Power IC control functions allow to power down the ECU, switching on and off the sensor
 *  supply voltages of the ECU, enabling/disabling the power stage enable signal and the safety
 *  switches.
 *
 *  The ECU provides 2 separate inputs \c #IO_K15 and \c #IO_WAKEUP for ECU power on and off
 *  functionalities.
 *
 * \note
 *  The ECU does not power down automatically when \c #IO_K15 and \c #IO_WAKEUP is low. When
 *  \c #IO_K15 becomes low, the postrun is still active, and a power down can be now initiated by
 *  the application SW.
 *
 * \remarks
 *  - The ECU shall only be powered down with
 *  \code
 *      IO_POWER_Set(IO_K15,
 *                   IO_POWER_OFF)
 *  \endcode
 *  or
 *  \code
 *      IO_POWER_Set(IO_WAKEUP,
 *                   IO_POWER_OFF)
 *  \endcode
 *  if \c #IO_K15 is low in order to ensure a correct shutdown.
 *
 * \remarks
 *  - It is the responsibility of the user to check for the right state of \c #IO_K15 directly
 *  before calling \c IO_POWER_Set().
 *
 * \attention
 *   - \c #IO_K15 is level sensitive.
 *     * If \c #IO_K15 is low, the ECU will power down completely.
 *       Otherwise the ECU's reaction to <tt>#IO_POWER_Set(#IO_K15, #IO_POWER_OFF)</tt> and
 *       <tt>#IO_POWER_Set(#IO_WAKEUP, #IO_POWER_OFF)</tt> depends on multiple variables.
 *     * If \c #IO_K15 is high, and the resets were enabled in the safety configuration, and the
 *       watchdog device error counter has not reached the final threshold value, the ECU will
 *       perform a hard reset. The hard reset will reset the entire ECU, including all internal
 *       status variables and the watchdog error counter, and then power up the device again. (The
 *       current value of the watchdog error counter can be requested with the \c DIAG_Status()
 *       function.)
 *     * In all other cases the shutdown functions have no effect.
 *
 * \attention
 *  - \c #IO_WAKEUP is edge sensitive.
 *    * If \c #IO_WAKEUP is high and \c IO_POWER_Set() is called,
 *      the ECU will power down and remain off until the next rising edge on \c #IO_WAKEUP (assuming
 *      that \c #IO_K15 is low).
 *
 * \remarks
 *  - Whenever \c IO_POWER_Set() with \c #IO_POWER_OFF is called, the postrun will be disabled.
 *
 * \remarks
 *  - If the ECU can not power down for some reason (e.g. \c #IO_K15 is high and a hard reset is
 *  not possible) a power down will occur as soon as \c #IO_K15 goes low.
 *
 * \remarks
 *  - The application SW can reenable the postrun by calling \c IO_POWER_Set() with
 *  \c #IO_POWER_ON.
 *
 * \section power_examples Power Code Examples
 * \brief Examples for using the Power API
 * \subsection power_init_examples Example for Power initialization
 * \details The Power driver does not need an explicit initialization.
 *          The Power driver is initialized by \c IO_Driver_Init().
 * \code
 *      IO_Driver_Init(NULL);
 * \endcode
 *
 * \subsection sens_example Example for Sensor Supply
 * \code
 *
 *      // switch on sensor supply 0
 *      IO_POWER_Set (IO_SENSOR_SUPPLY_0,
 *                    IO_POWER_ON);
 *
 *      // switch on variable sensor supply 2 with 8 volts
 *      IO_POWER_Set (IO_SENSOR_SUPPLY_2,
 *                    IO_POWER_ON_8V);
 *
 *      // switch off sensor supply 0
 *      IO_POWER_Set (IO_SENSOR_SUPPLY_0,
 *                    IO_POWER_OFF);
 * \endcode
 *
 * \subsection ssw_example Example for Safety Switch
 * \code
 *      ubyte1 ssw2_state = IO_POWER_OFF;
 *
 *      // enable safety switch 0 (shut off group 0)
 *      IO_POWER_Set (IO_INT_SAFETY_SW_0,
 *                    IO_POWER_ON);
 *
 *      // get status of safety switch 2 (shut off group 2)
 *      IO_POWER_Get (IO_INT_SAFETY_SW_2,
 *                    &ssw2_state);
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_POWER_H_
#define IO_POWER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *
 * I N C L U D E S
 *
 *************************************************************************************************/

#include "IO_Error.h"

/**************************************************************************************************
 *
 * D E F I N I T I O N S
 *
 *************************************************************************************************/

/**
 * \name Power values
 * \anchor power_val_def
 *
 * Selects power configuration.
 *
 * @{
 */
#define IO_POWER_OFF      0U  /**< switch off - turns off the respective power function */
#define IO_POWER_ON       1U  /**< switch on - turns on the respective power function */
#define IO_POWER_ON_5V    2U  /**< switch on with 5 volts - configures the respective power
                                   function for 5 volts and turns it on (only available for
                                   \c #IO_SENSOR_SUPPLY_2) */
#define IO_POWER_ON_6V    3U  /**< switch on with 6 volts - configures the respective power
                                   function for 6 volts and turns it on (only available for
                                   \c #IO_SENSOR_SUPPLY_2) */
#define IO_POWER_ON_7V    4U  /**< switch on with 7 volts - configures the respective power
                                   function for 7 volts and turns it on (only available for
                                   \c #IO_SENSOR_SUPPLY_2) */
#define IO_POWER_ON_8V    5U  /**< switch on with 8 volts - configures the respective power
                                   function for 8 volts and turns it on (only available for
                                   \c #IO_SENSOR_SUPPLY_2) */
#define IO_POWER_ON_9V    6U  /**< switch on with 9 volts - configures the respective power
                                   function for 9 volts and turns it on (only available for
                                   \c #IO_SENSOR_SUPPLY_2) */
#define IO_POWER_ON_10V   7U  /**< switch on with 10 volts - configures the respective power
                                   function for 10 volts and turns it on (only available for
                                   \c #IO_SENSOR_SUPPLY_2) */
/** @} */

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Sets a certain mode of the Power IC
 *
 *   Sets a certain mode for a sensor supply pin or the device power
 *
 * \param pin               pin for which the mode shall be set, one of:
 *                              - \c #IO_INT_POWERSTAGE_ENABLE
 *                              - \c #IO_SENSOR_SUPPLY_0
 *                              - \c #IO_SENSOR_SUPPLY_1
 *                              - \c #IO_SENSOR_SUPPLY_2
 *                              - \c #IO_INT_SAFETY_SW_0 (shut off group 0)
 *                              - \c #IO_INT_SAFETY_SW_1 (shut off group 1)
 *                              - \c #IO_INT_SAFETY_SW_2 (shut off group 2)
 *                              - \c #IO_K15 (K15, for power down)
 *                              - \c #IO_WAKEUP (WakeUp, for power down after a wake up)
 * \param mode              sets a certain mode, one of:
 *                               - \c #IO_POWER_ON
 *                               - \c #IO_POWER_OFF
 *                               - \c #IO_POWER_ON_5V
 *                               - \c #IO_POWER_ON_6V
 *                               - \c #IO_POWER_ON_7V
 *                               - \c #IO_POWER_ON_8V
 *                               - \c #IO_POWER_ON_9V
 *                               - \c #IO_POWER_ON_10V
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist or is not available
 *                                         on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY             the given channel is not a POWER channel
 * \retval #IO_E_INVALID_PARAMETER         an invalid parameter has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    power module has not been initialized
 * \retval #IO_E_SAFE_STATE                the given channel is in a safe state
 * \retval #IO_E_INTERNAL_CSM              an internal error occurred
 * \retval #IO_E_UNKNOWN                   an unknown error occurred
 *
 **************************************************************************
 *
 * \remarks
 *    - \c #IO_INT_POWERSTAGE_ENABLE and \c #IO_INT_SAFETY_SW_0 .. \c #IO_INT_SAFETY_SW_2 are
 *    internal pins.
 *
 * \remarks
 *    - \c #IO_INT_POWERSTAGE_ENABLE controls the internal powerstage enable signal. Without
 *    enabling this signal all power outputs remain low (switched off).
 *
 * \remarks
 *    - \c #IO_INT_SAFETY_SW_0 .. \c #IO_INT_SAFETY_SW_2 control the internal safety switches for
 *    PWM outputs and each represents a shut off group. Without switching this signal to ON, the
 *    PWM power  *    stages will not be supplied and the pins will therefore remain low.
 *    The safety switch assignment to the output stages is defined as follows:
 *        - \c #IO_INT_SAFETY_SW_0 (shut off group 0):
 *        \c #IO_PWM_00 .. \c #IO_PWM_13 (\c #IO_DO_16  .. \c #IO_DO_29)
 *        - \c #IO_INT_SAFETY_SW_1 (shut off group 1):
 *        \c #IO_PWM_14 .. \c #IO_PWM_27 (\c #IO_DO_30  .. \c #IO_DO_43)
 *        - \c #IO_INT_SAFETY_SW_2 (shut off group 2):
 *        \c #IO_PWM_28 .. \c #IO_PWM_35 (\c #IO_DO_44  .. \c #IO_DO_51)
 *
 * \remarks
 *    - \c #IO_SENSOR_SUPPLY_0 .. \c #IO_SENSOR_SUPPLY_2 control the 3 external sensor supply pins.
 *    \c #IO_SENSOR_SUPPLY_0 and \c #IO_SENSOR_SUPPLY_1 are fixed to 5 volts. Calling this function
 *    with this pins, the mode can only be set to \c #IO_POWER_ON of \c #IO_POWER_OFF.
 *    \c #IO_SENSOR_SUPPLY_2 is a variable supply and can be configured in the range from 5 to 10
 *    volts. Calling this function with this pin, the mode can only be set to \c #IO_POWER_OFF or
 *    anything in between \c #IO_POWER_ON_5V and \c #IO_POWER_ON_10V.
 *
 *************************************************************************************************/
IO_ErrorType IO_POWER_Set(ubyte1 pin,
                          ubyte1 mode);

/**********************************************************************************************//**
 *
 * \brief Returns the current state of a POWER feature
 *
 *   Returns the state of sensor supplies, K15, WakeUp, safety switches and
 *   power stage enable
 *
 * \param pin               pin for which the mode shall be set, one of:
 *                              - \c #IO_INT_POWERSTAGE_ENABLE
 *                              - \c #IO_SENSOR_SUPPLY_0
 *                              - \c #IO_SENSOR_SUPPLY_1
 *                              - \c #IO_SENSOR_SUPPLY_2
 *                              - \c #IO_INT_SAFETY_SW_0 (shut off group 0)
 *                              - \c #IO_INT_SAFETY_SW_1 (shut off group 1)
 *                              - \c #IO_INT_SAFETY_SW_2 (shut off group 2)
 *                              - \c #IO_K15 (K15)
 *                              - \c #IO_WAKEUP (WakeUp)
 * \param[out] state        pointer to ubyte1, returns the state of the selected feature, one of:
 *                              - \c #IO_POWER_ON
 *                              - \c #IO_POWER_OFF
 *                              - \c #IO_POWER_ON_5V
 *                              - \c #IO_POWER_ON_6V
 *                              - \c #IO_POWER_ON_7V
 *                              - \c #IO_POWER_ON_8V
 *                              - \c #IO_POWER_ON_9V
 *                              - \c #IO_POWER_ON_10V
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist or is not available
 *                                         on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY             the given channel is not a POWER channel
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    power module has not been initialized
 * \retval #IO_E_NULL_POINTER              a NULL pointer has been passed
 * \retval #IO_E_SAFE_STATE                the given channel is in a safe state
 *
 **************************************************************************
 *
 * \remarks
 *    - \c #IO_INT_POWERSTAGE_ENABLE and \c #IO_INT_SAFETY_SW_0 .. \c #IO_INT_SAFETY_SW_2 are
 *    internal pins.
 *
 * \remarks
 *    - \c #IO_INT_POWERSTAGE_ENABLE controls the internal powerstage enable signal. Without
 *    enabling this signal all power outputs remain low (switched off).
 *
 * \remarks
 *    - \c #IO_INT_SAFETY_SW_0 .. \c #IO_INT_SAFETY_SW_2 control the internal safety switches for
 *    PWM outputs an represent a shut off group. Without switching this signal to ON, the PWM power
 *    stages will not be supplied and the pin will therefore remain low. The safety switch
 *    assignment to the output stage is defined as following:
 *        - \c #IO_INT_SAFETY_SW_0 (shut off group 0):
 *        \c #IO_PWM_00 .. \c #IO_PWM_13 (\c #IO_DO_16  .. \c #IO_DO_29)
 *        - \c #IO_INT_SAFETY_SW_1 (shut off group 1):
 *        \c #IO_PWM_14 .. \c #IO_PWM_27 (\c #IO_DO_30  .. \c #IO_DO_43)
 *        - \c #IO_INT_SAFETY_SW_2 (shut off group 2):
 *        \c #IO_PWM_28 .. \c #IO_PWM_35 (\c #IO_DO_44  .. \c #IO_DO_51)
 *
 * \remarks
 *    - \c #IO_SENSOR_SUPPLY_0 .. \c #IO_SENSOR_SUPPLY_2 control the 3 external sensor supply pins.
 *    \c #IO_SENSOR_SUPPLY_0 and \c #IO_SENSOR_SUPPLY_1 are fixed to 5 volts. Calling this function
 *    with this pins, the state can only return \c #IO_POWER_ON of \c #IO_POWER_OFF.
 *    \c #IO_SENSOR_SUPPLY_2 is a variable supply and can be configured in the range from 5 to 10
 *    volts. Calling this function with this pin, the state can return \c #IO_POWER_OFF or anything
 *    in between \c #IO_POWER_ON_5V and \c #IO_POWER_ON_10V.
 *
 *************************************************************************************************/
IO_ErrorType IO_POWER_Get(ubyte1 pin,
                          ubyte1 * const state);

/**********************************************************************************************//**
 *
 * \brief Configures the external shut off inputs
 *
 *   Configures and enables the external shut off inputs for a shut off group
 *
 * \param group             Group for which the external shut off inputs should be configured, one
 *                          of:
 *                              - \c #IO_INT_SAFETY_SW_0 (shut off group 0)
 *                              - \c #IO_INT_SAFETY_SW_1 (shut off group 1)
 *                              - \c #IO_INT_SAFETY_SW_2 (shut off group 2)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist or is not available
 *                                         on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY             the given channel is not a valid shut off group
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    power module has not been initialized
 * \retval #IO_E_CHANNEL_BUSY              the external shut off inputs were already configured or
 *                                         the associated ADC input channels are already used by
 *                                         another function
 * \retval #IO_E_UNKNOWN                   an unknown error occurred
 *
 **************************************************************************
 *
 * \note
 *   The external shut off functionality requires 2 analog inputs for each group. The following
 *   pins are used for each group:
 *       - \c #IO_INT_SAFETY_SW_0 (shut off group 0):
 *           - \c #IO_ADC_18 (input 0)
 *           - \c #IO_ADC_19 (input 1)
 *       - \c #IO_INT_SAFETY_SW_1 (shut off group 1):
 *           - \c #IO_ADC_20 (input 0)
 *           - \c #IO_ADC_21 (input 1)
 *       - \c #IO_INT_SAFETY_SW_2 (shut off group 2):
 *           - \c #IO_ADC_22 (input 0)
 *           - \c #IO_ADC_23 (input 1)
 *
 * \attention
 *   - Both inputs need to switch in opposite direction, using a normally open and a normally close
 *   contact.
 *   - The outputs can only be enabled, when input 0 reads a high level and input 1 reads a low
 *   level
 *   - The outputs will be disabled, when input 0 reads a low level and input 1 reads a high level
 *   - If both inputs read the same level, the diagnostic state machine will be informed, with the
 *   diagnostic error code being #DIAG_E_SSW_EXT_SHUTOFF.
 *
 * \remarks
 *   When the external shut off is applied, diagnostic is still possible on the affected outputs
 *
 *************************************************************************************************/
IO_ErrorType IO_POWER_EnableExtShutOff(ubyte1 group);

/**********************************************************************************************//**
 *
 * \brief Reads the state of the external shut off signal
 *
 *   Reads the evaluated signal of the external shut off inputs
 *
 * \param group             Group for which the status shall be read, one of:
 *                              - \c #IO_INT_SAFETY_SW_0 (shut off group 0)
 *                              - \c #IO_INT_SAFETY_SW_1 (shut off group 1)
 *                              - \c #IO_INT_SAFETY_SW_2 (shut off group 2)
 * \param[out] state        pointer to bool, returns the state of the shut off group, one of:
 *                              - \c #TRUE if the outputs were disabled by the external shut off
 *                              - \c #FALSE if the outputs were not disabled by the external shut
 *                              off
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist or is not available
 *                                         on the used ECU variant
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the external shut off has not been enabled
 * \retval #IO_E_NULL_POINTER              a NULL pointer has been passed
 * \retval #IO_E_UNKNOWN                   an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_POWER_GetExtShutOff(ubyte1 group,
                                    bool * const state);

#ifdef __cplusplus
}
#endif

#endif /* IO_POWER_H_ */
