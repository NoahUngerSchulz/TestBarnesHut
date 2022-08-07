#include <glad/glad.h> /* must add glad.c */
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <list>


//#include "file.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Geometry.h"

#include <NSim.h> /* Must still add cpp files for library */

const double pi = 3.14159265358979323846;

// function declaration
void glfwInitSetup(void);
GLFWwindow* glfwWindowSetup(void);
void gladInit(void);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



// NSim graphics functions
void printOCTree(class OCTree* T, class Shader& S, unsigned int bBox, unsigned int p, glm::mat4& projection, glm::vec4& color);
void print_octree(class OCTree* T, class Shader& S, unsigned int bBox, unsigned int p,glm::vec3 center,int depth,int index);
void printBranch(class branch* T, class Shader& S, unsigned int bBox, unsigned int p, glm::mat4& projection, glm::vec4& color);
void print_branch(class branch* T, class Shader& S, unsigned int bBox, unsigned int p);
//void printTree(class OCTree* T, class Shader& S, unsigned int bBox, unsigned int p, glm::mat4& projection, glm::vec4& color);
//void print_tree(class OCTree* T, class Shader& S, unsigned int bBox, unsigned int p,glm::vec3 center,int depth,int index);




// constants
#define SCR_WIDTH 1000
#define SCR_HEIGHT 800

