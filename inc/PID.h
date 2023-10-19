#ifndef PID_H_
#define PID_H_
#endif

#include "ptypes_apdb.h"
#include "ptypes_tms570.h"

int PID(ubyte2 time, ubyte2 real, ubyte2 need, ubyte2 kp, ubyte2 ki, ubyte2 kd, ubyte2 previous_error, ubyte2 ut, ubyte2 ut_max);