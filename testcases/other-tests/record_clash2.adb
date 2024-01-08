package body record_clash2 is
  procedure set_records is
  begin
    record1.fieldA.field1 := 5;
  end set_records;

  procedure set_other_records is
  begin
    record1 := record2;
  end set_other_records;
end record_clash2;
