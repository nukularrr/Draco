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
    : NDI_Base(gendir_in, "tn", library_in, reaction_in, discretization_in) {
  int gendir_handle, dataset_handle, ndi_size_error;
  constexpr int c_str_len = 4096;
  char c_str_buf[c_str_len];

  //! Open gendir file (index of a complete NDI dataset)
  SAFE_NDI(NDI2_open_gendir(&gendir_handle, gendir.c_str()));

  //! Set dataset option
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

  //! Get number of reaction products
  int num_products;
  SAFE_NDI(NDI2_get_int_val(dataset_handle, NDI_NUM_SEC_PARTS, &num_products));
  products.resize(num_products);
  product_temperatures.resize(num_products);
  Require(num_products > 0);

  //! Specify multigroup option
  SAFE_NDI(NDI2_set_option(dataset_handle, NDI_COLLAPSE, "4_lanl"));

  printf("rn: %s\n", reaction_name.c_str());

  //! Loop over reaction products
  for (int n = 0; n < num_products; n++) {
    //! Get ZAID of reaction product
    SAFE_NDI(NDI2_get_string_val_n(dataset_handle, NDI_SEC_PART_TYPES, n,
                                   c_str_buf, c_str_len));
    std::string product_zaid = c_str_buf;
    products[n] = std::stoi(product_zaid);

    //! Set NDI to reaction product
    SAFE_NDI(
        NDI2_set_option(dataset_handle, NDI_CURR_PART, product_zaid.c_str()));

    //! Get number of temperature support points (this can depend on reaction
    //! product)
    int num_temps =
        NDI2_get_size(dataset_handle, NDI_EDIST_TEMPS, &ndi_size_error);
    printf("ndi_size_error = %i\n", ndi_size_error);
    Require(ndi_size_error == 0);
    product_temperatures[n].resize(num_temps);

    //! Get the number of interp regions... for now just throw an exception if
    //! this is not equal to 1
    int num_edist_interp_regions = NDI2_get_size(dataset_handle, NDI_EDIST_INTERP_REG, &ndi_size_error);
    printf("ndi_size_error = %i\n", ndi_size_error);
    Require(ndi_size_error == 0);
    printf("%i\n", num_edist_interp_regions);
    Insist(num_edist_interp_regions == 1, "Only 1 edist interp region supported!");
  }

  //! Close datafile
  SAFE_NDI(NDI2_close_gendir(gendir_handle));
}

} // namespace rtt_cdi_ndi

//----------------------------------------------------------------------------//
// End cdi_ndi/NDI_TN.cc
//----------------------------------------------------------------------------//
