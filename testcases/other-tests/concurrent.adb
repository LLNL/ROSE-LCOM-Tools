package body concurrent is

  protected body Buffer is
    entry Get(x : out Integer) when head /= tail is
    begin
      x := elems(head);
      head := head + 1; 
    end Get;

    entry Put(x : in Integer) when tail+1 /= head is
    begin
      elems(tail) := x;
      tail := tail + 1;
    end Put; 
  end Buffer;

  protected body Counter is
    procedure inc is
    begin
      i := i + 1;
    end;

    function get return integer is
    begin
      return i;
    end;
  end Counter;

end concurrent;

