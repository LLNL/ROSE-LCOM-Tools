package body c4 is
    procedure m1 is
    begin
        a1 := 1;
        a2 := 2;
    end m1;
    procedure m2 is
    begin
        a1 := 1;
        m3;
    end m2;
    procedure m3 is
    begin
        a3 := 3;
    end m3;
    procedure m4 is
    begin
        a4 := 4;
        m3;
    end m4;
end c4;
