#ifndef _MYTAXIS_H_
#define _MYTAXIS_H_

#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <cmath>

#include <sys/types.h>
#include <time.h>
#include <sys/time.h>           //gettimeofday
#include <errno.h>              //error number

#include <sys/shm.h>            //shared memory
#include <sys/stat.h>           //shared memory
#include "myClientSocket.h"

using namespace std;

struct encoder_data_t {
    char i2c_address;
    char i2c_status;
    int data[8];
};

//class myTAxis : public myClientSocket {
class myTAxis {
    public:
        myTAxis( const char * fileName, char id, 
                 struct encoder_data_t * motor,
                 struct encoder_data_t * worm,
                 struct encoder_data_t * axis  );
        //myTAxis( const char * fileName, char id, const char * socket_name );
        ~myTAxis();
        
        int readConfigFile( const char * configFileName  );
        
        int     setAxisDegPerSec( double deg_per_sec );
        int     incrementTrackingRate( void );
        int     decrementTrackingRate( void );
        int     setMaxSlewRate( int n );
        int     setMotorInputVoltage( int V );
        int     setAxisDir( int dir );
        
        int     setMotorEncoderHomePositionToCurrentPosition( void );
        double  setMotorEncoderHomePositionTo( double );
        
        double  getPosition( void );
        double  getPosition_MotorEncoderPrecision( void );
        double  getPosition_WormEncoderPrecision( void );
        double  getPosition_AxisEncoderPrecision( void );
        void    resetPosition( double degrees );
        void    printHomePosition( void );
        double  setTargetPosition_MotorEncoderPrecision( double deg );
        double  setTargetPosition_WormEncoderPrecision( double deg );
        double  setTargetPosition_AxisEncoderPrecision( double deg );
        
        int     getIntegerPosition( int precision );
        int     getMotorIntegerPosition( void );
        int     getWormIntegerPosition( void );
        int     getAxisIntegerPosition( void );
        
        int readDeviceMemory( unsigned char i2c_address, char mem_address );
        //int readWormMemory( char mem_address );
        //int readAxisMemory( char mem_address );
        //int readMotorMemory( char mem_address );
        int setDeviceMemory( unsigned char i2c_address, char mem_address, int data );
        //int setWormMemory( char mem_address, int data );
        //int setAxisMemory( char mem_address, int data );
        //int setMotorMemory( char mem_address, int data );

        int resetMotor( void );
        int stopMotor( void );
        int runMotor( void );
        int runMotorToPosition( double degs );
        int startTracking( int );
        int stopTracking( void );
        int stopMotorWithInputVoltage( void );
        int runMotorWithInputVoltage( void );
        
        int updatePosition( int resolution );
        int updatePosition( void );
        int updateMotorPosition( void );
        int updateWormPosition( void );
        int updateAxisPosition( void );
        
        char receiveData( char * buffer );
        char receiveStatus( char * buffer );
        
        int  set_fd( int fd );
        int  get_fd( void );

    private:
        struct encoder_data_t * m_motor;
        struct encoder_data_t * m_worm;
        struct encoder_data_t * m_axis;
        
        //char *          m_socket_name;                          //XS
        char            m_id;
        unsigned char   m_Motor_I2CAdress;                      //XM2
        double          m_Motor_Kp;                             //XMP
        double          m_Motor_Ki;                             //XMI
        double          m_Motor_Kd;                             //XMD
        double          m_MotorEncoderToAxis_Reduction;                //XMR
        double          m_MotorEncoder_TicsPerRev;                     //XMT
        double          m_MotorEncoder_HomePosition;                   //XWH
        
        int             m_Motor_InputVoltage;
        
        double          m_MotorEncoder_Position;
        double          m_MotorEncoder_TargetPosition;
        int             m_MotorEncoder_TicsPerSec;      
        int             m_Motor_AxisDir;
        int             m_MotorEncoder_Current_TicsPerSec;
            
        unsigned char   m_WormEncoder_I2CAdress;                //XW2
        double          m_WormEncoderToAxis_Reduction;          //XWR
        double          m_WormEncoder_TicsPerRev;               //XWT
        double          m_WormEncoder_HomePosition;             //XWH

        int             m_WormEncoder_Init;     
        double          m_WormEncoder_Position;
        double          m_WormEncoder_TargetPosition;
        double          m_WormEncoder_Current_TicsPerSec;

        unsigned char   m_AxisEncoder_I2CAdress;                //XA2
        double          m_AxisEncoderToAxis_Reduction;          //XAR
        double          m_AxisEncoder_TicsPerRev;               //XAT
        double          m_AxisEncoder_HomePosition;             //XAH
        
        double          m_AxisEncoder_Position;        
        double          m_AxisEncoder_TargetPosition;        
        double          m_AxisEncoder_Current_TicsPerSec;

        double          m_AxisSpeed_DegPerSec;
        double          m_AxisSpeed_Current_DegPerSec;

        int             m_MotorData[6];
        int             m_WormData[6];
        int             m_AxisData[6];

        char            m_MotorStatus;
        char            m_WormStatus;
        char            m_AxisStatus;
        
        int             m_fd;
};




#endif // * _MYTAXIS_H_ *
