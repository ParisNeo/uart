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
```cpp
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
uart_configuration.nbits_per_byte                = 8;
// parity check is off by default
uart_configuration.enable_parity_check           = false;
// we use a single stop bit by default
uart_configuration.enable_two_stop_bits          = false;
// by default hardware flow control is off
uart_configuration.enable_hw_flow_control        = false;
// by default software flow control is off
uart_configuration.enable_sw_flow_control        = false;
// by default echo is distabled
uart_configuration.enable_echo                   = false;
// by default erasure is distabled
uart_configuration.enable_erasure                = false;
// by default new line echo is distabled
uart_configuration.enable_newline_echo           = false;
// by default new special bytes handling is deactivated
uart_configuration.enable_bytes_special_handling = false;
// by default minimum number of bytes is 0
uart_configuration.V_MIN                         = 0;
// by default maximum time between bytes is 10 deciseconds (1s)
uart_configuration.V_TIME                        = 10;
```

If you are using [FTDI](https://ftdichip.com/) component to communicate with uart, the chip uses a timer to pack the uart frames into USB frames. By default this timer is set to 16ms. Which means that you may not have a stable frame rate if you are running at higher frame rates (100fps for example). Fortunatuly, on linux, there is a way to reduce this timer interval to 1ms using ioctl commands. This library contains a function that allow enabling this mode directly.

```c
    // Activate low latency mode
    uart_activate_low_latency(&uart_configuration);
```

You can also change the size of the reception buffer:

```c
    // Set buffer size
    uart_set_buffer_sizes(&uart_configuration, 1024);
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

# Complete example
Let's write a complete application that can connect to an arduino and just show stuff that was sent by the board

```c
#include<stdio.h>
#include<uart.h>

// UART configuration
uart_cfg uart_configuration;

// Prepare buffers
char write_buf[256];
char read_buf[256];

int main(int argc, char ** argv)
{
    // Initialize every thing to default
    init_uart_cfg(&uart_configuration);

    // Set port and baudrate
    uart_configuration.port = "/dev/ttyUSB0";
    uart_configuration.baudrate = 115200;

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
    // Now we are ready to send and receive data
    // First we send something
    strcpy(write_buf, "Hello world");
    write(uart_configuration.fd, write_buf, strlen(write_buf));
    // Then we read something
    int num_bytes = read(uart_configuration.fd, &read_buf, sizeof(read_buf));
    if(num_bytes==0)
    {
        continue;
    }
    printf("received %s",read_buf);
    // That's it!
    // Now you can play with that, like putting the reception in a loop or another thread etc.
}
```

# Complete example
Here is a more complex code if you want more flexibility

```c
#include<stdio.h>
#include<uart.h>

// UART configuration
uart_cfg uart_configuration;

// Prepare buffers
char write_buf[256];
char read_buf[256];

int main(int argc, char ** argv)
{
    // Initialize every thing to default
    init_uart_cfg(&uart_configuration);

    // Set port and baudrate
    uart_configuration.port = "/dev/ttyUSB0";
    uart_configuration.baudrate = 115200;

    // Set other stuff
    // Number of bits per byte
    uart_configuration.nbits_per_byte                = 8;
    // parity check is off by default
    uart_configuration.enable_parity_check           = false;
    // we use a single stop bit by default
    uart_configuration.enable_two_stop_bits          = false;
    // by default hardware flow control is off
    uart_configuration.enable_hw_flow_control        = false;
    // by default software flow control is off
    uart_configuration.enable_sw_flow_control        = false;
    // by default echo is distabled
    uart_configuration.enable_echo                   = false;
    // by default erasure is distabled
    uart_configuration.enable_erasure                = false;
    // by default new line echo is distabled
    uart_configuration.enable_newline_echo           = false;
    // by default new special bytes handling is deactivated
    uart_configuration.enable_bytes_special_handling = false;
    // by default minimum number of bytes is 0
    uart_configuration.V_MIN                         = 0;
    // by default maximum time between bytes is 10 deciseconds (1s)
    uart_configuration.V_TIME                        = 10;
    

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
    // Now we are ready to send and receive data
    // First we send something
    strcpy(write_buf, "Hello world");
    write(uart_configuration.fd, write_buf, strlen(write_buf));
    // Then we read something
    int num_bytes = read(uart_configuration.fd, &read_buf, sizeof(read_buf));
    if(num_bytes==0)
    {
        continue;
    }
    printf("received %s",read_buf);
    // That's it!
    // Now you can play with that, like putting the reception in a loop or another thread etc.
}
```


# Information
This is one of multiple libraries I have developed to simplify some tasks we do with C on linux. The objective is for it to be open source, eazy to use and compatible with both c and cpp. The build system is make and is compatible with gcc building system. It is very eazy to use this with cmake or other build systems. All these libraries have been tested on raspberry pi with raspbian. They help starting a new application that requires configuration, communication with arduino tools and spreading information between multiple services on the raspberry pi.

The licence is MIT, so you can use this code in your projects without worrying about licence contamination that could happen when using GPL licences. So you still can use it for free in commercial applications.

Tests and bugfixes are welcome. Just clone it, repare it and send a pull request. I want to keep this code as clean and simple as possible so please avoid feature creaping.

# Useful links
Check out my [cfg library](https://github.com/ParisNeo/cfg) built in the same spirit as this library.
Check out my [udp library](https://github.com/ParisNeo/udp) built in the same spirit as this library.



