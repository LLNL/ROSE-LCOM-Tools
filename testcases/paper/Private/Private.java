public class Private {
    int i, j;

    public void publicMethod() {
        i = 1;
    }

    private void privateMethod() {
        j = 2;
    }

    public static void main(String[] args) {
        return;
    }
}

// Private don't count: LCOM4 = 1 (+1 if static counts)
// Private count:       LCOM4 = 2 (+1 if static counts)