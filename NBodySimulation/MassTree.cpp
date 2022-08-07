#include <MassTree.h>
/*
branch::branch(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop,int depth,std::string name,class branch parent,std::valarray<double> center,std::valarray<int> corner) :
	mass{0},
	r_cm{0,0,0},
	x_min{ (*start)->pos[0] },
	x_max{ (*start)->pos[0] },
	y_min{ (*start)->pos[1] },
	y_max{ (*start)->pos[1] },
	z_min{ (*start)->pos[2] },
	z_max{ (*start)->pos[2] },
	b___{nullptr},
	b__1{nullptr},
	b_1_{nullptr},
	b_11{nullptr},
	b1__{nullptr},
	b1_1{nullptr},
	b11_{nullptr},
	b111{nullptr},
	name{name},
	depth{depth},
	parent{parent},
	start{start},
	stop{stop},
	center{center},
	corner{corner}
{
}
//*/

branch::branch(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop,int depth,std::string name,class branch* parent,std::valarray<double>& center,std::valarray<int>& corner,std::valarray<double> scale) :
        mass{0},
	r_cm{0,0,0},
	x_min{ (*start)->pos[0] },
	x_max{ (*start)->pos[0] },
	y_min{ (*start)->pos[1] },
	y_max{ (*start)->pos[1] },
	z_min{ (*start)->pos[2] },
	z_max{ (*start)->pos[2] },
	b___{nullptr},
	b__1{nullptr},
	b_1_{nullptr},
	b_11{nullptr},
	b1__{nullptr},
	b1_1{nullptr},
	b11_{nullptr},
	b111{nullptr},
	name{name},
	depth{depth},
	parent{parent},
	start{start},
	stop{stop},
	center{center},
	corner{corner},
	scale{scale}
{
}


branch::~branch(void) {
  if (b___ != nullptr) delete b___;
  if (b__1 != nullptr) delete b__1;
  if (b_1_ != nullptr) delete b_1_;
  if (b_11 != nullptr) delete b_11;
  if (b1__ != nullptr) delete b1__;
  if (b1_1 != nullptr) delete b1_1;
  if (b11_ != nullptr) delete b11_;
  if (b111 != nullptr) delete b111;

}

std::valarray<double> edges(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop){
  double x_min=1000.0;
  double x_max=-1000.0;
  double y_min=1000.0;
  double y_max=-1000.0;
  double z_min=1000.0;
  double z_max=-1000.0;
  for (std::list<class Particle*>::iterator p = start; p != stop; p++) {
		if ((*p)->pos[0] < x_min) x_min = (*p)->pos[0];
		if ((*p)->pos[0] > x_max) x_max = (*p)->pos[0];
		if ((*p)->pos[1] < y_min) y_min = (*p)->pos[1];
		if ((*p)->pos[1] > y_max) y_max = (*p)->pos[1];
		if ((*p)->pos[2] < z_min) z_min = (*p)->pos[2];
		if ((*p)->pos[2] > z_max) z_max = (*p)->pos[2];
	}
  return {x_min,y_min,z_min,x_max,y_max,z_max};
}

std::valarray<int> branch::BinaryDigit(std::valarray<double> N,int D){
  std::valarray<double> n=N/pow(2,-D+1);
  return {(int)(2*(n[0]-(int(n[0])))),(int)(2*(n[1]-(int(n[1])))),(int)(2*(n[2]-(int(n[2]))))};
}

std::valarray<double> branch::Normalize(std::valarray<double> x){
  double zero = 0;
  return {(x[0]-x_min)/(x_max-x_min)+zero,(x[1]-y_min)/(y_max-y_min)+zero,(x[2]-z_min)/(z_max-z_min)+zero};
}

void branch::computeBondingBox() {
  x_min = center[0];
  x_max = center[0]+scale[0]*pow(.5,depth);
  y_min = center[1];
  y_max = center[1]+scale[1]*pow(.5,depth);
  z_min = center[2];
  z_max = center[2]+scale[2]*pow(.5,depth);
}

void branch::INIT(){
  center={center[0]+scale[0]*corner[0]*pow(0.5,depth),center[1]+scale[1]*corner[1]*pow(0.5,depth),center[2]+scale[2]*corner[2]*pow(0.5,depth)};
  computeBondingBox();
  for (std::list<class Particle*>::iterator p = start; p != stop; p++) {
    std::valarray<double> Nx=Normalize((*p)->pos);
    if (Nx[0]<=1 && Nx[0]>=0    &&     Nx[1]<=1 && Nx[1]>=0      &&    Nx[2]<=1 && Nx[2]>=0 ){
      points.push_back(*p);
      (*p)->box=this;   
    }
  }
  start=points.begin();
  stop=points.end();
  //std::cout<<distance(start,stop)<<std::endl;
  mass=0;
  for (std::list<class Particle*>::iterator p = start; p != stop; p++) {
    mass += (*p)->m;
    r_cm += (*p)->m * (*p)->pos;
  }
  if (mass>0) r_cm = r_cm/mass;
  
}

