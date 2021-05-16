import numpy as np
#import tkinter
import matplotlib
#matplotlib.use('tkagg') 
#matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import glob


files = glob.glob("/home/cs261/NVMcache/c_memcached_interface/benchmark_results/*.txt")

results = {}

for file in files:
	f = open(file)

	lines = f.readlines()

	set_mixture = int(file.split("/")[-1][-6:-4])
	print(set_mixture)

	results[set_mixture] = {}
	name = ""
	benchmark = False


	for line in lines:
		if line[0] == "/":
			path = line.split("/")
			version = path[4].strip()
			if version == "memcached-mod":
				version = "memcached-1.6.9"
			name = version +"-" + path[5].split("-")[-1].strip()
			print(name)	
		elif line[0] == "B":
			benchmark = True
		elif benchmark == True:
			#speed = ''.join(filter(str.isdigit, line.split()[-1][0:-3]))
			speed = line.split()[-1][0:-3]
			if speed[0] == 'g':
				speed = 0
			results[set_mixture][name] = float(speed) # float(line.split()[-1][0:-3])
			benchmark = False

print(results)

#plt.bar(results.keys(), results.values())
xs = list(results.keys())
xs.sort()
print(xs)

ys = list(results[xs[0]].keys())


for version in ys:
	y = []
	for x in xs:
		y.append(results[x][version])
	print(xs)
	print(y)
	plt.plot(xs, y, label=version)

print(ys)
plt.legend(fontsize="xx-large")
#plt.xticks(rotation=90)
plt.xlabel("Percentage of SET requests", fontsize=20)
plt.ylabel("Net throughput (M/s)", fontsize=20)
plt.title("Net throughput with 100B values", fontsize=20)
plt.xticks(xs)
plt.tight_layout()
plt.show()
