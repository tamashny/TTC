/**************************************************************************************************
 * Copyright (c) 2022 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_Driver.h
 *
 * \latexonly \hypertarget{IO_Driver}{} \endlatexonly
 *
 * \brief High level interface to IO Driver
 *
 *  The IO Driver high level interface provides a general initialization function, a version API
 *  and general task functions which shall wrap the whole user application.
 *
 * \section appl_structure Basic structure of an application
 *
 *  The IO Driver API provides two different types of functions:
 *  - Initialization functions:
 *    These functions are designed to be called once at the beginning of an application.
 *  - Task functions:
 *    These functions are designed to be called periodically at runtime.
 *
 *  The function IO_Driver_Init() needs to be the first function which is called during the
 *  initialization.
 *
 *  All task functions need to be enclosed by the functions IO_Driver_TaskBegin() and
 *  IO_Driver_TaskEnd()
 *
 * \section startup_limits Limitations during startup
 *
 *  During the first cycles some measurement values (eg. ADC, PWD, etc.) are invalid, which is
 *  indicated by the corresponding fresh flag when reading the value. After some cycles the actual
 *  measurement values are available.
 *
 *  Further limitations for safety controllers:
 *  Various tests are performed at startup. For this reason the measurement values are not
 *  available during this process. The application software should wait until the CPU has entered
 *  the Main state before evaluating the input variables. For details on CPU states and how to
 *  retrieve the current state, refer to \ref DIAG_Functions.h "Functions for ECU diagnostics"
 *
 * \section driver_examples IO Driver Code Examples
 *
 * \brief Examples for using the IO Driver API
 * \subsection appl_example Example of an application
 * \code
 * static ubyte4 timestamp = 0;
 *
 * void task (void)
 * {
 *     IO_Driver_TaskBegin();
 *
 *     // User Application
 *     // and calls to driver task functions.
 *
 *     IO_Driver_TaskEnd();
 *
 *     while (IO_RTC_GetTimeUS(timestamp) < 5000);   // wait until 5ms have passed
 *     timestamp += 5000;                            // increase time stamp by cycle time
 *
 *     // Note: If one cycle takes longer than the configured cycle time, for the next
 *     // cycle less time is available. This method helps to prevent a phase shift between
 *     // application runtime and hardware runtime
 * }
 *
 *
 * void main (void)
 * {
 *     ubyte1 ecu_serial[IO_DRIVER_ECU_SERIAL_LENGTH];
 *
 *     //--------------------------------//
 *     // start of driver initialization //
 *     //--------------------------------//
 *
 *     // IO_Driver_Init() is the first function:
 *     IO_Driver_Init(NULL);  // no safety critical application
 *
 *     //------------------------------//
 *     // end of driver initialization //
 *     //------------------------------//
 *
 *     // Read ECU serial number
 *     IO_Driver_GetSerialNumber(ecu_serial);
 *
 *     // Get timestamp
 *     IO_RTC_StartTime(&timestamp);
 *
 *     //----------------------------------------------//
 *     // from now on only the task function is called //
 *     //----------------------------------------------//
 *     while (1)
 *     {
 *         task();
 *     }
 * }
 *
 * \endcode
 *
 * \details The \c task function is called every 5000us = 5ms. Please refer to the \ref IO_RTC.h
 * "Real Time Clock" documentation for details on how to use the RTC functions.
 *
 * \subsection safety_cb_example Example implementation for application Safety-Callback
 *
 * See #DIAG_ERROR_CB for details on the application safety callback function.
 *
 * \code
 * static ubyte2 APPL_SafetyCb(ubyte1 diag_state,
 *                             ubyte1 watchdog_state,
 *                             DIAG_ERRORCODE * const error)
 * {
 *     ubyte2 action;
 *
 *     // Error codes can for example be saved to error memory of application software
 *     // (APPL_WriteErrMem is a hypothetical function of the application software).
 *     APPL_WriteErrMem( diag_state, watchdog_state, error );
 *
 *     // Just a simple example which does
 *     // not take the type of error into account
 *     switch (error->device_num)
 *     {
 *         case IO_ADC_00:
 *         case IO_ADC_01:
 *             // ...
 *             // Do something sophisticated
 *             // ...
 *             action = DIAG_ERR_NOACTION;
 *             break;
 *         case IO_PWM_00:
 *         case IO_PWM_01:
 *         case IO_PWM_02:
 *         case IO_PWM_03:
 *             // ...
 *             // Do something sophisticated
 *             // ...
 *             action = DIAG_ERR_DISABLE_SSW0;
 *             break;
 *         case IO_PWM_04:
 *         case IO_PWM_05:
 *         case IO_PWM_06:
 *         case IO_PWM_07:
 *             // ...
 *             // Do something sophisticated
 *             // ...
 *             action = (DIAG_ERR_DISABLE_SSW0 | DIAG_ERR_DISABLE_SSW1);
 *             break;
 *         case IO_PWM_08:
 *         case IO_PWM_09:
 *         case IO_PWM_10:
 *         case IO_PWM_11:
 *             // ...
 *             // Do something sophisticated
 *             // ...
 *             action = (DIAG_ERR_DISABLE_SSW2 | DIAG_ERR_DISABLE_HS05 | DIAG_ERR_DISABLE_HS06);
 *             break;
 *         case IO_PWM_12:
 *         case IO_PWM_13:
 *         case IO_PWM_14:
 *         case IO_PWM_15:
 *             // ...
 *             // Do something sophisticated
 *             // ...
 *             action = DIAG_ERR_DISABLE_SSW2;
 *             break;
 *         default:
 *             action = DIAG_ERR_SAFESTATE;
 *             break;
 *     }
 *
 *     return action;
 * }
 *
 * \endcode
 *
 * \subsection notify_cb_example Example implementation for application Notification-Callback
 *
 * See #DIAG_NOTIFY_CB for details on the application notification callback function.
 *
 * \code
 * static void APPL_NotifyCb(ubyte1 diag_state,
 *                           ubyte1 watchdog_state,
 *                           DIAG_ERRORCODE * const error)
 * {
 *     IO_CAN_DATA_FRAME can_frame = { 0 };
 *
 *     // Error codes can for example be send over CAN
 *     // A CAN channel needs to be initialized before with \c #IO_CAN_Init()
 *     // and a mailbox for transmission needs to be set up
 *
 *     // assemble frame
 *     can_frame.id = 0x22;
 *     can_frame.id_format = IO_CAN_STD_FRAME;
 *     can_frame.length = 8;
 *     can_frame.data[0] = diag_state;
 *     can_frame.data[1] = watchdog_state;
 *     can_frame.data[2] = error->device_num;
 *     can_frame.data[3] = error->error_code;
 *     can_frame.data[4] = (ubyte1)(error->faulty_value >> 24);
 *     can_frame.data[5] = (ubyte1)(error->faulty_value >> 16);
 *     can_frame.data[6] = (ubyte1)(error->faulty_value >> 8);
 *     can_frame.data[7] = (ubyte1)error->faulty_value;
 *
 *     // write CAN message
 *     IO_CAN_WriteMsg(can_handle_cb, // handle for writing
 *                     &can_frame);   // local CAN buffer
 * }
 *
 * \endcode
 *
 * \subsection safety_driver_example Example for safety critical Driver configuration
 * \code
 *
 * // Safety configuration for IO Driver
 * static const IO_DRIVER_SAFETY_CONF c_driver_safety_conf =
 * {
 *     40,                                  // glitch_filter_time [ms]
 *     10000,                               // command_period [us]
 *     SAFETY_CONF_WINDOW_SIZE_50_PERCENT,  // window_size: 50 percent watchdog window
 *     SAFETY_CONF_RESETS_DISABLED,         // reset_behavior: no resets allowed
 *
 *     // error_callback:
 *     // (Set to \c #NULL if IO Driver should decide what to do on errors)
 *     &APPL_SafetyCb,
 *
 *     // notify_callback:
 *     // (Set to \c #NULL if the application is not interested in the error code when the
 *     // safe state is entered and before a configured reset takes place)
 *     &APPL_NotifyCb
 * };
 *
 * static ubyte4 timestamp = 0;
 *
 * void task (void)
 * {
 *     IO_ErrorType rc_driv_begin;
 *     IO_ErrorType rc_driv_end;
 *
 *     rc_driv_begin = IO_Driver_TaskBegin();
 *
 *     if (rc_driv_begin != IO_E_OK)
 *     {
 *         // User code
 *     }
 *
 *     // User Application
 *     // and calls to driver task functions.
 *
 *     rc_driv_end = IO_Driver_TaskEnd();
 *
 *     if (rc_driv_end != IO_E_OK)
 *     {
 *         // User code
 *     }
 *
 *     while (IO_RTC_GetTimeUS(timestamp) < 10000);   // wait until 10ms have passed
 *     timestamp += 10000;                            // increase time stamp by cycle time
 * }
 *
 *
 * void main (void)
 * {
 *     IO_ErrorType rc_driv_init;
 *
 *     //--------------------------------//
 *     // start of driver initialization //
 *     //--------------------------------//
 *
 *     // IO_Driver_Init() is the first function:
 *     rc_driv_init = IO_Driver_Init(&c_driver_safety_conf);  // safety critical application
 *
 *     if (rc_driv_init != IO_E_OK)
 *     {
 *         // User code
 *     }
 *
 *     //------------------------------//
 *     // end of driver initialization //
 *     //------------------------------//
 *
 *     // Get timestamp
 *     IO_RTC_StartTime(&timestamp);
 *
 *     //--------------------------------------------//
 *     // from now on only task functions are called //
 *     //--------------------------------------------//
 *     while (1)
 *     {
 *         task();
 *     }
 * }
 *
 * \endcode
 *
 *
 * \subsection fpu_cb_example Example implementation for FPU-Callback
 *
 * See #IO_DRIVER_FPU_HANDLER for details on the FPU callback function.
 *
 * \code
 * static ubyte4 timestamp = 0;
 *
 * static void FPU_Handler(bool division_by_zero,
 *                         bool input_denormal,
 *                         bool invalid_operation,
 *                         bool overflow,
 *                         bool underflow)
 * {
 *     if (division_by_zero != FALSE)
 *     {
 *         // Application specific handling
 *     }
 *
 *     if (input_denormal != FALSE)
 *     {
 *         // Application specific handling
 *     }
 *
 *     if (invalid_operation != FALSE)
 *     {
 *         // Application specific handling
 *     }
 *
 *     if (overflow != FALSE)
 *     {
 *         // Application specific handling
 *     }
 *
 *     if (underflow != FALSE)
 *     {
 *         // Application specific handling
 *     }
 * }
 *
 * void task (void)
 * {
 *     IO_Driver_TaskBegin();
 *
 *     // User Application
 *     // and calls to driver task functions.
 *
 *     IO_Driver_TaskEnd();
 *
 *     while (IO_RTC_GetTimeUS(timestamp) < 5000);   // wait until 5ms have passed
 *     timestamp += 5000;                            // increase time stamp by cycle time
 * }
 *
 * void main (void)
 * {
 *     //--------------------------------//
 *     // start of driver initialization //
 *     //--------------------------------//
 *
 *     // IO_Driver_Init() is the first function:
 *     IO_Driver_Init(NULL);  // no safety critical application
 *
 *     // Enable FPU callback
 *     IO_Driver_SetFPUHandler(&FPU_Handler);
 *
 *     //------------------------------//
 *     // end of driver initialization //
 *     //------------------------------//
 *
 *     // Get timestamp
 *     IO_RTC_StartTime(&timestamp);
 *
 *     //--------------------------------------------//
 *     // from now on only task functions are called //
 *     //--------------------------------------------//
 *     while(1)
 *     {
 *         task();
 *     }
 * }
 *
 * \endcode
 *
 * \page iodrv_endian Endianness
 * The endianness of the ARM Cortex-R4F core of the TI TMS570 CPU is configured to <b>BE32</b>.
 * Big-endian systems store the most significant byte of a multi-byte data field in the lowest
 * memory address. Also, the address of the multi-byte data field is the lowest address.
 *
 * \note
 *  The endianness of the HY-TTC 500 controllers can not be changed.
 *
 *************************************************************************************************/

