package body MultiVariables is

  procedure resetX is
  begin
    varx := 0;
  end;

  procedure resetY is
  begin
    vary := 0;
  end;

  function X return Integer is
  begin
    return varx;
  end;

  function Y return Integer is
  begin
    return vary;
  end;
end MultiVariables;
