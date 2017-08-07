#include <dlfcn.h>
#include <sil_ext.h>

typedef double (*unop)(double);
typedef double (*nunop)(int, double);
typedef double (*binop)(double, double);
typedef int (*intfop)(double);
typedef int (*intop)(int);

#define math_unop(_op) \
int _op(sil_State *S) { \
    double x = sil_todouble(S, 1); \
    unop real_op = (unop)dlsym(RTLD_NEXT, #_op); \
    sil_pop(S, 1); \
    sil_pushdouble(S, real_op(x)); \
    return 0; \
}

#define math_binop(_op) \
int _op(sil_State *S) { \
    double x = sil_todouble(S, 1); \
    double y = sil_todouble(S, 2); \
    binop real_op = (binop)dlsym(RTLD_NEXT, #_op); \
    sil_pop(S, 2); \
    sil_pushdouble(S, real_op(x, y)); \
    return 0; \
}

#define math_nunop(_op) \
int _op(sil_State *S) { \
    int   n = sil_tointeger(S, 1); \
    double x = sil_todouble(S, 2); \
    nunop real_op = (nunop)dlsym(RTLD_NEXT, #_op); \
    sil_pop(S, 2); \
    sil_pushdouble(S, real_op(n, x)); \
    return 0; \
}

#define math_intfop(_op) \
int _op(sil_State *S) { \
    double x = sil_todouble(S, 1); \
    intfop real_op = (intfop)dlsym(RTLD_NEXT, #_op); \
    sil_pop(S, 1); \
    sil_pushinteger(S, real_op(x)); \
    return 0; \
}

// fake integer operation
#define math_fintop(_op) \
int _op(sil_State *S) { \
    double x = sil_todouble(S, 1); \
    unop real_op = (unop)dlsym(RTLD_NEXT, #_op); \
    sil_pop(S, 1); \
    sil_pushinteger(S, (int)real_op(x)); \
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
//math_unop(exp10);
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
math_unop(fabs);

math_intfop(ilogb);
math_fintop(floor);
math_fintop(ceil);
math_fintop(round);

// create int, double operations
math_nunop(jn);
math_nunop(yn);

// create double, double operations
math_binop(pow);
math_binop(atan2);
math_binop(hypot);

// rename these functions
int gamma(sil_State *S) {
    double x = sil_todouble(S, 1);
    unop real_op = (unop)dlsym(RTLD_NEXT, "tgamma");
    sil_pop(S, 1);
    sil_pushdouble(S, real_op(x));
    return 0;
}

// use thread-safe version
typedef double (*lgamop)(double, int *);
int lgamma(sil_State *S) {
    int signp;
    double x = sil_todouble(S, 1);
    lgamop real_op = (lgamop)dlsym(RTLD_NEXT, "lgamma_r");
    sil_pop(S, 1);
    sil_pushdouble(S, real_op(x, &signp));
    return 0;
}

int mod(sil_State *S) {
    int a = sil_tointeger(S, 1);
    int b = sil_tointeger(S, 2);
    sil_pop(S, 2);
    sil_pushinteger(S, ((a%b)+b) % b);
    return 0;
}

int fmod(sil_State *S) {
    double x = sil_todouble(S, 1);
    double y = sil_todouble(S, 2);
    binop real_op = (binop)dlsym(RTLD_NEXT, "fmod");
    sil_pop(S, 2);
    sil_pushdouble(S, real_op(real_op(x, y)+y, y));
    return 0;
}


