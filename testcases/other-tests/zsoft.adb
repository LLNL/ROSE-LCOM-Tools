package body ZSoft is
  MAX_DEPT : constant := 99;

  type DeptDatabase is array (UniqueID range 1..MAX_DEPT) of Dept;  
  type PersDatabase is array (UniqueID range MAX_DEPT+1..1000) of Pers;  

  deptdb  : DeptDatabase;
  persdb  : PersDatabase;
  deptctr : UniqueID := 0;
  persctr : UniqueID := MAX_DEPT+1;

  procedure create_department(id : out UniqueID; abbrv : String; head, parent : UniqueID) is
  begin
    deptctr := deptctr + 1;
    id := deptctr;
    deptdb(id) := Dept'(id, abbrv, head, parent); 
  end;

  procedure create_person(id : out UniqueID; dept : UniqueID; name, role : String) is
  begin
    persctr := persctr + 1;
    id := persctr;
    persdb(id) := Pers'(id, dept, name, role); 
  end;

  function element_kind(id : UniqueID) return Kind is
  begin
    return (if id <= MAX_DEPT then Department else Person);
  end;

  procedure get_person(id : UniqueID; dept : out UniqueID; name, role : out String) is
  begin
    dept := persdb(id).dept;
    name := persdb(id).name;
    role := persdb(id).role;
  end;

  procedure get_department(id : UniqueID; abbrv : out String; head, parent : out UniqueID) is
  begin
    abbrv  := deptdb(id).abbrv;
    head   := deptdb(id).head;
    parent := deptdb(id).parent;
  end;
end ZSoft;
