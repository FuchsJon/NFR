#ifndef GENSTATE_H_
#define GENSTATE_H_

#include "Unit.h"
#include "Noekeon.h"
#include <algorithm>

/*
	GenState is the class used to generate all the patterns that are within the weight bound.
	All settings regarding to the variant of the cipher and the weight bound are found int he type.h file.
*/
class Unit;
class Noekeon;


class GenState
{
private:
    vector<Unit> state;
    Noekeon calc;
    unsigned long long count = 0;
	int wa;
	int wb;
    int w1;
    int w2;
    int mode = 0;
    /*
      The unit limit is the upper bound for the depth, i.e how many active slices we allow in a state.
    */
    const static int unitlimit =TYPE/16;
    /*
      private method used for the ++operator.
    */
    void get_next();
    /*
	This method compares the order of two patterns, returns true if a > b.
    */
	bool pattern_cmp_b(const vector<Unit> a,const vector<Unit> b);
    unsigned int table[25][25] = {};

public:
	bool is_valid();
    GenState();
    GenState(int start);
    GenState(int w1,int w2, int mode);
    unsigned long long get_count(){ return count;}
    /*
      This method allows to add a slice to the current state.
    */
    void push_slice(int sliceindex,int slicevalue);
	void pop_slice(){state.pop_back();}
    vector<Unit> get_state()
    {
        return state;
    }
    /*
	returns the value wa+wb that the pattern represents.	
    */
    unsigned int get_weight(){
       return calc.total_weight();
    }
    /*
      Function that defines when to stop generating more states.
    */
    bool is_last();

    /*
       Z-canonicity check
    */
    bool is_z_min();
    /*
      Increment operator, gives us the next valid state.
    */
    GenState& operator++();
    friend ostream& operator<<(ostream& os,  GenState& state);
    void print_table();
    void update_table();
    State get_stateform(){return calc.get_state();}



};


#endif
