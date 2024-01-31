class Parent {
 public:
  int A = 0;
  void methodA(int val) { A = val; }
  class Child {
   public:
    int B = 2;
    void methodB(int val) { B = val; }
  };
};

// Child methods count:       LCOM4 = 2
// Child methods don't count: LCOM4 = 1