#include<iostream>
#include<string.h>
#include<vector>
#include<ctime>
#include <sstream>
#include <fstream>
#include"People.h"

//Define the window displayed
const float WIN_LEFT= -20;
const float WIN_RIGHT= 20;
const float WIN_TOP= 40;
const float WIN_BOTTOM= 0;


const float Dt=0.001;   //time step.0.05
bool FIRST= true;

const int TOTAL=1000;
float pos0[TOTAL];
float pos1[TOTAL];


class Simulation
{
public:
	Simulation(){ dt= Dt; Time=0; }
	~Simulation();
    void setDt(float _dt){ dt= _dt; }

	int isNeighbor(const People& ip, const People& jp);
	void searchNP();       

	std::vector<People*>& getPeople() { return pvector; }
	void nextVelocity();

	void updateInt_p(); 
	void updateInt_wp();
	void updatePosition();
	void updateTotforce(); 
	void updateVelocity();
	void updateStatus();
	void interactionpeople(People& fp, People& ep);
	void interactionwall(People& fp, People& fw);
	void updateTime(){Time=Time+dt;};

	void generatepts();  
	Position checkpt[1][5];
	void initial_checkpts() {
	     checkpt[0][0] = Position(0,0);
		 checkpt[0][1] = Position(0,0);
		 checkpt[0][2] = Position(0,0);
		 checkpt[0][3] =  Position(0,0);
}
private:
	std::vector<People*> pvector; 
	float dt; 
	float Time;
};

Simulation::~Simulation()
{
	std::size_t size= pvector.size();
	for (unsigned int i= 0; i<size; i++)
		delete pvector[i];
}

Simulation::Simulation()
{
	dt= Dt;
}

void Simulation::searchNP()  //|ip-jp|<k*h
{
	std::size_t size= pvector.size();
	for (unsigned int i= 0; i< size; i++)
	 {	 
	  if ( pvector[i]->get_type() != WAL )
	  {
		pvector[i]->clearNP();
	  }
	 }
	for (unsigned int i= 0; i< size-1; i++)
	{  
	  if ( pvector[i]->get_type() != WAL )
	  {
		  for (unsigned int j= i+1; j<size; j++) 	
		  {
			  int index= isNeighbor((*pvector[i]),(*pvector[j]));
			  // if particle j is not a neighboring particle of particle i
			  if (index == 0)  
				  continue;	 
			  else
			  {
				pvector[i]->addNP(j);
				pvector[j]->addNP(i);
			  }
		  }
	  }
	}
}

int Simulation::isNeighbor(const People& ip, const People& jp)
{
	float dx= fabs(ip.get_pos().getx()-jp.get_pos().getx());
	float dy= fabs(ip.get_pos().gety()-jp.get_pos().gety());
	float r_p= ip.get_rp();
	if ( dx<r_p && dy<r_p && (dx*dx+dy*dy)<(r_p*r_p) )
		return -1;
	else
		return 0;
}
		
void Simulation::updateTotforce()  
{
	std::size_t size= pvector.size();
	for (unsigned int i= 0; i< size; i++)
	{
		TYPE t= pvector[i]->get_type();
		Position totinteraction(0.,0.); 
		std::set<int>& np= pvector[i]->getNP();
		for (std::set<int>::iterator it= np.begin(); it != np.end(); it++)
		{
				totinteraction = totinteraction + interactionpeople(*pvector[i],*pvector[it])+ interactionwall(*pvector[i],*pvector[it]);
		}
		pvector[i]->set_totforce(totinteraction);
	}
}

void Simulation::updatePosition()
{   
	std::size_t size= pvector.size();
	for (unsigned int i= 0; i<size; i++)
	{	
      if ( pvector[i]->get_type() != WAL )
	  {
		 Position intermedia=pvector[i]->get_pos();

		 pvector[i]->set_pos_new(pvector[i]->get_totforce()*pow(dt,2)/pvector[i]->get_m() 
			+ 2*pvector[i]->get_pos() - pvector[i]->get_pos_old());
		 
		 pvector[i]->set_pos_old(intermedia);

	  }
	}
	searchNP();
}

