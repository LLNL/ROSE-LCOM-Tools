public class FilterMethodCalls {
    int i, j;
    public int A() {
        i = ExternalMethodClass.External();
        return i;
    }
    public int B() {
        j = ExternalMethodClass.External();
        return j;
    }
}

// Analysis should not join these methods together on external methods.