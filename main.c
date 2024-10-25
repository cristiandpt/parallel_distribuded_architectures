
#include <omp.h> // Include OpenMP header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funciones placeholder para la carga y guardado de imágenes
void cargarImagen(int *imagen, int width, int height, FILE *archivo);
void guardarImagen(int *imagen, int width, int height, FILE *archivo);

// Función para aplicar un filtro simple
void aplicarFiltro(int *imagen, int *imagenProcesada, int width, int height);

// Función para calcular la suma de los píxeles (como una estadística)
int calcularSumaPixeles(int *imagen, int width, int height);

char *filename;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Uso: %s <nombre_de_archivo>\n", argv[0]);
    exit(1);
  }

  omp_set_num_threads(16 * 2);

  filename = argv[1];
  int width, height;

  // Abrir el archivo binario para lectura
  FILE *archivo = fopen(filename, "rb");
  if (archivo == NULL) {
    perror("Error al abrir el archivo para cargar la imagen");
    exit(1);
  }

  // Leer ancho y alto desde el inicio del archivo binario
  size_t elementosLeidos = fread(&width, sizeof(int), 1, archivo);
  if (elementosLeidos != 1) {
    perror("Error al leer el ancho de la imagen");
    fclose(archivo);
    exit(1);
  }

  elementosLeidos = fread(&height, sizeof(int), 1, archivo);
  if (elementosLeidos != 1) {
    perror("Error al leer el alto de la imagen");
    fclose(archivo);
    exit(1);
  }

  printf("Dimensiones de la imagen: %d x %d\n", width, height);

  // Asignar memoria basada en dimensiones dinámicas
  int *imagen = (int *)malloc(width * height * sizeof(int));
  int *imagenProcesada = (int *)malloc(width * height * sizeof(int));

  if (imagen == NULL || imagenProcesada == NULL) {
    perror("Error al asignar memoria para las imágenes");
    fclose(archivo);
    exit(1);
  }

  // Cargar la imagen desde el archivo binario
  cargarImagen(imagen, width, height, archivo);
  fclose(archivo);

  // Aplicar filtro (sin paralelismo para pruebas)
  aplicarFiltro(imagen, imagenProcesada, width, height);

  // Calcular suma de píxeles
  int sumaPixeles = calcularSumaPixeles(imagenProcesada, width, height);

  printf("Suma de píxeles: %d\n", sumaPixeles);

  // Guardar la imagen procesada en un nuevo archivo binario
  char output_filename[512];
  snprintf(output_filename, sizeof(output_filename), "%s.new", filename);
  FILE *archivo_salida = fopen(output_filename, "wb");
  if (archivo_salida == NULL) {
    perror("Error al abrir el archivo para guardar la imagen");
    free(imagen);
    free(imagenProcesada);
    exit(1);
  }

  // Escribir ancho y alto en el archivo de salida
  size_t elementosEscritos = fwrite(&width, sizeof(int), 1, archivo_salida);
  if (elementosEscritos != 1) {
    perror("Error al escribir el ancho en el archivo de salida");
    fclose(archivo_salida);
    free(imagen);
    free(imagenProcesada);
    exit(1);
  }

  elementosEscritos = fwrite(&height, sizeof(int), 1, archivo_salida);
  if (elementosEscritos != 1) {
    perror("Error al escribir el alto en el archivo de salida");
    fclose(archivo_salida);
    free(imagen);
    free(imagenProcesada);
    exit(1);
  }

  // Escribir los datos de la imagen procesada
  guardarImagen(imagenProcesada, width, height, archivo_salida);
  fclose(archivo_salida);

  printf("Imagen procesada guardada en %s\n", output_filename);

  free(imagen);
  free(imagenProcesada);
  return 0;
}

// Carga una imagen desde un archivo binario
void cargarImagen(int *imagen, int width, int height, FILE *archivo) {
  size_t elementosLeidos = fread(imagen, sizeof(int), width * height, archivo);
  if (elementosLeidos != width * height) {
    perror("Error al leer la imagen desde el archivo");
    fclose(archivo);
    free(imagen);
    exit(1);
  }
}

// Guarda una imagen en un archivo binario
void guardarImagen(int *imagen, int width, int height, FILE *archivo) {
  size_t elementosEscritos =
      fwrite(imagen, sizeof(int), width * height, archivo);
  if (elementosEscritos != width * height) {
    perror("Error al escribir la imagen en el archivo");
    fclose(archivo);
    exit(1);
  }
}

// Paralelizar esta función con OpenMP
void aplicarFiltro(int *imagen, int *imagenProcesada, int width, int height) {
  int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

  // Puesto que un for el cual tiene un complejidad muy alta (3 nidados) se opta
  // por dividir cada uno en independiente y asi se baja exponente en la
  // complejidad
#pragma omp parallel for shared(imagen, Gx, Gy, width, height, imagenProcesada)
  for (int y = 1; y < height - 1; y++) {
    for (int x = 1; x < width - 1; x++) {
      int sumX = 0;
      int sumY = 0;

      // Aplicar máscaras de Sobel (Gx y Gy)
      for (int ky = -1; ky <= 1; ky++) {
        for (int kx = -1; kx <= 1; kx++) {
          sumX += imagen[(y + ky) * width + (x + kx)] * Gx[ky + 1][kx + 1];
          sumY += imagen[(y + ky) * width + (x + kx)] * Gy[ky + 1][kx + 1];
        }
      }

      // Calcular magnitud del gradiente
      int magnitude = abs(sumX) + abs(sumY);
      imagenProcesada[y * width + x] =
          (magnitude > 255) ? 255 : magnitude; // Normalizar a 8 bits
    }
  }
}

// Paralelizar esta función con OpenMP
int calcularSumaPixeles(int *imagen, int width, int height) {
  int suma = 0;

// Una reducción en paralelo para acumular la suma sin condiciones de carrera y
// procesada individualamente
#pragma omp parallel for reduction(+ : suma)
  for (int i = 0; i < width * height; i++) {
    suma += imagen[i];
  }

  return suma;
}
