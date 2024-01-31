class Private:
    def publicMethod(self):
        self.i = 1

    def __privateMethod(self):
        self.j = 2



# Private don't count: LCOM4 = 1
# Private count:       LCOM4 = 2