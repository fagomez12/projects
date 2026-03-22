import pandas as pd
import matplotlib.pyplot as plt

# Load CSV file
df = pd.read_csv('output.csv')  # Replace with your actual file path
print(df.columns)


# Plot the two signals vs time
plt.figure(figsize=(10, 5))
plt.plot(df['Time'], df['Y'], label='Plant Response', color='red')
plt.plot(df['Time'], df['Step'], label='Step', color='blue')

# Formatting

plt.title('Step Response')
plt.xlabel('Time [s]')
plt.ylabel('Amplitude')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()