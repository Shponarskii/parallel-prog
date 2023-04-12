public class MyThread extends Thread {
    private int[][] a;
    private int[][] b;
    private int[][] c;
    private int n, l, m;

    public MyThread(int[][] a, int[][] b, int[][] c, int n, int l, int m) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.n = n;
        this.l = l;
        this.m = m;
    }

    public void run() {
        for (int i = l; i < l + n/m; i++)
            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    c[i][j] += a[i][k] * b[k][j];
    }
}