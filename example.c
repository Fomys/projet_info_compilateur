int c;

void test(int a) {
    int b;
}

int testts(int a, int b) {
    return a + b;
}

void main(void) {
    int a = 0;
    int b = 1;
    if (a == 0) {
        a = 2;
    }
    while(a < 4) {
        a = a + 1;
    }
    printf(a);
}
