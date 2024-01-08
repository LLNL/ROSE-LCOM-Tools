package P is
   type My_Class is tagged record
      A : Integer;
   end record;

   procedure Foo (Self : in out My_Class);

   type Derived is new My_Class with record
      B : Integer;
   end record;

   overriding
   procedure Foo (Self : in out Derived);
end P;