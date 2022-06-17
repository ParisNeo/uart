/**
 * \file        udp.c
 * \brief       A c library to manage udp 
 * \author      Saifeddine ALOUI
 * \version     0.1
 * \date        12 juin 2022
 * 
 *	Provides helper tools to build udp connection .
 *
 */
#include <uart.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

void show_tty_cfg_mem(struct termios * tty)
{
    printf("----\n");
    for (int i =0;i<sizeof(struct termios);i++)
    {
        printf("%2x",((char*)tty)[i]);
    }
    printf("----\n");
}

/**
 * \fn void init_uart_cfg(uart_cfg * uart)
 * \brief Initializes the udp_cfg structure
 *
 * \param udp      The struct containing the udp configuration
 * 
 * \return true if succeeded.
 */
void init_uart_cfg(uart_cfg * uart)
{
    strcpy(uart->port, UART_DEFAULT_COM_PORT);
    uart->baudrate=UART_DEFAULT_BAUD_RATE;
    uart->V_MIN=0;
    uart->V_TIME=10;

}

/**
 * \fn void init_uart_cfg_list(uart_cfg_list * uart_list)
 * \brief Initializes the uart_cfg_list structure with default values
 *
 * \param udp_list      The struct containing the udp list configuration
 * 
 * \return true if succeeded.
 */
void init_uart_cfg_list(uart_cfg_list * uart_list)
{
    uart_list->nb_cfg = 0;
    memset(uart_list->cfg,0, sizeof(uart_list->cfg));
}


/**
 * \fn bool configure_uart(uart_cfg * cfg)
 * \brief Configures uart connection
 *
 * \param udp      The struct containing the uart configuration
 * 
 * \return true if succeeded.
 */
bool configure_uart(uart_cfg * cfg)
{
	struct  termios tty;

    cfg->fd = open(cfg->port, O_RDWR);// | O_NOCTTY | O_NDELAY);

    // Check for errors
    if (cfg->fd < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return false;
    }

    
    //bzero(&tty, sizeof(struct termios));
    if (tcgetattr(cfg->fd, &tty)!=0)
    {
        return false;
    }


    
    tty.c_cflag &= ~(CSTOPB);
    tty.c_cflag &= ~(CRTSCTS);

    tty.c_cflag |= CREAD | CLOCAL; //8n1 see termios.h CS8 |

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // No software flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable especial handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent interpretation of output bytes
    tty.c_oflag &= ~ONLCR; // Prevent conversion of new line to carriege

    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 10;

    cfsetispeed(&tty, cfg->baudrate);// B115200);
    cfsetospeed(&tty, cfg->baudrate);// B115200);
    //cfsetispeed(&tty, B115200);
    //cfsetospeed(&tty, B115200);

    // Now let's configure the port
    // Save tty settings, also checking for error
    if (tcsetattr(cfg->fd, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return false;
    }
    

    return true;    
}