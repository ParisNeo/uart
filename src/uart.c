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
#include <linux/serial.h>
#include <linux/tty_flags.h>


/**
 * \fn void init_uart_cfg(uart_cfg * uart)
 * \brief Initializes the udp_cfg structure with default values (common values)
 *
 * \param uart      The struct containing the uart configuration
 * 
 * \return true if succeeded.
 */
void init_uart_cfg(uart_cfg * uart)
{
    // Use default configuration
    strcpy(uart->port, UART_DEFAULT_COM_PORT);
    uart->baudrate                      = UART_DEFAULT_BAUD_RATE;
    uart->nbits_per_byte                = 8;
    uart->enable_parity_check           = false;
    uart->enable_two_stop_bits          = false;
    uart->enable_hw_flow_control        = false;
    uart->enable_sw_flow_control        = false;
    uart->enable_canonical_mode         = false;
    uart->enable_echo                   = false;
    uart->enable_erasure                = false;
    uart->enable_newline_echo           = false;
    uart->enable_bytes_special_handling = false;
    uart->V_MIN                         = 0;
    uart->V_TIME                        = 10;

}

/**
 * \fn void init_uart_cfg_list(uart_cfg_list * uart_list)
 * \brief Initializes the uart_cfg_list structure with default values
 *
 * \param uart_list      The struct containing the uart list configuration
 * 
 * \return true if succeeded.
 */
void init_uart_cfg_list(uart_cfg_list * uart_list)
{
    uart_list->nb_cfg = 0;
    for(int i=0;i<UART_MAX_CFG;i++)
    {
        init_uart_cfg(&uart_list->uart[i]);
    }
}


/**
 * \fn bool configure_uart(uart_cfg * uart)
 * \brief Configures uart connection
 *
 * \param uart      The struct containing the uart configuration
 * 
 * \return true if succeeded.
 */
bool configure_uart(uart_cfg * uart)
{
	struct  termios2 tty;

    uart->fd = open(uart->port, O_RDWR | O_NOCTTY);//  | O_NDELAY);

    // Check for errors
    if (uart->fd < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return false;
    }

    
    bzero(&tty, sizeof(struct termios));
    /*
    if (tcgetattr(uart->fd, &tty)!=0)
    {
        return false;
    }*/
    ioctl(uart->fd, TCGETS2, &tty);

    if(uart->enable_parity_check) 
        tty.c_cflag |= PARENB; // Set parity bit, enabling parity (most common)
    else
        tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)

    if(uart->enable_two_stop_bits)
        tty.c_cflag |= CSTOPB; // Set stop field, two stop bits used in communication (most common)
    else
        tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)

    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size

    // Number of bits per byte
    switch (uart->nbits_per_byte)
    {
    case 8:
        tty.c_cflag |= CS8; // 8 bits per byte (most common)
        break;
    case 7:
        tty.c_cflag |= CS7; // 7 bits per byte (most common)
        break;
    case 6:
        tty.c_cflag |= CS6; // 6 bits per byte (most common)
        break;
    case 5:
        tty.c_cflag |= CS5; // 5 bits per byte (most common)
        break;
    
    default:
        tty.c_cflag |= CS8; // 8 bits per byte (most common)
        break;

    }

    if(uart->enable_hw_flow_control)
        tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    else
        tty.c_cflag |= CRTSCTS; // Disable RTS/CTS hardware flow control (most common)

    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    if(uart->enable_canonical_mode)
        tty.c_lflag |= ICANON; // use canonical mode
    else
        tty.c_lflag &= ~ICANON; // Don't use canonical mode

    if(uart->enable_echo)
        tty.c_lflag |= ECHO; // Enable echo
    else
        tty.c_lflag &= ~ECHO; // Disable echo

    if(uart->enable_erasure)
        tty.c_lflag |= ECHOE; // Enable erasure
    else
        tty.c_lflag &= ~ECHOE; // Disable erasure

    if(uart->enable_newline_echo)
        tty.c_lflag |= ECHONL; // Enable new-line echo
    else
        tty.c_lflag &= ~ECHONL; // Disable new-line echo

    if(uart->enable_sig_interpretation)
        tty.c_lflag |= ISIG; // Enable interpretation of INTR, QUIT and SUSP
    else
        tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    if(uart->enable_sw_flow_control)
        tty.c_iflag |= (IXON | IXOFF | IXANY); // Turn on s/w flow ctrl
    else
        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

    if(uart->enable_bytes_special_handling)
        tty.c_iflag |= (IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Enable special handling of received bytes
    else
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = uart->V_TIME;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = uart->V_MIN;

    // Now we set custom baudrate
    tty.c_cflag &= ~CBAUD;
    tty.c_cflag |= CBAUDEX; 
    
    tty.c_ispeed = uart->baudrate;// input baudrate
    tty.c_ospeed = uart->baudrate;// output baudrate


    tcflush(uart->fd, TCIFLUSH);
    if (ioctl(uart->fd, TCSETS2, &tty) == -1)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return false;
    }
    tcflush(uart->fd, TCIFLUSH);

    return true;    
}
/**
 * \fn uart_activate_low_latency(uart_cfg * uart)
 * \brief Activates low latency mode on linux systems. For example if you use an FTDI connection, this forces the timer to be reduced to 1ms instead of 16ms
 *
 * \param uart      The struct containing the uart configuration
 * 
 * \return nothing.
 */
void uart_activate_low_latency(uart_cfg * uart)
{
    struct serial_struct serial;    
    ioctl(uart->fd, TIOCGSERIAL, &serial);
    serial.flags |= ASYNC_LOW_LATENCY;
    ioctl(uart->fd, TIOCSSERIAL, &serial);
    sleep(1);
}

/**
 * \fn void uart_set_buffer_sizes(uart_cfg * uart, int buffer_size)
 * \brief sets the reception buffer size
 *
 * \param uart      The struct containing the uart configuration
 * 
 * \return nothing.
 */
void uart_set_buffer_sizes(uart_cfg * uart, int buffer_size)
{
    struct serial_struct serial;    
    ioctl(uart->fd, TIOCGSERIAL, &serial);
    serial.xmit_fifo_size = buffer_size;
    ioctl(uart->fd, TIOCSSERIAL, &serial);
    sleep(1);
}


/**
 * \fn void uart_send_string(uart_cfg * uart, char* str)
 * \brief sends a string to the uart driver
 *
 * \param uart     The struct containing the uart configuration
 * \param str      The string to be sent (standard c zero ending string)
 * 
 * \return nothing.
 */
void uart_send_string(uart_cfg * uart, char* str)
{
    write(uart->fd, str, strlen(str));
}

/**
 * \fn void uart_send_string(uart_cfg * uart, char* str)
 * \brief receives a string from the uart driver
 *
 * \param uart     The struct containing the uart configuration
 * \param str      A buffer where to store the string being read
 * 
 * \return the number of received bytes.
 */
int uart_receive_string(uart_cfg * uart, char* str, int buffer_size)
{
    int nb_read_bytes=0;
    while(nb_read_bytes<buffer_size)
    {
        int num_bytes = read(uart->fd, (&str)+nb_read_bytes, sizeof(buffer_size)-nb_read_bytes);
        nb_read_bytes += num_bytes;
        int nb_1 = nb_read_bytes-1;
        if(str[nb_1]=='\0' || str[nb_1]=='\n' || str[nb_1]=='\r')
        {
            break;
        }
    }
    return nb_read_bytes;
}