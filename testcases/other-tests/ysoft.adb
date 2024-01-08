package body YSoft is
  type Database is array (UniqueID range 1..100) of Element;  

  db  : Database;
  ctr : UniqueID := 0;

  procedure create_department(id : out UniqueID; abbrv : String; head, parent : UniqueID) is
  begin
    ctr := ctr + 1;
    id := ctr;
    db(ctr) := Element'(Department, id, abbrv, head, parent); 
  end;

  procedure create_person(id : out UniqueID; dept : UniqueID; name, role : String) is
  begin
    ctr := ctr + 1;
    id := ctr;
    db(ctr) := Element'(Person, id, dept, name, role); 
  end;

  function element_kind(id : UniqueID) return Kind is
  begin
    return db(id).k;
  end;

--  function text_representation(id : UniqueID) return String is
--    res : String := "(unknown)";
--  begin
--    case db(id).k is
--      when Department => 
--        res := db(id).abbrv;
--      when Person =>
--        res := db(id).name;
--    end case;

--    return res;
--  end;

  procedure get_person(id : UniqueID; dept : out UniqueID; name, role : out String) is
  begin
    dept := db(id).dept;
    name := db(id).name;
    role := db(id).role;
  end;

  procedure get_department(id : UniqueID; abbrv : out String; head, parent : out UniqueID) is
  begin
    abbrv  := db(id).abbrv;
    head   := db(id).head;
    parent := db(id).parent;
  end;
end YSoft;
