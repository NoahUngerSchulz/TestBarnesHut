import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
#probably put functions ad class stuff here
def read(file):
    with open(file) as f:
        lines = f.readlines()
    lines=lines[0].split(",")[1:len(lines)-2]
    lines=[float(w) for w in lines]
    return lines



class UpdateDist:
    def __init__(self, ax):
        self.ax = ax
        #keep this line^^^^
        #make your variables below
        self.X=[]
        self.Y=[]
        self.sim=[]
        more="y"
        while more=="y":
            I="B" #input("what tree\n")
            self.sim+=[I]
            self.sim+=[I+"R"]
            more="n"#input("more?\n")
        self.value="T" #input("what atribute\n")
        self.X+=[sim+"X.txt" for sim in self.sim]
        self.Y+=[sim+self.value+".txt" for sim in self.sim]

        # Set up plot parameters
        #self.ax.set_xlim(-10, 10)
        #self.ax.set_ylim(-10, 10)
        self.ax.grid(True)

    def __call__(self, i):
        #the stuff you want to plot
        #print(read(self.X[0]),read(self.Y[0]))
        print(len(self.X))
        return [ax.plot(read(self.X[n]),read(self.Y[n]),label=self.Y[n]) for n in range(len(self.X))],ax.legend()


fig, ax = plt.subplots()
ud = UpdateDist(ax)
anim = FuncAnimation(fig, ud, frames=100, interval=10000, blit=True)
plt.show()



