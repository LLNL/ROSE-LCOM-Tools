package body nested is
  procedure setfield1 is
  begin
    record1.field1 := 12;
  end setfield1;

  procedure setfield2 is
  begin
    record1.field2 := 13;
  end setfield2;

  procedure setrecords is
  begin
    record1 := record2;
  end setrecords;
end nested;