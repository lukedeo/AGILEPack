def ToAlphanum(string):
	return ''.join(ch for ch in string if ch.isalnum())

class PBS_Script(object):
	'''
	PBS_Script: class that takes a dictionary of parameters 
	and generates a pbs script for the omega supercomputing cluster
	'''
	def __init__(self, executable = None):
		super(PBS_Script, self).__init__()
		self.executable = executable
		self.addtl_args = list()
		self.variadic = {}


	def SetExcecutable(self, executable):
		if not isinstance(executable, str):
			raise TypeError('Must pass the name of an excecutable as a string')
		self.executable = executable

	def AddStaticArg(self, argument):
		if not isinstance(argument, str):
			raise TypeError("Must pass arguments as strings, a la foo.AddStaticArg('--file=myfile.txt')")
		self.addtl_args.append(argument)

	def AddVariadicArg(self, arg_dict):
		if not isinstance(arg_dict, dict):
			raise TypeError("Must pass variadic args as a dictionary")
		self.variadic.update(arg_dict)


	


# for tagger, tagger_specs in taggerdict.iteritems():		






