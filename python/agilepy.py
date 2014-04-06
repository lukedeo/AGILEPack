import yaml
import numpy as np

def sigmoid(x):
	return 1 / (1+ np.exp(-x))

def softmax(x):
	tmp = np.exp(x)
	return tmp / np.sum(tmp)

def identity(x):
	return x


def _scale(arr, scaling):
	for key in scaling['mean'].keys():
		arr[key] = (arr[key] - scaling['mean'][key]) / scaling['sd'][key]
	return arr

def _destringify(nrow, ncol, string):
	M = np.zeros(shape=(nrow, ncol))
	data = [float(num) for num in string[(string.find(',') + 1):].split(',')]
	for i in range(0, nrow):
		M[i, :] = data[ (i * ncol) : (i * ncol + ncol)]
	return M

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
		return self.activation(self.W * M + self.b)
		

		


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
		with open(filename, 'r') as f:
			y = yaml.load(f.read())

		if y.has_key('branches'):
			self.branches = y['branches'].keys()
			self.has_branches = True

		if y['network'].has_key('input_order'):
			self.inputs = y['network']['input_order']
			self.has_inputs = True

		if y['network'].has_key('scaling'):
			self.means, self.sd = y['network']['scaling']['mean'], y['network']['scaling']['sd']
			self.has_scaling = True

		if y.has_key('binning'):
			self.binning = y['binning']
			self.has_binning = True

		if y['network'].has_key('target_order'):
			self.outputs = y['network']['target_order']
			self.has_targets = True

		self.architecture = [Layer(_layer_from_yaml(y['network'][idx])) for idx in y['network']['layer_access']]

	def predict(self, data):
		data = data.T
		for layer in self.architecture:
			data = layer._fire(data)
		return data







		