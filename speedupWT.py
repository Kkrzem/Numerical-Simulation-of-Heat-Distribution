# Author: Karolin Krzeminski

import matplotlib.pyplot as plt

def read_data(file_path):
    with open(file_path, 'r') as file:
        data = file.readlines()

    # change sizes back to big N
    sizes = [500000, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000] 
    speedup_data = {size: [] for size in sizes}
    current_size = None

    for line in data:
        if "Running parallel program on size" in line:
            parts = line.split()
            current_size = int(parts[5])
        elif "Speedup WT" in line and current_size in sizes:
            try:
                speedup_wt = float(line.split('Speedup WT: ')[1].split(',')[0])
                speedup_data[current_size].append(speedup_wt)
            except ValueError as e:
                print(f"Error parsing line: {line}. Error: {e}")
                
    return speedup_data

def print_speedup_data(speedup_data):
    for size, speedups in speedup_data.items():
        print(f"Size {size}:")
        for i, speedup in enumerate(speedups):
            print(f"\tRun {i+1}: Speedup WT = {speedup}")

def plot_data(speedup_data):
    plt.figure(figsize=(10, 6))

    markers = ['o', 's', '^', 'd', '*', 'x', '+', 'p']
    thread_counts = [1, 2, 4, 8, 16, 32, 64]
    extended_thread_counts = list(range(0, 21))  # Extending thread counts to 20

    for i, (size, speedups) in enumerate(speedup_data.items()):
        marker = markers[i % len(markers)]
        plt.plot(thread_counts[:len(speedups)], speedups, label=f'Size {size}', marker=marker, linewidth=2)

    # Ideal line with slope of 1, extending to x=20
    plt.plot(extended_thread_counts, extended_thread_counts, label='Ideal', linestyle='--', color='blue', linewidth=2)

    plt.xlabel('Number of Threads')
    plt.ylabel('Speedup WT')
    plt.title('OpenMP Speedup WT vs. Number of Threads for Different Sizes')
    # plt.xlim(0, max(thread_counts))  # Setting x-axis limit to the maximum of your original thread count
    plt.ylim(0, 5)  # Setting y-axis limit to 20
    plt.legend()
    plt.grid(True)
    plt.show()


file_path = 'results.out'
speedup_data = read_data(file_path)
plot_data(speedup_data)



