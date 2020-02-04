//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   cdi_ndi/NDI_Base.cc
 * \author Ben R. Ryan
 * \date   2020 Feb 4
 * \brief  NDI_Base member definitions.
 * \note   Copyright (C) 2019-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#include "NDI_Base.hh"

namespace rtt_cdi_ndi {

NDI_Base::NDI_Base(const std::string &gendir_in, const std::string &dataset_in,
                   const std::string &library_in,
                   const std::string &reaction_in,
                   const DISCRETIZATION discretization_in)
    : gendir(gendir_in), dataset(dataset_in), library(library_in),
      reaction(reaction_in), discretization(discretization_in) {}

} // namespace rtt_cdi_ndi

//----------------------------------------------------------------------------//
// End cdi_ndi/NDI_Base.cc
//----------------------------------------------------------------------------//
