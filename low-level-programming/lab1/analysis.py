import matplotlib.pyplot as plt
import pandas as pd

# File paths for the CSV files
csv_files = {
    'addRowPerformance': './build/addRowPerformance.csv',
    'deleteRowsPerformance': './build/deleteRowsPerformance.csv',
    'fileSizeChanges': './build/fileSizeChanges.csv',
    'queryRowsPerformance': './build/queryRowsPerformance.csv',
    'updateRowsPerformance': './build/updateRowsPerformance.csv'
}

# Initialize a figure for subplots
plt.figure(figsize=(7, 7))

# Loop through each CSV file and create a subplot
for i, (key, filename) in enumerate(csv_files.items()):
    # Read the CSV file
    df = pd.read_csv(filename)

    # Create a subplot for each file
    plt.subplot(3, 2, i + 1)
    if key == 'fileSizeChanges':
        plt.plot(df['RowNumber'], df['FileSizeBytes'], label=key)
        plt.ylabel('FileSizeBytes')
    else:
        plt.plot(df['RowNumber'], df['TimeInSeconds'], label=key)
        plt.ylabel('TimeInSeconds')
    
    plt.xlabel('RowNumber')
    plt.title(key)
    plt.legend()

plt.tight_layout()
plt.show()
