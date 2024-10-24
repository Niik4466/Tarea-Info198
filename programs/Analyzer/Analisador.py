import sys
import pandas as pd
import os
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import numpy as np

def generate_graph(csv_file, graph_path):
    # Leer el archivo CSV
    data = pd.read_csv(csv_file)
    
    # Crear un identificador de repetición para cada ocurrencia de Threads
    data['Repeticion'] = data.groupby('Threads').cumcount() + 1
    
    # Crear una figura con dos subplots
    fig = plt.figure(figsize=(15, 10))
    gs = GridSpec(2, 1, height_ratios=[2, 1], hspace=0.3)
    
    # Primer subplot: Líneas individuales para cada repetición
    ax1 = fig.add_subplot(gs[0])
    
    # Obtener valores únicos de threads ordenados
    thread_values = sorted(data['Threads'].unique())
    
    # Graficar líneas individuales para cada repetición
    for rep in range(1, data.groupby('Threads').size().min() + 1):
        rep_data = data[data['Repeticion'] == rep]
        ax1.plot(rep_data['Threads'], rep_data['Time'], 'o-', 
                alpha=0.2, linewidth=1, 
                label=f'Repetición {rep}' if rep == 1 else None)
    
    # Calcular y graficar la media
    mean_by_threads = data.groupby('Threads')['Time'].mean()
    ax1.plot(thread_values, mean_by_threads, 'o-', 
            color='red', linewidth=2, label='Media', 
            zorder=len(thread_values) + 1)
    
    # Configurar escala logarítmica para el eje X
    ax1.set_xscale('log', base=2)
    ax1.grid(True, which="both", ls="--", alpha=0.5)
    ax1.set_xlabel('Número de Threads')
    ax1.set_ylabel('Tiempo de Ejecución (s)')
    ax1.set_title('Análisis de Rendimiento por Número de Threads')
    
    # Mostrar solo "Repetición 1" y "Media" en la leyenda
    handles, labels = ax1.get_legend_handles_labels()
    ax1.legend([handles[0], handles[-1]], ['Repeticiones individuales', 'Media'],
               loc='upper right')
    
    # Mostrar los valores exactos de threads en el eje X
    ax1.set_xticks(thread_values)
    ax1.set_xticklabels(thread_values)
    
    # Segundo subplot: Boxplot
    ax2 = fig.add_subplot(gs[1])
    
    # Crear boxplot con 'labels' en lugar de 'tick_labels'
    bp = ax2.boxplot([data[data['Threads'] == t]['Time'] for t in thread_values],
                     positions=thread_values,
                     labels=thread_values,  # Cambiado de tick_labels a labels
                     patch_artist=True)
    
    # Personalizar el boxplot
    for box in bp['boxes']:
        box.set(facecolor='lightblue', alpha=0.7)
    
    ax2.set_xscale('log', base=2)
    ax2.grid(True, which="both", ls="--", alpha=0.5)
    ax2.set_xlabel('Número de Threads')
    ax2.set_ylabel('Tiempo de Ejecución (s)')
    ax2.set_title('Distribución de Tiempos por Número de Threads')
    
    # Ajustar el espacio entre subplots
    plt.subplots_adjust(hspace=0.3)
    
    # Guardar el gráfico
    csv_filename = os.path.basename(csv_file)
    csv_name, _ = os.path.splitext(csv_filename)
    output_file = os.path.join(graph_path, f"execution_graph_{csv_name}.png")
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    
    # Mostrar el gráfico
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python Analisador.py <csv_file> <graph_path>")
        sys.exit(1)

    csv_file = sys.argv[1]
    graph_path = sys.argv[2]
    generate_graph(csv_file, graph_path)