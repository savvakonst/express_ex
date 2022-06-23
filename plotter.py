#!/usr/bin/env python
import numpy
import matplotlib
#matplotlib.use('Agg')
import matplotlib.pyplot as plt

datatype=[('index',numpy.float32), ('floati',numpy.float32),
          ('floatq',numpy.float32)]
datatype=[('index',numpy.float32)]
filename='output.bin'

def main():
    data = numpy.memmap(filename, datatype, 'r')
    print(len(data))
    plt.plot(data['index'])
    plt.show()
    plt.grid(True)
    plt.title("Signal-Diagram")
    plt.xlabel("Sample")
    plt.ylabel("In-Phase")
    #plt.savefig('foo2.png')


main()