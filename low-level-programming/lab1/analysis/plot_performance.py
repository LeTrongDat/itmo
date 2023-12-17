import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

insert_data = pd.read_csv('../build/performance_times.csv')
query_data = pd.read_csv('../build/performance_times.csv')
update_data = pd.read_csv('../build/update_performance.csv')
delete_data = pd.read_csv('../build/delete_performance.csv')

# Apply a rolling mean to smooth the data
window_size = 1000
insert_data['SmoothInsertTime'] = insert_data['InsertTime(ms)'].rolling(window=window_size).mean()
query_data['SmoothQueryTime'] = query_data['QueryTime(ms)'].rolling(window=window_size).mean()
update_data['SmoothUpdateTime'] = update_data['UpdateTime(ms)'].rolling(window=window_size).mean()
delete_data['SmoothDeleteTime'] = delete_data['DeleteTime(ms)'].rolling(window=window_size).mean()

# Create subplots
fig, axs = plt.subplots(2, 2, figsize=(12, 8))

# Plot for insert performance
axs[0, 0].plot(insert_data['Rows'], insert_data['SmoothInsertTime'], color='blue')
axs[0, 0].set_title('Insert Performance')
axs[0, 0].set_xlabel('Number of Rows')
axs[0, 0].set_ylabel('Time (ms)')

# Plot for query performance
axs[0, 1].plot(query_data['Rows'], query_data['SmoothQueryTime'], color='green')
axs[0, 1].set_title('Query Performance')
axs[0, 1].set_xlabel('Number of Rows')
axs[0, 1].set_ylabel('Time (ms)')

# Plot for update performance
axs[1, 0].plot(update_data['Rows'], update_data['SmoothUpdateTime'], color='orange')
axs[1, 0].set_title('Update Performance')
axs[1, 0].set_xlabel('Number of Rows')
axs[1, 0].set_ylabel('Time (ms)')

# Plot for delete performance
axs[1, 1].plot(delete_data['RemainingRows'], delete_data['SmoothDeleteTime'], color='red')
axs[1, 1].set_title('Delete Performance')
axs[1, 1].set_xlabel('Number of Rows')
axs[1, 1].set_ylabel('Time (ms)')

# Adjust layout
plt.tight_layout()

# Show the plot
plt.show()
