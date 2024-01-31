class Parent {
    int A = 0;
    public void methodA(int val) {
        A = val;
    }

    class Child {
        int B = 2;
        public void methodB(int val) {
            B = val;
        }
    }

    public static void main(String[] args) {
        return;
    }
}

// Child methods count:       LCOM4 = 2 (+1)
// Child methods don't count: LCOM4 = 1 (+1)