void branch::build(std::list<class Particle*>& ps,int b){
  if (std::distance(start,stop)>b){
    //std::cout<<"build"<<std::endl;
       std::valarray<int> CORNER={0,0,0};
        branch* b1 = new branch(ps,start,stop,depth+1,name,this,center,CORNER,scale);
	b1->INIT();
	b1->build(ps,b);
	b___=b1;
        CORNER={0,0,1};
	branch* b2 = new branch(ps,start,stop,depth+1,name,this,center,CORNER,scale);
	b2->INIT();
	b2->build(ps,b);
	b__1=b2;
        CORNER={0,1,0};
	branch* b3 = new branch(ps,start,stop,depth+1,name,this,center,CORNER,scale);
	b3->INIT();
	b3->build(ps,b);
	b_1_=b3;
        CORNER={0,1,1};
	branch* b4 = new branch(ps,start,stop,depth+1,name,this,center,CORNER,scale);
	b4->INIT();
	b4->build(ps,b);
	b_11=b4;
        CORNER={1,0,0};
	branch* b5 = new branch(ps,start,stop,depth+1,name,this,center,CORNER,scale);
	b5->INIT();
	b5->build(ps,b);
	b1__=b5;
        CORNER={1,0,1};
	branch* b6 = new branch(ps,start,stop,depth+1,name,this,center,CORNER,scale);
	b6->INIT();
	b6->build(ps,b);
	b1_1=b6;
        CORNER={1,1,0};
	branch* b7 = new branch(ps,start,stop,depth+1,name,this,center,CORNER,scale);
	b7->INIT();
	b7->build(ps,b);
	b11_=b7;
        CORNER={1,1,1};
	branch* b8 = new branch(ps,start,stop,depth+1,name,this,center,CORNER,scale);
	b8->INIT();
	b8->build(ps,b);
	b111=b8; 
  }
}






		//*/
OCTree::OCTree(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop,int index,int depth,std::string name) :
	mass{0},
	r_cm{0,0,0},
	x_min{ (*start)->pos[0] },
	x_max{ (*start)->pos[0] },
	y_min{ (*start)->pos[1] },
	y_max{ (*start)->pos[1] },
	z_min{ (*start)->pos[2] },
	z_max{ (*start)->pos[2] },
	l{nullptr},
	r{nullptr},
	name{name}
{
}

OCTree::~OCTree(void) {
	if (l != nullptr) delete l;
	if (r != nullptr) delete r;
}


void OCTree::computeBondingBox(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop) {
	//std::cout << "new tree" << std::endl;
	for (std::list<class Particle*>::iterator p = start; p != stop; p++) {
		if ((*p)->pos[0] < x_min) x_min = (*p)->pos[0];
		if ((*p)->pos[0] > x_max) x_max = (*p)->pos[0];
		if ((*p)->pos[1] < y_min) y_min = (*p)->pos[1];
		if ((*p)->pos[1] > y_max) y_max = (*p)->pos[1];
		if ((*p)->pos[2] < z_min) z_min = (*p)->pos[2];
		if ((*p)->pos[2] > z_max) z_max = (*p)->pos[2];
	}
	//std::cout << "x: (" << x_min << "," << x_max << ")\t";
	//std::cout << "y: (" << y_min << "," << y_max << ")\t";
	//std::cout << "z: (" << z_min << "," << z_max << ")" << std::endl;
}

int OCTree::BinaryDigit(double N,int D){
  double n=N/pow(2,-D+1);
  return (int)(2*(n-(int)(n)));
}

double OCTree::Normalize(double x,int index){
  double zero = pow(10,-5);
  if (index==0) x=(x-x_min)/(x_max-x_min)+zero*x;
  else if (index==1) x=(x-y_min)/(y_max-y_min)+zero*x;
  else x=(x-z_min)/(z_max-z_min)+zero*x;
  return x;
}

