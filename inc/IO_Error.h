/**************************************************************************************************
 * Copyright (c) 2022 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_Error.h
 *
 * \latexonly \hypertarget{IO_Error}{} \endlatexonly
 *
 * \brief Global error defines for IO driver
 *
 *  This header file defines all the error codes for the IO driver.
 *
 *************************************************************************************************/

#ifndef IO_ERROR_H_
#define IO_ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *
 * I N C L U D E S
 *
 *************************************************************************************************/

#include "ptypes_tms570.h"

/**************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/

/**< \brief IO Driver error type
 *
 * Type definition for all the IO Driver errors.
 */
typedef ubyte2  IO_ErrorType;

/**************************************************************************************************
 *
 * D E F I N I T I O N S
 *
 *************************************************************************************************/

/*
 * E R R O R   V A L U E S
 */

/* general codes */

/** everything is fine, no error has occurred. */
#define IO_E_OK                               0U

/** Module or function is busy.\n
 *  This error is reported if a function or module has not yet finished its task. For example the
 *  EEPROM write function will return this error code if a previous write command has not been
 *  finished yet.
 */
#define IO_E_BUSY                             2U

/** General error. No further information can be provided. */
#define IO_E_UNKNOWN                          3U



/** Global safety configuration is missing\n
 *  This error is reported if an IO is defined as safety critical although no safety configuration
 *  has been passed to the \c IO_Driver_Init() (parameter \c safety_conf) function. An IO pin is
 *  considered as safety critical if a valid safety configuration has been passed to the init
 *  function (see \c safety_conf parameter of the functions \c IO_ADC_ChannelInit(),
 *  \c IO_PWM_Init(), \c IO_DO_Init(), \c IO_PWD_IncInit(), \c IO_PWD_CountInit() and
 *  \c IO_PWD_ComplexInit() )
 */
#define IO_E_DRV_SAFETY_CONF_NOT_CONFIG       5U

/** The safety configuration for the channel to be configured is invalid\n
 *  This error is reported if a parameter in the safety configuration structure used for
 *  configuring a IO channel is wrong. (see \c safety_conf parameter of the functions
 *  \c IO_ADC_ChannelInit(), \c IO_PWM_Init(), \c IO_DO_Init(), \c IO_PWD_IncInit(),
 *  \c IO_PWD_CountInit() and \c IO_PWD_ComplexInit() )
 */
#define IO_E_INVALID_SAFETY_CONFIG            6U

/** The pin can not be configured anymore as safety critical, as the application cycle was already
 *  started.\n
 *  This error is reported if a pin is initialized as safety critical after the cyclic task begin
 *  function \c IO_Driver_TaskBegin() is called. An IO pin is considered as safety critical if a
 *  valid safety configuration has been passed to the init function (see \c safety_conf parameter
 *  of the functions \c IO_ADC_ChannelInit(), \c IO_PWM_Init(), \c IO_DO_Init(),
 *  \c IO_PWD_IncInit(), \c IO_PWD_CountInit() and \c IO_PWD_ComplexInit() )
 */
#define IO_E_DRV_SAFETY_CYCLE_RUNNING         7U


/* Output error codes */

/**
 * A open load condition has been detected on an output.
 *
 *  \par Digital outputs:
 *  This error code is reported when the output is set to low (power stage is switched off)
 *  and the analog feedback signal is between 1.5V and 5.5V.\n
 *  The error condition is reset as soon as the feedback voltage is below 1.5V or the output is set
 *  to high.
 */
#define IO_E_OPEN_LOAD                       10U

/**
 * A short circuit to ground condition has been detected on an output.
 *
 *  \par PWM output:
 *  This error code is reported if the PWM output was configured to apply a diagnostic margin and
 *  the digital timer feedback of the output was not able to read a valid signal. The feedback
 *  signal was read as low.\n
 *  The error condition is reset as soon as the timer feedback signal returns a valid signal again.
 *
 *  \par PWM output in digital output mode:
 *  This error code is reported when the output is set to high (power stage is switched on) and the
 *  digital feedback signal is low.\n
 *  The error condition is reset as soon as the feedback signal is high, or if the output is set to
 *  low.
 *
 *  \par Digital outputs high side:
 *  This error code is reported when the output is set to high (power stage is switched on) and the
 *  analog feedback signal is below 1.5V.\n
 *  The error condition is reset as soon as the feedback voltage is above 1.5V, or if the output is
 *  set to low.
 *
 *  \par Digital outputs low side:
 *  This error code is reported when the output is set to low (power stage is switched off) and the
 *  analog feedback signal is below 3V.\n
 *  The error condition is reset as soon as the feedback voltage is above 3V, or if the output is
 *  set to high.
 *
 *  \par Digital inputs:
 *  This error code is reported when the measured analog input voltage if below the defined low
 *  band.\n
 *  The error condition is reset as soon as the voltage is inside a defined voltage band.
 */
