
package body Func_Renaming_Use is

  procedure Report(s : String) is null;

  procedure ReportInt(i : Integer) renames Report;

  procedure Report(i : Integer) is
  begin
    Report(i'Image);
  end;

  procedure UseReportInt is
  begin
    ReportInt(5);
  end;

end Func_Renaming_Use;

