#/bin/bash

# Argumentos

extension="$1"
archivos_dir="$2"
salida_dir="$3"
cantidad_threads="$4"
mapa_archivo="$5"
stop_words="$6"

# Crear las ID's
{
count=0
find "$archivos_dir" -maxdepth 1 -name '*'"$extension" -print0 | while IFS= read -r -d '' archivo; do
  archivo=$(basename "$archivo")
  echo "$archivo; $count"
  count=$((count+1))
done
} > "$mapa_archivo/ID$extension"

# Encontrar los archivos y empezar el conteo de palabras en paralelo
find "$archivos_dir" -maxdepth 1 -name '*'"$extension" -print0 | xargs -0 -P "$cantidad_threads" -I {} bash -c './children.sh "$1" "$2" "$3" "$4" "$5"' _ {} "$salida_dir" "$mapa_archivo/ID$extension" "$extension" "$stop_words"
