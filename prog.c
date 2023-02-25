#include <stdio.h>
#include <stdlib.h>
#include "prog.h"

int CPUreg[ 8 ] = { 0 };
struct PCB_st* Head = NULL;
struct PCB_st* Tail = NULL;
int CLOCK = 0;
int Total_waiting_time = 0;
int Total_turnaround_time = 0;
int Total_job = 0;
int id, pr, burst;

int main( int argc, char* argv[ ] )
{
    printf( "Student Name: Colby Bailey ( nib667 )\n" );
    
    //int i = 0;
    //while( argv[ i++ ] != NULL )
        //printf("argv[%d] = %s\n", i, argv[ i ] );

    // open file
    FILE* f = NULL;
    if( argv[ 4 ] == NULL || !fopen( argv[ 4 ], "r+" ) )
    {
        fprintf( stderr, "ERROR: file does not exist or cannot be opened. Oops!\n" );
        return 1;
    }
    else
    {
        f = fopen( argv[ 4 ], "r+" );
        printf( "Input File Name: %s\n", argv[ 4 ] );
    }

    // scan lines
    while( ( fscanf( f, "%d %d %d", &id, &pr, &burst ) ) != EOF )
    {
        //
    }

    fclose( f );
    return 0;
}