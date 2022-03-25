#include "type.h"

void type_print(enum type type) {
    switch(type) {
        case TYPE_VOID:
            printf("void");
            break;
        case TYPE_INT:
            printf("int");
            break;
        default:
            printf("unknown");
            break;
    }
}