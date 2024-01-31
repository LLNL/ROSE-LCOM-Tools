class Static:
    def method(self):
        self.i = 1

    @staticmethod
    def staticMethod(self):
        self.j = 2



# Static don't count: LCOM4 = 1
# Static count:       LCOM4 = 2