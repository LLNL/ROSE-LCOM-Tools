package body Nested is
   function GetD return Integer is
   begin
      return D;
   end GetD;

   package body N is
      function GetX return Integer is
      begin
         return X;
      end GetX;

      function GetFoo return Integer is
      begin
         return Foo;
      end GetFoo;
   end N;

   function GetE return Integer is
   begin
      return E;
   end GetE;

   package body M is
      function GetY return Integer is
      begin
         return Y;
      end GetY;

      function GetBar return Integer is
      begin
         return Bar;
      end GetBar;

      function Monkey return Integer is
      begin
         return Y + D;
      end Monkey;
   end M;

end Nested;