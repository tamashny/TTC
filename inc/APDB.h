/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file APDB.h
 *
 * \latexonly \hypertarget{APDB}{} \endlatexonly
 *
 * \brief The Application Descriptor Block (APDB)
 *
 *  Contains the definition for the application descriptor block. This block contains information
 *  about a certain application, such as connection settings for CAN and Ethernet, application CRC
 *  and the application entry point.
 *
 *  The bootloader needs this information to determine whether or not an application is valid and 
 *  where the application actually starts. For this reason the field
 *  \ref BL_APDB.MainAddress "BL_APDB.MainAddress" must be provided by the application. Note that
 *  some fields are automatically provided by the TTC-Downloader.
 *
 *  APDB Usage:
 *   - \ref apdb_examples "Example for APDB definition"
 *
 * \section apdb_examples APDB Code Example
 * \brief Example for using the APDB
 * \subsection apdb_example Example for APDB definition in an application
 * \code
 *     volatile const BL_APDB Apdb_t =
 *     {
 *         APDB_VERSION,           // APDB version
 *         {0},                    // Flash date (provided by the TTC-Downloader)
 *                                 // Build date
 *         {((((RTS_TTC_FLASH_DATE_YEAR)   & 0x0FFF) <<  0) |
 *           (((RTS_TTC_FLASH_DATE_MONTH)  & 0x0F  ) << 12) |
 *           (((RTS_TTC_FLASH_DATE_DAY)    & 0x1F  ) << 16) |
 *           (((RTS_TTC_FLASH_DATE_HOUR)   & 0x1F  ) << 21) |
 *           (((RTS_TTC_FLASH_DATE_MINUTE) & 0x3F  ) << 26))},
 *         0,                      // Node type (provided by the TTC-Downloader)
 *         0,                      // CRC start address (provided by the TTC-Downloader)
 *         0,                      // Code size (provided by the TTC-Downloader)
 *         0,                      // Legacy application CRC (provided by the TTC-Downloader)
 *         0,                      // Application CRC (provided by the TTC-Downloader)
 *         1,                      // Node number
 *         0,                      // CRC seed (provided by the TTC-Downloader)
 *         0,                      // Flags
 *         0,                      // Hook 1
 *         0,                      // Hook 2
 *         0,                      // Hook 3
 *         APPL_START,             // Start address, i.e., application entry point
 *         {0, 1},                 // CAN download ID (standard format, ID 0x1)
 *         {0, 2},                 // CAN upload ID (standard format, ID 0x2)
 *         0,                      // Legacy header CRC (provided by the TTC-Downloader)
 *                                 // Application version (major.minor.revision)
 *         ((((ubyte4)REVISION_NUMBER) <<  0) |
 *          (((ubyte4)   MINOR_NUMBER) << 16) |
 *          (((ubyte4)   MAJOR_NUMBER) << 24)),
 *         500,                    // CAN baud rate in kbps
 *         0,                      // CAN channel
 *         0,                      // Password (disable password protection)
 *         0,                      // Magic seed (provided by the TTC-Downloader)
 *         { 10, 100,  30, 200},   // Target IP address (HY-TTC 500 family only)
 *         {255, 255,   0,   0},   // Subnet mask (HY-TTC 500 family only)
 *         {239,   0,   0,   1},   // Multicast IP address (HY-TTC 500 family only)
 *         0,                      // Debug key
 *         0,                      // Automatic baud rate detection timeout in seconds
 *                                 // (HY-TTC 30X family only)
 *         0x00,                   // Manufacturer ID (only required for applications that
 *                                 // are flashed during production of the ECU, provided by
 *                                 // TTControl GmbH, the generic ID 0xFF can be used for
 *                                 // other applications)
 *         0x00,                   // Application ID (only required for applications that
 *                                 // are flashed during production of the ECU, provided by
 *                                 // TTControl GmbH)
 *         {0},                    // Reserved, must be set to zero
 *         0                       // Header CRC (provided by the TTC-Downloader)
 *     };
 * \endcode
 *
 *************************************************************************************************/

#ifndef APDB_H_
#define APDB_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *
 * I N C L U D E S
 *
 *************************************************************************************************/

#include "ptypes_apdb.h"

/**************************************************************************************************
 *
 * D E F I N I T I O N S
 *
 *************************************************************************************************/

