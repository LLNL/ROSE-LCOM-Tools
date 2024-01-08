package Complex2 is

  type Complex is private;

  procedure clear_imag(x : in out Complex);
  procedure clear_real(x : in out Complex);
  procedure set(x : in out Complex; val : in Complex);

private

  type Complex is 
    record
      Re : Float := 0.0;
      Im : Float := 0.0;
    end record;
  
  I : Complex;

end Complex2;
