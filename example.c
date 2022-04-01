int a = 3 ;

void main(void) {
    int b;
    int c = 14 ;
    b = 2 ;

    if (a == 3) {
        b = 8;
    }

    if (a < c) {
        b = b + 1 ;
    } else {
        b = b - 1 ;
    }
    
    int compteur = 0;
    while (b > 0) {
        b = b - 1;
        compteur = compteur + 1;
    }

    print(compteur);
}