#define IO_E_SHORT_GND                       11U

/**
 * A short circuit to battery voltage condition has been detected on an output.
 *
 *  \par Digital outputs high side:
 *  This error code is reported when the output is set to low (power stage is switched off) and the
 *  analog feedback signal is above 5.5V.\n
 *  The error condition is reset as soon as the feedback voltage is below 1.5V, or if the output is
 *  set to high.
 *
 *  \par Digital outputs low side:
 *  This error code is reported when the output is set to high (power stage is switched on) and the
 *  analog feedback signal is out of range.\n
 *  The error condition is reset as soon as the output is set to low.
 *
 *  \par Digital inputs:
 *  This error code is reported when the measured analog input voltage if above the defined high
 *  band.\n
 *  The error condition is reset as soon as the voltage is inside a defined voltage band.
 */
#define IO_E_SHORT_BAT                       12U

/**
 * An open load or short circuit to battery voltage condition has been detected.
 *
 *  \par PWM output:
 *  This error code is reported if the PWM output was configured to apply a diagnostic margin and
 *  the digital timer feedback of the output was not able to read a valid signal. The feedback
 *  signal was read as high.\n
 *  The error condition is reset as soon as the timer feedback signal returns a valid signal again.
 *  In order to distinguish between open load and short to UBAT, refer to function
 *  \c IO_PWM_ResolveOpenLoadShortCircuit().
 *
 *  \par PWM output in digital output mode:
 *  This error code is reported when no load is connected and the output is off. In this case the
 *  level of the feedback channel is already high (pull up voltage) and can not be distinguished
 *  from a short to battery voltage.\n
 *  The error condition is reset as soon as a load is connected. This I/O type cannot distinguish
 *  between open load and short to UBAT.
 */
#define IO_E_OPEN_LOAD_OR_SHORT_BAT          13U

/**
 * The measured voltage does not fit to a voltage band.
 *
 *  \par Digital inputs:
 *  This error code is reported when the measured voltage is in between the defined voltage bands
 *  for high and low.
 */
#define IO_E_INVALID_VOLTAGE                 14U

/**
 *  No output diagnostic is currently possible.
 *
 *  This error code is reported in several different cases:
 *
 *  \par PWM - Not enough samples:
 *  The timer feedback has not captured enough samples yet in order to provide valid diagnostic
 *  information. Either the output was just enabled or just entered the valid diagnostic margin.
 *  The error condition is reset as soon as enough feedback samples have been captured.
 *
 *  \par PWM - Out of diagnostic range:
    The PWM output was configured to not apply any diagnostic margin and the output was set to
 *  a duty cycle below the minimum on-time or above the minimum off-time. In this case the timer
 *  feedback of the output is not able to read a valid signal. The error condition is reset as soon
 *  as a duty cycle above the minimum on-time and below the minimum off-time is set and enough
 *  feedback samples have been captured.
 *
 *  \par DO - No feedback after level change:
 *  After the level of a digital output was changed, no diagnostic feedback is available directly
 *  afterwards. The error condition is reset as soon as the output stays constantly at one level
 *  for at least 10 ms.
 */
#define IO_E_NO_DIAG                         15U

/**
 *  See #IO_E_NO_DIAG.
 */
#define IO_E_PWM_NO_DIAG                     IO_E_NO_DIAG

/** The pin is in the startup phase.\n
 *  This error is reported during the startup phase while several startup checks on the pins are
 *  made. During this phase, the pin can not be controlled by the application. The maximum
 *  duration of this startup phase depends on the I/O module:
 *  - ADC module:  max 50ms from the point in time the diagnostic state machine has reached
 *                 the main state
 *  - DIO module:  max 30ms from the point in time the diagnostic state machine has reached
 *                 the main state, max 20ms after changing the state of the digital output
 *  - PVG module:  max 110ms from the point in time the output has been initialized
 *  - PWD module:  max 120ms from the point in time the input has been initialized
 *  - PWM module:  max 5ms from the point in time the diagnostic state machine has reached
 *                 the main state, max 5ms after re-enabling the corresponding safety switch
 *  - VOUT module: max 110ms from the point in time the output has been initialized
 */
