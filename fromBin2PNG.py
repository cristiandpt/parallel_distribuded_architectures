import numpy as np
from PIL import Image
import sys
import os


if len(sys.argv) == 1:
  print("Dar nombre de archivo")
  sys.exit(1)

INPUT_FILE = sys.argv[1]
FILENAME = os.path.splitext(INPUT_FILE)[0]
OUTPUT_FILE = f"{FILENAME}.PNG"

# Leer el archivo binario y convertirlo a un array de NumPy
# Asegúrate de que estas dimensiones coincidan con las usadas en el programa C
with open(INPUT_FILE, 'rb') as f:
    # Leer ancho y alto
    width = np.fromfile(f, dtype='int32', count=1)[0]
    height = np.fromfile(f, dtype='int32', count=1)[0]
    
    # Leer los datos de los píxeles
    array_imagen = np.fromfile(f, dtype='int32').reshape((height, width))

# Convertir el array a una imagen en escala de grises
imagen = Image.fromarray(array_imagen.astype('uint8'))

# Guardar la imagen en formato PNG
imagen.save(OUTPUT_FILE)

