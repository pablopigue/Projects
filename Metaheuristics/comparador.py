import os
import pandas as pd

# Configuración de rutas
carpeta_origen1 = "resultados"
carpeta_origen2 = "resultadosConUniformeAleatorio"
carpeta_destino = "diferencia_medias"

os.makedirs(carpeta_destino, exist_ok=True)

# Obtener archivos comunes
archivos_origen1 = [f for f in os.listdir(carpeta_origen1) if 'medias' in f and f.endswith('.csv')]
archivos_origen2 = [f for f in os.listdir(carpeta_origen2) if 'medias' in f and f.endswith('.csv')]
archivos_comunes = set(archivos_origen1) & set(archivos_origen2)

print(f"Archivos a procesar ({len(archivos_comunes)}):")
for archivo in archivos_comunes:
    print(f"- {archivo}")

for archivo in archivos_comunes:
    ruta1 = os.path.join(carpeta_origen1, archivo)
    ruta2 = os.path.join(carpeta_origen2, archivo)
    
    print(f"\nProcesando: {archivo}")
    print(f"Ruta 1: {ruta1}")
    print(f"Ruta 2: {ruta2}")

    try:
        df1 = pd.read_csv(ruta1, sep=";")
        df2 = pd.read_csv(ruta2, sep=";")
        print("Columnas df1:", df1.columns.tolist())
        print("Columnas df2:", df2.columns.tolist())

        if not df1.columns.equals(df2.columns):
            print("¡Error: Columnas no coinciden!")
            continue

        columnas_numericas = df1.select_dtypes(include=['number']).columns
        print("Columnas numéricas detectadas:", columnas_numericas.tolist())

        if len(columnas_numericas) == 0:
            print("¡Advertencia: No se encontraron columnas numéricas!")
            continue

        df_diferencia = df1.copy()
        df_diferencia[columnas_numericas] = df1[columnas_numericas] - df2[columnas_numericas]
        
        ruta_destino = os.path.join(carpeta_destino, f"dif_{archivo}")
        df_diferencia.to_csv(ruta_destino, sep=";", index=False)
        print(f"Guardado en: {ruta_destino}")

    except Exception as e:
        print(f"¡Error al procesar {archivo}: {str(e)}")

print("\n¡Proceso completado!")
