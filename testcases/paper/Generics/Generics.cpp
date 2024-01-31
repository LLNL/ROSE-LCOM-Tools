template<typename Number>
class Complex {
private:
    Number re;
    Number im;

public:
    static const Complex<Number> I;

    Complex(Number re = Number{}, Number im = Number{}) : re(re), im(im) {}

    void clear_imag() {
        im = Number{};
    }

    void clear_real() {
        re = Number{};
    }

    void set(const Complex<Number>& val) {
        re = val.re;
        im = val.im;
    }

    Complex operator+(const Complex& rhs) const {
        return Complex(re + rhs.re, im + rhs.im);
    }
};

template<typename Number>
const Complex<Number> Complex<Number>::I(Number{}, Number{1});
