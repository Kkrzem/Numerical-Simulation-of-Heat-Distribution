# Author: Karolin Krzeminski
import matplotlib.pyplot as plt

def read_data(file_path):
    with open(file_path, 'r') as file:
        data = file.readlines()

    sizes = [500000, 1000000, 1500000, 2000000, 2500000, 3000000, 3500000, 4000000]
    overall_data = {size: [] for size in sizes}
    current_size = None

    for line in data:
        if "Running parallel program on size" in line:
            parts = line.split()
            current_size = int(parts[5])
            print(f"Current size set to: {current_size}")  
        elif "Parallel -" in line and current_size in sizes:
            try:
                overall = float(line.split('Overall Time: ')[1].split(',')[0])
                overall_data[current_size].append(overall)
                print(f"Added Overall Time: {overall} for size {current_size}")  
            except ValueError as e:
                print(f"Error parsing line: {line}. Error: {e}")  

    print("Final Overall Data:", overall_data)  
    return overall_data

def plot_data(overall_data):
    plt.figure(figsize=(10, 6))

    markers = ['o', 's', '^', 'd', '*', 'x', '+', 'p']  
    thread_counts = [1, 2, 4, 8, 16, 32, 64]  
    
    
    base_time = overall_data[min(overall_data.keys())][0] if overall_data[min(overall_data.keys())] else None

    if base_time:
        ideal_work = [base_time / x for x in thread_counts]

    for i, (size, overall) in enumerate(overall_data.items()):
        marker = markers[i % len(markers)]  
        plt.plot(thread_counts[:len(overall)], overall, label=f'Size {size}', marker=marker)

        
        # for x, y in zip(thread_counts, overall):
        #     plt.text(x, y, f'{y:.4f}', fontsize=10, ha='right', va='bottom')

    if base_time:
        plt.plot(thread_counts, ideal_work, label='Ideal', linestyle='--', color='blue', linewidth=2)
    plt.xlabel('Number of Threads')
    plt.ylabel('Overall Time')
    plt.title('OpenMP Overall Time vs. Number of Threads for Different Sizes')
    plt.legend()
    plt.grid(True)
    plt.show()

file_path = 'results.out'  
overall_data = read_data(file_path)
plot_data(overall_data)



