procedure Show_Protected_Objects is
   protected Obj is
      procedure Set (V : Integer);
      function Get return Integer;
   private
      Local : Integer := 0;
   end Obj;
   protected body Obj is
      procedure Set (V : Integer) is
      begin
         Local := V;
      end Set;
      function Get return Integer is
      begin
         return Local;
      end Get;
   end Obj;
   val : Integer := 1;
begin
   Obj.Set (5);
   val := Obj.Get;
end Show_Protected_Objects;