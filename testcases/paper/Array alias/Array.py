class Array:
    i = [0,0,0,0,0]
    k = i

    def method1(self):
        self.i[1] = 1

    def method2(self):
        self.i[2] = 2

    def method3(self):
        self.i[3] = 3

    def aliasMethod(self):
        self.k[1] = 3

# Alias doesn't count: LCOM4 = 2 or 4
# Alias counts:        LCOM4 = 1 or 3