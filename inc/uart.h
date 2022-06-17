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

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
//#include <asm/termios.h>
#include <termios.h>
//#include <sys/ioctl.h>
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
	int     baudrate;
	int		V_MIN;
	int		V_TIME;

	int     fd;
} uart_cfg;

typedef struct{
	int     	nb_cfg;
	uart_cfg    cfg[UART_MAX_CFG];
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
 * \fn bool configure_uart(uart_cfg * cfg)
 * \brief Configures uart connection
 *
 * \param udp      The struct containing the uart configuration
 * 
 * \return true if succeeded.
 */
bool configure_uart(uart_cfg * cfg);

#endif
