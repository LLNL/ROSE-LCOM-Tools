procedure Separated.Xchg is
begin
  varx := varx + vary;
  vary := varx - vary;
  varx := varx - vary;  
end;

