class MethodsCallMethods:
    def __init__(self):
        self.i = 0
        self.j = 0

    def A(self):
        return self.i

    def B(self):
        self.A()
        return self.j

