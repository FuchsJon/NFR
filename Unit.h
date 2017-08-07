#ifndef UNIT_H_
#define UNIT_H_

#include "type.h"
#include <iomanip>
#include <iostream>
using namespace std;

/*
	This class represents a Unit of Noekeon, which we defined as a slice, i.e the the group of colums that affect each other in the Theta function.
        A unit can be seen as the tuple (z,slice_value).
*/

class Unit
{
private:
    UINT16 a;
    UINT16 b;
    const static UINT16 max = 0xFFFF;
public:
    Unit();
    Unit(int sliceindex,int slicevalue);
    Unit& operator++();
    /*
	This method return the next Unit in respect to the one currently being used.
    */
    Unit next()
    {
        if(a==0 && b==0)
            return Unit(0,1);
        else
            return Unit(a+1,1);
    }

    /*
	Getters and setters
    */
    void set_a(UINT16 val){
    	a = val;
    }

    UINT16 get_a() const
    {
        return a;
    }
    UINT16 get_b() const
    {
        return b;
    }
    void set_b(UINT16 val){
    	b = val;
    }
    /*
	Comparating operators, used in order to check for z-canonicity.
    */
    bool operator<(const Unit& u)const {
    	if((a<u.a)||(a==u.a && b<u.b))
    		return true;
    	else
    		return false;
    }
    bool operator<=(const Unit& u)const{
    	return operator<(u) || (a == u.a && b == u.b);
    }
    bool operator>(const Unit& u)const{
    	return !operator<=(u);
    }
    bool operator>=(const Unit& u)const{
    	return !operator<(u);
    }
    bool operator==(const Unit& u)const{
    	return (u.a==a && u.b == b);
    }
    bool operator!=(const Unit& u)const{
    	return !operator==(u);
    }
    friend ostream &operator<<(ostream& os, const Unit& u);

};

#endif
