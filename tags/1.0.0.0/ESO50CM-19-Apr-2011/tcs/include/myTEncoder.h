#ifndef _MYTENCODER_H_
#define _MYTENCODER_H_

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <sys/socket.h>         //socket
#include <sys/un.h>             //ux-socket
#include <unistd.h>             //close
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>           //gettimeofday
#include <time.h>               //gmtime

#include <myBSemaphore.h>
//-------------------------------------------------------------
//  Instrument Shared memory and semaphore
//#define SHMKEY   0xFAFAFA00
#define WRSEMKEY 0xFAFAFA00
#define RDSEMKEY 0xFAFAFA02
//-------------------------------------------------------------
//  User Shared memory and semaphore
//#define USRSHMKEY   0xFAFAFA01
//#define USRSEMKEY   0xFAFAFA01

struct encoder_data_t {
    char i2c_address;
    char i2c_status;
    char message[16];
    char answer[16];
    int data[8];
};

struct my_tEncoder_data_t
{
    char   FirmwareName[16];               //VP
    char   FirmwareDate[16];               //VD
    char   FirmwareTime[16];               //VT
    char   FirmwareNumber[16];             //VN

    double EncoderToAxis_Reduction;
    double TicsPerRev;
    double HomePosition;

    double degs;                   //Degrees
    double degs_per_sec;           //Degrees per Seconds
};

class myTEncoder
{
    public:
        myTEncoder( char ax, char id, struct my_tEncoder_data_t * encoder );
        ~myTEncoder( void );

        void initializeEncoder( double encoder_to_axis_reduction,
                                double tics_per_revs,
                                double home_position );
        void setInstrumentMemorySpace( struct encoder_data_t * bin_encoder );

        double setEncoderToAxis_Reduction( double encoder_to_axis_reduction );
        double setTicsPerRev( double tics_per_revs );
        double setHomePosition( double home_position );

        double getEncoderToAxis_Reduction( void );
        double getTicsPerRev( void );
        double getHomePosition( void );

        double getPosition( void );
        double getDegPosition( void );
        int    setPosition( double degs );

        int    readDeviceMemory( char mem_address, int * value );
        int    readDeviceMemory( char mem_address, int * value, int m_verbose );
        int    setDeviceMemory( char mem_address, int * value );
        int    setDeviceMemory( char mem_address, int * value, int m_verbose );

    private:
        char m_ax;
        char m_id;
        struct my_tEncoder_data_t * m_encoder;
        struct encoder_data_t * m_bin_encoder;

        myBSemaphore * bin_write_semaphore;
        myBSemaphore * bin_read_semaphore;

};

#endif //_MYTENCODER_H_

