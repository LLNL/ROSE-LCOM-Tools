public class Protected {
    int i, j;
    public synchronized int A() {
        return i;
    }
    public synchronized int B() {
        return j;
    }
}
