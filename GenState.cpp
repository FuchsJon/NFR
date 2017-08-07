#include "GenState.h"

inline UINT16 rotateRight4(UINT16 in, int offset){
	offset = offset % 4;
	in = (in >> offset) | (in << (4-offset));
	return in &0xF;
}

inline UINT16 sliceRotate(UINT16 in, int offset){
	UINT16 mask = 0xF;
	UINT16 out =0;
	out = rotateRight4((in & mask),offset);
	out = out | (rotateRight4((in>>4 & mask),offset))<<4;
	out = out | (rotateRight4((in>>8 & mask),offset))<<8;
	out = out | (rotateRight4((in>>12 & mask),offset))<<12;
	return out;
}

inline Unit push_forward(Unit a){
	Unit out;
	if(a.get_a()+1 == (TYPE/16)){
		out.set_a(0);
		out.set_b(sliceRotate(a.get_b(),(TYPE/16)-1));
	}else{
		out.set_a(a.get_a()+1);
		out.set_b(a.get_b());
	}
	return out;
}
inline vector<Unit> push_forward(vector<Unit> vec){
	for(unsigned int i = 0; i < vec.size(); i++){
		vec[i] = push_forward(vec[i]);	
	}
		sort(vec.begin(),vec.end(),
			[](Unit a, Unit b){
				if(a.get_a() < b.get_a())
					return true;
				if(a.get_a() > b.get_a())
					return false;
				if(a.get_a() == b.get_a()){
					if(a.get_b() < b.get_b())
						return true;
					else
						return false;
				}
			}
		);
	return vec;
}

void GenState::update_table(){
	if(state.front().get_a() == 0){
		int wa = calc.get_wa(), wb = calc.get_wb();
		table[wa-1][wb-1]++;
	}
}


GenState::GenState() {
	push_slice(0,0);
	calc.push_slice(0,0);
}

GenState::GenState(int start) {
	push_slice(0,start);
	calc.push_slice(0,start);
}
GenState::GenState(int w1,int w2, int mode){
	push_slice(0,0);
	calc.push_slice(0,0);
    this->w1 = w1;
    this ->w2 = w2;
    this->mode = mode;
}

void GenState::push_slice(int sliceindex, int slicevalue) {
	Unit a(sliceindex, slicevalue);
	state.push_back(a);
}

void GenState::get_next() {
	if (!state.empty()) {
		do {
			++state.back();
			calc.pop_slice();
			calc.push_slice(state.back().get_a(),state.back().get_b());
			if (state.back().get_a() == unitlimit) {
				state.pop_back();
				calc.pop_slice();
				if (state.empty())
					break;
				++state.back();
				calc.pop_slice();
				calc.push_slice(state.back().get_a(),state.back().get_b());
			}

		} while (!is_valid() || !is_z_min());
	}
}

GenState& GenState::operator++() {
	if(!is_valid()){
			++state.back();
			calc.pop_slice();
			calc.push_slice(state.back().get_a(),state.back().get_b());
			if(is_valid()){
				if(state.front().get_a() == 0){
					int wa = calc.get_wa(), wb = calc.get_wb();
					table[wa-1][wb-1]++;
				}
				return *this;
			}	
	}
	if ((state.back().next().get_a() < unitlimit)) {
		state.push_back(Unit(state.back().get_a()+1,0));
		calc.push_slice(state.back().get_a(),state.back().get_b());
		get_next();
	} else
		get_next();

	if(state.front().get_a() == 0){
		int wa = calc.get_wa(), wb = calc.get_wb();
		table[wa-1][wb-1]++;
	}
	return *this;

}
bool GenState::is_valid() {
	bool output = false;
    if(mode ==0){
	unsigned int w = calc.total_weight();
	int wb = calc.get_wb();
	if(w <= WEIGHT_BOUND && w > 0)
		output = true;
    }
    if(mode == 1){
    int w = calc.total_weight();
    wa = calc.get_wa();
    wb = calc.get_wb();
    unsigned int target = TARGET_WEIGHT;
    if(wa <target -w1-w2 && wb <= w1 && w > 0 )
        output = true;
    }
	return output;
}

bool GenState::is_last() {
	bool output = false;
	Unit tmp = state.front();
	++tmp;
	if (tmp.get_a() == 1 || get_state().empty())
		output = true;
	return output;
}

bool GenState::is_z_min(){
	bool out = true;
	vector<Unit> temp = state;
	for(int i = 0; i < 32; i++){
		temp =push_forward(temp);
		if(pattern_cmp_b(state,temp))
			out=false;
	}
	return out;
}
bool GenState::pattern_cmp_b(const vector<Unit> a,const vector<Unit>b){
	bool output = false;
	for(unsigned int i = 0; i < a.size(); i++){
		if(a[i] != b[i]){
			if(a[i] < b[i]){
				output = false;
				break;
			}
			else{
				output = true;
				break;
			}
		}

	}
	return output;
}

void GenState::print_table(){
	for(int i = 0; i < 25; i++){
		for(int j = 0; j < 25; j++)
			cout <<setfill(' ')<< setw(5) << dec << table[i][j] << " ";
		cout << endl;
	}
}



ostream& operator<<(ostream &os,  GenState& gs) {
	unsigned int i;
		os << "w:" << setfill(' ') << setw(3)<<dec << gs.calc.total_weight() << " ";
		for (i = 0; i < gs.state.size(); i++)
			os << hex << gs.state[i] << ',' << ' ';
		os << endl;
	return os;
}

