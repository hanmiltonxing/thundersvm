//
// Created by jiashuai on 17-9-21.
//

#ifndef THUNDERSVM_SMO_KERNEL_H
#define THUNDERSVM_SMO_KERNEL_H

#include <thundersvm/clion_cuda.h>
#include <thundersvm/thundersvm.h>

__host__ __device__

bool is_I_up(float a, float y, float Cp, float Cn);

__host__ __device__

bool is_I_low(float a, float y, float Cp, float Cn);

__host__ __device__

bool is_free(float a, float y, float Cp, float Cn);

__global__ void
c_smo_solve_kernel(const int *label, real *f_values, real *alpha, real *alpha_diff, const int *working_set, int ws_size,
                   float Cp, float Cn, const float *k_mat_rows, const float *k_mat_diag, int row_len, real eps,
                   real *diff_and_bias, int max_iter);

__global__ void
nu_smo_solve_kernel(const int *label, real *f_values, real *alpha, real *alpha_diff, const int *working_set,
                    int ws_size, float C, const float *k_mat_rows, const float *k_mat_diag, int row_len, real eps,
                    real *diff_and_bias, int max_iter);

__global__ void update_f(real *f, int ws_size, const real *alpha_diff, const real *k_mat_rows, int n_instances);

#endif //THUNDERSVM_SMO_KERNEL_H
