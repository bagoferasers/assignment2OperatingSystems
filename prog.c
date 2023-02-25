#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prog.h"

int CPUreg[ 8 ] = { 0 };
struct PCB_st* Head = NULL;
struct PCB_st* Tail = NULL;
int CLOCK = 0;
int Total_waiting_time = 0;
int Total_turnaround_time = 0;
int Total_job = 0;
int id, priority, burst;

void freeLinkedList( struct PCB_st* Head );
void FIFO_Scheduling( );

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

    if( argv[ 2 ] == NULL )
    {
        fprintf( stderr, "ERROR: could not find name of scheduling algorithm in command line input.\n" );
        return -1;
    }
    else    
        printf( "CPU Scheduling Alg: %s\n", argv[ 2 ] );

    if( strcmp( argv[ 2 ], "FIFO" ) == 0 )
    {
        //printf("oh yeah fifo nice\n");
        FIFO_Scheduling( Head );
    }

    freeLinkedList( Head );
    return 0;
}

void freeLinkedList( struct PCB_st* Head )
{
    struct PCB_st* imHere = Head;
    while( imHere != NULL )
    {
        struct PCB_st* imNextOne = imHere->next;
        free( imHere );
        imHere = imNextOne;
    }
}

void FIFO_Scheduling( struct PCB_st* Head )
{
    struct PCB_st* removedOne;
    while( Head != NULL )
    {
        removedOne = Head;
        Head->next = Head;
        for( int i = 0; i < 8; i++ )
            CPUreg[ i ] = removedOne->myReg[ i ];
        for( int i = 0; i < 8; i++ )
            CPUreg[ i ]++;
        for( int i = 0; i < 8; i++ )
            removedOne->myReg[ i ] = CPUreg[ i ];
    }
}