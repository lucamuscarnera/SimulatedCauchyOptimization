import matplotlib.pyplot as plt
import numpy as np

# leggo il file con i punti
X = np.genfromtxt("../build/X.csv", delimiter = ',')

#leggo i file greedy
P_greedy = np.genfromtxt("../build/permutazioni_greedy.csv", delimiter = ',').astype(int)
L_greedy = np.genfromtxt("../build/loss_permutazioni_greedy.csv", delimiter = ',')

#leggo i file SOC
P_SOC   =  np.genfromtxt("../build/permutazioni.csv", delimiter = ',').astype(int)
L_SOC   =  np.genfromtxt("../build/loss_permutazioni.csv", delimiter = ',')

#classe delle permutazioni
class Permutation:
    def __init__(self, n , swaps = []):
        self.upper = np.arange(n)
        self.lower = np.arange(n)
        for s in swaps:
            tmp = self.lower[s[0]]
            self.lower[s[0]] =self.lower[s[1]]
            self.lower[s[1]] = tmp
    
    def fromArrays(self,upper,lower):
        ret = Permutation(len(upper))
        ret.upper = upper.copy()
        ret.lower = lower.copy()
        return ret
    
    def fromLower(lower):
        ret = Permutation(len(lower))
        
        ret.lower = lower.copy()
        return ret
    
    def apply(self,v):
        ret = v.copy()
        for i in range(len(v)):
            ret[self.upper[i]] = v[self.lower[i]]
        return ret
    def __str__(self):
        return str(self.upper) + "\n" + str(self.lower)
    
    def vicinato(self, returnDirs = False):
        coppie = list()
        for i in range(len(self.lower) - 1):
            for j in range(i+1, len(self.lower)):
                coppie.append( [i,j] )
        ret = list()
        dirs = list()
        for c in coppie:
            direction = Permutation( len(self.lower) ,swaps = [c] ) 
            dirs.append( direction  )
            ret.append( direction + self )
        if returnDirs:
            return ret, dirs
        return ret
    
    def generalizedDerivative(self, f):
        V, direzioni = self.vicinato(True)
        Y = np.array([ f(v) for v in V])
        return direzioni[Y.argmin()]
    
    def __eq__(self,other):
        self_lower = self.lower[self.upper.argsort()]
        other_lower = other.lower[other.upper.argsort()]
        return (self_lower == other_lower).all()
        
        
    def __add__(self,other):
        other_lower = other.lower.copy()
        other_upper = other.upper.copy()
        other_lower = other_lower[other_upper.argsort()]
        return self.fromArrays( self.upper, self.apply(other_lower))
        
        
#visualizzazione

def visualize_travel(X,P, plt = plt):
    newX = P.apply(X)
    plt.scatter(newX[:,0],newX[:,1])
    for q,p in zip(newX[:-1],newX[1:]):
        plt.arrow( q[0], q[1] , p[0] - q[0] , p[1] - q[1] )
    plt.arrow( newX[-1,0], newX[-1,1] , newX[0,0] - newX[-1,0] , newX[0,1] - newX[-1,1] )
        
#animazione

import matplotlib.animation as animation

fig = plt.figure(figsize=(10,5))
axs  = fig.subplots(1,2)

i_greedy = 0
i_soc    = 0

def frame(w):
    global i_soc, i_greedy
    
    axs[0].clear()
    plot = visualize_travel(X,Permutation.fromLower(P_greedy[i_greedy]), axs[0])
    axs[0].set_title(str(i_greedy) + " => " + str(L_greedy[i_greedy]))
    
    axs[1].clear()
    plot = visualize_travel(X,Permutation.fromLower(P_SOC[i_soc]), axs[1])
    axs[1].set_title(str(i_soc) + " => " + str(L_SOC[i_soc]))


    
    i_greedy = np.minimum(  i_greedy + 1  , len(L_greedy) - 1 )
    i_soc    = np.minimum(  i_soc + 1     , len(L_SOC)    - 1 )
    return plot

plt.close()
anim = animation.FuncAnimation( fig, frame, frames = np.maximum( len(P_SOC) , len(P_greedy) ) + 30 , repeat = True)
anim.save("../Visualizer/Test1/travel.gif", fps = 10)

plt.figure()
plt.imshow( P_SOC )
plt.savefig("../Visualizer/Test1/grid.jpg");