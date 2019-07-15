/*
 * FAUST.h
 *
 *  Created on: 27 Apr 2018
 *      Author: natchi
 */

#ifndef LEARNING_FAUST_H_
#define LEARNING_FAUST_H_

#include <SHS.h>
#include <armadillo>
#include <ginac/ginac.h>

class faust_t {
public:
  // attributes
  arma::mat X; // The state space
  arma::mat U; // The control actions
  std::vector<arma::mat> Tp; // Transition probability matrix for each discrete mode
  double E; // The maximal error generated by abstraction
  GiNaC::ex Kernel; // The symbolic Kernel as a function of x, u
  GiNaC::ex KernelInternal;
  shs_t<arma::mat, int> model; // The original state space model from the symbolic kernel is constructed
  double constant;
  arma::mat V; // The solution of the verification problem
  arma::umat OptimalPol; // The optimal policy generated
  // Functions
  faust_t();
  ~faust_t();
  faust_t(shs_t<arma::mat, int> &inModel);
  void myKernel(shs_t<arma::mat, int> &inModel);
  void myKernel(std::vector<arma::mat> x);
  void myKernel(std::vector<arma::mat> x, double constant);
  void myKernel(std::vector<arma::mat> x, shs_t<arma::mat, int> &inModel);
  void myKernel(std::vector<arma::mat> x, shs_t<arma::mat, int> &inModel,
                int dim, int dim_u);
  double GlobalLipschitz(arma::mat SafeSet, bool Xbar);
  arma::mat LocalLipschitz(int xl, int xu);
  arma::mat LocalLipschitzXbar(int xl, int xu);
  arma::mat SemiLocalLipschitz(arma::mat SafeSet, int q, int m);
  // Markov Chain scenario
  void Uniform_grid(double epsilon, double T, arma::mat SafeSet);
  void Uniform_grid_MCapprox(double epsilon, double T, arma::mat SafeSet);
  void Uniform_grid_ReachAvoid(double epsilon, double T, arma::mat SafeSet,
                               arma::mat TargetSet);
  void Uniform_grid_ReachAvoid_MCapprox(double epsilon, double T,
                                        arma::mat SafeSet, arma::mat TargetSet);
  void MCapprox(double epsilon);
  void MCapprox(double epsilon, shs_t<arma::mat, int> &model);
  void MCcreator(double epsilon);
  void MCapprox_Contr(double epsilon, shs_t<arma::mat, int> &model);
  void MCcreator_Contr(double epsilon);
  void Adaptive_grid_multicell(double epsilon, double T);
  void StandardProbSafety(int modes, arma::mat Tq, std::vector<arma::mat> Tpvec, int T);
  void StandardProbSafety(int T);
  void StandardReachAvoid(int modes, arma::mat Tq, std::vector<arma::mat> Tpvec,
                          arma::mat X, arma::mat Target, int elSmallest, int T);
  void StandardReachAvoid(arma::mat Target, int T);
  void Adaptive_grid_multicell_semilocal(double epsilon, double T,
                                         arma::mat SafeSet);
  void Adaptive_grid_ReachAvoid(double epsilon, double T, arma::mat SafeSet,
                                arma::mat TargetSet);
  void Adaptive_grid_ReachAvoid_semilocal(double epsilon, double T,
                                          arma::mat SafeSet,
                                          arma::mat TargetSet);
  void Adaptive_grid_MCapprox(double epsilon, double T, arma::mat SafeSet);
  void Adaptive_grid_ReachAvoidMCapprox(double epsilon, double T,
                                        arma::mat SafeSet, arma::mat TargetSet);
  // Markov Decision process i.e controlled
  double GlobalLipschitz_Contr(arma::mat SafeSet, arma::mat InputSet, int der);
  arma::cube LocalLipschitz_Contr(int xl, int xu, int ul, int uu);
  arma::cube LocalLipschitzToU_Contr(int xl, int xu, int ul, int uu);
  arma::mat SemiLocalLipschitz_Contr(arma::mat SafeSet, int xl, int xu, int ul, int uu);
  arma::mat SemiLocalLipschitzToU_Contr(arma::mat SafeSet, int xl, int xu, int ul, int uu);
  void Uniform_grid_Contr(double epsilon, double T, arma::mat SafeSet,
                          arma::mat InputSet);
  void Uniform_grid_MCapprox_Contr(double epsilon, double T, arma::mat SafeSet,
                                   arma::mat InputSet);
  void Uniform_grid_ReachAvoid_Contr(double epsilon, double T,
                                     arma::mat SafeSet, arma::mat InputSet,
                                     arma::mat TargetSet);
  void Uniform_grid_ReachAvoidMCapprox_Contr(double epsilon, double T,
                                             arma::mat SafeSet,
                                             arma::mat InputSet,
                                             arma::mat TargetSet);
  void StandardProbSafety_Contr(int T);
  void StandardReachAvoid_Contr(arma::mat Target, int T);
  void Adaptive_grid_multicell_Contr(double epsilon, double T,
                                     arma::mat SafeSet, arma::mat InputSet);
  void Adaptive_grid_semilocal_Contr(double epsilon, double T,
                                     arma::mat SafeSet, arma::mat InputSet);
  void Adaptive_grid_ReachAvoid_Contr(double epsilon, double T,
                                      arma::mat SafeSet, arma::mat InputSet,
                                      arma::mat TargetSet);
  void Adaptive_grid_ReachAvoid_semilocal_Contr(double epsilon, double T,
                                                arma::mat SafeSet,
                                                arma::mat InputSet,
                                                arma::mat TargetSet);
  void Adaptive_grid_MCapprox_Contr(double epsilon, double T, arma::mat SafeSet,
                                    arma::mat InputSet);
  void Adaptive_grid_ReachAvoid_MCapprox_Contr(double epsilon, double T,
                                               arma::mat SafeSet,
                                               arma::mat InputSet,
                                               arma::mat TargetSet);
  void export2PRISM(int m, int q, int problem, int N);
  void formatOutput(double time, std::string cT, int Problem, int timeHorizon);
};

