//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   cdi_analytic/Analytic_CP_Eloss.cc
 * \author Kendra P. Long
 * \date   Fri Aug  2 14:28:08 2019
 * \brief  Analytic_CP_Eloss member definitions.
 * \note   Copyright (C) 2016-2019 Triad National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#include "Analytic_CP_Eloss.hh"

namespace rtt_cdi_analytic {

//---------------------------------------------------------------------------//
// CONSTRUCTORS
//---------------------------------------------------------------------------//
/*!
 * \brief Constructor for an analytic gray eloss model.
 *
 * This constructor builds an eloss model defined by the
 * rtt_cdi_analytic::Analytic_Eloss_Model derived class argument.
 *
 * The reaction type for this instance of the class is determined by the
 * rtt_cdi::Reaction argument.
 *
 * \param model_in shared_ptr to a derived
 *                 rtt_cdi_analytic::Analytic_Eloss_Model object
 * \param target_in rtt_cdi::ParticleType target particle (enumeration)
 * \param particle_in rtt_cdi::ParticleType transporting particle (enumeration)
 */
Analytic_CP_Eloss::Analytic_CP_Eloss(SP_Analytic_Model model_in,
                                     rtt_cdi::ParticleType target_in,
                                     rtt_cdi::ParticleType particle_in)
    : analytic_model(model_in), target(target_in), particle(particle_in) {
  Ensure(analytic_model);
}

//---------------------------------------------------------------------------//
// OPACITY INTERFACE FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Return a scalar eloss given a scalar temperature, density, and 
 *        particle speed.
 *
 * Given a scalar temperature/density/speed, return an eloss for the reaction
 * type specified by the constructor.  The analytic eloss model is
 * specified in the constructor (Analytic_CP_Eloss()).
 *
 * \param temperature material temperature in keV
 * \param density material density in g/cm^3
 * \param v0 incident particle speed in cm/shk
 * \return eloss (time coefficient) in shk^-1
 *
 */
double Analytic_CP_Eloss::getEloss(double temperature, double density,
                                   double v0) const {
  Require(temperature >= 0.0);
  Require(density >= 0.0);

  // define return eloss
  double eloss = analytic_model->calculate_eloss(temperature, density, v0);

  Ensure(eloss >= 0.0);
  return eloss;
}

//---------------------------------------------------------------------------//
/*!
 * \brief Return a field of eloss coefficients given a field of temperatures,
 *        a scalar density, and a scalar particle speed.
 *
 * Given a temperature field, scalar density, and scalar particle speed,
 * return an eloss for the reaction type specified by the constructor.  
 * The returned eloss field has the same number of elements as the
 * temperature field.
 *
 * The field type for temperature is a std::vector.
 *
 * \param temperature material temperature field in keV
 * \param density material density in g/cm^3
 * \param v0 incident particle speed in cm/shk
 * \return eloss (vector of time coefficients) in shk^-1
 *
 */
Analytic_CP_Eloss::sf_double
Analytic_CP_Eloss::getEloss(const sf_double &temperature, double density,
                            double v0) const {
  Require(density >= 0.0);

  // define the return eloss field (same size as temperature field)
  sf_double eloss(temperature.size(), 0.0);

  // define an eloss iterator
  sf_double::iterator el = eloss.begin();

  // loop through temperatures and solve for eloss
  for (sf_double::const_iterator T = temperature.begin();
       T != temperature.end(); T++, el++) {
    Check(*T >= 0.0);

    // define eloss
    *el = analytic_model->calculate_eloss(*T, density, v0);

    Check(*el >= 0.0);
  }

  return eloss;
}

//---------------------------------------------------------------------------//
/*!
 * \brief Return a field of eloss coefficients given a field of densities,
 *        a scalar temperature, and a scalar particle speed.
 *
 * Given a field of densities a scalar temperature, and a scalar particle 
 * speed, return an eloss field for the reaction type specified by the 
 * constructor. The returned eloss field has the same number of elements as 
 * the density field.
 *
 * The field type for densities is a std::vector.
 *
 * \param temperature material temperature in keV
 * \param density material density field in g/cm^3
 * \param v0 incident particle speed in cm/shk
 * \return eloss (vector of time coefficients) in shk^-1
 */
Analytic_CP_Eloss::sf_double
Analytic_CP_Eloss::getEloss(double temperature, const sf_double &density,
                            double v0) const {
  Require(temperature >= 0.0);

  // define the return eloss field (same size as density field)
  sf_double eloss(density.size(), 0.0);

  // define an eloss iterator
  sf_double::iterator el = eloss.begin();

  // loop through densities and solve for eloss
  for (sf_double::const_iterator rho = density.begin(); rho != density.end();
       rho++, el++) {
    Check(*rho >= 0.0);

    // define eloss
    *el = analytic_model->calculate_eloss(temperature, *rho, v0);

    Check(*el >= 0.0);
  }

  return eloss;
}

} // namespace rtt_cdi_analytic
