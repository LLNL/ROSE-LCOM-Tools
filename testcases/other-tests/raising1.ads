package raising1 is
  NetworkFailure        : Exception;
  DiskFailure           : Exception;
  InternalStatusFailure : Exception;

  procedure NetworkCheck;
  procedure DiskCheck;
  procedure InternalStatusCheck;
end raising1;
