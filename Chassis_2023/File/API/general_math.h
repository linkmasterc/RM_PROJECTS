#ifndef __GENERAL_MATH__
#define __GENERAL_MATH__

#include "rm_pid_types.h"

#define REAL   float
#define TAN_MAP_RES      0.003921569f     /* (smallest non-zero value in table) */
#define TAN_MAP_SIZE     256

#define my_2_norm(x,y) (my_sqrt(my_pow((x)) + my_pow((y))))
#define my_pow(a) ((a)*(a))
#define ABS(x) ( (x)>0?(x):-(x) )
#define LIMIT( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )
#define LIMIT_ZERO( x,abs ) ( (((x) >= (-abs)) &((x) <= (abs)))  ?  0.0f : x )

float my_abs(float f);
REAL fast_atan2(REAL y, REAL x);
float my_sqrt(float number);
float my_sqrt_reciprocal(float number);
double mx_sin(double rad);
double my_sin(double rad);
float my_cos(double rad);
float invSqrt(float x);
void LpFilter(ST_LPF* lpf);

#endif

