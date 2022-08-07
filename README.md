# NBodySimulationsTests

TL;DR

on ubuntu 22.04 prepare the glad files with:

pip3 install glad
(cd /tmp && glad --out-path gladstuff --generator c)
cp /tmp/gladstuff/src/glad.c ./
mkdir glad
cp /tmp/gladstuff/include/glad/glad.h ./glad/

then install glm with:

sudo apt install libglm-dev

to run the code and see the output type

make run
