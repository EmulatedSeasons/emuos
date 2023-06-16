#include <io.h>
#include <stdint.h>
#include <stdio.h>
#include <arch/i386/ps2_controller.h>

#define PS2_DATA 0x60 // data port
#define PS2_STATUS 0x64 // status register
#define PS2_COMMAND 0x64 // command register

// set to true if 2nd ps2 port is available
bool ps2SecondaryEnabled = false;

void initialize_ps2_controller() {
    inputbuffer_wait();
    outb(PS2_COMMAND, 0xAD); // disable devices
    inputbuffer_wait();
    outb(PS2_COMMAND, 0xA7);
    
    outputbuffer_wait();
    inb(PS2_DATA); // flush output buffer
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);
    inb(PS2_DATA);

    
    inputbuffer_wait();
    outb(PS2_COMMAND, 0x20); // set controller configuration byte
    outputbuffer_wait();
    uint8_t conconfig = inb(PS2_DATA);
    conconfig &= 0b00110100;
    inputbuffer_wait();
    outb(PS2_COMMAND, 0x60);
    inputbuffer_wait();
    outb(PS2_DATA, conconfig);

    inputbuffer_wait();
    outb(PS2_COMMAND, 0xAA); // performs controller self test
    outputbuffer_wait();
    if (inb(PS2_DATA) == 0xFC) {
        printf("PS/2 controller failed self test\n");
    }
    inputbuffer_wait();
    outb(PS2_COMMAND, 0x60); // in case the controller resets from the test
    inputbuffer_wait();
    outb(PS2_DATA, conconfig);

    int portcount = 1;
    inputbuffer_wait();
    outb(PS2_COMMAND, 0xA8); // check if ps2 controller is dual channel
    inputbuffer_wait();
    outb(PS2_COMMAND, 0x20);
    outputbuffer_wait();
    conconfig = inb(PS2_DATA);
    if ((conconfig & 0b00100000) == 0b00000000) { // checks if bit 5 is cleared
        ps2SecondaryEnabled = true;
        //printf("Dual channel PS/2\n");
        portcount = 2;
    } else {
        //printf("Single channel PS/2\n");
    }
    
    inputbuffer_wait();
    outb(PS2_COMMAND, 0xAB); // tests ps2 ports
    outputbuffer_wait();
    if (inb(PS2_DATA) != 0x00) {
        printf("PS/2 port 1 failed self test\n");
        --portcount;
    }
    if (ps2SecondaryEnabled) {
        inputbuffer_wait();
        outb(PS2_COMMAND, 0xA9);
        outputbuffer_wait();
        if (inb(PS2_DATA) != 0x00) {
            printf("PS/2 port 2 failed self test\n");
            --portcount;
        }
    }
    if (!portcount) {
        printf("PS/2 initialization failed\n");
        return;
    }

    conconfig |= 0b00000011;
    inputbuffer_wait();
    outb(PS2_COMMAND, 0xAE); // enable ps2 ports
    conconfig |= 0b00000001;
    if (ps2SecondaryEnabled) {
        inputbuffer_wait();
        outb(PS2_COMMAND, 0xA8);
        outb(PS2_COMMAND, 0x60);
        conconfig |= 0b00000011;
        outb(PS2_DATA, conconfig);
    } else {
        inputbuffer_wait();
        outb(PS2_COMMAND, 0x60);
        conconfig |= 0b00000001;
        outb(PS2_DATA, conconfig);
    }

    //ps2_dev_send_command(0, 0xFF); // reset devices
    //inb(PS2_DATA);
    //io_wait();
    if (ps2SecondaryEnabled) {
        //ps2_dev_send_command(1, 0xFF);
        //inb(PS2_DATA);
        //io_wait();
    }

    //printf("PS/2 controller initialized\n");
}

// waits for input buffer (write) to be filled
void inputbuffer_wait() {
    for (int i = 0; i < 15; i++) {
        uint8_t status_reg = inb(PS2_STATUS);
        if ((status_reg & 0b00000010)) {
            return;
        }
        io_wait();
    }
}

// waits for outbut buffer (read) to be filled
void outputbuffer_wait() {
    for (int i = 0; i < 15; i++) {
        uint8_t status_reg = inb(PS2_STATUS);
        if ((status_reg & 0b00000001)) {
            return;
        }
        io_wait();
    }
}

// checks if inpbut buffer is set
bool inputbuffer_check() {
    uint8_t status_reg = inb(PS2_STATUS);
    if ((status_reg & 0b00000010)) {
        return true;
    } else {
        return false;
    }
}

bool outputbuffer_check() {
    uint8_t status_reg = inb(PS2_STATUS);
    if ((status_reg & 0b00000001)) {
        return true;
    } else {
        return false;
    }
}

/* Sends a command byte to a ps2 device 
Port numbers start from 0 */
int ps2_dev_send_command(int port, unsigned char command) {
    //bool commandRecieved = false;
    switch (port)
        {
        case 0:
            for (int i = 0; i < 5; i++) {
                uint8_t status_reg = inb(PS2_STATUS);
                if ((status_reg & 0b00000010) == 0b00000000) {
                    outb(PS2_DATA, command);
                    return 0;
                }
                inb(PS2_DATA);
                io_wait();
            }
            return 1; // error returned if timeout expires
        case 1:
            outb(PS2_COMMAND, 0xD4);
            for (int i = 0; i < 5; i++) {
                uint8_t status_reg = inb(PS2_STATUS);
                if ((status_reg & 0b00000010) == 0b00000000) {
                    outb(PS2_DATA, command);
                    return 0;
                }
                inb(PS2_DATA);
                io_wait();
            }
            return 1;
        default:
            return 2; // out of port range
        }
}