#ifndef IO_DRIVER_H_
#define IO_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************//**
 * \mainpage HY-TTC 500 I/O Driver Manual
 *
 * \version 3.4.1
 * \date    22.04.2022
 *
 *
 * This document describes the API of the HY-TTC 500 I/O Driver.
 *
 *
 * The following driver functions are described:
 *      - \ref IO_Driver.h          "Driver main components"
 *      - \ref IO_Error.h           "Error definitions"
 *      - \ref IO_Pin.h             "Pin definitions"
 *      - \ref APDB.h               "Application database for bootloader"\n\n
 *
 *      - \ref diag_error_reporting "DIAG - Diagnostic error reporting"
 *      - \ref DIAG_Constants.h     "DIAG - Diagnostic constants"
 *      - \ref DIAG_Functions.h     "DIAG - Diagnostic functions"\n\n
 *
 *      - \ref IO_ADC.h             "ADC - Analog to Digital Converter driver"
 *      - \ref IO_CAN.h             "CAN - Controller Area Network driver"
 *      - \ref IO_DEBUG.h           "DEBUG - Debugging utility functions"
 *      - \ref IO_DIO.h             "DIO - Driver for digital inputs and outputs"
 *      - \ref IO_DOWNLOAD.h        "DOWNLOAD - Driver for Ethernet download"
 *      - \ref IO_EEPROM.h          "EEPROM - External EEPROM/FRAM non-volatile memory driver"
 *      - \ref IO_FLASH.h           "FLASH - External flash non-volatile memory driver"
 *      - \ref IO_LIN.h             "LIN - Local Interconnect Network driver"
 *      - \ref IO_MPU.h             "MPU - Memory Protection Unit"
 *      - \ref IO_POWER.h           "POWER - Driver for ECU power functions"
 *      - \ref IO_PVG.h             "PVG - Proportional Valve output driver"
 *      - \ref IO_PWD.h             "PWD - Pulse Width Decode and digital timer input driver"
 *      - \ref IO_PWM.h             "PWM - Pulse Width Modulation driver"
 *      - \ref IO_RTC.h             "RTC - Real Time Clock driver"
 *      - \ref IO_UDP.h             "UDP - User Datagram Protocol communication driver"
 *      - \ref IO_UART.h            "UART - Universal Asynchronous Receiver Transmitter driver"
 *      - \ref IO_VOUT.h            "VOUT - Voltage Output driver"
 *
 *************************************************************************************************/

