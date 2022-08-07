#ifndef NSIM_HH
#define NSIM_HH

#include <valarray>
//#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <list>

#include "Particle.h"
#include "MassTree.h"
#include "PoissonSolver.h"
#include "Integrator.h"



struct NSim{
  double EnergyInit;
  std::valarray<double> momentumInit;
  std::valarray<double> spinInit;
  double Energy;
  std::valarray<double> momentum;
  std::valarray<double> spin;
  std::valarray<double> dif;
  std::list<double> X={};
  std::list<double> EnergyY={};
  std::list<double> momentumY={};
  std::list<double> spinY={};
  std::list<double> difY={};
  std::list<double> timeY={};
  double time;
  int N=1;
};

void clearstruct(struct NSim N,std::string name);


int Energy(std::vector<class Particle>& ps);
std::valarray<double> momentum(std::vector<class Particle>& ps);
std::valarray<double> spin(std::vector<class Particle>& ps);
void NSim_Step(std::vector<class Particle>& ps, class OCTree* T, double dt,int integrator);
void NSim_accuracy(std::vector<class Particle>& ps,std::vector<class Particle>& dps, std::string name,NSim& N);
void NSim_Init(std::vector<class Particle>& ps, int num_ps,NSim& N,int seed,double density);

void NSim_Step(std::vector<class Particle>& ps, class branch* T, double dt,int integrator);

std::valarray<double> difference(std::vector<class Particle>& ps,std::vector<class Particle>& dps);
void writegraph(NSim& N,std::string name);

void clear(std::string filename,std::ofstream& file_out);
void write(std::ofstream& file_out,double n);
void write(std::ofstream& file_out,std::string word);
void write(std::ofstream& file_out,std::list<double>& L);
void Write(std::string filex,std::list<double> X);
//*/


#endif
