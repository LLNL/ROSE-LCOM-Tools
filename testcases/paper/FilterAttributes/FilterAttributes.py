class ExternalClass:
    external = None

class FilterAttributes:
    def __init__(self):
        self.i = None
        self.j = None

    def A(self):
        self.i = ExternalClass.external
        return self.i

    def B(self):
        self.j = ExternalClass.external
        return self.j
