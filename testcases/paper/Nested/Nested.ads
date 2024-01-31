package nested is
  procedure setfield1;
  procedure setfield2;
  procedure setrecords;
  type SimpleRecord is
    record
      field1 : Integer := 0;
      field2 : Integer := 0;
    end record;
  record1 : SimpleRecord := (1,2);
  record2 : SimpleRecord := (3,4);
end nested;