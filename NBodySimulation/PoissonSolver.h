#ifndef POISSONSOLVER_HH
#define POISSONSOLVER_HH

#include <vector>

#include "MassTree.h"

const double G = 1.0; //6.6743e-11;

template<typename T>
T innerProduct(const std::valarray<T>& v1, const std::valarray<T>& v2);

void PoissonSolver(std::vector<class Particle>& ps, class OCTree* = /* T */ nullptr);
//void PoissonSolver(std::vector<class Particle>& ps, class Tree* = /* T */ nullptr);
void Poisson_CalculateForce_Tree(class Particle& p, class Tree* T);
void Poisson_OCTreeForce(std::vector<class Particle>& ps, class OCTree* T);
void Poisson_CalculateForce_OCTree(class Particle& p, class OCTree* T,int index);

//void Poisson_TreeForce(std::vector<class Particle>& ps, class Tree* T);
//void Poisson_CalculateForce_Tree(class Particle& p, class Tree* T,int index);

void PoissonSolver(std::vector<class Particle>& ps, class branch* T);
void Poisson_branch(std::vector<class Particle>& ps, class branch* T);
void Poisson_CalculateForce_branch(class Particle& p, class branch* T);




void Poisson_DirectSummation(std::vector<class Particle>& ps);
void Poisson_CalculateForce_Hook(class Particle& p1, class Particle& p2);
void Poisson_CalculateForce_Newton(class Particle& p1, class Particle& p2);
void Poisson_CalculateForce_SoftSphere(class Particle& p1, class Particle& p2);

#endif
