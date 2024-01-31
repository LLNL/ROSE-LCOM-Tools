package nested

type SimpleRecord struct {
    Field1 int
    Field2 int
}

type Nested struct {
    Record1 SimpleRecord
    Record2 SimpleRecord
}

func NewNested() *Nested {
    return &Nested{
        Record1: SimpleRecord{1, 2},
        Record2: SimpleRecord{3, 4},
    }
}

func (n *Nested) SetField1() {
    n.Record1.Field1 = 12
}

func (n *Nested) SetField2() {
    n.Record1.Field2 = 13
}

func (n *Nested) SetRecords() {
    n.Record1 = n.Record2
}
