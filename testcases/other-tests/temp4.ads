package Temp4 is
  subtype Number is Integer range -1000 .. 10000;

  function CelsiusToFahrenheit(cels : Number) return Number;
  function FahrenheitToCelsius(fahrenheit : Number) return Number;
  function CelsiusToKelvin(cels : Number) return Number;
  function KelvinToCelsius(kelv : Number) return Number;
  function KelvinToFahrenheit(kelv : Number) return Number;
  function FahrenheitToKelvin(fahrenheit : Number) return Number;
end Temp4;
