/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_RTC.h
 *
 * \latexonly \hypertarget{IO_RTC}{} \endlatexonly
 *
 * \brief RTC functions, provides exact timing functions and services
 *
 *   Three different timing related functionalities are accessible:
 *   - Microsecond based time stamp and time difference
 *   - Periodic timer call
 *   - Real Time Clock for date and time information
 *
 *   <b>Timestamp functions</b>
 *   - \c IO_RTC_StartTime()
 *   - \c IO_RTC_GetTimeUS()
 *
 *   The function \c IO_RTC_StartTime() returns a timestamp. The function \c IO_RTC_GetTimeUS()
 *   returns the time which has passed since the timestamp-value passed as an argument to this
 *   function. The application can use these two functions as often as it needs to. For different
 *   timing tasks only different timestamp variables need to be used.
 *
 *   <b>Periodic functions</b>
 *   - \c IO_RTC_PeriodicInit()
 *   - \c IO_RTC_PeriodicDeInit()
 *
 *   The function \c IO_RTC_PeriodicInit() can be used to register a periodic callback to the
 *   provided function which has to be passed as an argument. The function
 *   \c IO_RTC_PeriodicDeInit() will unregister this periodic call again.
 *
 *   <b>Real Time Clock (date and time) functions</b>
 *   - \c IO_RTC_InitDateAndTime()
 *   - \c IO_RTC_GetDateAndTime()
 *   - \c IO_RTC_SetDateAndTime()
 *   - \c IO_RTC_GetDateAndTimeStatus()
 *   - \c IO_RTC_DeInitDateAndTime()
 *
 *   The Real Time Clock functions provide a service for date and time keeping. It can be
 *   initialized by calling \c IO_RTC_InitDateAndTime(). After a power loss the RTC must be
 *   configured by calling \c IO_RTC_SetDateAndTime() with the desired date and time. The
 *   application can request the current date and time by calling \c IO_RTC_GetDateAndTime(). Both
 *   setting and requesting need to be checked for completeness by using the function
 *   \c IO_RTC_GetDateAndTimeStatus(). This service can be uninitialized again by calling
 *   \c IO_RTC_DeInitDateAndTime().
 *
 * \attention
 *   If the date and time are requested from a situation where no interrupts are available, such as
 *   from within the error or notification callback, the user application must ensure that the function
 *   \c IO_RTC_GetDateAndTimeStatus() is called periodically to trigger the RTC processing.
 *
 * \section rtc_examples RTC Code Examples
 * \brief Examples for using the RTC API
 * \subsection rtc_timestamp_example Example for RTC timestamp usage
 * \details The example initializes the RTC driver, and implements a loop
 *          which is executed every 5ms:
 * \code
 *      ubyte4 time_stamp;
 *
 *      IO_Driver_Init(NULL);             // initialize driver
 *
 *      IO_RTC_StartTime (&time_stamp);   // start time (get timestamp)
 *
 *      while (1)
 *      {
 *          task();      // user task function
 *
 *          while (IO_RTC_GetTimeUS (time_stamp) < 5000);   // wait until 5ms have passed
 *          time_stamp += 5000;                             // increase time stamp by cycle time
 *
 *          // Note: If one cycle takes longer than the configured cycle time, for the next
 *          // cycle less time is available. This method helps to prevent a phase shift between
 *          // application runtime and hardware runtime
 *      }
 *
 * \endcode
 *
 * \subsection rtc_date_example Example for RTC date and time usage
 * \details The example initializes the RTC driver, and setup the date and time
 * \code
 *
 *      ubyte1 seconds;
 *      ubyte1 minutes;
 *      ubyte1 hours;
 *      ubyte1 days;
 *      ubyte1 months;
 *      ubyte1 years;
 *
 *      // initialize real time clock
 *      IO_RTC_InitDateAndTime();
 *
 *      // wait for real time clock to be ready
 *      while(IO_RTC_GetDateAndTimeStatus() == IO_E_BUSY);
 *
 *      // set time to 11:30:00 and date to 05.07.13
 *      IO_RTC_SetDateAndTime(00,        // seconds
 *                            30,        // minutes
 *                            11,        // hours
 *                            05,        // days
 *                            07,        // months
 *                            13);       // years
 *
 *      // wait for real time clock to be ready
 *      while(IO_RTC_GetDateAndTimeStatus() == IO_E_BUSY);
 *
 *      // get date and time from real time clock
 *      IO_RTC_GetDateAndTime(&seconds,  // seconds
 *                            &minutes,  // minutes
 *                            &hours,    // hours
 *                            &days,     // days
 *                            &months,   // months
 *                            &years);   // years
 *
 *      // wait for transfer to be complete
 *      while(IO_RTC_GetDateAndTimeStatus() == IO_E_BUSY);
 *
 *      // check if data is valid
 *      if (IO_RTC_GetDateAndTimeStatus() == IO_E_OK)
 *      {
 *          // process data
 *      }
 *      else
 *      {
 *          // error
 *      }
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_RTC_H_
#define IO_RTC_H_

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

