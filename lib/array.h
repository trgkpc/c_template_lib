#include "slist.h"

#define define_array(T, type, FREE, PRINTF, DELIMITER)              \
    typedef struct array_##type##_ {                                \
        T* ptr;                                                     \
        int len;                                                    \
    } * array_##type;                                               \
                                                                    \
    array_##type new_array_##type(int length)                       \
    {                                                               \
        LIB_DEBUG_LOG("new_array_" #type " start\n");               \
        array_##type arr;                                           \
        arr = malloc(sizeof(struct array_##type##_));               \
        if (arr == NULL) {                                          \
            printf("Not enough memory. malloc fail.\n");            \
            exit(1);                                                \
        }                                                           \
        arr->ptr = malloc(((long unsigned int)length) * sizeof(T)); \
        arr->len = length;                                          \
        LIB_DEBUG_LOG("  new array_" #type " fin.\n");              \
        return arr;                                                 \
    }                                                               \
                                                                    \
    T* array_##type##_at(array_##type arr, int n)                   \
    {                                                               \
        LIB_DEBUG_LOG("array_" #type "_at called.\n");              \
        if (n < 0 || n >= arr->len) {                               \
            printf("index(%d) out of range! OMG!!!\n", n);          \
            exit(1);                                                \
        }                                                           \
        return arr->ptr + n;                                        \
    }                                                               \
                                                                    \
    void free_array_##type(array_##type arr)                        \
    {                                                               \
        LIB_DEBUG_LOG("free_array_" #type " start\n");              \
        for (int i = 0; i < arr->len; i++) {                        \
            /* 各要素がptrでないときはNUPを代入 */     \
            FREE(arr->ptr[i]);                                      \
        }                                                           \
        free(arr->ptr);                                             \
        free(arr);                                                  \
        LIB_DEBUG_LOG("  free_array_" #type " fin\n");              \
    }                                                               \
                                                                    \
    void print_array_##type(array_##type arr)                       \
    {                                                               \
        PRINT_DEBUG_LOG("print_array_" #type " start \n");          \
        for (int i = 0; i < arr->len; i++) {                        \
            PRINTF(arr->ptr[i]);                                    \
            printf(DELIMITER);                                      \
        }                                                           \
        PRINT_DEBUG_LOG("  print_array_" #type " fin. \n");         \
    }
