
package body Var_Renaming is
  type ReportHistory is (NoReport, IntReport, StringReport);

  last:  ReportHistory := NoReport;
  prev:  ReportHistory renames last; -- alias of last
  prior: ReportHistory renames prev; -- alias of (prev, last)

  procedure Report(s : String) is
  begin
    prev := StringReport;
  end;

  procedure Report(i : Integer) is
  begin
    last := IntReport;
  end;

end Var_Renaming;

