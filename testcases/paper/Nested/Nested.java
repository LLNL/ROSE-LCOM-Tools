public class Nested {
    public class SimpleRecord {
        public int field1 = 0;
        public int field2 = 0;
    }

    SimpleRecord record1 = new SimpleRecord();
    SimpleRecord record2 = new SimpleRecord();

    public void setField1() {
        record1.field1 = 1;
    }

    public void setField2() {
        record1.field2 = 1;
    }

    public void setRecords() {
        record1 = record2;
    }
}