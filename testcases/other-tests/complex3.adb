package body Complex3 is
  procedure clear_imag(x : in out Complex) is
  begin
    x := (x.re, 0.0);
  end;

  procedure clear_real(x : in out Complex) is
  begin
    x := (0.0, x.im);
  end;

  procedure set(x : in out Complex; val : in Complex) is
  begin
    x := val;
  end;

begin
  I := (0.0, 1.0);
end Complex3;
