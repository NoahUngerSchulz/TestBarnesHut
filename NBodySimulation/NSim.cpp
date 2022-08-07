#include "NSim.h"


void clear(std::string filename,std::ofstream& file_out){
  file_out.open(filename, std::ofstream::out | std::ofstream::trunc);
  file_out.close();
}
void write(std::ofstream& file_out,double n){
  file_out << n;
}
void write(std::ofstream& file_out,std::string word){
  file_out << word;
}
void write(std::ofstream& file_out,std::list<double>& L){
    for(std::list<double>::iterator i=L.begin(); i!=L.end(); i++){
      write(file_out,*i);
      write(file_out,",");
    }
}
void Write(std::string filex,std::list<double> X){
  std::ofstream file_out;
  clear(filex,file_out);
  file_out.open(filex, std::ios_base::app);
  write(file_out,X);
}

//*/


int Energy(std::vector<class Particle>& ps) {
  double TE = 0;
  for (std::vector<class Particle>::iterator p = ps.begin(); p != ps.end(); p++) {
    std::valarray<double> v = p->vel;
    double KE =1.0/2.0*innerProduct(v, v)*p->m;
    //std::cout<<"KE"<<KE<<std::endl;
    double UE=0;
    for (std::vector<class Particle>::iterator p1 = ps.begin(); p1 != ps.end(); p1++) {
      if (p != p1){
	std::valarray<double> r = p->pos - p1->pos;
	double R = sqrt(innerProduct(r, r));
	UE += -G * p->m * p1->m /R ;
      }
    }
    //std::cout<<"UE"<<UE<<std::endl;
    //std::cout<<"E"<<UE+KE<<std::endl;
    TE+=UE+KE;
  }
  return TE;
}

std::valarray<double> momentum(std::vector<class Particle>& ps) {
  std::valarray<double> TP = {0.0,0.0,0.0};
  for (std::vector<class Particle>::iterator p = ps.begin(); p != ps.end(); p++) {
    std::valarray<double> v = p->vel;
    std::valarray<double> P =v * p->m;
  TP+=P;
  }
  return TP;
}

std::valarray<double> spin(std::vector<class Particle>& ps) {
  std::valarray<double> TP = {0.0,0.0,0.0};
  std::valarray<double> com = {0.0,0.0,0.0};
  double totalmass=0;
  for (std::vector<class Particle>::iterator p = ps.begin(); p != ps.end(); p++) {
    totalmass+=p->m;
    com+=p->pos*p->m;
  }
  com=com/totalmass;
  for (std::vector<class Particle>::iterator p = ps.begin(); p != ps.end(); p++) {
    std::valarray<double> v = p->vel;
    std::valarray<double> r =p->pos-com;
    std::valarray<double> P ={v[2]*r[3]-v[3]*r[2],   v[3]*r[1]-v[1]*r[3],   v[1]*r[2]-v[2]*r[1]};
    P=p->m*P;
  TP+=P;
  }
  return TP;
}
double mag(std::valarray<double> P){
  double mag;
  for (int i=0; i != 3;i++) mag+=pow(P[i],2);
  mag=pow(mag,0.5);
  return mag;
}

std::valarray<double> difference(std::vector<class Particle>& ps,std::vector<class Particle>& dps) {
  std::valarray<double> TP = {0.0,0.0,0.0};
  std::vector<class Particle>::iterator D=dps.begin();
  for (std::vector<class Particle>::iterator p = ps.begin(); p != ps.end(); p++) {
    std::valarray<double> P=pow(pow(p->pos-D->pos,2),0.5);
    D++;
  TP+=P;
  }
  return TP;
}

void NSim_Step(std::vector<class Particle>& ps, class OCTree* T, double dt,int integrator) {
  Integrator( ps,  T,  dt, integrator);
}
void NSim_Step(std::vector<class Particle>& ps, class branch* T, double dt,int integrator) {
  Integrator( ps,  T,  dt, integrator);

}
void NSim_accuracy(std::vector<class Particle>& ps,std::vector<class Particle>& dps, std::string name,NSim& N){
  std::cout<<name<<std::endl;
  std::cout<<"Energy "<<(Energy(ps)-N.EnergyInit)/N.EnergyInit<<std::endl;
  std::cout<<"momentum "<<mag(momentum(ps)-N.momentumInit)/mag(N.momentumInit)<<std::endl;
  std::cout<<"spin "<<mag(spin(ps)-N.spinInit)/mag(N.spinInit)<<std::endl;
  std::cout<<"dif "<<mag(difference(ps,dps))<<std::endl;
  N.Energy+=(Energy(ps)-N.EnergyInit)/N.EnergyInit;
  N.momentum+=momentum(ps);
  N.spin+=spin(ps);
  N.dif+=difference(ps,dps);
}
void writegraph(NSim& N,std::string name){
  std::cout<<N.Energy;
  N.X.push_back(N.N);
  N.EnergyY.push_back(N.Energy);
  N.momentumY.push_back(mag(N.momentum));
  N.spinY.push_back(mag(N.spin));
  N.difY.push_back(mag(N.dif));
  N.timeY.push_back(N.time);
  Write(name+"X.txt",N.X);
  Write(name+"E"+".txt",N.EnergyY);
  Write(name+"M"+".txt",N.momentumY);
  Write(name+"S"+".txt",N.spinY);
  Write(name+"D"+".txt",N.difY);
  Write(name+"T"+".txt",N.timeY);
  N.Energy=0;
  N.momentum={0,0,0};
  N.spin={0,0,0};
  N.dif={0,0,0};
  N.time=0;
}

void NSim_Init(std::vector<class Particle>& ps, int num_ps,NSim& N,int seed,double density) {
  double scale=pow(num_ps,1.0/3.0);
  std::default_random_engine generator(seed);
  std::exponential_distribution<double> mass_dist(.3);
  std::uniform_real_distribution<double> pos_dist(-10.0*density*scale, 10.0*density*scale);
  std::uniform_real_distribution<double> vel_dist(-1.5*scale, 1.5*scale);
  std::vector<class Particle> tmp;
  std::cout <<"numps" << num_ps << std::endl;
  for (int i = 0; i < num_ps; i++) {
    double mass = mass_dist(generator) + 1;
    std::valarray<double> coords{ pos_dist(generator),pos_dist(generator),pos_dist(generator),vel_dist(generator),vel_dist(generator),vel_dist(generator) };
        Particle p(mass, coords);
	//std::cout << p.pos[0] << " , ";
        p.tag = i;
        tmp.push_back(p);
    }
    ps.assign(tmp.begin(), tmp.end());

    N.EnergyInit = Energy(ps);
    N.momentumInit = momentum(ps);
    N.spinInit = spin(ps);
    
}
void clearstruct(struct NSim N,std::string name){
  N.X={};
  N.EnergyY={};
  N.momentumY={};
  N.spinY={};
  N.difY={};
  N.timeY={};
  std::ofstream file_out;
  clear(name+"X.txt",file_out);
  clear(name+"E"+".txt",file_out);
  clear(name+"M"+".txt",file_out);
  clear(name+"S"+".txt",file_out);
  clear(name+"D"+".txt",file_out);
  clear(name+"T"+".txt",file_out);
  std::cout<<*N.X.end()<<std::endl;
}
