/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_PWM.h
 *
 * \latexonly \hypertarget{IO_PWM}{} \endlatexonly
 *
 * \brief IO Driver functions for PWM channels.
 *
 * \details
 *  Contains all service functions for the PWM (Pulse Width Modulation). Up to 36 channels can be
 *  configured: \c #IO_PWM_00 .. \c #IO_PWM_35
 *  - All PWM outputs have timer feedback
 *  - All PWM outputs have current measurement
 *  - The PWM outputs will be switched off immediately if the continuous current is above 4A. The
 *  step function for retrieving the current is \c IO_PWM_GetCur() or \c IO_PWM_GetCurQueue().
 *
 *  PWM-API Usage:
 *     - \ref pwm_examples "Examples for PWM API functions"
 *
 * \section pwm_output_protection PWM output protection
 *
 * Each PWM output is individually protected against overcurrent. Whenever a fatal overcurrent
 * situation (i.e., a current greater than 5.5A) is detected on any safe or unsafe PWM output,
 * the output is disabled by software latest within 7ms. Note that disabled by software means
 * that the duty cycle of the output signal is set to 0% but the states of the safety switch and
 * PWM power stage remain unchanged.
 *
 * When entering the protection state, the PWM output has to remain in this state for at least
 * 10s. After this wait time the PWM output can be reenabled via function \c
 * IO_PWM_ResetProtection(). Note that the number of reenabling operations for a single PWM
 * output is limited to 10. Refer to function \c IO_PWM_ResetProtection() for more information on
 * how to reenable a PWM output.
 *
 * \section pwm_examples PWM code examples
 * \brief Examples for using the PWM API
 * \subsection pwm_init_examples PWM initialization example
 * \code
 *      // Setup a PWM output
 *      IO_PWM_Init(IO_PWM_00,
 *                  100,           // frequency is 100 Hz
 *                  TRUE,          // positive polarity
 *                  FALSE,         // no diag margin
 *                  NULL);         // not safety critical
 *
 *      // Setup a PWM output
 *      IO_PWM_Init(IO_PWM_01,
 *                  166,           // frequency is 166 Hz
 *                  TRUE,          // positive polarity
 *                  FALSE,         // no diag margin
 *                  NULL);         // not safety critical
 *
 *      // Setup a PWM output
 *      IO_PWM_Init(IO_PWM_12,
 *                  200,           // frequency is 200 Hz
 *                  TRUE,          // positive polarity
 *                  FALSE,         // no diag margin
 *                  NULL);         // not safety critical
 *
 * \endcode
 *
 * \subsection pwm_task_examples PWM task example
 * \code
 *      ubyte2 curr;
 *      bool   new;
 *
 *      IO_PWM_SetDuty(IO_PWM_00,
 *                     0x8000,     // set duty cycle to 50%
 *                     NULL,       // feedback high time measurement ignored
 *                     NULL);      // feedback period measurement ignored
 *
 *      IO_PWM_GetCur(IO_PWM_00,   // read current value of PWM output
 *                    &curr,       // variable to store current
 *                    &new);       // variable to store information if a current value is available
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_PWM_H_
#define IO_PWM_H_

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
 * \name Size of current queue
 * \anchor pwm_curr_q_def
 *
 * Size of the queue holding the measured values of the equidistant
 * current measurement
 *
 * @{
 */
#define IO_PWM_CURRENT_QUEUE_MAX        6U    /**< maximum number of items in the current queue */
/** @} */


/**************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/

/**
 *
 * \brief Safety configuration for the PWM outputs
 *
 *  Stores all relevant safety configuration parameters for the PWM outputs.
 *  The internal checker modules verifies that this inputs contain valid values.
 *
 */
typedef struct io_pwm_safety_conf_
{
    bool enable_current_check;  /**< If set to \c #TRUE the diagnostic component of the I/O driver
                                     will check if the electric current through the load is
                                     within the specified limits */
    ubyte2 current_limit;       /**< Current limit in mA [0 .. 7500].
                                     The diagnostic component of the I/O driver will check if the
                                     electric current through the load does not exceed the
                                       specified limit */
    ubyte1 low_side_channel;    /**< Low side channel which is connected to the load on the
                                  configured PWM channel as a tertiary shut-off path. This channel
                                  is switched on and off together with the PWM channel's secondary
                                  shut-off path, the safety switch. Only one safety-critical
                                  PWM channel or HS digital output can be connected to a
                                  low-side channel.
                                   - \c #IO_PIN_NONE
                                     if the load is connected to the ground
                                   - \c #IO_DO_08 .. \c #IO_DO_15
                                     if the load is connected to a low side switch */
} IO_PWM_SAFETY_CONF;

/**
 *
 * \brief PWM current measurement queue
 * \anchor pwm_curr_q_struct
 *
 *  Stores results of the equidistant current measurement.
 *
 *  The queue holds all current measurement since the last retrieval via the step function
 *  \c IO_PWM_GetCur() or \c IO_PWM_GetCurQueue().
 *
 */
typedef struct io_pwm_current_queue_
{
    ubyte1 count;                            /**< Number of results stored in the queue */
    bool   overrun;                          /**< Signal queue overrun.\n
                                                  \c #TRUE means queue is full and older measurement
                                                  results may have been dropped.\n
                                                  \c #FALSE means queue is not overrun */
    ubyte2 values[IO_PWM_CURRENT_QUEUE_MAX]; /**< Buffer holding the measurement values in mA
                                                  [0 .. 7500].\n
                                                  Oldest value is <tt>values[0]</tt>. */
} IO_PWM_CURRENT_QUEUE;

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Setup a single PWM output
 *
 * \param pwm_channel       PWM channel, one of:
 *                              - \c #IO_PWM_00 .. \c #IO_PWM_35
 * \param frequency         PWM frequency (50Hz .. 1000Hz, only predefined frequencies with a
 *                          period of an integral multiple of 1ms, 0.5ms or 0.25ms are possible)
 * \param polarity          Polarity of output signal
 *                              - \c #FALSE: Low output signal is variable
 *                              - \c #TRUE:  High output signal is variable
 * \param diag_margin       Indicate if a margin should be applied or not.
 *                              - \c #TRUE:  margin is on
 *                              - \c #FALSE: no margin will be applied
 *                              - If a PWM channel is configured safety relevant this parameter has
 *                              to be set to \c #TRUE
 * \param[in] safety_conf   Safety configuration of the PWM channel. The \c low_side_channel field
 *                          is evaluated only with IO_PWM_InitWithLowside(). If a safety
 *                          configuration is passed, the parameter \c diag_margin will be
 *                          forced to \c #TRUE.
 *
 * \return #IO_ErrorType
 * \anchor io_pwm_init_retvals
 * \retval #IO_E_OK                          everything fine
 * \retval #IO_E_CHANNEL_BUSY                the PWM output channel or the timer input channel is
 *                                           currently used by another function
 * \retval #IO_E_INVALID_CHANNEL_ID          the given channel id does not exist or is not
 *                                           available on the used ECU variant
 * \retval #IO_E_CH_CAPABILITY               the given channel is not a PWM channel or the used ECU
 *                                           variant does not support this function
 * \retval #IO_E_INVALID_PARAMETER           a given parameter is out of range
 * \retval #IO_E_CM_CALIBRATION              the zero current calibration failed
 * \retval #IO_E_DRIVER_NOT_INITIALIZED      the common driver init function has not been called
 *                                           before
 * \retval #IO_E_FPGA_NOT_INITIALIZED        the FPGA has not been initialized
 * \retval #IO_E_INVALID_SAFETY_CONFIG       an invalid safety configuration has been passed
 * \retval #IO_E_DRV_SAFETY_CONF_NOT_CONFIG  global safety configuration is missing
 * \retval #IO_E_DRV_SAFETY_CYCLE_RUNNING    the init function was called after the task begin
 *                                           function
 * \retval #IO_E_UNKNOWN                     an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 * \anchor io_pwm_init_remarks
 *
 * \anchor pwm_frequency_table
 * \latexonly \hypertarget{IO_PWM_remarks}{} \endlatexonly
 *   - For the equidistant current measurement to work properly only certain frequencies are
 *   supported:
 * \verbatim[table]
 * ============== ========================== ===========================
 *   Available frequencies:
 * =====================================================================
 *   Period[ms]     Frequency[Hz]             Current sample period[ms]
 * -------------- -------------------------- ---------------------------
 *    \c  1         \c 1000                     \c  1
 * -------------- -------------------------- ---------------------------
 *    \c  1.25      \c  800                     \c  5
 * -------------- -------------------------- ---------------------------
 *    \c  1.5       \c  666 (\c 666.6666)       \c  3
 * -------------- -------------------------- ---------------------------
 *    \c  1.75      \c  571 (\c 571.4285)       \c  7
 * -------------- -------------------------- ---------------------------
 *    \c  2         \c  500                     \c  2
 * -------------- -------------------------- ---------------------------
 *    \c  2.25      \c  444 (\c 444.4444)       \c  9
 * -------------- -------------------------- ---------------------------
 *    \c  2.5       \c  400                     \c  5
 * -------------- -------------------------- ---------------------------
 *    \c  2.75      \c  363 (\c 363.6363)       \c 11
 * -------------- -------------------------- ---------------------------
 *    \c  3         \c  333 (\c 333.3333)       \c  3
 * -------------- -------------------------- ---------------------------
 *    \c  3.25      \c  307 (\c 307.6923)       \c 13
 * -------------- -------------------------- ---------------------------
 *    \c  3.5       \c  285 (\c 285.7142)       \c  7
 * -------------- -------------------------- ---------------------------
 *    \c  3.75      \c  266 (\c 266.6666)       \c 15
 * -------------- -------------------------- ---------------------------
 *    \c  4         \c  250                     \c  4
 * -------------- -------------------------- ---------------------------
 *    \c  4.25      \c  235 (\c 235.2941)       \c 17
 * -------------- -------------------------- ---------------------------
 *    \c  4.5       \c  222 (\c 222.2222)       \c  9
 * -------------- -------------------------- ---------------------------
 *    \c  4.75      \c  210 (\c 210.5263)       \c 19
 * -------------- -------------------------- ---------------------------
 *    \c  5         \c  200                     \c  5
 * -------------- -------------------------- ---------------------------
 *    \c  5.5       \c  181 (\c 181.8181)       \c 11
 * -------------- -------------------------- ---------------------------
 *    \c  6         \c  166 (\c 166.6666)       \c  6
 * -------------- -------------------------- ---------------------------
 *    \c  6.5       \c  153 (\c 153.8461)       \c 13
 * -------------- -------------------------- ---------------------------
 *    \c  7         \c  142 (\c 142.8571)       \c  7
 * -------------- -------------------------- ---------------------------
 *    \c  7.5       \c  133 (\c 133.3333)       \c 15
 * -------------- -------------------------- ---------------------------
 *    \c  8         \c  125                     \c  8
 * -------------- -------------------------- ---------------------------
 *    \c  8.5       \c  117 (\c 117.6470)       \c 17
 * -------------- -------------------------- ---------------------------
 *    \c  9         \c  111 (\c 111.1111)       \c  9
 * -------------- -------------------------- ---------------------------
 *    \c  9.5       \c  105 (\c 105.2631)       \c 19
 * -------------- -------------------------- ---------------------------
 *    \c 10         \c  100                     \c 10
 * -------------- -------------------------- ---------------------------
 *    \c 11         \c   90 (\c  90.9090)       \c 11
 * -------------- -------------------------- ---------------------------
 *    \c 12         \c   83 (\c  83.3333)       \c 12
 * -------------- -------------------------- ---------------------------
 *    \c 13         \c   76 (\c  76.9230)       \c 13
 * -------------- -------------------------- ---------------------------
 *    \c 14         \c   71 (\c  71.4285)       \c 14
 * -------------- -------------------------- ---------------------------
 *    \c 15         \c   66 (\c  66.6666)       \c 15
 * -------------- -------------------------- ---------------------------
 *    \c 16         \c   62 (\c  62.5)          \c 16
 * -------------- -------------------------- ---------------------------
 *    \c 17         \c   58 (\c  58.8235)       \c 17
 * -------------- -------------------------- ---------------------------
 *    \c 18         \c   55 (\c  55.5555)       \c 18
 * -------------- -------------------------- ---------------------------
 *    \c 19         \c   52 (\c  52.6315)       \c 19
 * -------------- -------------------------- ---------------------------
 *    \c 20         \c   50                     \c 20
 * ============== ========================== ===========================
 * \endverbatim
 *   - If you select an unavailable frequency within the allowed range, the next higher available
 *   frequency will be used. For example, if 180 Hz is specified, 181.8 Hz will be used.
 *
 * \remarks
 *   - The associated timer loopback channel will also be configured for open load and short
 *   circuit detection.
 *
 * \remarks
 *   - The associated current measurement will also be configured.
 *
 * \remarks
 *   - The duty cycle cannot exceed the margin of 100us(lower boundary) and 200us(upper boundary)
 *   used for diagnostic if the parameter diag_margin is \c #TRUE. This mode is important for
 *   hydraulic coils. If the parameter diag_margin is \c #FALSE, no duty cycle range margin will be
 *   applied.
 *
 * \remarks
 *   - All PWM channels have their own frequency time base.
 *
 * \remarks
 *    - If \c safety_conf != \c #NULL, the internal checker modules check the given channels
 *    against the \c current parameter in safety_conf, and the period and duty cycle feedback
 *    against the output. For more details about the current checking refer to the definition
 *    of \c #IO_PWM_SAFETY_CONF.
 *
 * \remarks
 *    - If \c safety_conf != \c #NULL, the parameter diag_margin is forced to \c #TRUE to allow
 *    diagnostics
 *
 * \remarks
 *    - Static friction and stiction can cause a hysteresis and make the control of a hydraulic
 *    valve erratic and unpredictable. In order to counteract these hysteresis effects, small
 *    vibrations about the desired position shall be created in the valve. This constantly breaks
 *    the static friction ensuring that it will move even with small input changes, and the effects
 *    of hysteresis are average out. A proper setting of PWM frequency according to the resonance
 *    frequency of the actuator allows to adjust this desired small vibration, low enough in
 *    amplitude to prevent noticeable oscillations on the hydraulic output but sufficient high to
 *    prevent friction. The PWM frequency can be set in the range of 50 .. 1000Hz, a typical range
 *    for hydraulic valves to operate without friction is 90 .. 160Hz.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWM_Init(ubyte1 pwm_channel,
                         ubyte2 frequency,
                         bool polarity,
                         bool diag_margin,
                         IO_PWM_SAFETY_CONF const * const safety_conf);

/**********************************************************************************************//**
 *
 * \brief Setup a single PWM output (which can be connected to a low side channel)
 * \anchor io_pwm_initwithlowside
 *
 * \param pwm_channel       PWM channel, one of:
 *                              - \c #IO_PWM_00 .. \c #IO_PWM_35
 * \param frequency         PWM frequency (50Hz .. 1000Hz, only predefined frequencies with a
 *                          period of an integral multiple of 1ms, 0.5ms or 0.25ms are possible)
 * \param polarity          Polarity of output signal
 *                              - \c #FALSE: Low output signal is variable
 *                              - \c #TRUE:  High output signal is variable
 * \param diag_margin       Indicate if a margin should be applied or not.
 *                              - \c #TRUE:  margin is on
 *                              - \c #FALSE: no margin will be applied
 *                              - If a PWM channel is configured safety relevant this parameter has
 *                              to be set to \c #TRUE
 * \param[in] safety_conf   Relevant safety configuration of the PWM channel.
 *                          If a safety configuration is passed the parameter \c diag_margin will
 *                          be forced to \c #TRUE.
 *
 * \return IO_ErrorType
 *
 * \retval The return values match exactly the
 *         \ref io_pwm_init_retvals "return values of IO_PWM_Init()".
 *
 *************************************************************************************************
 * \remarks
 *   - Unlike with IO_PWM_Init(), the \c low_side_channel field of the safety configuration *is*
 *   evaluated with this function.
 *
 * \remarks
 *   - If configured, the low side switch of a PWM channel is switched on and off together with
 *   the high side safety switch of the channel.
 *
 * \remarks
 *   - The \ref io_pwm_init_remarks "remarks for IO_PWM_Init()" also apply here.
 ************************************************************************************************/
IO_ErrorType IO_PWM_InitWithLowside(ubyte1 pwm_channel,
                                    ubyte2 frequency,
                                    bool polarity,
                                    bool diag_margin,
                                    IO_PWM_SAFETY_CONF const * const safety_conf);

/**********************************************************************************************//**
 *
 * \brief Deinitializes a PWM output.
 *
 *   Allows the re-initialization of the output by other functions
 *
 * \param pwm_channel       PWM channel, one of:
 *                              - \c #IO_PWM_00 .. \c #IO_PWM_35
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel has no PWM capability
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_PWM_DeInit(ubyte1 pwm_channel);

/**********************************************************************************************//**
 *
 * \brief Set the duty cycle for a PWM channel.
 *
 * \param pwm_channel       PWM channel, one of:
 *                              - \c #IO_PWM_00 .. \c #IO_PWM_35
 * \param duty_cycle        Duty cycle for the channel. Range: 0..65535 (0%..100%)
 * \param[out] high_time_fb High time feedback for the channels (optional). Returns high-time in us
 * \param[out] period_fb    Period feedback for the channels (optional). Returns period in us
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY          the given channel has no PWM capability
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_STARTUP                the output is in the startup phase
 * \retval #IO_E_PWM_NO_DIAG            No diagnostic is possible currently. Either the set duty
 *                                      cycle is not within a diagnostic margin or not enough
 *                                      samples have been captured yet.
 * \retval #IO_E_SHORT_GND              short circuit has been detected
 * \retval #IO_E_OPEN_LOAD_OR_SHORT_BAT open load or short circuit to battery has been detected
 * \retval #IO_E_OPEN_LOAD              open load  has been detected
 * \retval #IO_E_SHORT_BAT              short circuit to battery has been detected
 * \retval #IO_E_FET_PROT_ACTIVE        the output FET protection is active and has set the output
 *                                      to a constant low level. It can be reset with
 *                                      \c IO_PWM_ResetProtection() after the wait time is passed
 * \retval #IO_E_FET_PROT_REENABLE      the output FET protection is ready to be reset with
 *                                      \c IO_PWM_ResetProtection()
 * \retval #IO_E_FET_PROT_PERMANENT     the output FET is protected permanently because it was
 *                                      already reset more than 10 times. The output will remain at
 *                                      a constant low level
 * \retval #IO_E_SAFETY_SWITCH_DISABLED The safety switch of the corresponding output is disabled.
 *                                      The output is currently forced to low.
 * \retval #IO_E_SAFE_STATE             the PWM channel is in a safe state
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 * \retval #IO_E_RESOLVING              a previously started resolving operation for this channel
 *                                      is still ongoing
 * \retval #IO_E_RESOLVING_FAILED       a previously started resolving operation for this channel
 *                                      has failed to resolve the actual error (open load or
 *                                      short circuit to battery)
 *
 **************************************************************************************************
 *
 * \remarks
 *   - The duty cycle cannot exceed a margin of 100us(lower boundary) and 200us(upper boundary)
 *   used for diagnostic if the parameter diag_margin was set \c #TRUE (via the function
 *   \c IO_PWM_Init() ). This mode is important for hydraulic coils. If the parameter diag_margin
 *   is \c #FALSE, no duty cycle range margin will be applied.
 *
 * \remarks
 *   - If one of the parameter \c high_time_fb or \c period_fb is \c #NULL, the parameter is
 *   ignored. The parameter \c high_time_fb returns the measured pulse-width (high time) of the PWM
 *   signal in the last round in us. The parameter \c period_fb returns the measured period of the
 *   PWM signal in the last round in us. If the measurement is not finished yet, the parameter
 *   \c high_time_fb or \c period_fb holds the value 0.
 *
 *   \remarks
 *   - #IO_E_OPEN_LOAD and #IO_E_SHORT_BAT may be resolved by calling \c IO_PWM_ResolveOpenLoadShortCircuit()
 *   once and then continue calling \c IO_PWM_SetDuty().
 *
 * \attention
 *   At the time when a PWM channel is initialized with \c IO_PWM_Init(), it has to be ensured
 *   that the actor is completely discharged and no current is induced to the PWM output. Otherwise
 *   the PWM current measurement calibration will fail.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWM_SetDuty(ubyte1 pwm_channel,
                            ubyte2 duty_cycle,
                            ubyte2 * const high_time_fb,
                            ubyte2 * const period_fb);

/**********************************************************************************************//**
 *
 * \brief Returns the measured current of the given channel
 *
 * \param pwm_channel       PWM channel, one of:
 *                              - \c #IO_PWM_00 .. \c #IO_PWM_35
 * \param[out] current      Measured current in mA
 *                          Range: 0..7500 (0A .. 7.500A)
 * \param[out] fresh        Indicates if new values are available since
 *                          the last call.
 *                              - \c #TRUE: Value in "current" is valid
 *                              - \c #FALSE: No new value available.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PWM channel
 * \retval #IO_E_CM_CALIBRATION         the zero current calibration failed
 * \retval #IO_E_NULL_POINTER           a NULL pointer has been passed to the function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_REFERENCE              the internal reference voltage is out of range
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \attention
 *   The current measurement is equidistant which means that the sampling happens synchronous to
 *   the PWM period. Every 1ms a current value will be captured. The captured current values will
 *   be averaged over the time of one period of the PWM signal and then provided to the
 *   application.
 *
 * \remarks
 *   - When the function \c IO_PWM_GetCur() is called, the internal queue holding the values of the
 *   current measurement is flushed. If the function is called more than once in a cycle it may or
 *   may not deliver new values, depending on how many values the equidistant current measurement
 *   has sampled since the last call.
 *
 * \remarks
 *   - If there is no new current value available (for example the function \c IO_PWM_GetCur() gets
 *   called more frequently than the PWM period) the flag \c fresh will be set to \c #FALSE.
 *
 * \remarks
 *   - If the functions \c IO_PWM_GetCurQueue() and \c IO_PWM_GetCur() are called after each other,
 *   only the first function will deliver a current value because both functions will empty the
 *   internal measurement queue.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWM_GetCur(ubyte1 pwm_channel,
                           ubyte2 * const current,
                           bool * const fresh);

/**********************************************************************************************//**
 *
 * \brief Returns the measured current values since the last call of the given channel
 *
 * \param pwm_channel           PWM channel, one of:
 *                                  - \c #IO_PWM_00 .. \c #IO_PWM_35
 * \param[out] current_queue    Queue holding the current values since the last call of the step
 *                              function including a queue overrun flag and a counter for the
 *                              quantity of available values
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PWM channel
 * \retval #IO_E_CM_CALIBRATION         the zero current calibration failed
 * \retval #IO_E_NULL_POINTER           a NULL pointer has been passed to the function
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_REFERENCE              the internal reference voltage is out of range
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \attention
 *   The current measurement is equidistant which means that the sampling happens synchronous to
 *   the PWM period. Every 1ms a current value will be captured. The captured current values will
 *   be averaged over the time of one period of the PWM signal and then provided to the application
 *   in a queue. The queue storage was chosen to avoid the loss of any measurement value if the
 *   user application runs asynchronous to the current measurement or if its cycle time is lower
 *   than the PWM period. (Size of queue: \c #IO_PWM_CURRENT_QUEUE_MAX, Queue data type:
 *   \c #IO_PWM_CURRENT_QUEUE)
 *
 * \remarks
 *   - When the function \c IO_PWM_GetCurQueue() is called, the internal queue holding the values
 *   of the current measurement is flushed. If the function is called more than once in a cycle it
 *   may or may not deliver new values, depending on how many values the equidistant current
 *   measurement has sampled since the last call.
 *
 * \remarks
 *   - If there is no new current value available (for example the function \c IO_PWM_GetCurQueue()
 *   gets called more frequently than the PWM period) the value \c count in the structure
 *   \c current_queue will be 0.
 *
 * \remarks
 *   - If the functions \c IO_PWM_GetCurQueue() and \c IO_PWM_GetCur() are called after each other,
 *   only the first function will deliver a current value because both functions will empty the
 *   internal measurement queue.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWM_GetCurQueue(ubyte1 pwm_channel,
                                IO_PWM_CURRENT_QUEUE * const current_queue);

/**********************************************************************************************//**
 *
 * \brief Reset the output protection for a PWM channel
 *
 * \param pwm_channel       PWM channel, one of:
 *                              - \c #IO_PWM_00 .. \c #IO_PWM_35
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
 *                                      time of 10s is not already passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PWM channel
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
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
 *       - 10s for \c #IO_PWM_00 .. \c #IO_PWM_35
 *
 * \note
 *    - This function will set the output back again to the minimum pulse if a diagnostic margin is
 *    configured. After calling this function the output has to be set to the intended duty cycle
 *    with \c IO_PWM_SetDuty()
 *
 * \remarks
 *   If the parameter \c reset_cnt is \c #NULL, the parameter is ignored. The parameter
 *   \c reset_cnt returns the number of resets already performed.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWM_ResetProtection(ubyte1 pwm_channel,
                                    ubyte1 * const reset_cnt);

/**********************************************************************************************//**
 *
 * \brief Resolve an open load or short circuit to battery voltage condition for a PWM channel
 *
 * \param pwm_channel       PWM channel, one of:
 *                              - \c #IO_PWM_00 .. \c #IO_PWM_35
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_BUSY                   a previously started resolving operation is still ongoing
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a PWM channel
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel is not configured
 * \retval #IO_E_INVALID_OPERATION      the given channel does not return
 *                                      #IO_E_OPEN_LOAD_OR_SHORT_BAT or is not in an operational
 *                                      state
 *
 **************************************************************************************************
 *
 * \attention
 *   - The resolving of a PWM channel requires to shift an internal feedback signal that is used
 *     by all PWM channels. Although this does not influence other PWM channels, it can affect
 *     channels that are configured for the alternative functions digital input and timer input.
 *     Please refer to the safety manual for detailed information.
 *
 * \attention
 *   - The I/O driver does not provide any debouncing mechanism for the return value of function
 *     \c IO_PWM_SetDuty(). Thus, the application itself has to implement the debouncing mechanism
 *     before starting a resolving operation if required.
 *
 * \note
 *   - Resolving of a PWM channel can only be triggered if \c IO_PWM_SetDuty() returns
 *     #IO_E_OPEN_LOAD_OR_SHORT_BAT for that channel.
 *
 * \note
 *   - The result of the resolving is retrieved using the function \c IO_PWM_SetDuty():
 *     During the resolving operation, \c IO_PWM_SetDuty() returns #IO_E_RESOLVING.
 *     When the resolving completes, the returned value will be #IO_E_OPEN_LOAD or
 *     #IO_E_SHORT_BAT, depending on the detected condition.
 *
 * \note
 *   - After resolving an open load or short circuit to battery condition the PWM channel
 *     will be automatically disabled and cannot be reenabled anymore.
 *
 *************************************************************************************************/
IO_ErrorType IO_PWM_ResolveOpenLoadShortCircuit(ubyte1 pwm_channel);

#ifdef __cplusplus
}
#endif

#endif /* IO_PWM_H_ */
