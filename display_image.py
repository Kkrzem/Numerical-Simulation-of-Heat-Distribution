import numpy as np
import matplotlib.pyplot as plt
import sys

def readfromfile(filename):
    with open(filename, 'rb') as file:
        # Read  two integers for metadata (rows and cols)
        metadata = np.fromfile(file, dtype=np.int32, count=2)
        data = np.fromfile(file, dtype=np.float64)  # Read the rest as data
    return metadata, data

def plotdatafromfile(metadata, data, outputfilename):
    rows, cols = metadata
    datareshaped = data.reshape((cols, rows)).T  # Reshape data according to metadata

    plt.figure(figsize=(16, 6))
    plt.title('Temperature of Metal Strip Over Time w/ Contours')
    plt.xlabel('Time index (iterations)')
    plt.ylabel('Metal strip Depth (in)')
    
    # Set contour levels for filled contours
    contour_filled_levels = np.linspace(datareshaped.min(), datareshaped.max(), 100)
    contour = plt.contourf(datareshaped, levels=contour_filled_levels, cmap='bwr')
    cbar = plt.colorbar(contour, ticks=np.arange(0, 1.02, 0.2))
    cbar.set_label('Temperature')
    
    # Set contour levels for line contours
    contour_line_levels = np.arange(0, 1.5, 0.05)
    contour_lines = plt.contour(datareshaped, colors='black', linestyles='solid', levels=contour_line_levels)
    plt.clabel(contour_lines, inline=True, fontsize=8, fmt='%1.2f')

    # Set the x-axis ticks
    plt.xticks(np.arange(0, 10001, 2000))
    
    plt.gca().invert_yaxis()  # Invert the y-axis

    plt.savefig(outputfilename)
    plt.show()

def main(input_filename):
    base_name = input_filename.rsplit('.', 1)[0]
    outputFile = f'{base_name}.png'

    metadata, data = readfromfile(input_filename)
    plotdatafromfile(metadata, data, outputFile)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python display_image.py <input file name>")
        sys.exit(1)

    input_file_name = sys.argv[1]
    main(input_file_name)