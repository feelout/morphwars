print "boost.py - printing from python"
# teleport order for Boriska (don`t ask me why it`s boost)
class Boost(Order):
    def __init__(self, map_object, map_):
        Order.__init__(self, map_object, map_)
        print "Boost constructor called"
        print map_object.getTile().getX()
	def execute(self, target):
		print "Boost::execute"
		if target.isEnemy(self.target):
			print "Teleport target is occupied by enemy\n"
		else:
			print "Teleporting..."
			self.target.changePosition(target)
		self.done = true
	def process(self):
		print "Boost::process"
		pass
		
class BoostOrderCreator(IOrderCreator):
	def createOrder(self, map_object, map_):
		print "#1"
		def test(map_object, map_):
			return Boost(map_object, map_)
		print "#2"
		obj = test(map_object, map_)
		print "#3"
		return obj

#OrderFactory.getInstance().registerCreator('boost', BoostOrderCreator())
boc = BoostOrderCreator()
OrderFactory.getInstance().registerCreator('boost', boc)
