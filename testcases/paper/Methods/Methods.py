def outsideMethod():
    print("Outside method called.")

class MyClass:
    def methodOne(self):
        print("Method one called.")

    def methodTwo(self):
        print("Method two called.")
        self.methodOne() # Calls methodOne

    def methodThree(self):
        print("Method three called.")
        outsideMethod() # Calls outside method