#define IO_E_STARTUP                         16U

/** The pin is in a safe state.\n
 *  This error is reported if a pin or an entire shut off group was disabled by the application
 *  callback or the global safe state was entered.
 */
#define IO_E_SAFE_STATE                      17U

/** A reference voltage (sensor supply or internal reference) is out of range.\n
 *  This error is reported for pins with analog feedback if:
 *  - the sensor supply is out of range in ratiometric mode
 *  - the 2.5 reference is invalid in absolute mode
 *  - the reference voltages are unplausible in current mode
 *  - the reference is out of range in resistive mode
 */
#define IO_E_REFERENCE                       18U

/** A safety switch is disabled.\n
 *  This error is reported for pins configured as outputs in the shut off groups #IO_INT_SAFETY_SW_0,
 *  #IO_INT_SAFETY_SW_1 and #IO_INT_SAFETY_SW_2 when its corresponding safety switch is disabled.
 */
#define IO_E_SAFETY_SWITCH_DISABLED          19U

/* FET protection error codes */

/** An internal switch (FET) or output has been disabled to protect it from damage.\n
 *  The output or switch can be reenabled after a wait time. When this time has passed,
 *  \c #IO_E_FET_PROT_REENABLE will be returned.\n
 *  If the current on an internal FET or an output is too high, the FET will be switched off by
 *  software to protect it from damage. When a FET has been switched off by the protection
 *  mechanism, this error code will be returned by the respective task function. The measured
 *  values are therefore invalid and should not be used for further calculations. \n
 *  For more details on the output/input protection mechanism refer to the following sections:
 *   - \ref do_output_protection "Digital Output Protection"
 *   - \ref pvg_output_protection "PVG Output Protection"
 *   - \ref pwm_output_protection "PWM Output Protection"
 *   - \ref vout_output_protection "VOUT Output Protection"
 *   - \ref adc_input_protection "ADC Input Protection"
 *   - \ref pwd_input_protection "PWD Input Protection"
 */
#define IO_E_FET_PROT_ACTIVE                 20U

/** An internal switch (FET) or output has been disabled permanently to protect it from damage.\n
 *  The output can not be reenabled anymore, as the maximum reset count was reached.
 */
#define IO_E_FET_PROT_PERMANENT              21U

/** An internal switch (FET) or output has been disabled to protect it from damage.\n
 *  The switch or output is ready to be reenabled via the protection reset function.
 */
#define IO_E_FET_PROT_REENABLE               22U

/** The internal switch (FET) or output can not be reenabled, as the wait time is not passed
 *  already.
 */
#define IO_E_FET_PROT_WAIT                   23U

/** The internal switch (FET) or output is not protected, nothing can be reenabled.
 */
#define IO_E_FET_PROT_NOT_ACTIVE             24U

/** The resolving operation of a formerly detected open load or short circuit to battery voltage
 *  condition is still ongoing. The output is disabled by software and cannot be reenabled.
 */
#define IO_E_RESOLVING                       25U

/** The resolving operation of a formerly detected open load or short circuit to battery voltage
 *  condition failed. No detailed information about the actual reason is available. The output is
 *  disabled by software and cannot be reenabled.
 */
#define IO_E_RESOLVING_FAILED                26U

/* parameter error codes */

/** A NULL pointer has been passed to the function.\n
 *  This error is reported if a non-optional pointer parameter of the function has been set to
 *  \c #NULL.
 */
#define IO_E_NULL_POINTER                    30U

/** An invalid parameter has been passed to the function.\n
 *  This error is reported if at least one of the parameters which have been passed to the function
 *  is outside the allowed range.
 */
#define IO_E_INVALID_PARAMETER               31U

/** The IO channel (IO pin) is busy\n
 *  This error is reported if a IO Pin has been initialized before.\n
 *  To change the configuration of the channel during runtime the according DeInit function needs
 *  to be called before the channel can be initialized with a new configuration.
 */
#define IO_E_CHANNEL_BUSY                    32U