static GiNaC::symbol &get_symbol(const std::string &s) {
  static std::map<std::string, GiNaC::symbol> directory;
  std::map<std::string, GiNaC::symbol>::iterator i = directory.find(s);
  if (i != directory.end()) {
    return i->second;
  } else {
    return directory.insert(make_pair(s, GiNaC::symbol(s))).first->second;
  }
}
// TODO: generalise kernel
static int ff_kernel(unsigned ndim, const double *x, void *fdata, unsigned fdim,
                     double *fval) {

  auto *xbar = (std::vector<arma::mat> *)fdata; //  my_func_data;

  arma::mat Xvec(ndim, 1);
  arma::mat Xbvec = (*xbar)[0];
  arma::mat A = (*xbar)[1];
  arma::mat Sm = (*xbar)[2];
  arma::mat Q = (*xbar)[3];

  for (unsigned int i = 0; i < ndim; i++) {
    Xvec(i, 0) = x[i];
  }

  double exter =
      1 / (std::sqrt(std::pow((2 * arma::datum::pi), ndim) * arma::det(Sm)));
  arma::mat basicEx = A * Xvec;
  if(Q.n_rows > 0) {
    basicEx = basicEx + Q;
  }
  arma::mat E_xbar = basicEx - Xbvec.t();
  arma::mat inter = E_xbar.t() * arma::inv(Sm) * E_xbar;
  double res = inter(0, 0);
  fval[0] = exter * std::exp(-0.5 * res);
  return 0;
}

