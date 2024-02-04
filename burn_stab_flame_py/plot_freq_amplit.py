import numpy as np
import matplotlib.pyplot as plt


def count_ampl(pos_data):
    max_pos: float = np.max(pos_data)
    min_pos: float = np.min(pos_data)
    ampl: float = max_pos - min_pos
    return ampl


def count_aver(var1: float, var2: float):
    aver: float = (var1 + var2) / 2
    return aver


path: str = 'C:/users/gubar/VSProjects/burn_stab_flame/eval/'
m_t_s: list = [('0.7', '0'), ('0.71', '0'), ('0.72', '0'), ('0.73', '0'), ('0.74', '0'), ('0.75', '0'),
               ('0.76', '0'), ('0.77', '0'), ('0.773', '0'), ('0.777', '2000'), ('0.779', '2000'), ('0.78', '2000'),
               ('0.7805', '3000'), ('0.781', '8000'), ('0.782', '3000'), ('0.783', '8000'), ('0.784', '8000'),
               ('0.785', '7000'), ('0.786', '4000'), ('0.787', '6000'), ('0.79', '12000'), ('0.8', '1000')]
m_s = []
time: float = 1000

freq_list: list = []

center_ampl_list: list = []
edge_ampl_list: list = []

error: float = 1 / (2 * time)

for (m, t) in m_t_s:
    pos_name: str = 'pos-' + t + '-' + m + '.txt'
    par_name: str = 'par-' + t + '-' + m + '.txt'

    pos_path = path + pos_name
    par_path = path + par_name

    pos = np.loadtxt(pos_path)
    par = np.loadtxt(par_path)

    points: int = pos.shape[1]
    time_sim: float = float(par[1])

    center_pos = pos[:, points // 2]
    aver_pos = pos[:, points // 4]
    edge_pos = pos[:, 0]

    frames: int = len(center_pos)
    coeffs = np.fft.fft(center_pos)[1: frames // 2]
    freqs = np.fft.fftfreq(frames)[1: frames // 2]
    freq = freqs[np.argmax(abs(coeffs))] * frames / time_sim

    center_ampl: float = count_ampl(center_pos)
    edge_ampl: float = count_ampl(edge_pos)

    freq_list.append(freq)

    center_ampl_list.append(center_ampl)
    edge_ampl_list.append(edge_ampl)

    m_s.append(m)

m_float: list = list(map(float, m_s))
aver_ampl_list: list = list(map(count_aver, center_ampl_list, edge_ampl_list))
figure, (ax1, ax2) = plt.subplots(2, 1)

ax1.errorbar(m_float, freq_list, yerr=error)
ax1.set_xlabel('m')
ax1.set_ylabel('freq')
ax1.set_title('Frequency')
ax1.grid()

ax2.plot(m_float, center_ampl_list)
ax2.plot(m_float, edge_ampl_list)
ax2.plot(m_float, aver_ampl_list)
ax2.set_xlabel('m')
ax2.set_ylabel('ampl')
ax2.set_title('Amplitude')
ax2.legend(['center', 'edge', 'aver'])
ax2.grid()

plt.tight_layout()
plt.show()