/** The IO channel (IO pin) has not been initialized.\n
 *  This error is reported by an IO driver task function if the channel has not been initialized.
 *  To initialize the channel, the according Init function needs to be called.
 */
#define IO_E_CHANNEL_NOT_CONFIGURED          33U

/** The IO channel (IO pin) does not exist.\n
 *  Two conditions can lead to this error code:
 *  - When a non-existent channel ID has been passed to the function.
 *  - When trying to initialize an IO which is not available on the ECU variant.
 */
#define IO_E_INVALID_CHANNEL_ID              34U

/** An invalid limit configuration has been passed to the function.\n
 *  This error is reported if the limits configuration structure of a digital input is wrong
 */
#define IO_E_INVALID_LIMITS                  35U

/** The periodic interrupt timer has not been initialized.\n
 *  This error code is reported if trying to disable the periodic interrupt timer although it has
 *  not been setup.
 */
#define IO_E_PERIODIC_NOT_CONFIGURED         36U

/** The IO channel (IO pin) does not support the requested feature.\n
 *  Two conditions can lead to this error code:
 *  - When, for example, trying to initialize or use a ADC pin as PWM output.
 *  - When trying to initialize an IO for a pin function which is not available on the ECU variant.
 *    (For example when trying to initialize a PWD input but it is not physically mounted on the
 *    used ECU variant)
 */
#define IO_E_CH_CAPABILITY                   37U

/** The common driver init function \c IO_Driver_Init() has not been called.\n
 *  This error code is reported by the I/O driver init functions if the common init function
 *  \c IO_Driver_Init() has not been called or reported an error.
 */
#define IO_E_DRIVER_NOT_INITIALIZED          38U

/** The requested operation cannot be performed.\n
 *  This error code is returned if a valid set of parameters is provided but the requested
 *  operation is not allowed due to some internal state or condition.\n
 *  Example: Function \c IO_PWM_ResolveOpenLoadShortCircuit() has been called for a PWM channel
 *           that does not return #IO_E_OPEN_LOAD_OR_SHORT_BAT.
 */
#define IO_E_INVALID_OPERATION               39U


/* CAN error codes */

/** Message object or FIFO buffer overflow.\n
 *  This error is reported if CAN messages have been lost due to a full buffer. To avoid this error
 *  FIFO buffers can be used. If FIFO buffers are already used, try to increase the buffer size.
 */
#define IO_E_CAN_OVERFLOW                    40U

/** A wrong or invalid handle has been used.\n
 *  This error is reported if:
 *   - a non-existent handle has been used.
 *   - if a write handle has been passed to a read function or vice versa
 *   - if a message object handle has been passed to a FIFO function or vice versa
 */
#define IO_E_CAN_WRONG_HANDLE                41U

/** No more message objects are available.\n
 *  The maximum number of available message objects has been reached. A single message object is
 *  needed to setting up a single message object with the function \c IO_CAN_ConfigMsg(). When
 *  configuring a FIFO buffer with the function \c IO_CAN_ConfigFIFO() the number of needed message
 *  objects equals the size of the FIFO buffer (single message objects are joined together to a
 *  FIFO buffer).
 */
#define IO_E_CAN_MAX_MO_REACHED              42U

/** No more message handles are available.\n
 *  The maximum number of message handles has been reached. A message handle is generated every
 *  time the function \c IO_CAN_ConfigMsg() or \c IO_CAN_ConfigFIFO() is called without returning
 *  an error.
 */
#define IO_E_CAN_MAX_HANDLES_REACHED         43U

/** The FIFO is full, one or more new messages have been lost. */
#define IO_E_CAN_FIFO_FULL                   44U

/** No new data is available.\n
 *  This error is returned if no CAN frame has been received since the last successful read.
 */
#define IO_E_CAN_OLD_DATA                    45U

/** The CAN node is in error passive state.\n
 *  At least one of the error counters has reached 128.
 */
#define IO_E_CAN_ERROR_PASSIVE               46U

/** The CAN node is in bus off state.\n
 *  The transmit error counter has reached 255. The device remains in this state until 128
 *  occurrences of bus idle (128 * 11 consecutive recessive bits). The bus-off recovery sequence is
 *  triggered automatically.
 */
#define IO_E_CAN_BUS_OFF                     47U

