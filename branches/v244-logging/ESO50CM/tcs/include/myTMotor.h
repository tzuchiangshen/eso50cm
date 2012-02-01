#ifndef _MYTMOTOR_H_
#define _MYTMOTOR_H_

#include <myTEncoder.h>

class myTMotor : public myTEncoder
{
    public:
        myTMotor( char ax, char id, struct my_tEncoder_data_t * encoder );
		void setSimulationMode( bool mode);
		bool isSimulated( void );
        ~myTMotor( void );

        int setSpeed( double degs_per_sec  );
        int runEncSteps( int enc_steps );

    private:
	    bool m_simulated;

};

#endif //_MYTMOTOR_H_