/* typedef for function pointer for event handler of the periodic
 * interrupt handler
 */
/**
 * \brief function pointer for event handler of the periodic
 * interrupt handler
 */
typedef void (*IO_RTC_EVENT_HANDLER)(void);

/* Defined in order to maintain backwards compatibility */
/**
 * \brief The Macro maintains backwards compatibility for applications using the \c IO_RTC_Process()
 *  from I/O Driver 3.1.
 */
#define IO_RTC_Process   IO_RTC_GetDateAndTimeStatus

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Returns a timestamp
 *
 * Returns a timestamp which can be used for RTC timing functions
 *
 * \param[out] timestamp    Pointer for the returned timestamp value
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_NULL_POINTER            a null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the module has not been initialized
 *
 *************************************************************************************************/
IO_ErrorType IO_RTC_StartTime(ubyte4 * const timestamp);

/**********************************************************************************************//**
 *
 * \brief Returns the passed time
 *
 * The function returns the time in us which has passed since the timestamp-value passed as
 * an argument to this function.
 *
 * \param timestamp         Timestamp value (Could be received from a call of
 *                          \c IO_RTC_StartTime())
 *
 * \return ubyte4
 *
 **************************************************************************************************
 *
 * \remarks
 *    - If the RTC module has not been initialized, the function will return 0
 *
 * \remarks
 *    - Please keep in mind that the time between \c IO_RTC_StartTime() and \c IO_RTC_GetTimeUS()
 *    for one timestamp should not exceed 71min (overflow)
 *
 *************************************************************************************************/
ubyte4 IO_RTC_GetTimeUS(ubyte4 timestamp);

/**********************************************************************************************//**
 *
 * \brief Initializes the Periodic Timer
 *
 * \param period            Period on which the event handler should be called.
 *                          unit: us (500..65535)
 * \param[in] event_handler Function pointer to the periodic event handler
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     RTC is not supported by the used ECU variant
 * \retval #IO_E_NULL_POINTER           a null pointer was passed
 * \retval #IO_E_INVALID_PARAMETER      an invalid parameter was passed
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_CHANNEL_BUSY           the periodic timer is already used
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *   This function cannot be called before \c IO_Driver_Init() because the RTC driver needs to be
 *   initialized.
 *
 * \attention
 *   - It is not allowed to call any I/O driver function out of the periodic interrupt
 *   - The execution time of the periodic interrupt shall not exceed a time of 200us.
 *
 *************************************************************************************************/
IO_ErrorType IO_RTC_PeriodicInit(ubyte2 period,
                                 IO_RTC_EVENT_HANDLER event_handler);

/**********************************************************************************************//**
 *
 * \brief Deinitializes the Periodic Timer and stops it
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                       everything fine
 * \retval #IO_E_PERIODIC_NOT_CONFIGURED  the periodic timer was not configured
 * \retval #IO_E_UNKNOWN                  an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_RTC_PeriodicDeInit(void);

/**********************************************************************************************//**
 *
 * \brief Initializes the external RTC device for date and time operations.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     RTC is not supported by the used ECU variant
 * \retval #IO_E_CHANNEL_BUSY           the module has been initialized before
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *   This function cannot be called before \c IO_Driver_Init() because the SPI driver needs to be
 *   initialized.
 *
 *************************************************************************************************/
