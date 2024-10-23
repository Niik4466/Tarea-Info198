#!/bin/bash

# Seteamos nuestras variables
message=$1
path_results=$2
path_core=$3

# Interpretamos el mensaje y asignamos los valores a las variables
read id_core id_process operation n1 n2 <<< $(awk -F '[;,:()]' '{ id_core = $2; id_process=$3; operation=$4; n1=$5; n2=$6; print id_core, id_process, operation, n1, n2 }' <<< "$message")

# Creamos un archivo temporal para permitir la comunicación
echo "" > "$path_core/.$id_core"

case "$operation" in 
  suma)
    op="+"
    ;;
  resta)
    op="-"
    ;;
  multiplicacion)
    op="*"
    ;;
  division)
    op="/"
    ;;
  *)
    echo "Operación no válida: $operation"
    exit 1
    ;;
esac

# Ejecutamos el core y guardamos el mensaje de resultado.
result=$("$path_core/core" "$id_core" "$op" "$n1" "$n2" | awk -v message="$message" '{print message"=>"$1}')

echo "$result" >> "$path_results"

# Borramos el archivo temporal cuando termine de ejecutarse el core
rm "$path_core/.$id_core"
