import numpy as np
import matplotlib.pyplot as plt

hold_pos: float = 0
downstream: float = 15

pos_path: str = 'C:/users/gubar/source/repos/burn_stab_flame/prep_front/pos-0-0.773.txt'
par_path: str = 'C:/users/gubar/source/repos/burn_stab_flame/prep_front/par-0-0.773.txt'

pos = np.loadtxt(pos_path)
par = np.loadtxt(par_path)

y_bott: float = 0

width: float = float(par[0])
points: int = int(par[1])

dim_step: float = width / points
y_up: float = y_bott + width
y = np.arange(y_bott, y_up, dim_step)
plt.plot(pos, y)
plt.xlim(hold_pos, downstream)
plt.xlabel('F')
plt.ylabel('y')
plt.legend(['front'])
plt.title('Initial front')
plt.show()
