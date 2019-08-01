//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   special_functions/Erf.cc
 * \author Kendra Long
 * \date   Wed Jul 31 08:18:59 2019
 * \brief  Implementation of Erf approximation
 * \note   Copyright (C) 2019 Triad National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#include "Erf.hh"
#include "units/MathConstants.hh"
#include <cmath>

namespace rtt_sf {

//---------------------------------------------------------------------------//
/*!
 * The error function integral is defined as
 * \f[
 * Erf(x) = \frac{2}{\sqrt{\pi}} \int_0^{x} e^{-t^2} dt
 * \f]
 *
 * This is a two-term Burmann series approximation to the error function 
 * integral. Its largest error is ~3.6e-3 for arguments near x=1.3
 * It always produces a positive result, but erf() is an odd function, 
 * so simply return opposite sign if x<0.0.
 *
 * See Schopf and Supancic, ``On Burmann’s Theorem and Its Application to 
 *       Problems of Linear and Nonlinear Heat Transfer and Diffusion,'' 
 *       The Mathematica Journal, Vol. 16 (2014).
 * 
 * \param x argument to evaluate Erf at
 *
 * \return Value of \f$Erf(x)\f$
 *
 * \post \c Result>=0
 */
double Erf(double x) {

  const double coeff1 = 31.0 / 200.0;
  const double coeff2 = 341.0 / 8000.0;
  const double exp_mx2 = std::exp(-x * x);
  const double sqrt_pi = std::sqrt(rtt_units::PI);

  double erf =
      2.0 / sqrt_pi * std::sqrt(1.0 - exp_mx2) *
      (sqrt_pi / 2.0 + coeff1 * exp_mx2 - coeff2 * std::exp(-2.0 * x * x));

  // Return opposite sign if arg to erf is < 0.0.
  return (x >= 0.0) ? erf : -erf;
}

} // end namespace rtt_sf

//---------------------------------------------------------------------------//
// end of F12.cc
//---------------------------------------------------------------------------//
