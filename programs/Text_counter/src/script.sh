#!/bin/bash

# Variables para los argumentos
extension="$1"
input_dir="$2"
output_dir="$3"
trheads="$4"

# Procesar cada archivo encontrado
find "$input_dir" -name "*.$extension" -print0 -maxdepth 1 | while IFS= read -r -d '' file; do

    if [ ! -s "$file" ] || ! grep -q '[[:alpha:]]' "$file"; then
        count='0'
    else
        basename=$(basename "$file" "$extension")
        output_file="$output_dir/${basename}_count.txt"

        words=$(tr '[:upper:]' '[:lower:]' < "$file" | tr -cs '[:alpha:]' '[\n*]' | sort | uniq -c | sort -nr | awk '{print $1 "; " $2}')

        echo -n "$words" > "$output_file"

        count=$(echo "$words" | wc -l)
    fi

    echo "Archivo $file, $count palabras distintas"
done
