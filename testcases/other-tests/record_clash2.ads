package record_clash2 is
  procedure set_records;
  procedure set_other_records;
  type SimpleRecord is
    record
      field1 : Integer := 0;
      field2 : Integer := 0;
      field3 : Integer := 0;
    end record;
  type ComplexRecord is
    record
      fieldA : SimpleRecord := (1,2,3);
      fieldB : Integer := 0;
    end record;
  record1 : ComplexRecord := ((4,5,6),7);
  record2 : ComplexRecord := ((8,9,10),11);
end record_clash2;
