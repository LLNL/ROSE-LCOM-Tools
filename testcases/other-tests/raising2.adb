package body raising2 is

  procedure NetworkCheck is
  begin
    raise StatusFailure with "network failed";
  end;

  procedure DiskCheck is
  begin
    raise StatusFailure with "disk failed";
  end;

  procedure InternalStatusCheck is
  begin
    raise StatusFailure with "sensor reading out of range";
  end;
end raising2;
