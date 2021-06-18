#include "lib/mat.h"

#define TIMES(x, y) ((x) * (y))

#define COMPARE(x, y) ((x.id) - (y.id))
#define EQUAL(x, y) ((bool)(x.value == y.value))

define_sparce_matrix(double, d, free, "%lf", " ", TIMES, COMPARE, EQUAL, 1.0, 0.0);

int main()
{
    smat_d A = load_input_smat_d();

    smat_d B = product_smat_d(A, A);
    print_smat_d(B);

    free_smat_d(A);
    free_smat_d(B);
}

