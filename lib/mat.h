#include "array.h"

#define define_sparce_matrix(T, type, FREE, PRINT_FORMAT, DELIMITER, TIMES, COMPARE, EQUAL, ONE, ZERO) \
    typedef struct {                                                                                   \
        int id;                                                                                        \
        T value;                                                                                       \
    } pair_##type;                                                                                     \
                                                                                                       \
    void print_pair_##type(pair_##type p)                                                              \
    {                                                                                                  \
        PRINT_DEBUG_LOG("print pair " #type " called\n");                                              \
        printf("%d" DELIMITER PRINT_FORMAT, p.id + 1, p.value);                                        \
    }                                                                                                  \
                                                                                                       \
    define_slist(pair_##type, pair_##type, FREE, print_pair_##type, DELIMITER, COMPARE, EQUAL);        \
    define_array(slist_pair_##type, slist_pair_##type,                                                 \
        free_slist_pair_##type, print_slist_pair_##type, "-1\n");                                      \
    define_array(slobj_pair_##type, slop##type, FREE, NUP, DELIMITER);                                 \
                                                                                                       \
    T svector_inner_product_##type(slist_pair_##type a, slist_pair_##type b)                           \
    {                                                                                                  \
        T ans = ZERO;                                                                                  \
        slobj_pair_##type x = a->head;                                                                 \
        slobj_pair_##type y = b->head;                                                                 \
        while (true) {                                                                                 \
            if (x->value.id < y->value.id) {                                                           \
                x = x->next;                                                                           \
                if (x == NULL) {                                                                       \
                    break;                                                                             \
                }                                                                                      \
            } else if (x->value.id > y->value.id) {                                                    \
                y = y->next;                                                                           \
                if (y == NULL) {                                                                       \
                    break;                                                                             \
                }                                                                                      \
            } else {                                                                                   \
                ans += TIMES(x->value.value, y->value.value);                                          \
                x = x->next;                                                                           \
                if (x == NULL) {                                                                       \
                    break;                                                                             \
                }                                                                                      \
                y = y->next;                                                                           \
                if (y == NULL) {                                                                       \
                    break;                                                                             \
                }                                                                                      \
            }                                                                                          \
        }                                                                                              \
        return ans;                                                                                    \
    }                                                                                                  \
                                                                                                       \
    typedef struct smat_##type##_ {                                                                    \
        int n, m;                                                                                      \
        array_slist_pair_##type value;                                                                 \
    } * smat_##type;                                                                                   \
                                                                                                       \
    struct smat_##type##_set {                                                                         \
        smat_##type Matrix;                                                                            \
        smat_##type Transposed;                                                                        \
    };                                                                                                 \
                                                                                                       \
    smat_##type new_smat_##type(int n, int m)                                                          \
    {                                                                                                  \
        LIB_DEBUG_LOG("new smat " #type "(%d,%d)) start.\n", n, m);                                    \
        smat_##type mat;                                                                               \
        mat = malloc(sizeof(struct smat_##type##_));                                                   \
        if (mat == NULL) {                                                                             \
            printf("Not enough memory. malloc fin.\n");                                                \
            exit(1);                                                                                   \
        }                                                                                              \
        mat->n = n;                                                                                    \
        mat->m = m;                                                                                    \
        mat->value = new_array_slist_pair_##type(n);                                                   \
        for (int i = 0; i < n; i++) {                                                                  \
            *array_slist_pair_##type##_at(mat->value, i) = new_slist_pair_##type();                    \
        }                                                                                              \
        LIB_DEBUG_LOG("  new smat " #type " fin.\n");                                                  \
        return mat;                                                                                    \
    }                                                                                                  \
                                                                                                       \
    void free_smat_##type(smat_##type Mat)                                                             \
    {                                                                                                  \
        LIB_DEBUG_LOG("free smat " #type " start.\n");                                                 \
        free_array_slist_pair_##type(Mat->value);                                                      \
        free(Mat);                                                                                     \
        LIB_DEBUG_LOG("  free smat " #type " fin.\n");                                                 \
    }                                                                                                  \
                                                                                                       \
    void append_smat_##type(smat_##type Mat, int i, int j, T value)                                    \
    {                                                                                                  \
        LIB_DEBUG_LOG("append_smat_" #type "(%d,%d) start.\n", i, j);                                  \
        pair_##type p;                                                                                 \
        p.id = j;                                                                                      \
        p.value = value;                                                                               \
        append_slist_pair_##type(                                                                      \
            *array_slist_pair_##type##_at(Mat->value, i),                                              \
            p);                                                                                        \
        LIB_DEBUG_LOG("  append_smat_" #type " fin.\n");                                               \
    }                                                                                                  \
                                                                                                       \
    smat_##type identity_smat_##type(int n)                                                            \
    {                                                                                                  \
        smat_##type mat = new_smat_##type(n, n);                                                       \
        for (int i = 0; i < n; i++) {                                                                  \
            append_smat_##type(mat, i, i, ONE);                                                        \
        }                                                                                              \
        return mat;                                                                                    \
    }                                                                                                  \
                                                                                                       \
                                                                                                       \
    smat_##type load_input_smat_##type##_stable()                                                      \
    {                                                                                                  \
        LIB_DEBUG_LOG("load_input_smat " #type " start.\n");                                           \
        int n, m;                                                                                      \
        scanf("%d %d", &n, &m);                                                                        \
        smat_##type Mat = new_smat_##type(n, m);                                                       \
        LIB_DEBUG_LOG("    load %d %d\n", n, m);                                                       \
        for (int i = 0; i < n; i++) {                                                                  \
            int j = 0;                                                                                 \
            scanf("%d", &j);                                                                           \
            while (j > EOF) {                                                                          \
                /* pair作成 */                                                                       \
                T v;                                                                                   \
                scanf(PRINT_FORMAT, &v);                                                               \
                append_smat_##type(Mat, i, j - 1, v);                                                  \
                LIB_DEBUG_LOG("    load new value %d, %d " PRINT_FORMAT "\n", i, j - 1, v);            \
                scanf("%d", &j);                                                                       \
            }                                                                                          \
            LIB_DEBUG_LOG("    load new collum %d \n", i);                                             \
        }                                                                                              \
        LIB_DEBUG_LOG("  load smat " #type " fin.\n");                                                 \
        return Mat;                                                                                    \
    }                                                                                                  \
                                                                                                       \
    smat_##type load_input_smat_##type()                                                               \
    {                                                                                                  \
        LIB_DEBUG_LOG("load_input_smat " #type " start.\n");                                           \
        int n, m;                                                                                      \
        scanf("%d %d", &n, &m);                                                                        \
        smat_##type Mat = new_smat_##type(n, m);                                                       \
        LIB_DEBUG_LOG("    load %d %d\n", n, m);                                                       \
        for (int i = 0; i < n; i++) {                                                                  \
            int j = 0;                                                                                 \
            slist_pair_##type collum = *array_slist_pair_##type##_at(Mat->value, i);                   \
            scanf("%d", &j);                                                                           \
            while (j > EOF) {                                                                          \
                /* pair作成 */                                                                       \
                T v;                                                                                   \
                scanf(PRINT_FORMAT, &v);                                                               \
                pair_##type p;                                                                         \
                p.id = j - 1;                                                                          \
                p.value = v;                                                                           \
                append_slist_pair_##type(collum, p);                                                   \
                LIB_DEBUG_LOG("    load new value %d, %d " PRINT_FORMAT "\n", i, j - 1, v);            \
                                                                                                       \
                scanf("%d", &j);                                                                       \
            }                                                                                          \
        }                                                                                              \
        LIB_DEBUG_LOG("  load smat " #type " fin.\n");                                                 \
        return Mat;                                                                                    \
    }                                                                                                  \
                                                                                                       \
    struct smat_##type##_set load_input_smat_##type##_new()                                            \
    {                                                                                                  \
        int n, m;                                                                                      \
        scanf("%d %d", &n, &m);                                                                        \
        smat_##type Mat = new_smat_##type(n, m);                                                       \
        smat_##type Tra = new_smat_##type(m, n);                                                       \
        for (int i = 0; i < n; i++) {                                                                  \
            int j = 0;                                                                                 \
            while (j > EOF) {                                                                          \
                scanf("%d", &j);                                                                       \
                if (j > EOF) {                                                                         \
                    T v;                                                                               \
                    scanf(PRINT_FORMAT, &v);                                                           \
                    append_smat_##type(Mat, i, j - 1, v);                                              \
                    append_smat_##type(Tra, j - 1, i, v);                                              \
                } else {                                                                               \
                    break;                                                                             \
                }                                                                                      \
            }                                                                                          \
        }                                                                                              \
        struct smat_##type##_set set;                                                                  \
        set.Matrix = Mat;                                                                              \
        set.Transposed = Tra;                                                                          \
        return set;                                                                                    \
    }                                                                                                  \
                                                                                                       \
    void change_smat_##type(smat_##type Mat, int i, int j, T value)                                    \
    { /* 遅い */                                                                                     \
        /*LIB_DEBUG_LOG("change smat " #type " start. (%d,%d)=" PRNIT_FORMAT "\n", i, j, value);         \
        */pair_##type p;                                                                                 \
        p.id = j;                                                                                      \
        p.value = value;                                                                               \
        slist_pair_##type collum = *array_slist_pair_##type##_at(Mat->value, i);                       \
        if (collum->head == NULL) {                                                                    \
            LIB_DEBUG_LOG("    change 0 length collum\n");                                             \
            append_slist_pair_##type(collum, p);                                                       \
        } else if (collum->tail->value.id <= j) {                                                      \
            if (collum->tail->value.id < j) {                                                          \
                LIB_DEBUG_LOG("    append\n");                                                         \
                append_slist_pair_##type(collum, p);                                                   \
            } else {                                                                                   \
                LIB_DEBUG_LOG("    change tail value\n");                                              \
                collum->tail->value.value = value;                                                     \
            }                                                                                          \
        } else if (collum->head->value.id >= j) {                                                      \
            if (collum->head->value.id > j) {                                                          \
                LIB_DEBUG_LOG("    insert head\n");                                                    \
                insert_head_slist_pair_##type(collum, p);                                              \
            } else {                                                                                   \
                LIB_DEBUG_LOG("    change head value\n");                                              \
                collum->head->value.value = value;                                                     \
            }                                                                                          \
        } else {                                                                                       \
            slobj_pair_##type last_obj = collum->head;                                                 \
            slobj_pair_##type next_obj = last_obj->next;                                               \
            while (j > next_obj->value.id) {                                                           \
                slobj_pair_##type tmp = next_obj->next;                                                \
                last_obj = next_obj;                                                                   \
                next_obj = tmp;                                                                        \
            }                                                                                          \
            if (j == next_obj->value.id) {                                                             \
                LIB_DEBUG_LOG("change %d\n", next_obj->value.id);                                      \
                next_obj->value.value = value;                                                         \
            } else {                                                                                   \
                slobj_pair_##type new_obj = malloc(sizeof(struct slobj_pair_##type##_));               \
                if (new_obj == NULL) {                                                                 \
                    printf("Not enough memory. malloc fail...\n");                                     \
                    exit(1);                                                                           \
                }                                                                                      \
                new_obj->value = p;                                                                    \
                last_obj->next = new_obj;                                                              \
                new_obj->next = next_obj;                                                              \
            }                                                                                          \
        }                                                                                              \
        LIB_DEBUG_LOG("  change smat " #type " fin.\n");                                               \
    }                                                                                                  \
                                                                                                       \
    T access_smst_##type(smat_##type Mat, int i, int j)                                                \
    { /*デバッグされていない*/                                                               \
        LIB_DEBUG_LOG("access smat " #type " start. (%d,%d)\n", i, j);                                 \
        slist_pair_##type collum = *array_slist_pair_##type##_at(Mat->value, i);                       \
        if (collum->head == NULL) {                                                                    \
            return ZERO;                                                                               \
        } else if (collum->tail->value.id <= j) {                                                      \
            if (collum->tail->value.id < j) {                                                          \
                return ZERO;                                                                           \
            } else {                                                                                   \
                return collum->tail->value.value;                                                      \
            }                                                                                          \
        } else if (collum->head->value.id > j) {                                                       \
            return collum->head->value.value;                                                          \
        } else {                                                                                       \
            slobj_pair_##type obj = collum->head;                                                      \
            while (obj->value.id < j) {                                                                \
                obj = obj->next;                                                                       \
            }                                                                                          \
            if (obj->value.id == j) {                                                                  \
                return obj->value.value;                                                               \
            } else {                                                                                   \
                return ZERO;                                                                           \
            }                                                                                          \
        }                                                                                              \
        WARNING("  access smat fail? " #type " fin.\n");                                               \
        return ZERO;                                                                                   \
    }                                                                                                  \
                                                                                                       \
    smat_##type transpose_smat_##type(smat_##type Mat)                                                 \
    {                                                                                                  \
        LIB_DEBUG_LOG("transpose smat " #type " start.\n");                                            \
        int m = Mat->m;                                                                                \
        int n = Mat->n;                                                                                \
        smat_##type new_Mat = new_smat_##type(m, n);                                                   \
        for (int i = 0; i < n; i++) {                                                                  \
            slobj_pair_##type obj = (*array_slist_pair_##type##_at(Mat->value, i))->head;              \
            while (obj != NULL) {                                                                      \
                int j = obj->value.id;                                                                 \
                append_smat_##type(new_Mat, j, i, obj->value.value);                                   \
                obj = obj->next;                                                                       \
            }                                                                                          \
        }                                                                                              \
        LIB_DEBUG_LOG("  transpose smat " #type " fin.\n");                                            \
        return new_Mat;                                                                                \
    }                                                                                                  \
                                                                                                       \
    smat_##type product_smat_##type(smat_##type A, smat_##type B)                                      \
    {                                                                                                  \
        LIB_DEBUG_LOG("product smat " #type " start.\n");                                              \
        int n1 = A->n;                                                                                 \
        int m1 = A->m;                                                                                 \
        int n2 = B->n;                                                                                 \
        int m2 = B->m;                                                                                 \
        if (m1 != n2) {                                                                                \
            /*printf("matrix (%d,%d)*(%d,%d) product size error...\n", m1, n1, m2, n2);                \
            exit(1);  */                                                                               \
            return new_smat_##type(0, 0);                                                              \
        }                                                                                              \
        smat_##type Mat = new_smat_##type(n1, m2);                                                     \
        array_slop##type row_ptr = new_array_slop##type(n2);                                           \
        for (int i = 0; i < n2; i++) {                                                                 \
            *array_slop##type##_at(row_ptr, i) = (*array_slist_pair_##type##_at(B->value, i))->head;   \
            slobj_pair_##type obj = *array_slop##type##_at(row_ptr, i);                                \
            if (obj == NULL) {                                                                         \
                LIB_DEBUG_LOG("%dNULL\n", i);                                                          \
            } else {                                                                                   \
                LIB_DEBUG_LOG("%d,%d\n", i, obj->value.id);                                            \
            }                                                                                          \
        }                                                                                              \
        for (int j = 0; j < m2; j++) {                                                                 \
            for (int i = 0; i < n1; i++) {                                                             \
                /* 行方向に探索  */                                                              \
                T v = ZERO;                                                                            \
                slobj_pair_##type collum_obj = (*array_slist_pair_##type##_at(A->value, i))->head;     \
                LIB_DEBUG_LOG("collum search start.\n");                                               \
                while (collum_obj != NULL) {                                                           \
                    LIB_DEBUG_LOG("search collum.(%d,%d)\n", i, collum_obj->value.id);                 \
                    slobj_pair_##type row_obj = *array_slop##type##_at(row_ptr, collum_obj->value.id); \
                    if (row_obj == NULL) {                                                             \
                        LIB_DEBUG_LOG("row null\n");                                                   \
                    } else if (row_obj->value.id == j) {                                               \
                        v += TIMES(collum_obj->value.value, row_obj->value.value);                     \
                        LIB_DEBUG_LOG("row has value\n");                                              \
                    } else {                                                                           \
                        LIB_DEBUG_LOG("row nothing.,next=%d\n", row_obj->value.id);                    \
                    }                                                                                  \
                    collum_obj = collum_obj->next;                                                     \
                }                                                                                      \
                                                                                                       \
                /* 行探索終了処理*/                                                             \
                if (v != ZERO) {                                                                       \
                    append_smat_##type(Mat, i, j, v);                                                  \
                    LIB_DEBUG_LOG("%d,%d," PRINT_FORMAT "\n", i, j, v);                                \
                } else {                                                                               \
                    LIB_DEBUG_LOG("%d,%d,None\n", i, j);                                               \
                }                                                                                      \
            }                                                                                          \
            LIB_DEBUG_LOG("collum search end.\n");                                                     \
            for (int i = 0; i < n2; i++) {                                                             \
                slobj_pair_##type* row_obj = array_slop##type##_at(row_ptr, i);                        \
                if ((*row_obj) == NULL) {                                                              \
                    LIB_DEBUG_LOG("continue at row update\n");                                         \
                    continue;                                                                          \
                } else {                                                                               \
                    if ((*row_obj)->value.id == j) {                                                   \
                        LIB_DEBUG_LOG("next phase %d\n", j);                                           \
                        *row_obj = (*row_obj)->next;                                                   \
                    }                                                                                  \
                }                                                                                      \
            }                                                                                          \
        }                                                                                              \
        return Mat;                                                                                    \
    }                                                                                                  \
                                                                                                       \
    smat_##type product_smat_##type##_from_T(smat_##type A, smat_##type B)                             \
    {                                                                                                  \
        LIB_DEBUG_LOG("product smat " #type " start.\n");                                              \
        int n1 = A->n;                                                                                 \
        int m1 = A->m;                                                                                 \
        int n2 = B->n;                                                                                 \
        int m2 = B->m;                                                                                 \
        if (m1 != m2) {                                                                                \
            /*printf("matrix (%d,%d)*(%d,%d) product size error...\n", m1, n1, m2, n2);                \
            exit(1);  */                                                                               \
            return new_smat_##type(0, 0);                                                              \
        }                                                                                              \
        smat_##type Mat = new_smat_##type(n1, n2);                                                     \
        for (int j = 0; j < m2; j++) {                                                                 \
            slist_pair_##type b = *array_slist_pair_##type##_at(B->value, j);                          \
            for (int i = 0; i < n1; i++) {                                                             \
                /* 行方向に探索  */                                                              \
                slist_pair_##type a = *array_slist_pair_##type##_at(A->value, i);                      \
                T v = svector_inner_product_##type(                                                    \
                    a, b);                                                                             \
                if (v != ZERO) {                                                                       \
                    append_smat_##type(Mat, i, j, v);                                                  \
                    LIB_DEBUG_LOG("%d,%d," PRINT_FORMAT "\n", i, j, v);                                \
                } else {                                                                               \
                    LIB_DEBUG_LOG("%d,%d,None\n", i, j);                                               \
                }                                                                                      \
            }                                                                                          \
        }                                                                                              \
        return Mat;                                                                                    \
    }                                                                                                  \
                                                                                                       \
    void print_smat_##type(smat_##type Mat)                                                            \
    {                                                                                                  \
        PRINT_DEBUG_LOG("print smat " #type " called.\n");                                             \
        printf("%d %d\n", Mat->n, Mat->m);                                                             \
        print_array_slist_pair_##type(Mat->value);                                                     \
    }                                                                                                  \
                                                                                                       \
    void print_full_smat_##type(smat_##type Mat)                                                       \
    {                                                                                                  \
        PRINT_DEBUG_LOG("print smat as full matrix start\n");                                          \
        int m = Mat->m;                                                                                \
        int n = Mat->n;                                                                                \
        for (int i = 0; i < n; i++) {                                                                  \
            slobj_pair_##type obj = (*array_slist_pair_##type##_at(Mat->value, i))->head;              \
            for (int j = 0; j < m; j++) {                                                              \
                if (obj == NULL) {                                                                     \
                    printf(PRINT_FORMAT, ZERO);                                                        \
                } else if (obj->value.id == j) {                                                       \
                    printf(PRINT_FORMAT, obj->value.value);                                            \
                    obj = obj->next;                                                                   \
                } else {                                                                               \
                    printf(PRINT_FORMAT, ZERO);                                                        \
                }                                                                                      \
                printf(DELIMITER);                                                                     \
            }                                                                                          \
            printf("\n");                                                                              \
        }                                                                                              \
    }                                                                                                  \
                                                                                                       \
    smat_##type power_smat_##type(smat_##type A, int k)                                                \
    {                                                                                                  \
        smat_##type X = identity_smat_##type(A->n);                                                    \
        for (int i = 0; i < k; i++) {                                                                  \
            smat_##type Y = product_smat_##type(A, X);                                                 \
            free_smat_##type(X);                                                                       \
            X = Y;                                                                                     \
        }                                                                                              \
        return X;                                                                                      \
    }
