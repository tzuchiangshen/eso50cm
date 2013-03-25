#ifndef _SHM_DBG_H_
#define _SHM_DBG_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>

#define TELSHMKEY   0xFAFAFA00
#define TELSEMKEY   0xFAFAFA00

struct encoder_data_t {
    char i2c_address;
    char i2c_status;
    char message[16];
    char answer[16];
    int data[8];
};

struct telescope_data_t {
    struct timeval  gtime;
    struct timezone tzone;

    struct encoder_data_t encoder[6];

    char last[128];
    char msg[128];
    char new_data;
    char quit;
};

#endif // * _SHM_DBG_H_ *