/** The error counters of the CAN node have reached the warning state. The CAN node is still in
 *  active state.\n
 *  At least one of the error counters has reached the warning limit of 96.
 */
#define IO_E_CAN_ERROR_WARNING               48U

/** The CAN node reported a timeout during initialization or message transfer.\n
 *  Try again to initialize or to transfer.
 */
#define IO_E_CAN_TIMEOUT                     49U


/* EEPROM error codes */

/** Invalid address range.\n
 *  This error is reported if read or write operations are requested for non-existent EEPROM
 *  addresses.
 */
#define IO_E_EEPROM_RANGE                    60U

/** Internal SPI error.\n
 *  This error is reported if an error is detected in the SPI communication with the external EEPROM/FRAM.
 */
#define IO_E_EEPROM_SPI                      61U


/* UART error codes */

/** The UART software buffer is full.\n
 *  - When receiving: Too much data has been received since the last successful read operation.
 *  Data has been lost.
 *  - When transmitting: The given data does not fit into the buffer, data has been rejected. Try
 *  again when there is more space in the buffer.
 *
 *  The UART module has two buffers, one for transmitting and one for receiving, both with a size
 *  of #IO_UART_BUFFER_LEN.
 */
#define IO_E_UART_BUFFER_FULL                70U

/** The UART software buffer is empty.\n
 *  This error code is not used.
 */
#define IO_E_UART_BUFFER_EMPTY               71U

/** UART hardware reception buffer overrun.\n
 *  The UART receiver has not been serviced by the DMA in time. Data has been lost.
 */
#define IO_E_UART_OVERFLOW                   72U

/** UART parity error.\n
 *  The received parity bit doesn't match the calculated one.
 */
#define IO_E_UART_PARITY                     73U

/** UART framing error.\n
 *  An expected stop bit hasn't been found.
 */
#define IO_E_UART_FRAMING                    74U

/** Error in the UART DMA data processing.\n
 *  An error occurred in the UART DMA. Data has been lost.
 */
#define IO_E_UART_DMA                        75U


/* PWD error codes */

/** The required amount of signal edges couldn't be captured */
#define IO_E_PWD_NOT_FINISHED               101U

/** An overflow occurred during capturing */
#define IO_E_PWD_OVERFLOW                   102U

/** A current out of range was detected (too high) */
#define IO_E_PWD_CURRENT_THRESH_HIGH        104U

/** A current out of range was detected (too low) */
#define IO_E_PWD_CURRENT_THRESH_LOW         105U


/* current measurement error codes */

/** The current measurement calibration failed */
#define IO_E_CM_CALIBRATION                 146U


/* FPGA error codes */

/** The \c IO_Driver_Init() function has not been called, or reported one of the IO_E_FPGA_...
 *  errors.
 */
#define IO_E_FPGA_NOT_INITIALIZED           200U

/** The FPGA did not respond in time during its configuration. Some ECU functions will not be available. */
#define IO_E_FPGA_TIMEOUT                   201U

/** The FPGA reported a CRC error. Some ECU functions will not be available.\n
 *  Has the correct FPGA image been flashed in the CPU flash memory?
 */
#define IO_E_FPGA_CRC_ERROR                 202U

/** The FPGA version does not match with this I/O Driver version. Some ECU functions will not be available.\n
 *  Has the correct FPGA image been flashed in the CPU flash memory?
 */
#define IO_E_FPGA_VERSION                   203U

/** Error in the FPGA image. Some ECU functions will not be available.\n
 *  The FPGA image in the CPU flash memory is corrupted or its format is not supported.
 */
#define IO_E_FPGA_IMAGE                     204U


/* watchdog error codes */

/** The given configuration parameters for configuring the watchdogs are out of range.
 */
#define IO_E_WD_RANGE                       240U

/** One of the two available watchdog initialization functions of the watchdog system reported an
 * error.
 */
#define IO_E_WD_INITIALIZATION              241U

/** The given configuration parameters cannot be configured due to precision limitations of the
 *  internal watchdog module's prescaler. Either the task cycle time is too short or the window
 *  size is too narrow for the configured task cycle time.\n
 * Either the window size or the task cycle time must be increased.
 */
#define IO_E_WD_PRECISION                   242U

/** One of the two available watchdog activation functions reported an error.\n
 * This error code is used internally. It is not reported by any API function.
 */
#define IO_E_WD_ACTIVATION                  243U

