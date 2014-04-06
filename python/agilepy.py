import yaml

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


		self.branches = y['branches'].keys()

		self.inputs = y['network']['input_order']

		if y['network'].has_key('scaling'):
			self.means, self.sd = y['network']['scaling']['mean'], y['network']['scaling']['sd']



		






		