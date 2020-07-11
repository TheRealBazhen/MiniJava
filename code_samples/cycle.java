class foo {
    public static void main() {
        int a;
        int prod;
        a = 5;
        prod = 1;
        while (a >= 1 || a > 0) {
            prod = prod * a;
            a = a - 1;
        }
        System.out.println(prod);
    }
}