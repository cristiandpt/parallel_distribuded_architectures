from PIL import Image
import numpy as np
import sys
import os

if len(sys.argv) != 2:
  print("Proveer el nombre de un archivo PNG")
  sys.exit(1)

IMAGE_FILE = sys.argv[1]
FILENAME = os.path.splitext(IMAGE_FILE)[0]
OUTPUT_FILE = f"{FILENAME}.bin"

# Cargar la imagen y convertirla a escala de grises
imagen = Image.open(IMAGE_FILE).convert('L')  # 'L' convierte a escala de grises
height, width = imagen.size
# Convertir la imagen a un array de NumPy
array_imagen = np.array(imagen)

# Guardar el array como un archivo binario
with open(OUTPUT_FILE, 'wb') as f:
    np.int32(width).tofile(f)
    np.int32(height).tofile(f)
    array_imagen.astype('int32').tofile(f)


