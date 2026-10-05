#include <stdio.h>

typedef struct {
    int x;
    int y;
} Point;

static int recursive_sum(int n) {
    if (n <= 0) {
        return 0;
    }
    return n + recursive_sum(n - 1);
}

static void add_ten(int *value) {
    if (value != NULL) {
        *value += 10;
    }
}

static int point_manhattan_distance(Point p) {
    int ax = p.x < 0 ? -p.x : p.x;
    int ay = p.y < 0 ? -p.y : p.y;
    return ax + ay;
}

int main(void) {
    int x = 5;
    int *p = &x;

    printf("x=%d address=%p\n", x, (void *)&x);
    printf("p stores=%p dereference=%d\n", (void *)p, *p);

    add_ten(&x);
    printf("after add_ten x=%d\n", x);

    printf("recursive_sum(5)=%d\n", recursive_sum(5));

    Point point = {.x = -3, .y = 4};
    printf("point distance=%d\n", point_manhattan_distance(point));

    return (x == 15 && recursive_sum(5) == 15 && point_manhattan_distance(point) == 7)
               ? 0
               : 1;
}
