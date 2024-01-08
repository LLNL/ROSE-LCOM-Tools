package body Temp1 is
  celsiusBaseInF : constant := 32;
  celsiusBaseInK : constant := 273.15;

  function CelsiusToFahrenheit(cels : Number) return Number is
    res : Number;
  begin
    res := (cels * 9) / 5 + celsiusBaseInF;

    return res;
  end;

  function FahrenheitToCelsius(fahrenheit : Number) return Number is
    res : Number;
  begin
    res := (fahrenheit - celsiusBaseInF) * 5 / 9;

    return res;
  end;

  function CelsiusToKelvin(cels : Number) return Number is
    res : Number;
  begin
    res := cels + Number(celsiusBaseInK);

    return res;
  end;

  function KelvinToCelsius(kelv : Number) return Number is
    res : Number;
  begin
    res := kelv - Number(celsiusBaseInK);    

    return res;
  end;

  function KelvinToFahrenheit(kelv : Number) return Number is
    res : Number;
  begin
    res := CelsiusToFahrenheit(KelvinToCelsius(kelv));

    return res;
  end;

  function FahrenheitToKelvin(fahrenheit : Number) return Number is
    res : Number;
  begin
    res := CelsiusToKelvin(FahrenheitToCelsius(fahrenheit));

    return res;
  end;
end Temp1;
