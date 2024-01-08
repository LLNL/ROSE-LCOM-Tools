package ZSoft is
  type Kind is (Person, Department);
  type UniqueId is new Integer;
  type Pers is private;
  type Dept is private;

  procedure create_department(id : out UniqueID; abbrv : String; head, parent : UniqueID);
  procedure create_person(id : out UniqueID; dept : UniqueID; name, role : String);

  function element_kind(id : UniqueID) return Kind;
  procedure get_person(id : UniqueID; dept : out UniqueID; name, role : out String); 
  procedure get_department(id : UniqueID; abbrv : out String; head, parent : out UniqueID); 

private
  
  type Pers is record
         id   : UniqueId;
         dept : UniqueId;
         name : String(1..20);
         role : String(1..10);
       end record;

  type Dept is record
         id   : UniqueId;
         abbrv  : String(1..20);
         head   : UniqueId;
         parent : UniqueId;
       end record;

end ZSoft;
