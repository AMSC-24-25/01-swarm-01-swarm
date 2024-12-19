from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import pandas as pd
import os
from PIL import Image
import numpy as np
import glob


output_folder = "./animation"
frames_folder = "./frames"
os.makedirs(frames_folder, exist_ok=True)


def read_function_values(filename):
    x, y, z = [], [], []
    with open(filename, 'r') as file:
        for line in file:
            vals = line.strip().split(',')
            x.append(float(vals[0]))
            y.append(float(vals[1]))
            z.append(float(vals[2]))
    return np.array(x), np.array(y), np.array(z)


def read_positions(filename):
    positions = []
    with open(filename, 'r') as file:
        current_iter = []
        for line in file:
            if line.strip() == "END":
                positions.append(np.array(current_iter))
                current_iter = []
            else:
                x, y = map(float, line.strip().split(','))
                current_iter.append([x, y])
    return positions


x, y, z = read_function_values(os.path.join(output_folder, "function_values.csv"))
positions = read_positions(os.path.join(output_folder, "particle_positions.csv"))


resolution = int(np.sqrt(len(x)))
X, Y = x.reshape((resolution, resolution)), y.reshape((resolution, resolution))
Z = z.reshape((resolution, resolution))


fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')


for i, particle_set in enumerate(positions):
    ax.clear()

    
    z_min, z_max = np.min(Z), np.max(Z)
    if z_min == z_max:
        z_min -= 0.1  
        z_max += 0.1

    
    ax.scatter(X.flatten(), Y.flatten(), Z.flatten(),
               c=Z.flatten(), cmap="viridis", s=0.5, alpha=0.3, label="Objective Function")

    
    particle_x, particle_y = particle_set[:, 0], particle_set[:, 1]
    particle_z = np.array([z[np.argmin((X.flatten() - x)**2 + (Y.flatten() - y)**2)] for x, y in zip(particle_x, particle_y)])
    ax.scatter(particle_x, particle_y, particle_z, color='red', s=50, label='Particles')

    
    ax.set_xlim([np.min(X), np.max(X)])
    ax.set_ylim([np.min(Y), np.max(Y)])
    ax.set_zlim(z_min, z_max)
    ax.set_title(f"PSO - Iteration {i}")
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')

    
    frame_filename = os.path.join(frames_folder, f"frame_{i:03d}.png")
    plt.savefig(frame_filename)

print(f"All the frames have been generated in the folder: {frames_folder}")


frames = sorted(glob.glob(os.path.join(frames_folder, "frame_*.png")))
images = [Image.open(frame) for frame in frames]
images[0].save("pso_animation.gif", save_all=True, append_images=images[1:], duration=250, loop=0)

print("GIF saved as 'pso_animation.gif'")
