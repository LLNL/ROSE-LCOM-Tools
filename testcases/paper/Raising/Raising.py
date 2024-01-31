class NetworkFailure(Exception):
    pass

class DiskFailure(Exception):
    pass

class InternalStatusFailure(Exception):
    pass

def network_check():
    raise NetworkFailure()

def disk_check():
    raise DiskFailure()

def internal_status_check():
    raise InternalStatusFailure()
