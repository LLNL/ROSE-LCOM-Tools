--  with Ada.Text_IO; use Ada.Text_IO;

package body smalltest is
    procedure Proc is
    begin
        --  Put_Line ("A:" & Integer'Image (A));
        A := Func;
        --  Put_Line ("A:" & Integer'Image (A));
    end Proc;
    function Func return Integer is
    begin
        return 5;
    end Func;
end smalltest;

-- LCOM1: 1
-- LCOM91: 1-0 = 1
-- LCOM2: 1-0 = 1
-- LCOM3: 2
-- LCOM4: 1
-- LCOM5: a=1, l=1 (A2 unused), k=2, (1-2*1)/(1-2*1)=1