// TODO: generalise kernel
static int ff_kernel_U(unsigned ndim, const double *x, void *fdata,
                       unsigned fdim, double *fval) {

  auto *xbar = (std::vector<arma::mat> *)fdata; //  my_func_data;

  arma::mat Xvec(ndim, 1);
  arma::mat Xbvec = (*xbar)[0];
  arma::mat Uvec = (*xbar)[1];
  arma::mat A = (*xbar)[2];
  arma::mat B = (*xbar)[3];
  arma::mat Sm = (*xbar)[4];
  arma::mat Q = (*xbar)[5];
  arma::mat N = (*xbar)[6];
  for (unsigned int i = 0; i < ndim; i++) {
    Xvec(i, 0) = x[i];
  }
  double exter =
      1 / (std::sqrt(std::pow((2 * arma::datum::pi), ndim) * arma::det(Sm)));

  arma::mat basicEx = A * Xvec + B * Uvec;
  if( N.n_rows != 0 & Q.n_rows != 0) {
     basicEx = basicEx + N * arma::kron(Uvec.col(0), Xvec) + Q;
  }
  else{
    basicEx = basicEx + Q;
  }
  arma::mat E_xbar = basicEx - Xbvec.t();
  arma::mat inter = E_xbar.t() * arma::inv(Sm) * E_xbar;
  double res = inter(0, 0);
  fval[0] = exter * std::exp(-0.5 * res);
  return 0;
}

static std::vector<arma::mat>
StandardProbSafety_Contr(int modes, arma::mat Tq,
                         std::vector<std::vector<arma::mat>> Tpvec,
                         int elSmallest, int T) {
  arma::mat V = arma::ones<arma::mat>(elSmallest, 1);
  arma::mat Solution = arma::ones<arma::mat>(elSmallest, 1);
  arma::mat Vall = arma::ones<arma::mat>(elSmallest, modes);
  arma::mat OptimPol = arma::zeros<arma::mat>(elSmallest, T);
  arma::mat Vaux = arma::zeros<arma::mat>(elSmallest, Tpvec[0].size());
  arma::mat Vt = arma::zeros<arma::mat>(elSmallest, modes);
  std::vector<double> loc;
  // Backward recursion
  for (int i = T; i > 0; i--) {
    for (int q = 0; q < modes; q++) {

      for (unsigned int j = 0; j < Tpvec[0].size(); j++) {
        V = Vall.col(q);
        arma::mat temp2 = repmat(V.t(), elSmallest, 1);
        arma::mat temp = Tpvec[q][j] % temp2;
        Vaux.col(j) = sum(temp, 1);
      }
    }
    if (modes > 1)
      Vt = Vt * Tq;

    Vall = Vt;
    for (int k = 0; k < elSmallest; k++) {
      Solution(k, 0) = Vt.row(k).max();
      arma::uword i = Vt.row(k).index_max();
      OptimPol(k, i - 1) = Vt(i);
    }
  }
  std::vector<arma::mat> result;
  result.push_back(Solution);
  result.push_back(OptimPol);
  return result;
}

// TODO: Extend to SHS with multiple modes
static std::vector<arma::mat>
StandardReachAvoid_Contr(int modes, arma::mat Tq,
                         std::vector<std::vector<arma::mat>> Tpvec, arma::mat X,
                         arma::mat Target, int elSmallest, int T) {
  // STANDARDREACHAVOID_CONTR: Computes the reach avoid probability given N time
  // steps and the transition matrix Tp.
  //   The input needs to be: T=number of time steps and Tp the transition
  //   probabilities of the markov chain.
  arma::mat V = arma::ones<arma::mat>(elSmallest, modes);
  arma::mat Tpsum = arma::zeros<arma::mat>(elSmallest, modes);
  double nr = Tpvec[0][0].n_rows;
  arma::mat OptimPol = arma::zeros<arma::mat>(nr, T);
  // The cardinality
  int m = X.n_rows;

  //  The cardinality of U
  int q = Tpvec.size();

  // Dimension of the system
  int dim = X.n_cols / 2;

  // Initialization of value function W
  arma::mat A = repmat(Target.col(1), 1, m);
  arma::mat Xi = X.cols(1, dim);
  arma::mat C = repmat(Target.col(0), 1, m);

  // Matrices used for the calculation of V
  double nr3 = V.n_rows;
  arma::mat W_help1 = arma::ones<arma::mat>(nr3, 1) - V;
  arma::mat W_help2 = V;

  // The solution
  arma::mat Tp = Tpvec[0][0];
  double nc = V.n_cols;
  arma::mat Vt = arma::zeros<arma::mat>(nr3, nc);
  std::vector<arma::mat> W_aux;
  // The solution
  for (int i = T; i >= 1; i--) {
    for (int j = 0; j < modes; j++) {
      for (unsigned int k = 0; k < Tpvec[j].size(); k++) {
        arma::mat temp = V.col(j).t();
        temp = repmat(temp, m, q);
        Tp = Tpvec[j][k];
        Tp = Tp % temp;
        W_aux.push_back(sum(Tp, 1));
      }
      if (Tpvec[0].size() == 1) {
        Vt = W_aux[0];
      } else {
        arma::mat temp1 = arma::mat(elSmallest, W_aux.size());
        for (unsigned int m = 0; m < W_aux.size(); m++) {
          temp1.col(m) = W_aux[m];
        }
        Vt = max(temp1, 1);
      }

      for (unsigned k = 0; k < (unsigned)V.n_rows; k++) {
        V(k, 0) = Vt.row(k).max();
        arma::uword i = Vt.row(k).index_max();
        OptimPol(k, i - 1) = Vt(i);
      }

      V.col(j) = V.col(j) % (W_help1);
      V.col(j) += W_help2;
    }
    if (modes > 1)
      V = V * Tq;
  }

  std::vector<arma::mat> result;
  result.push_back(V);
  result.push_back(OptimPol);
  return result;
}

