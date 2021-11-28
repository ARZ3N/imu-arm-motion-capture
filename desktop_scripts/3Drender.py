import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation
fig = plt.figure()
plt.style.use('fivethirtyeight')
ax = fig.add_subplot(projection='3d')
plt.style.use('fivethirtyeight')
W = 0.0
X = 0.0
Y = 0.0
Z = 0.0


def animate(i):
    data = pd.read_csv('imu.csv')
    W = data['W']
    X = data['X']
    Y = data['Y']
    Z = data['Z']
    
    ax.clear()
    ax.scatter(X, Y, Z, marker='^')
    ax.scatter(0.0, 0.0, 0.0, marker='o', s=150, color='r')
    ax.plot(X, Y, Z, 'y', linewidth=1)
    #ax.quiver(0, 0, 0, X, Y, Z, length=0.06, arrow_length_ratio=0.2, pivot='tail', normalize=True, linewidth=0.8)
    ax.set_xlabel('X ')
    ax.set_ylabel('Y ')
    ax.set_zlabel('Z ')
    #plt.legend(loc='upper left')
    #plt.tight_layout()
    

ani = FuncAnimation(plt.gcf(), animate, interval=1000)

plt.tight_layout()
plt.show()
#del ax.lines