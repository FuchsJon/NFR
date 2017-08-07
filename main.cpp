#include "GenState.h"
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>
#include "Trail.h"

int main()
{
    GenState x;
    const clock_t begin = clock();
    int j = 0;
    do{
        ++x;
		if(x.get_state().empty())
		  break;
        cout << x <<endl;
    }while(!x.is_last());
    x.print_table();
    cout << float(clock() - begin) / CLOCKS_PER_SEC << endl;
    cout << "Total numbers of trails found : " << j << endl;
    return 0;
}
