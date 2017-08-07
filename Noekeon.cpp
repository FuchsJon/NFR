#include "Noekeon.h"
State Noekeon::Lambda(State s){
    s = P1(s);
    s = Theta(s);
    s = P2(s);
    return s;
}
State Noekeon::LambdaR(State s){
    s = P2(s);
    s = Theta(s);
    s = P1(s);
    return s;
}

