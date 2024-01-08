package XSoft is
  type Kind is (Person, Department);
  type UniqueId is new Integer;
  type Element is private;

  procedure create_department(id : out UniqueID; abbrv : String; head, parent : UniqueID);
  procedure create_person(id : out UniqueID; dept : UniqueID; name, role : String);

  function element_kind(id : UniqueID) return Kind;
  procedure get_person(id : UniqueID; dept : out UniqueID; name, role : out String); 
  procedure get_department(id : UniqueID; abbrv : out String; head, parent : out UniqueID); 

  function text_representation(id : UniqueID) return String;

private

  type Element(k : Kind := Person) is record
         id   : UniqueId;
         
         case k is
           when Person =>
             dept : UniqueId;
             name : String(1..20);
             role : String(1..10);
           when Department =>
             abbrv  : String(1..20);
             head   : UniqueId;
             parent : UniqueId;
          end case;             
       end record;

end XSoft;
