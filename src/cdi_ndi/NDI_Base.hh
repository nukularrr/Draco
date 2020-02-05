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
#include <algorithm>
#include <map>
#include <sstream>
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

  //! Map from reaction product ZAID to index
  std::map<int, int> product_zaid_to_index;

  //! Multiplicities for each reaction product
  std::vector<int> product_multiplicities;

  //! Temperature support point grid for reaction
  std::vector<double> reaction_temperature;

  //! Incident energy support point grid for reaction
  std::vector<double> einbar;

  //! Incident cross section support point grid for reaction
  std::vector<double> sigvbar;

  //! Temperature support point grids for each reaction product
  std::vector<std::vector<double>> product_temperatures;

  //! Distribution support point grids for each reaction product
  std::vector<std::vector<std::vector<double>>> product_distributions;

  //! Reaction Q value i.e. change in energy
  double q_reaction;

  //! Number of groups (if multigroup)
  int num_groups;

  //! Group boundaries (if multigroup)
  std::vector<double> group_bounds;

  //! Group average energies (if multigroup)
  std::vector<double> group_energies;

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

  //! Get number of reaction products
  inline uint32_t get_num_products() const { return products.size(); }

  //! Get vector of reaction products
  inline std::vector<int> get_products() const & { return products; }

  //! Get vector of reaction product multiplicities
  inline std::vector<int> get_product_multiplicities() const & {
    return product_multiplicities;
  }

  //! Get vector of reaction temperature grid support points
  inline std::vector<double> get_reaction_temperature() const & {
    return reaction_temperature;
  }

  //! Get vector of incident energy support points
  inline std::vector<double> get_einbar() const & { return einbar; }

  //! Get vector of cross section support points
  inline std::vector<double> get_sigvbar() const & { return sigvbar; }

  //! Get change in energy due to reaction
  inline double get_reaction_q() const { return q_reaction; }

  //! Get number of groups (if multigroup)
  inline int get_num_groups() const {
    Require(discretization == DISCRETIZATION::MULTIGROUP);
    return num_groups;
  }

  //! Get group boundaries (if multigroup)
  inline std::vector<double> get_group_bounds() const & {
    Require(discretization == DISCRETIZATION::MULTIGROUP);
    return group_bounds;
  }

  //! Get group energies (if multigroup)
  inline std::vector<double> get_group_energies() const & {
    Require(discretization == DISCRETIZATION::MULTIGROUP);
    return group_energies;
  }
};

} // namespace rtt_cdi_ndi

#endif // cdi_ndi_NDI_Base_hh

//----------------------------------------------------------------------------//
// End cdi_ndi/NDI_Base.hh
//----------------------------------------------------------------------------//
