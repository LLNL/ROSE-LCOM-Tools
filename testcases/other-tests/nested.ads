package Nested is
   D: Integer;
   function GetD return Integer;

   package N is
      X: Integer;
      function GetX return Integer;
   private
      Foo: Integer;
      function GetFoo return Integer;
   end N;

   E: Integer;
   function GetE return Integer;

private
   package M is
      Y: Integer;
      function GetY return Integer;
   private
      Bar: Integer;
      function GetBar return Integer;
      function Monkey return Integer;
   end M;

end Nested;