//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   special_functions/Erf.hh
 * \author Kendra Long
 * \date   Wed Jul 31 08:18:59 2019
 * \brief  Compute 2-term Burmann series approximation to error function (Erf)
 * \note   Copyright (C) 2019 Triad National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#ifndef sf_Erf_hh
#define sf_Erf_hh

#include "ds++/config.h"

namespace rtt_sf {
//! Calculate approximate error function at x
double Erf(double x);

} // end namespace rtt_sf

#endif // sf_Erf_hh

//---------------------------------------------------------------------------//
// end of special_functions/Erf.hh
//---------------------------------------------------------------------------//
