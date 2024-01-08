package body Complex2 is
  procedure clear_imag(x : in out Complex) is
  begin
    x.im := 0.0;
  end;

  procedure clear_real(x : in out Complex) is
  begin
    x.re := 0.0;
  end;

  procedure set(x : in out Complex; val : in Complex) is
  begin
    x.im := val.im;
    x.re := val.re;
  end;

begin
  I.re := 0.0;
  I.im := 1.0;
end Complex2;