/**************************************************************************************************
 *
 * I N C L U D E S
 *
 *************************************************************************************************/

#include "ptypes_tms570.h"
#include "IO_Error.h"
#include "IO_Pin.h"
#include "DIAG_Constants.h"

/**************************************************************************************************
 *
 * D E F I N I T I O N S
 *
 *************************************************************************************************/

/**
 * \name Global safety configuration watchdog window size definitions
 * \anchor safety_conf_wind
 *
 * These definitions have to be used when configuring the window size inside the global safety
 * configuration.
 *
 * The window margin size is a percentage of the maximum trigger time, not of the command period.
 * For example if the command period is 10 ms and the window size is 50 percent, the maximum
 * distance between two watchdog triggers will be 13.33 ms, and the watchdog can be triggered in
 * the last 50% of this window, meaning from 6.67 ms to 13.33 ms (a window margin of &plusmn; 33%
 * of the command period). This way a correct triggering would happen at the middle of the watchdog
 * trigger window.
 *
 * @{
 */
/** Watchdog window size is 100 percent of the maximum trigger time.
 *
 *  The resulting window starts from 0% and goes up to 200% the configured command period.
 *
 *  \remarks
 *    This setting results in a timeout-only watchdog, i.e., the watchdog can be triggered any
 *    time before the timeout of twice the configured command period is reached.
 */
