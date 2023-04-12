import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        int n, m;
        Scanner sc = new Scanner(System.in);
        System.out.println("Input size of matrix:");
        n = sc.nextInt();
        System.out.println("Input count of threads:");
        m = sc.nextInt();
        int[][] a, b, c;
        a = new int[n][n];
        b = new int[n][n];
        c = new int[n][n];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                a[i][j] = (int) (Math.random()*2);
                b[i][j] = (int) (Math.random()*2);
                c[i][j] = 0;
            }
        long start = System.nanoTime();
        for (int l = 0; l < n; l += n/m) {
            MyThread t = new MyThread(a, b, c, n, l, m);
            t.start();
        }
        long end = System.nanoTime();
        System.out.println(end - start);
//        for (int i=0;i<n;i++)
//        {
//            System.out.println();
//            for (int j=0;j<n;j++)
//                System.out.print(a[i][j]+" ");
//        }
//        System.out.println();
//        for (int i=0;i<n;i++)
//        {
//            System.out.println();
//            for (int j=0;j<n;j++)
//                System.out.print(b[i][j]+" ");
//        }
//        System.out.println();
//        for (int i=0;i<n;i++)
//        {
//            System.out.println();
//            for (int j=0;j<n;j++)
//                System.out.print(c[i][j]+" ");
//        }
    }
}