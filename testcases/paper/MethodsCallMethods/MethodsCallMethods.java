public class MethodsCallMethods {
    int i, j;
    public int A() {
        return i;
    }
    public int B() {
        A();
        return j;
    }
}
