class A:
    def __init__(self):
        self.i = 0
        self.j = 0
        self.k = 0

    class B:
        def __init__(self):
            self.x = 0
            self.y = 0
            self.z = 0

        def getdatax(self):
            return self.x

        def getdatay(self):
            return self.y

    def getdatai(self):
        return self.i

    def getdataj(self):
        return self.j