#define SAFETY_CONF_WINDOW_SIZE_100_PERCENT     ( 0U )

/** Watchdog window size is 50 percent of the maximum trigger time.
 *
 *  The resulting window starts from 66% and goes up to 133% of the configured command period. \n
 *  The \b actual \b window \b size thus will be 66% (+/-33%) of the command period
 */
#define SAFETY_CONF_WINDOW_SIZE_50_PERCENT      ( 1U )

/** Watchdog window size is 25 percent of the maximum trigger time.
 *
 *  The resulting window starts from 86% and goes up to 114% of the configured command period. \n
 *  The \b actual \b window \b size thus will be 28% (+/-14%) of the command period
 */
#define SAFETY_CONF_WINDOW_SIZE_25_PERCENT      ( 2U )

/** Watchdog window size is 12.5 percent of the maximum trigger time.
 *
 *  The resulting window starts from 93.4% and goes up to 106.6% of the configured command period. \n
 *  The \b actual \b window \b size thus will be 13.3% (+/-6.6%) of the command period
 */
#define SAFETY_CONF_WINDOW_SIZE_12_5_PERCENT    ( 3U )

/** Watchdog window size is 6.25 percent of the maximum trigger time.
 *
 *  The resulting window starts from 96.7% and goes up to 103.2% of the configured command period. \n
 *  The \b actual \b window \b size thus will be 6.45% (+/-3.22%) of the command period
 */
#define SAFETY_CONF_WINDOW_SIZE_6_25_PERCENT    ( 4U )

/** Watchdog window size is 3.125 percent of the maximum trigger time.
 *
 *  The resulting window starts from 98.5% and goes up to 101.5% of the configured command period. \n
 *  The \b actual \b window \b size thus will be 3.174% (+/-1.587%) of the command period
 */
#define SAFETY_CONF_WINDOW_SIZE_3_125_PERCENT   ( 5U )
/** @} */


/**
 * \name Global safety configuration reset behavior definitions
 * \anchor safety_conf_reset
 *
 * This definitions have to be used when configuring the reset behavior inside the global safety
 * configuration.
 *
 * @{
 */
/** Resets disabled.\n
 *  This means that the watchdog CPU can not reset the device and restart the device again. On any
 *  error the diagnostic state machine will directly enter a safe state.
 */
#define SAFETY_CONF_RESETS_DISABLED             ( 0U )

/** 1 Reset allowed.\n
 *  This means that the watchdog CPU can reset the device and try to restart the device for 1 time.
 *  For any further error the diagnostic state machine will enter the safe state.
 */
#define SAFETY_CONF_RESETS_1                    ( 1U )

