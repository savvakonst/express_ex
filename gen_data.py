#!/usr/bin/env python
import numpy
import numpy as np

npts=20*2**20
filename='bigdata.bin'

def main():
    data = (numpy.random.uniform(0,1,npts)) + np.sin(np.arange(npts)/float(npts)*20)
    data = data.astype(numpy.float32)

    #data[:,2] = 0.1*data[:,2]+numpy.exp(-((data[:,1]-0.5)**2.)/(0.25**2))
    print(len(data),"|\n")
    print(data)
    data.tofile(filename)#,data.size,data)


if __name__ == "__main__":
    main()