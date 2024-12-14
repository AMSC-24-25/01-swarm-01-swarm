from PIL import Image
import glob
import os
# Percorso dei frame
frames_folder = "./frames"
frames = sorted(glob.glob(os.path.join(frames_folder, "frame_*.png")))

# Crea una GIF
images = [Image.open(frame) for frame in frames]
images[0].save("animation.gif", save_all=True, append_images=images[1:], duration=500, loop=0)
print("GIF salvata come animation.gif")
