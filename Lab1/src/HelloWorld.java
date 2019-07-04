public class HelloWorld {
    public static void main (String[] args) {

        try {
            System.out.println("Hello World");
            String s = args[0];
            int i = Integer.valueOf(s);
            System.out.printf("decimal representation: %d\n",i);
            System.out.println("hexadecimal representation: 0x"+Integer.toHexString(i));
            System.out.println("binary representation: "+Integer.toBinaryString(i));
        } catch (Exception e) {
            System.out.println("invalid input\n");
            return;
        }
    }
}