#define APDB_VERSION                0x00000206UL    /**< Current APDB version is version 2.6.
                                                     *   This value should be set for field 
                                                     *   \ref BL_APDB.APDBVersion
                                                     *   "BL_APDB.APDBVersion".
                                                     *                                           */

/**
 * \name APDB flags
 * \anchor apdb_flags
 *
 * Defined APDB flags.
 *
 * @{
 */
#define APDB_FLAGS_ABRD_ENABLE      0x00000001UL    /**< Enables automatic baudrate detection at
                                                     *   start-up (HY-TTC 30X family only).
                                                     *   Access mode: read/write.
                                                     *                                           */
#define APDB_FLAGS_CRC64_ENABLE     0x40000000UL    /**< Indicates whether or not CRC-64 is used 
                                                     *   for application CRC.
                                                     *   Access mode: read only.
                                                     *                                           */
#define APDB_FLAGS_MULTI_APP        0x80000000UL    /**< Indicates whether or not the app-
                                                     *   lication is distributed over multiple
                                                     *   (incoherent) application regions.
                                                     *   Access mode: read only.
                                                     *                                           */
/** @} */

/**************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/

/**
 *
 * \brief Date structure
 *
 *  Data structure for saving dates like flash or build date.
 *
 */
typedef struct bl_t_date_
{
    ubyte4 date;                    /**< Date in format YYYY/MM/DD hh:mm. The fields are defined
                                     *   as followed:
                                     *      - bit  0-11 ... year
                                     *      - bit 12-15 ... month
                                     *      - bit 16-20 ... day
                                     *      - bit 21-25 ... hour
                                     *      - bit 26-31 ... minute
                                     *                                                           */
} BL_T_DATE;

/**
 *
 * \brief CAN ID structure
 *
 */
typedef struct bl_t_can_id_
{
    ubyte4 extended;                /**< Type of CAN identifier to be used. Valid values are:
                                     *      - 0 ... standard CAN identifier is used
                                     *      - 1 ... extended CAN identifier is used
                                     *                                                           */
    ubyte4 ID;                      /**< The CAN identifier (LSB must start at bit 0):
                                     *      - bit 0-10 ... if standard CAN identifier is used
                                     *      - bit 0-28 ... if extended CAN identifier is used
                                     *                                                           */
} BL_T_CAN_ID;

/**
 *
 * \brief APDB structure
 *
 *  Data structure for accessing the Application Descriptor Block.
 *
 */
