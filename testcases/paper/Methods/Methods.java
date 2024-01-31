public class Methods {
    public void methodOne() {
        System.out.println("Method one called.");
    }

    public void methodTwo() {
        System.out.println("Method two called.");
        methodOne(); // Calls methodOne
    }

    public void methodThree() {
        System.out.println("Method three called.");
        OutsideClass.outsideMethod(); // Calls outside method
    }

    public static void main(String[] args) {
        return;
    }
}
