/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_PWD.h
 *
 * \latexonly \hypertarget{IO_PWD}{} \endlatexonly
 *
 * \brief IO Driver functions for timer input channels.
 *
 *  Contains all service functions for the PWD (Pulse Width Demodulation). There are three groups
 *  of timer inputs available:
 *
 *  - \c #IO_PWD_00..\c #IO_PWD_05:
 *    * Complex mode:
 *         Can be configured to measure frequency and pulse-width at the same time. Additionally, a
 *         pull up/down interface can be configured. These inputs can accumulate up to 8 pulse
 *         samples. An average value is calculated automatically, but all samples are available on
 *         demand. These inputs support voltage and current signals (7mA/14mA). For current signals
 *         an additional range check is done.
 *    * Incremental mode:
 *         Can be configured to read incremental (relative) encoders. In this case two inputs are
 *         reserved for one incremental encoder (clock and direction) interface. Additionally, a
 *         pull up/down interface can be configured. The incremental interface will decrement when
 *         the 1st channel is leading and increment when the 2nd channel is leading. These inputs
 *         support voltage signals only.
 *    * Count mode:
 *         Can be configured to count rising, falling or both edges. Additionally, a pull up/down
 *         interface can be configured. These inputs support voltage signals only.
 *
 *    * Universal mode:
 *         Can be configured as a combination of complex, incremental and count mode.
 *
 *  - \c #IO_PWD_06..\c #IO_PWD_11:
 *    * Complex mode:
 *         Can be configured to measure frequency and pulse-width at the same time. Additionally, a
 *         pull up/down interface can be configured. These inputs can accumulate up to 8 pulse
 *         samples. An average value is calculated automatically. The different pulse samples
 *         cannot be gathered. These inputs support voltage signals only.
 *    * Incremental mode:
 *         Can be configured to read incremental (relative) encoders. In this case two inputs are
 *         reserved for one incremental encoder (clock and direction) interface. Additionally, a
 *         pull up/down interface can be configured. The incremental interface will decrement when
 *         the 1st channel is leading and increment when the 2nd channel is leading. These inputs
 *         support voltage signals only.
 *    * Count mode:
 *         Can be configured to count rising, falling or both edges. Additionally, a pull up/down
 *         interface can be configured. These inputs support voltage signals only.
 *
 *  - \c #IO_PWD_12..\c #IO_PWD_19:
 *    * Complex mode:
 *         Can be configured to measure frequency and pulse-width at the same time. However, these
 *         inputs do not accumulate a number of samples. Instead every pulse sample has to be
 *         handled directly. These inputs support voltage signals only.
 *
 *  PWD-API Usage:
 *    - \ref pwd_examples "Examples for PWD API functions"
 *
 * \section pwd_input_protection PWD input protection
 *
 * Each PWD input that is configured for complex mode in combination with a current sensor is
 * individually protected against overcurrent. Whenever two consecutive samples above 54326uA
 * are measured, the input protection is enabled latest within 22ms.
 *
 * When entering the protection state, the PWD input has to remain in this state for at least
 * 1s. After this wait time the PWD input can be reenabled via function \c
 * IO_PWD_ResetProtection(). Note that the number of reenabling operations for a single PWD
 * input is limited to 10. Refer to function \c IO_PWD_ResetProtection() for more information on
 * how to reenable a PWD input.
 *
 * \attention
 *  PWD input protection is only applicable to PWD channels \c #IO_PWD_00 .. \c #IO_PWD_05 when
 *  configured for complex mode in combination with current sensors.
 *
 * \section pwd_examples PWD code examples
 * \brief Examples for using the PWD API
 * \subsection pwd_init_examples PWD initialization example
 * \code
 *      // setup complex timer input (frequency and pulse measurement) for a voltage signal
 *      IO_PWD_ComplexInit(IO_PWD_00,               // timer input channel
 *                         IO_PWD_HIGH_TIME,        // measure high time
 *                         IO_PWD_FALLING_VAR,      // select falling edge as variable
 *                         8,                       // set number of accumulations
 *                         IO_PWD_PU_10K,           // select pull up resistor
 *                         NULL);                   // not safety critical
 * \endcode
 *
 * \subsection pwd_task_examples PWD task example
 * \code
 *      ubyte4 freq_8_val;
 *      ubyte4 pulse_8_val;
 *      bool pin_value;
 *
 *      // read complex timer values (frequency and pulse value
 *      IO_PWD_ComplexGet(IO_PWD_00,
 *                        &freq_8_val,
 *                        &pulse_8_val,
 *                        &pin_value,     // read digital pin value
 *                        NULL);          // pulse samples not needed
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_PWD_H_
#define IO_PWD_H_

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
 * \name High/low time
 * \anchor pwd_htlt_def
 *
 * Specifies whether the high,low or both(period) time shall be captured
 *
 * @{
 */
#define IO_PWD_LOW_TIME                 0U   /**< capture the low time of the input signal */
#define IO_PWD_HIGH_TIME                1U   /**< capture the high time of the input signal */
#define IO_PWD_PERIOD_TIME              2U   /**< capture the high and low time of the input
                                                  signal */
/** @} */

/**
 * \name Variable edge
 * \anchor pwd_var_edge_def
 *
 * Specify the variable edge of the input signal. \n
 * If the rising edge is variable, the frequency is measured between
 * the surrounding falling edges.
 *
 * @{
 */
#define IO_PWD_RISING_VAR               2U   /**< rising edge of the input signal is the variable
                                                  one */
#define IO_PWD_FALLING_VAR              3U   /**< falling edge of the input signal is the variable
                                                  one */
/** @} */

/**
 * \name Pull up / Pull down configuration for timer inputs
 * \anchor pwd_th_volt_def
 *
 * Configuration of the pull up or pull down resistors on the timer inputs. These defines can be
 * used for the \c pupd parameter of the functions \c IO_PWD_ComplexInit(), \c IO_PWD_IncInit() and
 * \c IO_PWD_CountInit().
 *
 * @{
 */
#define IO_PWD_NO_PULL                  0x03U    /**< fixed pull resistor */
#define IO_PWD_PU_10K                   0x01U    /**< 10 kOhm pull up */
#define IO_PWD_PD_10K                   0x00U    /**< 10 kOhm pull down */
#define IO_PWD_PD_90                    0x02U    /**< 90 Ohm pull down */
/** @} */

/**
 * \name PWD maximum pulse pulse-width samples
 * \anchor pwd_max_pulsewidth_def
 *
 * Maximum pulse-width samples that can be stored in the data structure \c #IO_PWD_PULSE_SAMPLES
 *
 * @{
 */
#define IO_PWD_MAX_PULSE_SAMPLES        8U
/** @} */

/**
 * \name Counting mode
 * \anchor pwd_count_mode_def
 *
 * Specify the counting mode of a incremental interface.
 *
 * @{
 */
#define IO_PWD_INC_2_COUNT              0x03U    /**< count on any edge of the two input channels */
#define IO_PWD_INC_1_COUNT              0x01U    /**< count on any edge of 1st input channel only:
                                                      - count on edges of \c #IO_PWD_00 for 1st incremental interface
                                                      - count on edges of \c #IO_PWD_02 for 2nd incremental interface
                                                      - count on edges of \c #IO_PWD_04 for 3rd incremental interface
                                                      - count on edges of \c #IO_PWD_06 for 4th incremental interface
                                                      - count on edges of \c #IO_PWD_08 for 5th incremental interface
                                                      - count on edges of \c #IO_PWD_10 for 6th incremental interface */
/** @} */

/**
 * \name edge count
 * \anchor pwd_count_edge_def
 *
 * Specify on which edge shall be counted
 *
 * @{
 */
#define IO_PWD_RISING_COUNT                1U   /**< count on a rising edge */
#define IO_PWD_FALLING_COUNT               2U   /**< count on a falling edge */
#define IO_PWD_BOTH_COUNT                  3U   /**< count on both edges */
/** @} */

/**
 * \name count direction
 * \anchor pwd_count_dir_def
 *
 * Specify the counting direction
 *
 * @{
 */
#define IO_PWD_UP_COUNT                    0U   /**< count up */
#define IO_PWD_DOWN_COUNT                  1U   /**< count down */
/** @} */

/**************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/

/**
 *
 * \brief Safety configuration for the Incremental or Counter PWD inputs
 *
 *  Stores all relevant safety configuration parameters for the Incremental or counter PWD inputs.
 *  The internal checker modules verify that this input is in the valid range.
 *
 */
typedef struct io_pwd_inc_safety_conf_
{
    ubyte2 pwd_cnt_val_lower;           /**< Lower PWD counter limit [1..65534] */
    ubyte2 pwd_cnt_val_upper;           /**< Upper PWD counter limit [1..65534] */
} IO_PWD_INC_SAFETY_CONF;

/**
 *
 * \brief PWD pulse-width data structure
 *
 *  stores each captured pulse-width for one measurement.
 *
 */
typedef struct io_pwd_pulse_samples_
{
    ubyte1 pulse_samples_count;                     /**< number of pulse_samples */
    ubyte4 pulse_sample[IO_PWD_MAX_PULSE_SAMPLES];  /**< stores each captured pulse-width for
                                                         one measurement */
} IO_PWD_PULSE_SAMPLES;

/**
 *
 * \brief Safety configuration for the Complex PWD inputs
 *
 *  Stores all relevant safety configuration parameters for the Complex PWD inputs. The internal
 *  checker modules verify that this input is in the valid range.
 */
typedef struct io_pwd_cplx_safety_conf_
{
    ubyte4 pwd_freq_val_lower;           /**< Lower PWD frequency limit in mHz [100..20000000] */
    ubyte4 pwd_freq_val_upper;           /**< Upper PWD frequency limit in mHz [100..20000000] */
    ubyte4 pwd_pulse_val_lower;          /**< Lower PWD pulse limit in us [20..10000000] */
    ubyte4 pwd_pulse_val_upper;          /**< Upper PWD pulse limit in us [20..10000000] */
} IO_PWD_CPLX_SAFETY_CONF;

/**
 * \brief Complex configuration for the Universal PWD inputs
 *
 * Stores all relevant configuration parameters for the complex mode of
 * Universal PWD inputs.
 */
typedef struct io_pwd_cplx_conf_
{
    ubyte1 pulse_mode;          /**< Specifies the pulse measurement mode
                                 *   - \c #IO_PWD_HIGH_TIME:   configuration to measure pulse-high-time
                                 *   - \c #IO_PWD_LOW_TIME:    configuration to measure pulse-low-time
                                 *   - \c #IO_PWD_PERIOD_TIME: configuration to measure period
                                 */
    ubyte1 freq_mode;           /**< Specifies the variable edge
                                 *   - \c #IO_PWD_RISING_VAR:  rising edge is variable this means,
                                 *                             frequency is measured on falling edges
                                 *   - \c #IO_PWD_FALLING_VAR: falling edge is variable this means,
                                 *                             frequency is measured on rising edges
                                 */
    ubyte1 capture_count;       /**< Number of frequency/pulse-width measurements that will be
                                 *   accumulated [1..8]
                                 */
} IO_PWD_CPLX_CONF;

/**
 * \brief Edge counter configuration for the Universal PWD inputs
 *
 * Stores all relevant configuration parameters for the edge counting mode of
 * Universal PWD inputs.
 */
typedef struct io_pwd_cnt_conf_
{
    ubyte1 mode;                /**< Defines the behavior of the edge counter
                                 *   - \c #IO_PWD_RISING_COUNT:  count on a rising edge
                                 *   - \c #IO_PWD_FALLING_COUNT: count on a falling edge
                                 *   - \c #IO_PWD_BOTH_COUNT:    count on a both edges
                                 */
    ubyte1 direction;           /**< Specifies the counting direction
                                 *   - \c #IO_PWD_UP_COUNT:      count up
                                 *   - \c #IO_PWD_DOWN_COUNT:    count down
                                 */
    ubyte2 init;                /**< Initial value of the edge counter [0..65535] */
} IO_PWD_CNT_CONF;

/**
 * \brief Incremental configuration for the Universal PWD inputs
 *
 * Stores all relevant configuration parameters for the incremental mode of
 * Universal PWD inputs.
 */
typedef struct io_pwd_inc_conf_
{
    ubyte1 mode;                /**< Defines the behavior of the incremental counter
                                 *   - \c #IO_PWD_INC_2_COUNT: Counts up/down on any edge of the two
                                 *                             input channels
                                 *   - \c #IO_PWD_INC_1_COUNT: Counts up/down on any edge of the 1st
                                 *                             input channel only:
                                 *                             - count on \c #IO_PWD_00 for 1st incremental interface
                                 *                             - count on \c #IO_PWD_02 for 2nd incremental interface
                                 *                             - count on \c #IO_PWD_04 for 3rd incremental interface
                                 */
    ubyte2 init;                /**< Initial value of the incremental counter [0..65535] */
} IO_PWD_INC_CONF;

/**
 *
 * \brief Safety configuration for the Universal PWD inputs
 *
 *  Stores all relevant safety configuration parameters for the Universal PWD inputs. The internal
 *  checker modules verify that this input is in the valid range.
 */
typedef struct io_pwd_universal_safety_conf_
{
    const IO_PWD_INC_SAFETY_CONF *pwd_inc_safety_conf;      /**< Safety configuration for the incremental timer mode. */
    const IO_PWD_INC_SAFETY_CONF *pwd_cnt_safety_conf;      /**< Safety configuration for the edge counting timer mode. */
    const IO_PWD_CPLX_SAFETY_CONF *pwd_cplx_safety_conf;    /**< Safety configuration for the complex timer mode. */
} IO_PWD_UNIVERSAL_SAFETY_CONF;

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Setup single timer channel that measures frequency and pulse-width at the same time
 *
 * \param timer_channel     Timer channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 *                              - \c #IO_PWD_12 .. \c #IO_PWD_19
 * \param pulse_mode        Specifies the pulse mode
 *                              - \c #IO_PWD_HIGH_TIME:   configuration to measure pulse-high-time
 *                              - \c #IO_PWD_LOW_TIME:    configuration to measure pulse-low-time
 *                              - \c #IO_PWD_PERIOD_TIME: configuration to measure pulse-high and
 *                                   low-time (Period)
 * \param freq_mode         Specifies the variable edge
 *                              - \c #IO_PWD_RISING_VAR:  rising edge is variable this means,
 *                                   frequency is measured on falling edges
 *                              - \c #IO_PWD_FALLING_VAR: falling edge is variable this means,
 *                                   frequency is measured on rising edges
 * \param capture_count     Number of frequency/pulse-width measurements that will be accumulated
 *                          (1..8)
 * \param pupd              Pull up/down interface:
 *                              - \c #IO_PWD_NO_PULL:    fixed pull resistor
 *                              - \c #IO_PWD_PU_10K:     Pull up 10 kOhm
 *                              - \c #IO_PWD_PD_10K:     Pull down 10 kOhm
 *                              - \c #IO_PWD_PD_90:      Pull down 90 Ohm (for 7mA/14mA sensors)
 * \param[in] safety_conf   Relevant safety configurations for the checker modules
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_BUSY                the channel is currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID          the given channel id does not exist or is not
 *                                           available on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY               the given channel is not a PWD channel or the used ECU
 *                                           variant does not support this function
 * \retval #IO_E_INVALID_PARAMETER           invalid parameter has been passed
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      the common driver init function has not been called
 *                                           before
 * \retval #IO_E_FPGA_NOT_INITIALIZED        the FPGA has not been initialized
 * \retval #IO_E_INVALID_SAFETY_CONFIG       an invalid safety configuration has been passed
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  global safety configuration is missing
 * \retval #IO_E_DRV_SAFETY_CYCLE_RUNNING    the init function was called after the task begin
 *                                           function
 * \retval #IO_E_INTERNAL_CSM                an internal error occurred
 * \retval #IO_E_UNKNOWN                     an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    - The supported features depend on the selected channel:
 *        - \c #IO_PWD_00 .. \c #IO_PWD_05:
 *             - \c pulse_mode:    \c #IO_PWD_HIGH_TIME, \c #IO_PWD_LOW_TIME or
 *               \c #IO_PWD_PERIOD_TIME
 *             - \c freq_mode:     \c #IO_PWD_RISING_VAR or \c #IO_PWD_FALLING_VAR
 *             - \c capture_count: 1..8
 *             - \c pupd:          \c #IO_PWD_PU_10K, \c #IO_PWD_PD_10K or \c #IO_PWD_PD_90
 *             - \c safety_conf:   Safety configuration
 *        - \c #IO_PWD_06 .. \c #IO_PWD_11:
 *             - \c pulse_mode:    \c #IO_PWD_HIGH_TIME, \c #IO_PWD_LOW_TIME or
 *               \c #IO_PWD_PERIOD_TIME
 *             - \c freq_mode:     \c #IO_PWD_RISING_VAR or \c #IO_PWD_FALLING_VAR
 *             - \c capture_count: 1..8
 *             - \c pupd:          \c #IO_PWD_PU_10K or \c #IO_PWD_PD_10K
 *        - \c #IO_PWD_12 .. \c #IO_PWD_19:
 *             - \c pulse_mode:    \c #IO_PWD_HIGH_TIME, \c #IO_PWD_LOW_TIME or
 *               \c #IO_PWD_PERIOD_TIME
 *             - \c freq_mode:     \c #IO_PWD_RISING_VAR or \c #IO_PWD_FALLING_VAR
 *
 * \attention
 *   Passing \c #IO_PWD_PD_90 as \c pupd parameter configures the input for a current sensor
 *   (7mA/14mA). In this case an additional range check on the current signal will be performed.
 *
 * \remarks
 *    - A channel that is initialized with this function can retrieve frequency and duty cycle by
 *    calling the function: \c IO_PWD_ComplexGet()
 *
 * \remarks
 *    - The timing measurement for channels \c #IO_PWD_00 .. \c #IO_PWD_05 is based on a 27bit
 *    timer with a resolution of 0.5 us, therefore the period that shall be measured must be
 *    smaller than 67.108.863 us (~ 67 s).
 *
 *    - The timing measurement for channels \c #IO_PWD_06 .. \c #IO_PWD_11 is hardware limited to
 *    10.000.000 us (= 10 s) with a resolution of 0.5 us. Because those timers work in an
 *    accumulating fashion the product of the measured period and the \c capture_count must be
 *    below this time.
 *
 *    - The timing measurement for channels \c #IO_PWD_12 .. \c #IO_PWD_19 is hardware limited to
 *    10.000.000 us (= 10 s) with a resolution of 1 us.
 *
 * \remarks
 *    - The maximum frequency that can be measured with \c #IO_PWD_00 .. \c #IO_PWD_11 is 20kHz.
 *    - The maximum frequency that can be measured with \c #IO_PWD_12 .. \c #IO_PWD_19 is 10kHz.
 *
 * \remarks
 *    - The driver captures exactly as many measurements are given in the parameter capture_count,
 *    except for \c #IO_PWD_12 .. \c #IO_PWD_19.
 *
 * \note
 *   Until not all configured samples are captured, the driver doesn't return a value.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_ComplexInit(ubyte1 timer_channel,
                                ubyte1 pulse_mode,
                                ubyte1 freq_mode,
                                ubyte1 capture_count,
                                ubyte1 pupd,
                                IO_PWD_CPLX_SAFETY_CONF const * const safety_conf);

