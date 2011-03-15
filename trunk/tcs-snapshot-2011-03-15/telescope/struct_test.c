#include <stdio.h>
#include <stdlib.h>

struct telescope_data {
    int data0;
    int data1;
    int count;
    int revolutions;
    int position;
    int data5;
    int data6;
    int data7;
};

int main( void ) {
    char data[ 6*8*4 ];
    int  * data_array[6];
    struct telescope_data * data_struct[6];

    int i;
    int j;

    printf( "Size of data = %d\n", sizeof( data ) );
    printf( "Size of telescope_data = %d\n", sizeof( struct telescope_data ) );
    printf( "Size of telescope_data * 6 = %d\n", sizeof( struct telescope_data ) * 6 );

    for( i = 0; i < 6; i ++ ) {
        data_array[i] = (int *) & data[i*32];
        data_struct[i] = (struct telescope_data *) & data[i*32];
        for( j = 0; j < 8; j ++ ) {
            data_array[i][j] = (i+1)*1000 +j;
            printf( "data_array[%d][%d] = %d\n", i, j, data_array[i][j] );
        }
        printf( "\n" );
    }

    for( i = 0; i < 6; i ++ ) {
        //data_struct[i] = (struct telescope_data *) & data[i*32];
        printf( "data_struct[%d] -> %p\n", i, data_struct[i] );
        printf( "data_array[%d]  -> %p\n", i, data_array[i] );
        printf( "data[%d*32]     -> %p\n", i, & data[i*32] );
        printf( "data_struct[%d]->data0       = %d\n", i, data_struct[i]->data0 );
        printf( "data_struct[%d]->data1       = %d\n", i, data_struct[i]->data1 );
        printf( "data_struct[%d]->count       = %d\n", i, data_struct[i]->count );
        printf( "data_struct[%d]->revolutions = %d\n", i, data_struct[i]->revolutions );
        printf( "data_struct[%d]->position    = %d\n", i, data_struct[i]->position );
        printf( "data_struct[%d]->data5       = %d\n", i, data_struct[i]->data5 );
        printf( "data_struct[%d]->data6       = %d\n", i, data_struct[i]->data6 );
        printf( "data_struct[%d]->data7       = %d\n", i, data_struct[i]->data7 );
        printf( "\n" );
    }

    return 0;
}