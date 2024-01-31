public class Generics<Number> {
    private Number re;
    private Number im;
    private static final Generics<?> I = new Generics<>(null, null); // Placeholder, requires actual zero and one representations

    public Generics(Number re, Number im) {
        this.re = re;
        this.im = im;
    }

    public void clearImag(Addable<Number> zeroRep) {
        this.im = zeroRep.zero();
    }

    public void clearReal(Addable<Number> zeroRep) {
        this.re = zeroRep.zero();
    }

    public void set(Generics<Number> val) {
        this.re = val.re;
        this.im = val.im;
    }

    public Generics<Number> add(Generics<Number> rhs, Addable<Number> adder) {
        return new Generics<>(adder.add(this.re, rhs.re), adder.add(this.im, rhs.im));
    }

    public interface Addable<N> {
        N add(N left, N right);
        N zero();
    }
}
