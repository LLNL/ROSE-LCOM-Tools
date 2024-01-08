package body GComplex is
  procedure clear_imag(x : in out Complex) is
  begin
    x := (x.re, zerorep);
  end;

  procedure clear_real(x : in out Complex) is
  begin
    x := (zerorep, x.im);
  end;

  procedure set(x : in out Complex; val : in Complex) is
  begin
    x := val;
  end;

  function "+"(lhs, rhs : Complex) return Complex is
  begin
    return Complex'(lhs.re + rhs.re, lhs.im + rhs.im);
  end; 

end GComplex;
