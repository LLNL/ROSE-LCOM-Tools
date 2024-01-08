
package body Func_Renaming is

  procedure Report(s : String) is null;

  procedure ReportInt(i:Integer) renames Report;

  procedure Report(i : Integer) is
  begin
    Report(i'Image);
  end;

end Func_Renaming;

