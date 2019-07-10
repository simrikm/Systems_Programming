#ifndef TIMER_H
#define TIMER_H

int Timer_start();
int Timer_elapsedWallclockTime(double *wc);
int Timer_elapsedUserTime(double *ut);
int Timer_elapsedSystemTime(double *sys);
int Timer_elapsedTime(double *ec, double *us, double *sys);


#endif
