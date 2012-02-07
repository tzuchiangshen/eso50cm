#include <stdio.h>
#include <math.h>
#include <string.h>

int main( void ) 
{
	int fd_stdin;
	int fd_stdout;
	char buffer[128];
	int hrs, min, sec;
	double degs;
	int bytes_read;

	fd_stdin = fileno( stdin );
	fd_stdout = fileno( stdout );

	if( (bytes_read = read( fd_stdin, buffer, 128 )) > 0 ) {
		printf( "[%d]\n", bytes_read );
		sscanf( buffer, "%d:%d:%d", & hrs, & min, & sec );
		degs = fabs( (double) hrs ) + ((double) min)/60. + ((double) sec)/3600.;
		degs *= 15.0;
		sprintf( buffer, "%+11.6lf\n", degs );
		write( fd_stdout, buffer, strlen( buffer ) );						
	} else {
		printf( "read ERROR\n" );
	}
	
	return 0;
}



