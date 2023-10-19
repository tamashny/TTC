/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_MPU.h
 *
 * \latexonly \hypertarget{IO_MPU}{} \endlatexonly
 *
 * \brief IO Driver functions for the Memory Protection Unit (MPU)
 *
 * The MPU allows the user to set the access attributes of certain parts of the memory.
 *
 * If an address falls in more than one enabled regions, the one with the greater ID has the higher
 * priority and will determine the access rights.
 * If a region is disabled, it does not affect the access rights in any way.
 *
 * An overview of the memory protection concept and a discusion about the solutions available for the user application
 * can be found in \ref iodrv_prot "I/O Driver Memory Protection".
 *
 * \section mpu_init_section MPU initialization
 *
 * User MPU regions can be initialized only after \c IO_Driver_Init() has been called.
 *
 * Once a region is initialized, it cannot be deinitialized or reconfigured to different parameters.
 *
 * \section mpu_access_section MPU access settings
 *
 * A User MPU region's access settings can be combined by setting the read, write or executable bits in
 * the \c attributes parameter of \c IO_MPU_Init(), with the exception that an area cannot be
 * write-only (i.e. write but not read access).
 *
 * \section mpu_fault_section MPU fault handling
 *
 * See \ref iodrv_prot_concept_violation "MPU Violations" in the I/O Driver Memory Protection.
 *
 * \section mpu_subregions_section MPU subregions
 *
 * Every region is divided into 8 equally-sized subregions. The integrator can freely choose which
 * subregions of a region should be enabled, but it is not possible to disable all subregions. After
 * a region has been initialized, the list of its enabled subregions cannot be changed.
 *
 * For example, to enable subregion 5 through 7, the following needs to be used as the
 * \c subregions parameter of \c IO_MPU_Init():
 * \code
 *   IO_MPU_ENABLE_SUBREGION_5 | IO_MPU_ENABLE_SUBREGION_6 | IO_MPU_ENABLE_SUBREGION_7
 * \endcode
 *
 * All regions are enabled using the macro #IO_MPU_ENABLE_ALL_SUBREGIONS.
 *
 * \section mcu_examples_section MPU code example
 *
 * \code
 *    // Initialize HY-TTC 500 driver
 *    io_error = IO_Driver_Init(NULL);
 *
 *    // Configure User MPU region 0 to protect the I/O Driver RAM (read only).
 *    io_error = IO_MPU_Init(IO_MPU_REGION_0,
 *                           0x08038000UL,
 *                           IO_MPU_SIZE_32_KB,
 *                           IO_MPU_ENABLE_ALL_SUBREGIONS,
 *                           IO_MPU_ACCESS_READ);
 *
 *    // Initialize User MPU region 1 with read-only access to a custom protected data location.
 *    // This region will be enabled before calling the function with limited access
 *    // and disabled after it returns.
 *    io_error = IO_MPU_Init(IO_MPU_REGION_1,
 *                           PROTECTED_DATA_START,
 *                           PROTECTED_DATA_SIZE,
 *                           IO_MPU_ENABLE_ALL_SUBREGIONS,
 *                           IO_MPU_ACCESS_READ);
 *
 *    // start the RTC
 *    IO_RTC_StartTime(&timestamp);
 *
 *    // loop forever
 *    while (1)
 *    {
 *        io_error = IO_Driver_TaskBegin();
 *
 *        // application begin
 *
 *        // call a function without any active access limitations
 *        any_safe_function();
 *
 *        IO_MPU_Enable(IO_MPU_REGION_1);
 *        {
 *            // protection begin
 *
 *            // call a function with limited access rights on the custom memory area
 *            limited_function();
 *
 *            // protection end
 *        }
 *        IO_MPU_Disable(IO_MPU_REGION_1);
 *
 *        // call a function without any active access limitations
 *        any_safe_function();
 *
 *        IO_MPU_Enable(IO_MPU_REGION_1);
 *        IO_MPU_Enable(IO_MPU_REGION_0);
 *        {
 *            // unsafe code begin
 *
 *            // call a unsafe function with (highly) limited access rights
 *            unsafe_function();
 *
 *            // unsafe code end
 *        }
 *        IO_MPU_Disable(IO_MPU_REGION_0);
 *        IO_MPU_Disable(IO_MPU_REGION_1);
 *
 *        // application end
 *        io_error = IO_Driver_TaskEnd();
 *
 *        while (IO_RTC_GetTimeUS(timestamp) < CYCLE_TIME); // wait until cycle time has passed
 *        timestamp += CYCLE_TIME;                          // increase time stamp by cycle time
 *    }
 * \endcode
 *
 *
 * \page iodrv_prot I/O Driver Memory Protection
 *
 * The concept of memory protection becomes relevant when the user application is partitioned into
 * components with different safety levels. The aim is that the memory reserved for safety critical software components
 * (safe code) cannot be corrupted by faults in non-critical components (unsafe code).
 *
 * The CPU supports the memory protection on the lowest (hardware) level by means of the Memory Protection Unit (MPU).
 * The I/O Driver includes an interface for a limited control of the MPU.
 *
 * It should be noted that the support for the memory protection in the I/O Driver is very basic and allows only
 * for rudimentary safety mechanisms.
 * TTControl GmbH offers state of the art memory protection architecture in the SafeRTOS Integration product.
 * It contains a safety-certified operating system that ensures full support for the Freedom from interference,
 * as required by the current safety standards.
 *
 * \section iodrv_prot_concept Concept
 *
 * The MPU defines access permissions to the CPU address space in twelve MPU regions.
 * The IO_MPU module in the I/O Driver allows limited control of four highest-priority regions;
 * the other eight are used to configure the default access privileges to all available areas of the CPU address space,
 * and are invisible to the user.
 *
 * The IO_MPU module allows these four User MPU regions to be initialized and enabled/disabled from the user
 * application to control the access permissions to the memories or memory-mapped peripherals.
 * The I/O Driver start-up code ensures that all available memory is fully accessible by default.
 * The intended use of the User MPU regions is therefore to restrict (deny) access to sensitive memory areas
 * for unsafe code.
 *
 * \subsection iodrv_prot_concept_policies MPU Protection Policies
 *
 * The I/O Driver uses internal data that is necessary for its correct function.
 * The RAM memory where this data is located should be among the protected memory areas when unsafe code runs,
 * however this interferes with the intended functionality of the I/O Driver.
 * The I/O Driver uses several interrupts that can occur during the execution of the unsafe code and that require
 * access to the I/O Driver data, which cannot work with the memory protection in place.
 * For this reason, the I/O Driver can be configured to deactivate the User MPU regions for the execution
 * of its internal interrupt code. The regions are restored when the interrupt returns so that it has no impact
 * on the user application. The I/O Driver offers three different configurations, called MPU protection policies:
 * - #IO_MPU_POLICY_REGION0: The I/O Driver deactivates User MPU region 0 (#IO_MPU_REGION_0) for its internal tasks.
 *   This is the legacy setting that allows using #IO_MPU_REGION_0 for memory protection that may interfere
 *   with the I/O Driver. Other User MPU regions can be used for user-specific data but cannot include addresses that
 *   are accessed by the I/O Driver.
 * - #IO_MPU_POLICY_ALLREGIONS: The I/O Driver deactivates all User MPU regions for its internal tasks.
 *   This policy allows all regions to be used freely (provided that the \ref iodrv_prot_reqs "requirements" below
 *   are fulfilled), including e.g. the memory-mapped peripherals, which is generally required in safety-critical
 *   applications.
 * - #IO_MPU_POLICY_OFF: The I/O Driver does not make any changes to the MPU configuration in its internal tasks.
 *   This is used when the MPU is fully managed by the user application or controlled by an external
 *   software component, e.g. SafeRTOS or CODESYS. This setting can be used if the MPU configuration
 *   does not obstruct the execution of the I/O Driver internal tasks in any way.
 *
 * \attention
 *   The user callbacks and event handlers that are called from the I/O Driver's interrupt code will execute
 *   with the same MPU protection state as the I/O Driver code itself, i.e. with the User MPU regions deactivated
 *   depending on the selected MPU protection policy. The callbacks include the following:
 *   - The error callback (#DIAG_ERROR_CB) and notification callback (#DIAG_NOTIFY_CB).
 *   - The FPU exception callback (#IO_DRIVER_FPU_HANDLER).
 *   - The RTC periodic event handler (#IO_RTC_EVENT_HANDLER).
 *
 * \subsection iodrv_prot_concept_violation MPU Violations
 *
 * When the CPU attempts to access memory for which it does not have sufficient permissions, one of two CPU Exceptions
 * will occur: Data Abort for data accesses, and Prefetch Abort for instruction fetches. In both cases the application
 * is notified in the Notification callback (with the error code #DIAG_E_DATA_ABORT or #DIAG_E_PREFETCH_ABORT),
 * the unit goes to the Safe state, and as the I/O Driver cannot reliably determine how to recover from
 * the faulty memory access, the application execution does *not* continue.
 *
 * \section iodrv_prot_reqs I/O Driver Requirements
 *
 * The MPU defines the CPU's access permissions to the full address space. There are several protection checks built
 * in the IO_MPU API functions that prevent incorrect configuration, though ultimately the user application
 * is in control.
 * The application's MPU configuration can deny access to the memory for the unsafe code as well as
 * the I/O Driver functions, thus preventing the correct functionality of the I/O Driver.
 * This chapter lists the requirements that the user application must follow in order to avoid interfering
 * with the I/O Driver.
 *
 * \subsection iodrv_prot_reqs_iomem I/O Driver Memory Sections
 *
 * The following requirements ensure that the I/O Driver has sufficient access permissions to the I/O Driver code,
 * constants and data memory. The memory sections are defined in the linker file (.lsf) which is part of
 * the I/O Driver release package.
 *
 * \par Requirement "Code"
 * \anchor req_mpu_code
 * The user application shall ensure that the I/O Driver code has read and execute permissions
 * for the following internal flash memory sections:
 * - CSM_CODE,
 * - IO_DRIVER_CODE,
 * - Exception vectors and the Bootloader memory area (address range 0x0 to 0x0001FFFF).
 *
 * \par Requirement "Const"
 * \anchor req_mpu_const
 * The user application shall ensure that the I/O Driver code has read permissions
 * for the following memory sections:
 * - CSM_CONST,
 * - IO_DRIVER_CONST.
 *
 * \par Requirement "Data"
 * \anchor req_mpu_data
 * The user application shall ensure that the I/O Driver code has read and write permissions
 * for the following RAM memory sections:
 * - CSM_VAR_ZERO_INIT_UNSPECIFIED, CSM_VAR_NO_INIT_UNSPECIFIED,
 * - IO_DRIVER_DATA_NORMAL,
 * - IO_DRIVER_DATA_COMMON (address range 0x0803FAE0 to 0x0803FEDF),
 * - Shared Memory area (address range 0x0803FEE0 to 0x0803FFFF).
 *
 * \attention
 *   Note that any modification to the size and location of the memory section IO_DRIVER_DATA_COMMON
 *   or the Shared Memory area is not permitted as they are a part of the interface to the Bootloader.
 *
 * \subsection iodrv_prot_reqs_periph Peripherals
 *
 * The MPU can also protect the CPU peripherals which are controlled from memory-mapped registers.
 * In general, unsafe code should have no access to peripherals, however there is one exception:
 * The VIM peripheral is responsible for dispatching CPU interrupts. The VIM registers must be accessible
 * even from unsafe code, otherwise the I/O Driver interrupts will not execute.
 *
 * \par Requirement "Peripherals"
 * \anchor req_mpu_periph
 * The user application shall always grant at least read access permissions
 * to the VIM (Vectored Interrupt Manager) peripheral memory area (address range 0xFFFFFE00 to 0xFFFFFEFF).
 *
 * \subsection iodrv_prot_reqs_usage Usage
 *
 * The I/O Driver code executes in the main application thread and in interrupts.
 * The above requirements for the MPU access permissions must be fulfilled in both cases:
 *
 * In the main thread, the application enables and disables the User MPU regions using the IO_MPU API functions
 * (e.g. \c IO_MPU_EnableAll() and \c IO_MPU_DisableAll()) as needed to comply with the I/O Driver requirements
 * and with the safety requirements specific to the application. When a User MPU region is configured
 * to deny access to one of the I/O Driver memory sections, this region has to be disabled from the API before
 * any I/O Driver function can be called.
 *
 * The behavior in interrupts depends on the selected MPU protection policy. As explained in
 * \ref iodrv_prot_concept_policies "MPU Policies" above, the I/O Driver automatically disables selected
 * User MPU regions in order to ensure the necessary access permissions.  This happens without direct involvement
 * of the user application, i.e. the user's only responsibility is to correctly configure the User MPU regions.
 *
 * \section iodrv_prot_howto How to Use
 *
 * The solution selected to implement the memory protection depends on the safety level of the user application.
 * The legacy approach only protects sensitive data in the RAM.
 * The #IO_MPU_POLICY_ALLREGIONS policy allows for more thorough protection by including e.g. the CPU peripherals.
 *
 * \subsection iodrv_prot_howto_legacy Legacy Implementation
 *
 * The original approach allowed protecting the I/O Driver data together with safety-critical data
 * of the user application in one User MPU region #IO_MPU_REGION_0. This legacy implementation uses
 * the MPU protection policy #IO_MPU_POLICY_REGION0.
 *
 * The I/O Driver data is placed in dedicated data sections (listed in the \ref req_mpu_data "\"Data\" requirement")
 * by the linker. Some I/O Driver data sections have a fixed location at the end of the internal RAM.
 * The MPU region that covers these sections can be configured as follows:
 *
 * \code
 *   io_error = IO_MPU_Init (IO_MPU_REGION_0,
 *                           0x08038000U,
 *                           IO_MPU_SIZE_32_KB,
 *                           IO_MPU_ENABLE_ALL_SUBREGIONS,
 *                           IO_MPU_ACCESS_READ);
 * \endcode
 *
 * This 32 KB MPU region, when used with the recommended linker file, will include all the I/O Driver data
 * and still leave space for the safety-critical data of the user application.
 * The region will be write-protected (i.e. read-only).
 *
 * The protection is activated when #IO_MPU_REGION_0 is enabled, and deactivated when disabled:
 *
 * \code
 *   // Enable protection of the I/O Driver RAM.
 *   io_error = IO_MPU_Enable (IO_MPU_REGION_0);
 *
 *   // Unsafe code:
 *   // The I/O Driver RAM (together with other data in the same memory area) is now write protected.
 *   // Do not call any IO_...() functions here!
 *
 *   // Disable protection of the I/O Driver RAM.
 *   io_error = IO_MPU_Disable (IO_MPU_REGION_0);
 * \endcode
 *
 * If #IO_MPU_REGION_0 is used for the I/O Driver data together with the data of the user application
 * and its size is not sufficient, it can be enlarged. In such cases, the start address has to be adjusted accordingly.
 *
 * The other three User MPU regions can be used for user-specific data but cannot include any I/O Driver data
 * or memory addresses that are accessed by the I/O Driver.
 *
 * \subsection iodrv_prot_howto_full Full Solution
 *
 * Selecting the #IO_MPU_POLICY_ALLREGIONS policy allows use of all four User MPU regions for memory protection,
 * regardless of if they cover I/O Driver data, CPU peripherals or other types of memory.
 *
 * The memory location of user-specific data can be controlled by the linker file. This approach has many benefits
 * over manual placement: Among other things, the linker will reserve a memory area of defined size
 * for the memory region and ensure that its location is aligned according to the size:
 *
 * \code
 *   // Memory section reserved for safe user data.
 *   USER_SAFE_DATA : load=ram, palign=0x400, START(UserSafeData_Start), SIZE(UserSafeData_Size)
 * \endcode
 *
 * This memory section can be used in a C source file to store safe data as follows:
 *
 * \code
 *   // Safe user data - placed in the protected memory area.
 *   #pragma DATA_SECTION (safe_data, "USER_SAFE_DATA")
 *   ubyte4 safe_data = 0u;
 * \endcode
 *
 * The linker will also export two symbols, \c UserSafeData_Start and \c UserSafeData_Size, which can be used
 * to initialize the User MPU regions. The following code snippet shows an example MPU configuration that protects
 * I/O Driver data, safe user data and all peripherals except the RTI and VIM:
 *
 * \code
 *   extern ubyte4 UserSafeData_Start;
 *   extern ubyte4 UserSafeData_Size;
 *
 *   ...
 *
 *   // Configure the MPU protection policy.
 *   io_error = IO_MPU_Policy (IO_MPU_POLICY_ALLREGIONS);
 *
 *   // User MPU region 0: I/O Driver protected data.
 *   io_error = IO_MPU_Init (IO_MPU_REGION_0,
 *                           0x08038000UL,
 *                           IO_MPU_SIZE_32_KB,
 *                           IO_MPU_ENABLE_ALL_SUBREGIONS,
 *                           IO_MPU_ACCESS_READ);
 *
 *   // User MPU region 1: Protect all peripherals (no access).
 *   io_error = IO_MPU_Init (IO_MPU_REGION_1,
 *                           0xFC000000UL,
 *                           IO_MPU_SIZE_64_MB,
 *                           IO_MPU_ENABLE_ALL_SUBREGIONS,
 *                           IO_MPU_ACCESS_NONE);
 *
 *
 *   // User MPU region 2: Allow access to peripherals RTI + VIM (override IO_MPU_REGION_1).
 *   io_error = IO_MPU_Init (IO_MPU_REGION_2,
 *                           0xFFFFF800UL,
 *                           IO_MPU_SIZE_2_KB,
 *                           IO_MPU_ENABLE_SUBREGION_4 | IO_MPU_ENABLE_SUBREGION_6,
 *                           IO_MPU_ACCESS_READ);
 *
 *   // User MPU region 3: Safe user data.
 *   io_error = IO_MPU_Init (IO_MPU_REGION_3,
 *                           (ubyte4) &UserSafeData_Start,
 *                           (ubyte4) &UserSafeData_Size,
 *                           IO_MPU_ENABLE_ALL_SUBREGIONS,
 *                           IO_MPU_ACCESS_READ);
 * \endcode
 *
 * Using this MPU configuration is as simple as before:
 *
 * \code
 *   // Enable MPU protection.
 *   io_error = IO_MPU_EnableAll();
 *
 *   // Unsafe code:
 *   // All four User MPU regions are now enabled.
 *   // Do not call any IO_...() functions here!
 *
 *   // Disable MPU protection.
 *   io_error = IO_MPU_DisableAll();
 * \endcode
 *
 * \section iodrv_prot_limitations Limitations
 *
 * As briefly mentioned at the beginning, the support for the memory protection is very basic and has several
 * weaknesses. Probably the most severe limitation comes from the fact that without the help of an operating
 * system (OS), pure I/O Driver applications run in the privileged (SVC) mode of the CPU, regardless if the code
 * is safe or unsafe. This means that the unsafe code can reconfigure or even deactivate the MPU -- thus
 * disabling whatever protection was in place. The compliance with the safety standards in this case
 * has to be ensured by other means (e.g. a code review).
 *
 * When unsafe functions are called directly from safe functions, they will share the same stack.
 * The MPU module does not offer any special functions to protect the safe part of the stack from the unsafe code.
 * This protection, if necessary, has to be again achieved using other mechanisms, e.g. calculating the CRC
 * of the safe stack just before and checking it just after calling the unsafe function.
 *
 *************************************************************************************************/

#ifndef IO_MPU_H_
#define IO_MPU_H_

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
 * \name User MPU regions
 * \anchor mpu_regions
 * @{
 */

/** User MPU region 0 (lowest priority region) */
#define IO_MPU_REGION_0         0U

/** User MPU region 1 */
#define IO_MPU_REGION_1         1U

/** User MPU region 2 */
#define IO_MPU_REGION_2         2U

/** User MPU region 3 (highest priority region) */
#define IO_MPU_REGION_3         3U

/** @} */

/**
 * \name MPU subregions
 * \anchor mpu_subregions
 * @{
 */

/** This MPU setting enables subregion 0 */
#define IO_MPU_ENABLE_SUBREGION_0         0x01U

/** This MPU setting enables subregion 1 */
#define IO_MPU_ENABLE_SUBREGION_1         0x02U

/** This MPU setting enables subregion 2 */
#define IO_MPU_ENABLE_SUBREGION_2         0x04U

/** This MPU setting enables subregion 3 */
#define IO_MPU_ENABLE_SUBREGION_3         0x08U

/** This MPU setting enables subregion 4 */
#define IO_MPU_ENABLE_SUBREGION_4         0x10U

/** This MPU setting enables subregion 5 */
#define IO_MPU_ENABLE_SUBREGION_5         0x20U

/** This MPU setting enables subregion 6 */
#define IO_MPU_ENABLE_SUBREGION_6         0x40U

/** This MPU setting enables subregion 7 */
#define IO_MPU_ENABLE_SUBREGION_7         0x80U

/** This MPU setting enables all subregions */
#define IO_MPU_ENABLE_ALL_SUBREGIONS      0xFFU

/** @} */

/**
 * \name MPU region sizes
 * \anchor mpu_sizes
 * @{
 */

/** This size setting defines the memory region size to be 32 bytes */
#define IO_MPU_SIZE_32_B        0x00000020UL

/** This size setting defines the memory region size to be 64 bytes */
#define IO_MPU_SIZE_64_B        0x00000040UL

/** This size setting defines the memory region size to be 128 bytes */
#define IO_MPU_SIZE_128_B       0x00000080UL

/** This size setting defines the memory region size to be 256 bytes */
#define IO_MPU_SIZE_256_B       0x00000100UL

/** This size setting defines the memory region size to be 512 bytes */
#define IO_MPU_SIZE_512_B       0x00000200UL

/** This size setting defines the memory region size to be 1 KB (1024 bytes) */
#define IO_MPU_SIZE_1_KB        0x00000400UL

/** This size setting defines the memory region size to be 2 KB (2048 bytes) */
#define IO_MPU_SIZE_2_KB        0x00000800UL

/** This size setting defines the memory region size to be 4 KB (4096 bytes) */
#define IO_MPU_SIZE_4_KB        0x00001000UL

/** This size setting defines the memory region size to be 8 KB (8192 bytes) */
#define IO_MPU_SIZE_8_KB        0x00002000UL

/** This size setting defines the memory region size to be 16 KB (16,384 bytes) */
#define IO_MPU_SIZE_16_KB       0x00004000UL

/** This size setting defines the memory region size to be 32 KB (32,768 bytes) */
#define IO_MPU_SIZE_32_KB       0x00008000UL

/** This size setting defines the memory region size to be 64 KB (65,536 bytes) */
#define IO_MPU_SIZE_64_KB       0x00010000UL

/** This size setting defines the memory region size to be 128 KB (131,072 bytes) */
#define IO_MPU_SIZE_128_KB      0x00020000UL

/** This size setting defines the memory region size to be 256 KB (262,144 bytes) */
#define IO_MPU_SIZE_256_KB      0x00040000UL

/** This size setting defines the memory region size to be 512 KB (524,288 bytes) */
#define IO_MPU_SIZE_512_KB      0x00080000UL

/** This size setting defines the memory region size to be 1 MB (1,048,576 bytes) */
#define IO_MPU_SIZE_1_MB        0x00100000UL

/** This size setting defines the memory region size to be 2 MB (2,097,152 bytes) */
#define IO_MPU_SIZE_2_MB        0x00200000UL

/** This size setting defines the memory region size to be 4 MB (4,194,304 bytes) */
#define IO_MPU_SIZE_4_MB        0x00400000UL

/** This size setting defines the memory region size to be 8 MB (8,388,608 bytes) */
#define IO_MPU_SIZE_8_MB        0x00800000UL

/** This size setting defines the memory region size to be 16 MB (16,777,216 bytes) */
#define IO_MPU_SIZE_16_MB       0x01000000UL

/** This size setting defines the memory region size to be 32 MB (33,554,432 bytes) */
#define IO_MPU_SIZE_32_MB       0x02000000UL

/** This size setting defines the memory region size to be 64 MB (67,108,864 bytes) */
#define IO_MPU_SIZE_64_MB       0x04000000UL

/** @} */

/**
 * \name MPU region access attributes
 * \anchor mpu_attributes
 * @{
 */

/** This access setting doesn't allow any access to the memory region. */
#define IO_MPU_ACCESS_NONE              0x0U

/** This access setting allows only read access to the memory region. */
#define IO_MPU_ACCESS_READ              0x1U

/** This access setting allows read and write accesses to the memory region. */
#define IO_MPU_ACCESS_READ_WRITE        0x3U

/** This access setting allows read and execute accesses to the memory region. */
#define IO_MPU_ACCESS_READ_EXECUTE      0x5U

/** This access setting allows any access to the memory region (read, write and execute). */
#define IO_MPU_ACCESS_ANY               0x7U

/** @} */

/**
 * \name MPU protection policies
 * \anchor mpu_policies
 * @{
 */

/** I/O Driver deactivates #IO_MPU_REGION_0 for its internal tasks.
 *  This is the behavior of I/O Driver version 3.0.0 and earlier.
 *  This setting is the default (i.e. it will be used if \c IO_MPU_Policy() is never called)
 *  to maintain backwards compatibility.
 *
 *  See also \ref iodrv_prot_howto_legacy "I/O Driver Memory Protection".
 */
#define IO_MPU_POLICY_REGION0           0x0U

/** I/O Driver does not make any changes to the MPU configuration in its internal tasks.
 *  This setting is used when the MPU is fully controlled by an external software component, e.g. SafeRTOS or CODESYS.
 *  User applications can use this setting if their configuration of the \ref mpu_regions "User MPU regions"
 *  does not obstruct the execution of the I/O Driver internal tasks.
 */
#define IO_MPU_POLICY_OFF               0x1U

/** I/O Driver deactivates all \ref mpu_regions "User MPU regions" for its internal tasks.
 *  With this setting, users can use all \ref mpu_regions "User MPU regions" freely
 *  to implement the memory protection needed in their application.
 *
 *  See also \ref iodrv_prot_howto_full "I/O Driver Memory Protection".
 */
#define IO_MPU_POLICY_ALLREGIONS        0x2U
/** @} */

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Initializes a User MPU region.
 *
 * \param region_id         Region ID. One of the macros in the \ref mpu_regions "User MPU regions".
 *
 * \param start_address     Start address of the region. Must be a multiple of the region size.
 *
 * \param size              Size of the region. One of the macros in the
 *                          \ref mpu_sizes "MPU region sizes".
 *
 * \param subregions        Defines which subregions of this region are enabled, where the
 *                          N-th least significant bit denotes the N-th subregion.
 *                          See section \ref mpu_subregions_section "MPU subregions" for details.
 *
 * \param attributes        Access attributes of the region. One of the macros in the
 *                          \ref mpu_attributes "MPU region access attributes".
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      Everything went fine.
 * \retval #IO_E_BUSY                    The MPU region has already been initialized once.
 * \retval #IO_E_INVALID_PARAMETER       An invalid parameter was given.
 * \retval #IO_E_DRIVER_NOT_INITIALIZED  The common driver init function has not been called before
 *
 **************************************************************************************************
 *
 * \remarks
 * The region must lie entirely (including its beginning and end):
 *  - between 0x0000.0000 and 0x002F.FFFF for code regions (internal flash memory),
 *  - between 0x0800.0000 and 0x0803.FFFF for regions in the internal RAM,
 *  - between 0x6000.0000 and 0x601F.FFFF for regions in the external RAM,
 *  - between 0x6400.0000 and 0x647F.FFFF for regions in the external flash memory.
 *  - between 0xF000.0000 and 0xF07F.FFFF for regions in the OTP, ECC, and application configuration data region
 *    (internal flash memory).
 *  - between 0xFC00.0000 and 0xFFFF.FFFF for regions in the peripherals address space.
 *
 *************************************************************************************************/
IO_ErrorType IO_MPU_Init(ubyte1 region_id,
                         ubyte4 start_address,
                         ubyte4 size,
                         ubyte1 subregions,
                         ubyte1 attributes);

/**********************************************************************************************//**
 *
 * \brief Enables (activates) the given User MPU region configuration
 *
 * \param region_id         Region ID. One of the macros in the \ref mpu_regions "User MPU regions".
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything went fine.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The given region has not been initialized.
 * \retval #IO_E_MPU_REGION_ENABLED     The given region is already enabled.
 * \retval #IO_E_INVALID_PARAMETER      An invalid region ID was given.
 *
 *************************************************************************************************/
IO_ErrorType IO_MPU_Enable(ubyte1 region_id);

/**********************************************************************************************//**
 *
 * \brief Disables (deactivates) the given User MPU region configuration
 *
 * \param region_id         Region ID. One of the macros in the \ref mpu_regions "User MPU regions".
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     Everything went fine.
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED The given region has not been initialized.
 * \retval #IO_E_MPU_REGION_DISABLED    The given region is already disabled.
 * \retval #IO_E_INVALID_PARAMETER      An invalid region ID was given.
 *
 *************************************************************************************************/
IO_ErrorType IO_MPU_Disable(ubyte1 region_id);

/**********************************************************************************************//**
 *
 * \brief Configures the MPU protection policy.
 *
 * \param policy            The policy that will be used for handling the \ref mpu_regions "User MPU regions"
 *                          in the I/O Driver internal tasks:
 *                              - #IO_MPU_POLICY_REGION0
 *                              - #IO_MPU_POLICY_OFF
 *                              - #IO_MPU_POLICY_ALLREGIONS
 * \note
 * The I/O Driver defaults to use the #IO_MPU_POLICY_REGION0 setting if this function is not called.
 *
 * \attention
 * Using this function requires Bootloader 3.2 or newer.
 *
 * \attention
 * If an upgrade of the Bootloader is not possible, the function can be used with older Bootloader versions
 * provided that the following requirement is met:\n
 * The memory area IO_DRIVER_DATA_COMMON (address range 0x0803FAE0 to 0x0803FEDF) shall be covered by #IO_MPU_REGION_0
 * or no User MPU region at all.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      Operation successful.
 * \retval #IO_E_INVALID_PARAMETER       The specified policy is not valid.
 *
 *************************************************************************************************/
IO_ErrorType IO_MPU_Policy (ubyte4 policy);

/**********************************************************************************************//**
 *
 * \brief Enables (activates) all initialized \ref mpu_regions "User MPU regions"
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      Operation successful.
 * \retval #IO_E_MPU_REGION_ENABLED      One of the regions is already enabled.
 * \retval #IO_E_DRIVER_NOT_INITIALIZED  The common driver initialization function has not been called before.
 *
 *************************************************************************************************/
IO_ErrorType IO_MPU_EnableAll (void);

/**********************************************************************************************//**
 *
 * \brief Disables (deactivates) all initialized \ref mpu_regions "User MPU regions"
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      Operation successful.
 * \retval #IO_E_MPU_REGION_DISABLED     One of the regions is already disabled.
 * \retval #IO_E_DRIVER_NOT_INITIALIZED  The common driver initialization function has not been called before.
 *
 *************************************************************************************************/
IO_ErrorType IO_MPU_DisableAll (void);

#ifdef __cplusplus
}
#endif

#endif /* IO_MPU_H_ */
