-- Deals with the case where a child procedure accesses a parent procedure's attributes.

package body Child is
  procedure Set(val : Integer) is
  begin
    A := val;
  end;
  package body SubChild is
    procedure Set(val : Integer) is
    begin
      A := val;
      B := val;
    end;
  end SubChild;
end Child;
