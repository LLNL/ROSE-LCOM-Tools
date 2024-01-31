public class Array {
    int[] i = new int[50];
    int[] j = i;

    public void method1() {
        i[1] = 1;
    }

    public void method2() {
        i[2] = 2;
    }

    public void method3() {
        i[3] = 3;
    }

    public void aliasMethod() {
        j[1] = 3; 
    }

    public static void main(String[] args) {
        return;
    }
}

// Alias doesn't count: LCOM4 = 2 or 4 (+1)
// Alias counts:        LCOM4 = 1 or 3 (+1)