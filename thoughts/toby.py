class MyClass:
    """ An Example class """
    def __init__(self):
        self.data = []
        print "Constructor"

    i = 12345
    def f(self):
        return 'Hello World!'


class bag:
    def __init__(self):
        self.data = []

    def add(self, x):
        self.data.append(x)

    def addtwice(self, x):
        self.add(x)
        self.add(x)

    def echo(self):
        for i in self.data:
            print i


class small_bag(bag):
    pass




b = small_bag()
b.add('Fruit')
b.add('Vegtables')
b.addtwice('Chicken')

b.echo()

