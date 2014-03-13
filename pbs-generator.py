class PBS_Script(object):
	'''
	PBS_Script: class that takes a dictionary of parameters 
	and generates a pbs script for the omega supercomputing cluster
	'''
	def __init__(self, executable = None):
		super(PBS_Script, self).__init__()
		self.executable = executable
		self.addtl_args = list()


	def set_executable(executable):
		if not isinstance(executable, str):
			raise TypeError('Must pass the name of an excecutable as a string')
		self.executable = executable

	def add_static_arg(argument):
		if not isinstance(argument, str):
			raise TypeError("Must pass arguments as strings, a la foo.add_static_arg('--file=myfile.txt')")
		self.addtl_args.append(argument)




		

		