#ifndef MASSTREE_HH
#define MASSTREE_HH

#include <list>
#include <vector>
#include <iostream>

#include "Particle.h"






enum class Partition_axis {
	X,
	Y,
	Z
};

class OCTree {
public:
	double mass;
	std::valarray<double> r_cm;

	double x_min;
	double x_max;
	double y_min;
	double y_max;
	double z_min;
	double z_max;
	Partition_axis axis;
	class OCTree* r;
	class OCTree* l;
	int index;
	int depth;
	class OCTree*	parent;
	std::list<class Particle*>::iterator	start;
	std::list<class Particle*>::iterator	stop;
	std::valarray<double> center;
	std::valarray<int> 	corner;
	std::string name;
	OCTree(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop,int index,int depth,std::string name);
	~OCTree(void);
	void computeBondingBox(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop);
	void partitionVolume(void);
	std::list<class Particle*>::iterator Partition(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop,int index,int depth);
	int BinaryDigit(double N,int D);
        double Normalize(double x, int index);
	std::list<class Particle*>::iterator getPartitionIterator(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop);
	void computeMassMoments(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop);
	// center of mass and mass moments
};



class branch{
 public:
  double mass;
  std::valarray<double> r_cm;
  double x_min;
  double x_max;
  double y_min;
  double y_max;
  double z_min;
  double z_max;
  class branch*	b___;
  class branch*	b__1;
  class branch*	b_1_;
  class branch*	b_11;
  class branch*	b1__;
  class branch*	b1_1;
  class branch*	b11_;
  class branch*	b111;
  std::string name;
  int depth;
  class branch*	parent;
  std::list<class Particle*>::iterator	start;
  std::list<class Particle*>::iterator	stop;
  std::valarray<double> center;
  std::valarray<int> 	corner;
  std::valarray<double> scale;
  std::list<class Particle*> points;
  
  
  branch(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop,int depth,std::string name,class branch* parent,std::valarray<double>& center,std::valarray<int>& corner,std::valarray<double> scale);
  ~branch(void);
  std::valarray<int> BinaryDigit(std::valarray<double> N,int D);
  std::valarray<double> Normalize(std::valarray<double> x);
  void computeBondingBox();
  void INIT();
  void build(std::list<class Particle*>& ps,int b);
};
  //*/
  




class Tree: public OCTree {
public:
        double mass;
	std::valarray<double> r_cm;

	double x_min;
	double x_max;
	double y_min;
	double y_max;
	double z_min;
	double z_max;
	Partition_axis axis;
	class Tree* r;
	class Tree* l;
	Tree(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop,int index,int depth,std::string name);
	void computeBondingBox(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop);
	~Tree(void);
	void partitionVolume(void);
	void sortParticles(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop);
	std::list<class Particle*>::iterator getPartitionIterator(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop);
	// center of mass and mass moments
};

void buildTree(class Tree* T, std::list<class Particle*>& ptrs, std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop, std::list<class Particle*>::iterator part,int b);
//*/


void buildOCTree(class OCTree* T, std::list<class Particle*>& ptrs, std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop, std::list<class Particle*>::iterator part,int index,int depth,int b);
void orderParticles(std::vector<class Particle>& ps, std::list<class Particle*>& ptrs);
std::valarray<double> edges(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop);
//void printLeafs(class Tree* T);
//*/
#endif
