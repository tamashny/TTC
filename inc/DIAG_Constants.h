/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************//**
 * \file DIAG_Constants.h
 *
 * \latexonly \hypertarget{DIAG_Constants}{} \endlatexonly
 *
 * \brief Global defines for IO Driver diagnostic module
 *
 *  This header file defines the Error Codes for the diagnostic and watchdog modules.
 *
 * \section diag_error_reporting Diagnostic state machine error reporting
 * \brief Details about the error reporting of the diagnostic state machine.
 *
 * Errors of the diagnostic state machine are either returned by the DIAG_Status() function or by the callback
 * functions (#DIAG_ERROR_CB and #DIAG_NOTIFY_CB).
 *
 * Each reported error consists of an error code, a device number and a faulty value (see \ref DIAG_ERRORCODE). The
 * device number specifies the source of the error. This can either be an internal device (see \ref diag_devices
 * "Diagnostic devices") or an I/O pin (see \ref io_pins "Connector pins"). The faulty value depends on the reported
 * error code (see \ref diag_errors for a list of error codes).
 *
 * \section diag_errors Diagnostic state machine error codes
 * \brief Details about the errors of the diagnostic state machine.
 *
 * The following table gives an overview of the error codes and their types.
 *
 * There are <b>four types of errors</b>:
 *  - <b>Persistent fatal errors</b>\n
 *    If a fatal error is detected, the diagnostic state machine always activates the safe state. The application
 *    will be informed about the error before a reset occurs (via the notification callback, see #DIAG_NOTIFY_CB).
 *  - <b>Persistent non-fatal errors</b>\n
 *    For non-fatal errors the application can decide which action to take
 *    (via the error callback, see #DIAG_ERROR_CB).
 *  - <b>Temporary fatal errors</b>\n
 *    For these errors a glitch filter (de-bounce) is implemented, which means that they are
 *    reported after the configured anti-glitch time. After the glitch filter time has expired,
 *    the error will be treated like a persistent fatal error.
 *  - <b>Temporary non-fatal errors</b>\n
 *    For these errors a glitch filter (de-bounce) is implemented, which means that they are
 *    reported after the configured anti-glitch time. After the glitch filter time has expired,
 *    the error will be treated like a persistent non-fatal error.
 *
 * \verbatim[table]
 * ======== =================================== ======== ============ =========================================================
 *  Number   Name                                Fatal?   Temporary?   Short description
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    0      #DIAG_E_NOERROR                                           No error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    1      #DIAG_E_ADC_3MODE_SWITCH_TEST         no         no       Start-up test error with ADC mode selection switches
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    2      #DIAG_E_ADC_3MODE_SWITCH_PERIODIC     no         yes      Run-time test error with ADC mode selection switches
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    3      #DIAG_E_ADC_2MODE_RED_CHANNEL_TEST    no         no       Redundant channel start-up test error of 2 mode ADCs
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    4      #DIAG_E_ADC_SR_CONF_CHECK             yes        no       Internal shift register readback error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    5      #DIAG_E_ADC_RANGE                     no         yes      ADC measurement range check error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    6      #DIAG_E_ADC_UBAT                      yes        yes      Battery voltage out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    7      #DIAG_E_ADC_BOARD_TEMP                yes        yes      Board temperature out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    8      #DIAG_E_ADC_SENSOR_SUPPLY             no         yes      Sensor supply voltage out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *    9      #DIAG_E_ADC_2V5_REF                   yes        yes      Internal 2V5 reference voltage out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   10      #DIAG_E_ADC_1V2                       yes        yes      Internal 1V2 supply voltage out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   11      #DIAG_E_ADC_VPGATE                    yes        yes      Internal VP Gate voltage out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   12      #DIAG_E_PWM_SHORT_CIRCUIT             no         no       PWM start-up test detected a short circuit
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   13      #DIAG_E_PWM_OPEN_LOAD                 no         no       PWM start-up test detected an open load
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   14      #DIAG_E_PWM_FEEDBACK                  no         yes      PWM pulse or period feedback out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   15      #DIAG_E_PWM_CURRENT                   no         yes      PWM current check detected an overcurrent
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   16      #DIAG_E_DO_SHORT_CIRCUIT              no         no       DO start-up test detected a short circuit
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   17      #DIAG_E_DO_OPEN_LOAD                  no         no       DO check detected an open load
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   18      #DIAG_E_DO_FEEDBACK                   no         yes      DO feedback value out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   19      #DIAG_E_PWD_RANGE                     no         yes      PWD counter value, frequency or pulse width out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   20      #DIAG_E_PWD_CURRENT                   no         yes      PWD current check error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   21      #DIAG_E_PWD_THRESH                    no         yes      PWD threshold analog feedback out of range
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   22      #DIAG_E_SSW_TEST                      no         no       Safety switch start-up test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   23      #DIAG_E_SSW_PERIODIC                  no         yes      Safety switch periodic check error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   24      #DIAG_E_SSW_EXT_SHUTOFF               no         yes      Safety switch external shut-off redundancy check error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   25      #DIAG_E_VMON_TEST                     yes        no       Internal voltage monitor start-up test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   26      #DIAG_E_VMON_PERIODIC                 yes        yes      Internal voltage monitor periodic check error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   27      #DIAG_E_ENABLE_TREE_TEST              yes        no       Enable tree start-up test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   28      #DIAG_E_WD_INIT                       yes        no       Watchdog initialization failed
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   29      #DIAG_E_WD_ACTIVATION                 yes        no       Watchdog activation failed
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   30      #DIAG_E_WD_TRIGGER                    yes        no       Watchdog triggering failed
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   31      #DIAG_E_INIT_CORE_L2L3                yes        no       L2L3 interconnect test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   32      #DIAG_E_INIT_CORE_EFUSE_ECC           yes        no       eFuse controller test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   33      #DIAG_E_INIT_CORE_FLASH_WR_ECC        yes        no       Flash Module Controller - ECC logic test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   34      #DIAG_E_INIT_CORE_STC_TEST            yes        no       CPU self-test controller test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   35      #DIAG_E_INIT_CORE_SELFTEST            yes        no       CPU self-test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   36      #DIAG_E_INIT_CORE_PSCON_SELFTEST      yes        no       Diagnostic power state controller test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   37      #DIAG_E_INIT_CORE_PBIST_TEST          yes        no       Programmable built-in self-test controller test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   38      #DIAG_E_INIT_CORE_RAM_ECC_B0          yes        no       CPU ECC logic for accesses to TCRAM B0 test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   39      #DIAG_E_INIT_CORE_RAM_ECC_B1          yes        no       CPU ECC logic for accesses to TCRAM B1 test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   40      #DIAG_E_INIT_CORE_FLASH_DATA_ECC      yes        no       CPU ECC logic accesses to program flash test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   41      #DIAG_E_INIT_CORE_CFG_FLASH_ECC       yes        no       CPU ECC logic accesses to configuration flash test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   42      #DIAG_E_INIT_CORE_IOMM_LOCK           yes        no       IOMM Lock test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   43      #DIAG_E_INIT_CORE_CCM_SELFTEST        yes        no       CCM-R4F module test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   44      #DIAG_E_INIT_CORE_ADD_DECODE_B0       yes        no       TCRAM B0 address redundant comparator test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   45      #DIAG_E_INIT_CORE_ADD_DECODE_B1       yes        no       TCRAM B1 address redundant comparator test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   46      #DIAG_E_INIT_CORE_DCC1_SELFTEST       yes        no       DCC1 self-test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   47      #DIAG_E_INIT_CORE_DCC2_SELFTEST       yes        no       DCC2 self-test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   48      #DIAG_E_INIT_CORE_ERROR_PIN_TEST      yes        no       Error pin test error during start-up
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   49      #DIAG_E_INIT_CORE_MPU_TEST            yes        no       MPU test error during start-up
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   50      #DIAG_E_INIT_CORE_RAM_PARITY_TEST     yes        no       RAM parity test error during start-up
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   51      #DIAG_E_INIT_CORE_RAM_PBIST           yes        no       RAM self-test error during start-up
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   52      #DIAG_E_CORE_READBACK                 yes        no       Configuration read back error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   53      #DIAG_E_APPL_SAFE_STATE               yes        no       Application requested to activate the safe state
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   54      #DIAG_E_DRIVER_INIT                   yes        no       Error detected during the execution of IO_Driver_Init()
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   55      #DIAG_E_DATA_ABORT                    yes        no       CPU generated a data abort
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   56      #DIAG_E_PREFETCH_ABORT                yes        no       CPU generated a prefetch abort
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   57      #DIAG_E_UNDEF_INSTRUCTION             yes        no       CPU generated an undefined instruction exception
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   58      #DIAG_E_ESM_HLI                       yes        no       ESM exception generated - fatal High Level Interrupt
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   59      #DIAG_E_ESM_LLI                       yes        no       ESM exception generated - fatal Low Level Interrupt
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   60      #DIAG_E_ESM_LLI_CALLBACK              no         no       ESM exception generated - non-fatal Low Level Interrupt
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   61      #DIAG_E_PARITIY_FALLBACK              yes        no       CPU generated an interrupt vector parity fallback exception
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   62      #DIAG_E_INVALID_DIAG_STATE            yes        no       Invalid state for diagnostic state machine
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   63      #DIAG_E_INVALID_IRQ                   yes        no       Invalid interrupt detected
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   64      #DIAG_E_PRG_OVERFLOW                  yes        no       NHET program overflow error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   65      #DIAG_E_MAIN_LOOP                     yes        no       The application returned from the `main()` function
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   66      #DIAG_E_INIT_CORE_IOMM_PROT_TEST      yes        no       IOMM lock test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   67      #DIAG_E_INIT_CORE_PLL1_SLIP_TEST      yes        no       PLL1 slip test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   68      #DIAG_E_INIT_CORE_PLL2_SLIP_TEST      yes        no       PLL2 slip test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   69      #DIAG_E_INIT_CORE_OSC_FAIL_TEST       yes        no       Main oscillator test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   70      #DIAG_E_INIT_CORE_ADD_BUS_PAR_B0      yes        no       TCRAM B0 address bus parity test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   71      #DIAG_E_INIT_CORE_ADD_BUS_PAR_B1      yes        no       TCRAM B1 address bus parity test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   72      #DIAG_E_INIT_CORE_FLASH_BUS1_PAR      yes        no       Flash address bus1 parity test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   73      #DIAG_E_INIT_CORE_DMA_BASIC_TEST      yes        no       DMA Basic functionality test error
 * -------- ----------------------------------- -------- ------------ ---------------------------------------------------------
 *   74      #DIAG_E_ERROR_CALLBACK_RECURSION      yes        no       An error callback recursion has been detected
 * ======== =================================== ======== ============ =========================================================
 * \endverbatim
 **************************************************************************/
#ifndef DIAG_CONSTANTS_H_
#define DIAG_CONSTANTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "ptypes_tms570.h"
#include "IO_Pin.h"

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**
 *
 * \brief Diagnostic error code structure
 *
 *  Stores all relevant error parameters returned from the diagnostic
 *  state machine or returned from the WD. See \ref diag_error_reporting
 *  for details about the diagnostic error reporting mechanism.
 *
 */
typedef struct diag_errorcode_
{
    ubyte1 error_code;          /**< The error code (see \ref diag_errors). */
    ubyte1 device_num;          /**< The device number which caused the
                                     error. This can either be an internal
                                     device (see \ref diag_devices
                                     "Diagnostic devices") or an I/O pin
                                     (see \ref io_pins "Connector pins").   */
    ubyte4 faulty_value;        /**< The value which caused the error       */
} DIAG_ERRORCODE;

/**
 * \brief Callback function for non-fatal errors.
 *
 * If a non-fatal error occurs, the diagnostic state machine
 * asks the application which action to take by calling
 * this callback function.
 * The callback function is passed to the function IO_Driver_Init()
 * in a field of the safety configuration (#IO_DRIVER_SAFETY_CONF).
 *
 * Please refer to \ref diag_errors "Diagnostic state machine error codes" for
 * details on the error types and how they are treated by the diagnostic state machine.
 *
 * \param diag_state      current state of the diagnostic state machine
 * \param watchdog_state  current state of the watchdog CPU
 * \param[out] error      error code structure, describing the detected error
 *
 * \return #ubyte2
 *
 * Except for #DIAG_ERR_NOACTION and #DIAG_ERR_SAFESTATE all values may be combined (OR- bitmask).
 * \retval #DIAG_ERR_NOACTION        take no action (ignore the error)
 * \retval #DIAG_ERR_SAFESTATE       enter the safe state (switch off all outputs)
 * \retval #DIAG_ERR_DISABLE_SSW0    disable the shut-off group 0 (\c #IO_INT_SAFETY_SW_0)
 * \retval #DIAG_ERR_DISABLE_SSW1    disable the shut-off group 1 (\c #IO_INT_SAFETY_SW_1)
 * \retval #DIAG_ERR_DISABLE_SSW2    disable the shut-off group 2 (\c #IO_INT_SAFETY_SW_2)
 * \retval #DIAG_ERR_DISABLE_HS00    disable the high side digital output 0 (\c #IO_DO_00)
 * \retval #DIAG_ERR_DISABLE_HS01    disable the high side digital output 1 (\c #IO_DO_01)
 * \retval #DIAG_ERR_DISABLE_HS02    disable the high side digital output 2 (\c #IO_DO_02)
 * \retval #DIAG_ERR_DISABLE_HS03    disable the high side digital output 3 (\c #IO_DO_03)
 * \retval #DIAG_ERR_DISABLE_HS04    disable the high side digital output 4 (\c #IO_DO_04)
 * \retval #DIAG_ERR_DISABLE_HS05    disable the high side digital output 5 (\c #IO_DO_05)
 * \retval #DIAG_ERR_DISABLE_HS06    disable the high side digital output 6 (\c #IO_DO_06)
 * \retval #DIAG_ERR_DISABLE_HS07    disable the high side digital output 7 (\c #IO_DO_07)
 *
 * \remarks
 *   See \ref safety_cb_example "Example implementation for Safety-Callback".
 *
 */
typedef ubyte2 (* DIAG_ERROR_CB)( ubyte1 diag_state,
                                  ubyte1 watchdog_state,
                                  DIAG_ERRORCODE * const error );

/**
 * \brief Callback notification function for fatal errors.
 *
 * If a fatal error occurs, the diagnostic state machine notifies the application
 * about the error occurrence by calling this callback function.
 * The implementation of this callback is passed to \c IO_Driver_Init()
 * in the \c notify_callback field of the safety configuration structure #IO_DRIVER_SAFETY_CONF.
 *
 * The safe state is activated \b before this callback is called.
 * If the I/O Driver is configured with resets (field \c reset_behavior in #IO_DRIVER_SAFETY_CONF),
 * a reset is expected after 22 ms.
 *
 * Please refer to \ref diag_errors "Diagnostic state machine error codes" for
 * details on the error types and how they are treated by the diagnostic state machine.
 *
 * \param diag_state      current state of the diagnostic state machine
 * \param watchdog_state  current state of the watchdog CPU
 * \param[out] error      error code structure, describing the detected error
 *
 */
typedef void (* DIAG_NOTIFY_CB)( ubyte1 diag_state,
                                 ubyte1 watchdog_state,
                                 DIAG_ERRORCODE * const error );

/**************************************************************************
 *
 * D E F I N I I T O N S
 *
 **************************************************************************/

/*
 * D I A G N O S T I C   S T A T E   M A C H I N E   S T A T E S
 */

/**
 * \name States of the diagnostic state machine
 * \anchor diag_states
 *
 * Diagnostic state information returned by the function DIAG_Status()
 *
 * @{
 */
#define DIAG_STATE_DISABLED             0x00UL  /**< Diagnostic state machine is disabled        */
#define DIAG_STATE_INIT                 0x03UL  /**< Diagnostic state machine is in init state   */
#define DIAG_STATE_CONFIG               0x05UL  /**< Diagnostic state machine is in config state */
#define DIAG_STATE_MAIN                 0x06UL  /**< Diagnostic state machine is in main state   */
#define DIAG_STATE_SAFE                 0x09UL  /**< Diagnostic state machine is in safe state   */
/** @} */


/*
 * W A T C H D O G   C P U   S T A T E S
 */

/**
 * \name States of the watchdog CPU
 * \anchor diag_wd_states
 *
 * Watchdog state information returned by the function DIAG_Status()
 *
 * @{
 */
#define DIAG_WD_STATE_STANDBY           0x01U  /**< Watchdog CPU is in standby state    */
#define DIAG_WD_STATE_RESET             0x03U  /**< Watchdog CPU is in reset state      */
#define DIAG_WD_STATE_DIAGNOSTIC        0x05U  /**< Watchdog CPU is in diagnostic state */
#define DIAG_WD_STATE_ACTIVE            0x06U  /**< Watchdog CPU is in active state     */
#define DIAG_WD_STATE_SAFE              0x09U  /**< Watchdog CPU is in safe state       */
#define DIAG_WD_STATE_UNKNOWN           0xFFU  /**< Watchdog CPU is in an unknown state */
/** @} */


/*
 * D I A G N O S T I C   S T A T E   M A C H I N E   E R R O R   V A L U E S
 */

/**
 * \name Diagnostic state machine error values
 * \anchor diag_err_values_doc
 *
 * These errors codes are used by the function DIAG_Status() and by the notify and error callbacks
 * in the parameter `error_code` of the structure #DIAG_ERRORCODE.
 *
 * @{
 */

/** No error */
#define DIAG_E_NOERROR                          0U

/**
 * Start-up test error with ADC mode selection switches
 *
 * If this error is raised, the ADC channel in the `error_device` parameter is not functional.
 *
 *  - Error device: pin number of 3 mode ADC input whose start-up test failed (#IO_ADC_00..#IO_ADC_07)
 */
#define DIAG_E_ADC_3MODE_SWITCH_TEST            1U

/**
 * Run-time test error with ADC mode selection switches
 *
 * If this error is raised, the ADC channel in the `error_device` parameter is not functional anymore.
 *
 *  - Error device: pin number of 3 mode ADC input whose periodic test failed (#IO_ADC_00..#IO_ADC_07)
 */
#define DIAG_E_ADC_3MODE_SWITCH_PERIODIC        2U

/**
 * Redundant channel start-up test error of 2 mode ADCs
 *
 * This error is raised if the redundant channel configuration doesn't comply
 * with the rules in the description of #IO_ADC_SAFETY_CONF.
 *
 *  - Error device: #DIAG_DEV_ADC
 *  - Faulty value: number of safety critical 2 mode ADC inputs
 * \par
 *  - Error device: pin number of 2 mode ADC input (#IO_ADC_08..#IO_ADC_23)
 *  - Faulty value: pin number of related ADC input with incorrect configuration
 */
#define DIAG_E_ADC_2MODE_RED_CHANNEL_TEST       3U

/**
 * Internal shift register readback error
 */
#define DIAG_E_ADC_SR_CONF_CHECK                4U

/**
 * ADC measurement range check error
 *
 * This error is raised when the ADC value falls outside the range defined
 * in the safety configuration of the pin.
 *
 *  - Error device: pin number of safe ADC input (#IO_ADC_00..#IO_ADC_07, #IO_ADC_08..#IO_ADC_23)
 *  - Faulty value: last reading of ADC input in physical units (mV/uA/ohm)
 */
#define DIAG_E_ADC_RANGE                        5U

/**
 * Battery voltage out of range
 *
 *  - Error device: #IO_ADC_UBAT
 *  - Faulty value: CPU supply voltage in mV
 */
#define DIAG_E_ADC_UBAT                         6U

/**
 * Board temperature out of range
 *
 *  - Error device: #IO_ADC_BOARD_TEMP
 *  - Faulty value: raw ADC value of board temperature (value can be converted with
 *                  IO_ADC_BoardTempFloat() or IO_ADC_BoardTempSbyte() )
 */
#define DIAG_E_ADC_BOARD_TEMP                   7U

/**
 * Sensor supply voltage out of range
 *
 *  - Error device: pin number of safety-critical sensor supply
 *                  (#IO_SENSOR_SUPPLY_0 or #IO_SENSOR_SUPPLY_1)
 */
#define DIAG_E_ADC_SENSOR_SUPPLY                8U

/**
 * Internal 2V5 reference voltage out of range
 */
#define DIAG_E_ADC_2V5_REF                      9U

/**
 * Internal 1V2 supply voltage out of range
 */
#define DIAG_E_ADC_1V2                         10U

/**
 * Internal VP Gate voltage out of range
 */
#define DIAG_E_ADC_VPGATE                      11U

/**
 * PWM start-up test detected a short circuit
 *
 *  - Error device: pin number of safe PWM channel which has a short circuit  (#IO_PWM_00..#IO_PWM_35)
 *  - Faulty value: pin number of one of the pins which the error device is incorrectly connected to
 */
#define DIAG_E_PWM_SHORT_CIRCUIT               12U

/**
 * PWM start-up test detected an open load
 *
 *  - Error device: pin number of safe PWM channel which has an open load (#IO_PWM_00..#IO_PWM_35)
 *  - Faulty value: 0
 */
#define DIAG_E_PWM_OPEN_LOAD                   13U

/**
 * PWM pulse or period feedback out of range
 *
 *  - Error device: pin number of safe PWM channel (#IO_PWM_00..#IO_PWM_35)
 *  - Faulty value:
 *     * 0xFFFFFFFF (4,294,967,295) if IO_PWM_SetDuty() hasn't been called recently, otherwise
 *     * bits 0..15: duty cycle feedback in microseconds
 *     * bits 16..31: period feedback in microseconds
 */
#define DIAG_E_PWM_FEEDBACK                    14U

/**
 * PWM current check detected an overcurrent
 *
 *  - Error device: pin number of safe PWM channel (#IO_PWM_00..#IO_PWM_35)
 *  - Faulty value:
 *     * 0xFFFF (65535) if neither IO_PWM_GetCur() nor IO_PWM_GetCurQueue() have been called recently
 *     * otherwise the PWM current in milliamperes, as returned by IO_PWM_GetCur() or IO_PWM_GetCurQueue()
 */
#define DIAG_E_PWM_CURRENT                     15U

/**
 * DO start-up test detected a short circuit
 *
 *  - Error device: pin number of safe high-side or low-side digital output which has a short circuit
 *                  (#IO_DO_00..#IO_DO_07, #IO_DO_08..#IO_DO_15)
 *  - Faulty value: pin number of one of the pins which the error device is incorrectly connected to
 */
#define DIAG_E_DO_SHORT_CIRCUIT                16U

/**
 * DO check detected an open load
 *
 *  - Error device: pin number of safe high-side digital output which has an open load (#IO_DO_00..#IO_DO_07)
 *  - Faulty value: 0
 */
#define DIAG_E_DO_OPEN_LOAD                    17U

/**
 * DO feedback value out of range
 *
 *  - Error device: pin number of safe high-side digital output (#IO_DO_00..#IO_DO_07)
 *  - Faulty value: error code returned by IO_DO_Set()
 */
#define DIAG_E_DO_FEEDBACK                     18U

/**
 * PWD counter value, frequency or pulse width out of range
 *
 *  - Error device: pin number of safe PWD input (#IO_PWD_00..#IO_PWD_05)
 *  - Faulty value:
 *     * 0xFFFFFFFF (4,294,967,295) if IO_PWD_ComplexGet() or IO_PWD_UniversalGet() was not called recently
 *     * if bit 31 is set, bits 0..27 denote the PWD counter value which falls outside the defined safety range
 *     * if bit 30 is set, bits 0..27 denote the PWD frequency value which falls outside the defined safety range
 *     * if bit 29 is set, bits 0..27 denote the PWD pulse width value which falls outside the defined safety range
 */
#define DIAG_E_PWD_RANGE                       19U

/**
 * PWD current check error
 *
 * This error is thrown if a PWD channel was initialized as a current input
 * (with #IO_PWD_PD_90 pull resistor) and its current is outside the range of 4..20 mA,
 * or if the #IO_PWD_GetCurrent() function was not called recently.
 *
 *  - Error device: pin number of safe PWD input (#IO_PWD_00..#IO_PWD_05)
 */
#define DIAG_E_PWD_CURRENT                     20U

/**
 * PWD threshold analog feedback out of range
 *
 * This error means that digital reading on any PWD input (neither on #IO_PWD_00..#IO_PWD_05
 * nor on #IO_PWD_06..#IO_PWD_11) cannot be trusted because the comparator threshold used
 * for digitalizing the analog signals falls outside the safe range.
 *
 * This error doesn't affect the analog reading of the PWD inputs.
 *
 *  - Error device: #DIAG_DEV_PWD
 */
#define DIAG_E_PWD_THRESH                      21U

/**
 * Safety switch start-up test error
 *
 *  - Error device: pin number of safety critical safety switch (#IO_INT_SAFETY_SW_0..#IO_INT_SAFETY_SW_2)
 *  - Faulty value:
 *     * 0 if the safety switch cannot be switched on
 *     * 1 if the safety switch cannot be switched off
 */
#define DIAG_E_SSW_TEST                        22U

/**
 * Safety switch periodic check error
 *
 *  - Error device: pin number of safety critical safety switch (#IO_INT_SAFETY_SW_0..#IO_INT_SAFETY_SW_2)
 */
#define DIAG_E_SSW_PERIODIC                    23U

/**
 * Safety switch external shut-off redundancy check error
 *
 * This error is raised if the two inputs of an external shut-off group have an invalid reading
 * (both low or both high).
 *
 *  - Error device: #DIAG_DEV_EXT_SHUTOFF_0..#DIAG_DEV_EXT_SHUTOFF_2
 *  - Faulty value:
 *     * bits 0..15: analog reading of input 1 of the given external shut-off group, in millivolts
 *     * bits 16..31: analog reading of input 0 of the given external shut-off group, in millivolts
 */
#define DIAG_E_SSW_EXT_SHUTOFF                 24U

/**
 * Internal voltage monitor start-up test error
 */
#define DIAG_E_VMON_TEST                       25U

/**
 * Internal voltage monitor periodic check error
 */
#define DIAG_E_VMON_PERIODIC                   26U

/**
 * Enable tree start-up test error
 */
#define DIAG_E_ENABLE_TREE_TEST                27U

/**
 * Watchdog initialization failed
 *
 *  - Error device: #DIAG_DEV_MAIN_CPU or #DIAG_DEV_WATCHDOG_CPU
 *  - Faulty value: one of the error definitions in \ref IO_Error.h "IO_Error.h"
 */
#define DIAG_E_WD_INIT                         28U

/**
 * Watchdog activation failed
 *
 *  - Error device: #DIAG_DEV_WATCHDOG_CPU
 *  - Faulty value: one of the error definitions in \ref IO_Error.h "IO_Error.h"
 */
#define DIAG_E_WD_ACTIVATION                   29U

/**
 * Watchdog triggering failed
 *
 *  - Error device: #DIAG_DEV_MAIN_CPU or #DIAG_DEV_WATCHDOG_CPU,
 *  - Faulty value: one of the error definitions in \ref IO_Error.h "IO_Error.h"
 */
#define DIAG_E_WD_TRIGGER                      30U

/**
 * L2L3 interconnect test error
 */
#define DIAG_E_INIT_CORE_L2L3                  31U

/**
 * eFuse controller test error
 */
#define DIAG_E_INIT_CORE_EFUSE_ECC             32U

/**
 * Flash Module Controller - ECC logic test error
 */
#define DIAG_E_INIT_CORE_FLASH_WR_ECC          33U

/**
 * CPU self-test controller test error
 */
#define DIAG_E_INIT_CORE_STC_TEST              34U

/**
 * CPU self-test error
 */
#define DIAG_E_INIT_CORE_SELFTEST              35U

/**
 * Diagnostic power state controller test error
 */
#define DIAG_E_INIT_CORE_PSCON_SELFTEST        36U

/**
 * Programmable built-in self-test controller test error
 */
#define DIAG_E_INIT_CORE_PBIST_TEST            37U

/**
 * CPU ECC logic for accesses to TCRAM B0 test error
 */
#define DIAG_E_INIT_CORE_RAM_ECC_B0            38U

/**
 * CPU ECC logic for accesses to TCRAM B1 test error
 */
#define DIAG_E_INIT_CORE_RAM_ECC_B1            39U

/**
 * CPU ECC logic accesses to program flash test error
 */
#define DIAG_E_INIT_CORE_FLASH_DATA_ECC        40U

/**
 * CPU ECC logic accesses to configuration flash test error
 */
#define DIAG_E_INIT_CORE_CFG_FLASH_ECC         41U

/**
 * IOMM Lock test error
 */
#define DIAG_E_INIT_CORE_IOMM_LOCK             42U

/**
 * CCM-R4F module test error
 */
#define DIAG_E_INIT_CORE_CCM_SELFTEST          43U

/**
 * TCRAM B0 address redundant comparator test error
 */
#define DIAG_E_INIT_CORE_ADD_DECODE_B0         44U

/**
 * TCRAM B1 address redundant comparator test error
 */
#define DIAG_E_INIT_CORE_ADD_DECODE_B1         45U

/**
 * DCC1 self-test error
 */
#define DIAG_E_INIT_CORE_DCC1_SELFTEST         46U

/**
 * DCC2 self-test error
 */
#define DIAG_E_INIT_CORE_DCC2_SELFTEST         47U

/**
 * Error pin test error during start-up
 */
#define DIAG_E_INIT_CORE_ERROR_PIN_TEST        48U

/**
 * MPU test error during start-up
 */
#define DIAG_E_INIT_CORE_MPU_TEST              49U

/**
 * RAM parity test error during start-up
 */
#define DIAG_E_INIT_CORE_RAM_PARITY_TEST       50U

/**
 * RAM self-test error during start-up
 */
#define DIAG_E_INIT_CORE_RAM_PBIST             51U

/**
 * Configuration read back error
 */
#define DIAG_E_CORE_READBACK                   52U

/**
 * Application requested to activate the safe state
 *
 *  - Error device: #DIAG_DEV_MAIN_CPU
 *  - Faulty value: 0
 */
#define DIAG_E_APPL_SAFE_STATE                 53U

/**
 * Error detected during the execution of IO_Driver_Init()
 *
 *  - Error device: #DIAG_DEV_MAIN_CPU
 *  - Faulty value: one of the error definitions in \ref IO_Error.h "IO_Error.h"
 */
#define DIAG_E_DRIVER_INIT                     54U

/**
 * CPU generated a data abort
 *
 * When a data abort is generated due to MPU violation, the error device is #DIAG_DEV_MAIN_CPU.
 *
 *  - Error device:
 *     * #DIAG_DEV_MAIN_CPU if the source of the abort is the CPU core,
 *     * otherwise one of the diagnostic error devices starting with "DIAG_DEV_ESM_"
 *       (e.g. #DIAG_DEV_ESM_UNKNOWN)
 *  - Faulty value: holds the address of the data which caused a data abort
 */
#define DIAG_E_DATA_ABORT                      55U

/**
 * CPU generated a prefetch abort
 *
 *  - Error device:
 *     * #DIAG_DEV_MAIN_CPU if the source of the abort is the CPU core,
 *     * otherwise one of the diagnostic error devices starting with "DIAG_DEV_ESM_"
 *       (e.g. #DIAG_DEV_ESM_UNKNOWN)
 *  - Faulty value: holds the address of the instruction that caused a prefetch abort
 */
#define DIAG_E_PREFETCH_ABORT                  56U

/**
 * CPU generated an undefined instruction exception
 *
 *  - Error device: #DIAG_DEV_MAIN_CPU
 *  - Faulty value: 0
 */
#define DIAG_E_UNDEF_INSTRUCTION               57U

/**
 * ESM exception generated - fatal High Level Interrupt
 */
#define DIAG_E_ESM_HLI                         58U

/**
 * ESM exception generated - fatal Low Level Interrupt
 */
#define DIAG_E_ESM_LLI                         59U

/**
 * ESM exception generated - non-fatal Low Level Interrupt
 *
 *  - Error device: #DIAG_DEV_ESM_FMC_CFG_FLASH_UNC_ERR
 *  - Faulty value:
 *     * 0xFFFFFFFF if the SECDED loses the capability to properly correct
 *       a 1-bit ECC error for access to EEPROM region
 *     * else the address of the last uncorrectable error in the configuration flash
 */
#define DIAG_E_ESM_LLI_CALLBACK                60U

/**
 * CPU generated an interrupt vector parity fallback exception
 */
#define DIAG_E_PARITIY_FALLBACK                61U

/**
 * Invalid state for diagnostic state machine
 */
#define DIAG_E_INVALID_DIAG_STATE              62U

/**
 * Invalid interrupt detected
 */
#define DIAG_E_INVALID_IRQ                     63U

/**
 * NHET program overflow error
 */
#define DIAG_E_PRG_OVERFLOW                    64U

/**
 * The application returned from the `main()` function
 *
 *  - Error device: #DIAG_DEV_MAIN_CPU
 *  - Faulty value: 0
 */
#define DIAG_E_MAIN_LOOP                       65U

/**
 * IOMM lock test error
 */
#define DIAG_E_INIT_CORE_IOMM_PROT_TEST        66U

/**
 * PLL1 slip test error
 */
#define DIAG_E_INIT_CORE_PLL1_SLIP_TEST        67U

/**
 * PLL2 slip test error
 */
#define DIAG_E_INIT_CORE_PLL2_SLIP_TEST        68U

/**
 * Main oscillator test error
 */
#define DIAG_E_INIT_CORE_OSC_FAIL_TEST         69U

/**
 * TCRAM B0 address bus parity test error
 */
#define DIAG_E_INIT_CORE_ADD_BUS_PAR_B0        70U

/**
 * TCRAM B1 address bus parity test error
 */
#define DIAG_E_INIT_CORE_ADD_BUS_PAR_B1        71U

/**
 * Flash address bus1 parity test error
 */
#define DIAG_E_INIT_CORE_FLASH_BUS1_PAR        72U

/**
 * DMA Basic functionality test error
 */
#define DIAG_E_INIT_CORE_DMA_BASIC_TEST        73U

/**
 * An error callback recursion has been detected
 */
#define DIAG_E_ERROR_CALLBACK_RECURSION        74U
/** @} */


/*
 * D I A G   D E V I C E S
 */

/**
 * \name Diagnostic devices
 * \anchor diag_devices
 *
 * Additional device defines.
 */
#define DIAG_DEV_SAFETY_SW_VP                   IO_INT_PIN_SAFETY_SW_VP /**< Internal VP Gate                          */
#define DIAG_DEV_PWD                            IO_INT_PIN_PWD          /**< PWD threshold comparator                  */
#define DIAG_DEV_REF_2V5                        IO_INT_PIN_REF_2V5      /**< Internal 2.5V reference voltage           */
#define DIAG_DEV_1V2                            IO_INT_PIN_1V2          /**< Internal 1.2V supply voltage              */
#define DIAG_DEV_VMON                           IO_INT_PIN_VMON         /**< Internal voltage monitor                  */


#define DIAG_DEV_ESM                            133U  /**< TMS570 ESM peripheral                                       */
#define DIAG_DEV_VIM                            134U  /**< TMS570 VIM peripheral                                       */
#define DIAG_DEV_ADC                            135U  /**< TMS570 ADC peripheral                                       */
#define DIAG_DEV_SPI                            136U  /**< TMS570 SPI peripheral                                       */
#define DIAG_DEV_DIO                            137U  /**< TMS570 DIO peripheral                                       */
#define DIAG_DEV_RTC                            138U  /**< TMS570 RTC peripheral                                       */
#define DIAG_DEV_DMA                            139U  /**< TMS570 DMA peripheral                                       */
#define DIAG_DEV_NHET                           140U  /**< TMS570 NHET peripheral                                      */

#define DIAG_DEV_ESM_MIBADC2_PARITY             141U  /**< MibADC2 - parity error                                      */
#define DIAG_DEV_ESM_DMA_MPU_VIOLATION          142U  /**< DMA - MPU                                                   */
#define DIAG_DEV_ESM_DMA_PARITY                 143U  /**< DMA - parity error                                          */
#define DIAG_DEV_ESM_DMA_DMM_IMPR_READ          144U  /**< DMA - imprecise read error                                  */
#define DIAG_DEV_ESM_N2HET1_2_PARITY            145U  /**< N2HET1/N2HET2 - parity error                                */
#define DIAG_DEV_ESM_HET_TU1_2_PARITY           146U  /**< HET TU1/HET TU2 - parity error                              */
#define DIAG_DEV_ESM_PLL1_SLIP                  147U  /**< PLL - slip                                                  */
#define DIAG_DEV_ESM_CLOCK_MONITOR              148U  /**< Clock Monitor - interrupt                                   */
#define DIAG_DEV_ESM_DMA_DMM_IMPR_WRITE         149U  /**< DMA - imprecise write error                                 */
#define DIAG_DEV_ESM_VIM_RAM_PARITY             150U  /**< VIM RAM - parity error                                      */
#define DIAG_DEV_ESM_MIBSPI1_PARITY             151U  /**< MibSPI1 - parity error                                      */
#define DIAG_DEV_ESM_MIBSPI3_PARITY             152U  /**< MibSPI3 - parity error                                      */
#define DIAG_DEV_ESM_MIBADC1_PARITY             153U  /**< MibADC1 - parity error                                      */
#define DIAG_DEV_ESM_CPU_SLFTST                 154U  /**< CPU - self-test error                                       */
#define DIAG_DEV_ESM_DCC1_ERROR                 155U  /**< DCC1 - error                                                */
#define DIAG_DEV_ESM_CCM_R4_SLFTST              156U  /**< CCM-R4 - self-test error                                    */
#define DIAG_DEV_ESM_FMC_CFG_FLASH_UNC_ERR      157U  /**< FMC - uncorrectable error (configuration flash bank access) */
#define DIAG_DEV_ESM_IOMM_MUX_CONFIG            158U  /**< IOMM - Mux configuration error                              */
#define DIAG_DEV_ESM_PWR_DOM_CNTL_COMP          159U  /**< Power domain controller compare error                       */
#define DIAG_DEV_ESM_PWR_DOM_CNTL_SLFTST        160U  /**< Power domain controller self-test error                     */
#define DIAG_DEV_ESM_E_FUSE_CNTL                161U  /**< eFuse Controller error                                      */
#define DIAG_DEV_ESM_E_FUSE_CNTL_SLFTST         162U  /**< eFuse Controller - self-test error                          */
#define DIAG_DEV_ESM_PLL2_SLIP                  163U  /**< PLL2 - slip                                                 */
#define DIAG_DEV_ESM_DCC2                       164U  /**< DCC2 - error                                                */
#define DIAG_DEV_ESM_CCMR4_COMPARE              165U  /**< CCMR4 - compare error                                       */
#define DIAG_DEV_ESM_FMC_B1_UNC_ERR             166U  /**< FMC - uncorrectable error (address parity on bus1 accesses) */
#define DIAG_DEV_ESM_RAM_B0_UNC_ERR             167U  /**< RAM even bank (B0TCM) - uncorrectable error                 */
#define DIAG_DEV_ESM_RAM_B1_UNC_ERR             168U  /**< RAM odd bank (B1TCM) - uncorrectable error                  */
#define DIAG_DEV_ESM_RAM_B0_ADDR_PARITY         169U  /**< RAM even bank (B0TCM) - address bus parity error            */
#define DIAG_DEV_ESM_RAM_B1_ADDR_PARITY         170U  /**< RAM odd bank (B1TCM) - address bus parity error             */
#define DIAG_DEV_ESM_FLASH_ECC_LIVE_LOCK        171U  /**< Flash (ATCM) - ECC live lock detect                         */
#define DIAG_DEV_ESM_RTI_WDD_NMI                172U  /**< RTI_WWD_NMI                                                 */
#define DIAG_DEV_ESM_E_FUSE_AUTOLOAD            173U  /**< eFuse Controller - autoload error                           */
#define DIAG_DEV_ESM_RAM_B0_ECC_UNC_ERR         174U  /**< RAM even bank (B0TCM) - ECC uncorrectable error             */
#define DIAG_DEV_ESM_RAM_B1_ECC_UNC_ERR         175U  /**< RAM odd bank (B1TCM) - ECC uncorrectable error              */
#define DIAG_DEV_ESM_FMC_B1_B2_UNC_ERR          176U  /**< FMC - uncorrectable error: bus1 and bus2 interfaces         */
#define DIAG_DEV_ESM_UNKNOWN                    177U  /**< Unknown ESM device                                          */

#define DIAG_DEV_2MODE_CONF_0                   178U  /**< 2Mode ADC Configuration 0 */
#define DIAG_DEV_2MODE_CONF_1                   179U  /**< 2Mode ADC Configuration 1 */
#define DIAG_DEV_2MODE_CONF_2                   180U  /**< 2Mode ADC Configuration 2 */
#define DIAG_DEV_2MODE_CONF_3                   181U  /**< 2Mode ADC Configuration 3 */
#define DIAG_DEV_2MODE_CONF_4                   182U  /**< 2Mode ADC Configuration 4 */
#define DIAG_DEV_2MODE_CONF_5                   183U  /**< 2Mode ADC Configuration 5 */
#define DIAG_DEV_2MODE_CONF_6                   184U  /**< 2Mode ADC Configuration 6 */
#define DIAG_DEV_2MODE_CONF_7                   185U  /**< 2Mode ADC Configuration 7 */

#define DIAG_DEV_PWD_CONF_0                     186U  /**< PWD Configuration 0       */
#define DIAG_DEV_PWD_CONF_1                     187U  /**< PWD Configuration 1       */
#define DIAG_DEV_PWD_CONF_2                     188U  /**< PWD Configuration 2       */
#define DIAG_DEV_PWD_CONF_3                     189U  /**< PWD Configuration 3       */
#define DIAG_DEV_PWD_CONF_4                     190U  /**< PWD Configuration 4       */
#define DIAG_DEV_PWD_CONF_5                     191U  /**< PWD Configuration 5       */

#define DIAG_DEV_DO_CONF_0                      192U  /**< DO Configuration 0        */
#define DIAG_DEV_DO_CONF_1                      193U  /**< DO Configuration 1        */
#define DIAG_DEV_DO_CONF_2                      194U  /**< DO Configuration 2        */

#define DIAG_DEV_EXT_SHUTOFF_0                  195U  /**< External shut-off group 0 */
#define DIAG_DEV_EXT_SHUTOFF_1                  196U  /**< External shut-off group 1 */
#define DIAG_DEV_EXT_SHUTOFF_2                  197U  /**< External shut-off group 2 */

#define DIAG_DEV_WATCHDOG_CPU                   198U  /**< Safety companion          */
#define DIAG_DEV_MAIN_CPU                       199U  /**< Main CPU                  */

#define DIAG_DEV_MCU                            200U  /**< TMS570 MCU peripheral    This item used to be device number 132*/

#define DIAG_DEV_MAX                            201U
#define DIAG_DEV_NONE                           IO_PIN_NONE


/*
 * C A L L B A C K    R E A C T I O N S
 */

/**
 * \name Error callback reaction
 * \anchor diag_err_cb_reaction
 *
 * Allowed return values of error callback function.
 * Except for #DIAG_ERR_NOACTION and #DIAG_ERR_SAFESTATE all values may be combined (OR- bitmask)
 */
#define DIAG_ERR_NOACTION       0x0001U        /**< take no action (ignore the error)                     */
#define DIAG_ERR_SAFESTATE      0x0002U        /**< enter the safe state (switch off all outputs)         */
#define DIAG_ERR_DISABLE_SSW0   0x0004U        /**< disable the shut-off group 0 (\c #IO_INT_SAFETY_SW_0) */
#define DIAG_ERR_DISABLE_SSW1   0x0008U        /**< disable the shut-off group 1 (\c #IO_INT_SAFETY_SW_1) */
#define DIAG_ERR_DISABLE_SSW2   0x0010U        /**< disable the shut-off group 2 (\c #IO_INT_SAFETY_SW_2) */
#define DIAG_ERR_DISABLE_HS00   0x0020U        /**< disable the high side digital output 0 (\c #IO_DO_00) */
#define DIAG_ERR_DISABLE_HS01   0x0040U        /**< disable the high side digital output 1 (\c #IO_DO_01) */
#define DIAG_ERR_DISABLE_HS02   0x0080U        /**< disable the high side digital output 2 (\c #IO_DO_02) */
#define DIAG_ERR_DISABLE_HS03   0x0100U        /**< disable the high side digital output 3 (\c #IO_DO_03) */
#define DIAG_ERR_DISABLE_HS04   0x0200U        /**< disable the high side digital output 4 (\c #IO_DO_04) */
#define DIAG_ERR_DISABLE_HS05   0x0400U        /**< disable the high side digital output 5 (\c #IO_DO_05) */
#define DIAG_ERR_DISABLE_HS06   0x0800U        /**< disable the high side digital output 6 (\c #IO_DO_06) */
#define DIAG_ERR_DISABLE_HS07   0x1000U        /**< disable the high side digital output 7 (\c #IO_DO_07) */

#ifdef __cplusplus
}
#endif

#endif /* DIAG_CONSTANTS_H_ */
