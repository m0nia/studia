package cp1.solution;

public class Pair<T, S> {
    private T x;
    private S y;

    public Pair(T x, S y) {
        this.x = x;
        this.y = y;
    }

    public T first() {
        return x;
    }

    public S second() {
        return y;
    }
}
