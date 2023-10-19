/**************************************************************************************************
 * Copyright (c) 2021 TTControl. All rights reserved. Confidential, proprietary.
 * Schoenbrunnerstrasse 7, A-1040 Wien, Austria. office@ttcontrol.com
 *************************************************************************************************/
/**********************************************************************************************//**
 * \file IO_UDP.h
 *
 * \latexonly \hypertarget{IO_UDP}{} \endlatexonly
 *
 * \brief IO Driver functions for UDP communication
 *
 * \section udp_limitations Limitations
 *
 * This preliminary implementation contains a very limited handling of the ARP packets:
 * * No ARP requests are sent.
 * * No filtering on received ARP packets - all ARP packets are responded to.
 *
 *************************************************************************************************/

#ifndef IO_UDP_H_
#define IO_UDP_H_

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 *
 * I N C L U D E S
 *
 *************************************************************************************************/

#include "IO_Error.h"

/**************************************************************************************************
 *
 * D E F I N I T I O N S
 *
 *************************************************************************************************/

/**
 * \name Data transfer direction
 * \anchor upd_socket_direction
 * @{
 */
#define IO_UDP_READ     1U   /**< used to set up a socket for receiving */
#define IO_UDP_WRITE    2U   /**< used to set up a socket for transmitting */
/** \cond */
#define IO_UDP_NODIR    0U   /**< reset value for socket direction - not part of the API! */
/** \endcond */
/** @} */
 
/**
 * \name Ethernet speed
 * \anchor upd_speed
 * @{
 */
/** \cond */
#define IO_ETH_SPEED_UNKNOWN     0u   /* not part of the API! */
/** \endcond */
#define IO_ETH_SPEED_10_MB       1u   /**< 10 Mbit/s */
#define IO_ETH_SPEED_100_MB      2u   /**< 100 Mbit/s */
/** @} */

/**************************************************************************************************
 *
 * F U N C T I O N S
 *
 *************************************************************************************************/

/**********************************************************************************************//**
 *
 * \brief Initialize the Ethernet interface and the UDP server.
 *
 * \param baudrate            baudrate parameter, one of
 *                            - \c #IO_ETH_SPEED_10_MB
 *                            - \c #IO_ETH_SPEED_100_MB
 * \param local_ip            ASCIZ string of local IP address (ipv4), format "www.xxx.yyy.zzz"
 * \param pc_ip               ASCIZ string of host (PC) IP address (ipv4), format "www.xxx.yyy.zzz"
 *
 * \return #IO_ErrorType
 *
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_INVALID_CHANNEL_ID     the ECU variant doesn't support this feature
 * \retval #IO_E_CHANNEL_BUSY           the channel has already been initialized
 * \retval #IO_E_ETH_INIT_TIMEOUT       a timeout error occurred during Ethernet initialization
 * \retval #IO_E_ETH_INIT_FAIL          an error during Ethernet initialization occurred
 * \retval #IO_E_ETH_MAC_INVALID        the configured MAC address is invalid
 * \retval #IO_E_ETH_MDIO_TIMEOUT       a timeout error occurred when performing an MDIO transfer
 * \retval #IO_E_ETH_MDIO_READ          an MDIO read transfer error occurred
 * \retval #IO_E_INTERNAL_CSM           an internal error occurred
 *
 *************************************************************************************************/
IO_ErrorType IO_UDP_Init(ubyte2 baudrate, const ubyte1 *local_ip, const ubyte1  *pc_ip);


/**********************************************************************************************//**
 *
 * \brief Deinitialize the Ethernet interface and free all sockets.
 *
 * \return #IO_ErrorType
 *
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED     interface was not configured
 * \retval #IO_E_INTERNAL_CSM               internal CSM error
 * \retval #IO_E_ETH_DEINIT_TIMEOUT         timeout on de-initialization of the Ethernet interface
 *
 *************************************************************************************************/
IO_ErrorType IO_UDP_DeInit(void);


/**********************************************************************************************//**
 *
 * \brief Create a socket for transmission or reception.
 *
 * \param ubReadWrite         direction of transfer, one of
 *                            - \c #IO_UDP_READ socket for receive
 *                            - \c #IO_UDP_WRITE socket for transmit
 * \param port                local (source) port
 * \param IPAddr              ASCIZ string of local IP address (ipv4)
 * \param[out] socket_id      handle of socket created
 *
 * \return #IO_ErrorType
 
 * \retval #IO_E_OK                         everything fine
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED     interface was not configured
 * \retval #IO_E_ETH_MDIO_READ              an MDIO read transfer error occurred
 * \retval #IO_E_ETH_MDIO_TIMEOUT           a timeout error occurred when performing an MDIO transfer
 * \retval #IO_E_BUSY                       auto-negotiation is not finished
 * \retval #IO_E_ETH_NO_LINK                no valid link has been established on the Ethernet interface
 * \retval #IO_E_UDP_NOMORESOCKETS          no more UDP sockets are available
 * \retval #IO_E_SOCKET_NOT_INITIALIZED     socket initialization failed due to illegal IP address
 *
 *************************************************************************************************/
