#include "Unit.h"

Unit::Unit()
{
    a=0;
    b=0;
}

Unit::Unit(int sliceindex, int slicevalue)
{
    a = sliceindex;
    b = slicevalue;
}

Unit& Unit::operator++()
{
    if(b != max)
        b++;
    else
    {
        a++;
        b=1;
    }
    //cout << "trying" <<*this << endl;
    return *this;

}

ostream& operator<<(ostream& os, const Unit& u)
{

    os << '(' << dec << u.a << ','<< "0x"<< setfill('0') <<setw(4) << hex << u.b << ')';
    return os;
}
