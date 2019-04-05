#ifndef ANALYSIS_H_
#define ANALYSIS_H_
vector<vector<int>> diffWeight = {
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2},
{0,0,0,0,3,3,3,3,2,0,0,2,0,0,0,0},
{0,0,0,0,3,3,3,3,2,0,0,2,0,0,0,0},
{0,0,3,3,3,0,3,0,3,0,3,0,3,3,0,0},
{0,0,3,3,0,3,0,3,3,0,3,0,0,0,3,3},
{0,0,3,3,3,0,3,0,3,0,3,0,0,0,3,3},
{0,0,3,3,0,3,0,3,3,0,3,0,3,3,0,0},
{0,0,2,2,3,3,3,3,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,2,0,2,3,3,3,3},
{0,0,0,0,3,3,3,3,0,0,0,0,3,3,3,3},
{0,0,2,2,0,0,0,0,0,2,0,2,0,0,0,0},
{0,2,0,0,3,0,0,3,0,3,3,0,0,0,2,0},
{0,2,0,0,3,0,0,3,0,3,3,0,0,2,0,0},
{0,2,0,0,0,3,3,0,0,3,3,0,2,0,0,0},
{0,2,0,0,0,3,3,0,0,3,3,0,0,0,0,2}};

vector<vector<int>> linearDiff ={
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,1,2,2,0,0,2,2,1,0,2,2,0,0,2,2},
{0,2,2,0,0,2,2,0,0,2,2,1,1,2,2,0},
{0,2,0,2,1,2,0,2,1,2,0,2,0,2,0,2},
{0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0},
{0,0,2,2,0,1,2,2,1,0,2,2,0,0,2,2},
{0,2,2,0,0,2,2,0,0,2,2,0,1,2,2,1},
{0,2,0,2,1,2,0,2,1,2,0,2,0,2,0,2},
{0,1,0,1,0,1,0,1,0,0,0,0,0,0,0,0},
{0,0,2,2,1,0,2,2,0,1,2,2,0,0,2,2},
{0,2,2,0,0,2,2,0,0,2,2,0,1,2,2,1},
{0,2,1,2,0,2,0,2,0,2,0,2,0,2,1,2},
{0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0},
{0,0,2,2,1,0,2,2,0,0,2,2,0,1,2,2},
{0,2,2,0,0,2,2,0,0,2,2,1,1,2,2,0},
{0,2,0,2,0,2,1,2,0,2,1,2,0,2,0,2}};

int checkWeight(State x, State y,vector<vector<int>> diff){
    int result = 0 ;
    for (int i = 0; i < TYPE/4; i++){
        result += diff[x.get_col(i)][y.get_col(i)];
    }   
    return result;
}
vector<int> minDiffWeight = {0,2,2,2,3,3,3,3,2,2,3,2,2,2,2,2};
vector<int> minLinWeight  = {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int checkMinWeight(State x,vector<int> minW){
    int result = 0;
    for (int i = 0; i < TYPE/4; i++) result+= minW[x.get_col(i)];
    return result; 
}
bool badTrail(State x,State y){
     return (x[0] == y[0] && x[1] == y[1] && x[2] == y[2] && x[3] == y[3]);
}
void setOffset(vector<unsigned int> input){
    OFFSET[0] = input[0];
    OFFSET[1] = input[1];
    OFFSET[2] = input[2];
    OFFSET[3] = input[3];
}

void generateR3(SVEC &round3, vector<vector<unsigned int>> &wvec,int type){
    GenState x;
    do{

        ++x;
		if(x.get_state().empty())
		  break;
        State s = x.get_stateform();
        Trail t(s,x.get_weight(),1,type);
        State tmp = t.get_next_trail();
        while(tmp.get_weight() > 0){
            wvec.push_back({x.get_weight(),tmp.get_weight()});
            round3.push_back({s,tmp});
            tmp = t.get_next_trail();
        }
    }while(!x.is_last());
}

void generateR4 (SVEC &round4,SVEC &round3,vector<vector<unsigned int>> &wvec,int type){
    State tmp;
    Noekeon calc;
    for(int i = 0; i < round3.size(); i++){
        Trail t(round3[i][1],wvec[i][0]+wvec[i][1],0,type);
        tmp = t.get_next_trail();
        while(tmp.get_weight() > 0){
            round4.push_back({calc.P2(round3[i][0]),calc.P2(calc.Theta(round3[i][0])),calc.Lambda(round3[i][1]),round3[i][1],calc.Lambda(tmp),tmp});
            tmp = t.get_next_trail();
        }
        Trail k(round3[i][0],wvec[i][0]+wvec[i][1],2,type);
        tmp = k.get_next_trail();
        while(tmp.get_weight() > 0){
            round4.push_back({tmp,calc.Lambda(tmp),calc.P2(round3[i][0]),calc.P2(calc.Theta(round3[i][0])),calc.Lambda(round3[i][1]),round3[i][1]});
            tmp = k.get_next_trail();
        }
    }
}

void printTrail(SVEC &round4,int i){
           cout << "Trail " << i << ":\n" 
                << round4[i][0] << "---Lambda--->\n" << round4[i][1] << "---Gamma--->\n" 
                << round4[i][2] << "---Lambda--->\n" << round4[i][3] << "---Gamma--->\n" 
                << round4[i][4] << "---Lambda--->\n" << round4[i][5] << endl;
}
int trailWeight(SVEC &round4,int i,int type){

    if(type == 0) return checkMinWeight(round4[i][0],minDiffWeight) + checkWeight(round4[i][1],round4[i][2],diffWeight) + checkWeight(round4[i][3],round4[i][4],diffWeight) + checkMinWeight(round4[i][5],minDiffWeight);
    if(type == 1) return checkMinWeight(round4[i][0],minLinWeight) + checkWeight(round4[i][1],round4[i][2],linearDiff) + checkWeight(round4[i][3],round4[i][4],linearDiff) + checkMinWeight(round4[i][5],minLinWeight);
    return 0;
}
bool printResult(SVEC &round4,int type,bool print){
    int total_weight;
    int min=64;
    bool bad = false;
    if(type == 0) cout << "Differential Trails: " << endl;
    if(type == 1) cout << "Linear Trails: " << endl;
    for(int i = 0; i < round4.size();i++){
          if (print) printTrail(round4,i);
           total_weight = trailWeight(round4,i,type);
           if (min > total_weight) min = total_weight;
           bad = bad || badTrail(round4[i][0],round4[i][2]);
          if(print) cout << "Weight: "<< total_weight << endl;
    }
    cout << "Best 4-Round trail found with weight: " << min << endl;
    cout << "Bad trails: " << bad << endl;
    return bad;
}
bool analysis(int type,bool print){
    vector<vector<unsigned int>> wvec;
    SVEC round3;
    GenState x;
    Noekeon calc;
    SVEC round4;
    cout << "Analysis for rotation: " << OFFSET[0] <<OFFSET[1] <<OFFSET[2] <<OFFSET[3] << " T4=" << 2*TARGET_WEIGHT <<endl; 
    generateR3(round3,wvec,type);
    generateR4(round4,round3,wvec,type);
    cout << "Total numbers of trails found : " << round4.size() << endl;
    return printResult(round4,type,print);
}
bool diffAnalysis(bool print){
    return analysis(0,print);
}
bool linearAnalysis(bool print){
    return analysis(1,print);
}

#endif
