import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

df = pd.read_csv("results3.csv")

params = {
    'axes.titlesize': 16,
    'axes.labelsize': 12,
    'axes.titleweight':'bold'
}

df_list = df[df['type'] == 'list']
df_matrix = df[df['type'] == 'matrix']
df_boost = df[df['type'] == 'boost']

plt.title('A*: List and Matrix time complexity for 1001 vertices')
plt.rcParams.update(params)
plt.xlabel('Number of Edges', fontweight='bold')
plt.ylabel('Time in miliseconds', loc='bottom', fontweight='bold')
plt.grid(True)
plt.tight_layout()

def test(x, a, b):
  return a*x*np.log2(x) + b*45000*np.log2(x)

x_m = df_list['m'].to_numpy(dtype='float64')
x_k = df_list['k'].to_numpy(dtype='float64')
x = x_m + x_k
y = df_list['a_star_time'].to_numpy(dtype='float64')*1000
param, param_cov = curve_fit(test, x, y)
x_fit = np.linspace(0, 200000, num = 100)
fit = param[0]*x_fit*np.log2(1001) + param[1]*1001*np.log2(1001) 
print(param[0], param[1])
plt.scatter(x, y, marker='o', color='blue', label='a_star_list')
plt.plot(x_fit, fit, marker=',', color='red', label='a_star_list_fit')

x_m = df_matrix['m'].to_numpy(dtype='float64')
x_k = df_matrix['k'].to_numpy(dtype='float64')
x = x_m + x_k
y = df_matrix['a_star_time'].to_numpy(dtype='float64')*1000
param, param_cov = curve_fit(test, x, y)
x_fit = np.linspace(0, 200000, num = 100)
fit = param[0]*x_fit*np.log2(1001) + param[1]*1001*np.log2(1001) 
print(param[0], param[1])
plt.scatter(x, y, marker='o', color='green', label='a_star_matrix')
plt.plot(x_fit, fit, marker=',', color='orange', label='a_star_matrix_fit')
plt.legend()
plt.savefig(f'./figs_edge_var/a_star.png')