/**********************************************************************************************//**
 *
 * \brief Get the frequency and the pulse-width from the specified timer channel
 *
 * \param timer_channel         Timer channel:
 *                                  - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                                  - \c #IO_PWD_06 .. \c #IO_PWD_11
 *                                  - \c #IO_PWD_12 .. \c #IO_PWD_19
 * \param[out] frequency        Accumulated frequency in mHz (1/1000 Hz)
 * \param[out] pulse_width      Accumulated pulse-width in us
 * \param[out] pin_value        Digital value of the pin
 * \param[out] pulse_samples    Contains each pulse-width measure sample
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_PWD_NOT_FINISHED        not enough edges to accumulate a result
 * \retval #IO_E_PWD_OVERFLOW            a timer overflow occurred
 * \retval #IO_E_PWD_CURRENT_THRESH_HIGH the last measured current signal was above the threshold
 *                                       of 20 mA
 * \retval #IO_E_PWD_CURRENT_THRESH_LOW  the last measured current signal was below the threshold
 *                                       of 4 mA
 * \retval #IO_E_FET_PROT_ACTIVE         the input FET protection is active and has deactivated the
 *                                       internal switch. It can be reset with
 *                                       \c IO_PWD_ResetProtection() after the wait time is passed
 * \retval #IO_E_FET_PROT_REENABLE       the input FET protection is ready to be reset with
 *                                       \c IO_PWD_ResetProtection()
 * \retval #IO_E_FET_PROT_PERMANENT      the input FET is protected permanently because it was
 *                                       already reset more than 10 times
 * \retval #IO_E_NULL_POINTER            a NULL pointer has been passed to the function
 * \retval #IO_E_STARTUP                 the input is in the startup phase
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *   The return values \c #IO_E_PWD_CURRENT_THRESH_HIGH, \c #IO_E_PWD_CURRENT_THRESH_LOW,
 *   \c #IO_E_FET_PROT_ACTIVE, \c #IO_E_FET_PROT_REENABLE and \c #IO_E_FET_PROT_PERMANENT will
 *   only be returned in case of a current sensor configuration (\c pupd == \c #IO_PWD_PD_90)
 *
 * \remarks
 *     - The maximum frequency that can be measured with \c #IO_PWD_00 .. \c #IO_PWD_11 is 20kHz.
 *     - The maximum frequency that can be measured with \c #IO_PWD_12 .. \c #IO_PWD_19 is 10kHz.
 *
 * \remarks
 *     - The parameter \c pin_value is optional.  If not needed, the parameter can be set \c #NULL
 *     to ignore them.
 *
 * \remarks
 *     - If each individual measured pulse-width sample is not needed, the parameter
 *     \c pulse_samples should be set to \c #NULL
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_ComplexGet(ubyte1 timer_channel,
                               ubyte4 * const frequency,
                               ubyte4 * const pulse_width,
                               bool * const pin_value,
                               IO_PWD_PULSE_SAMPLES * const pulse_samples);

/**********************************************************************************************//**
 *
 * \brief Deinitializes a complex PWD input
 *
 * \param timer_channel     Timer channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 *                              - \c #IO_PWD_12 .. \c #IO_PWD_19
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_ComplexDeInit(ubyte1 timer_channel);

/**********************************************************************************************//**
 *
 * \brief Setup a single incremental interface
 *
 * \param inc_channel       Channel of the incremental interface:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 * \param mode              Defines the counter behavior
 *                              - \c #IO_PWD_INC_2_COUNT: Counts up/down on any edge of the two
 *                                   input channels
 *                              - \c #IO_PWD_INC_1_COUNT: Counts up/down on any edge of the 1st
 *                                   input channel only:
 *                                   - count on \c #IO_PWD_00 for 1st incremental interface
 *                                   - count on \c #IO_PWD_02 for 2nd incremental interface
 *                                   - count on \c #IO_PWD_04 for 3rd incremental interface
 *                                   - count on \c #IO_PWD_06 for 4th incremental interface
 *                                   - count on \c #IO_PWD_08 for 5th incremental interface
 *                                   - count on \c #IO_PWD_10 for 6th incremental interface
 * \param count_init        Init value of the incremental counter (0..65535)
 * \param pupd              Pull up/down interface:
 *                              - \c #IO_PWD_PU_10K:  Pull up 10 kOhm
 *                              - \c #IO_PWD_PD_10K:  Pull down 10 kOhm
 * \param[in] safety_conf   Relevant safety configurations for the checker modules
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID         the given channel id does not exist or is not available
 *                                          on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY              the given channel is not capable for an incremental
 *                                          interface or the used ECU variant does not support this
 *                                          function
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED     the given channel is not configured
 * \retval #IO_E_CHANNEL_BUSY               the channel is currently used by another function
 * \retval #IO_E_INVALID_PARAMETER          invalid parameter has been passed
 * \retval #IO_E_DRIVER_NOT_INITIALIZED     the common driver init function has not been called
 *                                          before
 * \retval #IO_E_FPGA_NOT_INITIALIZED       the FPGA has not been initialized
 * \retval #IO_E_INVALID_SAFETY_CONFIG      an invalid safety configuration has been passed
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG global safety configuration is missing
 * \retval #IO_E_DRV_SAFETY_CYCLE_RUNNING   the init function was called after the task begin
 *                                          function
 * \retval #IO_E_INTERNAL_CSM               an internal error occurred
 * \retval #IO_E_UNKNOWN                    an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    - The parameter \c safety_conf is only supported for the PWD channels \c #IO_PWD_00 ..
 *    \c #IO_PWD_05
 *
 * \remarks
 *    - A channel that is initialized with this function can retrieve the counter value:
 *    \c IO_PWD_IncGet()
 *
 * \remarks
 *    - Two PWD channels are used (needed) for one incremental interface.\n
 *   The pairs are defined in the following order:
 *        - \c #IO_PWD_00 and \c #IO_PWD_01 define the 1st incremental interface.
 *        - \c #IO_PWD_02 and \c #IO_PWD_03 define the 2nd incremental interface.
 *        - \c #IO_PWD_04 and \c #IO_PWD_05 define the 3rd incremental interface.
 *        - \c #IO_PWD_06 and \c #IO_PWD_07 define the 4th incremental interface.
 *        - \c #IO_PWD_08 and \c #IO_PWD_09 define the 5th incremental interface.
 *        - \c #IO_PWD_10 and \c #IO_PWD_11 define the 6th incremental interface.
 *
 * \note
 *    - The function initializes both PWD channels of the incremental interface, independently if
 *    the function gets called with the 1st or 2nd channel belonging to an incremental interface.
 *
 * \note
 *    - The incremental interface will decrement when the 1st channel is leading and increment when
 *    the 2nd channel is leading.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_IncInit(ubyte1 inc_channel,
                            ubyte1 mode,
                            ubyte2 count_init,
                            ubyte1 pupd,
                            IO_PWD_INC_SAFETY_CONF const * const safety_conf);

/**********************************************************************************************//**
 *
 * \brief Get the counter value of an incremental interface
 *
 * \param inc_channel       Channel of the incremental interface:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 * \param[out] count        Value of the incremental counter (0..65535)
 * \param[out] pin_value_0  Digital value of pin 0 (1st channel)
 * \param[out] pin_value_1  Digital value of pin 1 (2nd channel)
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_NULL_POINTER            a NULL pointer has been passed
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *    - The function can be called with either the 1st or the 2nd channel of the incremental
 *    interface.
 *
 * \note
 *    - The incremental interface will decrement when the 1st channel is leading and increment when
 *    the 2nd channel is leading.
 *
 * \remarks
 *     - The parameters \c pin_value_0 and \c pin_value_1 are optional. If not needed, these
 *     parameters can be set \c #NULL to ignore them.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_IncGet(ubyte1 inc_channel,
                           ubyte2 * const count,
                           bool * const pin_value_0,
                           bool * const pin_value_1);

/**********************************************************************************************//**
 *
 * \brief Set the counter value of an incremental interface
 *
 * \param inc_channel       Channel of the incremental interface:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 * \param count             Value of the incremental counter (0..65535)
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *    - The function can be called with either the 1st or the 2nd channel of the incremental
 *    interface.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_IncSet(ubyte1 inc_channel,
                           ubyte2 count);

/**********************************************************************************************//**
 *
 * \brief Deinitialize an incremental interface
 *
 * \param inc_channel       Channel of the incremental interface:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *    - The function can be called with either the 1st or the 2nd channel of the incremental
 *    interface.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_IncDeInit(ubyte1 inc_channel);

/**********************************************************************************************//**
 *
 * \brief Setup a single counter channel
 *
 * \param count_channel     Counter channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 * \param mode              Specify on which edge shall be count
 *                              - \c #IO_PWD_RISING_COUNT:  count on a rising edge
 *                              - \c #IO_PWD_FALLING_COUNT: count on a falling edge
 *                              - \c #IO_PWD_BOTH_COUNT:    count on a both edges
 * \param direction         Specify the counting direction
 *                              - \c #IO_PWD_UP_COUNT:      counts up
 *                              - \c #IO_PWD_DOWN_COUNT:    counts down
 * \param count_init        Init value of the counter (0..65535)
 * \param pupd              Pull up/down interface:
 *                              - \c #IO_PWD_PU_10K:  Pull up 10 kOhm
 *                              - \c #IO_PWD_PD_10K:  Pull down 10 kOhm
 * \param[in] safety_conf   Relevant safety configurations for the checker modules
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_BUSY                the channel is currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID          the given channel id does not exist or is not
 *                                           available on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY               the given channel is not a PWD channel or the used ECU
 *                                           variant does not support this function
 * \retval #IO_E_INVALID_PARAMETER           invalid parameter has been passed
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      the common driver init function has not been called
 *                                           before
 * \retval #IO_E_FPGA_NOT_INITIALIZED        the FPGA has not been initialized
 * \retval #IO_E_INVALID_SAFETY_CONFIG       an invalid safety configuration has been passed
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  global safety configuration is missing
 * \retval #IO_E_DRV_SAFETY_CYCLE_RUNNING    the init function was called after the task begin
 *                                           function
 * \retval #IO_E_INTERNAL_CSM                an internal error occurred
 * \retval #IO_E_UNKNOWN                     an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    - The parameter \c safety_conf is only supported for the PWD channels \c #IO_PWD_00 ..
 *    \c #IO_PWD_05
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_CountInit(ubyte1 count_channel,
                              ubyte1 mode,
                              ubyte1 direction,
                              ubyte2 count_init,
                              ubyte1 pupd,
                              IO_PWD_INC_SAFETY_CONF const * const safety_conf);

/**********************************************************************************************//**
 *
 * \brief Get the counter value of a counter channel
 *
 * \param count_channel     Counter channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 * \param[out] count        Value of the counter (0..65535)
 * \param[out] pin_value    Digital value of the pin
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_NULL_POINTER            a NULL pointer has been passed
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    The parameter \c pin_value is optional.\n
 *    If not needed, the parameter can be set \c #NULL to ignore them.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_CountGet(ubyte1 count_channel,
                             ubyte2 * const count,
                             bool * const pin_value);

/**********************************************************************************************//**
 *
 * \brief Set the counter value of a counter channel
 *
 * \param count_channel     Counter channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 * \param count             Value of the counter (0..65535)
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_CountSet(ubyte1 count_channel,
                             ubyte2 count);

/**********************************************************************************************//**
 *
 * \brief Deinitialize a single counter channel
 *
 * \param count_channel     Counter channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_CountDeInit(ubyte1 count_channel);

/**********************************************************************************************//**
 *
 * \brief Setup a single universal timer channel
 *
 * \param timer_channel     Timer channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 * \param[in] cplx_conf     Complex configuration
 * \param[in] cnt_conf      Edge counter configuration
 * \param[in] inc_conf      Incremental counter configuration
 * \param pupd              Pull up/down interface:
 *                              - \c #IO_PWD_PU_10K:     Pull up 10 kOhm
 *                              - \c #IO_PWD_PD_10K:     Pull down 10 kOhm
 *                              - \c #IO_PWD_PD_90:      Pull down 90 Ohm (for 7mA/14mA sensors)
 * \param[in] safety_conf   Relevant safety configurations for the checker modules
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_BUSY                the channel is currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID          the given channel id does not exist or is not
 *                                           available on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY               the given channel is not a PWD channel or the used ECU
 *                                           variant does not support this function
 * \retval #IO_E_INVALID_PARAMETER           invalid parameter has been passed
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
 * \attention
 *   Passing \c #IO_PWD_PD_90 as \c pupd parameter configures the input for a current sensor
 *   (7mA/14mA). In this case an additional range check on the current signal will be performed.
 *   Note that this setting is invalid if the input is also configured for edge and/or incremental
 *   counter mode.
 *
 * \attention
 *   If a channel is configured for incremental mode, both the primary and secondary channel are
 *   redundantly configured using the provided complex, edge counter and safety configuration.
 *   Thus, the application must periodically call \c IO_PWD_UniversalGet() for both channels if
 *   either the complex or edge counter mode is configured for safety.
 *
 * \remarks
 *     - The supported features depend on the selected channel:
 *         - \c #IO_PWD_00 .. \c #IO_PWD_05:
 *             - \c pulse_mode:    \c #IO_PWD_HIGH_TIME, \c #IO_PWD_LOW_TIME or
 *                                 \c #IO_PWD_PERIOD_TIME
 *             - \c freq_mode:     \c #IO_PWD_RISING_VAR or \c #IO_PWD_FALLING_VAR
 *             - \c capture_count: 1..8
 *             - \c pupd:          \c #IO_PWD_PU_10K, \c #IO_PWD_PD_10K or \c #IO_PWD_PD_90
 *             - \c safety_conf:   Safety configuration
 *
 * \remarks
 *     - Parameter \c safety_conf can be set to NULL in order to initialize the input as
 *       non-safety-critical. To configure the input as safety-critical, parameter
 *       \c safety_conf must provide exactly one valid (i.e., not NULL) safety configuration.
 *       Providing zero or multiple safety configurations or a safety configuration for a mode
 *       that is not configured will fail with return code #IO_E_INVALID_SAFETY_CONFIG.
 *
 * \remarks
 *     - A channel that is initialized with this function can retrieve its measurement results by
 *       calling the function: \c IO_PWD_UniversalGet()
 *
 * \remarks
 *    - The timing measurement for channels \c #IO_PWD_00 .. \c #IO_PWD_05 is based on a 27bit
 *    timer with a resolution of 0.5 us, therefore the period that shall be measured must be
 *    smaller than 67.108.863 us (~ 67 s).
 *
 * \remarks
 *     - The maximum frequency that can be measured with \c #IO_PWD_00 .. \c #IO_PWD_05 is 20kHz.
 *
 * \note
 *     - For complex mode, the driver doesn't return a value as long as not all configured samples
 *       are captured.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_UniversalInit(ubyte1 timer_channel,
                                  IO_PWD_CPLX_CONF const * const cplx_conf,
                                  IO_PWD_CNT_CONF const * const cnt_conf,
                                  IO_PWD_INC_CONF const * const inc_conf,
                                  ubyte1 pupd,
                                  IO_PWD_UNIVERSAL_SAFETY_CONF const * const safety_conf);

/**********************************************************************************************//**
 *
 * \brief Get the measurement results from the specified universal timer channel
 *
 * \param timer_channel             Timer channel:
 *                                      - \c #IO_PWD_00 .. \c #IO_PWD_05
 * \param[out] frequency            Accumulated frequency in mHz (1/1000 Hz)
 * \param[out] pulse_width          Accumulated pulse-width in us
 * \param[out] pulse_samples        Contains each pulse-width measure sample
 * \param[out] edge_count           Value of the edge counter (0..65535)
 * \param[out] inc_count            Value of the incremental counter (0..65535)
 * \param[out] primary_pin_value    Digital value of the primary pin
 * \param[out] secondary_pin_value  Digital value of the secondary pin
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_PWD_NOT_FINISHED        not enough edges to accumulate a result
 * \retval #IO_E_PWD_OVERFLOW            a timer overflow occurred
 * \retval #IO_E_PWD_CURRENT_THRESH_HIGH the last measured current signal was above the threshold
 *                                       of 20 mA
 * \retval #IO_E_PWD_CURRENT_THRESH_LOW  the last measured current signal was below the threshold
 *                                       of 4 mA
 * \retval #IO_E_FET_PROT_ACTIVE         the input FET protection is active and has deactivated the
 *                                       internal switch. It can be reset with
 *                                       \c IO_PWD_ResetProtection() after the wait time is passed
 * \retval #IO_E_FET_PROT_REENABLE       the input FET protection is ready to be reset with
 *                                       \c IO_PWD_ResetProtection()
 * \retval #IO_E_FET_PROT_PERMANENT      the input FET is protected permanently because it was
 *                                       already reset more than 10 times
 * \retval #IO_E_NULL_POINTER            a NULL pointer has been passed to the function
 * \retval #IO_E_STARTUP                 the input is in the startup phase
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *     - The function can be called with either the primary or the secondary channel of the
 *       incremental interface.
 *
 * \note
 *     - The return values \c #IO_E_PWD_CURRENT_THRESH_HIGH, \c #IO_E_PWD_CURRENT_THRESH_LOW,
 *       \c #IO_E_FET_PROT_ACTIVE, \c #IO_E_FET_PROT_REENABLE and \c #IO_E_FET_PROT_PERMANENT will
 *       only be returned in case of a current sensor configuration (\c pupd == \c #IO_PWD_PD_90)
 *
 * \remarks
 *     - The maximum frequency that can be measured with \c #IO_PWD_00 .. \c #IO_PWD_05 is 20kHz.
 *
 * \remarks
 *     - The parameters \c frequency, \c pulse_width, \c pulse_samples, \c edge_count, \c inc_count
 *       \c primary_pin_value and \c secondary_pin_value are optional. If not needed, these
 *       parameters can be set to \c #NULL to ignore them.
 *
 * \remarks
 *     - If each individual measured pulse-width sample is not needed, the parameter
 *       \c pulse_samples should be set to \c #NULL
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_UniversalGet(ubyte1 timer_channel,
                                 ubyte4 * const frequency,
                                 ubyte4 * const pulse_width,
                                 IO_PWD_PULSE_SAMPLES * const pulse_samples,
                                 ubyte2 * const edge_count,
                                 ubyte2 * const inc_count,
                                 bool * const primary_pin_value,
                                 bool * const secondary_pin_value);

/**********************************************************************************************//**
 *
 * \brief Set the counter values of an universal timer channel
 *
 * \param timer_channel     Timer channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 * \param[in] edge_count    Value of the edge counter (0..65535)
 * \param[in] inc_count     Value of the incremental counter (0..65535)
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *     - When configured for incremental mode, the function can be called with either the
 *       primary or the secondary channel.
 *
 * \remarks
 *     - The parameters \c edge_count and \c inc_count are optional. If not needed, these
 *       parameters can be set to \c #NULL to ignore them.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_UniversalSet(ubyte1 timer_channel,
                                 ubyte2 const * const edge_count,
                                 ubyte2 const * const inc_count);

/**********************************************************************************************//**
 *
 * \brief Deinitialize a single universal timer channel
 *
 * \param timer_channel     Timer channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *
 * \return #IO_ErrorType:
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_UniversalDeInit(ubyte1 timer_channel);

/**********************************************************************************************//**
 *
 * \brief Get the voltage feedback of a timer input channel
 *
 * \param pwd_channel       PWD channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 *                              - \c #IO_PWD_06 .. \c #IO_PWD_11
 * \param[out] voltage      Measured voltage in mV. Range: 0..32000  (0V..32.000V)
 * \param[out] fresh        Indicates if a new value is available since the last call.
 *                              - \c #TRUE: Value in "voltage" is valid
 *                              - \c #FALSE: No new value available.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_NULL_POINTER           a NULL pointer has been passed to the function
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PWD channel
 * \retval #IO_E_STARTUP                the input is in the startup phase
 * \retval #IO_E_CHANNEL_BUSY           the channel is currently used by another function
 * \retval #IO_E_REFERENCE              the internal reference voltage is out of range
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *   The return value \c #IO_E_CHANNEL_BUSY will only be returned in case of a current sensor
 *   configuration (\c pupd == \c #IO_PWD_PD_90)
 *
 * \remarks
 *   If there is no new voltage value available (for example the function \c IO_PWD_GetVoltage()
 *   gets called more frequently than the AD sampling) the flag \c fresh will be set to \c #FALSE.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_GetVoltage(ubyte1 pwd_channel,
                               ubyte2 * const voltage,
                               bool * const fresh);

/**********************************************************************************************//**
 *
 * \brief Get the current feedback of a timer input channel
 *
 * \param pwd_channel       PWD channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 * \param[out] current      Measured current in uA. Range: 0..20000  (0mA..20.000mA)
 * \param[out] fresh        Indicates if a new value is available since the last call.
 *                              - \c #TRUE: Value in "current" is valid
 *                              - \c #FALSE: No new value available.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_NULL_POINTER            a NULL pointer has been passed to the function
 * \retval #IO_E_PWD_CURRENT_THRESH_HIGH the last measured current signal was above the threshold
 *                                       of 20 mA
 * \retval #IO_E_PWD_CURRENT_THRESH_LOW  the last measured current signal was below the threshold
 *                                       of 4 mA
 * \retval #IO_E_FET_PROT_ACTIVE         the input FET protection is active and has deactivated the
 *                                       internal switch. It can be  reset with
 *                                       \c IO_PWD_ResetProtection() after the wait time is passed
 * \retval #IO_E_FET_PROT_REENABLE       the input FET protection is ready to be reset with
 *                                       \c IO_PWD_ResetProtection()
 * \retval #IO_E_FET_PROT_PERMANENT      the input FET is protected permanently because it was
 *                                       already reset more than 10 times
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           the given channel is not a PWD channel
 * \retval #IO_E_STARTUP                 the input is in the startup phase
 * \retval #IO_E_CHANNEL_BUSY            the channel is currently used by another function
 * \retval #IO_E_REFERENCE               the internal reference voltage is out of range
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *   The return value \c #IO_E_CHANNEL_BUSY will only be returned in case of a voltage sensor
 *   configuration (\c pupd != \c #IO_PWD_PD_90)
 *
 * \remarks
 *   If there is no new current value available (for example the function \c IO_PWD_GetCurrent()
 *   gets called more frequently than the AD sampling) the flag \c fresh will be set to \c #FALSE.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_GetCurrent(ubyte1 pwd_channel,
                               ubyte2 * const current,
                               bool * const fresh);

/**********************************************************************************************//**
 *
 * \brief Reset the FET protection for a timer input channel
 *
 * \param pwd_channel       PWD channel:
 *                              - \c #IO_PWD_00 .. \c #IO_PWD_05
 * \param[out] reset_cnt    Protection reset counter. Indicates how often the application already
 *                          reset the protection.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_FET_PROT_NOT_ACTIVE     no input FET protection is active
 * \retval #IO_E_FET_PROT_PERMANENT      the input FET is protected permanently because it was
 *                                       already reset more than 10 times
 * \retval #IO_E_FET_PROT_WAIT           the input FET protection can not be reset, as the wait
 *                                       time of 1s is not already passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY           the given channel is not a PWD channel
 * \retval #IO_E_CHANNEL_BUSY            the channel is currently used by another function
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *   The return value \c #IO_E_CHANNEL_BUSY will only be returned in case of a voltage sensor
 *   configuration (\c pupd != \c #IO_PWD_PD_90)
 *
 * \remarks
 *   When re-enabling the channel after it has been in protection state, it will do a transition
 *   to the startup state. Therefore the functions \c #IO_PWD_ComplexGet() and
 *   \c #IO_PWD_GetCurrent() will return \c #IO_E_STARTUP for a certain time.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWD_ResetProtection(ubyte1 pwd_channel,
                                    ubyte1 * const reset_cnt);

#ifdef __cplusplus
}
#endif

#endif /* IO_PWD_H_ */
