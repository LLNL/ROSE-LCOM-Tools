package body P is
   procedure Foo (Self : in out My_Class) is
   begin
      Self.A := 1;
   end Foo;

   overriding
   procedure Foo (Self : in out Derived) is
   begin
      Self.A := 2;
      Self.B := 3;
   end Foo;
end P;