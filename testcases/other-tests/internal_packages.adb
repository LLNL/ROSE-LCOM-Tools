procedure Internal_Packages is
  package greeting is
    procedure helloworld;
    procedure holamundo;
  end greeting;

  package body greeting is
    text : String := "undefined";

    procedure helloworld is
    begin
      text := "Hello World";
    end;

    procedure holamundo is
    begin
      text := "Hola Mundo";
    end;
  end greeting;

  package error is
    procedure Set(val : Integer := 0);
    function Get return Integer;
  end error;

  package body error is
    code : Integer := 0;

    procedure Set(val : Integer := 0) is
    begin
      code := val;
    end;

    function Get return Integer is
    begin 
      return code;
    end;
  end error;
begin
  Greeting.HelloWorld;
  Error.Set(0);
end;