/** One of the two available watchdog trigger functions reported an error.\n
 * This error code is used internally. It is not reported by any API function.
 */
#define IO_E_WD_TRIGGER                     244U

/** The watchdogs's device error counter reached it's maximum value. Therefore the external
 * watchdog stays locked in safe state. No reset takes place. No more activation is possible until
 * an externally asserted power cycle of the ECU.
 */
#define IO_E_WD_SAFE_LOCK                   245U

/** The debug key has been set to 0xC0FFEE in the corresponding field of the APDB and the very
 * first life cycle of the ECU has been detected. In this state, the external watchdog device is
 * kept in a frozen state to allow a reset and connection with the debugger. In this state no
 * watchdog activation takes place.
 */
#define IO_E_WD_DEBUGGING_PREPARED          246U

/** A watchdog task detected a timing error in it's own calling interval.\n
 * This can be either a too early or a too late call.
 */
#define IO_E_WD_SELF_MONITORING             247U

/** A watchdog task detected a timing error of the other watchdog.\n
 * The other watchdog has not been triggered until the latest possible point in time.
 */
#define IO_E_WD_VICE_VERSA_MONITORING       248U

/** The watchdog's status information could not be retrieved.
 */
#define IO_E_WD_STATUS_INVALID              249U

/* RTC error codes */

/** The integrity check of the Real Time Clock (responsible for date and time) failed.\n
 * Date and Time is lost. This can happen if a power loss on the RTC exceeds the buffer time
 * (approximately 10 minutes). To make the Real Time Clock counting again, it must be re-set by
 * calling IO_RTC_SetDateAndTime() with the appropriate date/time information.
 */
#define IO_E_RTC_CLOCK_INTEGRITY_FAILED     250U


/* LIN error codes */

/** A LIN bit error has been detected during the last transfer.
 */
#define IO_E_LIN_BIT                        260U

/** A LIN physical bus error has been detected during the last transfer.
 */
#define IO_E_LIN_PHYSICAL_BUS               261U

/** A LIN checksum error has been detected during the last data reception.
 */
#define IO_E_LIN_CHECKSUM                   262U

/** A LIN inconsistent synch field error has been detected during the last header reception.
 */
#define IO_E_LIN_INCONSISTENT_SYNCH_FIELD   263U

/** A LIN no-response error has been detected during the last transfer
 */
#define IO_E_LIN_NO_RESPONSE                264U

/** A LIN framing error has been detected during the last transfer
 */
#define IO_E_LIN_FRAMING                    265U

/** A LIN overrun error has been detected during the last transfer
 */
#define IO_E_LIN_OVERRUN                    266U

/** A LIN parity error has been detected during the last header reception.
 */
#define IO_E_LIN_PARITY                     267U

/** A LIN driver timeout error has been detected during the last transfer.
 */
#define IO_E_LIN_TIMEOUT                    268U


/* Flash error codes */

/** The flash chip did not return the expected device ID.\n
 *  This error is reported if during the initialization of the flash driver the checking of the
 *  flash chip device ID did not succeed.
 *
 *  \note
 *  This error is not expected after end-of-line testing.
 */
#define IO_E_FLASH_WRONG_DEVICE_ID          269U

/** Blank check during a flash write command failed.\n
 *  This error is reported when during a write operation the flash range includes a byte which is
 *  not blank (i.e. its value is not 0xFF).
 */
#define IO_E_FLASH_BLANK_CHECK_FAILED       270U

/** Flash operation failed.\n
 *  This error is reported by the \c IO_FLASH_GetStatus() function when the last flash operation
 *  failed (failure reported by the external flash chip).
 */
#define IO_E_FLASH_OP_FAILED                271U

/** Flash operation resulted in timeout.\n
 * This error is reported by the \c IO_FLASH_GetStatus() function when the last flash operation
 * took more time than expected.
 */
#define IO_E_FLASH_OP_TIMEOUT               272U

/** Flash module in suspended state.\n
 * This error is reported by \c IO_FLASH_GetStatus() when the \c IO_FLASH_Suspend() suspends
 * the flash operation.
 */
#define IO_E_FLASH_SUSPENDED                273U

/* Production Data related error codes */

/** The detected ECU variant is not supported.
 */
#define IO_E_INVALID_VARIANT                282U

/** An error has been detected during the integrity check of the Production Data area.
 */
