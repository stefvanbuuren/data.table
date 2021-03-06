#include <R.h>
#define USE_RINTERNALS
#include <Rinternals.h>
// #include <signal.h> // the debugging machinery + breakpoint aidee
// raise(SIGINT);
#include <stdint.h> // for uint64_t rather than unsigned long long
#include "myomp.h"

// data.table depends on R>=3.0.0 when R_xlen_t was introduced
// Before R 3.0.0, RLEN used to be switched to R_len_t as R_xlen_t wasn't available.
// We could now replace all RLEN with R_xlen_t directly. Or keep RLEN for the shorter
// name so as not to have to check closely one letter difference R_xlen_t/R_len_t. We
// might also undefine R_len_t to ensure not to use it.
typedef R_xlen_t RLEN;

#define IS_UTF8(x)  (LEVELS(x) & 8)
#define IS_ASCII(x) (LEVELS(x) & 64)
#define IS_LATIN(x) (LEVELS(x) & 4)

#define SIZEOF(x) sizes[TYPEOF(x)]

#ifdef MIN
#undef MIN
#endif
#define MIN(a,b) (((a)<(b))?(a):(b))

#ifdef MAX
#undef MAX
#endif
#define MAX(a,b) (((a)>(b))?(a):(b))

// Backport macros added to R in 2017 so we don't need to update dependency from R 3.0.0
#ifndef MAYBE_SHARED
# define MAYBE_SHARED(x) (NAMED(x) > 1)
#endif
#ifndef MAYBE_REFERENCED
# define MAYBE_REFERENCED(x) ( NAMED(x) > 0 )
#endif

// init.c
void setSizes();
SEXP char_integer64;
SEXP char_ITime;
SEXP char_IDate;
SEXP char_Date;
SEXP char_POSIXct;
SEXP char_nanotime;
SEXP sym_sorted;
SEXP sym_BY;
SEXP sym_starts, char_starts;
SEXP sym_maxgrpn;
Rboolean INHERITS(SEXP x, SEXP char_);
long long DtoLL(double x);
double LLtoD(long long x);
double NA_INT64_D;
long long NA_INT64_LL;

// dogroups.c
SEXP keepattr(SEXP to, SEXP from);
SEXP growVector(SEXP x, R_len_t newlen);
size_t sizes[100];  // max appears to be FUNSXP = 99, see Rinternals.h
SEXP SelfRefSymbol;

// assign.c
SEXP allocNAVector(SEXPTYPE type, R_len_t n);
void savetl_init(), savetl(SEXP s), savetl_end();
Rboolean isDatatable(SEXP x);

// forder.c
int StrCmp(SEXP x, SEXP y);
unsigned long long dtwiddle(void *p, int i, int order);
unsigned long long i64twiddle(void *p, int i, int order);
unsigned long long (*twiddle)(void *, int, int);
SEXP forder(SEXP DT, SEXP by, SEXP retGrp, SEXP sortStrArg, SEXP orderArg, SEXP naArg);

// reorder.c
SEXP reorder(SEXP x, SEXP order);

// fcast.c
SEXP int_vec_init(R_len_t n, int val);

// vecseq.c
SEXP vecseq(SEXP x, SEXP len, SEXP clamp);

// uniqlist.c
SEXP uniqlist(SEXP l, SEXP order);
SEXP uniqlengths(SEXP x, SEXP n);

// chmatch.c
SEXP chmatch(SEXP x, SEXP table, R_len_t nomatch, Rboolean in);

SEXP isOrderedSubset(SEXP, SEXP);
void setselfref(SEXP);

// fmelt.c
SEXP seq_int(int n, int start);
SEXP set_diff(SEXP x, int n);
SEXP which(SEXP x, Rboolean val);

// frank.c
SEXP dt_na(SEXP x, SEXP cols);

// assign.c
SEXP alloccol(SEXP dt, R_len_t n, Rboolean verbose);
void memrecycle(SEXP target, SEXP where, int r, int len, SEXP source);
SEXP shallowwrapper(SEXP dt, SEXP cols);

SEXP dogroups(SEXP dt, SEXP dtcols, SEXP groups, SEXP grpcols, SEXP jiscols,
                SEXP xjiscols, SEXP grporder, SEXP order, SEXP starts,
                SEXP lens, SEXP jexp, SEXP env, SEXP lhs, SEXP newnames,
                SEXP on, SEXP verbose);

// bmerge.c
SEXP bmerge(SEXP iArg, SEXP xArg, SEXP icolsArg, SEXP xcolsArg, SEXP isorted,
                SEXP xoArg, SEXP rollarg, SEXP rollendsArg, SEXP nomatchArg,
                SEXP multArg, SEXP opArg, SEXP nqgrpArg, SEXP nqmaxgrpArg);
SEXP ENC2UTF8(SEXP s);

// rbindlist.c
SEXP combineFactorLevels(SEXP factorLevels, int *factorType, Rboolean *isRowOrdered);

// quickselect
double dquickselect(double *x, int n, int k);
double iquickselect(int *x, int n, int k);

// fread.c
double wallclock();

// openmp-utils.c
int getDTthreads();
void avoid_openmp_hang_within_fork();

