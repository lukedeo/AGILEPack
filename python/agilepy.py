import yaml
import numpy as np
from numpy.lib import recfunctions
def sigmoid(x):
	'''
	Calculates 1 / (1 + e^(-x)) for any 'x' such np.exp(x) is defined. 
	'''
	return 1 / (1+ np.exp(-x))

def softmax(x):
	'''
	Induces a discrete probability distribution over an exponentially weighted 'x'. Defined for any 'x' such that np.exp(x) is valid.
	'''
	tmp = np.exp(x)
	return tmp / np.sum(tmp)

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


def generate_bins(data, rule, binnning):
	


class Layer(object):
	'''
	Layer class which will hold W, b, and activation function
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
	'''
	def __init__(self):
		super(NeuralNet, self).__init__()
		self.has_branches = False
		self.has_inputs = False
		self.has_targets = False
		self.has_scaling = False
		self.has_outputs = False
		self.has_binning = False

	def load(self, filename):
		'''
		
		'''
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
				if name not in data.dtype.names:
					raise IndexError('field \'{}\' not found in data passed.'.format(name))
			data = data[self.inputs]
		
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
			return np.array(d, dtype = dtypes_out)
		return d

	def apply_binning(self, data):
		if not self.has_binning or not self.has_inputs:
			return data

		for name in self.inputs:
			if 'categ_' in name:














		