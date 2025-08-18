import pandas as pd
import matplotlib.pyplot as plt

# Load CSV file
df = pd.read_csv('output.csv')  # Replace with your actual file path
print(df.columns)


# Plot the two signals vs time
plt.figure(figsize=(10, 5))
plt.plot(df['Time'], df['Sine wave with noise'], label='Sine wave with noise', color='blue')
plt.plot(df['Time'], df['Sine wave filtered'], label='Sine wave filtered', color='orange')

# Formatting

plt.title('IIR Module TEST - Low Pass second order 500 Hz')
plt.xlabel('Time [s]')
plt.ylabel('Amplitude')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.show()