// Rectangular grid in N-D space
// replicates the grid vectors to produce the coordinates of a rectangular grid
//  The i-th dimension of the output array Xi are copies  of elements of the
//  grid vector
static std::vector<arma::mat> nDgrid(std::vector<arma::mat> gridVectors) {
  int nDim = gridVectors.size();
  int nIn = gridVectors[nDim-1].n_elem;
  arma::mat siz(1,nDim);
  for(size_t sz = 0; sz < nDim; ++sz) {
    siz(0,sz) = gridVectors[sz].n_elem;
  }
  std::vector<arma::mat> result;
  if (nDim == 0) {
    std::cout << "Empty grid" << std::endl;
    exit(0);
  } else if (nDim == 1) {
    result.push_back(gridVectors[0]);
    if(gridVectors[0].n_rows > 100000) {
      std::cout << "Abstraction being generated is too large, please increase requested max error or chage time horizon" << std::endl;
      exit(0);
    }
  } else if (nDim == 2) {
    arma::mat x = arma::mat(gridVectors[0]);
    arma::mat y = arma::mat(gridVectors[1]).t();
    if((x.n_rows * y.n_rows) > 100000) {
      std::cout << "Abstraction being generated is too large, please increase requested max error or chage time horizon" << std::endl;
      exit(0);
    }
    result.push_back(arma::repelem(x, y.n_rows, y.n_cols));
    result.push_back(arma::repelem(y, x.n_rows, x.n_cols));
  } else {
    int size = 1;
    for (int i = 0; i < nDim; i++) {
      arma::mat x = arma::mat(gridVectors[i]);
      arma::mat s = arma::ones<arma::mat>(1, nDim);
      size *= x.n_rows;
      if((size) > 100000) {
        std::cout << "Abstraction being generated is too large, please increase requested max error or chage time horizon" << std::endl;
        exit(0);
      }
      s.col(0) = x.n_elem;
      x = arma::reshape(x, s(0,0), s(0,1));
      s = siz;
      s.col(0) = 1;
      arma::mat stemp = repmat(x, s(0,0), s(0,1));
      result.push_back(stemp);
    }

  }
  return result;
}


