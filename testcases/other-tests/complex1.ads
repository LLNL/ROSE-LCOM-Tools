package Complex1 is

  type Complex is private;

  I : constant Complex;

  procedure clear_imag(x : in out Complex);
  procedure clear_real(x : in out Complex);

private

  type Complex is 
    record
      Re : Float := 0.0;
      Im : Float := 0.0;
    end record;

  I : constant Complex := (0.0, 1.0);

end Complex1;
