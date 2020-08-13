class foo {
    public static void main() {
        int a;
        a = 1;
        System.out.println(a);
        int b;
        b = 1;
        {
            int a;
            a = 2;
            b = 2;
            System.out.println(a);
        }
        System.out.println(a);
        System.out.println(b);
    }
}