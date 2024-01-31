class Extends:
    def __init__(self):
        self.i = 0
        self.j = 0

    def A(self):
        return self.i

    def B(self):
        return self.j

class Extender(Extends):
    def A(self):
        return self.j