std::list<class Particle*>::iterator OCTree::Partition(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop,int index,int depth) {
  computeBondingBox(start,stop);
  int dist;
  int zeroes=0;
  for (std::list<class Particle*>::iterator itr = start; itr != stop; itr++) {
    //std::cout<<Normalize((**itr).pos[index],index)<<",";
    //std::cout<<(BinaryDigit(Normalize((**itr).pos[index],index),depth+1)==0)<<",";
    if (BinaryDigit(Normalize((**itr).pos[index],index),depth+1)==0){
      dist=std::distance(start,itr);
      ps.splice(start, ps, itr);
      start=itr;
      std::advance(itr,dist);
      zeroes+=1;
    }
  }
  std::list<class Particle*>::iterator part = start;
  std::advance(part,zeroes-1);
  //std::cout<<"sorted"<<distance(start,stop)<<",";
  //std::cout<<"sorted"<<distance(part,stop)<<",";
  //std::cout<<"sorted"<<distance(start,part)<<std::endl;
  return part;
}

void OCTree::computeMassMoments(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop) {
	if (l == nullptr && r == nullptr) {
		for (std::list<class Particle*>::iterator p = start; p != stop; p++) {
			mass += (*p)->m;
			r_cm += (*p)->m * (*p)->pos;
		}
		r_cm = r_cm / mass;
	}
	else if (l != nullptr && r != nullptr) {
		mass = l->mass + r->mass;
		r_cm = (l->mass * l->r_cm + r->mass * r->r_cm) / mass;
	}
	else {
		// should never run
		std::cout << "Check MassTree.ccp: Tree::computeMassMoments method." << std::endl;
		if (l == nullptr) {
			mass = r->mass;
			r_cm = r->r_cm;
		}
		else {
			mass = l->mass;
			r_cm = l->r_cm;
		}
	}
}



void buildOCTree(class OCTree* T, std::list<class Particle*>& ptrs, std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop, std::list<class Particle*>::iterator part,int index, int depth, int b) {
    if (index > 2){
    index=0;
    depth+=1;
  }
  std::string name="branch";
  auto left_dist = distance(start, part);
  auto right_dist = distance(part, stop);
  //std::cout<<left_dist<<std::endl;
  if (left_dist+right_dist > 2*b && depth<20) {
    //std::cout<<left_dist<<" , "<<right_dist<<std::endl;
    OCTree* left = new OCTree(ptrs, start, part,index+1,depth,name);
    T->l = left;
    if (left_dist > b) {
      std::list<class Particle*>::iterator pl = left->Partition(ptrs,start,part,index,depth);
      //std::cout<<"hi";
      buildOCTree(left, ptrs, start, part,pl,index+1,depth,b);
    }
    left->computeMassMoments(start, part);
    OCTree* right = new OCTree(ptrs, part, stop,index+1,depth,name);
    T->r = right;
    if (right_dist > b) {
      std::list<class Particle*>::iterator pr = right->Partition(ptrs,part,stop,index,depth);
      //std::cout<<"hi";
      buildOCTree(right, ptrs, part, stop, pr,index+1,depth,b);
    }
    right->computeMassMoments(part, stop);
  }
  //std::cout<<"built"<<depth<<std::endl;
}




Tree::Tree(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop,int index,int depth,std::string name):
  OCTree(ps, start, stop,index,depth,name),
	mass{0},
	r_cm{0,0,0},
	x_min{ (*start)->pos[0] },
	x_max{ (*start)->pos[0] },
	y_min{ (*start)->pos[1] },
	y_max{ (*start)->pos[1] },
	z_min{ (*start)->pos[2] },
	z_max{ (*start)->pos[2] },
	l{nullptr},
	r{nullptr}
	
{
	computeBondingBox(start,stop);
	partitionVolume();
	sortParticles(ps,start,stop);
}

Tree::~Tree(void) {
	if (l != nullptr) delete l;
	if (r != nullptr) delete r;
}


void Tree::computeBondingBox(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop) {
	//std::cout << "new tree" << std::endl;
	for (std::list<class Particle*>::iterator p = start; p != stop; p++) {
		if ((*p)->pos[0] < x_min) x_min = (*p)->pos[0];
		if ((*p)->pos[0] > x_max) x_max = (*p)->pos[0];
		if ((*p)->pos[1] < y_min) y_min = (*p)->pos[1];
		if ((*p)->pos[1] > y_max) y_max = (*p)->pos[1];
		if ((*p)->pos[2] < z_min) z_min = (*p)->pos[2];
		if ((*p)->pos[2] > z_max) z_max = (*p)->pos[2];
	}
	//std::cout << "x: (" << x_min << "," << x_max << ")\t";
	//std::cout << "y: (" << y_min << "," << y_max << ")\t";
	//std::cout << "z: (" << z_min << "," << z_max << ")" << std::endl;
}

