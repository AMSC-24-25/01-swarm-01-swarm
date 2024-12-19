import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Leggi i dati
data = pd.read_csv("experiment_data.csv")

functions = data["function"].unique()

for func in functions:
    data_func = data[data["function"] == func]

    # Grafico 1: Particelle vs Speedup
    data_particles = data_func[data_func["iterations"] == 50].sort_values(by="particles")
    plt.figure()
    plt.plot(data_particles["particles"], data_particles["speedup"], marker="o")
    plt.xlabel("Numero di particelle")
    plt.ylabel("Speedup")
    plt.title(f"Speedup rispetto al numero di particelle - {func}")
    plt.grid()
    plt.savefig(f"{func}_particles_vs_speedup.png")


    # Grafico 2: Iterazioni vs Speedup
    data_iterations = data_func[data_func["particles"] == 100]
    plt.figure()
    plt.plot(data_iterations["iterations"], data_iterations["speedup"], marker="o")
    plt.xlabel("Numero di iterazioni")
    plt.ylabel("Speedup")
    plt.title(f"Speedup rispetto al numero di iterazioni - {func}")
    plt.grid()
    plt.savefig(f"{func}_iterations_vs_speedup.png")

    # Grafico 3: Grafico 3D
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(data_func["particles"], data_func["iterations"], data_func["speedup"], c=data_func["speedup"], cmap="viridis")
    ax.set_xlabel("Numero di particelle")
    ax.set_ylabel("Numero di iterazioni")
    ax.set_zlabel("Speedup")
    ax.set_title(f"Grafico 3D dello speedup - {func}")
    plt.savefig(f"{func}_3d_speedup.png")
