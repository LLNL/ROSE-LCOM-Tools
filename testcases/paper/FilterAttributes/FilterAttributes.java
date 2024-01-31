public class FilterAttributes {
    int i, j;
    public int A() {
        i = ExternalClass.external;
        return i;
    }
    public int B() {
        j = ExternalClass.external;
        return j;
    }
}

// Analysis should not join these methods together on external attributes.