// Rectangular grid in N-D space
// replicates the grid vectors to produce the coordinates of a rectangular grid
//  The i-th dimension of the output array Xi are copies  of elements of the
//  grid vector in the form of a cube
static arma::cube nDgrid_cb(std::vector<arma::mat> gridVectors) {
  int nDim = gridVectors.size();
  int nIn = gridVectors[nDim-1].n_elem;
  arma::mat siz(1,nDim);
  for(size_t sz = 0; sz < nDim; ++sz) {
    siz(0,sz) = gridVectors[sz].n_elem;
  }
  std::vector<arma::mat> res1;
  if (nDim == 0) {
    std::cout << "Empty grid" << std::endl;
    exit(0);
  } else if (nDim == 1) {
    arma::cube result(gridVectors[0].n_rows, gridVectors[0].n_cols, 1);
    if(gridVectors[0].n_rows > 100000) {
      std::cout << "Abstraction being generated is too large, please increase requested max error or chage time horizon" << std::endl;
      exit(0);
    }
    result.slice(0) = (gridVectors[0]);
  } else if (nDim == 2) {
    arma::mat x = arma::mat(gridVectors[0]);
    arma::mat y = arma::mat(gridVectors[1]).t();
    if((x.n_rows * y.n_rows) > 100000) {
      std::cout << "Abstraction being generated is too large, please increase requested max error or chage time horizon" << std::endl;
      exit(0);
    }
    arma::cube result(gridVectors[0].n_rows, gridVectors[0].n_cols, 1);
    result.slice(0) = (arma::repelem(x, y.n_rows, y.n_cols));
    result.slice(1) = arma::repelem(y, x.n_rows, x.n_cols);
  } else {
    int size = 1;
    for (int i = 0; i < nIn; i++) {
      arma::mat x = arma::mat(gridVectors[0]);
      size *= x.n_rows;
      if((size) > 100000) {
        std::cout << "Abstraction being generated is too large, please increase requested max error or chage time horizon" << std::endl;
        exit(0);
      }
      arma::mat s = arma::ones<arma::mat>(1, nDim);
      s.col(0) = x.n_elem;
      x = arma::reshape(x, s(0,0), s(0,1));
      s = siz;
      s.col(0) = 1;
      arma::mat stemp = repmat(x, s(0,0), s(0,1));
      res1.push_back(stemp);
    }
  }
  arma::cube result(res1[0].n_rows, res1[0].n_cols, res1.size());
  for(size_t i = 0; i < nIn; i++) {
    result.slice(i) = res1[i];
  }
  return result;
}

// Extend division reminder to vectors
// a       Dividend
// n       Divisor
template <typename T> T mod(T a, T n) { return a - floor(a / n) * n; }

static arma::mat modM(arma::mat a, arma::mat n) { return a - floor(a / n) % n; }
//  all possible combinations of N elements taken M at a time.  This function is
//  only practical for situations where M is less  than about 15.
static arma::mat combs(arma::mat v, unsigned k) {
  arma::mat P(0,1);
  if (v.n_cols == 1) {
    v = v.t();
  }
  unsigned n = arma::max(arma::size(v));
  if (n == k) {
    P.resize(v.n_rows, v.n_cols);
    P = v;
  } else if (n == k +1) {
    arma::mat tmp = v;
    arma::mat c = arma::repmat(tmp,n,1);
    c = arma::reshape(c,1,n*n);
    arma::vec rg = arma::regspace(0, n, n*n-1 );
    size_t a = (n*n) - rg.n_elem;
    P.resize(1,a);
    size_t count =0;
    // remove elements for c(0:n:n*n-1)
    for(size_t j = 0; j < c.n_cols; j++) {
      if(c(0,j) != rg(count,0)) {
        P(0,count) = c(0,j);
        if((count + 1) < rg.n_rows) {
          count++;
        }
      }
    }
    // reshape P
    P = reshape(P,n,n-1);
  } else if (k == 1) {
    P.resize(v.n_cols, v.n_rows);
    P = v.t();
  } else {
    if (k < n && k > 1) {
      for (size_t idx = 0; idx < (n - k + 1); idx++) {
        arma::mat Q = combs(v.cols(idx, n - 1), k-1);
        arma::mat inter = arma::ones<arma::mat>(Q.n_rows,1);
        if(v(0,idx) == 0 ) {
          inter = 0*inter;
        }
        inter= join_horiz(inter, Q);
        if(P.n_rows == 0) {
          P = inter;
        }
        else {
          P = join_vert(P, inter);
       }
      }
    }
  }
  return P;
}

