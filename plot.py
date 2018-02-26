import matplotlib.pyplot as plt
import numpy as np

filename = "scores.txt"

data = []

try:
    with open(filename) as f:
        for line in f:
            data.append([int(i) for i in line.split()])
    print('loaded')
except FileNotFoundError:
    print("Wrong file or file path")

print(data[0])

generations = np.arange(0, len(data[0]))
print(len(generations))
print(len(data[0]))
plt.plot(generations, data[0])
plt.show()