package record_clash is
  function compare_records return Boolean;
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
  record1 : SimpleRecord := (0,1,2);
  record2 : SimpleRecord := (0,1,3);
  record3 : ComplexRecord := ((9,8,7),6);
end record_clash;
