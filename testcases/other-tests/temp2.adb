with Debug;

package body Temp2 is
  celsiusBaseInF : constant := 32;
  celsiusBaseInK : constant := 273.15;

  function CelsiusToFahrenheit(cels : Number) return Number is
    res : Number;
  begin
    Debug.RoutineEntry("C2F");
    res := (cels * 9) / 5 + celsiusBaseInF;
    Debug.RoutineExit("C2F");

    return res;
  end;

  function FahrenheitToCelsius(fahrenheit : Number) return Number is
    res : Number;
  begin
    Debug.RoutineEntry("F2C");
    res := (fahrenheit - celsiusBaseInF) * 5 / 9;
    Debug.RoutineExit("F2C");

    return res;
  end;

  function CelsiusToKelvin(cels : Number) return Number is
    res : Number;
  begin
    Debug.RoutineEntry("C2K");
    res := cels + Number(celsiusBaseInK);
    Debug.RoutineExit("C2K");

    return res;
  end;

  function KelvinToCelsius(kelv : Number) return Number is
    res : Number;
  begin
    Debug.RoutineEntry("K2C");
    res := kelv - Number(celsiusBaseInK);    
    Debug.RoutineExit("K2C");

    return res;
  end;

  function KelvinToFahrenheit(kelv : Number) return Number is
    res : Number;
  begin
    Debug.RoutineEntry("K2F");
    res := CelsiusToFahrenheit(KelvinToCelsius(kelv));
    Debug.RoutineExit("K2F");

    return res;
  end;

  function FahrenheitToKelvin(fahrenheit : Number) return Number is
    res : Number;
  begin
    Debug.RoutineEntry("F2K");
    res := CelsiusToKelvin(FahrenheitToCelsius(fahrenheit));
    Debug.RoutineExit("F2K");

    return res;
  end;
end Temp2;
