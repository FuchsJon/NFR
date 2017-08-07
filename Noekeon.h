#ifndef NOEKEON_H_
#define NOEKEON_H_
#include "type.h"
#include "State.h"
#include <vector>
#include <bitset>
inline UINT64 rotate_left(UINT64 input, int offset){
	std::bitset<TYPE/4> temp(input);
	temp = (input << offset) | (input >> ((TYPE/4)-offset));
	return temp.to_ullong();
}

inline UINT64 rotate_right(UINT64 input, int offset){
	std::bitset<TYPE/4> temp(input);
	temp = (input >> offset) | (input << ((TYPE/4)-offset));
	return temp.to_ullong();
}
using namespace std;
class State;
class Noekeon{
private:
	vector<State> cache;
	
	int wa = 0, wb = 0;
public:
	Noekeon(){
		cache.push_back(State(0));	
	}
    Noekeon(State s){
        cache.push_back(s);    
    }
    void update(State s){
        cache.clear();
        cache.push_back(s);
    }
    State Theta(State in){
        UINT64 temp = in[0] ^ in[2];
	    temp ^= rotate_left(temp,TYPE/16) ^ rotate_right(temp,TYPE/16);
	    in[1] ^= temp;
	    in[3] ^= temp;
	    temp = in[1]^in[3];
	    temp ^= rotate_left(temp,TYPE/16) ^ rotate_right(temp,TYPE/16);
	    in[0] ^= temp;
	    in[2] ^= temp;
        return in;
    }
    State P1(State input){
	    input.rotate_left(0,OFFSET[0]);
        input.rotate_left(1,OFFSET[1]);
        input.rotate_left(2,OFFSET[2]);
        input.rotate_left(3,OFFSET[3]);
	    return input;
    }
	State P2(State input){
	    input.rotate_right(0,OFFSET[0]);
        input.rotate_right(1,OFFSET[1]);
        input.rotate_right(2,OFFSET[2]);
        input.rotate_right(3,OFFSET[3]);
	    return input;    
    }
	int get_wa(){return wa;}
	int get_wb(){return wb;}
	unsigned int total_weight(){
    update_weight();
	return wa+wb;
}
	State get_state(){ return cache.back();}
    State Lambda(State s);
    State LambdaR(State s);
	void push_slice(unsigned int sliceindex,UINT16 val){
		State temp(val);
		for(unsigned int i = 0; i < 4; i++){
			temp.rotate_left(i,sliceindex);
			temp[i] = temp[i] | cache.back()[i];			
		}
		cache.push_back(temp);
	}
	void pop_slice(){cache.pop_back();}
    void update_weight(){
        State tmp = cache.back();
        wa = P2(tmp).get_weight();
        wb = P2(Theta(tmp)).get_weight();
    }
};


#endif
