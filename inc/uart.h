/**
 * \file        uart.h
 * \brief       Tools to use a tty connection
 * \author      Saifeddine ALOUI
 * \version     0.1
 * \date        12 juin 2022
 *
 * This library.
 *
 */
#ifndef __UART__
#define __UART__
#include <fcntl.h>
#include <time.h>

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
//#include <asm/termios.h>
//#include <termios.h>
//#include <sys/ioctl.h>

#include <asm/ioctls.h>
#include <asm/termbits.h>

#include <sys/types.h> 
#include <stdbool.h>
#include <unistd.h> 
#include <string.h> 

// Linux headers
#include <fcntl.h>      // Contains file controls like O_RDWR
#include <errno.h>      // Error integer and strerror() function
#include <unistd.h>     // write(), read(), close()

// Defines -----------------------------------------------

#define UART_MAXPORTLEN 			128
#define UART_MAXBAUDRATELEN			16
#define UART_MAX_CFG				3

#define UART_DEFAULT_COM_PORT 	"/dev/ttyACM0"
#define UART_DEFAULT_BAUD_RATE	115200
// Structs -----------------------------------------------



typedef struct{
	char    port[UART_MAXPORTLEN];

	bool	open_prams_sync;
	bool	open_prams_noblock;
	bool	open_prams_noctty;
	bool	open_prams_nodelay;

	int		nbits_per_byte;
	bool	enable_parity_check;
	bool	enable_two_stop_bits;
	bool	enable_hw_flow_control;

	bool	enable_read;
	bool	enable_ignore_ctrl_lines;

	bool	enable_canonical_mode;
	bool 	enable_echo;
	bool	enable_erasure;
	bool	enable_newline_echo;
	bool	enable_sig_interpretation;
	bool 	enable_sw_flow_control;
	bool	enable_bytes_special_handling;

	int     baudrate;
	int		V_MIN;
	int		V_TIME;


	int     fd;
} uart_cfg;

typedef struct{
	int     	nb_cfg;
	uart_cfg    uart[UART_MAX_CFG];
} uart_cfg_list;

// Prototypes --------------------------------------------

/**
 * \fn void init_uart_cfg(uart_cfg * uart)
 * \brief Initializes the udp_cfg structure
 *
 * \param udp      The struct containing the udp configuration
 * 
 * \return true if succeeded.
 */
void init_uart_cfg(uart_cfg * uart);

/**
 * \fn void init_uart_cfg_list(uart_cfg_list * uart_list)
 * \brief Initializes the uart_cfg_list structure with default values
 *
 * \param uart_list      The struct containing the uart list configuration
 * 
 * \return true if succeeded.
 */
void init_uart_cfg_list(uart_cfg_list * uart_list);

/**
 * \fn bool configure_uart(uart_cfg * uart)
 * \brief Configures uart connection
 *
 * \param uart      The struct containing the uart configuration
 * 
 * \return true if succeeded.
 */
bool configure_uart(uart_cfg * uart);

/**
 * \fn uart_activate_low_latency(uart_cfg * uart)
 * \brief Activates low latency mode on linux systems. For example if you use an FTDI connection, this forces the timer to be reduced to 1ms instead of 16ms
 *
 * \param uart      The struct containing the uart configuration
 * 
 * \return nothing.
 */
void uart_activate_low_latency(uart_cfg * uart);

/**
 * \fn void uart_set_buffer_sizes(uart_cfg * uart, int buffer_size)
 * \brief sets the reception buffer size
 *
 * \param uart      The struct containing the uart configuration
 * 
 * \return nothing.
 */
void uart_set_buffer_sizes(uart_cfg * uart, int buffer_size);

/**
 * \fn void uart_send_string(uart_cfg * uart, char* str)
 * \brief sends a string to the uart driver
 *
 * \param uart     The struct containing the uart configuration
 * \param str      The string to be sent (standard c zero ending string)
 * 
 * \return nothing.
 */
void uart_send_string(uart_cfg * uart, char* str);

/**
 * \fn void uart_send_string(uart_cfg * uart, char* str)
 * \brief receives a string from the uart driver
 *
 * \param uart     The struct containing the uart configuration
 * \param str      A buffer where to store the string being read
 * 
 * \return the number of received bytes.
 */
int uart_receive_string(uart_cfg * uart, char* str, int buffer_size);

#endif
