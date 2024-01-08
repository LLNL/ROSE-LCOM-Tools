package body Debug is
  procedure RoutineEntry(name : String; newDebugMode : Mode := debugmode) is null;
  procedure RoutineExit(name : String) is null;
end Debug;