// Binomial coefficient
// V is a vector of length N, produces a matrix with N!/K!(N-K)! rows and K
// columns. Each row of the result has K of
//  the elements in the vector V.
static arma::mat nchoosek(arma::mat v, unsigned k) {
  arma::mat c;
  // generate actual combinations.
  unsigned n = arma::max(arma::size(v));
  if (v.n_cols == 1) {
    v = v.t();
  }
  switch (k) {
  case 1: {
    c = v.t();
    break;
  }
  case 0: {
    c = arma::zeros<arma::mat>(1, 0);
    break;
  }
  default: {
    if (k == n) {
      c = v;
    } else if (k == (n - 1)) {
      c = arma::repmat(v, n, 1);
      c = arma::reshape(c, n, k);
    } else if ((n < 17) && ((k > 3) || (n - k < 4))) {
      double nt = (std::pow(2, n) - 1);
      double N = nt - 2 + 1;
      arma::mat tmp = arma::linspace<arma::mat>(nt, 2, N);
      tmp = tmp.t();
      arma::mat tmpM = arma::zeros<arma::mat>(N, n);
      // convert to binary
      for (int i = 0; i < N; ++i) {
        std::string binary;
        switch (n) {
        case 64: {
          binary = std::bitset<64>(tmp(i)).to_string();
          break;
        }
        case 32: {
          binary = std::bitset<32>(tmp(i)).to_string();
          break;
        }
        case 30: {
          binary = std::bitset<10>(tmp(i)).to_string();
          break;
        }
        case 28: {
          binary = std::bitset<28>(tmp(i)).to_string();
          break;
        }
        case 26: {
          binary = std::bitset<26>(tmp(i)).to_string();
          break;
        }
        case 24: {
          binary = std::bitset<24>(tmp(i)).to_string();
          break;
        }
        case 22: {
          binary = std::bitset<22>(tmp(i)).to_string();
          break;
        }
        case 20: {
          binary = std::bitset<20>(tmp(i)).to_string();
          break;
        }
        case 18: {
          binary = std::bitset<18>(tmp(i)).to_string();
          break;
        }
        case 16: {
          binary = std::bitset<16>(tmp(i)).to_string();
          break;
        }
        case 14: {
          binary = std::bitset<14>(tmp(i)).to_string();
          break;
        }
        case 12: {
          binary = std::bitset<12>(tmp(i)).to_string();
          break;
        }
        case 10: {
          binary = std::bitset<10>(tmp(i)).to_string();
          break;
        }
        case 8: {
          binary = std::bitset<8>(tmp(i)).to_string();
          break;
        }
        case 6: {
          binary = std::bitset<6>(tmp(i)).to_string();
          break;
        }
        case 4: {
          binary = std::bitset<4>(tmp(i)).to_string();
          break;
        }
        case 2: {
          binary = std::bitset<2>(tmp(i)).to_string();
          break;
        }
        default: {
          binary = std::bitset<1>(tmp(i)).to_string();
          break;
        }
        }
        std::vector<int> intNumbers;
        for (unsigned i = 0; i < binary.length(); ++i) {
          std::string subbin = binary.substr(i, 1);
          int num = std::stoi(subbin, nullptr, 10);
          intNumbers.push_back(num);
        }
        tmpM.row(i) = arma::conv_to<arma::rowvec>::from(intNumbers);
      }
      arma::uvec a = find(arma::sum(tmpM, 1) == k);
      arma::mat idx = tmpM.rows(a);
      double nrows = idx.n_rows;
      arma::uvec rows = find(idx.t());
      arma::umat t = arma::ind2sub(size(idx.t()), rows);
      t = t.row(0).t();
      arma::mat v2 = reshape(v, t.n_rows, t.n_cols);
      v2 = v2(t);
      c = reshape(v2, k, nrows).t();
    } else {
      c = combs(v, k);
    }
    break;
  }
  }
  return c;
}

#endif /* LEARNING_FAUST_H_ */
