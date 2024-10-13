#!/bin/bash

# Primer argumento archivo a procesar $1
# Segundo argumento directorio de salida $2
# Tercer argumento archivo de las id $3
# Cuarto argumento extension del archivo $4
# Quinto argumento path al archivo stopwords $5

# Obtenemos el nombre del archivo
filename=$(basename "$1" "$4")
id=$(grep -w "$filename" "$3/ID$4" | awk -F '; ' '{print $2}')
echo "el id de $filename es $id"

tr "[:upper:]" "[:lower:]" < "$1" | tr -cs "[:lower:]" "[\n*]" | grep -vxFf "$5" | sort | uniq -c | sort -nr | awk '{print $1 "; " $2}' | tee -p "$2"/"$id$4" | wc -l | awk -v name="$1" '{print name ": " $1 " palabras"}' 