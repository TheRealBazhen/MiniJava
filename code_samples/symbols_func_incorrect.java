class Factorial {
  public static void main () {
    Fac fac;
    fac = new Fac();
    System.out.println (fac.ComputeFac (10));
  }
}

class Fac {
  public int ComputeFac (int num) {
    assert (num >= 0);
    int num_aux;
    if (num == 0) {
      return 1;
    } else {
      num_aux = num * this.ComputeFac (num-1);
    }
  }
}