# uart
Simple Linux C Uart library to help using serial communication on Linux platforms in a very simple way.

# Build
To build and install the library on your system, just use:
```console
foo@bar:~$ make clean;make; sudo make install
```
You can skip the make install part and copy the .a output file as well as the header file to your project to link it statically with your code
# Usage
Once the library is installed on your system or after adding the header file and the .a file to your project, you can use it like this.

First import the header files
```c
#include<uart.h>
```
if you are using c++, make sure you add extern "C" protection
```c
#ifdef __cplusplus
extern "C"
{
#endif
    // uart library
    #include <uart.h>
#ifdef __cplusplus
}
#endif
```
Now you need to declare your uart configuration object:
```c
// UART configuration
uart_cfg uart_configuration;
```

In your main, initialize the uart configuration with whatever you need. 
First call the init_uart_cfg_list function to initialize the structure with default values 

```c
// Initialize every thing to default
init_uart_cfg(&uart_configuration);
```
Now you can setup custom port name and baudrate.

```c
// Set port and baudrate
uart_configuration.port = "/dev/ttyUSB0";
uart_configuration.baudrate = 115200;
```

You can also configure some uart specific parameters if you want them to be different from default ones.

Here is the default configuration is you do nothing. It is built to communicate with most devices especially arduino devices.

```c
// Set custom configuration
// Number of bits per byte
uart->nbits_per_byte                = 8;
// parity check is off by default
uart->enable_parity_check           = false;
// we use a single stop bit by default
uart->enable_two_stop_bits          = false;
// by default hardware flow control is off
uart->enable_hw_flow_control        = false;
// by default software flow control is off
uart->enable_sw_flow_control        = false;
// by default echo is distabled
uart->enable_echo                   = false;
// by default erasure is distabled
uart->enable_erasure                = false;
// by default new line echo is distabled
uart->enable_newline_echo           = false;
// by default new special bytes handling is deactivated
uart->enable_bytes_special_handling = false;
// by default minimum number of bytes is 0
uart->V_MIN                         = 0;
// by default maximum time between bytes is 10 deciseconds (1s)
uart->V_TIME                        = 10;
```


We are quite there. Now we configure the uart and open it:

```c
    // Connecting UART 
    printf("Configuring uart %s ...", uart_configuration.port);
    if(configure_uart(&uart_configuration))
    {
        printf("OK\n"); //Success
    }
    else
    {
        printf("NOK\n");  // Failure
        exit(1);
    }
```

Now you can talk to your device.

To read and write you need to declare input and output buffers:
```c
    char read_buf [256];
    char write_buf [256];
```
And now you can read into the input buffer
```c
    int num_bytes = read(uart_configuration.fd, &read_buf, sizeof(read_buf));
    if(num_bytes==0)
    {
        continue;
    }
```

To send data to your device you can use:
```c
    write(uart_configuration.fd, write_buf, len);
```
here len is the length of the data to send. If write_buf contains caracters, you may use strlen(write_buf) instead.







