#ifndef _MYI2CDevice_H_
#define _MYI2CDevice_H_

class myI2CDevice
{
    public:
        myLCU( void );
        ~myLCU( void );

        int waitSemaphore( void );
        int postSemaphore( void );

        void createTelescope( void );
        void readConfigurationFile( void );
        void configureTelescope( void );

        int run( void );

        struct tm * getLCU_Time( void );
        void * get_lcu_data_address( void );

        myTelescope * telescope;
    private:
        struct my_lcu_data_t *  m_lcu_data;
        myBSemaphore *          m_lcu_semaphore;

        int                     m_quit;
        int                     m_semaphore_id;
        int                     m_segment_id;
        char *                  m_shared_memory;
        int                     m_segment_size;
        int                     m_shared_segment_size;

        struct shmid_ds         m_shmbuffer;
        struct tm               m_LCU_Time;

};

#endif //_MYI2CDevice_H_
