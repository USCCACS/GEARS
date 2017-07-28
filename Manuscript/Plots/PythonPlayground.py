import matplotlib.pyplot as plt
import numpy as np

def getPlotData(filename):
	f = open(filename)
	lines = f.readlines()
	data = [[float(line.split()[0]), float(line.split()[1])] for line in lines ]
	return data
		
list_of_files = [("plot1.plot", "Plot 1"), ("plot2.plot", "Plot 2"), ("plot3.plot", "Plot 3"), ("plot4.plot", "Plot 4")]
data_list = [ (getPlotData(filename), label) for filename, label in list_of_files ]

#for data, label in data_list:
i = 221
for data, label in data_list:
	# Establish Subplot
	plt.subplot(i)	
	i += 1

	# Get values as arrays
	x, y = np.asarray(data)[:,0], np.asarray(data)[:,1]
	m, b = np.polyfit(x, y, 1)

	# Plot Data
	plt.plot( x, y, '.', color='r')
	plt.plot(x, m*x + b, '-', color='y')
	plt.axis((3,15,3,12))
	plt.title(label)
	plt.xlabel("X")
	plt.ylabel("Y", rotation="horizontal")

# Show Plots
plt.tight_layout()
plt.show()