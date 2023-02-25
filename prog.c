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
int id, priority, burst;

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
    struct PCB_st* p;
    while( ( fscanf( f, "%d %d %d", &id, &priority, &burst ) ) != EOF )
    {
        p = ( struct PCB_st* )malloc( sizeof( struct PCB_st ) );
        if( !p )
        {
            free( p );
            fprintf( stderr, "ERROR: could not allocate memory for struct.\n" );
            return -1;
        }
        p->ProcId = id;
        p->ProcPR = priority;
        p->CPUburst = burst;
        for( int k = 0; k < 8; k++ )
            p->myReg[ k ] = id;
        p->queueEnterClock = 0;
        p->waitingTime = 0;
        if( Head == NULL )
        {
            Head = p;
            Tail = p;
        }
        else
        {
            Tail->next = p;
            Tail = p;
        }
    }

    fclose( f );
    return 0;
}