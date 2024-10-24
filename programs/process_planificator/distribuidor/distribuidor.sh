#!/bin/bash

# Seteamos nuestras variables
message=$1
path_results=$2
path_core=$3
# Limpiamos los paréntesis y separamos los valores
cleaned_message="${message//[()]/}"
IFS=':' read id_core rest <<< "$cleaned_message"
IFS=';' read id_process operation rest <<< "$rest"
IFS=',' read n1 n2 <<< "$rest"

# sleep 0.1

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
    exit 1
    ;;
esac

# Ejecutamos el core y guardamos el mensaje de resultado.
result=$("$path_core/core" "$id_core" "$op" "$n1" "$n2")

echo "$message=>$result" >> "$path_results"

# sleep 0.1

# Borramos el archivo temporal cuando termine de ejecutarse el core
rm "$path_core/.$id_core"
