//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   cdi_ndi/test/tstNDI_TN.cc
 * \author Ben R. Ryan
 * \date   2019 Nov 18
 * \brief  NDI_Base test
 * \note   Copyright (C) 2019-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#include "cdi_ndi/NDI_Common.hh"
#include "cdi_ndi/NDI_TN.hh"
#include "cdi/CDI.hh"
#include "ds++/Release.hh"
#include "ds++/ScalarUnitTest.hh"
#include "ds++/dbc.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using rtt_cdi_ndi::NDI_TN;
using rtt_dsxx::soft_equiv;

//----------------------------------------------------------------------------//
// TESTS
//----------------------------------------------------------------------------//

void gendir_test(rtt_dsxx::UnitTest &ut) {

  // Write a custom gendir file to deal with NDI-required absolute path to data
  std::string gendir_in = "gendir_tmp.all";
  std::string gendir_tmp_path = ut.getTestInputPath() + gendir_in;
  std::string data_path = ut.getTestSourcePath() + "ndi_data";
  std::ofstream gendir_tmp_file;
  gendir_tmp_file.open(gendir_tmp_path);
  gendir_tmp_file << "tndata\n";
  gendir_tmp_file << "  z=n+be7->p+li7.040ztn  d=12/20/2004  l=lanl04\n";
  gendir_tmp_file << "    f=" << data_path << "\n";
  gendir_tmp_file << "    ft=asc  ln=73  o=3372  end\n";
  gendir_tmp_file.close();

  std::string gendir_path = gendir_tmp_path;
  std::string library_in = "lanl04";
  std::string reaction_in = "n+be7->p+li7";

  NDI_TN tn(gendir_path, library_in, reaction_in,
            rtt_cdi_ndi::DISCRETIZATION::MULTIGROUP);

  // Check return values of getters
  FAIL_IF(tn.get_gendir().find(gendir_in) == std::string::npos);
  FAIL_IF_NOT(tn.get_dataset() == "tn");
  FAIL_IF_NOT(tn.get_library() == "lanl04");
  FAIL_IF_NOT(tn.get_reaction() == "n+be7->p+li7");
  FAIL_IF_NOT(tn.get_reaction_name() == "n+be7->p+li7.040ztn");
  FAIL_IF_NOT(tn.get_discretization() ==
              rtt_cdi_ndi::DISCRETIZATION::MULTIGROUP);
  FAIL_IF_NOT(tn.get_num_products() == 2);
  auto products = tn.get_products();
  FAIL_IF_NOT(products.size() == 2 && products[0] == 1001 && products[1] == 3007);
  auto multiplicities = tn.get_product_multiplicities();
  FAIL_IF_NOT(multiplicities.size() == 2 && multiplicities[0] == 1 &&
              multiplicities[1] == 1);
  auto reaction_temperature = tn.get_reaction_temperature();
  FAIL_IF_NOT(reaction_temperature.size() == 3);
  FAIL_IF_NOT(soft_equiv(reaction_temperature.front(), 1.000000000000000056e-01,
                         1.e-8));
  FAIL_IF_NOT(
      soft_equiv(reaction_temperature.back(), 1.165914400000000045e-01, 1.e-8));
  auto einbar = tn.get_einbar();
  FAIL_IF_NOT(einbar.size() == 3);
  FAIL_IF_NOT(soft_equiv(einbar.front(), 2.968071330000000008e-01, 1.e-8));
  FAIL_IF_NOT(soft_equiv(einbar.back(), 3.458878690000000145e-01, 1.e-8));
  auto sigvbar = tn.get_sigvbar();
  FAIL_IF_NOT(sigvbar.size() == 3);
  FAIL_IF_NOT(soft_equiv(sigvbar.front(), 7.504850620000000827e-23, 1.e-8));
  FAIL_IF_NOT(soft_equiv(sigvbar.back(), 7.467488500000000044e-23, 1.e-8));
  FAIL_IF_NOT(soft_equiv(tn.get_reaction_q(), 1.644289999999999964e+03, 1.e-8));
  FAIL_IF_NOT(tn.get_num_groups() == 4);
  auto group_bounds = tn.get_group_bounds();
  FAIL_IF_NOT(group_bounds.size() == 5);
  FAIL_IF_NOT(
      soft_equiv(group_bounds.front(), 1.700000000000000000e+04, 1.e-8));
  FAIL_IF_NOT(soft_equiv(group_bounds.back(), 1.669999999999999818e-01, 1.e-8));
  auto group_energies = tn.get_group_energies();
  FAIL_IF_NOT(group_energies.size() == 4);
  FAIL_IF_NOT(
      soft_equiv(group_energies.front(), 1.239500000000000000e+04, 1.e-8));
  FAIL_IF_NOT(
      soft_equiv(group_energies.back(), 9.208350000000000080e+01, 1.e-8));

  int product_zaid_1 = 1001;                    // proton
  int product_zaid_2 = 3007;                    // Lithium-7
  double material_temperature = 1.1e-1; // keV
  FAIL_IF_NOT(
      soft_equiv(tn.sample_distribution(product_zaid_1, material_temperature),
                 1.208000000000000227e+03, 1.e-8));
  FAIL_IF_NOT(
      soft_equiv(tn.sample_distribution(product_zaid_2, material_temperature),
                 9.208350000000000080e+01, 1.e-8));

  bool caught = false;
  try {
    NDI_TN bad_tn(gendir_path, library_in, reaction_in,
                  rtt_cdi_ndi::DISCRETIZATION::CONTINUOUS_ENERGY);
  } catch (const rtt_dsxx::assertion &error) {
    std::ostringstream message;
    message << "Successfully caught the following exception: \n"
            << error.what();
    PASSMSG(message.str());
    caught = true;
  }
  if (!caught) {
    std::ostringstream message;
    message << "Failed to catch an exception for continuous energy data.";
    FAILMSG(message.str());
  }

  if (ut.numFails == 0) {
    PASSMSG("NDI_Base test passes.");
  } else {
    FAILMSG("NDI_Base test fails.");
  }
}

//----------------------------------------------------------------------------//

int main(int argc, char *argv[]) {
  rtt_dsxx::ScalarUnitTest ut(argc, argv, rtt_dsxx::release);
  try {
    gendir_test(ut);
  }
  UT_EPILOG(ut);
}

//----------------------------------------------------------------------------//
// end of tstNDI_TN.cc
//----------------------------------------------------------------------------//
