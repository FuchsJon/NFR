#ifndef STATE_H_
#define STATE_H_


#include "type.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <bitset>
#include <vector>

class State{
private:
UINT64 a1,a2,a3,a4;
public:
State(){
	a1 =0;
	a2 =0;
	a3 =0;
	a4 =0;
}

State(UINT16 slice){
	a1 =0;
	a2 =0;
	a3 =0;
	a4 =0;
	slice_to_state(slice);
}
State(std::vector<UINT16> slices){
	a1 =0;
	a2 =0;
	a3 =0;
	a4 =0;
	slice_to_state(slices);
}
State(int offset, UINT16 colval){
	a1 = (colval&0x1) << offset;
	a2 = ((colval >> 1) & 0x1) << offset;
	a3 = ((colval >> 2) & 0x1) << offset;
	a4 = ((colval >> 3) & 0x1) << offset;
}
State(State a, State b){
    a1 = a[0] | b[0];
    a2 = a[1] | b[1];  
    a3 = a[2] | b[2];
    a4 = a[3] | b[3];  
}
State(UINT64 a, UINT64 b,UINT64 c,UINT64 d){
    a1 = a;
    a2 = b;
    a3 = c;
    a4 = d;
}
UINT64 &operator[](int index){
	switch(index){
		case 0 :
			return a1;
		case 1 :
			return a2;
		case 2 : 
			return a3;
		case 3 :
			return a4;
		default:
			std::cout << " index out of bound (0-3)" << std::endl;
			exit(EXIT_FAILURE);
	}
}
UINT16 get_col(int col){
	UINT16 out= (a1 >> col)&0x1;
	out = out | ((a2 >> col)&0x1)<<1;
	out = out | ((a3 >> col)&0x1)<<2;
	out = out | ((a4 >> col)&0x1)<<3;
	return out;
	
}
void rotate();
inline void apply_mask(UINT64& input){
	switch(TYPE){
		case 128:
			input = input & 0xFFFFFFFF;
			break;
		case 64 :
			input = input & 0xFFFF;
			break;
	}
	
}
void rotate_left(unsigned int index, unsigned int offset){
	UINT64 &temp = operator[](index);
	int offtype = TYPE/4;
	temp = temp << offset | temp >> (offtype-offset);
	apply_mask(temp);
}
void rotate_right(unsigned int index, unsigned int offset){
	UINT64 &temp = operator[](index);
	int offtype = TYPE/4;
	temp = temp >> offset | temp << (offtype-offset);
	apply_mask(temp);
}
void activate_bit(unsigned int row, unsigned int index);
unsigned int get_bit(unsigned int row, unsigned int index);
unsigned int get_weight(){
	UINT64 temp = a1 | a2 | a3 | a4;
    return std::bitset<TYPE/4>(temp).count();
}
void slice_to_state(UINT16 slice);
void slice_to_state(std::vector<UINT16> slices);
UINT16 state_to_slice();
friend std::ostream& operator<<(std::ostream& os,  State& state);
};

#endif
