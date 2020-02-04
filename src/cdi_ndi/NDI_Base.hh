//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   cdi_ndi/NDI_Base.hh
 * \author Ben R. Ryan
 * \date   2019 Nov 4
 * \brief  NDI_Base class definition.
 * \note   Copyright (C) 2019-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#ifndef cdi_ndi_NDI_Base_hh
#define cdi_ndi_NDI_Base_hh

#include "NDI_Common.hh"
#include "ndi.h"
#include "ds++/Assert.hh"
#include <string>
#include <vector>

namespace rtt_cdi_ndi {

//============================================================================//
/*!
 * \class NDI_Base
 *
 * \brief Base class for wrapping NDI calls to NDI data. Reads data, constructs
 *        internal storage amenable to radiation calculations, and provides
 *        accessors. Instantiated only through a data type-specific derived
 *        class.
 */
//============================================================================//

class NDI_Base {

protected:
  //! Path to gendir file, which indexes an NDI dataset
  const std::string gendir;
  //! Type of data to read (NDI supports multigroup_neutron, multigroup_photon,
  //! multigroup_multi, tn, tnreactions, and dosimetry_neutrons)
  const std::string dataset;
  //! Name of library in which to find reaction
  const std::string library;
  //! Name of reaction to read
  const std::string reaction;
  //! Multigroup or continuous energy data
  const DISCRETIZATION discretization;

  //! Constructor
  NDI_Base(const std::string &gendir_in, const std::string &dataset_in,
           const std::string &library_in, const std::string &reaction_in,
           const DISCRETIZATION discretization_in);

  //! Name of reaction as found in NDI data
  std::string reaction_name;

  //! Labels (ZAIDs) for reaction products
  std::vector<int> products;

  //! Temperature support point grids for each reaction product
  std::vector<std::vector<int>> product_temperatures;

public:
  //! Get the name of the gendir file
  inline std::string get_gendir() const { return gendir; }

  //! Get the dataset
  inline std::string get_dataset() const { return dataset; }

  //! Get the library
  inline std::string get_library() const { return library; }

  //! Get the reaction
  inline std::string get_reaction() const { return reaction; }

  //! Get the name of the reaction from the NDI file
  inline std::string get_reaction_name() const { return reaction_name; }

  //! Get the energy discretization
  inline DISCRETIZATION get_discretization() const { return discretization; }

  //! Get vector of reaction products
  inline std::vector<int> get_products() const { return products; }
};

} // namespace rtt_cdi_ndi

#endif // cdi_ndi_NDI_Base_hh

//----------------------------------------------------------------------------//
// End cdi_ndi/NDI_Base.hh
//----------------------------------------------------------------------------//
