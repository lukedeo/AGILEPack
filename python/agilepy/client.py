"""
agilepy -- A (currently) simple pythonic thin client for AGILEPack
Author: Luke de Oliveira
email: luke.deoliveira@yale.edu
"""

import yaml
import numpy as np
from numpy.lib import recfunctions

def sigmoid(x):
    '''
    Calculates sigmoid non-linearity 1 / (1 + e^(-x)) for any 'x' such np.exp(x) is defined. 
    '''
    return 1 / (1 + np.exp(-x))


def softmax(x):
    '''
    Induces a discrete probability distribution over an exponentially weighted 'x'. Defined for any 'x' such that np.exp(x) is valid.
    '''
    def _internal(x):
        tmp = np.exp(x)
        return tmp / np.sum(tmp)

    return np.apply_along_axis(_internal, 0, x)

def identity(x):
    '''
    Placeholder for the identity function.
    '''
    return x


def _scale(arr, scaling):
    for key in scaling['mean'].keys():
        arr[key] = (arr[key] - scaling['mean'][key]) / scaling['sd'][key]
    return arr

def _destringify(nrow, ncol, string):
    M = np.zeros(shape=(nrow, ncol))
    data = [float(num) for num in string[(string.find(',') + 1):].split(',')]
    for i in range(0, nrow):
        for j in range(0, ncol):
            M[i, j] = data[i * ncol + j]

    return np.matrix(M)

def _layer_from_yaml(d):
    W = _destringify(d['outputs'], d['inputs'], d['weights'])
    b = _destringify(d['outputs'], 1, d['bias'])

    if d['activation'] == 'sigmoid':
        f = sigmoid
    elif d['activation'] == 'linear':
        f = identity
    elif d['activation'] == 'softmax':
        f = softmax
    else:
        raise LookupError("activation function type \'{}\' not yet supported.".format(d['activation']))
    return [W, b, f]


def _find_bin(point, strategy, abs_val = identity):
    if strategy.__class__ == list:
        nbins = len(strategy)
    else:
        nbins = strategy.shape[0]
    for i in range(1, nbins):
        if abs_val(point) >= strategy[i - 1] and abs_val(point) < strategy[i]:
            return i - 1;
    return -1

def _abs_strip(string):
    return string.replace('abs(', '').replace(')', '')

def generate_bins(data, rule, binning):
    '''
    Creates binned values based on the 'binning' parameter (an array or list).

    Parameters
    ----------

    data: a numpy structured array or record array.
        Input data.
    rule: string
        Rule specifies which variable the binning strategy specified by 'binning' is 
        to be applied to. For a variable 'x_i', a rule can be either 'x_i' or 
        'abs(x_i)', the latter of which will result in absolute value based binning.
    binning: an array (numpy) or a list
        The bin edges for the binning to be used. For example, [1, 3.2, 5.5] could 
        be used.

    Returns:
    --------

    A numpy array with the same shape as 'data', with each entry corresponing to 
    the binning. If an element isn't in the binning range, the method imputes '-1'.

    '''
    if 'abs(' in rule:
        name = _abs_strip(rule)
        return np.array([float(_find_bin(x, binning, np.abs)) for x in data[name]])
    return np.array([float(_find_bin(x, binning)) for x in data[rule]])



class Layer(object):
    '''
    Layer class which will hold W, b, and activation function. Only users planning
    on extending functionality should modify or interact with this class.
    '''
    def __init__(self, parms = [None, None, None]):
        super(Layer, self).__init__()
        self.W, self.b, self.activation = parms
    
    def _set(self, parms):
        self.W, self.b, self.activation = parms

    def _fire(self, M):
        return self.activation(self.W.dot(M) + self.b)
        
class NeuralNet(object):
    '''
    NeuralNet: a class for holding a serialized network estimated using AGILEPack.

    Attrbutes
    ---------

    inputs: list of strings (existance depends on net)
        A list containing all variables used as inputs into the network.

    outputs: list of strings (existance depends on net)
        A list containing all variables used as outputs from the network.

    branches: list of strings (existance depends on net)
        A list containing all branch names used in training.
    
    scaling: a dictionary of dictionaries
        A mapping {mean, sd} to branch names to estimated values.

    Methods
    -------

    load(self, filename): 
        loads a neural network

    predict(self, data): 
        when passed a {record, structured} array, provides predictions.

    apply_binning(self, data):
        Applies the binning specified in the network file loaded to 'data',
        and returns a record array with the appropriate fields appended.


    Usage
    -----
        >>> net = agilepy.NeuralNet()
        >>> net.load('MyNetwork.yaml')
        >>> predictions = net.predict(net.apply_binning(mydata))



    '''
    def __init__(self):
        super(NeuralNet, self).__init__()
        self.has_branches = False
        self.has_inputs = False
        self.has_targets = False
        self.has_scaling = False
        self.has_outputs = False
        self.has_binning = False
        self.branches = None
        self.inputs = None
        self.binning = None
        self.outputs = None
        self.architecture = None

    def load(self, filename):
        '''
        
        '''
        self.has_branches = False
        self.has_inputs = False
        self.has_targets = False
        self.has_scaling = False
        self.has_outputs = False
        self.has_binning = False
        self.branches = None
        self.inputs = None
        self.binning = None
        self.outputs = None
        self.architecture = None
        with open(filename, 'r') as f:
            y = yaml.load(f.read())
        if y.has_key('branches'):
            self.branches = y['branches'].keys()
            self.has_branches = True

        if y['network'].has_key('input_order'):
            self.inputs = y['network']['input_order']
            self.has_inputs = True

        if y['network'].has_key('scaling'):
            self.scaling = y['network']['scaling']
            self.has_scaling = True

        if y.has_key('binning'):
            self.binning = y['binning']
            self.has_binning = True

        if y['network'].has_key('target_order'):
            self.outputs = y['network']['target_order']
            self.has_targets = True

        self.architecture = [Layer(_layer_from_yaml(y['network'][idx])) for idx in y['network']['layer_access']]

    def predict(self, data):
        if self.has_inputs:
            for name in self.inputs:
                if name not in data.dtype.names and 'categ' not in name:
                    raise IndexError('field \'{}\' not found in data passed.'.format(name))
            data = data[self.inputs]
        # if self.has_binning:
            # data = self.apply_binning(data)

        data = data.astype([(k, float) for k in data.dtype.names])

        if self.has_scaling:
            d = _scale(data, self.scaling)
        else:
            d = data
        d = d.view((np.float64, len(d.dtype.names)))
        d = np.matrix(d.T)
        for layer in self.architecture:
            d = layer._fire(d)
        d = d.T
        if self.has_targets:
            dtypes_out = [(name + '_predicted', '<f8') for name in self.outputs]
            return np.core.records.fromarrays(d.T, dtype = dtypes_out)
        return d[0]

    def apply_binning(self, data):
        if not self.has_binning or not self.has_inputs:
            return data
        return recfunctions.append_fields(
            data, ['categ_' + _abs_strip(name) for name in self.binning.keys()], 
            [generate_bins(data, rule, binning) for rule, binning in self.binning.iteritems()], 
            usemask=False, asrecarray=True)















        