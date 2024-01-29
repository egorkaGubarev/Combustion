import numpy as np
import matplotlib.pyplot as plt

pos_path: str = 'C:/users/gubar/source/repos/burn_stab_flame/eval/pos-0-0.785.txt'
par_path: str = 'C:/users/gubar/source/repos/burn_stab_flame/eval/par-0-0.785.txt'
temp_path: str = 'C:/users/gubar/source/repos/burn_stab_flame/eval/temp-0-0.785.txt'

pos = np.loadtxt(pos_path)
par = np.loadtxt(par_path)
temp = np.loadtxt(temp_path)

points: int = pos.shape[1]
time_sim: float = float(par[1])
x_stab: float = float(par[6])
temp_stab: float = float(par[7])

center_pos = pos[:, points // 2]
center_temp = temp[:, points // 2]

center_pos_norm = center_pos / x_stab
center_temp_norm = center_temp / temp_stab

frames: int = len(center_pos)
coeffs = np.fft.fft(center_pos)[1: frames // 2]
freqs = np.fft.fftfreq(frames)[1: frames // 2]
freq = freqs[np.argmax(abs(coeffs))] * frames / time_sim

max_pos: float = np.max(center_pos)
min_pos: float = np.min(center_pos)
ampl: float = max_pos - min_pos

print('Amplitude:', ampl)
print('Frequency:', freq)

time = np.linspace(0, time_sim, frames)
plt.plot(time, center_pos_norm)
plt.plot(time, center_temp_norm)
plt.xlabel('Time')
plt.title('Center')
plt.legend(['pos', 'temp'])
plt.grid()
plt.show()
