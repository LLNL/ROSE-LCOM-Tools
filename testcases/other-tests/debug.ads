package Debug is

  type Mode is (off, entries, exits, full);

  debugmode : Mode := off;

  procedure RoutineEntry(name : String; newDebugMode : Mode := debugmode);
  procedure RoutineExit(name : String);
end Debug;
