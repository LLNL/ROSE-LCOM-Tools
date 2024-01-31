public class Constructors {
    int i, j;

    public Constructors() {
        i = 1;
    }

    public void method() {
        j = 2;
    }

    public static void main(String[] args) {
        return;
    }
}

// Constructors don't count: LCOM4 = 1 (+1 if static counts)
// Constructors count:       LCOM4 = 2 (+1 if static counts)