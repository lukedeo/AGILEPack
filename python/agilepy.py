import yaml

def sigmoid(x):
	return 1 / (1+ np.exp(-x))

def softmax(x):
	tmp = np.exp(x)
	return tmp / np.sum(tmp)


def _scale(arr, scaling):
	for key in scaling['mean'].keys():
		arr[key] = (arr[key] - scaling['mean'][key]) / scaling['sd'][key]
	return arr


class Layer(object):
	'''Layer class which will hold W, b, and activation function'''
	def __init__(self, W = None, b = None, activation = None):
		super(Layer, self).__init__()
		self.W = W
		self.b = b
		self.activation = activation
	
	def _set(self, W, b, activation):
		self.W, self.b = W, b
		self.activation = activation

	def _fire(self, M):
		return self.activation(self.W * M + b)
		

		


class NeuralNet(object):
	'''
	NeuralNet: a class for holding a serialized network estimated using AGILEPack.
	'''
	def __init__(self):
		super(NeuralNet, self).__init__()
		self.has_branches = False
		self.has_inputs = False
		self.has_scaling = False
		self.has_outputs = False
		self.has_binning = False

	def load(filename):
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





		