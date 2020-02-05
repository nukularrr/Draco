//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   cdi_ndi/NDI_TN.hh
 * \author Ben R. Ryan
 * \date   2019 Nov 4
 * \brief  NDI_TN class definition.
 * \note   Copyright (C) 2019-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#ifndef cdi_ndi_NDI_TN_hh
#define cdi_ndi_NDI_TN_hh

#include "NDI_Common.hh"
#include "NDI_Base.hh"
#include "ndi.h"
#include "ds++/Assert.hh"
#include "rng/Rnd_Control_Inline.hh"
#include "rng/Counter_RNG.hh"
#include <memory>
#include <string>

namespace rtt_cdi_ndi {

//============================================================================//
/*!
 * \class NDI_TN
 *
 * \brief Class for wrapping NDI calls to NDI tn data. Reads data, constructs
 *        internal storage amenable to radiation calculations, and provides
 *        accessors.
 */
//============================================================================//

class NDI_TN : public NDI_Base{

private:
  //constexpr static int seed = 1234567;
  //rtt_rng::Rnd_Control control(seed);
  rtt_rng::Rnd_Control control;
  rtt_rng::Counter_RNG ran1;
  //std::unique_ptr<rtt_rng::Counter_RNG_ref> rng_ptr;

public:
  //! Constructor
  NDI_TN(const std::string &gendir_in,
           const std::string &library_in, const std::string &reaction_in,
           const DISCRETIZATION discretization_in);

  //! Sample energy distribution at given temperature to determine reaction
  //! product energy
  double sample_distribution(const int product_zaid, const double temperature);
};

} // namespace rtt_cdi_ndi

#endif // cdi_ndi_NDI_TN_hh

//----------------------------------------------------------------------------//
// End cdi_ndi/NDI_TN.hh
//----------------------------------------------------------------------------//
