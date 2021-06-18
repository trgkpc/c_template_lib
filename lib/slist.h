#include "msg.h"
#include <stdbool.h>
#include <stdlib.h>

// COMPARE(p,q): p-q
#define define_slist(T, type, FREE, PRINTF, DELIMITER, COMPARE,        \
    KEY_MATCHING)                                                      \
    typedef struct slobj_##type##_ {                                   \
        T value;                                                       \
        struct slobj_##type##_* next;                                  \
    } * slobj_##type;                                                  \
                                                                       \
    typedef struct slist_##type##_ {                                   \
        slobj_##type head;                                             \
        slobj_##type tail;                                             \
    } * slist_##type;                                                  \
                                                                       \
    slist_##type new_slist_##type()                                    \
    {                                                                  \
        LIB_DEBUG_LOG("new_slist_" #type " start\n");                  \
        slist_##type lis;                                              \
        lis = malloc(sizeof(struct slist_##type##_));                  \
        if (lis == NULL) {                                             \
            printf("Not enough memory. malloc fail.\n");               \
            exit(1);                                                   \
        }                                                              \
        lis->head = NULL;                                              \
        lis->tail = NULL;                                              \
        LIB_DEBUG_LOG("  new_slist_" #type " fin.\n");                 \
        return lis;                                                    \
    }                                                                  \
                                                                       \
    void free_slist_##type(slist_##type lis)                           \
    {                                                                  \
        LIB_DEBUG_LOG("free_slist_" #type " start.\n");                \
        slobj_##type obj = lis->head;                                  \
        while (obj != NULL) {                                          \
            slobj_##type next = obj->next;                             \
            FREE(obj);                                                 \
            obj = next;                                                \
        }                                                              \
        free(lis);                                                     \
        LIB_DEBUG_LOG("  free_slist_" #type " fin.\n");                \
    }                                                                  \
                                                                       \
    void append_slist_##type(slist_##type lis, T value)                \
    {                                                                  \
        LIB_DEBUG_LOG("append_slit_" #type " start\n");                \
        slobj_##type obj = malloc(sizeof(struct slobj_##type##_));     \
        if (obj == NULL) {                                             \
            printf("Not enough memory. malloc fail.\n");               \
            exit(1);                                                   \
        }                                                              \
        obj->value = value;                                            \
        obj->next = NULL;                                              \
        if (lis->head == NULL) {                                       \
            lis->head = obj;                                           \
        } else {                                                       \
            lis->tail->next = obj;                                     \
        }                                                              \
        lis->tail = obj;                                               \
        LIB_DEBUG_LOG("  append_slit_" #type " fin.\n");               \
    }                                                                  \
                                                                       \
    void insert_head_slist_##type(slist_##type lis, T value)           \
    {                                                                  \
        LIB_DEBUG_LOG("insert_head_slit_" #type " start\n");           \
        slobj_##type obj = malloc(sizeof(struct slobj_##type##_));     \
        if (obj == NULL) {                                             \
            printf("Not enough memory. malloc fail.\n");               \
            exit(1);                                                   \
        }                                                              \
        obj->value = value;                                            \
        obj->next = lis->head;                                         \
        if (lis->tail == NULL) {                                       \
            lis->tail = obj;                                           \
        }                                                              \
        lis->head = obj;                                               \
        LIB_DEBUG_LOG("  insert_slit_" #type " fin.\n");               \
    }                                                                  \
                                                                       \
    void print_slist_##type(slist_##type lis)                          \
    {                                                                  \
        PRINT_DEBUG_LOG("print_slit_" #type " start\n");               \
        slobj_##type obj = (lis->head);                                \
        while (obj != NULL) {                                          \
            PRINTF(obj->value);                                        \
            printf(DELIMITER);                                         \
            obj = obj->next;                                           \
        }                                                              \
        PRINT_DEBUG_LOG("  print slit_" #type " end\n");               \
    }                                                                  \
                                                                       \
    void insert_slist_##type(slist_##type lis, T value)                \
    {                                                                  \
        LIB_DEBUG_LOG("insert_slist_" #type " start\n");               \
        if (lis->head == NULL) {                                       \
            insert_head_slist_##type(lis, value);                      \
        } else if (COMPARE(lis->head->value, value) <= 0) {            \
            insert_head_slist_##type(lis, value);                      \
        } else if (COMPARE(lis->tail->value, value) >= 0) {            \
            append_slist_##type(lis, value);                           \
        } else {                                                       \
            /*insertするobjを作る*/                               \
            slobj_##type obj = malloc(sizeof(struct slobj_##type##_)); \
            slobj_##type ptr = lis->head;                              \
            slobj_##type next = ptr->next;                             \
            for (;;) {                                                 \
                if (COMPARE(next->value, value) >= 0) {                \
                    break;                                             \
                } else {                                               \
                    ptr = next;                                        \
                    next = ptr->next;                                  \
                }                                                      \
            }                                                          \
            ptr->next = obj;                                           \
            obj->value = value;                                        \
            obj->next = next;                                          \
        }                                                              \
    }                                                                  \
                                                                       \
    void delete_slist_##type(slist_##type lis, T key)                  \
    {                                                                  \
        slobj_##type obj = lis->head;                                  \
        lis->head = NULL;                                              \
        lis->tail = NULL;                                              \
        while (obj != NULL) {                                          \
            if (KEY_MATCHING(obj->value, key)) {                       \
                slobj_##type old = obj;                                \
                obj = obj->next;                                       \
                FREE(old);                                             \
            } else {                                                   \
                if (lis->head == NULL) {                               \
                    lis->head = obj;                                   \
                } else {                                               \
                    lis->tail->next = obj;                             \
                }                                                      \
                lis->tail = obj;                                       \
                obj = obj->next;                                       \
            }                                                          \
        }                                                              \
    }                                                                  \
                                                                       \
    slobj_##type search_slist_##type(slist_##type lis, T key)          \
    {                                                                  \
        slobj_##type obj = lis->head;                                  \
        while (obj != NULL) {                                          \
            if (KEY_MATCHING(obj->value, key)) {                       \
                break;                                                 \
            }                                                          \
            obj = obj->next;                                           \
        }                                                              \
        return obj;                                                    \
    }                                                                  \
                                                                       \
    slist_##type merge_slist_##type(slist_##type a, slist_##type b)    \
    {                                                                  \
        slist_##type lis = new_slist_##type();                         \
        while (a->head != NULL && b->head != NULL) {                   \
            if (COMPARE(a->head->value, b->head->value) < 0) {         \
                /* (b-a)>0 <--> [b > a] */                             \
                if (lis->tail == NULL) {                               \
                    lis->head = a->head;                               \
                } else {                                               \
                    lis->tail->next = a->head;                         \
                }                                                      \
                lis->tail = a->head;                                   \
                a->head = a->head->next;                               \
            } else {                                                   \
                /* (b-a)<=0 <--> [a >= b] */                           \
                if (lis->tail == NULL) {                               \
                    lis->head = b->head;                               \
                } else {                                               \
                    lis->tail->next = b->head;                         \
                }                                                      \
                lis->tail = b->head;                                   \
                b->head = b->head->next;                               \
            }                                                          \
        }                                                              \
        if (a->head != NULL) {                                         \
            if (lis->tail == NULL) {                                   \
                lis->head = a->head;                                   \
            } else {                                                   \
                lis->tail->next = a->head;                             \
            }                                                          \
            lis->tail = a->tail;                                       \
        }                                                              \
        if (b->head != NULL) {                                         \
            if (lis->tail == NULL) {                                   \
                lis->head = b->head;                                   \
            } else {                                                   \
                lis->tail->next = b->head;                             \
            }                                                          \
            lis->tail = b->tail;                                       \
        }                                                              \
        lis->tail->next = NULL;                                        \
        free(a);                                                       \
        free(b);                                                       \
        return lis;                                                    \
    }                                                                  \
                                                                       \
    slist_##type sort_slist_##type(slist_##type lis)                   \
    {                                                                  \
        if (lis->tail == NULL) {                                       \
            /* 長さが1以下ならmergeすることはない */     \
            return lis;                                                \
        } else if (lis->head->next == NULL) {                          \
            return lis;                                                \
        }                                                              \
                                                                       \
        bool change_tail = true;                                       \
        bool first = true;                                             \
        slobj_##type left_tail = lis->head;                            \
        slobj_##type ptr = lis->head;                                  \
        while (ptr != NULL) {                                          \
            ptr = ptr->next;                                           \
            if (change_tail) {                                         \
                if (first) {                                           \
                    first = false;                                     \
                } else {                                               \
                    left_tail = left_tail->next;                       \
                }                                                      \
            }                                                          \
            change_tail ^= true;                                       \
        }                                                              \
                                                                       \
        slist_##type right = new_slist_##type();                       \
        right->head = left_tail->next;                                 \
        right->tail = lis->tail;                                       \
                                                                       \
        slist_##type left = new_slist_##type();                        \
        left->head = lis->head;                                        \
        left->tail = left_tail;                                        \
        left->tail->next = NULL;                                       \
                                                                       \
        slist_##type sorted_left = sort_slist_##type(left);            \
        slist_##type sorted_right = sort_slist_##type(right);          \
        return merge_slist_##type(sorted_left, sorted_right);          \
    }
