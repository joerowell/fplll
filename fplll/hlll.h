/*
   This file is part of fplll. fplll is free software: you
   can redistribute it and/or modify it under the terms of the GNU Lesser
   General Public License as published by the Free Software Foundation,
   either version 2.1 of the License, or (at your option) any later version.

   fplll is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with fplll. If not, see <http://www.gnu.org/licenses/>. */

#ifndef FPLLL_HLLL_H
#define FPLLL_HLLL_H

#include "householder.h"
#include <cmath>

FPLLL_BEGIN_NAMESPACE

template <class ZT, class FT> class HLLLReduction
{
public:
  /**
   * Constructor.
   * The precision of FT must be defined before creating an instance of the
   * class and must remain the same until the object is destroyed (or no longer
   * needed).
   */
  // TODO: what is c?
  HLLLReduction(MatHouseholder<ZT, FT> &arg_m, double delta, double eta, double theta, double c,
                int flags)
      : m(arg_m)
  {
    this->delta = delta;
    this->eta   = eta;
    this->theta = theta;
    this->c     = c;
    sr          = pow(2.0, -(double)m.get_d() * c);
    verbose     = flags & LLL_VERBOSE;
    dR.resize(m.get_d());
  }

  /**
    @brief Househorder inside LLL reduction.
    */
  void lll();

private:
  // Paramters to (delta, eta, theta) hlll-reduce the basis b in m.
  FT delta, eta, theta;
  MatHouseholder<ZT, FT> &m;

  // Arbitraty c > 0
  FT c;
  // Multiplicative coefficient used to check if a vector is size-reduced or not.
  FT sr;
  // Verbose mode.
  bool verbose;

  // Temporary variables
  FT ftmp0, ftmp1;

  /**
     @brief Size reduction.

     Perform size reduction of vector kappa.

     @param kappa index of the vector
  */
  void size_reduction(int kappa);

  /**
   * In verbose mode, print informations to reproduce the computation (parameters, enable features)
   */
  inline void print_params();

  // Precompute delta_ * R(k, k)^2
  vector<FT> dR;

  // Compute dR[k]
  inline void compute_dR(int k, FT delta_);

  // Set the value dr[k] to s*delta_ where s must be equal to R(k, k)^2.
  inline void set_dR(int k, FT s, FT delta_);
};

template <class ZT, class FT> inline void HLLLReduction<ZT, FT>::print_params()
{
  cerr << "Entering HLLL" << endl
       << "delta = " << delta << endl
       << "eta = " << eta << endl
       << "theta = " << theta << endl
       << "c = " << c << endl
       << "precision = " << FT::get_prec() << endl
       << "row_expo = " << static_cast<int>(m.is_enable_row_expo()) << endl
       << "long_in_size_reduction = " << static_cast<int>(m.is_row_op_force_long()) << endl;

#ifndef HOUSEHOLDER_PRECOMPUTE_INVERSE
  cerr << "householder_precompute_inverse = 0" << endl;
#else   // HOUSEHOLDER_PRECOMPUTE_INVERSE
  cerr << "householder_precompute_inverse = 1" << endl;
#endif  // HOUSEHOLDER_PRECOMPUTE_INVERSE
}

template <class ZT, class FT> inline void HLLLReduction<ZT, FT>::compute_dR(int k, FT delta_)
{
  long expo;
  m.get_R(dR[k], k, k, expo);
  dR[k].mul(dR[k], dR[k]);
  dR[k].mul(delta_, dR[k]);  // dR[k] = delta_ * R(k, k)^2
}

template <class ZT, class FT> inline void HLLLReduction<ZT, FT>::set_dR(int k, FT s, FT delta_)
{
  dR[k].mul(delta_, s);  // dR[k] = delta_ * s = delta_ * R(k, k)^2
}

template <class ZT, class FT>
bool is_hlll_reduced(MatHouseholder<ZT, FT> &m, double delta, double eta);

FPLLL_END_NAMESPACE

#endif /* FPLLL_HLLL_H */
