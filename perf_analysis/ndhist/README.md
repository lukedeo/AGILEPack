## ndhist

Lightweight histogram class for filling and saving to HDF5.

Histograms can be arbitrary dimensions (up to 32 can be saved by 
HDF5). The result is saved as a HDF5 DataSet, with attributes 
indicating the binning. Attributes are saved as an array, with the 
Nth entry in the array corresponding to the Nth axis. The following
attributes are saved: 

- `names`: axis names, each axis must have a unique name
- `n_bins`: number of bins in this axis (not including overflow)
- `min`: bottom range of the axis
- `max`: upper range of the axis
- `units`: (optional) string indicating units

If the hist::eat_nan flag is used, Histogram will also record the number
of NaN inputs the binner encounters in the attribute 'nan'. 
There is only one NaN counter for all axes. 
