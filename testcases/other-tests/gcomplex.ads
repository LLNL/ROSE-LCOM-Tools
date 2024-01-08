generic 

  type Number is private;
  zerorep : Number;
  onerep  : Number;
  with function "+" (left,right: Number) return Number is <>;

package GComplex is  

  type Complex is private;

  procedure clear_imag(x : in out Complex);
  procedure clear_real(x : in out Complex);
  procedure set(x : in out Complex; val : in Complex);

  function "+" (lhs, rhs : in Complex) return Complex;

  I : constant Complex;

private
  type Complex is 
    record
      Re : Number;
      Im : Number;
    end record;

  I : constant Complex := (zerorep, onerep);
end GComplex;
