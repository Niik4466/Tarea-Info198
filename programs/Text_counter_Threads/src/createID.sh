# Argumentos

extension="$1"
archivos_dir="$2"
mapa_archivo="$3"

# Crear las ID's

count=0
find "$archivos_dir" -maxdepth 1 -name '*'"$extension" -print0 | while IFS= read -r -d '' archivo; do
  archivo=$(basename "$archivo" "$extension")
  echo "$archivo; $count"
  count=$((count+1))
done > "$mapa_archivo/ID$extension"
