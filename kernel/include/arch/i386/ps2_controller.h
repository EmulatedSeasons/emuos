#ifndef PS2_CONTROLLER_H
#define PS2_CONTROLLER_H

void initialize_ps2_controller();
int ps2_dev_send_command(int, unsigned char);
void inputbuffer_wait();
void outputbuffer_wait();
bool inputbuffer_check();
bool outputbuffer_check();

#endif