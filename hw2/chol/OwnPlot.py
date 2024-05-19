import numpy as np
import matplotlib.pyplot as plt
import sys

def load_data(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        version = filename.replace(".m", "").replace("output_", "") 
        data = []
        for line in lines:
            if '[' in line or 'version' in line:
                continue
            elif "];" in line:
                break
            values = line.strip().split()
            data.append([int(values[0]), float(values[1]), float(values[2])])
        return version, np.array(data)

def plot_data(data, version, ax, color):
    x = data[:,0]
    y1 = data[:,1]
    y2 = data[:,2]

    ax.errorbar(x, y1, label=f'{version}', fmt='-o', color = color)  
    return max(y1)
    
def save_plot(ax, plot_name, max_value):
    ax.set_title(f'{plot_name} Performance Comparison')
    ax.set_xlabel('Dimension (m=n=k)')
    ax.set_ylabel('GFLOPS/sec.')
    ax.legend()
    ax.grid(True)
    # plt.show()
    plt.savefig(f"{plot_name}_comparison.png")
    if plot_name == "chol":
        print(f"Max value is: {max_value}")

def plot_error(error_data, plot_name, colors):
    fig, ax = plt.subplots(figsize = (15,9))
    ax.set_title(f'{plot_name} Error Comparison')
    ax.set_xlabel('Dimension (m=n=k)')
    ax.set_ylabel('Error')
    for i, error in enumerate(error_data):
        ax.errorbar(range(1, len(error)+1), error, label=f'Version {i+1}', fmt='-o', color = colors[i])
    
    ax.legend()
    ax.grid(True)
    plt.savefig(f"{plot_name}_error_comparison.png")



def plot_figures(res_name = "chol"):
    fig, ax = plt.subplots(figsize = (15,9))
    plot_name = "chol"
    # colors_linspace_1 = plt.cm.tab20(np.linspace(0, 1, 5))
    colors_linspace_2 = plt.cm.Set3(np.linspace(0, 1, 5))
    colors_linspace_3 = plt.cm.Dark2(np.linspace(0, 1, 5))
    colors = np.concatenate((colors_linspace_2, colors_linspace_3))
    max_value = -float("inf")

    if res_name == "chol":
        # error_data = []
        for n in range(1, 9):
            filename = f"output_{plot_name}_{n}.m"
            version, data = load_data(filename)
            max_value = max(plot_data(data, version, ax, colors[n]), max_value)
            # error_data.append(data[:,2])    
        save_plot(ax, plot_name, max_value)
        # plot_error(error_data, plot_name, colors)

    elif res_name == "loop":
        fig, ax = plt.subplots(figsize = (15,9))
        for i in range(1, 5):
            filename = f"output_{plot_name}_{i}.m"
            version, data = load_data(filename)
            max_value = max(plot_data(data, version, ax, colors_linspace_2[i]), max_value)
        save_plot(ax, "loop_" + plot_name, max_value)

    elif res_name == "vecs":
        fig, ax = plt.subplots(figsize = (15,9))
        for i in range(5, 9):
            filename = f"output_{plot_name}_{i}.m"
            version, data = load_data(filename)
            max_value = max(plot_data(data, version, ax, colors_linspace_3[i - 5]), max_value)  
        save_plot(ax, "vecs_" + plot_name, max_value)    

if __name__ == "__main__":
    if len(sys.argv) == 1:
        plot_figures()
    elif len(sys.argv) == 2:
        plot_figures(sys.argv[1])
