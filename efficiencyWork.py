# Author: Karolin Krzeminski

import matplotlib.pyplot as plt

def read_data(file_path):
    with open(file_path, 'r') as file:
        data = file.readlines()

    sizes = [500000, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000]
    Efficiency_data = {size: [] for size in sizes}
    current_size = None

    for line in data:
        if "Running parallel program on size" in line:
            parts = line.split()
            current_size = int(parts[5])
        elif "Efficiency OT" in line and current_size in sizes:
            try:
                Efficiency_wt = float(line.split('Efficiency OT: ')[1].split(',')[0])
                Efficiency_data[current_size].append(Efficiency_wt)
            except ValueError as e:
                print(f"Error parsing line: {line}. Error: {e}")
                
        print("Final Efficiency Data:", Efficiency_data)
    return Efficiency_data

def plot_data(Efficiency_data):
    plt.figure(figsize=(10, 6))

    markers = ['o', 's', '^', 'd', '*', 'x', '+', 'p']
    thread_counts = [1, 2, 4, 8, 16, 32, 64]  

    for i, (size, Efficiencys) in enumerate(Efficiency_data.items()):
        marker = markers[i % len(markers)]
        plt.plot(thread_counts[:len(Efficiencys)], Efficiencys, label=f'Size {size}',
                 marker=marker, linewidth=2)

        # Add data labels
        # for x, y in zip(thread_counts, Efficiencys):
        #     plt.text(x, y, f'{y:.4f}', fontsize=10, ha='right', va='bottom')

    
    plt.axhline(y=1.0, color='blue', linestyle='--', label='Ideal')

    plt.xlabel('Number of Threads')
    plt.ylabel('Efficiency WT')
    plt.title('OpenMP Efficiency WT vs. Number of Threads for Different Sizes')
    plt.legend()
    plt.grid(True)
    plt.show()

file_path = 'results.out'
Efficiency_data = read_data(file_path)
plot_data(Efficiency_data)
