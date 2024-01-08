package body Body_Attribute4 is
  assign: Integer := 1;
  test: Integer := assign;
  procedure Foo is
  begin
    bar := 2;
  end Foo;
end Body_Attribute4;