/** 2 Resets allowed.\n
 *  This means that the watchdog CPU can reset the device and try to restart the device for 2
 *  times. For any further error the diagnostic state machine will enter the safe state.
 */
#define SAFETY_CONF_RESETS_2                    ( 2U )

/** 3 Resets allowed.\n
 *  This means that the watchdog CPU can reset the device and try to restart the device for 3
 *  times. For any further error the diagnostic state machine will enter the safe state.
 */
#define SAFETY_CONF_RESETS_3                    ( 3U )

/** 4 Resets allowed.\n
 *  This means that the watchdog CPU can reset the device and try to restart the device for 4
 *  times. For any further error the diagnostic state machine will enter the safe state.
 */
#define SAFETY_CONF_RESETS_4                    ( 4U )

/** 5 Resets allowed.\n
 *  This means that the watchdog CPU can reset the device and try to restart the device for 5
 *  times. For any further error the diagnostic state machine will enter the safe state.
 */
#define SAFETY_CONF_RESETS_5                    ( 5U )

/** 6 Resets allowed.\n
 *  This means that the watchdog CPU can reset the device and try to restart the device for 6
 *  times. For any further error the diagnostic state machine will enter the safe state.
 */
#define SAFETY_CONF_RESETS_6                    ( 6U )

/** 7 Resets allowed.\n
 *  This means that the watchdog CPU can reset the device and try to restart the device for 7
 *  times. For any further error the diagnostic state machine will enter the safe state.
 */
#define SAFETY_CONF_RESETS_7                    ( 7U )

/** 8 Resets allowed.\n
 *  This means that the watchdog CPU can reset the device and try to restart the device for 8
 *  times. For any further error the diagnostic state machine will enter the safe state.
 */
#define SAFETY_CONF_RESETS_8                    ( 8U )

/** 9 Resets allowed.\n
 *  This means that the watchdog CPU can reset the device and try to restart the device for 9
 *  times. For any further error the diagnostic state machine will enter the safe state.
 */
#define SAFETY_CONF_RESETS_9                    ( 9U )
/** @} */


/**
 * \name Number of bytes of the ECU's serial number
 * \anchor serial_length
 *
 * Defines the number of bytes of the ECU's serial number
 *
 * @{
 */
/** ECU serial number length.
 */
#define IO_DRIVER_ECU_SERIAL_LENGTH             ( 14U )
/** @} */


/**
 * \name Number of bytes of the ECU's MAC address
 * \anchor mac_length
 *
 * Defines the number of bytes of the ECU MAC address
 *
 * @{
 */
/** ECU MAC address length.
 */
#define IO_DRIVER_ECU_MAC_ADD_LENGTH            ( 12U )
/** @} */


/**
 * \name Number of bytes of the ECU's production code
 * \anchor prodcode_length
 *
 * Defines the number of bytes of the ECU's production code
 *
 * @{
 */
/** ECU production code length.
 */
#define IO_DRIVER_ECU_PROD_CODE_LENGTH          ( 30U )
/** @} */


/**********************************************************************************************//**
 *
 * \brief Function pointer for FPU exception handler.
 *
 * If a FPU exception occurs, the FPU unit notifies the application of the exception reason by
 * calling this callback function.
 *
 * The callback function is passed to the function #IO_Driver_SetFPUHandler(..).
 *
 * The exception reasons are passed to the callback function as parameter:
 *
 * \param division_by_zero      The exception is caused if a divide operation has a zero divisor
 *                              and a dividend that is not zero, an infinity or a NaN
 * \param input_denormal        The exception is caused if a denormalized input operand is
 *                              replaced in the computation by a zero
 * \param invalid_operation     The exception is caused if the result of an operation has no
 *                              mathematical value or cannot be represented
 * \param overflow              The exception is caused if the absolute value of the result of an
 *                              operation, produced after rounding, is greater than the maximum
 *                              positive normalized number for the destination precision.
 * \param underflow             The exception is caused if the absolute value of the result of an
 *                              operation, produced before rounding, is less than the minimum
 *                              positive normalized number for the destination precision and the
 *                              rounded result is inexact.
 *
 * \remarks
 *   See \ref fpu_cb_example "Example implementation for FPU-Callback".
 *
 */
typedef void (*IO_DRIVER_FPU_HANDLER)(bool division_by_zero,
                                      bool input_denormal,
                                      bool invalid_operation,
                                      bool overflow,
                                      bool underflow);

