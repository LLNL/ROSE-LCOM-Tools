class Complex:
    def __init__(self, re, im):
        self.re = re
        self.im = im

    def clear_imag(self, zero_rep):
        self.im = zero_rep

    def clear_real(self, zero_rep):
        self.re = zero_rep

    def set(self, val):
        self.re = val.re
        self.im = val.im

    def __add__(self, other):
        return Complex(self.re + other.re, self.im + other.im)

# Constants I would be defined with actual zero and one representations.
