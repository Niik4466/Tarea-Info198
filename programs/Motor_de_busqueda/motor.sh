#!/bin/bash

# Path al archivo INVERTED_INDEX
inverted_index="$1"

# Cerrar todo socket escuchando en 2021
fuser -k 2021/tcp > /dev/null

# Crear un proceso en segundo plano que escuche en el puerto 2021 y redirija la entrada/salida a pipes
coproc socket { nc -lk -p 2021; }
nc_output="${socket[0]}"
nc_input="${socket[1]}"
TOPK=3


# Leer del pipe de entrada
while IFS= read -r message <&"$nc_output"; do
    
    #echo "Mensaje recibido: $message"
    
    # Verifica si el mensaje es "kill" para terminar el proceso
    if [[ "$message" == "SALIR AHORA" ]]; then
        echo "Servidor terminado." >& "$nc_input"
        pkill -f "nc localhost 2021" > /dev/null
        exit
    fi
    # Formateamos el mensaje para ser aceptado como una expresion regular
    message_buscar=$(tr '[:upper:]' '[:lower:]' <<< "$message" | tr " " "|")
    # Realizamos la busqueda. Los resultados tendran el formato (IDx, CantPalabras)\n
    
    cant=$(grep -o '|' <<< "$message_buscar" | wc -l)

    if [[ $cant > 0 ]]; then # Si la palabra a buscar tiene algun or (|), entonces
        result=$(
            grep -w -E "$message_buscar" "$inverted_index" | # Busqueda inicicial
            cut -d ';' -f2- | tr -d '\n' | tr -s ';' '[\n*]' | tr -d '()' | # Formateo de datos
            awk -v cant="$cant" -F ',' '{count[$1]++; data[$1] = data[$1] "\n" $0} END {for (id in count) if (count[id] > cant) printf "%s", data[id]}' | #Interseccion
            awk -F ',' '{sum[$1] += $2} END {for (id in sum) if (id != "") print "("id "," sum[id]")"}' | # Suma de elementos
            sort -t ',' -k2,2nr | head -n "$TOPK" | tr '\n' ';' # Formateo y devolucion de los datos
        )
    else # Si no, entonces es una sola palabra.
        result=$(
            grep -w "$message_buscar" "$inverted_index" | # Busqueda inicial
            cut -d ';' -f2- | tr -s ';' '[\n*]' |  # Formateo de datos
            sort -t ',' -k2,2nr | head -n "$TOPK"| tr '\n' ';' #Ordenamiento y devolucion de datos
        )        
    fi
    echo "$message;$result" >& "$nc_input" # Enviamos el mensaje por el pipe
done

# Cerrar el coproc cuando termine el script
exec {nc_output}>&-
exec {nc_input}>&-
# Cerrar el socket cuando termine el script
fuser -k 2021/tcp > /dev/null
