//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   cdi_ndi/NDI_Common.hh
 * \author Ben R. Ryan <brryan@lanl.gov>
 * \date   2020 Feb 4
 * \brief  Datatypes needed in NDI classes.
 * \note   Copyright (C) 2016-2020 Los Alamos National Securty, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#ifndef rtt_cdi_NDI_Common_hh
#define rtt_cdi_NDI_Common_hh

//! Macro for wrapping calls to NDI library and handling exceptions
#define SAFE_NDI(x)                                                            \
  {                                                                            \
    int ndi_error = x;                                                         \
    Insist(ndi_error == 0, #x);                                                \
  }

namespace rtt_cdi_ndi {

enum class DISCRETIZATION {
  MULTIGROUP = 0, /*!< Multigroup data */
  CONTINUOUS_ENERGY = 1 /*!< Continuous energy data */
};

} // namespace rtt_cdi_ndi

#endif // rtt_cdi_NDI_Common_hh

//----------------------------------------------------------------------------//
// End cdi_ndi/NDI_Common.hh
//----------------------------------------------------------------------------//
