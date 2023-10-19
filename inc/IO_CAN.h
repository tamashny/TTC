/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_CAN.h
 *
 * \latexonly \hypertarget{IO_CAN}{} \endlatexonly
 *
 * \brief IO Driver functions for CAN communication.
 *
 *  The CAN driver uses the DCAN module of the TMS570 CPU.
 *
 *  The CAN driver supports up to 7 CAN channels. The number of available channels depends on the
 *  used HW variant.
 *
 *  All CAN nodes have an own set of message objects and can not be allocated to other CAN nodes.
 *  For further details on message objects see \ref can_handles "CAN handles and message objects".
 *
 *  CAN-API Usage:
 *    - \ref can_examples "Examples for CAN API functions"
 *
 *  \section can_ac_mask_usage Usage of the acceptance masks
 *
 *  The acceptance mask defines the relevant bits of the CAN ID. A binary 1 marks a relevant bit in
 *  the CAN ID on the same position.
 *
 *  Setting all bits of the acceptance mask (0x1FFFFFFF) only accepts the ID set with the ID
 *  parameter and rejects all other IDs. Setting the acceptance mask to 0 causes the message buffer
 *  to accept any IDs.
 *
 *  Using this mechanism a message buffer can be used to accept a  range of CAN IDs.
 *
 *  Example for an 11-bit ID:
 *  \code
 *      ac_mask = 0x00000700 = 0 b 0 0000 0000 0000 0000 0111 0000 0000
 *      id      = 0x00000200 = 0 b 0 0000 0000 0000 0000 0010 0000 0000
 *  \endcode
 *  in this example all messages with an ID between 0x200 and 0x2FF
 *  are accepted.
 *
 *
 *  \section can_handles CAN handles and message objects
 *
 *  The DCAN module of the TMS570 CPU provides a certain number of so called <em>CAN message
 *  objects</em>. A message object is a dedicated memory area which is used to store a CAN message.
 *  Every message object has its own identifier and acceptance mask. The number of available
 *  message objects depends on the CAN channel:
 *
 * \verbatim[table]
 * ======================== ======================================================
 *   CAN channel                   message objects
 * ------------------------ ------------------------------------------------------
 *   \c #IO_CAN_CHANNEL_0          <b>64</b> for receive and transmit
 * ------------------------ ------------------------------------------------------
 *   \c #IO_CAN_CHANNEL_1          <b>64</b> for receive and transmit
 * ------------------------ ------------------------------------------------------
 *   \c #IO_CAN_CHANNEL_2          <b>64</b> for receive and transmit
 * ------------------------ ------------------------------------------------------
 *   \c #IO_CAN_CHANNEL_3          <b>24</b> for receive, <b>8</b> for transmit
 * ------------------------ ------------------------------------------------------
 *   \c #IO_CAN_CHANNEL_4          <b>24</b> for receive, <b>8</b> for transmit
 * ------------------------ ------------------------------------------------------
 *   \c #IO_CAN_CHANNEL_5          <b>24</b> for receive, <b>8</b> for transmit
 * ------------------------ ------------------------------------------------------
 *   \c #IO_CAN_CHANNEL_6          <b>24</b> for receive, <b>8</b> for transmit
 * ------------------------ ------------------------------------------------------
 * \endverbatim
 *
 *  For receiving and transmitting CAN messages a message object needs to be configured. During the
 *  configuration the driver will return a so called <em>CAN handle</em>.
 *
 *  The <em>CAN handle</em> is a reference to the configured message object and is used used to
 *  exchange data with the driver. There are two types of CAN handles:
 *   - Standard message handles for receive and transmit
 *   - FIFO buffer handles for receive and transmit
 *
 *  The number of CAN handles is identical to the number of message objects.
 *
 *  \subsection can_handles_std Standard message handles
 *    A standard message handle is configured by calling the driver function \c IO_CAN_ConfigMsg().
 *    It references to a single message object which can store <em>one</em> CAN message.
 *
 *    For receiving/transmitting data via a single message object the driver functions
 *    \c IO_CAN_ReadMsg() and \c IO_CAN_WriteMsg() are provided.
 *
 *    The status of a standard message handle can be checked with the function
 *    \c IO_CAN_MsgStatus().
 *
 *  \subsection can_handles_fifo FIFO buffer handles
 *    \attention The driver supports FIFO buffers only for receive direction.
 *
 *    A FIFO buffer can be created by linking several message objects to one FIFO buffer which is
 *    able to store multiple CAN message.
 *
 *    A FIFO buffer handle is configured by calling the function \c IO_CAN_ConfigFIFO(). The
 *    desired size of the buffer is passed to the function via a parameter.
 *
 *    For receiving data via FIFO buffers the driver provides the function \c IO_CAN_ReadFIFO().
 *    With this function multiple CAN messages can be retrieved with one function call.
 *
 *    The status of a FIFO buffer handle can be checked with the function \c IO_CAN_FIFOStatus().
 *
 * \section can_examples CAN Code Examples
 * \brief Example for using the CAN API
 * \subsection can_init_examples Examples for CAN initialization
 * \code
 *      ubyte2 handle_w, handle_r;
 *
 *      IO_CAN_Init(IO_CAN_CHANNEL_0,
 *                  IO_CAN_BIT_500_KB,
 *                  0,     // default
 *                  0,     // default
 *                  0,     // default
 *                  0);    // default
 *
 *      // standard message buffers
 *
 *      IO_CAN_ConfigMsg(&handle_w,
 *                       IO_CAN_CHANNEL_0,  // channel 0
 *                       IO_CAN_MSG_WRITE,  // transmit message buffer
 *                       IO_CAN_STD_FRAME,  // standard ID
 *                       0,
 *                       0);
 *
 *      IO_CAN_ConfigMsg(&handle_r,
 *                       IO_CAN_CHANNEL_0,  // channel 0
 *                       IO_CAN_MSG_READ,   // receive message buffer
 *                       IO_CAN_STD_FRAME,  // standard ID
 *                       1,
 *                       0x1FFFFFFF);       // accept only ID 1
 *
 * \endcode
 *
 * \subsection can_task_examples Example for CAN task function call
 * \code
 *      IO_CAN_DATA_FRAME can_frame;
 *
 *      // check if new message has been received
 *      if (IO_CAN_MsgStatus(handle_r) == IO_E_OK)
 *      {
 *          // if message has been received, read the message from the buffer
 *          IO_CAN_ReadMsg(handle_r,
 *                         &can_frame);
 *
 *          // received message is now stored in can_frame
 *          // and can be used by the application
 *      }
 *
 *
 *      // assemble CAN frame:
 *      can_frame.id = 1;
 *      can_frame.id_format = IO_CAN_STD_FRAME;
 *      can_frame.length = 4;
 *      can_frame.data[0] = 1;
 *      can_frame.data[1] = 2;
 *      can_frame.data[2] = 3;
 *      can_frame.data[3] = 4;
 *
 *      // transmit message
 *      IO_CAN_WriteMsg(handle_w,
 *                      &can_frame);
 *
 *      // wait until the transmission has been finished:
 *      while (IO_CAN_MsgStatus(handle_w) != IO_E_OK);
 *
 * \endcode
 *
 * \subsection can_fifo_examples Example for CAN FIFO usage
 * \code
 *      ubyte2 handle_fifo;
 *      ubyte1 rx_count;
 *      IO_CAN_DATA_FRAME fifo_frame[20];
 *
 *      // FIFO message buffer initialization
 *
 *      IO_CAN_ConfigFIFO(&handle_fifo,
 *                        IO_CAN_CHANNEL_0, // channel 0
 *                        20,               // 20 items
 *                        IO_CAN_STD_FRAME, // standard ID
 *                        0x700,            // accept all IDs from
 *                        0x700);           // 0x700 - 0x7FF
 *
 *      while (1)
 *      {
 *          // Read FIFO buffer
 *          IO_CAN_ReadFIFO(handle_fifo,
 *                          &fifo_frame[0],
 *                          sizeof(fifo_frame) / sizeof(IO_CAN_DATA_FRAME),
 *                          &rx_count);
 *
 *          if (rx_count > 0)
 *          {
 *              // process received frames
 *          }
 *      }
 *
 * \endcode
 *
 *************************************************************************************************/

