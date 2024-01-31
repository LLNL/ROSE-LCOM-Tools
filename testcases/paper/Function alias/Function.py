class Function:
    i = 0
    j = 1

    def method(self):
        return self.i

    aliasMethod = method

    def method2(self):
        self.j = 2
        return self.aliasMethod()


# Function aliases don't count: LCOM4 = 2
# Functions aliases count:      LCOM4 = 1