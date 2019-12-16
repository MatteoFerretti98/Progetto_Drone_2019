#ifndef SUPPORT_FUNCTIONS_H_
#define SUPPORT_FUNCTIONS_H_

#include <stdlib.h>

//Maps a value from a range to an other
inline float map (float VAL, float FROM_SRC, float TO_SRC, float FROM_DST, float TO_DST)
{
	return (((TO_DST-FROM_DST)/(TO_SRC-FROM_SRC))*(VAL-FROM_SRC)) + FROM_DST;
}

//Converts two's complement value in a decimal one
inline short C2toD(short src){
	if((src & 0x8000) == 0)	//first bit is 0
		return src;
	else					//first bit is 1
		return (src & 0x7FFF) - 0x8000;
}

#endif /* SUPPORT_FUNCTIONS_H_ */
