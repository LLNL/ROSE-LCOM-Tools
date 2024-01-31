public class Static {
    int i;
    static int j;

    public void method() {
        i = 1;
    }

    public static void main(String[] args) {
        j = 2;
        return;
    }
}

// Static don't count: LCOM4 = 1
// Static count:       LCOM4 = 2