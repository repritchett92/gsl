/* blas/source_sbmv.h
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Gerard Jungman
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Author:  G. Jungman
 * RCS:     $Id$
 */

{
    size_t i, j;
    size_t ix, iy, jx, jy;

    if (alpha == 0.0 && beta == 1.0)
	return;

    /* form  y := beta*y */
    if (beta == 0.0) {
        size_t iy = OFFSET(N, incY);
	for (i = 0; i < N; i++) {
	    Y[iy] = 0.0;
	    iy += incY;
	}
    } else if (beta != 1.0) {
	size_t iy = OFFSET(N, incY);
	for (i = 0; i < N; i++) {
	    Y[iy] *= beta;
	    iy += incY;
	}
    }

    if (alpha == 0.0)
	return;

    /* form  y := alpha*A*x + y */

    if (order == CblasRowMajor && Uplo == CblasUpper) {
      size_t ix = OFFSET(N, incX);
      size_t iy = OFFSET(N, incY);

      for (i = 0; i < N; i++) {
	    BASE tmp1 = alpha * X[ix];
	    BASE tmp2 = 0.0;
            const size_t j_min = i + 1;
            const size_t j_max = GSL_MIN(N, i + K + 1);
            size_t jx = OFFSET(N, incX) + j_min * incX;
            size_t jy = OFFSET(N, incY) + j_min * incY;
	    Y[iy] += tmp1 * A[lda * K + i];
	    for (j = j_min; j < j_max; j++) {
              BASE Aij = A[lda * (K+i-j) + j];
		Y[jy] += tmp1 * Aij;
		tmp2 += Aij * X[jx];
                jx += incX;
                jy += incY;
	    }
	    Y[iy] += alpha * tmp2;
	    ix += incX;
	    iy += incY;
	}
    } else if (order == CblasColMajor && Uplo == CblasLower) {
      size_t ix = OFFSET(N, incX);
      size_t iy = OFFSET(N, incY);

      for (i = 0; i < N; i++) {
	    BASE tmp1 = alpha * X[ix];
	    BASE tmp2 = 0.0;
            const size_t j_min = (K > i) ? 0 : i - K;
            const size_t j_max = i;
            size_t jx = OFFSET(N, incX) + j_min * incX;
            size_t jy = OFFSET(N, incY) + j_min * incY;
	    Y[iy] += tmp1 * A[0 + lda * i];
	    for (j = j_min; j < j_max; j++) {
              BASE Aij = A[(i-j) + lda * j];
		Y[jy] += tmp1 * Aij;
		tmp2 += Aij * X[jx];
                jx += incX;
                jy += incY;
	    }
	    Y[iy] += alpha * tmp2;
	    ix += incX;
	    iy += incY;
	}

    }  else if (order == CblasRowMajor && Uplo == CblasLower) {
      size_t ix = OFFSET(N, incX);
      size_t iy = OFFSET(N, incY);

      for (i = 0; i < N; i++) {
	    BASE tmp1 = alpha * X[ix];
	    BASE tmp2 = 0.0;
            const size_t j_min = (K > i) ? 0 : i - K;
            const size_t j_max = i;
            size_t jx = OFFSET(N, incX) + j_min * incX;
            size_t jy = OFFSET(N, incY) + j_min * incY;
	    Y[iy] += tmp1 * A[lda * 0 +  i];
	    for (j = j_min; j < j_max; j++) {
              BASE Aij = A[lda * (i-j) + j];
		Y[jy] += tmp1 * Aij;
		tmp2 += Aij * X[jx];
                jx += incX;
                jy += incY;
	    }
	    Y[iy] += alpha * tmp2;
	    ix += incX;
	    iy += incY;
	}
    } else if (order == CblasColMajor && Uplo == CblasUpper) {
      size_t ix = OFFSET(N, incX);
      size_t iy = OFFSET(N, incY);

      for (i = 0; i < N; i++) {
	    BASE tmp1 = alpha * X[ix];
	    BASE tmp2 = 0.0;
            const size_t j_min = i + 1;
            const size_t j_max = GSL_MIN(N, i + K + 1);
            size_t jx = OFFSET(N, incX) + j_min * incX;
            size_t jy = OFFSET(N, incY) + j_min * incY;
	    Y[iy] += tmp1 * A[K + lda * i];
	    for (j = j_min; j < j_max; j++) {
              BASE Aij = A[(K+i-j) + lda * j];
		Y[jy] += tmp1 * Aij;
		tmp2 += Aij * X[jx];
                jx += incX;
                jy += incY;
	    }
	    Y[iy] += alpha * tmp2;
	    ix += incX;
	    iy += incY;
	}
    } else {
      BLAS_ERROR ("unrecognized operation");
    }

}
