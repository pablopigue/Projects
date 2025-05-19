#!/bin/bash

# Configuración
DATADIR="datos_MDD"                          # Directorio de archivos de datos
SEMILLAS=(10 53 69 420 502)                  # Semillas a ejecutar
mkdir -p resultados                          # Crear carpeta de resultados

# Archivo de log
LOG_FILE="resultados/log_general.txt"
echo "=== INICIO DE EJECUCIÓN $(date) ===" > "$LOG_FILE"

# Función para clasificar archivos (versión robusta con grep)
clasificar_archivo() {
    local archivo=$1
    if echo "$archivo" | grep -q "_n25_"; then
        echo "resultados/GKD-b_n25.txt"
    elif echo "$archivo" | grep -q "_n50_"; then
        echo "resultados/GKD-b_n50.txt"
    elif echo "$archivo" | grep -q "_n100_"; then
        echo "resultados/GKD-b_n100.txt"
    elif echo "$archivo" | grep -q "_n125_"; then
        echo "resultados/GKD-b_n125.txt"
    elif echo "$archivo" | grep -q "_n150_"; then
        echo "resultados/GKD-b_n150.txt"
    else
        echo "resultados/otros.txt"
    fi
}

# Verificar existencia de archivos clave
echo "Verificando archivos en $DATADIR..." | tee -a "$LOG_FILE"
find "$DATADIR" -name "GKD-b_*" | sort -V | tee -a "$LOG_FILE"

# Procesamiento principal (ordenado numéricamente)
while IFS= read -r -d '' archivo; do
    nombre_archivo=$(basename "$archivo")
    archivo_resultados=$(clasificar_archivo "$nombre_archivo")
    
    echo "Procesando $nombre_archivo -> $archivo_resultados" | tee -a "$LOG_FILE"
    
    # Cabecera del archivo de resultados
    echo "=== Resultados para $nombre_archivo ===" >> "$archivo_resultados"
    echo "Semilla | Resultado" >> "$archivo_resultados"
    echo "-------------------" >> "$archivo_resultados"
    
    # Ejecución para cada semilla
    for semilla in "${SEMILLAS[@]}"; do
        echo "  Semilla $semilla..." | tee -a "$LOG_FILE"
        resultado=$(./main "$semilla" "$archivo")
        echo "$semilla | $resultado" >> "$archivo_resultados"
    done
    
    echo "=====================================" >> "$archivo_resultados"
done < <(find "$DATADIR" -name "GKD-b_*" -print0 | sort -V -z)

# Resumen final
echo "=== RESUMEN ===" | tee -a "$LOG_FILE"
echo "Archivos procesados:" | tee -a "$LOG_FILE"
find resultados/ -type f -name "GKD-b_*.txt" | tee -a "$LOG_FILE"
echo "Proceso completado. Ver $LOG_FILE para detalles."
