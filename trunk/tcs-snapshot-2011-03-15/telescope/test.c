#include <stdio.h>

int main( void ) 
{
    int i;
    
    i = 0;
    do {
        printf( "[test loop] i = %d\n", i );
    } while ( ++i < 10 );
    printf( "[test     ] i = %d\n", i );
    i = 0;
    do {
        printf( "[test loop] i = %d\n", i );
    } while ( i++ < 10 );
    printf( "[test     ] i = %d\n", i );
    return 0;
}
