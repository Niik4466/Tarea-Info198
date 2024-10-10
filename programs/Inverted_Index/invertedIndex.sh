# Ordenamos los argumentos
extension="$1"
archivos_dir="$2"
inverted_index_dir="$3"

# Hacemos el for que se encargara de ordenar los elementos
for archivo in $(find "$archivos_dir" -maxdepth 1 -name "*$extension"); do
  # Encontramos el nombre base del archivo (en este caso la ID)
  basename=$(basename "$archivo" "$extension")
  # Imprimimos en el formato de indices invertidos
  awk -v ID="$basename" -F '; ' '{print $2 ";(ID" ID "," $1 ");"}' < "$archivo"
  # Luego del done, tomamos todo lo impreso desde el buffer en el ciclo for y lo transformamos a los indices invertidos
done | awk -F ';' '{key=$1; value=$2; result[key]=result[key] value ";" } END {for (k in result) print k ";" result[k]}' | sort -u > "$inverted_index_dir/$1.INDEX"

echo "$inverted_index_dir/$1.INDEX"
