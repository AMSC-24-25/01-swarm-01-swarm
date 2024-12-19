import pandas as pd
import matplotlib.pyplot as plt


speedup_data = pd.read_csv("speedup_data.csv")
error_data = pd.read_csv("error_data.csv")


plt.figure(figsize=(10, 6))
for func in speedup_data["function"].unique():
    data_func = speedup_data[(speedup_data["function"] == func) & (speedup_data["iterations"] == 50)]
    data_func = data_func.sort_values(by="particles")
    plt.plot(data_func["particles"], data_func["speedup"], marker="o", label=func)

plt.xlabel("Number of Particles")
plt.ylabel("Speedup")
plt.title("Speedup vs Number of Particles (50 Iterations)")
plt.legend()
plt.grid()
plt.savefig("speedup_vs_particles.png")


plt.figure(figsize=(10, 6))
for func in speedup_data["function"].unique():
    data_func = speedup_data[(speedup_data["function"] == func) & (speedup_data["particles"] == 50)]
    data_func = data_func.sort_values(by="iterations")
    plt.plot(data_func["iterations"], data_func["speedup"], marker="o", label=func)

plt.xlabel("Number of Iterations")
plt.ylabel("Speedup")
plt.title("Speedup vs Number of Iterations (50 Particles)")
plt.legend()
plt.grid()
plt.savefig("speedup_vs_iterations.png")


plt.figure(figsize=(10, 6))
for func in error_data["function"].unique():
    data_func = error_data[error_data["function"] == func]
    plt.plot(data_func["iterations"], data_func["error"], marker="o", label=func)

#plt.yscale("log")  
plt.xlabel("Number of Iterations")
plt.ylabel("Error")
plt.title("Error vs Number of Iterations (50 Particles)")
plt.legend()
plt.grid()
plt.savefig("error_vs_iterations.png")
