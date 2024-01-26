import numpy as np
import matplotlib.pyplot as plt

hold_pos: float = 0
downstream: float = 10

pos_path: str = '../../VSProjects/burn_stab_flame/eval/pos.txt'
par_path: str = '../../VSProjects/burn_stab_flame/eval/par.txt'

pos = np.loadtxt(pos_path)
par = np.loadtxt(par_path)

y_bott: float = 0

width: float = float(par[0])
points: int = int(par[5])

dim_step: float = width / points
y_up: float = y_bott + width
y = np.arange(y_bott, y_up, dim_step)
init_prof = pos[0, :]
plt.plot(init_prof, y)
plt.xlim(hold_pos, downstream)
plt.xlabel('F')
plt.ylabel('y')
plt.legend(['front'])
plt.title('Initial front')
plt.show()