/**************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/

/**
 *
 * \brief Driver Safety Configuration
 *
 * This structure is used to pass the configuration for safety critical application to the
 * IO Driver.
 *
 * \note Note that the hereby defined values are the configuration values of the TMS570
 *       watchdog configuration values! The processor offers a unique watchdog triggering technic,
 *       which does not based on a +/- window manner.
 *       In order to support a command period centric +/- watchdog window approach
 *       the underlying implementation - due to processor limitations - will distort the
 *       window size that was requested through the safety configuration. \n
 *       To calculate your exact window size (in %) you can use the following formula: \n
 *       \b actual_window_size = {400/(200-\b choosen_wsize)-2}*100 \n
 *       where \a choosen_wsize is one of the \a SAFETY_CONF_WINDOW_SIZE below (6.25%,12.5%,25%,etc). \n
 *       Example, with a windows size choose 25%, the actual size will be 28,57%.
 */
typedef struct io_driver_safety_conf_
{
    ubyte1 glitch_filter_time;      /**<  Only if an error condition persists after expiration of
                                          this time range, an error reaction is taken(1..180 [ms]).
                                    */
    ubyte4 command_period;          /**<  Time in [us], interval between two consecutive software
                                          cycles (1000..50000).
                                    */
    ubyte1 window_size;             /**<  Watchdog window size. One of:
                                              - \c #SAFETY_CONF_WINDOW_SIZE_100_PERCENT
                                              - \c #SAFETY_CONF_WINDOW_SIZE_50_PERCENT
                                              - \c #SAFETY_CONF_WINDOW_SIZE_25_PERCENT
                                              - \c #SAFETY_CONF_WINDOW_SIZE_12_5_PERCENT
                                              - \c #SAFETY_CONF_WINDOW_SIZE_6_25_PERCENT
                                              - \c #SAFETY_CONF_WINDOW_SIZE_3_125_PERCENT
                                    */
    ubyte1 reset_behavior;          /**<  Watchdog reset behavior. One of:
                                              - \c #SAFETY_CONF_RESETS_DISABLED
                                              - \c #SAFETY_CONF_RESETS_1
                                              - \c #SAFETY_CONF_RESETS_2
                                              - \c #SAFETY_CONF_RESETS_3
                                              - \c #SAFETY_CONF_RESETS_4
                                              - \c #SAFETY_CONF_RESETS_5
                                              - \c #SAFETY_CONF_RESETS_6
                                              - \c #SAFETY_CONF_RESETS_7
                                              - \c #SAFETY_CONF_RESETS_8
                                              - \c #SAFETY_CONF_RESETS_9
                                    */
    DIAG_ERROR_CB error_callback;   /**<  Callback function for non-fatal errors. Set this
                                          parameter to \c #NULL to disable this feature.
                                    */
    DIAG_NOTIFY_CB notify_callback; /**<  Callback function for fatal errors. Set this
                                          parameter to \c #NULL to disable this feature.
                                    */
} IO_DRIVER_SAFETY_CONF;

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Global initialization of IO driver.
 *
 * This function shall be called before any other driver function (except functions
 * IO_Driver_GetVersionOfDriver(), IO_Driver_GetVersionOfBootloader(),
 * IO_Driver_GetSerialNumber() and IO_Driver_GetMacAddress()).
 *  - Switches off all power outputs
 *  - Initializes the RTC
 *  - Switches on the interrupts of the CPU
 *  - Initializes the internal measurements
 *  - Initializes the checker modules
 *
 * \param[in] safety_conf   Configuration for safety critical applications. Set pointer to
 *                          \c #NULL to disable the safety features.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_SAFETY_CONFIG  an invalid safety configuration has been passed
 * \retval #IO_E_CHANNEL_BUSY           the driver was already initialized
 * \retval #IO_E_INVALID_PROD_DATA      the ECU's production data is corrupted
 * \retval #IO_E_INVALID_SERIAL_NUMBER  the ECU's serial number is invalid
 * \retval #IO_E_INVALID_VARIANT        the used ECU variant is not valid
 * \retval #IO_E_FPGA_TIMEOUT           the FPGA did not respond
 * \retval #IO_E_FPGA_CRC_ERROR         the FPGA reported a CRC error
 * \retval #IO_E_FPGA_VERSION           the FPGA has a wrong version
 * \retval #IO_E_WD_INITIALIZATION      the watchdog system could not be initialized
 * \retval #IO_E_WD_PRECISION           the configured limits require too high precision
 * \retval #IO_E_WD_SAFE_LOCK           the ECU is already locked in safe state
 * \retval #IO_E_WD_DEBUGGING_PREPARED  the watchdog system has been prepared for debugging
 * \retval #IO_E_INTERNAL_CSM           an error in the internal low level driver occurred
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    If \c safety_conf != \c #NULL, the application is configured safety critical. The parameter
 *    \c safety_conf defines the global safety properties. Then in other Init functions, like
 *    \c IO_ADC_ChannelInit(), each channel itself can be configured safety critical.
 *
 * \note
 *    The watchdog window size has to be configured to provide a minimum positive window of at
 *    least 700 microseconds (including losses caused by rounding) in order to guarantee that all
 *    internal background mechanisms cannot lead to runtime violations in worst case scenarios. In
 *    case this limit is not fulfilled, \c #IO_E_WD_PRECISION is returned.
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_Init(const IO_DRIVER_SAFETY_CONF * const safety_conf);

