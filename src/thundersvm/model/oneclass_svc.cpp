//
// Created by jiashuai on 17-10-6.
//

#include <iostream>
#include <thundersvm/model/oneclass_svc.h>
#include <thundersvm/solver/csmosolver.h>

void OneClassSVC::train(const DataSet &dataset, SvmParam param) {
    model_setup(dataset, param);
    int n_instances = dataset.n_instances();
    SyncData<real> alpha(n_instances);
    SyncData<real> f_val(n_instances);

    KernelMatrix kernelMatrix(dataset.instances(), param);

    alpha.mem_set(0);
    int n = static_cast<int>(param.nu * n_instances);
    for (int i = 0; i < n; ++i) {
        alpha[i] = 1;
    }
    if (n < n_instances)
        alpha[n] = param.nu * n_instances - n;
    int ws_size = min(min(max2power(n), max2power(n_instances - n)) * 2, 1024);

    //TODO batch, thrust
    f_val.mem_set(0);
    SyncData<int> y(n_instances);
    for (int i = 0; i < n_instances; ++i) {
        y[i] = 1;
    }
    CSMOSolver solver;
    solver.solve(kernelMatrix, y, alpha, rho[0], f_val, param.epsilon, 1, 1, ws_size);

    //todo these codes are similar to svr, try to combine them
    LOG(INFO) << "rho = " << rho[0];
    vector<real> coef_vec;
    for (int i = 0; i < n_instances; ++i) {
        if (alpha[i] != 0) {
            sv.push_back(dataset.instances()[i]);
            coef_vec.push_back(alpha[i]);
        }
    }
    LOG(INFO) << "#sv = " << sv.size();
    n_sv[0] = sv.size();
    n_sv[1] = 0;
    coef.resize(coef_vec.size());
    coef.copy_from(coef_vec.data(), coef_vec.size());
}

vector<real> OneClassSVC::predict(const DataSet::node2d &instances, int batch_size) {
    vector<real> dec_values = SvmModel::predict(instances, batch_size);
    vector<real> predict_y;
    for (int i = 0; i < dec_values.size(); ++i) {
        predict_y.push_back(dec_values[i] > 0 ? 1 : -1);
    }
    return predict_y;
}


