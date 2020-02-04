//----------------------------------*-C++-*-----------------------------------//
/*!
 * \file   cdi_ndi/test/tstNDI_Base.cc
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

using rtt_cdi_ndi::NDI_TN;

//----------------------------------------------------------------------------//
// TESTS
//----------------------------------------------------------------------------//

void gendir_test(rtt_dsxx::UnitTest &ut) {

  std::string gendir_in = ut.getTestSourcePath() + "gendir.all";
  std::string library_in = "lanl04";
  std::string reaction_in = "d+t->n+a";

  NDI_TN tn(gendir_in, library_in, reaction_in,
                rtt_cdi_ndi::DISCRETIZATION::MULTIGROUP);

  printf("gendir \"%s\"\n", tn.get_gendir().c_str());

  printf("reaction \"%s\"\n", tn.get_reaction_name().c_str());

  FAIL_IF(tn.get_gendir().find("gendir.all") == std::string::npos);
  FAIL_IF_NOT(tn.get_reaction_name() == "d+t->n+a.011ztn");

  std::vector<int> products = tn.get_products();
  for (auto &product : products) {
    printf("product: %i\n", product);
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
// end of tstNDI_Base.cc
//----------------------------------------------------------------------------//
