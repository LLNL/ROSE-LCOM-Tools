
package body NoFunc_Renaming is

  procedure Report(s : String) is null;

  procedure ReportInt(i:Integer) is
  begin
    Report(i);
  end;

  procedure Report(i : Integer) is
  begin
    Report(i'Image);
  end;

end NoFunc_Renaming;

