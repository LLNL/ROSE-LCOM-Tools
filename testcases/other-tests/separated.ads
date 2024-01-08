package Separated is

  procedure resetX;
  procedure resetY;

  function X return Integer;
  function Y return Integer;
  
private
  varx, vary : Integer := 0;  
end Separated;