#define IO_E_INVALID_PROD_DATA              283U

/** An error has been detected during the integrity check of the variant configuration data.
 */
#define IO_E_INVALID_PIN_CONFIG             284U

/** An error has been detected during the check of the Serial Number.
 */
#define IO_E_INVALID_SERIAL_NUMBER          285U


/* Self-test related error codes */

/** An error has been detected during the test of the core.
 */
#define IO_E_CORE_TEST_FAILED               286U

/** A timeout error has been detected during the test of the TMS error pin.
 */
#define IO_E_ERROR_PIN_TEST_TIMEOUT         287U

/** An error has been detected during the test of the TMS error pin.
 */
#define IO_E_ERROR_PIN_TEST_FAILED          288U

/** An error has been detected in the internal memory.
 */
#define IO_E_INTERNAL_MEM_FAILED            289U

/** An error has been detected in the start status of the ESM module.
 */
#define IO_E_INVALID_ESM_INIT_STATUS        290U

/** An error has been detected in the internal COSMO low level drivers.
 */
#define IO_E_INTERNAL_CSM                   292U


/* Ethernet error codes */

/** An error has been detected during the initialization of the Ethernet interface.
 */
#define IO_E_ETH_INIT_FAIL                  300U

/** A timeout has been detected during the initialization of the Ethernet interface.
 */
#define IO_E_ETH_INIT_TIMEOUT               301U

/** A timeout has been detected during the de-initialization of the Ethernet interface.
 */
#define IO_E_ETH_DEINIT_TIMEOUT             302U

/** An invalid MAC address has been read out from the production database
 */
#define IO_E_ETH_MAC_INVALID                303U

/** An error has been detected during a read operation on the Ethernet interface.
 */
#define IO_E_ETH_READ_FAIL                  304U

/** An error has been detected during a write operation on the Ethernet interface.
 */
#define IO_E_ETH_WRITE_FAIL                 305U

/** No valid link has been established on the Ethernet interface.
 */
#define IO_E_ETH_NO_LINK                    306U

/** A timeout has been detected during a read operation on the MDIO interface.
 */
#define IO_E_ETH_MDIO_TIMEOUT               307U

/** An error has been detected during a read operation on the MDIO interface.
 */
#define IO_E_ETH_MDIO_READ                  308U



/* Download error codes */

/** No valid request from the TTC-Downloader has been detected.
 */
#define IO_E_DOWNLOAD_NO_REQ                310U

/** A timeout as been detected when establishing the handshake with the TTC-Downloader.
 */
#define IO_E_DOWNLOAD_TIMEOUT               311U

/** An error as been detected when establishing the handshake with the TTC-Downloader.
 */
#define IO_E_DOWNLOAD_HANDSHAKE             312U


/* Shared Memory error codes */

/** An error as been detected when verifying the shared memory's integrity.
 */
#define IO_E_SHM_INTEGRITY                  320U


/* MPU error codes */

/** \c IO_MPU_Enable() was called on a region which is already enabled.
 */
#define IO_E_MPU_REGION_ENABLED             330U

/** \c IO_MPU_Disable() was called on a region which is already disabled.
 */
#define IO_E_MPU_REGION_DISABLED            331U


/* Error codes related to variant configuration */

/** The option encoded in the variant configuration table is not supported.
 */
#define IO_E_OPTION_NOT_SUPPORTED           340U

/* UDP error codes                         */
/** no more UDP sockets are available
 *
 */
#define IO_E_UDP_NOMORESOCKETS       440U

/**  overflow of message object
 *
 */
#define IO_E_UDP_OVERFLOW          442U

/** data overflow on receive, tx data overflow  on send
 *
 */
#define IO_E_UDP_WRONG_HANDLE     443U

/**  socket not initialized
 *
 */
#define IO_E_SOCKET_NOT_INITIALIZED      444U

/**  IP address or port do not fit
 *
 */
#define IO_E_WRONG_ADDRESS             445U

/** invalid buffer has been passed
 *
 */
#define IO_E_UDP_INVALID_BUFFER     446U

/**  invalid port; only relevant for SafeRTOS
 *
 */
#define IO_E_UDP_WRONG_PORT      447U


/**  ARP package received
 *
 */
#define IO_E_UDP_ARP_RECEIVED    449U

#ifdef __cplusplus
}
#endif

#endif /* IO_ERROR_H_ */
