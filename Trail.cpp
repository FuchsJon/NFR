#include "Trail.h"
SVEC Trail::generate_prop(State S)
{
    SVEC output;
    VEC tmpv;
    vector<UINT16> vec;
    State tmp;
    for(int j = 0; j < TYPE/4; j++){
        vec = get_lut(S.get_col(j));
        tmpv.clear();
        for(int i = 0; i < vec.size(); i++){
            tmp = State(j,vec[i]);
            tmpv.push_back(tmp);   
            
        }
        if(vec.size() > 0)
            output.push_back(tmpv);
    }
    return output;
}
void Trail::increase_odometer(int rn){
    int K = prop[rn].size();
    if(indices[rn][0] == prop[rn][0].size()){
        prop.pop_back();
        indices.pop_back();
    }else{
        indices[rn][K-1]++;
        for(int j = K-1; (j > 0) && (indices[rn][j] == prop[rn][j].size());--j){
            indices[rn][j] = 0;
            indices[rn][j-1]++;
        }
    }
    
}
State Trail::gen_next_comb(int rn){
    int K = prop[rn].size();
    State tmp;
    for(int i = 0; i < K; i++)
        tmp = State(tmp,prop[rn][i][indices[rn][i]]);
    return tmp;
}

State Trail::extend_one_round(int r){
  State result;
  if(prop.size() > 0){
      result = gen_next_comb(r);
      result = calc.Lambda(result);
      bool cond;
      if(type == 1)
        cond = previousweight + result.get_weight() >= TARGET_WEIGHT -1;
      if(type == 2 || type ==0)
        cond = previousweight + result.get_weight() >= TARGET_WEIGHT;
      if(cond)
        result =0;
      else{
        cout <<"R2: " << endl << s << endl;
        State tmp = calc.Lambda(result);
        cout << "R3A: "<< endl << tmp << endl;
        cout << "R3B:" << endl << result << endl;
      }
      increase_odometer(r);
      if(indices[r][0] == prop[r][0].size())
            prop.pop_back();
  }
  return result;
  
}

VEC Trail::extend_two_rounds(){
    VEC output;
    int w1 = s.get_weight() + calc.Lambda(s).get_weight();
    State result = gen_next_comb(0);
    result = calc.Lambda(result);
    int w2 = result.get_weight();
    if(w1+w2 < TARGET_WEIGHT){
        if(prop.size() == 1){
            SVEC p = generate_prop(result);
            vector<int> index(p.size(),0);
            prop.push_back(p);
            int round1size =1;
            for(int i = 0; i < p.size(); i++)
             round1size *= prop[1][i].size();
            cout << "Round 2 size : " << dec << round1size << endl;
            indices.push_back(index);
        }
        State round4 = gen_next_comb(1);
        round4 = calc.Lambda(round4);
        increase_odometer(1);
        if(prop.size() == 1)
            increase_odometer(0);
        
        int w3 = round4.get_weight();
        if(w1+w2+w3 < TARGET_WEIGHT){
            cout << "w1: "<< w1 << endl;
            cout << "w2: " <<w2 << endl;
            cout << "w3: " <<w3 << endl;
            output.push_back(calc.Lambda(s));
            output.push_back(s);
            output.push_back(result);
            output.push_back(round4);
            if(round4.get_weight() > WEIGHT_BOUND){
                output.push_back(round4);
                output.push_back(calc.Lambda(result));
                output.push_back(calc.Lambda(s));
            }
        }
    }else{
        increase_odometer(0);
    }
    return output;   
}
State Trail::get_next_trail(){
    State output;
    while(prop.size() > 0 && output.get_weight() ==0){
        output = extend_one_round(0);
    }
    return output;
}


bool Trail::is_diff_compatible(State s, State v){
    bool output = true;
    vector<UINT16> gamma;
    for(int i = 0; i < TYPE/4; i++){
            gamma = get_lut(s.get_col(i));
            bool tmp = (find(gamma.begin(),gamma.end(),v.get_col(i)) != gamma.end());
            output &= tmp;

    }
    return output;
}


