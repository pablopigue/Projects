import os
import subprocess
import glob
import csv
from statistics import mean
import re

# Parámetros
semillas = [10, 53, 69, 420, 502]
directorio = "datos_MDD"
binario = "./main"
carpeta_resultados = "resultados"

# Función para ordenar naturalmente los nombres de archivo
def natural_sort_key(s):
    return [int(text) if text.isdigit() else text.lower() 
            for text in re.split('([0-9]+)', s)]

# Crear carpeta de resultados si no existe
os.makedirs(carpeta_resultados, exist_ok=True)

# Obtener nombre del algoritmo
nombre_algoritmo = "Algoritmo"
try:
    primer_fichero = sorted(glob.glob(f"{directorio}/GKD-b_*.txt"), key=natural_sort_key)[0]
    resultado = subprocess.run([binario, str(semillas[0]), primer_fichero], 
                             capture_output=True, text=True, timeout=60)
    primera_linea = resultado.stdout.strip().splitlines()[0]
    nombre_algoritmo = primera_linea.split(':')[0].strip() if ':' in primera_linea else primera_linea.strip()
    nombre_algoritmo = nombre_algoritmo.replace(' ', '_')
except Exception as e:
    print(f"⚠️ No se pudo obtener el nombre del algoritmo: {e}")

# Obtener y ordenar ficheros
ficheros = sorted(glob.glob(f"{directorio}/GKD-b_*.txt"), key=natural_sort_key)

# Salidas
resultados_crudos = []
resumen_medias = []

# Verificación
if not os.path.isfile(binario):
    print(f"❌ No se encontró el ejecutable '{binario}'. Asegúrate de haber compilado main.cpp.")
    exit(1)

# Ejecutar pruebas
print(f"\nAlgoritmo usado: {nombre_algoritmo.replace('_', ' ')}")
print(f"Ejecutando pruebas para {len(ficheros)} ficheros...\n")
for idx, fichero in enumerate(ficheros, 1):
    print(f"[{idx}/{len(ficheros)}] Ejecutando: {os.path.basename(fichero)}")
    tiempos, resultados, evaluaciones = [], [], []

    for semilla in semillas:
        try:
            resultado = subprocess.run([binario, str(semilla), fichero], 
                                      capture_output=True, text=True, timeout=60)
            salida = resultado.stdout.strip().splitlines()
            if len(salida) >= 2:
                valores = salida[1].split()
                if len(valores) == 3:
                    tiempo_ms = float(valores[0])
                    fitness = float(valores[1])
                    evaluacion = float(valores[2])
                    resultados_crudos.append([os.path.basename(fichero), tiempo_ms, fitness, evaluacion])
                    tiempos.append(tiempo_ms)
                    resultados.append(fitness)
                    evaluaciones.append(evaluacion)
        except Exception as e:
            print(f"  ⚠️ Error con semilla {semilla}: {e}")

    if tiempos and resultados and evaluaciones:
        media_tiempo = mean(tiempos)
        media_resultado = mean(resultados)
        media_eval = mean(evaluaciones)
        resumen_medias.append([
            os.path.basename(fichero), 
            round(media_tiempo, 3), 
            round(media_resultado, 3), 
            round(media_eval, 3)
        ])

# Nombres de archivos de salida
nombre_crudos = f"resultados_crudos_{nombre_algoritmo}.csv"
nombre_medias = f"resultados_medias_{nombre_algoritmo}.csv"

# Guardar resultados crudos como CSV
with open(os.path.join(carpeta_resultados, nombre_crudos), "w", newline='') as csvfile:
    writer = csv.writer(csvfile, delimiter=';')
    writer.writerow(["Fichero", "Tiempo", "Resultado", "Evaluaciones"])
    writer.writerows(resultados_crudos)

# Guardar medias como CSV
with open(os.path.join(carpeta_resultados, nombre_medias), "w", newline='') as csvfile:
    writer = csv.writer(csvfile, delimiter=';')
    writer.writerow(["Fichero", "Tiempo", "Resultado", "Evaluaciones"])
    writer.writerows(resumen_medias)

print("\n✅ Proceso completado. Archivos generados:")
print(f"- {nombre_crudos}")
print(f"- {nombre_medias}")
print(f"Guardados en la carpeta: '{carpeta_resultados}'")

