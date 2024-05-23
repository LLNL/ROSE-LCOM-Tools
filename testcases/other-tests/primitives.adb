procedure Primitives is
  package A is
    type AName is array (1..20) of Character;    

    type Person is tagged record
                     id   : Integer;
                     name : AName;
         end record;

    function GetName(p : in out Person) return AName;
    function GetName(p : access Person) return AName;
  end A;

  package body A is
    function GetName(p : in out Person) return AName is
    begin
      return p.name;
    end;
    
    function GetName(p : access Person) return AName is
    begin
      -- in the AST this is represented as:
      --   p.all.name  DotExp(PointerDerefExp(VarRefExp(p)), VarRefExp(name))
      -- .all is pointer derefence in Ada and it can be implicit
      -- ROSE represents the dereference regardless if it is implicit or explicit
      return p.name; 
    end;
  end A;
    
begin
  null;
end;
