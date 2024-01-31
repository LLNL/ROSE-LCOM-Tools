class Nested:
    def __init__(self):
        self.record1 = SimpleRecord(1, 2)
        self.record2 = SimpleRecord(3, 4)

    def setfield1(self):
        self.record1.field1 = 12

    def setfield2(self):
        self.record1.field2 = 13

    def setrecords(self):
        self.record1 = self.record2

class SimpleRecord:
    def __init__(self, field1=0, field2=0):
        self.field1 = field1
        self.field2 = field2