typedef struct bl_apdb_
{
    ubyte4 APDBVersion;             /**< The APDB version (see #APDB_VERSION):
                                     *      - bit 0-7  ... minor number
                                     *      - bit 8-15 ... major number
                                     *                                                           */
    BL_T_DATE FlashDate;            /**< The date when the application has been flashed
                                     *   (automatically provided by the TTC-Downloader).
                                     *                                                           */
    BL_T_DATE BuildDate;            /**< The application's build date (must be provided by
                                     *   the application).
                                     *                                                           */
    ubyte4 NodeType;                /**< The hardware type the application is built for
                                     *   (automatically provided by the TTC-Downloader).
                                     *                                                           */
    ubyte4 CRCStartAddress;         /**< Start address for CRC calculation or if a CRC table is 
                                     *   used, start address of the CRC table (automatically 
                                     *   provided by the TTC-Downloader).
                                     *                                                           */
    ubyte4 CodeSize;                /**< Code size in bytes (used for CRC calculation) or if a
                                     *   CRC table is used, number of CRC table entries
                                     *   (automatically provided by the TTC-Downloader).
                                     *                                                           */
    ubyte4 LegacyApplicationCRC;    /**< Legacy application CRC for flash checker (automatically
                                     *   provided by the TTC-Downloader).
                                     *                                                           */
    ubyte4 ApplicationCRC;          /**< CRC-32 value calculated over the application or if a
                                     *   CRC table is used, CRC-32 value calculated over the CRC
                                     *   table (automatically provided by the TTC-Downloader).
                                     *                                                           */
    ubyte4 NodeNumber;              /**< The unique node number used to identify nodes of a 
                                     *   CODESYS application. Note that only values from 0 to
                                     *   127 are allowed (must be provided by the application).
                                     *                                                           */
    ubyte4 CRCSeed;                 /**< Seed for application CRC calculation (automatically
                                     *   provided by the TTC-Downloader).
                                     *                                                           */
    ubyte4 Flags;                   /**< Predefined application flags can be specified here.
                                     *    Following flags can be specified:
                                     *       - bit 0 ... Auto-BaudrateDetectionEnable
                                     *                   (HY-TTC 30X family only):
                                     *         - 0 = disable automatic baud rate detection
                                     *         - 1 = enable automatic baud rate detection
                                     *                                                           */
    ubyte4 Hook1;                   /**< Custom hook 1.                                          */
    ubyte4 Hook2;                   /**< Custom hook 2.                                          */
    ubyte4 Hook3;                   /**< Custom hook 3.                                          */
    ubyte4 MainAddress;             /**< The application's start address. Note that the 
                                     *   bootloader uses this address to start the application
                                     *   after reset/power-up. See #APPL_START defined in 
                                     *   file ptypes_apdb.h (must be provided by the 
                                     *   application).
                                     *                                                           */
    BL_T_CAN_ID CANDownloadID;      /**< The CAN identifier used for download direction
                                     *   (TTC-Downloader -> target, must be provided by
                                     *   the application).
                                     *                                                           */
    BL_T_CAN_ID CANUploadID;        /**< The CAN identifier used for upload direction
                                     *   (target -> TTC-Downloader, must be provided by
                                     *   the application).
                                     *                                                           */
    ubyte4 LegacyHeaderCRC;         /**< Legacy header CRC for flash checker (automatically
                                     *   provided by the TTC-Downloader).
                                     *                                                           */
    ubyte4 ApplicationVersion;      /**< The application version (must be provided by the
                                     *   application):
                                     *      - bit  0-15 ... revision number
                                     *      - bit 16-23 ... minor number
                                     *      - bit 24-31 ... major number
                                     *                                                           */
    ubyte4 CANBaudrate;             /**< Baud rate in kbit/s used for CAN communication (must 
                                     *   be provided by the application).
                                     *                                                           */
    ubyte4 CANChannel;              /**< The channel used for CAN communication (must be
                                     *   provided by the application).
                                     *                                                           */
    ubyte4 Password;                /**< The password hash for memory access. Set this field to
                                     *   0 or 0xFFFFFFFF to disable password protection. Note
                                     *   that it is highly recommended to set a password upon
                                     *   application download with the TTC-Downloader.
                                     *                                                           */
    ubyte4 MagicSeed;               /**< Seed for CRC calculation with the MCHK HW module
                                     *   (automatically provided by the TTC-Downloader).
                                     *                                                           */
    ubyte1 TargetIPAddress[4];      /**< Target IP address for Ethernet download (most
                                     *   significant byte first, HY-TTC 500 family only).
                                     *                                                           */
    ubyte1 SubnetMask[4];           /**< Subnet mask for Ethernet download (most
                                     *   significant byte first, HY-TTC 500 family only).
                                     *                                                           */
    ubyte1 DLMulticastIPAddress[4]; /**< Multicast IP address of the TTC-Downloader (most
                                     *   significant byte first, HY-TTC 500 family only).
                                     *                                                           */
    ubyte4 DebugKey;                /**< Debug key for booting the device in debug mode
                                     *   (HY-TTC 500 family only).
                                     *                                                           */
    ubyte4 ABRDTimeout;             /**< The timeout for automatic CAN baud rate detection in
                                     *   seconds (HY-TTC 30X family only).
                                     *                                                           */
    ubyte1 ManufacturerID;          /**< The manufacturer identifier (only required for 
                                     *   applications that are flashed during production of the
                                     *   ECU, provided by TTControl GmbH, the generic ID 0xFF 
                                     *   can be used for other applications)                     */
    ubyte1 ApplicationID;           /**< The application identifier (must be provided by the
                                     *   application).
                                     *                                                           */
    ubyte2 Reserved;                /**< Reserved for future use. Shall be set to 0.             */
    ubyte4 HeaderCRC;               /**< The CRC value calculated over the whole APDB
                                     *   (automatically provided by the TTC-Downloader).
                                     *                                                           */
} BL_APDB;

/**************************************************************************************************
 *
 * E X T E R N A L S
 *
 *************************************************************************************************/

/**
 * \name Application Descriptor Block (APDB)
 *
 * The APDB, which is defined by the user.
 */
extern volatile const BL_APDB Apdb_t;

#ifdef __cplusplus
}
#endif

#endif /* APDB_H_ */
