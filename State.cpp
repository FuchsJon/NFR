#include "State.h"

inline unsigned int bit(UINT64 &input, unsigned int index){
	return (input >> index) & 0x1;
}



inline UINT64 expend_slice(UINT16 slice,unsigned int row){
	UINT64 result = ((slice >> (row*4)) & 0xF);
	int offset = (TYPE/16);
	UINT64 temp=0;
	UINT64 out = 0;
	for(int i = 0; i < 4; i++){
		temp = (result>>i)&0x1;
		temp = temp << (offset*i);
		out = out | temp;
	}
	return out;
}


/*void State::rotate_left(unsigned int index, unsigned int offset){

}*/

/*void State::rotate_right(unsigned int index, unsigned int offset){
	UINT64 &temp = operator[](index);
	int offtype = TYPE/4;
	temp = temp >> offset | temp << (offtype-offset);
	apply_mask(temp);
}*/
void State::rotate(){
    rotate_left(0,1);
    rotate_left(1,1);
    rotate_left(2,1);
    rotate_left(3,1);
}

void State::activate_bit(unsigned int row, unsigned int index){
	UINT64 &temp = operator[](row);
	UINT64 bit = 0x1 << index;
	temp = temp | bit;
}

unsigned int State::get_bit(unsigned int row, unsigned int index) {
	UINT64 temp = operator[](row);
	return bit(temp,index);
}

void State::slice_to_state(UINT16 slice){
	UINT64 b0 = 0 ,b1 = 0 ,b2 = 0,b3 = 0;
	b0 = expend_slice(slice,0);
	b1 = expend_slice(slice,1);
	b2 = expend_slice(slice,2);
	b3 = expend_slice(slice,3);
	a1 = b0;
	a2 = b1;
	a3 = b2;
	a4 = b3;
}

void State::slice_to_state(std::vector<UINT16> slices){
	UINT64 b0,b1,b2,b3;
	for(unsigned int i = 0; i < slices.size(); i++){
		b0 = expend_slice(slices[i],0);
		b1 = expend_slice(slices[i],1);
		b2 = expend_slice(slices[i],2);
		b3 = expend_slice(slices[i],3);
		a1 = a1 | (b0<<i);
		a2 = a2 | (b1<<i);
		a3 = a3 | (b2<<i);
		a4 = a4 | (b3<<i);
	}
}

UINT16 State::state_to_slice(){
    UINT16 output = 0;
    unsigned int offset = TYPE/16;
    output = output | get_bit(0,0);
    output = output | get_bit(0,offset)<<1;
    output = output | get_bit(0,offset*2)<<2;
    output = output | get_bit(0,offset*3)<<3;
    output = output | get_bit(1,0)<<4;
    output = output | get_bit(1,offset)<<5;
    output = output | get_bit(1,offset*2)<<6;
    output = output | get_bit(1,offset*3)<<7;
    output = output | get_bit(2,0)<<8;
    output = output | get_bit(2,offset)<<9;
    output = output | get_bit(2,offset*2)<<10;
    output = output | get_bit(2,offset*3)<<11;
    output = output | get_bit(3,0)<<12;
    output = output | get_bit(3,offset)<<13;
    output = output | get_bit(3,offset*2)<<14;
    output = output | get_bit(3,offset*3)<<15;
    return output;
}
std::ostream& operator<<(std::ostream& os,  State& state){
	for(unsigned int i = 0; i < 4; i++){
		std::bitset<TYPE/4> x(state[i]);
		os << x << std::endl;	
	}	
	return os;
}
