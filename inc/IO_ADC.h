/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_ADC.h
 *
 * \latexonly \hypertarget{IO_ADC}{} \endlatexonly
 *
 * \brief IO Driver functions for ADC.
 * \details
 *  Contains all service functions for the ADC. There are three groups of ADC inputs:
 *
 *  - 3 mode ADC:
 *   * 5V ADC inputs.
 *   * Up to 8 ADC inputs for 0-5V measurement (#IO_ADC_00..#IO_ADC_07).
 *   * Can be configured as resistive, current, absolute or ratiometric input. An additional
 *     reference channel or a sensor supply measurement (for the ratiometric case) will be
 *     configured and serves to correct the ADC signal.
 *   * Range: 0-5V
 *
 *  - 2 mode 10V ADC:
 *   * Software configurable ADC inputs.
 *   * Up to 8 ADC inputs for 0-10V measurement (#IO_ADC_08..#IO_ADC_15).
 *   * Can be configured as current, absolute or ratiometric input. An additional reference channel
 *     or a sensor supply measurement (for the ratiometric case) will be configured and serves to
 *     correct the ADC signal.
 *   * Range: can be configured to 0-5V or 0-10V for absolute and ratiometric measurement.
 *
 *  - 2 mode 32V ADC:
 *   * Software configurable ADC inputs.
 *   * Up to 8 ADC inputs for 0-32V measurement (#IO_ADC_16..#IO_ADC_23).
 *   * Can be configured as current, absolute or ratiometric input. An additional reference channel
 *     or a sensor supply measurement (for the ratiometric case) will be configured and serves to
 *     correct the ADC signal.
 *   * Range: can be configured to 0-5V or 0-32V for absolute and ratiometric measurement.
 *
 *  - Normal ADC:
 *   * Various ADC inputs for retrieving onboard voltages (UBat, Sensor-Supply, Board-Temperature,
 *     ...)
 *
 * \note
 *  The ADC channels will be sampled every 2ms. After this time a new measurement is available and
 *  will be flagged as fresh via the parameter \c fresh of IO_ADC_Get().
 *
 * \attention
 *  For #IO_ADC_RATIOMETRIC mode the following configuration options are recommended:
 *  - 3 mode ADC: (#IO_ADC_00..#IO_ADC_07) with #IO_ADC_SENSOR_SUPPLY_0 or #IO_ADC_SENSOR_SUPPLY_1.
 *  - 3 mode ADC: (#IO_ADC_00..#IO_ADC_07) with #IO_ADC_SENSOR_SUPPLY_2 (sensor supply voltage setting 5V ... 10V).
 *  - 2 mode 10V ADC: (#IO_ADC_08..#IO_ADC_15) configured in #IO_ADC_RANGE_5V with #IO_ADC_SENSOR_SUPPLY_0 or
 *  #IO_ADC_SENSOR_SUPPLY_1.
 *  - 2 mode 10V ADC: (#IO_ADC_08..#IO_ADC_15) configured in #IO_ADC_RANGE_5V range with #IO_ADC_SENSOR_SUPPLY_2
 *  (sensor supply voltage setting 5V ... 10V)
 *  - 2 mode 10V ADC: (#IO_ADC_08..#IO_ADC_15) configured in #IO_ADC_RANGE_10V range with #IO_ADC_SENSOR_SUPPLY_2
 *  (sensor supply voltage setting 6V ... 10V).
 *
 * \attention
 *  The precision of the remaining configuration options in #IO_ADC_RATIOMETRIC mode for reading a ratiometric
 *  sensor connected to one of the sensor supplies is limited by the hardware design and
 *  cannot be significantly improved by selecting the ratiometric mode.
 *  <B>The configuration options not listed above are NOT recommended</B>.
 *
 *  ADC-API Usage:
 *    - \ref adc_examples "Examples for ADC API functions"
 *
 * \section adc_input_protection ADC input protection
 *
 * Each ADC input that is configured for current measurement mode is individually protected against
 * overcurrent. Whenever two consecutive samples above 27234uA are measured, the input protection is
 * enabled latest within 22ms.
 *
 * When entering the protection state, the ADC input has to remain in this state for at least
 * 1s. After this wait time the ADC input can be reenabled via function \c
 * IO_ADC_ResetProtection(). Note that the number of reenabling operations for a single ADC
 * input is limited to 10. Refer to function \c IO_ADC_ResetProtection() for more information on
 * how to reenable an ADC input.
 *
 * \attention
 *  ADC input protection is only applicable to ADC channels \c #IO_ADC_00 .. \c #IO_ADC_23 when
 *  configured for current measurement mode.
 *
 * \section adc_examples ADC Code Examples
 * \brief Examples for using the ADC API
 * \subsection adc_init_examples Example for ADC initialization
 * \code
 *      // 3 mode ADC:
 *      IO_ADC_ChannelInit(IO_ADC_00,
 *                         IO_ADC_RATIOMETRIC,         // ratiometric configuration
 *                         IO_ADC_NO_RANGE,
 *                         IO_ADC_NO_PULL,
 *                         IO_SENSOR_SUPPLY_0,         // sensor supply 0 is used
 *                         NULL);                      // not safety critical
 *
 *      // 2 mode 10V ADC:
 *      IO_ADC_ChannelInit(IO_ADC_08,
 *                         IO_ADC_ABSOLUTE,            // absolute configuration
 *                         IO_ADC_RANGE_10V,           // 10V configuration
 *                         IO_ADC_NO_PULL,
 *                         IO_PIN_NONE,
 *                         NULL);                      // not safety critical
 *
 *      // Normal ADC:
 *      IO_ADC_ChannelInit(IO_ADC_UBAT,
 *                         IO_ADC_ABSOLUTE,
 *                         IO_ADC_RANGE_32V,
 *                         IO_ADC_NO_PULL,
 *                         IO_PIN_NONE,
 *                         NULL);                      // not safety critical
 * \endcode
 *
 * \subsection adc_task_examples Example for ADC task function call
 * \details This function call is identical for every type of ADC inputs.
 * \code
 *      ubyte4 adc_val_20;
 *      bool adc_fresh_20;
 *      IO_ErrorType rc;
 *
 *      rc = IO_ADC_Get(IO_ADC_20,
 *                      &adc_val_20,
 *                      &adc_fresh_20);
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_ADC_H_
#define IO_ADC_H_

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
 * \name Types of configurable Analog inputs
 * \anchor adc_config
 *
 * Input configuration for configurable ADC inputs. These defines can be used for the \c type
 * parameter of the function \c IO_ADC_ChannelInit().
 *
 * @{
 */
/** Ratiometric configuration \n
 *  use this configuration if the connected sensor is supplied by one of the sensor supplies
 *  (\c #IO_SENSOR_SUPPLY_0 or \c #IO_SENSOR_SUPPLY_1 or \c #IO_SENSOR_SUPPLY_2) and delivers a
 *  voltage signal. \n
 *  Task function returns voltage in [mV]
 */
#define IO_ADC_RATIOMETRIC          0x00U
/** Current loop configuration \n
 *  use this configuration if the connected sensor delivers a current signal (4..25mA sensors) \n
 *  Task function returns current in [uA]
 */
#define IO_ADC_CURRENT              0x01U
/** Resistive configuration \n
 *  use this configuration if the sensor value shall be determined by measuring its resistance \n
 *  Task function returns resistance in [Ohm]
 */
#define IO_ADC_RESISTIVE            0x02U
/** Absolute voltage measurement \n
 *  use this configuration to measure an absolute voltage signal \n
 *  Task function returns voltage in [mV]
 */
#define IO_ADC_ABSOLUTE             0x03U
/** @} */

/**
 * \name Pull up / Pull down configuration for ADC inputs
 * \anchor adc_pupd_settings
 *
 * Configuration of the pull up or pull down resistors on the ADC inputs. These defines can be used
 * for the \c pupd parameter of the function \c IO_ADC_ChannelInit().
 *
 * @{
 */
#define IO_ADC_NO_PULL              0x00U       /**< fixed pull resistor */
#define IO_ADC_PU_10K               0x02U       /**< 10 kOhm pull up */
#define IO_ADC_PD_10K               0x01U       /**< 10 kOhm pull down */
/** @} */

/**
 * \name Range configuration for ADC inputs
 * \anchor adc_range
 *
 * Configuration of the ADC input range. These defines can be used for the \c range parameter of
 * the function \c IO_ADC_ChannelInit().
 *
 * @{
 */
#define IO_ADC_NO_RANGE             0x00U       /**< ADC range not configurable */
#define IO_ADC_RANGE_5V             0x01U       /**< ADC range 0V .. 5V */
#define IO_ADC_RANGE_10V            0x02U       /**< ADC range 0V .. 10V */
#define IO_ADC_RANGE_32V            0x03U       /**< ADC range 0V .. 32V */
/** @} */

/**************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/

/**
 *
 * \brief Safety configuration for the ADC inputs
 *
 *  Stores all relevant safety configuration parameters for the ADC inputs. The internal checker
 *  modules verify that this inputs contain valid values
 *
 * \attention
 * For the safety configuration of a 2 mode ADC channel, the following rules need to be fulfilled:
 * - The primary channel needs to specify a redundant channel (\c #IO_ADC_08 .. \c #IO_ADC_23) at
 *      the \c redundant_channel field.
 * - The redundant channel must not reference a redundant channel by itself. Thus \c #IO_PIN_NONE
 *      shall be used for the redundant channel.
 * - Every redundant channel can only be used once as redundant channel.
 * - It's not allowed to have a redundant channel which is not used by a primary channel.
 * - If the ratiometric measurement mode is used, the primary and the redundant channel must use
 *      different sensor supplies (but not \c #IO_SENSOR_SUPPLY_2).
 *
 * \attention
 * For the safety configuration of a 3 mode ADC channel, the following rules need to be fulfilled:
 * - The channel must have \c #IO_PIN_NONE in the \c redundant_channel field.
 * - If the ratiometric measurement mode is used, the channel must not use \c #IO_SENSOR_SUPPLY_2.
 */
typedef struct io_adc_safety_conf_
{
    ubyte1 adc_val_lower;           /**< Lower ADC limit in % [4..96] */
    ubyte1 adc_val_upper;           /**< Upper ADC limit in % [4..96] */
    ubyte1 redundant_channel;       /**< Redundant channel for 2 mode inputs. */
} IO_ADC_SAFETY_CONF;

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Setup one ADC channel
 *
 * \param adc_channel       ADC channel, one of:
 *                              - \c #IO_ADC_00 .. \c #IO_ADC_07
 *                              - \c #IO_ADC_08 .. \c #IO_ADC_15
 *                              - \c #IO_ADC_16 .. \c #IO_ADC_23
 *                              - \c #IO_ADC_24 .. \c #IO_ADC_35
 *                              - \c #IO_ADC_36 .. \c #IO_ADC_43
 *                              - \c #IO_ADC_44 .. \c #IO_ADC_51
 *                              - \c #IO_ADC_52 .. \c #IO_ADC_59
 *                              - \c #IO_ADC_SENSOR_SUPPLY_0
 *                              - \c #IO_ADC_SENSOR_SUPPLY_1
 *                              - \c #IO_ADC_SENSOR_SUPPLY_2
 *                              - \c #IO_ADC_K15
 *                              - \c #IO_ADC_WAKE_UP
 *                              - \c #IO_ADC_UBAT
 *                              - \c #IO_ADC_SAFETY_SW_0
 *                              - \c #IO_ADC_SAFETY_SW_1
 *                              - \c #IO_ADC_SAFETY_SW_2
 *                              - \c #IO_ADC_BOARD_TEMP
 * \param type              Type of input:
 *                              - \c #IO_ADC_RATIOMETRIC: voltage input on sensor supply
 *                              - \c #IO_ADC_CURRENT:     0-25mA input
 *                              - \c #IO_ADC_RESISTIVE:   0-100000Ohm input
 *                              - \c #IO_ADC_ABSOLUTE:    normal voltage input
 * \param range             Range of input:
 *                              - \c #IO_ADC_NO_RANGE:    voltage input with fixed range
 *                              - \c #IO_ADC_RANGE_5V:    voltage input 0-5000mV
 *                              - \c #IO_ADC_RANGE_10V:   voltage input 0-10200mV
 *                              - \c #IO_ADC_RANGE_32V:   voltage input 0-32000mV
 * \param pupd              Pull up/down configuration:
 *                              - \c #IO_ADC_NO_PULL:     fixed pull resistor
 *                              - \c #IO_ADC_PU_10K:      pull up 10 kOhm
 *                              - \c #IO_ADC_PD_10K:      pull down 10 kOhm
 * \param sensor_supply     Sensor supply:
 *                              - \c #IO_SENSOR_SUPPLY_0: 5V
 *                              - \c #IO_SENSOR_SUPPLY_1: 5V
 *                              - \c #IO_SENSOR_SUPPLY_2: variable
 *                              - \c #IO_PIN_NONE:        no sensor supply is used
 * \param[in] safety_conf   Relevant safety configurations for the checker modules. The
 *                          following ADC channels can be configured safety relevant:
 *                              - \c #IO_ADC_00 .. \c #IO_ADC_07
 *                              - \c #IO_ADC_08 .. \c #IO_ADC_15
 *                              - \c #IO_ADC_16 .. \c #IO_ADC_23
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID          the given channel id does not exist or is not
 *                                           available on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY               the given channel is not an ADC channel or the used
 *                                           ECU variant does not support this function
 * \retval #IO_E_INVALID_PARAMETER           parameter is out of range
 * \retval #IO_E_CHANNEL_BUSY                the ADC input channel is currently used by another
 *                                           function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      the common driver init function has not been called
 *                                           before
 * \retval #IO_E_INVALID_SAFETY_CONFIG       an invalid safety configuration has been passed
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  global safety configuration is missing
 * \retval #IO_E_DRV_SAFETY_CYCLE_RUNNING    the init function was called after the task begin
 *                                           function
 * \retval #IO_E_UNKNOWN                     an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    - The supported features depend on the selected channel:
 *        - \c #IO_ADC_00 .. \c #IO_ADC_07:
 *             - \c type:          \c #IO_ADC_RATIOMETRIC, \c #IO_ADC_CURRENT,
 *                                 \c #IO_ADC_RESISTIVE or \c #IO_ADC_ABSOLUTE
 *             - \c sensor_supply: \c #IO_SENSOR_SUPPLY_0, \c #IO_SENSOR_SUPPLY_1 or
 *                                 \c #IO_SENSOR_SUPPLY_2
 *             - \c safety_conf:   Safety configuration
 *        - \c #IO_ADC_08 .. \c #IO_ADC_15:
 *             - \c type:          \c #IO_ADC_RATIOMETRIC, \c #IO_ADC_CURRENT or
 *                                 \c #IO_ADC_ABSOLUTE
 *             - \c range:         \c #IO_ADC_RANGE_5V or \c #IO_ADC_RANGE_10V (supported for type
 *                                 \c #IO_ADC_ABSOLUTE and \c #IO_ADC_RATIOMETRIC)
 *             - \c sensor_supply: \c #IO_SENSOR_SUPPLY_0, \c #IO_SENSOR_SUPPLY_1 or
 *                                 \c #IO_SENSOR_SUPPLY_2
 *             - \c safety_conf:   Safety configuration
 *        - \c #IO_ADC_16 .. \c #IO_ADC_23:
 *             - \c type:          \c #IO_ADC_RATIOMETRIC, \c #IO_ADC_CURRENT or
 *                                 \c #IO_ADC_ABSOLUTE
 *             - \c range:         \c #IO_ADC_RANGE_5V or \c #IO_ADC_RANGE_32V (supported for type
 *                                 \c #IO_ADC_ABSOLUTE and \c #IO_ADC_RATIOMETRIC)
 *             - \c sensor_supply: \c #IO_SENSOR_SUPPLY_0, \c #IO_SENSOR_SUPPLY_1 or
 *                                 \c #IO_SENSOR_SUPPLY_2
 *             - \c safety_conf:   Safety configuration
 *        - \c #IO_ADC_24 .. \c #IO_ADC_35
 *             - \c type:          \c #IO_ADC_RATIOMETRIC or \c #IO_ADC_ABSOLUTE
 *             - \c sensor_supply: \c #IO_SENSOR_SUPPLY_0, \c #IO_SENSOR_SUPPLY_1 or
 *                                 \c #IO_SENSOR_SUPPLY_2 (supported for type
 *                                 \c #IO_ADC_RATIOMETRIC)
 *             - \c pupd:          \c #IO_ADC_PU_10K or \c #IO_ADC_PD_10K
 *        - \c #IO_ADC_36 .. \c #IO_ADC_43
 *             - \c type:          \c #IO_ADC_RATIOMETRIC or \c #IO_ADC_ABSOLUTE
 *             - \c sensor_supply: \c #IO_SENSOR_SUPPLY_0, \c #IO_SENSOR_SUPPLY_1 or
 *                                 \c #IO_SENSOR_SUPPLY_2 (supported for type
 *                                 \c #IO_ADC_RATIOMETRIC)
 *             - \c pupd:          \c #IO_ADC_PU_10K or \c #IO_ADC_PD_10K
 *        - \c #IO_ADC_44 .. \c #IO_ADC_51
 *             - \c type:          \c #IO_ADC_RATIOMETRIC or \c #IO_ADC_ABSOLUTE
 *             - \c sensor_supply: \c #IO_SENSOR_SUPPLY_0, \c #IO_SENSOR_SUPPLY_1 or
 *                                 \c #IO_SENSOR_SUPPLY_2 (supported for type
 *                                 \c #IO_ADC_RATIOMETRIC)
 *        - \c #IO_ADC_52 .. \c #IO_ADC_59
 *             - \c type:          \c #IO_ADC_RATIOMETRIC or \c #IO_ADC_ABSOLUTE
 *             - \c sensor_supply: \c #IO_SENSOR_SUPPLY_0, \c #IO_SENSOR_SUPPLY_1 or
 *                                 \c #IO_SENSOR_SUPPLY_2 (supported for type
 *                                 \c #IO_ADC_RATIOMETRIC)
 *             - \c pupd:          \c #IO_ADC_PU_10K or \c #IO_ADC_PD_10K
 *        - \c #IO_ADC_SENSOR_SUPPLY_0:
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_SENSOR_SUPPLY_1:
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_SENSOR_SUPPLY_2:
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_K15
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_WAKE_UP
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_UBAT
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_SAFETY_SW_0
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_SAFETY_SW_1
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_SAFETY_SW_2
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *        - \c #IO_ADC_BOARD_TEMP
 *             - \c type:          \c #IO_ADC_ABSOLUTE
 *
 * \remarks
 *    - If a channel does not support a function, the value of the associated parameter will be
 *    ignored.
 *
 * \attention
 *   For safety critical configuration (\c safety_conf != \c #NULL) following restrictions
 *   additionally apply:
 *    - Resistive measurement type \c #IO_ADC_RESISTIVE is not valid
 *    - Variable sensor supply \c #IO_SENSOR_SUPPLY_2 is not valid
 *
 * \note
 *   If a channel is configured as safety critical and a sensor supply was configured, also the
 *   sensor supply voltage will be checked by the diagnostic modules.
 *
 *************************************************************************************************/
IO_ErrorType IO_ADC_ChannelInit(ubyte1 adc_channel,
                                ubyte1 type,
                                ubyte1 range,
                                ubyte1 pupd,
                                ubyte1 sensor_supply,
                                const IO_ADC_SAFETY_CONF * const safety_conf);

/**********************************************************************************************//**
 *
 * \brief Deinitializes one ADC input
 *
 * deinitializes the given ADC channel, allows reconfiguration by \c IO_ADC_ChannelInit()
 *
 * \param adc_channel           ADC channel, one of:
 *                                       - \c #IO_ADC_00 .. \c #IO_ADC_07
 *                                       - \c #IO_ADC_08 .. \c #IO_ADC_15
 *                                       - \c #IO_ADC_16 .. \c #IO_ADC_23
 *                                       - \c #IO_ADC_24 .. \c #IO_ADC_35
 *                                       - \c #IO_ADC_36 .. \c #IO_ADC_43
 *                                       - \c #IO_ADC_44 .. \c #IO_ADC_51
 *                                       - \c #IO_ADC_52 .. \c #IO_ADC_59
 *                                       - \c #IO_ADC_SENSOR_SUPPLY_0
 *                                       - \c #IO_ADC_SENSOR_SUPPLY_1
 *                                       - \c #IO_ADC_SENSOR_SUPPLY_2
 *                                       - \c #IO_ADC_K15
 *                                       - \c #IO_ADC_WAKE_UP
 *                                       - \c #IO_ADC_UBAT
 *                                       - \c #IO_ADC_SAFETY_SW_0
 *                                       - \c #IO_ADC_SAFETY_SW_1
 *                                       - \c #IO_ADC_SAFETY_SW_2
 *                                       - \c #IO_ADC_BOARD_TEMP
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY           the given channel is not an ADC channel
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_DRIVER_NOT_INITIALIZED  the common driver init function has not been called before
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_ADC_ChannelDeInit(ubyte1 adc_channel);

/**********************************************************************************************//**
 *
 * \brief Returns the value of the given ADC channel
 *
 * \param adc_channel           ADC channel, one of:
 *                                       - \c #IO_ADC_00 .. \c #IO_ADC_07
 *                                       - \c #IO_ADC_08 .. \c #IO_ADC_15
 *                                       - \c #IO_ADC_16 .. \c #IO_ADC_23
 *                                       - \c #IO_ADC_24 .. \c #IO_ADC_35
 *                                       - \c #IO_ADC_36 .. \c #IO_ADC_43
 *                                       - \c #IO_ADC_44 .. \c #IO_ADC_51
 *                                       - \c #IO_ADC_52 .. \c #IO_ADC_59
 *                                       - \c #IO_ADC_SENSOR_SUPPLY_0
 *                                       - \c #IO_ADC_SENSOR_SUPPLY_1
 *                                       - \c #IO_ADC_SENSOR_SUPPLY_2
 *                                       - \c #IO_ADC_K15
 *                                       - \c #IO_ADC_WAKE_UP
 *                                       - \c #IO_ADC_UBAT
 *                                       - \c #IO_ADC_SAFETY_SW_0
 *                                       - \c #IO_ADC_SAFETY_SW_1
 *                                       - \c #IO_ADC_SAFETY_SW_2
 *                                       - \c #IO_ADC_BOARD_TEMP
 * \param[out] adc_value    ADC value, the range depends on the input group and its configuration
 *                          (\c type parameter of IO_ADC_ChannelInit() ):
 *                              - \c #IO_ADC_00 .. \c #IO_ADC_07
 *                                  - \c #IO_ADC_RATIOMETRIC:           0..5000   (0V..5.000V)
 *                                  - \c #IO_ADC_CURRENT:               0..25000  (0mA..25.000mA)
 *                                  - \c #IO_ADC_RESISTIVE:             0..100000 (0Ohm..100000Ohm)
 *                                  - \c #IO_ADC_ABSOLUTE:              0..5000   (0V..5.000V)
 *                              - \c #IO_ADC_08 .. \c #IO_ADC_15
 *                                  - \c #IO_ADC_RATIOMETRIC:           0..5000   (0V..5.000V)
 *                                  - \c #IO_ADC_CURRENT:               0..25000  (0mA..25.000mA)
 *                                  - \c #IO_ADC_ABSOLUTE:              0..10200  (0V..10.200V)
 *                              - \c #IO_ADC_16 .. \c #IO_ADC_23
 *                                  - \c #IO_ADC_RATIOMETRIC:           0..5000   (0V..5.000V)
 *                                  - \c #IO_ADC_CURRENT:               0..25000  (0mA..25.000mA)
 *                                  - \c #IO_ADC_ABSOLUTE:              0..32000  (0V..32.000V)
 *                              - \c #IO_ADC_24 .. \c #IO_ADC_35
 *                                  - \c #IO_ADC_RATIOMETRIC:           0..32000  (0V..32.000V)
 *                                  - \c #IO_ADC_ABSOLUTE:              0..32000  (0V..32.000V)
 *                              - \c #IO_ADC_36 .. \c #IO_ADC_43
 *                                  - \c #IO_ADC_RATIOMETRIC:           0..32000  (0V..32.000V)
 *                                  - \c #IO_ADC_ABSOLUTE:              0..32000  (0V..32.000V)
 *                              - \c #IO_ADC_44 .. \c #IO_ADC_51
 *                                  - \c #IO_ADC_RATIOMETRIC:           0..32000  (0V..32.000V)
 *                                  - \c #IO_ADC_ABSOLUTE:              0..32000  (0V..32.000V)
 *                              - \c #IO_ADC_52 .. \c #IO_ADC_59
 *                                  - \c #IO_ADC_RATIOMETRIC:           0..32000  (0V..32.000V)
 *                                  - \c #IO_ADC_ABSOLUTE:              0..32000  (0V..32.000V)
 *                              - \c #IO_ADC_SENSOR_SUPPLY_0
 *                                  - \c #IO_ADC_ABSOLUTE:              0..5254   (0V..5.254V)
 *                              - \c #IO_ADC_SENSOR_SUPPLY_1
 *                                  - \c #IO_ADC_ABSOLUTE:              0..5254   (0V..5.254V)
 *                              - \c #IO_ADC_SENSOR_SUPPLY_2
 *                                  - \c #IO_ADC_ABSOLUTE:              0..10560  (0V..10.560V)
 *                              - \c #IO_ADC_K15
 *                                  - \c #IO_ADC_ABSOLUTE:              0..55000  (0V..55.000V)
 *                              - \c #IO_ADC_WAKE_UP
 *                                  - \c #IO_ADC_ABSOLUTE:              0..55000  (0V..55.000V)
 *                              - \c #IO_ADC_UBAT
 *                                  - \c #IO_ADC_ABSOLUTE:              0..55000  (0V..55.000V)
 *                              - \c #IO_ADC_SAFETY_SW_0
 *                                  - \c #IO_ADC_ABSOLUTE:              0..55000  (0V..55.000V)
 *                              - \c #IO_ADC_SAFETY_SW_1
 *                                  - \c #IO_ADC_ABSOLUTE:              0..55000  (0V..55.000V)
 *                              - \c #IO_ADC_SAFETY_SW_2
 *                                  - \c #IO_ADC_ABSOLUTE:              0..55000  (0V..55.000V)
 *                              - \c #IO_ADC_BOARD_TEMP
 *                                  - \c #IO_ADC_ABSOLUTE:              3700..25250 (-63.00..152.50
 *                                       degree C)
 * \param[out] fresh        Status of the ADC value
 *                              - \c #TRUE:  ADC value is fresh (channel has been converted)
 *                              - \c #FALSE: ADC value is old (channel has not been converted)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY           the given channel is not an ADC channel
 * \retval #IO_E_STARTUP                 the input is in the startup phase
 * \retval #IO_E_FET_PROT_ACTIVE         the input FET protection is active and has deactivated the
 *                                       internal switch. It can be reset with
 *                                       \c IO_ADC_ResetProtection() after the wait time is passed
 * \retval #IO_E_FET_PROT_REENABLE       the input FET protection is ready to be reset with
 *                                       \c IO_ADC_ResetProtection()
 * \retval #IO_E_FET_PROT_PERMANENT      the input FET is protected permanently because it was
 *                                       already reset more than 10 times
 * \retval #IO_E_REFERENCE               A reference voltage (sensor supply or internal reference) is out of range.
 * \retval #IO_E_NULL_POINTER            a NULL pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_DRIVER_NOT_INITIALIZED  the common driver init function has not been called before
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    - If there is no new measurement value available (for example the function \c IO_ADC_Get()
 *    gets called more frequently than the AD sampling) the flag \c fresh will be set to \c #FALSE.
 *
 * \remarks
 *    - The temperature value in degree C must recalculated in the following manner, since the
 *    value has an offset of 10000 and is multiplied with 100:\n
 *   <b>degree_value = ((float4) adc_value - 10000) / 100</b>\n
 *
 *************************************************************************************************/
IO_ErrorType IO_ADC_Get(ubyte1 adc_channel,
                        ubyte4 * const adc_value,
                        bool * const fresh);

/**********************************************************************************************//**
 *
 * \brief Calculates the board temperature in degree Celsius.
 *
 * The function converts the raw ADC value (retrieved from \c IO_ADC_Get()) to a temperature in
 * degree Celsius and returns it as a float value.
 *
 * \param raw_value         raw adc board temperature returned from the \c IO_ADC_Get() function
 *
 * \return the board temperature in degree Celsius ( -63.00 .. 152.50 degree C )
 *
 **************************************************************************************************
 *
 * \remarks
 *   Usage:
 *    \code
 *        IO_ADC_Get(IO_ADC_BOARD_TEMP,
 *                   &raw_value,
 *                   &fresh);
 *
 *        temp = IO_ADC_BoardTempFloat(raw_value);
 *    \endcode
 *
 *************************************************************************************************/
float4 IO_ADC_BoardTempFloat(ubyte4 raw_value);

/**********************************************************************************************//**
 *
 * \brief Calculates the board temperature in degree Celsius.
 *
 * The function converts the raw ADC value (retrieved from \c IO_ADC_Get()) to a temperature in
 * degree Celsius and returns it as a signed value.
 *
 * \param raw_value         raw adc board temperature returned from the \c IO_ADC_Get() function
 *
 * \return the board temperature in degree Celsius ( -63 .. 152 degree C )
 *
 **************************************************************************************************
 *
 * \remarks
 *   Usage:
 *    \code
 *        IO_ADC_Get(IO_ADC_BOARD_TEMP,
 *                   &raw_value,
 *                   &fresh);
 *
 *        temp = IO_ADC_BoardTempSbyte(raw_value);
 *    \endcode
 *
 *************************************************************************************************/
sbyte2 IO_ADC_BoardTempSbyte(ubyte4 raw_value);

/**********************************************************************************************//**
 *
 * \brief Reset the FET protection for an ADC channel
 *
 * \param adc_channel       ADC channel:
 *                              - \c #IO_ADC_00 .. \c #IO_ADC_07:
 *                                  - \c configured type:   \c #IO_ADC_CURRENT
 *                              - \c #IO_ADC_08 .. \c #IO_ADC_15
 *                                  - \c configured type:   \c #IO_ADC_CURRENT
 *                              - \c #IO_ADC_16 .. \c #IO_ADC_23
 *                                  - \c configured type:   \c #IO_ADC_CURRENT
 * \param[out] reset_cnt    Protection reset counter. Indicates how often the application already
 *                          reset the protection.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_FET_PROT_NOT_ACTIVE    no input FET protection is active
 * \retval #IO_E_FET_PROT_PERMANENT     the input FET is protected permanently because it was
 *                                      already reset more than 10 times
 * \retval #IO_E_FET_PROT_WAIT          the input FET protection can not be reset, as the wait time
 *                                      of 1s is not already passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel has no FET protection
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \attention
 *   The protection can be reset 10 times, afterwards the input will remain permanently protected
 *
 * \note
 *    - The input can only enter a protection state if configured in current mode
 *
 * \note
 *    - After entering the input protection, a certain time has to pass before the input protection
 *    can be reset:
 *       - 1s for \c #IO_ADC_00 .. \c #IO_ADC_07
 *       - 1s for \c #IO_ADC_08 .. \c #IO_ADC_15
 *       - 1s for \c #IO_ADC_16 .. \c #IO_ADC_23
 *
 * \note
 *    - This function will close the internal switch to enable the input back again. After calling
 *    this function new measurement values can be read with \c IO_ADC_Get()
 *
 * \remarks
 *   If the parameter \c reset_cnt is \c #NULL, the parameter is ignored. The parameter
 *   \c reset_cnt returns the number of resets already performed.
 *
 *************************************************************************************************/
IO_ErrorType IO_ADC_ResetProtection(ubyte1 adc_channel,
                                    ubyte1 * const reset_cnt);

#ifdef __cplusplus
}
#endif

#endif /* IO_ADC_H_ */
