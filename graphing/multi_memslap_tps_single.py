import numpy as np
#import tkinter
import matplotlib
#matplotlib.use('tkagg') 
#matplotlib.use('TkAgg')
import matplotlib.pyplot as plt


f = open("/home/cs261/NVMcache/c_memcached_interface/benchmark_results/result_10.txt")

lines = f.readlines()

# print(lines)

name = ""
benchmark = False

results = {}

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
		speed = line.split()[-3]
		if speed[0] == 'g':
			speed = 0
		results[name] = float(speed) # float(line.split()[-1][0:-3])
		benchmark = False
#	print(line)

print(results)

plt.figure(figsize=(10,7))

plt.title("Throughput with 100B Values and 10% SET Requests", fontsize=20)
# plt.xlabel("Memcached Version", fontsize=20)
plt.ylabel("Transactions per second (ops/s)", fontsize=20)
plt.bar(results.keys(), results.values())
plt.xticks(rotation=75, fontsize = 12)
plt.tight_layout()
plt.show()
