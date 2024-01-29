import numpy as np
import matplotlib.pyplot as plt

path: str = 'C:/users/gubar/source/repos/burn_stab_flame/eval/'
m_s: list = ['0.7', '0.71', '0.72', '0.73', '0.74', '0.75', '0.76', '0.77', '0.78', '0.79', '0.8']
time: float = 1000

freq_list: list = []
error: float = 1 / (2 * time)

for m in m_s:
    pos_name: str = 'pos-0-' + m + '.txt'
    par_name: str = 'par-0-' + m + '.txt'
    temp_name: str = 'temp-0-' + m + '.txt'

    pos_path = path + pos_name
    par_path = path + par_name
    temp_path = path + temp_name

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
    freq_list.append(freq)

plt.errorbar(list(map(float, m_s)), freq_list, yerr=error)
plt.xlabel('m')
plt.ylabel('Freq')
plt.title('Frequency')
plt.grid()
plt.show()
