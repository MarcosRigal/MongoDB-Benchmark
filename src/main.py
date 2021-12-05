import matplotlib.pyplot as plt


def readFile():
    with open('../bin/reportPath.txt', 'r') as file:
        fileName = file.readline().strip().split('\n')[0]
    with open(fileName, 'r') as file:
        report = {int(k): float(v) for line in file for (k, v)
                  in [line.strip().split('\n')[0].split(",")]}
    return report


report = readFile()
threadsId = sorted(report)
time_spents = [report[threadId] for threadId in threadsId ]

plt.bar(range(len(report)), time_spents, tick_label=range(len(report)))
plt.show()
