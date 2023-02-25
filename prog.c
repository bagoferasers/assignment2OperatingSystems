#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
void FIFO_Scheduling( struct PCB_st* Head, FILE* output );
void SJF_Scheduling( struct PCB_st* Head, FILE* output );
void PR_Scheduling( struct PCB_st* Head, FILE* output );
void RR_Scheduling( struct PCB_st* Head, FILE* output );

int main( int argc, char* argv[ ] )
{
    //int i = 0;
    //while( argv[ i++ ] != NULL )
        //printf("argv[%d] = %s\n", i, argv[ i ] );

    // open file
    FILE* f = NULL;
    FILE* output = NULL;
    if( argv[ 4 ] == NULL || !fopen( argv[ 4 ], "r+" ) )
    {
        fprintf( stderr, "ERROR: file does not exist or cannot be opened. Oops!\n" );
        return 1;
    }
    else
    {
        f = fopen( argv[ 4 ], "r+" );
        printf( "Input File Name: %s\n", argv[ 4 ] );
        printf( "\nStudent Name: Colby Bailey ( nib667 )\n" );
    }

    if( !fopen( "output1.txt", "w" ) )
    {
        fprintf( stderr, "ERROR: file does not exist or cannot be opened. Oops!\n" );
        return 1;
    }
    else
    {
        output = fopen( "output1.txt", "w" );
        fprintf( output, "\nStudent Name: Colby Bailey ( nib667 )\n" );
        fprintf( output, "Input File Name: %s\n", argv[ 4 ] );
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
    {
        printf( "CPU Scheduling Alg: %s\n\n", argv[ 2 ] );
        fprintf( output, "CPU Scheduling Alg: %s\n\n", argv[ 2 ] );
    }

    if( strcmp( argv[ 2 ], "FIFO" ) == 0 )
        FIFO_Scheduling( Head, output );
    else if( strcmp( argv[ 2 ], "SJF" ) == 0 )
        SJF_Scheduling( Head, output );
    else if( strcmp( argv[ 2 ], "PR" ) == 0 )
        PR_Scheduling( Head, output );

    //freeLinkedList( Head );
    fclose( output );
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

void FIFO_Scheduling( struct PCB_st* Head, FILE* output )
{
    struct PCB_st* removedOne;
    int count = 1;
    while( Head != NULL )
    {
        removedOne = Head;
        Head = Head->next;
        // context switching
        for( int i = 0; i < 8; i++ )
            CPUreg[ i ] = removedOne->myReg[ i ];
        for( int i = 0; i < 8; i++ )
            CPUreg[ i ]++;
        for( int i = 0; i < 8; i++ )
            removedOne->myReg[ i ] = CPUreg[ i ];
        // data collection for performance metrics
        removedOne->waitingTime = removedOne->waitingTime + CLOCK - removedOne->queueEnterClock;
        Total_waiting_time = Total_waiting_time + removedOne->waitingTime;
        CLOCK = CLOCK + removedOne->CPUburst;
        Total_turnaround_time = Total_turnaround_time + CLOCK;
        Total_job = Total_job + 1;
        free( removedOne );
        printf( "Process %d is completed at %d ms\n", count, CLOCK );
        fprintf( output, "Process %d is completed at %d ms\n", count++, CLOCK );
    }
    printf( "\nAverage Waiting time = %d ms\n", ( Total_waiting_time / Total_job ) );
    printf( "Average Turnaround time = %d ms\n", ( Total_turnaround_time / Total_job ) );
    printf( "Throughput = %.2lf jobs per ms\n\n", ( ( double )Total_job / CLOCK ) );
    fprintf( output, "\nAverage Waiting time = %d ms\n", ( Total_waiting_time / Total_job ) );
    fprintf( output, "Average Turnaround time = %d ms\n", ( Total_turnaround_time / Total_job ) );
    fprintf( output, "Throughput = %.2lf jobs per ms\n\n", ( ( double )Total_job / CLOCK ) );
}

void SJF_Scheduling( struct PCB_st* Head, FILE* output )
{
    if( Head == NULL )
        return;
    struct PCB_st* removedOne;
    struct PCB_st* previousOne;
    struct PCB_st* smallest;
    int count = 1;
    while( Head != NULL )
    {
        removedOne = Head;
        previousOne = NULL;
        smallest = removedOne;
        while( removedOne != NULL )
        {
            if( removedOne->CPUburst < smallest->CPUburst )
                smallest = removedOne;
            else
                previousOne = removedOne;
            if( removedOne == NULL )
                break;
            removedOne = removedOne->next;
        }
        if( smallest == Head )
            Head = Head->next;
        else
            previousOne->next = smallest->next;

        // context switching
        for( int i = 0; i < 8; i++ )
            CPUreg[ i ] = smallest->myReg[ i ];
        for( int i = 0; i < 8; i++ )
            CPUreg[ i ]++;
        for( int i = 0; i < 8; i++ )
            smallest->myReg[ i ] = CPUreg[ i ];
        // data collection for performance metrics
        smallest->waitingTime = smallest->waitingTime + CLOCK - smallest->queueEnterClock;
        Total_waiting_time = Total_waiting_time + smallest->waitingTime;
        CLOCK = CLOCK + smallest->CPUburst;
        Total_turnaround_time = Total_turnaround_time + CLOCK;
        Total_job = Total_job + 1;
        free( smallest );
        printf( "Process %d is completed at %d ms\n", count, CLOCK );
        fprintf( output, "Process %d is completed at %d ms\n", count++, CLOCK );
    }
    printf( "\nAverage Waiting time = %d ms\n", ( Total_waiting_time / Total_job ) );
    printf( "Average Turnaround time = %d ms\n", ( Total_turnaround_time / Total_job ) );
    printf( "Throughput = %.2lf jobs per ms\n\n", ( ( double )Total_job / CLOCK ) );
    fprintf( output, "\nAverage Waiting time = %d ms\n", ( Total_waiting_time / Total_job ) );
    fprintf( output, "Average Turnaround time = %d ms\n", ( Total_turnaround_time / Total_job ) );
    fprintf( output, "Throughput = %.2lf jobs per ms\n\n", ( ( double )Total_job / CLOCK ) );
}

void PR_Scheduling( struct PCB_st* Head, FILE* output )
{
    if( Head == NULL )
        return;
    struct PCB_st* removedOne;
    struct PCB_st* previousOne;
    struct PCB_st* largest;
    int count = 1;
    while( Head != NULL )
    {
        removedOne = Head;
        previousOne = NULL;
        largest = removedOne;
        while( removedOne != NULL )
        {
            if( removedOne->CPUburst > largest->CPUburst )
                largest = removedOne;
            else
                previousOne = removedOne;
            if( removedOne == NULL )
                break;
            removedOne = removedOne->next;
        }
        if( largest == Head )
            Head = Head->next;
        else
            previousOne->next = largest->next;

        // context switching
        for( int i = 0; i < 8; i++ )
            CPUreg[ i ] = largest->myReg[ i ];
        for( int i = 0; i < 8; i++ )
            CPUreg[ i ]++;
        for( int i = 0; i < 8; i++ )
            largest->myReg[ i ] = CPUreg[ i ];
        // data collection for performance metrics
        largest->waitingTime = largest->waitingTime + CLOCK - largest->queueEnterClock;
        Total_waiting_time = Total_waiting_time + largest->waitingTime;
        CLOCK = CLOCK + largest->CPUburst;
        Total_turnaround_time = Total_turnaround_time + CLOCK;
        Total_job = Total_job + 1;
        free( largest );
        printf( "Process %d is completed at %d ms\n", count, CLOCK );
        fprintf( output, "Process %d is completed at %d ms\n", count++, CLOCK );
    }
    printf( "\nAverage Waiting time = %d ms\n", ( Total_waiting_time / Total_job ) );
    printf( "Average Turnaround time = %d ms\n", ( Total_turnaround_time / Total_job ) );
    printf( "Throughput = %.2lf jobs per ms\n\n", ( ( double )Total_job / CLOCK ) );
    fprintf( output, "\nAverage Waiting time = %d ms\n", ( Total_waiting_time / Total_job ) );
    fprintf( output, "Average Turnaround time = %d ms\n", ( Total_turnaround_time / Total_job ) );
    fprintf( output, "Throughput = %.2lf jobs per ms\n\n", ( ( double )Total_job / CLOCK ) );
}

void RR_Scheduling( struct PCB_st* Head, FILE* output )
{
    //
}