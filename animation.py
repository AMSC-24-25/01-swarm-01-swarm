from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import pandas as pd
import os
from PIL import Image
import glob
# Creazione delle cartelle per i frame
output_folder = "./animation"
frames_folder = "./frames"
os.makedirs(frames_folder, exist_ok=True)

# Dati della griglia
grid_data = pd.read_csv(os.path.join(output_folder, "grid.csv"))

# File delle particelle
particle_files = sorted(glob.glob(os.path.join(output_folder, "particles_*.csv")))

# Controlla se ci sono file
if not particle_files:
    print("Nessun file di particelle trovato!")
    exit()

# Figura per il grafico 3D
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Genera i frame
for i, particle_file in enumerate(particle_files):
    particles = pd.read_csv(particle_file)

    ax.clear()

    # Calcola i limiti di z
    z_min, z_max = grid_data['z'].min(), grid_data['z'].max()
    if z_min == z_max:
        z_min -= 0.1  # Espandi leggermente i limiti per evitare problemi
        z_max += 0.1

    # Grafico della funzione
    ax.scatter(
        grid_data['x'], grid_data['y'], grid_data['z'],
        c=grid_data['z'], cmap='viridis', s=0.5, alpha=0.3,
        label='Rastrigin Function'
    )

    # Particelle
    ax.scatter(particles['x'], particles['y'], particles['z'], color='red', s=50, label='Particles')

    ax.set_xlim(-512, 512)
    ax.set_ylim(-512, 512)
    ax.set_zlim(z_min, z_max)  # Usa i nuovi limiti calcolati
    ax.set_title(f"PSO - Iteration {i}")
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    # Salva il frame
    frame_filename = os.path.join(frames_folder, f"frame_{i:03d}.png")
    plt.savefig(frame_filename)

print(f"Tutti i frame sono stati salvati nella cartella: {frames_folder}")

frames = sorted(glob.glob(os.path.join(frames_folder, "frame_*.png")))

# Crea una GIF
images = [Image.open(frame) for frame in frames]
images[0].save("animation.gif", save_all=True, append_images=images[1:], duration=500, loop=0)
print("GIF salvata come animation.gif")