// global
float alpha_v = 0.0f;
// camera
Camera C(glm::vec3(0.0f, 0.0f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime;
float lastFrame;

int main() {
    glfwInitSetup();

    GLFWwindow* window = glfwWindowSetup();

    gladInit();

    glEnable(GL_DEPTH_TEST);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum vertex attributes supported: " << nrAttributes << std::endl;

    Shader S1("VertexShader.vs", "FragmentShader.fs");
    Shader S2("VertexSimple.vs", "FragmentSimple.fs");

    Sphere S(.5, 40, 40);

    std::vector<float> vs = S.vertices;
    std::vector<unsigned int> indices = S.indices;

    unsigned int S_VBO, S_VAO, S_EBO;
    glGenVertexArrays(1, &S_VAO);
    glGenBuffers(1, &S_VBO);
    glGenBuffers(1, &S_EBO);

    glBindVertexArray(S_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, S_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vs[0]) * vs.size(), vs.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, S_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    RectangularPrism RP(1, 1, 1, Decomp_type::WIR);

    vs = RP.vertices;
    indices = RP.indices;

    unsigned int RP_VBO, RP_VAO, RP_EBO;
    glGenVertexArrays(1, &RP_VAO);
    glGenBuffers(1, &RP_VBO);
    glGenBuffers(1, &RP_EBO);

    glBindVertexArray(RP_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, RP_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vs[0]) * vs.size(), vs.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RP_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Rectangle R(1, 1, Decomp_type::WIR);

    vs = R.vertices;
    indices = R.indices;

    unsigned int R_VBO, R_VAO, R_EBO;
    glGenVertexArrays(1, &R_VAO);
    glGenBuffers(1, &R_VBO);
    glGenBuffers(1, &R_EBO);

    glBindVertexArray(R_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, R_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vs[0]) * vs.size(), vs.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, R_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::vec4 orange(1.0f, 0.5f, 0.2f, 1.0f);
    glm::vec4 purple(1.0f, 0.0f, 0.5f, 1.0f);
    glm::vec4 green(0.2f, 0.7f, 0.4f, 1.0f);





    
    // NSim
    std::vector<class Particle> ps_tree, ps_direct,ps_branch,psR_tree, psR_direct,psR_branch;
    int numps=10;
    NSim Tsim;
    NSim Dsim;
    NSim Bsim;
    NSim TRsim;
    NSim DRsim;
    NSim BRsim;
    double density=1.0;
    clearstruct(Tsim,"T");
    clearstruct(Dsim,"D");
    clearstruct(Bsim,"B");
    clearstruct(TRsim,"TR");
    clearstruct(DRsim,"DR");
    clearstruct(BRsim,"BR");
    int seed;
    NSim_Init(ps_tree, numps,Tsim,seed,density);
    NSim_Init(ps_direct, numps,Dsim,seed,density);
    NSim_Init(ps_branch,numps,Bsim ,seed,density);
    NSim_Init(psR_tree, numps,TRsim,seed,density);
    NSim_Init(psR_direct, numps,DRsim,seed,density);
    NSim_Init(psR_branch,numps,BRsim ,seed,density);
    std::list<class Particle*> ptrs;
    std::list<class Particle*>::iterator left, right, part;
    double dt = 0.1;
    double before;
    double after;
    int steps=0;
    int cycles=60;
    int b=1;
    double X;




   
    
    lastFrame = static_cast<float>(glfwGetTime());
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
	//std::cout<<"       TIME"<<deltaTime<<std::endl;
        lastFrame = currentFrame;

        //std::cout << 1/deltaTime << std::endl;

        processInput(window);

        // render background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(C.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        S2.set();
        S2.setUniform_Mat4("projection", projection);
        S2.setUniform_Mat4("view", C.GetViewMatrix());







	
        // NSim
	
	steps+=1;
	if (steps>cycles){
	  std::cout<<steps<<"steps"<<std::endl;
	  NSim_accuracy(ps_branch,ps_direct, "B",Bsim);
	  NSim_accuracy(ps_tree,ps_direct, "T",Tsim);
	  NSim_accuracy(ps_direct,ps_direct, "D",Dsim);
	  NSim_accuracy(psR_branch,psR_direct, "BR",BRsim);
	  NSim_accuracy(psR_tree,psR_direct, "TR",TRsim);
	  NSim_accuracy(psR_direct,psR_direct, "DR",DRsim);
	  steps=0;
	  seed+=1;
	  NSim_Init(ps_tree, numps,Tsim,seed,density);
	  NSim_Init(ps_direct, numps,Dsim,seed,density);
	  NSim_Init(ps_branch,numps,Bsim ,seed,density);
	  NSim_Init(psR_tree, numps,TRsim,seed,density);
	  NSim_Init(psR_direct, numps,DRsim,seed,density);
	  NSim_Init(psR_branch,numps,BRsim ,seed,density);
	  if (seed>1){
	    X=1000*dt;
	    Bsim.N=X;
	    Tsim.N=X;
	    Dsim.N=X;
	    BRsim.N=X;
	    TRsim.N=X;
	    DRsim.N=X;
	    seed=1;
	    writegraph(Bsim,"B");
	    writegraph(Tsim,"T");
	    writegraph(Dsim,"D");
	    writegraph(BRsim,"BR");
	    writegraph(TRsim,"TR");
	    writegraph(DRsim,"DR");
	    //std::cout<<"                            DIST"<<distance(Dsim.EnergyY.begin(),Dsim.EnergyY.end());
	    //changing variable
	    //std::cout<<"density"<<density<<std::endl;
	    //density+=.1;
	    //std::cout<<"numps"<<numps<<std::endl;
	    //numps=(int)numps*1.3;
	    //std::cout<<"time passed"<<cycles<<std::endl;
	    //cycles+=10;
	    //std::cout<<"particles per box"<<b<<std::endl;
	    //b+=1;
	    std::cout<<"dt"<<dt<<std::endl;
	    dt+=.05;
	  }
	}
 

	//*/
	/*
	before = static_cast<float>(glfwGetTime());
        
	for (std::vector<class Particle>::iterator p = ps_octree.begin(); p != ps_octree.end(); p++) ptrs.push_back(&*p);
	before = static_cast<float>(glfwGetTime());
        left = ptrs.begin();
        right = ptrs.end();
        OCTree* O = new OCTree(ptrs, left, right,0,0,"octree");
        part = O->Partition(ptrs,left,right,0,0);
	
	left = ptrs.begin();
        right = ptrs.end();
	
	
        buildOCTree(O, ptrs, left, right, part,0,0);
	before = static_cast<float>(glfwGetTime());
        O->computeMassMoments(left, right);;
        orderParticles(ps_octree, ptrs);;
        //printTree(T, S2, RP_VAO, R_VAO, projection, orange);
        NSim_Step(ps_octree, O, dt);	
	after = static_cast<float>(glfwGetTime());
	std::cout<<"         OCTIME "<<after-before<<std::endl;
	delete O;
	ptrs.erase(ptrs.begin(), ptrs.end());
	//*/
	
// NSim


	
	for (std::vector<class Particle>::iterator p = ps_branch.begin(); p != ps_branch.end(); p++) ptrs.push_back(&*p);
	before = static_cast<float>(glfwGetTime());
        left = ptrs.begin();
        right = ptrs.end();
	std::valarray<int> CORNER={0,0,0};
	std::valarray<double> Edges=edges(left,right);
	std::valarray<double>scale={Edges[3]-Edges[0],Edges[4]-Edges[1],Edges[5]-Edges[2]};
	std::valarray<double>center={Edges[0],Edges[1],Edges[2]};
	branch* B = new branch(ptrs, left, right,0,"B",nullptr,center,CORNER,scale);
	B->INIT();
	B->build(ptrs,b);
        NSim_Step(ps_branch, B,dt,0);
	after = static_cast<float>(glfwGetTime());
	//std::cout<<"         BRANCHTIME "<<after-before<<std::endl;
	Bsim.time+=after-before;
	//NSim_accuracy(ps_branch,ps_direct, B,Bsim);
	//printBranch(B, S2, RP_VAO, R_VAO, projection, orange);
	//delete B;
	ptrs.erase(ptrs.begin(), ptrs.end());
	for (std::vector<class Particle>::iterator p = psR_branch.begin(); p != psR_branch.end(); p++) ptrs.push_back(&*p);
	before = static_cast<float>(glfwGetTime());
        left = ptrs.begin();
        right = ptrs.end();
        CORNER={0,0,0};
        Edges=edges(left,right);
        scale={Edges[3]-Edges[0],Edges[4]-Edges[1],Edges[5]-Edges[2]};
        center={Edges[0],Edges[1],Edges[2]};
	branch* BR = new branch(ptrs, left, right,0,"BR",nullptr,center,CORNER,scale);
	BR->INIT();
	BR->build(ptrs,b);
        NSim_Step(psR_branch, BR,dt,0);
	after = static_cast<float>(glfwGetTime());
	//std::cout<<"         BRANCHRTIME "<<after-before<<std::endl;
	BRsim.time+=after-before;
	//NSim_accuracy(ps_branch,ps_direct, B,Bsim);
	//printBranch(BR, S2, RP_VAO, R_VAO, projection, orange);
	//delete B;
	ptrs.erase(ptrs.begin(), ptrs.end());

	


	
	for (std::vector<class Particle>::iterator p = ps_tree.begin(); p != ps_tree.end(); p++) ptrs.push_back(&*p);
	before = static_cast<float>(glfwGetTime());
        left = ptrs.begin();
        right = ptrs.end();
        Tree* T = new Tree(ptrs, left, right,0,0,"T");
        part = T->getPartitionIterator(left,right);
        buildTree(T, ptrs, left, right, part,b);
        T->computeMassMoments(left, right);
        orderParticles(ps_tree, ptrs);
        NSim_Step(ps_tree, T, dt,1);
	after = static_cast<float>(glfwGetTime());
	//std::cout<<"         TREETIME "<<after-before<<std::endl;
	Tsim.time+=after-before;
	//NSim_accuracy(ps_tree,ps_direct, T,Tsim);
	delete T;
	ptrs.erase(ptrs.begin(), ptrs.end());
	//*/
	for (std::vector<class Particle>::iterator p = psR_tree.begin(); p != psR_tree.end(); p++) ptrs.push_back(&*p);
	before = static_cast<float>(glfwGetTime());
        left = ptrs.begin();
        right = ptrs.end();
        Tree* TR = new Tree(ptrs, left, right,0,0,"TR");
        part = TR->getPartitionIterator(left,right);
        buildTree(TR, ptrs, left, right, part,b);
        TR->computeMassMoments(left, right);
        orderParticles(psR_tree, ptrs);
        NSim_Step(psR_tree, TR, dt,1);
	after = static_cast<float>(glfwGetTime());
	//std::cout<<"         TREERTIME "<<after-before<<std::endl;
	TRsim.time+=after-before;
	//NSim_accuracy(ps_tree,ps_direct, T,Tsim);
	delete TR;
	ptrs.erase(ptrs.begin(), ptrs.end());
	//*/
	

	
	for (std::vector<class Particle>::iterator p = ps_direct.begin(); p != ps_direct.end(); p++) ptrs.push_back(&*p);
	before = static_cast<float>(glfwGetTime());
        Tree* D = new Tree(ptrs, left, right,0,0,"D");
        NSim_Step(ps_direct, D,dt,1/10.0);
	delete D;
	after = static_cast<float>(glfwGetTime());
	//std::cout<<"         TIME "<<after-before<<std::endl;
	Dsim.time+=after-before;
	//NSim_accuracy(ps_direct,ps_direct, D,Dsim);
	ptrs.erase(ptrs.begin(), ptrs.end());
	//*/
	
	for (std::vector<class Particle>::iterator p = psR_direct.begin(); p != psR_direct.end(); p++) ptrs.push_back(&*p);
	before = static_cast<float>(glfwGetTime());
        Tree* DR = new Tree(ptrs, left, right,0,0,"DR");
        NSim_Step(psR_direct, DR,dt,1/10.0);
	delete DR;
	after = static_cast<float>(glfwGetTime());
	//std::cout<<"         RTIME "<<after-before<<std::endl;
	DRsim.time+=after-before;
	//NSim_accuracy(ps_direct,ps_direct, D,Dsim);
	ptrs.erase(ptrs.begin(), ptrs.end());
	//*/
	
    
	
	
        
	

	
	
        glBindVertexArray(S_VAO);
        S2.setUniform_Vec4("color", purple);
        for (int i = 0; i < ps_branch.size(); i++) {
            glm::vec3 offset(ps_branch[i].coords[0], ps_branch[i].coords[1], ps_branch[i].coords[2]);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, offset);
            S2.setUniform_Mat4("model", model);
            glDrawElements(GL_TRIANGLES, S.indices.size(), GL_UNSIGNED_INT, 0);
        }
	
        S2.setUniform_Vec4("color", orange);
        for (int i = 0; i < ps_direct.size(); i++) {
            glm::vec3 offset(ps_direct[i].coords[0], ps_direct[i].coords[1], ps_direct[i].coords[2]);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, offset);
            S2.setUniform_Mat4("model", model);
            glDrawElements(GL_TRIANGLES, S.indices.size(), GL_UNSIGNED_INT, 0);
	    }
	S2.setUniform_Vec4("color", green);
        for (int i = 0; i < ps_tree.size(); i++) {
            glm::vec3 offset(ps_tree[i].coords[0], ps_tree[i].coords[1], ps_tree[i].coords[2]);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, offset);
            S2.setUniform_Mat4("model", model);
            glDrawElements(GL_TRIANGLES, S.indices.size(), GL_UNSIGNED_INT, 0);
	    }

        glfwSwapBuffers(window);
        glfwPollEvents();
	
