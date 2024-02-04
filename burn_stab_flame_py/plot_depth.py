import numpy as np
import matplotlib.pyplot as plt

depth_path: str = 'C:/users/gubar/VSProjects/burn_stab_flame/count_front_depth/depth.txt'
par_path: str = 'C:/users/gubar/VSProjects/burn_stab_flame/count_front_depth/par.txt'

depth = np.loadtxt(depth_path)
par = np.loadtxt(par_path)

time_sim: float = float(par)
frames: int = len(depth)
time = np.linspace(0, time_sim, frames)
plt.plot(time, depth)
plt.xlabel('Time')
plt.ylabel('Depth')
plt.title('Front depth')
plt.grid()
plt.show()
