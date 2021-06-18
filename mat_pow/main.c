// 対象な0,1行列の2乗を高速に計算する
#include "slist.h"
#include <time.h>

#define COMPARE(p, q) ((p) - (q))

define_slist(int, i, free, COMPARE);

typedef struct pair_ {
    int id;
    int v;
} pair;


#define ALWAYS_TRUE(x, y) true

define_slist(pair, p, free, ALWAYS_TRUE);

#define free_slist(type, lis)              \
    {                                      \
        slobj_##type obj = (lis)->head;    \
        while (obj != NULL) {              \
            slobj_##type next = obj->next; \
            free(obj);                     \
            obj = next;                    \
        }                                  \
        free(lis);                         \
    }

int main()
{
    clock_t t0, tf;
    t0 = clock();

    // サイズ読み込み
    int n, m;
    scanf("%d %d", &n, &m);

    // 入力行列記憶用
    slist_i* M = malloc(n * sizeof(slist_i));
    // 出力行列記録用
    slist_p* ans = malloc(n * sizeof(slist_p));
    for (int i = 0; i < n; i++) {
        M[i] = new_slist_i();
        ans[i] = new_slist_p();
    }


    // 内積の値を記録する用
    int* tmp = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        tmp[i] = 0;
    }

    // 非ゼロ成分記録用
    slist_i components = new_slist_i();
    for (int i = 0; i < n; i++) {
        // 対称成分の例外処理
        tmp[i] = 1;

        // 行の入力受付
        int j;
        scanf("%d", &j);
        while (j > EOF) {  // ここの条件分岐はbit演算とか色々試したけどこれが速いんちゃうかな思います
            double v;
            j--;
            // 受け取った成分を記録
            append_slist_i(M[j], &i);

            // 受け取った成分との掛け算を計算
            slobj_i obj = M[j]->head;
            while (obj != NULL) {
                int k = obj->value;
                // 非ゼロ成分のリストに加える
                if (!tmp[k]) {
                    append_slist_i(components, &k);
                }
                tmp[k]++;
                obj = obj->next;
            }

            scanf("%lf", &v);
            scanf("%d", &j);
        }

        // i行 xor i列の成分を記録する
        slist_i idx = sort_slist_i(components);
        slobj_i obj = idx->head;
        slobj_i obj_ptr;
        while (obj != NULL) {
            int j = obj->value;

            pair p;
            p.id = j;
            p.v = tmp[j];
            // i行j列に記録
            append_slist_p(ans[i], &p);

            p.id = i;
            // j行i列に記録
            append_slist_p(ans[j], &p);

            obj_ptr = obj;
            obj = obj->next;
            free(obj_ptr);
            // 内積計算の初期化
            tmp[j] = 0;
        }
        // 非ゼロ成分記録リストの初期化
        components->head = NULL;
        components->tail = NULL;

        // i行i列の成分を記録
        if (tmp[i] != 1) {
            pair p;
            p.id = i;
            p.v = tmp[i] - 1;
            append_slist_p(ans[i], &p);
        }
        //内積計算の初期化
        tmp[i] = 0;
    }

    tf = clock();
    fprintf(stderr, "input: %d\n", tf - t0);
    t0 = tf;

    printf("%d %d\n", n, n);
    for (int i = 0; i < n; i++) {
        slobj_p obj = ans[i]->head;
        while (obj != NULL) {
            // 出力
            printf("%d %d.000000 ", obj->value.id + 1, obj->value.v);
            obj = obj->next;
        }
        printf("-1\n");
    }

    tf = clock();
    fprintf(stderr, "output: %d\n", tf - t0);
    t0 = tf;

    // freeにする
    for (int i = 0; i < n; i++) {
        free_slist(i, ans[i]);
        free_slist(i, M[i]);
    }
    free(M);
    free(ans);
    free(tmp);
    free(components);

    tf = clock();
    fprintf(stderr, "free: %d\n", tf - t0);
}