#ifndef IO_CAN_H_
#define IO_CAN_H_

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
 * \name Message buffer direction
 * \anchor can_buffer_direction
 *
 * Selects the transmission direction of a CAN message buffer
 *
 * @{
 */
#define IO_CAN_MSG_READ     0U   /**< used to setup a message buffer for receiving */
#define IO_CAN_MSG_WRITE    1U   /**< used to setup a message buffer for transmitting */
/** @} */

/**
 * \name CAN frame format
 * \anchor can_format
 *
 * Selects the format for a CAN frame
 *
 * @{
 */
#define IO_CAN_STD_FRAME    0U   /**< the ID parameter holds a standard (11-bit) ID */
#define IO_CAN_EXT_FRAME    1U   /**< the ID parameter holds an extended (29-bit) ID */
/** @} */

/**
 * \name CAN baudrate
 * \anchor can_baudrate
 *
 * Selects the baudrate for a CAN channel
 *
 * @{
 */
#define IO_CAN_BIT_USER       0U      /**< Configure CAN Channel with user defined baudrate
                                           (\c tseg1, \c tseg2, \c sjw and \c brp) */
#define IO_CAN_BIT_50_KB      50U     /**< Configure CAN Channel with 50 kBit/s */
#define IO_CAN_BIT_100_KB     100U    /**< Configure CAN Channel with 100 kBit/s */
#define IO_CAN_BIT_125_KB     125U    /**< Configure CAN Channel with 125 kBit/s */
#define IO_CAN_BIT_250_KB     250U    /**< Configure CAN Channel with 250 kBit/s */
#define IO_CAN_BIT_500_KB     500U    /**< Configure CAN Channel with 500 kBit/s */
#define IO_CAN_BIT_1000_KB    1000U   /**< Configure CAN Channel with 1000 kBit/s */
/** @} */

/**************************************************************************************************
 *
 * D A T A   S T R U C T U R E S
 *
 *************************************************************************************************/

/**
 *
 * \brief CAN data frame
 *
 *  Stores a data frame for the CAN communication.
 *
 */
typedef struct io_can_data_frame_
{
    ubyte1 data[8];      /**< data buffer */
    ubyte1 length;       /**< number of words in transmit buffer */
    ubyte1 id_format;    /**< standard or extended format */
    ubyte4 id;           /**< ID for CAN communication */
} IO_CAN_DATA_FRAME;

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Initialization of the CAN communication driver.
 *
 *   The function
 *      - Enables the module
 *      - Sets the module clock to 16MHz
 *      - Automatically sets up the bit timing for the given baudrate
 *
 * \param channel           CAN channel, one of:
 *                              - \c #IO_CAN_CHANNEL_0
 *                              - \c #IO_CAN_CHANNEL_1
 *                              - \c #IO_CAN_CHANNEL_2
 *                              - \c #IO_CAN_CHANNEL_3
 *                              - \c #IO_CAN_CHANNEL_4
 *                              - \c #IO_CAN_CHANNEL_5
 *                              - \c #IO_CAN_CHANNEL_6
 * \param baudrate          Baud rate in kbit/s, one of:
 *                              - \c #IO_CAN_BIT_50_KB
 *                              - \c #IO_CAN_BIT_100_KB
 *                              - \c #IO_CAN_BIT_125_KB
 *                              - \c #IO_CAN_BIT_250_KB
 *                              - \c #IO_CAN_BIT_500_KB
 *                              - \c #IO_CAN_BIT_1000_KB
 *                              - \c #IO_CAN_BIT_USER
 * \param tseg1             Time segment before sample point (3 ... 16)
 * \param tseg2             Time segment after sample point (2 ... 8)
 * \param sjw               Synchronization jump width (1 ... 4)
 * \param brp               Baud rate prescaler ( 1 ... 64)
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_PARAMETER      invalid parameter has been passed
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist or is not available on
 *                                      the used ECU variant
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a CAN channel
 * \retval #IO_E_CHANNEL_BUSY           channel has been initialized before
 * \retval #IO_E_DRIVER_NOT_INITIALIZED the common driver init function has not been called before
 * \retval #IO_E_FPGA_NOT_INITIALIZED   the FPGA has not been initialized
 * \retval #IO_E_CAN_TIMEOUT            the CAN node reported a timeout
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 * \remarks
 *    - Module is initialized only once. To re-initialize the module, the function IO_CAN_DeInit()
 *    needs to be called.
 *
 * \remarks
 *    - For baudrates unequal to \c #IO_CAN_BIT_USER the following bit timings are set, independent
 *    from the user configuration:
 *       - tseg1 = 6
 *       - tseg2 = 1
 *       - sjw = 1
 *       - Sampling Point: 87.5%
 *
 * \remarks
 *    - Set the parameter baudrate to \c #IO_CAN_BIT_USER, to set custom bit timings. The following
 *    has to be fulfilled:\n
 *       - tseg1 >= tseg2
 *       - tseg2 > sjw
 *
 * \remarks
 *    - The timing parameters and baudrate are calculated as follows:\n
 *   The time quanta <b>"tq"</b> results from the baudrate prescaler:\n
 *   tq = ( 1 / 16,000,000Hz ) * ( brp * 2)\n
 *   \n
 *   The synchronization time (<b>Tsync [s]</b>), Phase Buffer Segment Time 1 (<b>Tseg1 [s]</b>)\n
 *   and Phase Buffer Segment Time (<b>Tseg2 [s]</b>) are calculated as follows based
 *   upon <b>"tq"</b>:\n
 *   Tsync = 1 * tq\n
 *   TSeg1 = tseg1 * tq\n
 *   TSeg2 = tseg2 * tq\n
 *   \n
 *   The overall <b>bit time [s]</b> and <b>baudrate [bit/s]</b> are calculated with:\n
 *   bit_time = Tsync + TSeg1 + TSeg2\n
 *   baudrate = 1 / bit_time\n
 *   \n
 *   <b>Example:</b>\n
 *   brp = 4, tseg1 = 3, tseg2 = 4 and sjw = 1\n
 *   tq = ( 1 / 16,000,000Hz ) * ( 4 * 2) = 500ns\n
 *   Tsync = 1 * 500ns = 500ns\n
 *   TSeg1 = 3 * 500ns = 1500ns\n
 *   TSeg2 = 4 * 500ns = 2000ns\n
 *   \n
 *   bit_time = 500ns + 1500ns + 2000ns = 4000ns\n
 *   baudrate = 1/4000ns = 250,000 bit/s\n
 *   \n
 *   The <b>sampling point</b> can be calculated with:\n
 *   sampling_point = (1 + tseg1) / (1 + tseg1 + tseg2)
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_Init(ubyte1 channel,
                         ubyte2 baudrate,
                         ubyte1 tseg1,
                         ubyte1 tseg2,
                         ubyte1 sjw,
                         ubyte1 brp);

/**********************************************************************************************//**
 *
 * \brief Deinitializes a single message handle
 *
 *   Allows re-initialization by \c IO_CAN_ConfigMsg().
 *
 * \param handle            CAN message handle (retrieved from \c IO_CAN_ConfigMsg())
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_CAN_WRONG_HANDLE        a wrong handle has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  handle has not been initialized
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_DeInitHandle(ubyte2 handle);

/**********************************************************************************************//**
 *
 * \brief Deinitializes the given CAN channel.
 *
 *   Allows re-initialization by \c IO_CAN_Init()
 *
 * \param channel           CAN channel, one of:
 *                              - \c #IO_CAN_CHANNEL_0
 *                              - \c #IO_CAN_CHANNEL_1
 *                              - \c #IO_CAN_CHANNEL_2
 *                              - \c #IO_CAN_CHANNEL_3
 *                              - \c #IO_CAN_CHANNEL_4
 *                              - \c #IO_CAN_CHANNEL_5
 *                              - \c #IO_CAN_CHANNEL_6
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY           the given channel is not a CAN channel
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  channel has not been initialized
 * \retval #IO_E_CAN_TIMEOUT             the CAN node reported a timeout
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 * \remarks
 *   This function will also reset all the handles related to this channel.
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_DeInit(ubyte1 channel);

/**********************************************************************************************//**
 *
 * \brief Configures a message object for the given CAN channel.
 *
 *   Returns a message object handle.
 *
 * \param[out] handle       Returns the message object handle
 * \param channel           CAN channel, one of:
 *                              - \c #IO_CAN_CHANNEL_0
 *                              - \c #IO_CAN_CHANNEL_1
 *                              - \c #IO_CAN_CHANNEL_2
 *                              - \c #IO_CAN_CHANNEL_3
 *                              - \c #IO_CAN_CHANNEL_4
 *                              - \c #IO_CAN_CHANNEL_5
 *                              - \c #IO_CAN_CHANNEL_6
 * \param mode              Mode for CAN Message, one of:
 *                              - \c #IO_CAN_MSG_READ,
 *                              - \c #IO_CAN_MSG_WRITE
 * \param id_format         Format of message identifier, one of:
 *                              - \c #IO_CAN_STD_FRAME,
 *                              - \c #IO_CAN_EXT_FRAME
 * \param id                CAN message identifier
 *                              - 0..0x7FF for \c #IO_CAN_STD_FRAME
 *                              - 0..0x1FFFFFFF for \c #IO_CAN_EXT_FRAME
 * \param ac_mask           CAN acceptance mask. Refer to \ref can_ac_mask_usage
 *                          "Usage of the acceptance mask" for further details.
 *                              - 0..0x7FF for \c #IO_CAN_STD_FRAME
 *                              - 0..0x1FFFFFFF for \c #IO_CAN_EXT_FRAME
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_CAN_MAX_MO_REACHED      no more HW message objects are available
 * \retval #IO_E_CAN_MAX_HANDLES_REACHED no more handles are available
 * \retval #IO_E_NULL_POINTER            null Pointer has been passed
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY           the given channel is not a CAN channel
 * \retval #IO_E_INVALID_PARAMETER       invalid parameter has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel was not initialized
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_ConfigMsg(ubyte2 * const handle,
                              ubyte1 channel,
                              ubyte1 mode,
                              ubyte1 id_format,
                              ubyte4 id,
                              ubyte4 ac_mask);

/**********************************************************************************************//**
 *
 * \brief Reads a message from a given message object
 *
 *   Returns the data of a message object and whether the message is new or
 *   not.
 *
 * \param handle            CAN message object handle (retrieved from \c IO_CAN_ConfigMsg())
 * \param[out] buffer       Pointer to data buffer structure. The received frame will be stored
 *                          there.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CAN_OVERFLOW           overflow of message object
 * \retval #IO_E_CAN_OLD_DATA           no new data has been received since the last read
 * \retval #IO_E_CAN_WRONG_HANDLE       invalid handle has been passed
 * \retval #IO_E_NULL_POINTER           null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given handle was not configured
 * \retval #IO_E_CAN_TIMEOUT            the CAN node reported a timeout
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_ReadMsg(ubyte2 handle,
                            IO_CAN_DATA_FRAME * const buffer);

/**********************************************************************************************//**
 *
 * \brief Transmits a CAN message, using the given channel and message object
 *
 *   Returns whether the transmission has been started successfully or not.
 *
 * \param handle            CAN message object handle (retrieved from \c IO_CAN_ConfigMsg())
 * \param[in] data          Pointer to data structure. The data in this structure will be
 *                          transmitted.
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_BUSY                   message object busy - no data has been transmitted
 * \retval #IO_E_CAN_WRONG_HANDLE       invalid handle has been passed
 * \retval #IO_E_NULL_POINTER           null pointer has been passed
 * \retval #IO_E_INVALID_PARAMETER      an invalid message has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given handle has not been configured
 * \retval #IO_E_CAN_TIMEOUT            the CAN node reported a timeout
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_WriteMsg(ubyte2 handle,
                             const IO_CAN_DATA_FRAME * const data);

/**********************************************************************************************//**
 *
 * \brief Returns the error counters of the CAN channel
 *
 * \param   channel             CAN channel, one of:
 *                                  - \c #IO_CAN_CHANNEL_0
 *                                  - \c #IO_CAN_CHANNEL_1
 *                                  - \c #IO_CAN_CHANNEL_2
 *                                  - \c #IO_CAN_CHANNEL_3
 *                                  - \c #IO_CAN_CHANNEL_4
 *                                  - \c #IO_CAN_CHANNEL_5
 *                                  - \c #IO_CAN_CHANNEL_6
 * \param[out] rx_error_counter Value of the receive error counter
 * \param[out] tx_error_counter Value of the transmit error counter
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CAN_ERROR_WARNING      error counter has reached the warning limit,
 *                                      controller is still in active state
 * \retval #IO_E_CAN_ERROR_PASSIVE      controller is in error passive state
 * \retval #IO_E_CAN_BUS_OFF            controller is in bus off state
 * \retval #IO_E_INVALID_CHANNEL_ID     the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY          the given channel is not a CAN channel
 * \retval #IO_E_NULL_POINTER           null pointer has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given channel has not been initialized
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \attention
 *   If the CAN is in bus off, it will automatically recover after 128 occurrences of bus idle (128
 *   * 11 consecutive recessive bits). During this state messages can neither be received or
 *   transmitted.
 *
 * \note
 *   If the CAN is in error passive, the transmitter may still transmit CAN frames and passive
 *   error frames.
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_Status(ubyte1 channel,
                           ubyte1 * const rx_error_counter,
                           ubyte1 * const tx_error_counter);

/**********************************************************************************************//**
 *
 * \brief Returns the status of a message buffer object
 *
 * \param handle            CAN message object handle (retrieved from \c IO_CAN_ConfigMsg())
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_BUSY                   transmission is ongoing
 * \retval #IO_E_CAN_OVERFLOW           message object overflow
 * \retval #IO_E_CAN_OLD_DATA           no new data received
 * \retval #IO_E_CAN_WRONG_HANDLE       invalid handle has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given handle has not been configured
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_MsgStatus(ubyte2 handle);

/**********************************************************************************************//**
 *
 * \brief Configures a receive FIFO buffer for the given CAN channel.
 *
 *   Returns a FIFO buffer handle.
 *
 * \param[out] handle       Returns the FIFO buffer handle
 * \param channel           CAN channel, one of:
 *                              - \c #IO_CAN_CHANNEL_0
 *                              - \c #IO_CAN_CHANNEL_1
 *                              - \c #IO_CAN_CHANNEL_2
 *                              - \c #IO_CAN_CHANNEL_3
 *                              - \c #IO_CAN_CHANNEL_4
 *                              - \c #IO_CAN_CHANNEL_5
 *                              - \c #IO_CAN_CHANNEL_6
 * \param size              Size of the FIFO buffer:
 *                              - \c #IO_CAN_CHANNEL_0 .. \c #IO_CAN_CHANNEL_2: [2..63]
 *                              - \c #IO_CAN_CHANNEL_3 .. \c #IO_CAN_CHANNEL_6: [2..24]
 * \param id_format         Format of message identifier, one of:
 *                              - \c #IO_CAN_STD_FRAME,
 *                              - \c #IO_CAN_EXT_FRAME
 * \param id                CAN message identifier
 *                              - 0..0x7FF for \c #IO_CAN_STD_FRAME
 *                              - 0..0x1FFFFFFF for \c #IO_CAN_EXT_FRAME
 * \param ac_mask           CAN acceptance mask. Refer to \ref can_ac_mask_usage "Usage of the
 *                          acceptance mask" for further details.
 *                              - 0..0x7FF for \c #IO_CAN_STD_FRAME
 *                              - 0..0x1FFFFFFF for \c #IO_CAN_EXT_FRAME
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                      everything fine
 * \retval #IO_E_CAN_MAX_MO_REACHED      no more HW message objects are available
 * \retval #IO_E_CAN_MAX_HANDLES_REACHED no more handles are available
 * \retval #IO_E_NULL_POINTER            null Pointer has been passed
 * \retval #IO_E_INVALID_CHANNEL_ID      the given channel id does not exist
 * \retval #IO_E_CH_CAPABILITY           the given channel is not a CAN channel
 * \retval #IO_E_INVALID_PARAMETER       invalid parameter has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED  the given channel was not initialized
 * \retval #IO_E_INTERNAL_CSM            an internal error occurred
 * \retval #IO_E_UNKNOWN                 an unknown error occurred
 *
 **************************************************************************************************
 *
 * \note
 *   FIFO buffers are only available for receive direction.
 *
 * \remarks
 *   For the CAN channels \c #IO_CAN_CHANNEL_0, \c #IO_CAN_CHANNEL_1 and \c #IO_CAN_CHANNEL_2 the
 *   function has to reserve internally <b>\c size + 1</b> mailboxes for each configured FIFO.
 *   Therefore, on those channels not all <b>64</b> mailboxes are available for FIFO usage.
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_ConfigFIFO(ubyte2 * const handle,
                               ubyte1 channel,
                               ubyte1 size,
                               ubyte1 id_format,
                               ubyte4 id,
                               ubyte4 ac_mask);

/**********************************************************************************************//**
 *
 * \brief Reads the data from a FIFO buffer.
 *
 *   Copies all received CAN frames from a given FIFO buffer since the last
 *   call.
 *
 * \param handle            CAN FIFO buffer handle (retrieved from \c IO_CAN_ConfigFIFO())
 * \param[out] buffer       Pointer to data buffer structure. The received frames will be stored
 *                          there.
 * \param buffer_size       Size of \c buffer. The buffer has to be of the same length as the FIFO
 *                          was configured.
 * \param[out] rx_frames    Number of frames which have been received since the last call and
 *                          copied to \c buffer
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_NULL_POINTER           null pointer has been passed to function
 * \retval #IO_E_INVALID_PARAMETER      the buffer size does not match with the
 *                                      configuration
 * \retval #IO_E_CAN_FIFO_FULL          the FIFO is full, new messages were lost
 * \retval #IO_E_CAN_OLD_DATA           no new data received
 * \retval #IO_E_CAN_WRONG_HANDLE       invalid handle has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given handle has not been configured
 * \retval #IO_E_CAN_TIMEOUT            the CAN node reported a timeout
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 **************************************************************************************************
 *
 * \remarks
 *   The complete FIFO buffers has to be read with this function call. The
 *   provided buffer \c buffer has to be of the same size as the configured
 *   FIFO size.
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_ReadFIFO(ubyte2 handle,
                             IO_CAN_DATA_FRAME * const buffer,
                             ubyte1 buffer_size,
                             ubyte1 * const rx_frames);

/**********************************************************************************************//**
 *
 * \brief Returns the status of a FIFO buffer
 *
 * \param handle            CAN FIFO buffer handle (retrieved from \c IO_CAN_ConfigFIFO())
 *
 * \return #IO_ErrorType
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_CAN_FIFO_FULL          the FIFO is full, new messages were lost
 * \retval #IO_E_CAN_OLD_DATA           no new data received
 * \retval #IO_E_CAN_WRONG_HANDLE       invalid handle has been passed
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED the given handle has not been configured
 * \retval #IO_E_CAN_TIMEOUT            the CAN node reported a timeout
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
 * \retval #IO_E_UNKNOWN                an unknown error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_CAN_FIFOStatus(ubyte2 handle);

#ifdef __cplusplus
}
#endif

#endif /* IO_CAN_H_ */
