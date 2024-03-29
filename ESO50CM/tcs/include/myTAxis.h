#ifndef _MYTAXIS_H_
#define _MYTAXIS_H_

#include <myTEncoder.h>
#include <myTMotor.h>
#include "LoggerHelper.h"

struct my_TAxis_data_t
{
    struct my_tEncoder_data_t motorE;
    struct my_tEncoder_data_t wormE;
    struct my_tEncoder_data_t axisE;
};

class myTAxis {
    public:
        myTAxis( char id, struct my_TAxis_data_t * axis_data, LoggerHelper *logLCUImpl );
        ~myTAxis();

        void setInstrumentMemorySpace( struct encoder_data_t * bin_motor_e,
                                       struct encoder_data_t * bin_worm_e,
                                       struct encoder_data_t * bin_axis_e );

        double getPosition( void );
        int offsetAxisInDeg( double degs );

        myTEncoder * AxisE;
        myTEncoder * WormE;
        myTMotor   * Motor;

	LoggerHelper *logger;	
    private:
        char m_id;

};




#endif // * _MYTAXIS_H_ *
