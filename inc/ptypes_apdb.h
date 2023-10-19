/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file ptypes_apdb.h
 *
 * \latexonly \hypertarget{ptypes_apdb}{} \endlatexonly
 *
 * \brief APDB target abstraction
 *
 *   This file defines the interface between the generic APDB format and the platform specific
 *   includes and definitions.
 *
 *************************************************************************************************/

#ifndef PTYPES_APDB_H_
#define PTYPES_APDB_H_

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
 * D E F I N I T I O N S
 *
 *************************************************************************************************/

/**
 *
 * \brief Application entry point
 *
 *  The entry point of the user application, which is called by the bootloader (must be written to
 *  the MainAddress field of the APDB to ensure proper application startup).
 *
 */
#define APPL_START                      ((ubyte4)_c_int00)


/**
 * \name APDB default date defines
 *
 * Default defines for the date structure in the APDB.
 * These values should be normally set during the compilation process.
 *
 * @{
 */
#ifndef RTS_TTC_FLASH_DATE_YEAR
    #define RTS_TTC_FLASH_DATE_YEAR     0UL     /**< year of the date   */
#endif

#ifndef RTS_TTC_FLASH_DATE_MONTH
    #define RTS_TTC_FLASH_DATE_MONTH    0UL     /**< month of the date  */
#endif

#ifndef RTS_TTC_FLASH_DATE_DAY
    #define RTS_TTC_FLASH_DATE_DAY      0UL     /**< day of the date    */
#endif

#ifndef RTS_TTC_FLASH_DATE_HOUR
    #define RTS_TTC_FLASH_DATE_HOUR     0UL     /**< hour of the date   */
#endif

#ifndef RTS_TTC_FLASH_DATE_MINUTE
    #define RTS_TTC_FLASH_DATE_MINUTE   0UL     /**< minute of the date */
#endif
/** @} */

/**************************************************************************************************
 *
 * E X T E R N A L S
 *
 *************************************************************************************************/
/**
 * \name Startup entry point
 */
extern void _c_int00(void);

#ifdef __cplusplus
}
#endif

#endif /* PTYPES_APDB_H_ */