IO_ErrorType IO_RTC_InitDateAndTime(void);

/**********************************************************************************************//**
 *
 * \brief Returns the date and time of the real time clock
 *
 * The function triggers the reception from the RTC.\n
 * The reception can take several cycles to be completed, depending on the SPI load.
 * 
 * The date and time information is available on the addresses where the parameters point to as
 * soon as the operation is finished. The state can be polled using the
 * \c IO_RTC_GetDateAndTimeStatus() function.
 *
 * \param[out] seconds      Seconds (0..59)
 * \param[out] minutes      Minutes (0..59)
 * \param[out] hours        Hours (0..23)
 * \param[out] days         Days (1..31)
 * \param[out] months       Months (1..12)
 * \param[out] years        Years (0..99)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_BUSY                    a get or set transfer is still ongoing, wait to be
 *                                       complete
 * \retval #IO_E_NULL_POINTER            a null pointer was passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the module has not been initialized
 *
 **************************************************************************************************
 *
 * \remarks
 *   This function cannot be called before \c IO_RTC_InitDateAndTime().
 *
 *************************************************************************************************/
IO_ErrorType IO_RTC_GetDateAndTime(ubyte1 * const seconds,
                                   ubyte1 * const minutes,
                                   ubyte1 * const hours,
                                   ubyte1 * const days,
                                   ubyte1 * const months,
                                   ubyte1 * const years);

/**********************************************************************************************//**
 *
 * \brief Sets the date and time of the real time clock
 *
 * The function triggers the configuration of the RTC.\n
 * The operation can take several cycles to be completed, depending on the SPI load.
 * 
 * The configuration is completed as soon as the SPI communication has been finished. The state
 * can be polled using the \c IO_RTC_GetDateAndTimeStatus() function.
 *
 * \param seconds           Seconds (0..59)
 * \param minutes           Minutes (0..59)
 * \param hours             Hours (0..23)
 * \param days              Days (1..31)
 * \param months            Months (1..12)
 * \param years             Years (0..99)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_BUSY                    a get or set transfer is still ongoing, wait to be
 *                                       complete
 * \retval #IO_E_INVALID_PARAMETER       an invalid parameter has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the module has not been initialized
 *
 **************************************************************************************************
 *
 * \remarks
 *   This function cannot be called before \c IO_RTC_InitDateAndTime().
 *
 *************************************************************************************************/
IO_ErrorType IO_RTC_SetDateAndTime(ubyte1 seconds,
                                   ubyte1 minutes,
                                   ubyte1 hours,
                                   ubyte1 days,
                                   ubyte1 months,
                                   ubyte1 years);

/**********************************************************************************************//**
 *
 * \brief Gets status of the RTC Date and Time module and triggers the internal processing.
 * 
 * \remarks
 *   Periodic polling using this function is necessary to trigger the processing of the RTC operations
 *   in situations where interrupts are not available, such as within the error or notification callbacks.
 *
 *  \remarks
 *   If used from the error/notification callback, the nature of the failure will effect whether the
 *   RTC device is usable. In these cases it is important to include a timeout on any operations.
 * 
 * \return #IO_ErrorType
 * \retval #IO_E_OK                          Everything fine.
 * \retval #IO_E_BUSY                        A get or set transfer is still ongoing, wait to be complete.
 * \retval #IO_E_RTC_CLOCK_INTEGRITY_FAILED  The clock integrity has failed due to a power loss.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED      The module has not been initialized.
 * \retval #IO_E_UNKNOWN                     An error occurred converting the data from the RTC chip.
 *
 **************************************************************************************************
 *
 * \remarks
 *   This function cannot be called before \c IO_RTC_InitDateAndTime().
 *
 *************************************************************************************************/
IO_ErrorType IO_RTC_GetDateAndTimeStatus(void);

/**********************************************************************************************//**
 *
 * \brief Deinitializes the external RTC device
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the module has not been initialized
 *
 *************************************************************************************************/
IO_ErrorType IO_RTC_DeInitDateAndTime(void);

#ifdef __cplusplus
}
#endif

#endif /* IO_RTC_H_ */
