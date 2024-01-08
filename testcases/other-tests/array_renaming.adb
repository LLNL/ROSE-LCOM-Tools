
package body Array_Renaming is
  type ReportHistory is (NoReport, IntReport, StringReport);
  type HistoryLog is array (Integer range <>) of ReportHistory;

  history : HistoryLog(1..10) := (others => NoReport);
  lastTwo : HistoryLog renames history(9..10);

  procedure Report(s : String) is
  begin
    for I in history'first .. history'last-1 loop
      history(i+1) := history(i);
    end loop;

    history(1) := StringReport;
  end;

  procedure Report(i : Integer) is
  begin
    for I in lastTwo'first .. lastTwo'last-1 loop
      lastTwo(i+1) := lastTwo(i);
    end loop;

    lastTwo(lastTwo'first) := IntReport;
  end;

end Array_Renaming;

