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

int main( int argc, char* argv[ ] )
{
    printf( "Student Name: Colby Bailey ( nib667 )\n" );
    
    FILE* f = NULL;
    if( argv[ 1 ] == NULL || !fopen( argv[ 1 ], "r+" ) )
    {
        fprintf( stderr, "ERROR: file does not exist or cannot be opened. Oops!\n" );
        return 1;
    }
    else
    {
        f = fopen( argv[ 1 ], "r+" );
        printf( "Input File Name: %s\n", argv[ 3 ] );
    }

    fclose( f );
    return 0;
}