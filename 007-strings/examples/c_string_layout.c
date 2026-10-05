#include <stdio.h>
#include <string.h>

int main(void) {
    char hello[] = "hello";
    char embedded[] = {'a', 'b', '\0', 'c', 'd', '\0'};

    printf("hello sizeof=%zu strlen=%zu\n", sizeof hello, strlen(hello));
    for (size_t i = 0; i < sizeof hello; ++i) {
        printf("hello[%zu]=%u address=%p\n",
               i, (unsigned char)hello[i], (void *)&hello[i]);
    }

    printf("embedded storage=%zu visible strlen=%zu\n",
           sizeof embedded, strlen(embedded));

    return (sizeof hello == 6 && strlen(hello) == 5 && strlen(embedded) == 2)
               ? 0
               : 1;
}
