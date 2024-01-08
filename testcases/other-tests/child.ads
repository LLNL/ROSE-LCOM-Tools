package Child is
  A : Integer := 0;
  procedure Set(val : Integer);
  package SubChild is
    B : Integer := 2;
    procedure Set(val : Integer);
  end SubChild;
end Child;
