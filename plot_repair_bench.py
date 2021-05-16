import matplotlib.pyplot as plt


csv = "lru_times.txt"
x_axis = [64*5461, 128*5461, 256*5461, 512*5461, 1024*5461]

file_ = open(csv, "r")
lines = file_.readlines()
file_.close()


y_axis = []
for line in lines :
    y_axis.append(float(line)*1000)

plt.plot(x_axis, y_axis, marker='o')

plt.xlabel('Items in Cache')
plt.ylabel('Time (ms)')

plt.show()