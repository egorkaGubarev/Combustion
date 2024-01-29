import numpy as np
import matplotlib.pyplot as plt

path: str = 'C:/users/gubar/source/repos/burn_stab_flame/eval/'
m_t_s: list = [('0.7', '0'), ('0.71', '0'), ('0.72', '0'), ('0.73', '0'), ('0.74', '0'), ('0.75', '0'),
               ('0.76', '0'), ('0.77', '0'), ('0.773', '0'), ('0.777', '2000'), ('0.779', '2000'), ('0.78', '2000'),
               ('0.785', '2000'), ('0.79', '3000'), ('0.8', '1000')]
m_s = []
time: float = 1000

freq_list: list = []
ampl_list: list = []
error: float = 1 / (2 * time)

for (m, t) in m_t_s:
    pos_name: str = 'pos-' + t + '-' + m + '.txt'
    par_name: str = 'par-' + t + '-' + m + '.txt'
    temp_name: str = 'temp-' + t + '-' + m + '.txt'

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

    max_pos: float = np.max(center_pos)
    min_pos: float = np.min(center_pos)
    ampl: float = max_pos - min_pos

    freq_list.append(freq)
    ampl_list.append(ampl)

    m_s.append(m)

m_float: list = list(map(float, m_s))
figure, (ax1, ax2) = plt.subplots(2, 1)

ax1.errorbar(m_float, freq_list, yerr=error)
ax1.set_xlabel('m')
ax1.set_ylabel('freq')
ax1.set_title('Frequency')
ax1.grid()

ax2.plot(m_float, ampl_list)
ax2.set_xlabel('m')
ax2.set_ylabel('ampl')
ax2.set_title('Amplitude')
ax2.grid()

plt.show()
