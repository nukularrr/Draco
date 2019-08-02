//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   cdi_analytic/test/tstAnalytic_CP_Eloss.cc
 * \author Kendra P. Long
 * \date   Fri Aug  2 14:28:08 2019
 * \brief  Analytic_CP_Eloss test.
 * \note   Copyright (C) 2016-2019 Triad National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#include "cdi_analytic_test.hh"
#include "cdi/CDI.hh"
#include "cdi_analytic/Analytic_CP_Eloss.hh"
#include "cdi_analytic/Analytic_Models.hh"
#include "ds++/Release.hh"
#include "ds++/ScalarUnitTest.hh"
#include "ds++/dbc.hh"
#include <sstream>

using namespace std;

using rtt_cdi::CDI;
using rtt_cdi_analytic::Analytic_CP_Eloss;
using rtt_cdi_analytic::Analytic_Eloss_Model;
//---------------------------------------------------------------------------//
// TESTS
//---------------------------------------------------------------------------//

void KP_alpha_test(rtt_dsxx::UnitTest &ut) {

  rtt_cdi::ParticleType target_in = rtt_cdi::DEUTERIUM;
  rtt_cdi::ParticleType particle_in = rtt_cdi::ALPHA;

  std::shared_ptr<Analytic_Eloss_Model> model_in(
      new rtt_cdi_analytic::Analytic_KP_Alpha_Eloss_Model());

  Analytic_CP_Eloss eloss_mod(model_in, target_in, particle_in);

  // Check that accessors return the correct data:
  if (target_in != eloss_mod.getTargetType())
    ITFAILS;
  if (particle_in != eloss_mod.getParticleType())
    ITFAILS;

  // Get eloss values for some sample data:
  {
    double T = 1.0;    // keV
    double rho = 10.0; // g / cc
    double vel0 = 1.0; // cm / shk

    double eloss_coeff = eloss_mod.getEloss(T, rho, vel0);

    if (!rtt_dsxx::soft_equiv(eloss_coeff, 212.287, 1.0e-3))
      ITFAILS;
  }

  // Point near the maximum on rho in [0, 20] g/cc, T in [0, 100] keV:
  {
    double T = 15.0;   // keV
    double rho = 20.0; // g / cc
    double vel0 = 1.0; // cm / shk

    double eloss_coeff = eloss_mod.getEloss(T, rho, vel0);

    if (!rtt_dsxx::soft_equiv(eloss_coeff, 78.5845, 1.0e-4))
      ITFAILS;
  }

  // Point where the range fit goes negative -- eloss should return 1.0e25:
  // (equivalent to the particle range limiting to a very small number)
  {
    double T = 90.0;   // keV
    double rho = 17.5; // g / cc
    double vel0 = 1.0; // cm / shk

    double eloss_coeff = eloss_mod.getEloss(T, rho, vel0);

    if (!rtt_dsxx::soft_equiv(eloss_coeff, 1.0e25))
      ITFAILS;
  }

  // Take a temperature stripe along a constant-density line, and be sure trend
  // is what you expect (Loose check on correct shape)
  {
    std::vector<double> Ts{18.0, 19.0, 20.0, 22.0, 24.0, 26.0, 28.0, 30.0};
    double rho = 15.0;
    double vel0 = 1.0;

    std::vector<double> eloss_coeffs = eloss_mod.getEloss(Ts, rho, vel0);

    if (!rtt_dsxx::is_strict_monotonic_increasing(eloss_coeffs.begin(),
                                                  eloss_coeffs.end()))
      ITFAILS;
  }

  // Take a temperature stripe along a constant-density line, and be sure trend
  // is what you expect (Loose check on correct shape)
  {
    std::vector<double> Ts{0.0, 1.0, 2.0, 4.0, 6.0, 8.0, 10.0, 12.0};
    double rho = 15.0;
    double vel0 = 1.0;

    std::vector<double> eloss_coeffs = eloss_mod.getEloss(Ts, rho, vel0);

    if (!rtt_dsxx::is_strict_monotonic_decreasing(eloss_coeffs.begin(),
                                                  eloss_coeffs.end()))
      ITFAILS;
  }

  // Take a density stripe along a constant-temperature line, and be sure trend
  // is what you expect (Loose check on correct shape)
  {
    std::vector<double> rhos{0.0, 1.0, 2.0, 3.0, 4.0, 8.0, 12.0, 16.0};
    double T = 15.0;
    double vel0 = 1.0;

    std::vector<double> eloss_coeffs = eloss_mod.getEloss(T, rhos, vel0);

    if (!rtt_dsxx::is_strict_monotonic_increasing(eloss_coeffs.begin(),
                                                  eloss_coeffs.end()))
      ITFAILS;
  }

  // Check that speed only operates as a scalar on the range
  {
    double rho = 1.0;
    double T = 15.0;
    double vel0_1 = 1.0;
    double vel0_4 = 4.0;

    double eloss_coeff_1 = eloss_mod.getEloss(T, rho, vel0_1);
    double eloss_coeff_4 = eloss_mod.getEloss(T, rho, vel0_4);

    // Because the eloss scales linearly with particle speed, we
    // expect the coefficient ratio to be exactly 4.0.
    if (!rtt_dsxx::soft_equiv(eloss_coeff_4 / eloss_coeff_1, 4.0))
      ITFAILS;
  }

  if (ut.numFails == 0)
    PASSMSG("Analytic_CP_Eloss test passes.");
  else
    FAILMSG("Analytic_CP_Eloss test fails.");

  return;
}

//---------------------------------------------------------------------------//

int main(int argc, char *argv[]) {
  rtt_dsxx::ScalarUnitTest ut(argc, argv, rtt_dsxx::release);
  try {
    KP_alpha_test(ut);
  }
  UT_EPILOG(ut);
}

//---------------------------------------------------------------------------//
// end of tstAnalytic_CP_Eloss.cc
//---------------------------------------------------------------------------//
