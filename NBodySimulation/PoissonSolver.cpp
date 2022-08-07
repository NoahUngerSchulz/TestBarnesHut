#include "PoissonSolver.h"

double softeningLength;

template<typename T>
T innerProduct(const std::valarray<T>& v1, const std::valarray<T>& v2) {
	return (v1 * v2).sum();
}

void Poisson_DirectSummation(std::vector<class Particle>& ps) {
	for (std::vector<class Particle>::iterator i = ps.begin(); i != ps.end(); i++) {
		for (std::vector<class Particle>::iterator j = i + 1; j != ps.end(); j++) {
			Poisson_CalculateForce_Newton(*i,*j);
		}
	}
}

void Poisson_CalculateForce_Hook(class Particle& p1, class Particle& p2) {
	std::valarray<double> r = std::valarray<double>(p1.tmp_coords[pos_i]) - std::valarray<double>(p2.tmp_coords[pos_i]);
	std::valarray<double> F = -r;
	p1.force += F;
	p2.force += -F;
}

void Poisson_CalculateForce_Newton(class Particle& p1, class Particle& p2) {
	std::valarray<double> r = std::valarray<double>(p1.tmp_coords[pos_i]) - std::valarray<double>(p2.tmp_coords[pos_i]);
	double R = pow(sqrt(innerProduct(r, r)), 3);
	std::valarray<double> F = -G * p1.m * p2.m * r / R;
	p1.force += F;
	p2.force += -F;
}

void Poisson_CalculateForce_SoftSphere(class Particle& p1, class Particle& p2) {
	std::valarray<double> r = std::valarray<double>(p1.tmp_coords[pos_i]) - std::valarray<double>(p2.tmp_coords[pos_i]);
	double SF = (innerProduct(r, r) + 5 * pow(softeningLength, 2) / 2) / pow(sqrt(innerProduct(r, r) + pow(softeningLength, 2)), 5);
	std::valarray<double> F = -G * p1.m * p2.m * r * SF;
	p1.force += F;
	p2.force += -F;
}



int children;
int NN;
int nln;
void Poisson_OCTreeForce(std::vector<class Particle>& ps, class OCTree* T) {
  children=0;
  NN=0;
  nln=0;
	for (std::vector<class Particle>::iterator p = ps.begin(); p != ps.end(); p++) {
	  Poisson_CalculateForce_OCTree(*p, T,0);
	}
	//std::cout<<T->name<<"children"<<children<<std::endl;
	//std::cout<<T->name<<"N*N"<<NN<<std::endl;
	//std::cout<<T->name<<"N*ln(N)"<<NN<<std::endl;
}



const double theta = 0.0;
void Poisson_CalculateForce_OCTree(class Particle& p, class OCTree* T,int index) {
  if (index>2) index=0;
  std::valarray<double> r = p.pos - T->r_cm;
  double com_dist = sqrt(innerProduct(r, r));
  if (T->l != nullptr && T->r != nullptr) {
    //std::cout << "Has Children" << std::endl;
    // cell length would be better quantified by the diagonal length of box
    // more difficult to calculate with max difference approx 1.4
    // how good of a measure is "node length" anyway?
    double cell_length;
    if (index  == 0) cell_length = T->x_max - T->x_min;
    else if (index == 1) cell_length = T->y_max - T->y_min;
    else cell_length = T->z_max - T->z_min;
		if (cell_length / com_dist < theta ) {
		  nln+=1;
		  double R = pow(com_dist, 3);
		  p.force += -G * p.m * T->mass * r / R; //* SF;
		}
		else {
		  children+=1;
		  Poisson_CalculateForce_OCTree(p, T->l,index+1);
		  Poisson_CalculateForce_OCTree(p, T->r,index+1);
		}
  }
  else {
    double tol = 0.1;
    if (com_dist > tol) {
	    //std::cout << "N*N interaction" <<T->name << std::endl;
      NN+=1;
      double R = pow(com_dist, 3);
      p.force += -G * p.m * T->mass * r / R; //* SF;
    }
	}
}

void Poisson_branchForce(std::vector<class Particle>& ps, class branch* T) {
	for (std::vector<class Particle>::iterator p = ps.begin(); p != ps.end(); p++) {
	  Poisson_CalculateForce_branch(*p, T);
	}
	//std::cout<<"poissoned"<<std::endl;
}

void Poisson_CalculateForce_branch(class Particle& p, class branch* T) {
  std::valarray<double> r;
  double com_dist;
  double R;
  class branch* BOX;
  BOX=p.box;
  //for (std::list<class Particle*>::iterator P = BOX->points.begin(); P != BOX->points.end(); P++) Poisson_CalculateForce_Newton(p,**P);
  class branch* NEWBOX;
  if (BOX!=nullptr){
    BOX=BOX->parent;
    while (BOX!=nullptr){
      NEWBOX=BOX->parent;
      if (NEWBOX!=nullptr){
	std::list<class branch*> Others={NEWBOX->b___,NEWBOX->b__1,NEWBOX->b_1_,NEWBOX->b_11,NEWBOX->b1__,NEWBOX->b1_1,NEWBOX->b11_,NEWBOX->b111};
	for (std::list<class branch*>::iterator b=Others.begin(); b!=Others.end(); b++){
	  if ((*b)!=BOX && (*b)!=nullptr){
	    r = p.pos - (*b)->r_cm;
	    com_dist = sqrt(innerProduct(r, r));
	    R = pow(com_dist, 3);
	    p.force += -G * p.m * (*b)->mass * r / R;
	  }
	}
      }
      BOX=NEWBOX;
    }
  }
}
//*/

void PoissonSolver(std::vector<class Particle>& ps, class OCTree* T) {
  //softeningLength = .98 * pow(ps.size(), -0.26);	
  if (T->name == "D" || T->name == "DR") {
    Poisson_DirectSummation(ps);
  }else Poisson_OCTreeForce(ps, T);
}
void PoissonSolver(std::vector<class Particle>& ps, class branch* T) {
  //softeningLength = .98 * pow(ps.size(), -0.26);
  if (T->name == "D" || T->name == "DR") {
    Poisson_DirectSummation(ps);
  }else Poisson_branchForce(ps, T);
}