/**********************************************************************************************//**
 *
 * \brief Returns the version number of the IO driver
 *
 * \param[out] major        Major version
 * \param[out] minor        Minor version
 * \param[out] patchlevel   Patchlevel
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                    everything fine
 * \retval #IO_E_NULL_POINTER          null pointer has been passed
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_GetVersionOfDriver(ubyte1 * const major,
                                          ubyte1 * const minor,
                                          ubyte2 * const patchlevel);

/**********************************************************************************************//**
 *
 * \brief Returns the version number of the FPGA IP
 *
 * \param[out] rev0         Revision 0, for internal use only
 * \param[out] rev1         Revision 1, for internal use only
 * \param[out] rev2         Revision 2, for internal use only
 * \param[out] device       Device type
 * \param[out] release      Release version
 * \param[out] patchlevel   Patchlevel
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                    everything fine
 * \retval #IO_E_NULL_POINTER          null pointer has been passed
 * \retval #IO_E_FPGA_NOT_INITIALIZED  the FPGA has not been initialized
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_GetVersionOfFPGA(ubyte2 * const rev0,
                                        ubyte2 * const rev1,
                                        ubyte2 * const rev2,
                                        ubyte1 * const device,
                                        ubyte1 * const release,
                                        ubyte1 * const patchlevel);

/**********************************************************************************************//**
 *
 * \brief Returns the version number of the Bootloader
 *
 * \param[out] major        Major version
 * \param[out] minor        Minor version
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_NULL_POINTER            null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  shared memory is not initialized
 * \retval #IO_E_SHM_INTEGRITY           shared memory is corrupted
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_GetVersionOfBootloader(ubyte1 * const major,
                                              ubyte1 * const minor);

/**********************************************************************************************//**
 *
 * \brief Returns the ECU's serial number
 *
 * \param[out] serialnumber   Pointer to the ECU's serial number data structure of size
 *                            #IO_DRIVER_ECU_SERIAL_LENGTH. The read serial number will be stored
 *                            here, but not as a null-terminated string, only a list of characters.
 *
 * \verbatim[table]
 * ========= ====================================
 *   Byte      Description
 * --------- ------------------------------------
 *     00      last digit of year
 * --------- ------------------------------------
 *   01-02     calendar week
 * --------- ------------------------------------
 *     03      day of the week (1=Monday)
 * --------- ------------------------------------
 *   04-07     daily counter
 * --------- ------------------------------------
 *   08-12     product number
 * --------- ------------------------------------
 *     13      product index
 * ========= ====================================
 * \endverbatim
 * Example:\n
 * "30820015008180"
 * \verbatim[table]
 * ========= ====================================
 *   Values    Description
 * --------- ------------------------------------
 *     3       year 2013
 * --------- ------------------------------------
 *     08      Calendar week 8
 * --------- ------------------------------------
 *     2       day of week (Tuesday)
 * --------- ------------------------------------
 *   0015      daily counter
 * --------- ------------------------------------
 *   00818     product number 00818
 * --------- ------------------------------------
 *     0       product index 0
 * ========= ====================================
 * \endverbatim
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                    everything fine
 * \retval #IO_E_NULL_POINTER          null pointer has been passed
 * \retval #IO_E_INVALID_PROD_DATA     the Production Data is invalid
 * \retval #IO_E_INVALID_SERIAL_NUMBER the ECU's serial number is invalid
 *
 **************************************************************************************************
 *
 * \remarks
 *    The 14 digits of the serial number are returned as ASCII code. The application has to convert
 *    it to a number representation if desired.
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_GetSerialNumber(ubyte1 * const serialnumber);

/**********************************************************************************************//**
 *
 * \brief Returns the ECU's MAC address
 *
 * \param[out] macaddress   Pointer to the ECU's MAC address data structure of size
 *                          #IO_DRIVER_ECU_MAC_ADD_LENGTH. The read MAC address will be stored
 *                          here, but not as a null-terminated string, only a list of characters.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                    everything fine
 * \retval #IO_E_NULL_POINTER          null pointer has been passed
 * \retval #IO_E_INVALID_PROD_DATA     the Production Data is invalid
 *
 **************************************************************************************************
 *
 * \remarks
 *    The 12 digits of the MAC address are returned as ASCII code. The application has to convert
 *    it to a number representation if desired.
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_GetMacAddress(ubyte1 * const macaddress);

/**********************************************************************************************//**
 *
 * \brief Returns the ECU's production code
 *
 * \param[out] prodcode     Pointer to the ECU's production code data structure of size
 *                          #IO_DRIVER_ECU_PROD_CODE_LENGTH. The read production code will be
 *                          stored here, but not as a null-terminated string, only a list of
 *                          characters.
 *
 * \verbatim[table]
 * ========= ====================================
 *   Byte      Description
 * --------- ------------------------------------
 *   00-06     ECU type
 * --------- ------------------------------------
 *   07-11     Further ECU details
 * --------- ------------------------------------
 *     12      Housing (S = sealed O = open)
 * --------- ------------------------------------
 *   13-20     Production BOM Version
 * --------- ------------------------------------
 *   21-25     Software Bundle ID
 * --------- ------------------------------------
 *   26-29     Product Label Version
 * ========= ====================================
 * \endverbatim
 * Example:\n
 * "TTC-58000000SV010000BU0368L51A"
 * \verbatim[table]
 * ========= ====================================
 *   Values    Description
 * --------- ------------------------------------
 *  TTC-580    ECU type HY TTC-580
 * --------- ------------------------------------
 *   00000     Further ECU details
 * --------- ------------------------------------
 *     S       S = sealed
 * --------- ------------------------------------
 *  V010000B   Production BOM Version V01.00.00-B
 * --------- ------------------------------------
 *   U0368     Software Bundle U0368
 * --------- ------------------------------------
 *    L51A     Product Label Version L51A
 * ========= ====================================
 * \endverbatim
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                    everything fine
 * \retval #IO_E_NULL_POINTER          null pointer has been passed
 * \retval #IO_E_INVALID_PROD_DATA     the Production Data is invalid
 *
 **************************************************************************************************
 *
 * \remarks
 *    The 30 digits of the production code are returned as ASCII code. The application has to
 *    convert it to a number representation if desired.
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_GetProdCode(ubyte1 * const prodcode);

/**********************************************************************************************//**
 *
 * \brief Task function for IO Driver. This function shall be called at the beginning of the task.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_WD_SELF_MONITORING         the function has not been called at the right point in
 *                                          time
 * \retval #IO_E_WD_VICE_VERSA_MONITORING   the triggering of the external watchdog exceeded it's
 *                                          limit
 * \retval #IO_E_DRIVER_NOT_INITIALIZED     the driver has not been initialized
 * \retval #IO_E_UNKNOWN                    an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_TaskBegin(void);

/**********************************************************************************************//**
 *
 * \brief Task function for IO Driver. This function shall be called at the end of the task.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_DRIVER_NOT_INITIALIZED  the driver has not been initialized
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_TaskEnd(void);

/**********************************************************************************************//**
 *
 * \brief Registers an application callback for FPU exceptions
 *
 * \param[in] fpu_handler   Application FPU handler
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_DRIVER_NOT_INITIALIZED  the driver has not been initialized
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *    If the parameter \c fpu_handler is set to \c #NULL the FPU exception
 *    application callback is disabled again.
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_SetFPUHandler(IO_DRIVER_FPU_HANDLER fpu_handler);

/**********************************************************************************************//**
 *
 * \brief Enables/disables integer division by zero exceptions
 *
 * \param[in] enable    Integer division by zero exception mode:
 *                          - \c #TRUE:  integer division by zero exception enabled
 *                          - \c #FALSE: integer division by zero exception disabled
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_DRIVER_NOT_INITIALIZED  the driver has not been initialized
 *
 **************************************************************************************************
 *
 * \remarks
 *    If the integer division by zero exception is disabled, an integer division
 *    by zero always results in the value of zero.
 *
 * \remarks
 *    The integer division by zero exception is disabled by default.
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_SetIntegerDivisionByZeroException(bool enable);

/**********************************************************************************************//**
 *
 * \brief Performs a software reset of the device. The intended use of this function is to enter
 *        the bootloader from an application.
 *
 * \remarks
 *          In order to perform the reset the watchdog must be in either state #DIAG_WD_STATE_SAFE or
 *          #DIAG_WD_STATE_ACTIVE.
 * \remarks
 *          Upon reset if the application is entered the device will boot into safe state and the
 *          IO_Driver_Init will return an error.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_BUSY                       reset could not be executed due to a wrong state
 * \retval #IO_E_WD_STATUS_INVALID          watchdog status couldn't be obtained
 * \retval #IO_E_SHM_INTEGRITY              the shared memory's integrity is invalid
 * \retval #IO_E_INTERNAL_CSM               an internal error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_Driver_Reset(void);

#ifdef __cplusplus
}
#endif

#endif /* IO_DRIVER_H_ */
