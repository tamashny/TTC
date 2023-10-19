/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_DIO.h
 *
 * \latexonly \hypertarget{IO_DIO}{} \endlatexonly
 *
 * \brief IO Driver functions for Digital Input/Output.
 *
 *   Contains all service functions for the digital in/outputs.\n
 *
 * \note
 *   The digital inputs reflect the current status of the input at the point in time where the
 *   function is called.\n
 *
 *   The digital outputs \c #IO_DO_00 .. \c #IO_DO_15 are controlled over SPI shift registers,
 *   therefore the outputs will be periodically updated with a cycle of 1ms.
 *
 *   DIO-API Usage:
 *     - \ref dio_examples "Examples for DIO API functions"
 *
 * \section do_output_protection Digital output protection
 *
 * Each digital output is individually protected against overcurrent. Whenever a fatal overcurrent
 * situation is detected on any safe or unsafe digital output, the output is disabled by software.
 * Note that disabled by software means that the output is set to low but the state of the safety
 * switch (if configured) remains unchanged.
 *
 * When entering the protection state, the digital output has to remain in this state for the given
 * wait time. After this wait time the digital output can be reenabled via function \c
 * IO_DO_ResetProtection(). Note that the number of reenabling operations for a single digital
 * output is limited to 10. Refer to function \c IO_DO_ResetProtection() for more information on
 * how to reenable a digital output.
 *
 * Detailed information about fatal overcurrent situations, the reaction time and wait time are
 * listed in the following table.
 *
 * <table>
 * <caption>Digital output protection overview</caption>
 * <tr><th>Digital output                                                            <th>Fatal overcurrent situation                                        <th>Latest disabled within <th>Wait time
 * <tr><td rowspan="4">\c #IO_DO_00 .. \c #IO_DO_15,<br>\c #IO_DO_52 .. \c #IO_DO_59 <td>3.5A if board temperature    > 85&deg;C and persistent for > 100ms <td rowspan="4">12ms       <td rowspan="4">1s
 * <tr>                                                                              <td>4.7A if board temperature &le; 85&deg;C and persistent for > 100ms
 * <tr>                                                                              <td>5 samples above 7A
 * <tr>                                                                              <td>the switch has already switched off by itself
 * <tr><td rowspan="3">\c #IO_DO_16 .. \c #IO_DO_51                                  <td>3.1A if board temperature    > 85&deg;C and persistent for > 100ms <td rowspan="3">6ms       <td rowspan="3">10s
 * <tr>                                                                              <td>4.1A if board temperature &le; 85&deg;C and persistent for > 100ms
 * <tr>                                                                              <td>1 sample above 5.5A
 * </table>
 *
 * \section dio_examples DIO Code Examples
 * \brief Examples for using the DIO API
 * \subsection dio_init_examples DIO initialization example
 * \code
 *      IO_DI_Init(IO_DI_00,            // digital input
 *                 IO_DI_NO_PULL,       // fixed pull resistor
 *                 NULL);               // no limit configuration (only for analog channels)
 *
 *      IO_DO_Init(IO_DO_18,            // digital output
 *                 FALSE,               // diagnostic disabled
 *                 NULL);               // no safety configuration
 * \endcode
 *
 * \subsection dio_task_examples DIO task function example
 * \code
 *      bool di_val_0;
 *
 *      IO_DI_Get(IO_DI_00,             // read value of digital input
 *                &di_val_0);
 *
 *      IO_DO_Set(IO_DO_18,             // set digital output value
 *                TRUE);
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_DIO_H_
#define IO_DIO_H_

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
 * \name Pull up / Pull down configuration for digital inputs
 * \anchor dio_pupd_settings
 *
 * Configuration of the pull up or pull down resistors on the digital inputs. These defines can be
 * used for the \c pupd parameter of the function \c IO_DI_Init().
 *
 * @{
 */
#define IO_DI_NO_PULL       0x00U    /**< fixed pull resistor */
#define IO_DI_PU_10K        0x01U    /**< 10 kOhm pull up     */
#define IO_DI_PD_10K        0x02U    /**< 10 kOhm pull down   */
/** @} */

/**************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/

/**
 *
 * \brief Voltage limits for digital inputs
 *
 * Contains the thresholds for valid low- and high-levels for digital inputs.\n
 * The range for the low-level is defined by the voltages \c low_thresh1 and \c low_thresh2, where
 * \c low_thresh1 is the lower limit for a low-level and \c low_thresh2 the upper limit.\n
 * The range for the high-level is defined by the voltages \c high_thresh1 and \c high_thresh2,
 * where \c high_thresh1 is the lower limit for a high-level and \c high_thresh2 the upper limit.
 * \n\n
 * The value of \c low_thresh1 must always be smaller than \c low_thresh2 and \c high_thresh1 must
 * always be smaller than \c high_thresh2.\n
 * The value of \c low_thresh2 must always be smaller than \c high_thresh1.
 * \n
 * Examples:
 * \code
 * // voltage limits
 * IO_DIO_LIMITS limits1 = { 0, 2000, 3000, 5000 };
 * \endcode
 *
 * In the above example \c limits1 defines the range 0-2000mV as valid low-level and 3000-5000mV as
 * valid high-level.
 *
 * \note
 * If no limits will be specified by the application, the following default limits will be applied:
 * { 0, 2500, 2500, 32000 }
 *
 */
typedef struct io_dio_limits_
{
    ubyte2 low_thresh1;  /**< Defines the lower voltage limit of valid low signal
                              (0mV ... 32000mV) */
    ubyte2 low_thresh2;  /**< Defines the upper voltage limit of valid low signal
                              (0mV ... 32000mV) */
    ubyte2 high_thresh1; /**< Defines the lower voltage limit of valid high signal
                              (1mV ... 32000mV) */
    ubyte2 high_thresh2; /**< Defines the upper voltage limit of valid high signal
                              (1mV ... 32000mV) */
} IO_DIO_LIMITS;

/**
 *
 * \brief Safety configuration for the digital outputs
 *
 *  Stores all relevant safety configuration parameters for the digital outputs. The internal
 *  checker modules verify that these outputs still work correctly.
 *
 *  \attention
 *     In order to make diagnostics on channels \c #IO_DO_00 .. \c #IO_DO_15 (against open load and
 *     short to VBAT) possible, a delay time of at least 20 ms is needed between transitions of the
 *     output state. If this timing is not fulfilled, a diagnostic error may get raised.
 */
typedef struct io_do_safety_conf_
{
    ubyte1 low_side_channel;       /**< Low side channel. This channel has to be connected together
                                        with the specified high side output channel. The low side
                                        outputs are grouped as follows:
                                         - \c #IO_DO_08 .. \c #IO_DO_15 */
} IO_DO_SAFETY_CONF;

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Setup a digital input
 *
 * \param di_channel        Digital input:
 *                              - \c #IO_DI_00 .. \c #IO_DI_35
 *                              - \c #IO_DI_36 .. \c #IO_DI_47
 *                              - \c #IO_DI_48 .. \c #IO_DI_55
 *                              - \c #IO_DI_56 .. \c #IO_DI_63
 *                              - \c #IO_DI_64 .. \c #IO_DI_71
 *                              - \c #IO_DI_72 .. \c #IO_DI_79
 *                              - \c #IO_DI_80 .. \c #IO_DI_87
 *                              - \c #IO_DI_88 .. \c #IO_DI_95
 * \param pupd              Pull up/down configuration:
 *                              - \c #IO_DI_NO_PULL: fixed pull resistor
 *                              - \c #IO_DI_PU_10K:  Pull up 10 kOhm
 *                              - \c #IO_DI_PD_10K:  Pull down 10 kOhm
 * \param[in] limits        Voltage limits for low/high-levels. If \c #NULL, default limits will be
 *                          used. See #IO_DIO_LIMITS for details.
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist or is not available on
 *                                      the used ECU variant
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a digital input channel or the
 *                                      used ECU variant does not support this function
 * \retval #IO_E_CHANNEL_BUSY           the digital output channel is
 *                                      currently used by another function
 * \retval #IO_E_INVALID_PARAMETER      parameter is out of range
 * \retval #IO_E_INVALID_LIMITS         the given voltage limits are not valid
 * \retval #IO_E_FPGA_NOT_INITIALIZED   the FPGA has not been initialized
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    - The supported features depend on the selected channel:
 *        - \c #IO_DI_00 .. \c #IO_DI_35:
 *             - \c pupd and \c limits ignored
 *        - \c #IO_DI_36 .. \c #IO_DI_47:
 *             - \c pupd:   \c #IO_DI_PU_10K or \c #IO_DI_PD_10K
 *        - \c #IO_DI_48 .. \c #IO_DI_55:
 *             - \c pupd and \c limits ignored
 *        - \c #IO_DI_56 .. \c #IO_DI_63:
 *             - \c limits: Voltage limits for low/high-levels
 *        - \c #IO_DI_64 .. \c #IO_DI_71:
 *             - \c limits: Voltage limits for low/high-levels
 *        - \c #IO_DI_72 .. \c #IO_DI_79:
 *             - \c pupd:   \c #IO_DI_PU_10K or \c #IO_DI_PD_10K
 *             - \c limits: Voltage limits for low/high-levels
 *        - \c #IO_DI_80 .. \c #IO_DI_87:
 *             - \c limits: Voltage limits for low/high-levels
 *        - \c #IO_DI_88 .. \c #IO_DI_95:
 *             - \c pupd:   \c #IO_DI_PU_10K or \c #IO_DI_PD_10K
 *             - \c limits: Voltage limits for low/high-levels
 *
 * \remarks
 *    - The inputs \c #IO_DI_80 .. \c #IO_DI_87 have a fixed pull up. Depending on the parameter
 *    \c limits switches to ground or BAT can be read.
 *
 * \remarks
 *    - The inputs \c #IO_DI_00 .. \c #IO_DI_35, \c #IO_DI_36 .. \c #IO_DI_47 and \c #IO_DI_48 ..
 *    \c #IO_DI_55 have a fixed switching threshold of 2.5V.
 * 
 * \attention
 *   The inputs \c #IO_DI_00 .. \c #IO_DI_35 and \c #IO_DI_48 .. \c #IO_DI_55 have a fixed pull up
 *   and are only suitable for switches to ground.
 *
 * \attention
 *   The inputs \c #IO_DI_56 .. \c #IO_DI_63 and \c #IO_DI_64 .. \c #IO_DI_71 are only suitable for
 *   switches to BAT.
 *
 *************************************************************************************************/
IO_ErrorType IO_DI_Init(ubyte1 di_channel,
                        ubyte1 pupd,
                        const IO_DIO_LIMITS * const limits);

/**********************************************************************************************//**
 *
 * \brief Setup a digital output
 *
 * \param do_channel        Digital output:
 *                              - \c #IO_DO_00 .. \c #IO_DO_07
 *                              - \c #IO_DO_08 .. \c #IO_DO_15
 *                              - \c #IO_DO_16 .. \c #IO_DO_51
 *                              - \c #IO_DO_52 .. \c #IO_DO_59
 * \param diagnostic        Output configuration:
 *                              - \c #TRUE:  diagnostic pull-up enabled
 *                              - \c #FALSE: diagnostic pull-up disabled
 * \param[in] safety_conf   Relevant safety configurations for the checker modules. The following
 *                          DO channels can be configured safety relevant:
 *                              - \c #IO_DO_00 .. \c #IO_DO_07
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID          the given channel id does not exist or is not
 *                                           available on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY               the given channel is not a digital output channel or
 *                                           the used ECU variant does not support this function
 * \retval #IO_E_CHANNEL_BUSY                the digital output channel is
 *                                           currently used by another function
 * \retval #IO_E_CM_CALIBRATION              the zero current calibration failed
 * \retval #IO_E_FPGA_NOT_INITIALIZED        the FPGA has not been initialized
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      the common driver init function has not been called
 *                                           before
 * \retval #IO_E_INVALID_SAFETY_CONFIG       an invalid safety configuration has been passed
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  global safety configuration is missing
 * \retval #IO_E_DRV_SAFETY_CYCLE_RUNNING    the init function was called after the task begin
 *                                           function
 * \retval #IO_E_INTERNAL_CSM                an internal error occurred
 * \retval #IO_E_UNKNOWN                     an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *   The error code \c #IO_E_CM_CALIBRATION is only returned for \c #IO_DO_16 .. \c #IO_DO_51
 *
 * \remarks
 *    - The digital output channels \c #IO_DO_00 .. \c #IO_DO_15 are controlled over SPI shift
 *    registers. Therefore the outputs will be periodically updated with a cycle of 1ms.
 *
 * \remarks
 *    - The digital output channels \c #IO_DO_16 .. \c #IO_DO_51 are an alternative function to
 *    \c #IO_PWM_00 .. \c #IO_PWM_35.
 *
 * \remarks
 *    - The digital output channels \c #IO_DO_52 .. \c #IO_DO_59 are an alternative function to
 *    \c #IO_PVG_00 .. \c #IO_PVG_07.
 *
 * \remarks
 *    - The parameter \c diagnostic is only applied to the channels \c #IO_DO_00 .. \c #IO_DO_07
 *    and \c #IO_DO_52 .. \c #IO_DO_59. If \c diagnostic is \c #TRUE, the output can detect open
 *    load and short circuit. If it is \c #FALSE, the output can not detect open load or short
 *    circuit. Select \c #FALSE for loads with low current consumption like LEDs. With
 *    \c diagnostic == \c #FALSE the pull up will be switched off.
 *
 * \remarks
 *    - If \c safety_conf != \c #NULL, a low side and high side channel have to be connected
 *    together. The internal checker modules check the given channels against the parameter in
 *    \c safety_conf. For more detail about each parameter look on the definition of
 *    \c #IO_DO_SAFETY_CONF
 *
 * \remarks
 *    - If \c safety_conf != \c #NULL, the parameter diagnostic is forced to \c #TRUE to allow
 *    diagnostics
 *
 *************************************************************************************************/
IO_ErrorType IO_DO_Init(ubyte1 do_channel,
                        bool diagnostic,
                        const IO_DO_SAFETY_CONF * const safety_conf);

/**********************************************************************************************//**
 *
 * \brief Deinitializes a digital input
 *
 * \param di_channel        Digital input:
 *                              - \c #IO_DI_00 .. \c #IO_DI_35
 *                              - \c #IO_DI_36 .. \c #IO_DI_47
 *                              - \c #IO_DI_48 .. \c #IO_DI_55
 *                              - \c #IO_DI_56 .. \c #IO_DI_63
 *                              - \c #IO_DI_64 .. \c #IO_DI_71
 *                              - \c #IO_DI_72 .. \c #IO_DI_79
 *                              - \c #IO_DI_80 .. \c #IO_DI_87
 *                              - \c #IO_DI_88 .. \c #IO_DI_95
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                        everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID        the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED    the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY             the given channel is not a digital input channel
 * \retval #IO_E_INVALID_PARAMETER         internally an invalid parameter has been passed
 * \retval #IO_E_UNKNOWN                   an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_DI_DeInit(ubyte1 di_channel);

/**********************************************************************************************//**
 *
 * \brief Deinitializes a digital output
 *
 * \param do_channel        Digital output:
 *                              - \c #IO_DO_00 .. \c #IO_DO_07
 *                              - \c #IO_DO_08 .. \c #IO_DO_15
 *                              - \c #IO_DO_16 .. \c #IO_DO_51
 *                              - \c #IO_DO_52 .. \c #IO_DO_59
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           the given channel is not a digital output channel
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_DO_DeInit(ubyte1 do_channel);

/**********************************************************************************************//**
 *
 * \brief Gets the value of a digital input
 *
 * \param di_channel        Digital input:
 *                              - \c #IO_DI_00 .. \c #IO_DI_35
 *                              - \c #IO_DI_36 .. \c #IO_DI_47
 *                              - \c #IO_DI_48 .. \c #IO_DI_55
 *                              - \c #IO_DI_56 .. \c #IO_DI_63
 *                              - \c #IO_DI_64 .. \c #IO_DI_71
 *                              - \c #IO_DI_72 .. \c #IO_DI_79
 *                              - \c #IO_DI_80 .. \c #IO_DI_87
 *                              - \c #IO_DI_88 .. \c #IO_DI_95
 * \param[out] di_value     Input value:
 *                              - \c #TRUE:  High level
 *                              - \c #FALSE: Low level
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_SHORT_BAT               the measured analog value is above the valid high band
 * \retval #IO_E_SHORT_GND               the measured analog value is below the valid low band
 * \retval #IO_E_INVALID_VOLTAGE         the measured analog value is in between the valid bands
 *                                       for high and low
 * \retval #IO_E_INVALID_CHANNEL_ID      the channel id does not exist
 * \retval #IO_E_NULL_POINTER            NULL pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           the given channel is not a digital input channel
  * \retval #IO_E_STARTUP                the output is in the startup phase
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_REFERENCE               the internal reference voltage is out of range
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *   This function returns the value of the digital input pins at the time when the function is
 *   called for:
 *   - \c #IO_DI_00 .. \c #IO_DI_35
 *   - \c #IO_DI_36 .. \c #IO_DI_47
 *
 * \remarks
 *   This function returns the value of the digital input pins at the time when the last AD value
 *   was sampled for.
 *   - \c #IO_DI_48 .. \c #IO_DI_55
 *   - \c #IO_DI_56 .. \c #IO_DI_63
 *   - \c #IO_DI_64 .. \c #IO_DI_71
 *   - \c #IO_DI_72 .. \c #IO_DI_79
 *   - \c #IO_DI_80 .. \c #IO_DI_87
 *   - \c #IO_DI_88 .. \c #IO_DI_95
 *
 * \remarks
 *   The error codes \c #IO_E_SHORT_BAT, \c #IO_E_SHORT_GND and \c #IO_E_INVALID_VOLTAGE are only
 *   returned for \c #IO_DI_56 .. \c #IO_DI_63, \c #IO_DI_64 .. \c #IO_DI_71, \c #IO_DI_72 ..
 *   \c #IO_DI_79, \c #IO_DI_80 .. \c #IO_DI_87 and \c #IO_DI_88 .. \c #IO_DI_95 and only if
 *   corresponding limits are defined.
 *
 *************************************************************************************************/
IO_ErrorType IO_DI_Get(ubyte1 di_channel,
                       bool * const di_value);

/**********************************************************************************************//**
 *
 * \brief Sets the value of a digital output
 *
 * \param do_channel        Digital output:
 *                              - \c #IO_DO_00 .. \c #IO_DO_07
 *                              - \c #IO_DO_08 .. \c #IO_DO_15
 *                              - \c #IO_DO_16 .. \c #IO_DO_51
 *                              - \c #IO_DO_52 .. \c #IO_DO_59
 * \param do_value          Output value:
 *                              - \c #TRUE:  High level
 *                              - \c #FALSE: Low level
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a digital output channel
 * \retval #IO_E_STARTUP                the output is in the startup phase
 * \retval #IO_E_NO_DIAG                no diagnostic feedback available
 * \retval #IO_E_OPEN_LOAD              open load has been detected
 * \retval #IO_E_SHORT_GND              short circuit has been detected
 * \retval #IO_E_SHORT_BAT              short to battery voltage has been detected
 * \retval #IO_E_OPEN_LOAD_OR_SHORT_BAT open load or short to battery voltage has been detected
 * \retval #IO_E_FET_PROT_ACTIVE        the output FET protection is active and has set the output
 *                                      to low. It can be reset with \c IO_DO_ResetProtection()
 *                                      after the wait time is passed
 * \retval #IO_E_FET_PROT_REENABLE      the output FET protection is ready to be reset with
 *                                      \c IO_DO_ResetProtection()
 * \retval #IO_E_FET_PROT_PERMANENT     the output FET is protected permanently because it was
 *                                      already reset more than 10 times. The output will remain
 *                                      low
 * \retval #IO_E_SAFETY_SWITCH_DISABLED The safety switch of the corresponding output is disabled.
 *                                      The output is currently forced to low.
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
 * \retval #IO_E_SAFE_STATE             the digital output channel is in a safe state
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    - The digital output channels \c #IO_DO_00 .. \c #IO_DO_15 are controlled over SPI shift
 *    registers. Therefore the outputs will be periodically updated with a cycle of 1ms.
 *
 * \remarks
 *    - The digital output channels \c #IO_DO_16 .. \c #IO_DO_51 are an alternative function to
 *    \c #IO_PWM_00 .. \c #IO_PWM_35.
 *
 * \remarks
 *    - The digital output channels \c #IO_DO_52 .. \c #IO_DO_59 are an alternative function to
 *    \c #IO_PWD_00 .. \c #IO_PWD_11.
 *
 *************************************************************************************************/
IO_ErrorType IO_DO_Set(ubyte1 do_channel,
                       bool do_value);

/**********************************************************************************************//**
 *
 * \brief Returns the measured current of a digital output
 *
 * \param do_channel        Digital output:
 *                              - \c #IO_DO_00 .. \c #IO_DO_07
 *                              - \c #IO_DO_08 .. \c #IO_DO_15
 *                              - \c #IO_DO_16 .. \c #IO_DO_51
 *                              - \c #IO_DO_52 .. \c #IO_DO_59
 * \param[out] current      Measured current in mA
 *                          Range: 0..7500 (0A .. 7.500A)
 * \param[out] fresh        Indicates if new values are available since
 *                          the last call.
 *                              - \c #TRUE: Value in "current" is valid
 *                              - \c #FALSE: No new value available.
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_NULL_POINTER            a NULL pointer has been passed to the function
 * \retval #IO_E_CH_CAPABILITY           the given channel is not a digital output channel
 * \retval #IO_E_CM_CALIBRATION          the zero current calibration failed
 * \retval #IO_E_REFERENCE               the internal reference voltage is out of range
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *   The error code \c #IO_E_CM_CALIBRATION is only returned for \c #IO_DO_16 .. \c #IO_DO_51
 *
 * \remarks
 *   If there is no new current value available (for example the function \c IO_DO_GetCur() gets
 *   called more frequently than the AD sampling) the flag \c fresh will be set to \c #FALSE.
 *
 *************************************************************************************************/
IO_ErrorType IO_DO_GetCur(ubyte1 do_channel,
                          ubyte2 * const current,
                          bool * const fresh);

/**********************************************************************************************//**
 *
 * \brief Returns the measured voltage of a digital output
 *
 * \param do_channel        Digital output:
 *                              - \c #IO_DO_00 .. \c #IO_DO_07
 *                              - \c #IO_DO_52 .. \c #IO_DO_59
 * \param[out] voltage      Measured voltage in mV. Range: 0..32000 (0V..32.000V)
 * \param[out] fresh        Indicates if new values are available since the last call.
 *                              - \c #TRUE: Value in "voltage" is valid
 *                              - \c #FALSE: No new value available.
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the channel id does not exist
 * \retval #IO_E_NULL_POINTER            a NULL pointer has been passed to the function
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           the given channel is not a digital output channel
 * \retval #IO_E_REFERENCE               the internal reference voltage is out of range
 *
 **************************************************************************************************
 *
 * \remarks
 *   If there is no new voltage value available (for example the function \c IO_DO_GetVoltage()
 *   gets called more frequently than the AD sampling) the flag \c fresh will be set to \c #FALSE.
 *
 *************************************************************************************************/
IO_ErrorType IO_DO_GetVoltage(ubyte1 do_channel,
                              ubyte2 * const voltage,
                              bool * const fresh);

/**********************************************************************************************//**
 *
 * \brief Reset the output protection for a digital output
 *
 * \param do_channel        Digital output:
 *                              - \c #IO_DO_00 .. \c #IO_DO_07
 *                              - \c #IO_DO_08 .. \c #IO_DO_15
 *                              - \c #IO_DO_16 .. \c #IO_DO_51
 *                              - \c #IO_DO_52 .. \c #IO_DO_59
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
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a digital output channel
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \attention
 *   The protection can be reset 10 times, afterwards the output will remain permanently protected
 *
 * \note
 *    - After entering the output protection, a certain time has to pass before the output
 *    protection can be reset:
 *       - 1s for \c #IO_DO_00 .. \c #IO_DO_07
 *       - 1s for \c #IO_DO_08 .. \c #IO_DO_15
 *       - 1s for \c #IO_DO_52 .. \c #IO_DO_59
 *       - 10s for \c #IO_DO_16 .. \c #IO_DO_51
 *
 * \note
 *    - This function will not set the output back again to high. After calling this function the
 *    output has to be set to the intended level with \c IO_DO_Set()
 *
 * \remarks
 *   If the parameter \c reset_cnt is \c #NULL, the parameter is ignored. The parameter
 *   \c reset_cnt returns the number of resets already performed.
 *
 *************************************************************************************************/
IO_ErrorType IO_DO_ResetProtection(ubyte1 do_channel,
                                   ubyte1 * const reset_cnt);

#ifdef __cplusplus
}
#endif

#endif /* IO_DIO_H_ */
