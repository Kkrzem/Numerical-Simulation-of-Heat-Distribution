# Author: Karolin Krzeminski

import matplotlib.pyplot as plt

def read_data(file_path):
    with open(file_path, 'r') as file:
        data = file.readlines()

    sizes = [500000, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000]
    IO_data = {size: [] for size in sizes}
    current_size = None

    for line in data:
        if "Running parallel program on size" in line:
            parts = line.split()
            current_size = int(parts[5])
            print(f"Current size set to: {current_size}")  
        elif "Parallel - " in line and current_size in sizes:
            try:
                IO = float(line.split('I/O Time: ')[1].split(',')[0])
                IO_data[current_size].append(IO)
                print(f"Added IO Time: {IO} for size {current_size}")  
            except ValueError as e:
                print(f"Error parsing line: {line}. Error: {e}")  

    print("Final IO Data:", IO_data)  
    return IO_data

def plot_data(IO_data):
    plt.figure(figsize=(10, 6))

    markers = ['o', 's', '^', 'd', '*', 'x', '+', 'p', 'v']  
    thread_counts = [1, 2, 4, 8, 16, 32, 64]  

    base_time = IO_data[min(IO_data.keys())][0] if IO_data[min(IO_data.keys())] else None

    if base_time:
        ideal_IO = [base_time / x for x in thread_counts]

    for i, (size, IO) in enumerate(IO_data.items()):
        marker = markers[i % len(markers)]
        plt.plot(thread_counts[:len(IO)], IO, label=f'Size {size}', marker=marker)

        # Add data labels
        for x, y in zip(thread_counts, IO):
            plt.text(x, y, f'{y:.5f}', fontsize=10, va='bottom')

    if base_time:
        plt.plot(thread_counts, ideal_IO, label='Ideal', linestyle='--', color='blue')

    plt.xlabel('Number of Threads')
    plt.ylabel('IO Time')
    plt.title('OpenMP IO Time vs. Number of Threads for Different Sizes')
    plt.legend()
    plt.grid(True)
    plt.show()

file_path = 'results.out'
IO_data = read_data(file_path)
plot_data(IO_data)