import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

df = pd.read_csv("results4.csv")

params = {
    'axes.titlesize': 16,
    'axes.labelsize': 12,
    'axes.titleweight':'bold'
}

df_list = df[df['type'] == 'list']
df_matrix = df[df['type'] == 'matrix']
df_boost = df[df['type'] == 'boost']

plt.title('A*: List and Matrix time complexity for 45000 edges')
plt.rcParams.update(params)
plt.xlabel('Number of vertices', fontweight='bold')
plt.ylabel('Time in miliseconds', loc='bottom', fontweight='bold')
plt.grid(True)
plt.tight_layout()

def test(x, a, b):
  return a*x*np.log2(x) + b*45000*np.log2(x)

x = df_list['n'].to_numpy(dtype='float64')
y = df_list['a_star_time'].to_numpy(dtype='float64')*1000
param, param_cov = curve_fit(test, x, y)
x_fit = np.linspace(10, 10000, num = 100)
fit = param[0]*x_fit*np.log2(x_fit) + param[1]*45000*np.log2(x_fit)
print(param[0], param[1])
plt.scatter(x, y, marker='o', color='blue', label='a_star_list')
plt.plot(x_fit, fit, marker=',', color='red', label='a_star_list_fit')

x = df_matrix['n'].to_numpy(dtype='float64')
y = df_matrix['a_star_time'].to_numpy(dtype='float64')*1000
param, param_cov = curve_fit(test, x, y)
x_fit = np.linspace(10, 10000, num = 100)
fit = param[0]*x_fit*np.log2(x_fit) + param[1]*45000*np.log2(x_fit)
print(param[0], param[1])
plt.scatter(x, y, marker='o', color='green', label='a_star_matrix')
plt.plot(x_fit, fit, marker=',', color='orange', label='a_star_matrix_fit')
plt.legend()
plt.savefig(f'./figs_vertice_var/a_star.png')



