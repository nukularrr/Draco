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
#include <cmath>

namespace rtt_cdi_ndi {

//----------------------------------------------------------------------------//
// CONSTRUCTORS
//----------------------------------------------------------------------------//
/*!
 * \brief Constructor for NDI reader specific to TN reaction data.
 *
 * This constructor opens an NDI file, navigates to the appropriate data, reads
 * the data into internal buffers, and closes the file. For more details on NDI,
 * see https://xweb.lanl.gov/projects/data/nuclear/ndi/ndi.html
 *
 * \param[in] gendir_in path to gendir file
 * \param[in] library_in name of requested NDI data library
 * \param[in] reaction_in name of requested reaction
 * \param[in] discretization_in type of energy discretization to request
 */
NDI_TN::NDI_TN(const std::string &gendir_in, const std::string &library_in,
               const std::string &reaction_in,
               const DISCRETIZATION discretization_in)
    : NDI_Base(gendir_in, "tn", library_in, reaction_in, discretization_in),
      control(1234567) {
  int gendir_handle, dataset_handle, ndi_size_error;
  constexpr int c_str_len = 4096;
  char c_str_buf[c_str_len];

  control.initialize(1, rng);

  // Open gendir file (index of a complete NDI dataset)
  SAFE_NDI(NDI2_open_gendir(&gendir_handle, gendir.c_str()));

  // Set dataset option
  SAFE_NDI(NDI2_set_option_gendir(gendir_handle, NDI_LIB_TYPE_DEFAULT,
                                  dataset.c_str()));

  //! Set library option
  SAFE_NDI(NDI2_set_option_gendir(gendir_handle, NDI_LIBRARY_DEFAULT,
                                  library.c_str()));

  //! Get dataset handle
  SAFE_NDI(NDI2_make_handle(gendir_handle, dataset.c_str(), &dataset_handle));

  //! Set reaction
  SAFE_NDI(NDI2_set_reaction(dataset_handle, reaction.c_str()));

  //! Store reaction name from NDI file
  SAFE_NDI(NDI2_get_string_val(dataset_handle, NDI_ZAID, c_str_buf, c_str_len));
  reaction_name = c_str_buf;

  //! Get number of temperature support points for reaction
  int num_temps;
  SAFE_NDI(NDI2_get_int_val(dataset_handle, NDI_NUM_TEMPS, &num_temps));
  reaction_temperature.resize(num_temps);

  //! Get temperature support points for reaction
  SAFE_NDI(NDI2_get_float64_vec(dataset_handle, NDI_TEMPS,
                                reaction_temperature.data(),
                                static_cast<int>(reaction_temperature.size())));
  // MeV -> keV
  for (auto &temperature : reaction_temperature) {
    temperature *= 1000.;
  }

  //! Get number of incident energy support points for reaction
  int num_einbar = NDI2_get_size(dataset_handle, NDI_EINBAR, &ndi_size_error);
  Require(ndi_size_error == 0);
  einbar.resize(num_einbar);

  //! Get incident energy support points for reaction
  SAFE_NDI(NDI2_get_float64_vec(dataset_handle, NDI_EINBAR, einbar.data(),
                                static_cast<int>(einbar.size())));
  // MeV -> keV
  for (auto &energy : einbar) {
    energy *= 1000.;
  }

  //! Get the number of interp regions... for now just throw an exception if
  //! this is not equal to 1
  int num_einbar_interp_regions =
      NDI2_get_size(dataset_handle, NDI_EINBAR_INTERP_REG, &ndi_size_error);
  Require(ndi_size_error == 0);
  Insist(num_einbar_interp_regions == 1,
         "Only 1 einbar interp region supported!");

  //! Get number of cross section support points for reaction
  int num_sigvbar =
      NDI2_get_size(dataset_handle, NDI_SIGVBARS, &ndi_size_error);
  Require(ndi_size_error == 0);
  sigvbar.resize(num_sigvbar);

  //! Get cross section support points for reaction
  SAFE_NDI(NDI2_get_float64_vec(dataset_handle, NDI_SIGVBARS, sigvbar.data(),
                                static_cast<int>(sigvbar.size())));
  // cm^3 s^-1 -> cm^3 sh^-1
  for (auto &sigma : sigvbar) {
    sigma *= 1.e-8;
  }

  //! Get the number of interp regions... for now just throw an exception if
  //! this is not equal to 1
  int num_sigvbar_interp_regions =
      NDI2_get_size(dataset_handle, NDI_SIGVBAR_INTERP_REG, &ndi_size_error);
  Require(ndi_size_error == 0);
  Insist(num_sigvbar_interp_regions == 1,
         "Only 1 sigvbar interp region supported!");

  //! Get number of reaction products
  int num_products;
  SAFE_NDI(NDI2_get_int_val(dataset_handle, NDI_NUM_SEC_PARTS, &num_products));
  products.resize(num_products);
  product_temperatures.resize(num_products);
  product_distributions.resize(num_products);
  product_multiplicities.resize(num_products);
  Require(num_products > 0);

  //! Get reaction product multiplicity
  SAFE_NDI(NDI2_get_int_vec(dataset_handle, NDI_RPRODS_MLT,
                            product_multiplicities.data(),
                            static_cast<int>(product_multiplicities.size())));

  //! Get change in energy due to reaction
  SAFE_NDI(NDI2_get_float64_val(dataset_handle, NDI_REAC_Q, &q_reaction));
  q_reaction *= 1000.; // MeV -> keV

  Insist(discretization == DISCRETIZATION::MULTIGROUP,
         "Only multigroup discretization currently supported!");

  if (discretization == DISCRETIZATION::MULTIGROUP) {
    //! Specify multigroup option
    SAFE_NDI(NDI2_set_option(dataset_handle, NDI_COLLAPSE, "4_lanl"));

    //! Get number of groups
    int num_groups_int;
    SAFE_NDI(NDI2_get_int_val(dataset_handle, NDI_NUM_GRPS, &num_groups_int));
    num_groups = static_cast<uint32_t>(num_groups_int);
    group_bounds.resize(num_groups + 1);
    group_energies.resize(num_groups);

    //! Get boundaries of energy groups
    SAFE_NDI(NDI2_get_float64_vec(dataset_handle, NDI_E_BOUNDS,
                                  group_bounds.data(),
                                  static_cast<int>(group_bounds.size())));
    // MeV -> keV
    for (auto &bound : group_bounds) {
      bound *= 1000;
    }

    //! Get average energies of energy groups
    SAFE_NDI(NDI2_get_float64_vec(dataset_handle, NDI_E_AVG,
                                  group_energies.data(),
                                  static_cast<int>(group_energies.size())));
    // MeV -> keV
    for (auto &energy : group_energies) {
      energy *= 1000;
    }

    //! Loop over reaction products
    for (int n = 0; n < num_products; n++) {
      //! Get ZAID of reaction product
      SAFE_NDI(NDI2_get_string_val_n(dataset_handle, NDI_SEC_PART_TYPES, n,
                                     c_str_buf, c_str_len));
      std::string product_zaid = c_str_buf;
      products[n] = std::stoi(product_zaid);
      product_zaid_to_index.insert(std::pair<int, int>(products[n], n));

      //! Set NDI to reaction product
      SAFE_NDI(
          NDI2_set_option(dataset_handle, NDI_CURR_PART, product_zaid.c_str()));

      //! Get number of temperature support points (this can depend on reaction
      //! product)
      int num_product_temps =
          NDI2_get_size(dataset_handle, NDI_EDIST_TEMPS, &ndi_size_error);
      Require(ndi_size_error == 0);
      product_temperatures[n].resize(num_product_temps);
      product_distributions[n].resize(num_product_temps);

      //! Get temperature support points
      SAFE_NDI(NDI2_get_float64_vec(
          dataset_handle, NDI_TEMPS, product_temperatures[n].data(),
          static_cast<int>(product_temperatures[n].size())));
      // MeV -> keV
      for (auto &temperature : product_temperatures[n]) {
        temperature *= 1000.;
      }

      //! Get the number of interp regions... for now just throw an exception if
      //! this is not equal to 1
      int num_edist_interp_regions =
          NDI2_get_size(dataset_handle, NDI_EDIST_INTERP_REG, &ndi_size_error);
      Require(ndi_size_error == 0);
      Insist(num_edist_interp_regions == 1,
             "Only 1 edist interp region supported!");

      // Loop over temperatures
      for (size_t m = 0; m < product_temperatures[n].size(); m++) {
        std::ostringstream temp_stream;
        temp_stream << product_temperatures[n][m] / 1000; // keV -> MeV
        SAFE_NDI(NDI2_set_option(dataset_handle, NDI_TEMP,
                                 temp_stream.str().c_str()));

        product_distributions[n][m].resize(num_groups);
        SAFE_NDI(NDI2_get_float64_vec(
            dataset_handle, NDI_EDIST, product_distributions[n][m].data(),
            static_cast<int>(product_distributions[n][m].size())));
      }
    }
  }

  //! Close datafile
  SAFE_NDI(NDI2_close_gendir(gendir_handle));
}

//----------------------------------------------------------------------------//
/*!
 * \brief Sample reaction product energy distribution at fixed temperature.
 * \param[in] product_zaid ZAID of reaction product to sample
 * \param[in] temperature of plasma (keV)
 * \return Sampled reaction product energy (keV)
 */
double NDI_TN::sample_distribution(const int product_zaid,
                                   const double temperature) {
  const int product_index = product_zaid_to_index[product_zaid];

  Require(temperature > product_temperatures[product_index].front());
  Require(temperature < product_temperatures[product_index].back());

  auto temp_1 =
      std::upper_bound(product_temperatures[product_index].begin(),
                       product_temperatures[product_index].end(), temperature);
  uint32_t index_1 = static_cast<uint32_t>(
      temp_1 - product_temperatures[product_index].begin());
  uint32_t index_0 = index_1 - 1;
  double temp_0 = product_temperatures[product_index][index_0];
  double fac = 1. - (temperature - temp_0) / (*temp_1 - temp_0);

  std::vector<double> dist_interp(num_groups);
  double dist_sum = 0.;
  for (uint32_t n = 0; n < num_groups; n++) {
    double dist_0 = product_distributions[product_index][index_0][n];
    double dist_1 = product_distributions[product_index][index_1][n];
    dist_interp[n] = dist_0 * fac + dist_1 * (1. - fac);
    dist_sum += dist_interp[n];
  }

  // Ensure normalized PDF after interpolation
  for (uint32_t n = 0; n < num_groups; n++) {
    dist_interp[n] /= dist_sum;
  }

  rtt_rng::Counter_RNG_Ref rng_ref = rng.ref();

  uint32_t index;
  do {
    index = static_cast<uint32_t>(std::round(num_groups * rng_ref.ran()));
  } while (rng_ref.ran() > dist_interp[index]);
  double energy = group_energies[index];

  return energy;
}

} // namespace rtt_cdi_ndi

//----------------------------------------------------------------------------//
// End cdi_ndi/NDI_TN.cc
//----------------------------------------------------------------------------//
