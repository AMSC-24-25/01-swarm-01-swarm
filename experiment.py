import pandas as pd
import matplotlib.pyplot as plt


data = pd.read_csv("results.csv")


plt.figure(figsize=(12, 8))
for function in data["function"].unique():
    subset = data[data["function"] == function]
    plt.plot(subset["n_particles"], subset["error"], marker='o', label=function)

plt.yscale("log")  
plt.title("Error comparison for different functions(Logaritmic scale)")
plt.xlabel("Number of particles")
plt.ylabel("Errore (logaritmic scale)")
plt.grid(True, which="both", linestyle="--", linewidth=0.5)
plt.legend()
plt.savefig("comparison_plot_log.png")
print("Graph saved as 'comparison_plot_log.png'")
