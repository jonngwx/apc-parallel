import numpy as np
import matplotlib.pyplot as plt

def plot(filename):
    T = np.loadtxt(filename)
    s = T.shape
    nx = s[0];
    ny = s[1];
    x = np.linspace(0,np.pi,nx);
    y = np.linspace(0,np.pi,ny);
    plt.pcolormesh(x,y,T)
    plt.show()
