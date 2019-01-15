/*
* @Author: kmrocki
* @Date:   2016-11-07 17:56:01
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-07 20:01:23
*
* Author: Kamil Rocki <kmrocki@us.ibm.com>
*
* Copyright (c) 2016, IBM Corporation. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*
*
* PRECISE_MATH will enable double type and things
* such as gradient check
*
*/

#ifndef __DTYPE_H_
#define __DTYPE_H_

#ifdef __PRECISE_MATH__

#define dtype double
#define cl_dtype cl_double

#define cblas_gemm cblas_dgemm

/* use appropriate versions of functions for floats */
#define _log2 log2
#define _log log
#define _tanh tanh
#define _pow pow
#define _exp exp
#define _sqrt sqrt
#define _fabs fabs

#else /* PRECISE_MATH not defined */

#define dtype float
#define cl_dtype cl_float

#define cblas_gemm cblas_sgemm

/* use appropriate versions of functions for floats */
#define _log2 log2f
#define _log logf
#define _tanh tanhf
#define _pow powf
#define _exp expf
#define _sqrt sqrtf
#define _fabs fabsf

#endif  /* __PRECISE_MATH__ */

#endif /* __DTYPE_H_ */
