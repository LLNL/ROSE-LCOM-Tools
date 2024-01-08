
package body Novar_Renaming is
  type ReportHistory is (NoReport, IntReport, StringReport);

  last: ReportHistory := NoReport;

  procedure Report(s : String) is
  begin
    last := StringReport;
  end;

  procedure Report(i : Integer) is
  begin
    last := IntReport;
  end;

end Novar_Renaming;

