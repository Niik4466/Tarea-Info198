import sys
import pandas as pd
import matplotlib.pyplot as plt

def generate_graph(csv_file, graph_path):
    # Leer el archivo CSV
    data = pd.read_csv(csv_file)

    # Crear el gráfico
    plt.figure(figsize=(10, 6))
    for thread in data['Threads'].unique():
        subset = data[data['Threads'] == thread]
        plt.plot(subset['Time'], subset['ExitCode'], label=f'Threads: {thread}')

    plt.xlabel('Time')
    plt.ylabel('Exit Code')
    plt.title('Execution Times and Exit Codes by Thread Count')
    plt.legend()
    plt.grid(True)

    # Guardar el gráfico
    output_file = graph_path + "/execution_graph.png"
    plt.savefig(output_file)
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python Analisador.py <archivo_csv> <graph_path>")
        sys.exit(1)

    csv_file = sys.argv[1]
    graph_path = sys.argv[2]
    generate_graph(csv_file, graph_path)