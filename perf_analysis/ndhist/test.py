#!/usr/bin/env python2.7 

from h5py import File, Dataset
import numpy as np

f = File('test.h5')
for name, hist in f.iteritems(): 
    print name
    print hist.shape
    if type(hist) == Dataset: 
        a = np.array(hist)
        print a[2,:,:]
    for atname, at in hist.attrs.iteritems(): 
        print atname, at
