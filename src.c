#include <math.h>
#include <dlfcn.h>
#include <sil_ext.h>

#define math_unop(_op) \
int _op(struct sil_State *S) { \
    double x = sil_todouble(S, 1); \
    double (*real_op)(double) = dlsym(RTLD_NEXT, #_op); \
    sil_pop(S, 1); \
    sil_pushdouble(S, real_op(x)); \
    return 0; \
}

#define math_binop(_op) \
int _op(struct sil_State *S) { \
    double x = sil_todouble(S, 1); \
    double y = sil_todouble(S, 2); \
    double (*real_op)(double, double) = dlsym(RTLD_NEXT, #_op); \
    sil_pop(S, 2); \
    sil_pushdouble(S, real_op(x, y)); \
    return 0; \
}

#define math_nunop(_op) \
int _op(struct sil_State *S) { \
    int   n = sil_tointeger(S, 1); \
    double x = sil_todouble(S, 2); \
    double (*real_op)(int, double) = dlsym(RTLD_NEXT, #_op); \
    sil_pop(S, 2); \
    sil_pushdouble(S, real_op(n, x)); \
    return 0; \
}

// create unary operations
math_unop(sin);
math_unop(cos);
math_unop(tan);
math_unop(asin);
math_unop(acos);
math_unop(atan);
math_unop(sinh);
math_unop(cosh);
math_unop(tanh);
math_unop(asinh);
math_unop(acosh);
math_unop(atanh);
math_unop(exp);
math_unop(exp2);
math_unop(exp10);
math_unop(pow1);
math_unop(log);
math_unop(log10);
math_unop(log2);
math_unop(logb);
math_unop(sqrt);
math_unop(cbrt);
math_unop(expm1);
math_unop(log1p);
math_unop(erf);
math_unop(erfc);
math_unop(j0);
math_unop(j1);
math_unop(y0);
math_unop(y1);

// create int, double operations
math_nunop(jn);
math_nunop(yn);

// create double, double operations
math_binop(pow);
math_binop(atan2);
math_binop(hypot);

// rename these functions
int gamma(struct sil_State *S) {
    double x = sil_todouble(S, 1);
    double (*real_op)(double, int *) = dlsym(RTLD_NEXT, "tgamma");
    sil_pop(S, 1);
    sil_pushdouble(S, real_op(x));
    return 0;
}

// use thread-safe version
int lgamma(struct sil_State *S) {
    int signp;
    double x = sil_todouble(S, 1);
    double (*real_op)(double, int *) = dlsym(RTLD_NEXT, "lgamma_r");
    sil_pop(S, 1);
    sil_pushdouble(S, real_op(x, &signp));
    return 0;
}