IO_ErrorType IO_UDP_CreateSocket(ubyte1 ubReadWrite, ubyte2 port,
                                 const ubyte1 *IPAddr, ubyte2 *socket_id);

/**********************************************************************************************//**
 *
 * \brief Free socket.
 *
 * \param socket_id           handle of socket to be freed
 *
 * \return #IO_ErrorType
 *
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_UDP_WRONG_HANDLE       socket handle invalid
 * \retval #IO_E_SOCKET_NOT_INITIALIZED socket not initialized
 *
 *************************************************************************************************/
IO_ErrorType IO_UDP_FreeSocket(ubyte2 socket_id);

/**********************************************************************************************//**
 *
 * \brief Read data from the socket and process ARP requests.
 *
 * \note
 * This function needs to be called periodically in order to handle the received ARP requests:
 * When an ARP requests is received, the function automatically sends the response.
 * In this case, #IO_E_UDP_ARP_RECEIVED is returned.
 *
 * \param  socket_id          handle of socket to be used for reception
 * \param[out]  port          source port of received message
 * \param  buf                buffer for received data
 * \param[out]  len           number of bytes actually received
 * \param maxlen              maximum size of receive buffer
 *
 * \return #IO_ErrorType
 *
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_UDP_WRONG_HANDLE       invalid handle has been passed
 * \retval #IO_E_NULL_POINTER           null pointer was passed
 * \retval #IO_E_UDP_INVALID_BUFFER     buffer invalid: null pointer or len = 0
 * \retval #IO_E_ETH_NO_LINK            no ethernet link available
 * \retval #IO_E_BUSY                   link busy (possibly download in progress)
 * \retval #IO_E_CHANNEL_NOT_CONFIGURED interface was not configured
 * \retval #IO_E_ETH_READ_FAIL          error in a read operation on the Ethernet interface
 * \retval #IO_E_ETH_WRITE_FAIL         error in a write operation on the Ethernet interface
 * \retval #IO_E_UDP_ARP_RECEIVED       Address Resolution Package received
 * \retval #IO_E_UDP_OVERFLOW           received data length greater than maximum buffer size
 * \retval #IO_E_WRONG_ADDRESS          IP address of received message does not match
 * \retval #IO_E_UDP_WRONG_PORT         port number of received message does not match
 *
 *************************************************************************************************/
IO_ErrorType IO_UDP_Read(ubyte2 socket_id, ubyte2 *port,  ubyte1 *buf, ubyte2 *len, ubyte2 maxlen);

/**********************************************************************************************//**
 *
 * \brief Send data to the receiver defined by target_addr and port.
 *
 * \note
 * The transmitted MAC address will be set to the remote MAC address in the last received packet.
 *
 * \param socket_id           handle of socket to be used for transmit
 * \param buf                 buffer containing the data to be sent
 * \param len                 number of bytes to be sent
 * \param port                target port for transmitted message
 * \param target_ip           ASCIZ string of target IP address (ipv4)
 *
 * \return #IO_ErrorType
 * 
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_UDP_OVERFLOW           packet too long
 * \retval #IO_E_UDP_WRONG_HANDLE       invalid handle has been passed
 * \retval #IO_E_NULL_POINTER           null pointer has been passed
 * \retval #IO_E_WRONG_ADDRESS          illegal target IP address
 * \retval #IO_E_INVALID_PARAMETER      invalid header parameter
 * \retval #IO_E_BUSY                   link busy
 * \retval #IO_E_ETH_NO_LINK            no ethernet link available
 * \retval #IO_E_ETH_WRITE_FAIL         error in a write operation on the Ethernet interface
 *
 *************************************************************************************************/
IO_ErrorType IO_UDP_WriteTo(ubyte2 socket_id, const ubyte1 *  buf, ubyte2 len, ubyte2 port, const ubyte1 *target_ip);

/**********************************************************************************************//**
 *
 * \brief Get status info for specified socket.
 *
 * \param  socket_id          handle of the socket
 * \param[out] socket_status  status of socket, one of:
 *                            - \c #IO_E_SOCKET_NOT_INITIALIZED socket not used
 *                            - \c #IO_E_OK   socket initialized
 * \return #IO_ErrorType 
 *
 * \retval #IO_E_OK                     everything fine
 * \retval #IO_E_NULL_POINTER           null pointer was passed
 * \retval #IO_E_UDP_WRONG_HANDLE       invalid handle has been passed
 *
 *************************************************************************************************/
IO_ErrorType IO_UDP_GetStatus(ubyte2 socket_id, ubyte2 *socket_status);

#ifdef __cplusplus
}
#endif

#endif /* IO_UDP_H_ */
