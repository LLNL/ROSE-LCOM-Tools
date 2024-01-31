class ExternalClass:
    def External(self):
        return 5

class FilterMethodCalls:
    def __init__(self):
        self.i = None
        self.j = None

    def A(self):
        self.i = ExternalClass.External()
        return self.i

    def B(self):
        self.j = ExternalClass.External()
        return self.j
