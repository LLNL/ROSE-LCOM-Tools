public class Raising {
    public static class NetworkFailure extends Exception {}
    public static class DiskFailure extends Exception {}
    public static class InternalStatusFailure extends Exception {}

    public static void networkCheck() throws NetworkFailure {
        throw new NetworkFailure();
    }

    public static void diskCheck() throws DiskFailure {
        throw new DiskFailure();
    }

    public static void internalStatusCheck() throws InternalStatusFailure {
        throw new InternalStatusFailure();
    }
}
