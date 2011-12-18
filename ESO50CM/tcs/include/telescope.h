#ifndef TELESCOPE_H
#define TELESCOPE_H

#include <sys/types.h>

#include <termios.h>
//#include <unistd.h>

#include "LoggerHelper.h"

/** Symbols defined in semaphorec.  ************************************/
extern int binary_semaphore_allocate( key_t key, int sem_flags );
extern int binary_semaphore_deallocate( int sem_id );
extern int binary_semaphore_initialize( int sem_id );
extern int binary_semaphore_wait( int sem_id );
extern int binary_semaphore_post( int sem_id );

/** Symbols defined in common.c.  ************************************/
/** The name of this program.  */
extern const char * program_name;

/** If nonzero, print verbose messages.  */
extern int verbose;

/** Logger variable for telescope61 functions */
extern LoggerHelper logger;

/** Like malloc, except aborts the program if allocation fails.  */
extern void* xmalloc (size_t size);

/** Like realloc, except aborts the program if allocation fails.  */
extern void* xrealloc (void* ptr, size_t size); 

/** Like strdup, except aborts the program if allocation fails.  */
extern char* xstrdup (const char* s); 

/** Print an error message for a failed call OPERATION, using the value
    of errno, and end the program.  */
extern void system_error (const char* operation);

/** Print an error message for failure involving CAUSE, including
    a descriptive MESSAGE, and end the program.  */
extern void error (const char* cause, const char* message);

/** Return the directory containing the running programs executable.
    The return value is a memory buffer that the caller must deallocate
    using free.  This function calls abort on failure.  */
extern char* get_self_executable_directory ();

/** Symbols defined in module.c  **************************************/
/** An instance of a loaded server module.  */
struct server_module {
    /** The shared library handle corresponding to the loaded module.  */
    void* handle;

    /** A name describing the module.  */
    const char* name;

    /** The function that generates the HTML results for this module.  */
    void (* generate_function) (int);
};

/** The directory from which modules are loaded.  */
extern char* module_dir;

/** Attempt to load a server module with the name MODULE_PATH.  If a
    server module exists with this path, loads the module and returns a
    server_module structure representing it.  Otherwise, returns NULL.  */
extern struct server_module* module_open (const char* module_path);

/** Close a server module and deallocate the MODULE object.  */
extern void module_close (struct server_module* module);

/** Symbols defined in server.c.  ************************************/

/** Run the server on LOCAL_ADDRESS and PORT.  */
extern void telescope_run( const char * device, speed_t baudrate, const char * socket_name );

extern int open_RS232( const char * m_device, speed_t m_baudrate );
extern int close_RS232( int m_port );
extern int read_RS232( int m_port, char * str, int max );
extern int write_RS232( int m_port, const char * s, int length );
extern int flush_RS232( int m_port );
extern int status_RS232( int m_port, int m_microsecs );

#endif  /* TELESCOPE_H */
