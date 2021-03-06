#define powell3_N         2
#define powell3_P         2

static double powell3_x0[powell3_P] = { 0.0, 1.0 };
static double powell3_epsrel = 1.0e-8;

static double powell3_J[powell3_N * powell3_P];

static void
powell3_checksol(const double x[], const double sumsq,
                 const double epsrel, const char *sname,
                 const char *pname)
{
  size_t i;
  const double sumsq_exact = 0.0;
  const double powell3_x[powell3_P] = { 1.09815932969975976e-05,
                                        9.10614673986700218 };

  gsl_test_rel(sumsq, sumsq_exact, epsrel, "%s/%s sumsq",
               sname, pname);

  for (i = 0; i < powell3_P; ++i)
    {
      gsl_test_rel(x[i], powell3_x[i], epsrel, "%s/%s i=%zu",
                   sname, pname, i);
    }
}

static int
powell3_f (const gsl_vector * x, void *params, gsl_vector * f)
{
  double x1 = gsl_vector_get(x, 0);
  double x2 = gsl_vector_get(x, 1);

  gsl_vector_set(f, 0, 1.0e4*x1*x2 - 1.0);
  gsl_vector_set(f, 1, exp(-x1) + exp(-x2) - 1.0001);

  (void)params; /* avoid unused parameter warning */

  return GSL_SUCCESS;
}

static int
powell3_df (CBLAS_TRANSPOSE_t TransJ, const gsl_vector * x,
            const gsl_vector * u, void * params, gsl_vector * v,
            gsl_matrix * JTJ)
{
  gsl_matrix_view J = gsl_matrix_view_array(powell3_J, powell3_N, powell3_P);
  double x1 = gsl_vector_get(x, 0);
  double x2 = gsl_vector_get(x, 1);

  gsl_matrix_set(&J.matrix, 0, 0, 1.0e4*x2);
  gsl_matrix_set(&J.matrix, 0, 1, 1.0e4*x1);

  gsl_matrix_set(&J.matrix, 1, 0, -exp(-x1));
  gsl_matrix_set(&J.matrix, 1, 1, -exp(-x2));

  if (v)
    gsl_blas_dgemv(TransJ, 1.0, &J.matrix, u, 0.0, v);

  if (JTJ)
    gsl_blas_dsyrk(CblasLower, CblasTrans, 1.0, &J.matrix, 0.0, JTJ);

  (void)params; /* avoid unused parameter warning */

  return GSL_SUCCESS;
}

static int
powell3_fvv (const gsl_vector * x, const gsl_vector * v,
             void *params, gsl_vector * fvv)
{
  double x1 = gsl_vector_get(x, 0);
  double x2 = gsl_vector_get(x, 1);
  double v1 = gsl_vector_get(v, 0);
  double v2 = gsl_vector_get(v, 1);

  gsl_vector_set(fvv, 0, 2.0e4 * v1 * v2);
  gsl_vector_set(fvv, 1, v1*v1*exp(-x1) + v2*v2*exp(-x2));

  (void)params; /* avoid unused parameter warning */

  return GSL_SUCCESS;
}

static gsl_multilarge_nlinear_fdf powell3_func =
{
  powell3_f,
  powell3_df,
  powell3_fvv,
  powell3_N,
  powell3_P,
  NULL,
  0,
  0,
  0,
  0
};

static test_fdf_problem powell3_problem =
{
  "powell_badly_scaled",
  powell3_x0,
  NULL,
  &powell3_epsrel,
  &powell3_checksol,
  &powell3_func
};
