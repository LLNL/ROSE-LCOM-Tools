package concurrent is
  N : constant := 8;
  
  type BufPos is mod N;
  type BufArray is array (BufPos) of Integer;
  
  protected type Buffer is
    entry Get(x : out Integer);
    entry Put(x : in Integer);
  private
    elems : BufArray := (others => 0);
    head  : BufPos := BufPos'First;
    tail  : BufPos := BufPos'First;
  end Buffer;

  protected type Counter is
    procedure inc;
    function get return integer;
  private
    i : Integer := 0;
  end Counter;   
end concurrent;

