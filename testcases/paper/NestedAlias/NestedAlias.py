class NestedAlias:
    def original_method(self):
        self.i = 1
        print("Original method")

    # Create an alias for the original_method
    alias_method = original_method

    def call_alias(self):
        self.j = 2
        self.alias_method()
        pass