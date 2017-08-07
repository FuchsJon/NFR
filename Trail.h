#ifndef TRAIL_H_
#define TRAIL_H_
#include "Noekeon.h"
#include "State.h"
#include <vector>
#include <map>
#include <algorithm>

typedef std::vector<State> VEC;
typedef std::vector<std::vector<State>> SVEC;
typedef std::vector<SVEC> PVEC;

using namespace std;
class Noekeon;
class State;

class Trail{
private:

map< UINT16,std::vector<UINT16> > lut;
vector<vector<int>> indices;
PVEC prop;
State s;
bool nt = true;
Noekeon calc;
void reset_odometer(){
    indices.clear();
}
void increase_odometer(int rn);
void next_odoval();
int previousweight;
int type;
public:

Trail(State in,int w1,int t){
	std::vector<UINT16> temp;
	temp ={0xc,0xd,0xe,0xf};
	lut[0x1] = temp;
	temp.clear();
	temp ={0x4,0x8,0x5,0x6,0x7,0xb};
	lut[0x2] = temp;
	temp.clear();
	temp ={0x2,0x4,0x8,0x3,0x6,0xa,0xc,0xd};
	lut[0x4] = temp;
	temp.clear();
	temp ={0x2,0x4,0x3,0x5,0x6,0x7};
	lut[0x8] = temp;
	temp.clear();
	temp ={0x4,0x8,0x5,0x6,0x7,0xb};
	lut[0x3] = temp;
	temp.clear();
	temp ={0x2,0x8,0x3,0x5,0xa,0x7,0xe,0xf};
	lut[0x5] = temp;
	temp.clear();
	temp ={0x2,0x4,0x8,0x3,0x6,0xa,0xe,0xf};
	lut[0x6] = temp;
	temp.clear();
	temp ={0x9,0xc,0xb,0xd,0xe,0xf};
	lut[0x9] = temp;
	temp.clear();
	temp ={0x4,0x5,0x6,0xc,0x7,0xd,0xe,0xf};
	lut[0xa] = temp;
	temp.clear();
	temp ={0x1,0x4,0x9,0xa,0x7,0xe};
	lut[0xc] = temp;
	temp.clear();
	temp ={0x2,0x8,0x3,0x5,0xa,0xc,0x7,0xd};
	lut[0x7] = temp;
	temp.clear();
	temp ={0x2,0x3,0x9,0xb};
	lut[0xb] = temp;
	temp.clear();
	temp ={0x1,0x4,0x9,0xa,0x7,0xd};
	lut[0xd] = temp;
	temp.clear();
	temp ={0x1,0x5,0x6,0x9,0xa,0xc};
	lut[0xe] = temp;
	temp.clear();
	temp ={0x1,0x5,0x6,0x9,0xa,0xf};
	lut[0xf] = temp;
	temp.clear();
	lut[0] = {};    
    s = in;	
    type = t;
    if(type == 1)
        s = calc.P2(calc.Theta(s));
    if(type == 2)
        s = calc.P2(s);
    SVEC p = generate_prop(s);
    prop.push_back(p);
    vector<int> index(p.size(),0);
    indices.push_back(index);
    previousweight = w1;

    
}
std::vector<UINT16> get_lut(UINT16 index){
	return lut[index];
}

SVEC generate_prop(State s);
State gen_next_comb(int rn);
State extend_one_round(int r);
VEC extend_two_rounds();
void update_state(State s){
    this->s = s;
    SVEC p = generate_prop(s);
    prop.clear();
    prop.push_back(p);
    indices.clear();
    vector<int> index(prop[0].size(),0);
    indices.push_back(index);
    
}
bool is_diff_compatible(State s, State v);
bool is_linear_compatible(State s, State v);
State get_next_trail();

};



#endif 
