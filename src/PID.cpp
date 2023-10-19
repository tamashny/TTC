#include "PID.h"

int PID(ubyte2 time, ubyte2 real, ubyte2 need, ubyte2 kp, ubyte2 ki, ubyte2 kd, ubyte2 previous_error, ubyte2 ut, ubyte2 ut_max)
{
    ubyte2 dt = time/1000000; // Время цикла в секундах
    ubyte2 error = need - real;
    ubyte2 P = error;
    ubyte2 I = I + (error)*dt;
    ubyte2 D = (error - previous_error)/dt;
    previous_error = need - real;
    
    if ((P*kp + I*ki + D*kd)>ut){
        ut_max = P*kp + I*ki + D*kd;
    }

    ubyte2 ut = P*kp + I*ki + D*kd; // Управляющая функция
    return (ut/ut_max);
}