//*/	
    }

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &RP_VAO);
    glDeleteBuffers(1, &RP_VBO);
    glDeleteBuffers(1, &RP_EBO);

    glDeleteVertexArrays(1, &R_VAO);
    glDeleteBuffers(1, &R_VBO);
    glDeleteBuffers(1, &R_EBO);

    glDeleteVertexArrays(1, &S_VAO);
    glDeleteBuffers(1, &S_VBO);
    glDeleteBuffers(1, &S_EBO);

    glfwTerminate();
    return 0;
}

// function implementations

void glfwInitSetup(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 4
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 6
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

GLFWwindow* glfwWindowSetup(void) {
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

void gladInit(void) {
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        alpha_v += .1f;
        if (alpha_v >= 1.0f) alpha_v = 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        alpha_v -= .1f;
        if (alpha_v <= 0.0f) alpha_v = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        C.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        C.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        C.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        C.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    C.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    C.ProcessMouseScroll(static_cast<float>(yoffset));
}










// NSim

void printOCTree(class OCTree* T, class Shader& S, unsigned int bBox, unsigned int p, glm::mat4& projection, glm::vec4& color) {
    // enable shader
    S.set();
    S.setUniform_Mat4("projection", projection);
    S.setUniform_Mat4("view", C.GetViewMatrix());
    S.setUniform_Vec4("color", color);
    print_octree(T, S, bBox, p, glm::vec3(0.0,0.0,12.0),0,0);
}

void print_octree(class OCTree* T, class Shader& S, unsigned int bBox, unsigned int p,glm::vec3 center,int depth,int index) {
    // check if leaf contains only one particle
  if(index>2){
    index=0;
    depth+=1;
  }
  std::cout<<index<<","<<depth<<std::endl;
  glm::mat4 model = glm::mat4(1.0f);
  glm::vec3 s =glm::vec3 (pow(0.5,depth),pow(0.5,depth),pow(0.5,depth));
  glm::vec3 t;
  if (index==0){t=glm::vec3(pow(0.5,depth+2),0,0);
  }else if (index==1) {t=glm::vec3(0,pow(0.5,depth+2),0);
  }else {t=glm::vec3(0,0,pow(0.5,depth+2));}
  glBindVertexArray(bBox);
  model = glm::translate(model, glm::vec3(center));
  model = glm::scale(model, glm::vec3(s));
  S.setUniform_Mat4("model", model);
  if(index==0)glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
  // recurse
  if (T->l != nullptr) print_octree(T->l, S, bBox, p,center+t,depth,index+1);
  if (T->r != nullptr) print_octree(T->r, S, bBox, p,center,depth,index+1);
}


void printBranch(class branch* T, class Shader& S, unsigned int bBox, unsigned int p, glm::mat4& projection, glm::vec4& color) {
    // enable shader
    S.set();
    S.setUniform_Mat4("projection", projection);
    S.setUniform_Mat4("view", C.GetViewMatrix());
    S.setUniform_Vec4("color", color);
    print_branch(T, S, bBox, p);
}

void print_branch(class branch* T, class Shader& S, unsigned int bBox, unsigned int p) {
    // check if leaf contains only one particle
  int depth=T->depth;
  glm::mat4 model = glm::mat4(1.0f);
  std::valarray<double> scale=T->scale;
  //{scale[0]*pow(0.5,depth),scale[1]*pow(0.5,depth),scale[2]*pow(0.5,depth)}
  //std::cout<<"depth"<<depth<<std::endl;
  
  glm::vec3 s =glm::vec3 ({scale[0]*pow(0.5,depth),scale[1]*pow(0.5,depth),scale[2]*pow(0.5,depth)});
  glBindVertexArray(bBox);
  std::valarray<double> center=T->center;
  model = glm::translate(model, glm::vec3({center[0]+s[0]/2,center[1]+s[1]/2,center[2]+s[2]/2}));
  model = glm::scale(model, glm::vec3(s));
  S.setUniform_Mat4("model", model);
  glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
  // recurse
  if (T->b___ != nullptr) print_branch(T->b___, S, bBox, p);
  if (T->b__1 != nullptr) print_branch(T->b__1, S, bBox, p);
  if (T->b_1_ != nullptr) print_branch(T->b_1_, S, bBox, p);
  if (T->b_11 != nullptr) print_branch(T->b_11, S, bBox, p);
  if (T->b1__ != nullptr) print_branch(T->b1__, S, bBox, p);
  if (T->b1_1 != nullptr) print_branch(T->b1_1, S, bBox, p);
  if (T->b11_ != nullptr) print_branch(T->b11_, S, bBox, p);
  if (T->b111 != nullptr) print_branch(T->b111, S, bBox, p);
}