void Tree::partitionVolume(void) {
	double dx = x_max - x_min;
	double dy = y_max - y_min;
	double dz = z_max - z_min;
	//std::cout << "dx: " << dx << "\tdy: " << dy << "\tdz: " << dz << std::endl;
	double tol = 1e-5;
	if (dx > dy + tol && dx > dz + tol) axis = Partition_axis::X;
	else if (dy > dx + tol && dy > dz + tol) axis = Partition_axis::Y;
	else if (dz > dx + tol && dz > dy + tol) axis = Partition_axis::Z;
	else axis = Partition_axis::Z;
}

void Tree::sortParticles(std::list<class Particle*>& ps, std::list<class Particle*>::iterator& start, std::list<class Particle*>::iterator& stop) {
	int index;
	double partition;
	if (axis == Partition_axis::X) {
		index = 0;
		partition = (x_max + x_min) / 2;
	}
	else if (axis == Partition_axis::Y) {
		index = 1;
		partition = (y_max + y_min) / 2;
	}
	else {
		index = 2;
		partition = (z_max + z_min) / 2;
	}
	int dist;
	for (std::list<class Particle*>::iterator itr = start; itr != stop; itr++) {
		// could be more careful to ensure base case always skips
		if ((*itr)->pos[index] < partition) {
			dist = std::distance(start, itr);
			ps.splice(start, ps, itr);
			start = itr;
			std::advance(itr,dist);
		}
	}
	//std::cout << "axis: " << index << "\tpart: " << partition << std::endl;
	//std::cout << "tag\tloc\tcoord" << std::endl;
	//for (std::list<class Particle*>::iterator itr = start, int i = 0; itr != stop; itr++, i++) //std::cout << (*itr)->tag << "\t" << i << "\t" << (*itr)->pos[index] << std::endl;
}

std::list<class Particle*>::iterator Tree::getPartitionIterator(std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop) {
	int index;
	double p;
	if (axis == Partition_axis::X) {
		p = (x_max + x_min) / 2;
		index = 0;
	}
	else if (axis == Partition_axis::Y) {
		p = (y_max + y_min) / 2;
		index = 1;
	}
	else {
		p = (z_max + z_min) / 2;
		index = 2;
	}
	//std::cout << "check partition" << std::endl;
	//std::cout << "axis: " << index << "\tpart: " << p << std::endl;
	std::list<class Particle*>::iterator curr = start;
	while ((*curr)->pos[index] < p) {
		// part could be either start or stop for base case
		//std::cout << (*curr)->pos[index] << std::endl;
		curr++;
	}
	//std::cout << "part: " << (*curr)->pos[index] << std::endl;
	return curr;
}



void buildTree(class Tree* T, std::list<class Particle*>& ptrs, std::list<class Particle*>::iterator start, std::list<class Particle*>::iterator stop, std::list<class Particle*>::iterator part,int b) {
	//std::cout << "new recurse" << std::endl;
	auto left_dist = distance(start, part);
	auto right_dist = distance(part, stop);
	//std::cout << "dist:\t" << left_dist << "\t" << right_dist << "\t" << left_dist + right_dist << std::endl;
	//std::cout << 3 * b / 2 << std::endl;
	std::string name="branch";
	if (left_dist + right_dist >= 3*b/2 && T->x_min!=T->x_max) {
	  Tree* left = new Tree(ptrs, start, part,0,0,name);
	  
		T->l = left;
	        
		if (left_dist > b) {
			std::list<class Particle*>::iterator pl = left->getPartitionIterator(start, part);
			buildTree(left, ptrs, start, part, pl,b);
		}
	        
		left->computeMassMoments(start, part);
	        
		Tree* right = new Tree(ptrs, part, stop,0,0,name);
	        
		T->r = right;
	        
		if (right_dist > b) {
			std::list<class Particle*>::iterator pr = right->getPartitionIterator(part, stop);
			buildTree(right, ptrs, part, stop, pr,b);
		}
	        
		right->computeMassMoments(part, stop);
		//right->computeMassMoments(part, stop);
		//std::cout << "end recurse" << std::endl;
	}
	//else {
		//std::cout << "base case" << std::endl;
	//}
}
//*/



void orderParticles(std::vector<class Particle>& ps, std::list<class Particle*>& ptrs) {
	std::vector<class Particle> tmp;
	for (std::list<class Particle*>::iterator p = ptrs.begin(); p != ptrs.end(); p++) tmp.push_back(**p);
	ps.assign(tmp.begin(), tmp.end());
}

//*/


