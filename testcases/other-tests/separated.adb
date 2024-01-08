package body Separated is

  procedure resetX is separate;

  procedure resetY is
  begin
    vary := 0;
  end;

  function X return Integer is separate;

  function Y return Integer is
  begin
    return vary;
  end;
end Separated;
