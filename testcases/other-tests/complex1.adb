package body Complex1 is
  procedure clear_imag(x : in out Complex) is
  begin
    x.im := 0.0;
  end;

  procedure clear_real(x : in out Complex) is
  begin
    x.re := 0.0;
  end;
end Complex1;
