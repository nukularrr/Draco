//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   cdi_ndi/NDI_TN.cc
 * \author Ben R. Ryan
 * \date   2020 Feb 4
 * \brief  NDI_TN member definitions.
 * \note   Copyright (C) 2019-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#include "NDI_TN.hh"

namespace rtt_cdi_ndi {

NDI_TN::NDI_TN(const std::string &gendir_in, const std::string &library_in,
               const std::string &reaction_in,
               const DISCRETIZATION discretization_in)
    : NDI_Base(gendir_in, "tn", library_in, reaction_in, discretization_in) {
  int gendir_handle, reaction_handle;

  //! Open gendir file (index of a complete NDI dataset)
  SAFE_NDI(NDI2_open_gendir(&gendir_handle, gendir.c_str()));

  //! Set dataset option
  SAFE_NDI(NDI2_set_option_gendir(gendir_handle, NDI_LIB_TYPE_DEFAULT,
                                  dataset.c_str()));

  //! Set library option
  SAFE_NDI(NDI2_set_option_gendir(gendir_handle, NDI_LIBRARY_DEFAULT, library.c_str()));
}

} // namespace rtt_cdi_ndi

//----------------------------------------------------------------------------//
// End cdi_ndi/NDI_TN.cc
//----------------------------------------------------------------------------//
