#include<vector>
#include<set>
#include<algorithm>
#include"Position.h"


enum TYPE {PEOP,WAL};

class People
{
public:
	
	People(float _tau, float _D_i, float _t_d, float _mass= 1., TYPE _type= PEOP,
		Position _pos= Position(), Position _vel= Position()):
	tau(_tau), D_i(_D_i), t_d(_t_d), mass(_mass), type(_type), People_pos_old(_pos),People_velocity(_vel){checkptindex=0;}
	
	float get_tau() const { return tau; }
	float get_Di() const { return D_i; }
	float get_td() const { return t_d; }
	float get_m() const {return mass; }   
	float get_mu() const {return mu;}
	float get_goalv() const {return goalvelocity;}
	TYPE get_type(){ return type; } 
	int get_checkptindex(){checkptindex++; return checkptindex-1;}
	float get_rp(){return r_p;}
	float get_rw(){return r_w;}
	Position get_interactionpeople() const { return Int_p; } 
	Position get_interationwall() const { return Int_pw; } 
	Position get_totforce() const { return totalforce; }
	Position get_velocity() const { return People_velocity; }
	Position get_pos() const { return People_pos_new; } 
	Position get_pos_old() const { return People_pos_old; }  
	
	void set_m(float _m) {mass= _m; }
	void set_Intp(Position _f){ Int_p= _f; }
	void set_Intpw(Position _f){ Int_pw= _f; }
	void set_totforce(Position _f){ totalforce= _f; }
	void set_velocity(Position _v){ People_velocity= _v; }
	void set_pos_new(Position _p) {  People_pos_new= _p;}
	void set_pos_old(Position _p) { People_pos_old= _p;}
	
	void clearNP(){ neighborlist.clear(); }
	void addNP(int index) { neighborlist.insert(index);}
    std::set<int>& getNP() { return neighborlist; }

private:
	float tau;
	float D_i, t_d;
	float mu, r_w, r_p;
	float mass;
	float goalvelocity;
	std::set<int> neighborlist;
	Position People_pos_old, People_pos_new;
	Position Int_p,Int_pw,totalforce;  
	Position People_velocity;
	TYPE type;  
	int checkptindex;


};