void Simulation::updateVelocity()
{
	std::size_t size= pvector.size();
	for (unsigned int i= 0; i<size; i++)
	{	
	  if ( pvector[i]->get_type() != WAL )
		pvector[i]->set_velocity((pvector[i]->get_pos() - pvector[i]->get_pos_old())/dt);
	}
}

void Simulation::updateStatus()
{	
	updateTotforce();
    updatePosition();
	updateVelocity();
}


void Simulation::interactionpeople(People& fp, People& ep) 
{

	Position rij= fp.get_pos() - ep.get_pos();
	Position eij= rij/rij.len();
	Position eji=eij.inver();

	float Di=fp.get_Di()+fp.get_td()*fp.get_velocity().len();
	float Dj=ep.get_Di()+ep.get_td()*ep.get_velocity().len();

	float dij=rij.len()-0.5*(Di+Dj);
	if (dij<0)
		std::cout<<"dij: negative distance"<<std::endl;

	float vij=0.5*((fp.get_velocity()-ep.get_velocity())*eij + abs((fp.get_velocity()-ep.get_velocity())*eij));
	float vji=0.5*((ep.get_velocity()-fp.get_velocity())*eji + abs((ep.get_velocity()-fp.get_velocity())*eji));

	float kij=0, kji=0;

	if (fp.get_velocity().len()>0)
		kij=0.5*(fp.get_velocity()*eij + abs(fp.get_velocity()*eij) ) / (fp.get_velocity().len());

	if (ep.get_velocity().len()>0)
		kji=0.5*(ep.get_velocity()*eji + abs(ep.get_velocity()*eji) ) / (ep.get_velocity().len());

	
	fp.set_Intp(-fp.get_m()*kij*pow((fp.get_mu()*fp.get_goalv()+vij),2)/dij*eij);
    ep.set_Intp(-ep.get_m()*kji*pow((ep.get_mu()*ep.get_goalv()+vji),2)/dij*eji);
	
}

void Simulation::interactionwall(People& fp, People& fw) 
{

	Position riw= fw.get_pos() - fp.get_pos();
	Position eiw= riw/riw.len();

	float Di=fp.get_Di()+fp.get_td()*fp.get_velocity().len();
	float diw=riw.len()-0.5*Di;
	
	if (diw<0)
	{	std::cout<<"diw: negative distance"<<std::endl;
	    std::abort();
	}

	float kiw=0;

	if (fp.get_velocity().len()>0)
		kiw=0.5*(fp.get_velocity()*eiw + abs(fp.get_velocity()*eiw) ) / (fp.get_velocity().len());

	//following portion is not general, needs to be revised in future for more general cases, yifan
	float vin;
	if (fw.get_pos().getx()>0.9){
		if (fw.get_pos().gety()<12)
			vin=fp.get_velocity()*Position(1,0);
		else
			vin=fp.get_velocity()*Position(0,1);
	}
	else{
		if (fw.get_pos().gety()<12)
			vin=fp.get_velocity()*Position(-1,0);
		else
			vin=fp.get_velocity()*Position(0,1);
	}
	
	fp.set_Intp(-fp.get_m()*kiw*pow((fw.get_mu()*fp.get_goalv()+vin),2)/diw*eiw);
    fw.set_Intp(Position());
	
}

void Simulation::generatepts() 
{
	srand(time(0));
	
    People* pt; 
	for (unsigned int i= 0; i<4; i++)
	{	
		for ( int j= 0; j<5; j++)	
		{
		 pt= new People( 0.3, 0.18, 0.1, 1.0, PEOP, Position(j*2.,i*2), Position(0,0) );
		 pvector.push_back(pt);
		}
	}
 
	std::string STRING;
    std::ifstream infile;

    int a;
    double xx=0;
    double yy=0;
	int z=0;
	
    infile.open ("domain.neu");

    for (int i=0;i<7;i++){
		getline(infile,STRING); 
		std::istringstream iss(STRING);
	    iss>>a;
	}

	getline(infile,STRING);
	getline(infile,STRING);
    for(int i=0;i<a;i++) // To get you all the lines.
    {
        getline(infile,STRING); // Saves the line in STRING.
        std::istringstream iss(STRING);
		iss>>z>>xx>>yy;
     
        pt= new People(0.3,0.18,0.1,1,WAL,Position(xx,yy),Position(0,0));
		pvector.push_back(pt);
    }
    infile.close();
}

