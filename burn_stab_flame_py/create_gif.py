import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

hold_pos: float = 0
x_upstream_limit: float = 0.7
downstream: float = 1.3

pos_path: str = 'C:/users/gubar/VSProjects/burn_stab_flame/eval/pos-4000-0.786.txt'
temp_path: str = 'C:/users/gubar/VSProjects/burn_stab_flame/eval/temp-4000-0.786.txt'
par_path: str = 'C:/users/gubar/VSProjects/burn_stab_flame/eval/par-4000-0.786.txt'

pos = np.loadtxt(pos_path)
temp = np.loadtxt(temp_path)
par = np.loadtxt(par_path)

y_bott: float = 0
time_start: float = 0

width: float = float(par[0])
time_stop: float = float(par[1])
iter_log: int = int(par[2])
time_step: float = float(par[3])
points: int = int(par[5])
x_stab: float = float(par[6])
temp_stab: float = float(par[7])

dim_step: float = width / points
y_up: float = y_bott + width
time_eval: float = time_stop - time_start
time_points: int = round(time_eval / time_step)
frames: int = time_points // iter_log
y = np.arange(y_bott, y_up, dim_step)
fig, ax = plt.subplots()
front_plot, = plt.plot([], [])
temp_plot, = plt.plot([], [])


def init():
    ax.set_xlim(x_upstream_limit, downstream)
    ax.set_ylim(y_bott, y_up)
    plt.ylabel('y')
    plt.legend(['front', 'temp'])
    plt.grid()
    return front_plot, temp_plot


def update_plot(frame):
    pos_prof = pos[frame, :] / x_stab
    temp_prof = temp[frame, :] / temp_stab

    front_plot.set_data(pos_prof, y)
    temp_plot.set_data(temp_prof, y)

    plt.title('Profiles at ' + str(frame) + ' frame')
    return front_plot


animation = FuncAnimation(fig, update_plot, frames=frames, init_func=init)
animation.save('front.gif', writer='pillow', fps=24)
