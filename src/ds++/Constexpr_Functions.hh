//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   ds++/Constexpr_Functions.hh
 * \author Alex R. Long
 * \date   Tue July 2 16:56:16 2019
 * \brief  Constexpr versions of math calls
 * \note   Copyright (C) 2019 Triad National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#ifndef rtt_dsxx_Constexpr_Functions_hh
#define rtt_dsxx_Constexpr_Functions_hh

#include <numeric>

namespace rtt_dsxx {

double constexpr ce_fabs(double x) {
  if (x >= 0 && x < std::numeric_limits<double>::infinity())
    return x;
  else if (x < 0)
    return -1.0 * x;
  return std::numeric_limits<double>::quiet_NaN();
}

//! Simple recursive Newtwon Raphson used in square root
double constexpr sqrtNewtonRaphson(double x, double curr, double prev) {
  return (ce_fabs((curr - prev) / curr) < 1.0e-15)
             ? curr
             : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
}

//! Constexpr version of the square root
// For a finite and non-negative value of "x", returns an approximation for the
// square root of "x", Otherwise, returns NaN.
double constexpr ce_sqrt(double x) {
  return x >= 0 && x < std::numeric_limits<double>::infinity()
             ? sqrtNewtonRaphson(x, x, 0)
             : std::numeric_limits<double>::quiet_NaN();
}

} // namespace rtt_dsxx

#endif // rtt_dsxx_Constexpr_Functions_hh

//---------------------------------------------------------------------------//
// end of ds++/Constexpr_Functions.hh
//---------------------------------------------------------------------------//
