#include "byte_string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

static double build_dynamic(size_t n) {
    ByteString *s = byte_string_create();
    if (s == NULL) return -1.0;

    struct timespec a,b;
    timespec_get(&a, TIME_UTC);
    for (size_t i=0;i<n;++i) {
        if (!byte_string_append_char(s, (char)('a'+(i%26)))) {
            byte_string_free(s);
            return -1.0;
        }
    }
    timespec_get(&b, TIME_UTC);
    double t=elapsed(a,b);
    byte_string_free(s);
    return t;
}

static double build_repeated_copy(size_t n) {
    char *s = malloc(1);
    if (s == NULL) return -1.0;
    s[0]='\0';
    size_t len=0;

    struct timespec a,b;
    timespec_get(&a, TIME_UTC);
    for (size_t i=0;i<n;++i) {
        char *next = malloc(len+2);
        if (next == NULL) { free(s); return -1.0; }
        memcpy(next,s,len);
        next[len]=(char)('a'+(i%26));
        next[len+1]='\0';
        free(s);
        s=next;
        ++len;
    }
    timespec_get(&b, TIME_UTC);
    double t=elapsed(a,b);
    free(s);
    return t;
}

int main(void) {
    const size_t ns[]={1000,2000,4000,8000};
    puts("n,dynamic,repeated_copy");
    for(size_t i=0;i<sizeof ns/sizeof ns[0];++i){
        double a=build_dynamic(ns[i]);
        double b=build_repeated_copy(ns[i]);
        if(a<0||b<0) return 1;
        printf("%zu,%.9f,%.9f\n",ns[i],a,b);
    }
    return 0;
}
