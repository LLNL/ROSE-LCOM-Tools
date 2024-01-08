package body raising1 is

  procedure NetworkCheck is
  begin
    raise NetworkFailure;
  end;

  procedure DiskCheck is
  begin
    raise DiskFailure;
  end;

  procedure InternalStatusCheck is
  begin
    raise InternalStatusFailure with "sensor reading out of range";
  end;
end raising1;
