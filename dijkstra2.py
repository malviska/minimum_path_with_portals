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

plt.title('Dijkstra: List and Matrix time complexity for 45000 edges')
plt.rcParams.update(params)
plt.xlabel('Number of Vertices', fontweight='bold')
plt.ylabel('Time in miliseconds', loc='bottom', fontweight='bold')
plt.grid(True)
plt.tight_layout()

def test(x, a, b):
  return a*x*np.log2(x) + b*45000*np.log2(x)

x = df_list['n'].to_numpy(dtype='float64')
y = df_list['dijkstra_time'].to_numpy(dtype='float64')*1000
param, param_cov = curve_fit(test, x, y)
x_fit = np.linspace(10, 10000, num = 100)
fit = param[0]*x_fit*np.log2(x_fit) + param[1]*45000*np.log2(x_fit) 
print(param[0], param[1])
plt.scatter(x, y, marker='o', color='blue', label='dijkstra_list')
plt.plot(x_fit, fit, marker=',', color='red', label='dijkstra_list_fit')

def test2(x, a, b):
  return a*np.power(x,2)*np.log2(x) + b*45000*np.log2(x)

x = df_matrix['n'].to_numpy(dtype='float64')
y = df_matrix['dijkstra_time'].to_numpy(dtype='float64')*1000
param, param_cov = curve_fit(test2, x, y)
x_fit = np.linspace(10, 10000, num = 100)
fit = param[0]*np.power(x_fit,2)*np.log2(x_fit) + param[1]*45000*np.log2(x_fit)  
print(param[0], param[1])
plt.scatter(x, y, marker='o', color='green', label='dijkstra_matrix')
plt.plot(x_fit, fit, marker=',', color='orange', label='dijkstra_matrix_fit')
plt.legend()
plt.savefig(f'./figs_vertice_var/dijkstra.png')



