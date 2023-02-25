#ifndef _prog_h
#define _prog_h

struct PCB_st
{
    int ProcId;
    int ProcPR;
    int CPUburst;
    int myReg[ 8 ];
    int queueEnterClock;
    int waitingTime;
    struct PCB_st* next;
} PCB_st;

#endif