//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   cdi_ndi/test/tstNDI_TN.cc
 * \author Ben R. Ryan
 * \date   2019 Nov 18
 * \brief  NDI_Base test
 * \note   Copyright (C) 2019-2020 Triad National Security, LLC.
 *         All rights reserved. */
//----------------------------------------------------------------------------//

#include "cdi_ndi/NDI_TN.hh"
#include "cdi_ndi/NDI_Common.hh"
#include "cdi/CDI.hh"
#include "ds++/Release.hh"
#include "ds++/ScalarUnitTest.hh"
#include "ds++/dbc.hh"
#include <sstream>
#include <vector>

using rtt_cdi_ndi::NDI_TN;

//----------------------------------------------------------------------------//
// TESTS
//----------------------------------------------------------------------------//

void gendir_test(rtt_dsxx::UnitTest &ut) {

  std::string gendir_in = "gendir_reduced.all";
  std::string gendir_path = ut.getTestSourcePath() + gendir_in;
  std::string library_in = "lanl04";
  std::string reaction_in = "d+t->n+a";

  NDI_TN tn(gendir_path, library_in, reaction_in,
                rtt_cdi_ndi::DISCRETIZATION::MULTIGROUP);

  printf("gendir \"%s\"\n", tn.get_gendir().c_str());

  printf("reaction \"%s\"\n", tn.get_reaction_name().c_str());

  FAIL_IF(tn.get_gendir().find(gendir_in) == std::string::npos);
  FAIL_IF_NOT(tn.get_reaction_name() == "d+t->n+a.011ztn");
  FAIL_IF_NOT(tn.get_reaction_temperature().size() == 121);
  FAIL_IF_NOT(tn.get_einbar().size() == 121);
  FAIL_IF_NOT(tn.get_sigvbar().size() == 121);
  FAIL_IF_NOT(tn.get_num_products() == 2);

  auto rt = tn.get_reaction_temperature();
  printf("nt_pts: %i\n", rt.size());

  auto pm = tn.get_product_multiplicities();
  FAIL_IF_NOT(pm.size() == 2 && pm[0] == 1 && pm[1] == 1);

  std::vector<int> products = tn.get_products();
  for (auto &product : products) {
    printf("product: %i\n", product);
  }

  FAIL_IF_NOT(rtt_dsxx::soft_equiv(tn.get_reaction_q(), 1.758928e1, 1.e-8));

  printf("q: %28.18e\n", tn.get_reaction_q());

  printf("ng: %i\n", tn.get_num_groups());

  for (int k = 0; k < 100; k++) {
  double en = tn.sample_distribution(2004, 9.e-1);
  printf("en = %e\n", en);
  }

  bool caught = false;
  try {
    NDI_TN bad_tn(gendir_path, library_in, reaction_in,
                rtt_cdi_ndi::DISCRETIZATION::CONTINUOUS_ENERGY);
  } catch (const rtt_dsxx::assertion &error) {
    std::ostringstream message;
    message << "Successfully caught the following exception: \n" << error.what();
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
