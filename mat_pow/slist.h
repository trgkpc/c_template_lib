#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int merge_counter;

// COMPARE(p,q): p-q
#define define_slist(T, type, FREE, COMPARE)                        \
    typedef struct slobj_##type##_ {                                \
        T value;                                                    \
        struct slobj_##type##_* next;                               \
    } * slobj_##type;                                               \
                                                                    \
    typedef struct slist_##type##_ {                                \
        slobj_##type head;                                          \
        slobj_##type tail;                                          \
    } * slist_##type;                                               \
                                                                    \
    slist_##type new_slist_##type()                                 \
    {                                                               \
        slist_##type lis;                                           \
        lis = malloc(sizeof(struct slist_##type##_));               \
        lis->head = NULL;                                           \
        lis->tail = NULL;                                           \
        return lis;                                                 \
    }                                                               \
                                                                    \
    void free_slist_##type(slist_##type lis)                        \
    {                                                               \
        slobj_##type obj = lis->head;                               \
        while (obj != NULL) {                                       \
            slobj_##type next = obj->next;                          \
            FREE(obj);                                              \
            obj = next;                                             \
        }                                                           \
        free(lis);                                                  \
    }                                                               \
                                                                    \
    void append_slist_##type(slist_##type lis, T* value)            \
    {                                                               \
        slobj_##type obj = malloc(sizeof(struct slobj_##type##_));  \
        obj->value = *value;                                        \
        obj->next = NULL;                                           \
        if (lis->head == NULL) {                                    \
            lis->head = obj;                                        \
        } else {                                                    \
            lis->tail->next = obj;                                  \
        }                                                           \
        lis->tail = obj;                                            \
    }                                                               \
                                                                    \
    slist_##type merge_slist_##type(slist_##type a, slist_##type b) \
    {                                                               \
        slist_##type lis = new_slist_##type();                      \
        while (a->head != NULL && b->head != NULL) {                \
            if (COMPARE(a->head->value, b->head->value) < 0) {      \
                /* (b-a)>0 <--> [b > a] */                          \
                if (lis->tail == NULL) {                            \
                    lis->head = a->head;                            \
                } else {                                            \
                    lis->tail->next = a->head;                      \
                }                                                   \
                lis->tail = a->head;                                \
                a->head = a->head->next;                            \
            } else {                                                \
                /* (b-a)<=0 <--> [a >= b] */                        \
                if (lis->tail == NULL) {                            \
                    lis->head = b->head;                            \
                } else {                                            \
                    lis->tail->next = b->head;                      \
                }                                                   \
                lis->tail = b->head;                                \
                b->head = b->head->next;                            \
            }                                                       \
        }                                                           \
        if (a->head != NULL) {                                      \
            if (lis->tail == NULL) {                                \
                lis->head = a->head;                                \
            } else {                                                \
                lis->tail->next = a->head;                          \
            }                                                       \
            lis->tail = a->tail;                                    \
        }                                                           \
        if (b->head != NULL) {                                      \
            if (lis->tail == NULL) {                                \
                lis->head = b->head;                                \
            } else {                                                \
                lis->tail->next = b->head;                          \
            }                                                       \
            lis->tail = b->tail;                                    \
        }                                                           \
        lis->tail->next = NULL;                                     \
        free(a);                                                    \
        free(b);                                                    \
        return lis;                                                 \
    }                                                               \
                                                                    \
                                                                    \
    slist_##type sort_slist_##type(slist_##type lis)                \
    {                                                               \
        if (lis->tail == NULL || lis->head->next == NULL) {         \
            /* 長さが1以下ならmergeすることはない */  \
            return lis;                                             \
        }                                                           \
                                                                    \
        bool change_tail = true;                                    \
        bool first = true;                                          \
        slobj_##type left_tail = lis->head;                         \
        slobj_##type ptr = lis->head;                               \
        while (ptr != NULL) {                                       \
            ptr = ptr->next;                                        \
            if (change_tail) {                                      \
                if (first) {                                        \
                    first = false;                                  \
                } else {                                            \
                    left_tail = left_tail->next;                    \
                }                                                   \
            }                                                       \
            change_tail ^= true;                                    \
        }                                                           \
                                                                    \
        slist_##type right = new_slist_##type();                    \
        right->head = left_tail->next;                              \
        right->tail = lis->tail;                                    \
                                                                    \
        slist_##type left = new_slist_##type();                     \
        left->head = lis->head;                                     \
        left->tail = left_tail;                                     \
        left->tail->next = NULL;                                    \
                                                                    \
        slist_##type sorted_left = sort_slist_##type(left);         \
        slist_##type sorted_right = sort_slist_##type(right);       \
        return merge_slist_##type(sorted_left, sorted_right);       \
    }
