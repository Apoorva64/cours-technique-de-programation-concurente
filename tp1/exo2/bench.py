import subprocess
import os
import matplotlib.pyplot as plt
import re
import numpy as np
import csv  # Import the csv module

# Define the padding values
padding_values = [2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]

# List to store execution times
execution_times = []

# Number of times to run the program
num_runs = 5

# Iterate over the padding values
for padding in padding_values:
    # Set the environment variables
    os.environ['USE_STRUCT'] = '1'
    os.environ['PADDING'] = str(padding)

    # Run the cmake and make commands
    subprocess.run(["cmake", "-S", ".", "-B", "out", "-DCMAKE_BUILD_TYPE=Debug"], check=True)
    subprocess.run(["make", "-C", "out"], check=True)

    # List to store execution times for each run
    run_times = []

    # Run the program num_runs times
    for _ in range(num_runs):
        # Execute the The_silent_killer program
        result = subprocess.run(["./out/The_silent_killer"], capture_output=True, text=True)

        # Extract the execution time from the output
        match = re.search(r'Execution time: (\d+\.\d+) seconds', result.stdout)
        if match:
            execution_time = float(match.group(1))
            run_times.append(execution_time)

    # Calculate the average execution time
    avg_time = np.mean(run_times)
    execution_times.append(avg_time)

    # Print the average execution time
    print(f'PADDING: {padding}, AVERAGE TIME: {avg_time}')

# Export the padding values and average execution times to a CSV file
with open('execution_times.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Padding', 'Average Execution Time'])
    for padding, execution_time in zip(padding_values, execution_times):
        writer.writerow([padding, execution_time])

# Plot the execution times
plt.plot(padding_values, execution_times)
plt.xlabel('Padding')
plt.ylabel('Average Execution Time (seconds)')
plt.title('Average Execution Time vs Padding')
plt.yscale('log')  # Set y-axis to logarithmic scale
plt.tight_layout()  # Adjust the plot to fit the figure

# Add tags for each padding value
for i in range(len(padding_values)):
    plt.text(padding_values[i], execution_times[i], f'{padding_values[i]}')

# Save the plot to a file
plt.savefig('execution_times.png', dpi=400)
