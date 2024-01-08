class Class {
public:
  int attribute = 1;
  int Method() {
    return attribute;
  }
};

int main(int argc, char* argv[]) {
  Class classInst;
  classInst.attribute = 0;
  return classInst.Method();
}