#include "GenState.h"
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include "Trail.h"
//TODO fix trail.cpp
int main()
{
    vector<vector<unsigned int>> wvec;
    SVEC round3;
    GenState x;
    const clock_t begin = clock();
    int j = 0;
    do{
        ++x;
		if(x.get_state().empty())
		  break;
        cout << x <<endl;
        State s = x.get_stateform();
        Trail t(s,x.get_weight(),1);
        State tmp = t.get_next_trail();
        while(tmp.get_weight() > 0){
            wvec.push_back({x.get_weight(),tmp.get_weight()});
            round3.push_back({s,tmp});
            tmp = t.get_next_trail();
        }
    }while(!x.is_last());
    VEC round4;
    State tmp;
    cout << round3.size() << endl;
    for(int i = 0; i < round3.size(); i++){
        cout << "Trail number: " << i << endl;
        Trail t(round3[i][1],wvec[i][0]+wvec[i][1],0);
        tmp = t.get_next_trail();
        while(tmp.get_weight() > 0){
            tmp = t.get_next_trail();
            j++;
        }
        Trail k(round3[i][0],wvec[i][0]+wvec[i][1],2);
        tmp = k.get_next_trail();
        while(tmp.get_weight() > 0){
            cout << "Backward trail found" << endl;
            tmp = k.get_next_trail();
        }
    }
    /* ADD ONE BACKWARD EXP AND LINEAR CORROLATION TABLE
        */
    x.print_table();
    cout << float(clock() - begin) / CLOCKS_PER_SEC << endl;
    cout << "Total numbers of trails found : " << j << endl;
    
    return 0;
}

