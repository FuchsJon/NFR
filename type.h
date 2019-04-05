#ifndef TYPE_H_
#define TYPE_H_
#include <vector>
#include <map>

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

/****
	In this file we define all the parameters that need to be changed for each variant of  
        Noekeon
*****/

const static int WEIGHT_BOUND = 9;

const static int TARGET_WEIGHT = 18;

const static int TYPE = 64;

const static unsigned int thetaoffset = TYPE/16;

static int OFFSET[4] = {0,1,5,2};



#endif
