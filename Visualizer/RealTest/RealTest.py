import numpy as np
import matplotlib.pyplot as plt
datapath = ""
losses   = np.genfromtxt(datapath + "loss.csv")
xs       = np.genfromtxt(datapath + "posizioni_x.csv")

misurazioni = datapath + "misurazioni.csv"

import csv
datafile = open(misurazioni, 'r')
datareader = csv.reader(datafile)
data = []
for row in datareader:
    data.append(list(row))
minimo = float(data[0][0] )
massimo = float(data[0][1] )
data   = data[1:]
data = np.array(data).astype(float)

import matplotlib.animation as animation

x = np.linspace(minimo,massimo,data.shape[1])

plt.plot(x, data.T )
plt.show()

fig = plt.figure(figsize=(10,5))
ax  = fig.subplots(1,1)

i_greedy = 0
i_soc    = 0

def frame(w):
    global i_soc, i_greedy
    
    ax.clear()
    plot = ax.plot(x, data[w])
    
    x_plot =       xs[w]
    y_plot = data[w][np.abs(xs[w] - x).argmin()]
    
    plot = ax.scatter(x_plot,y_plot)
    ax.set_title(w)

    return plot

plt.close()
anim = animation.FuncAnimation( fig, frame, frames = len(data) , repeat = True)
anim.save("./Visualizer/RealTest/real_test.gif", fps = 30)