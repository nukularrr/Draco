//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   cdi_analytic/Analytic_CP_Eloss.hh
 * \author Kendra P. Long
 * \date   Fri Aug  2 14:28:08 2019
 * \brief  Analytic_CP_Eloss class definition.
 * \note   Copyright (C) 2016-2019 Triad National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#ifndef __cdi_analytic_Analytic_CP_Eloss_hh__
#define __cdi_analytic_Analytic_CP_Eloss_hh__

#include "Analytic_Models.hh"
#include "cdi/CPEloss.hh"
#include <memory>

namespace rtt_cdi_analytic {
//===========================================================================//
/*!
 * \class Analytic_CP_Eloss
 *
 * \brief Derived rtt_cdi::CPEloss class for analytic eloss.
 *        This class implements the interface found in cdi/CPEloss.hh for
 *        the case where CP energy loss data is in analytic form.
 *
 */
//===========================================================================//

class Analytic_CP_Eloss : public rtt_cdi::CPEloss {
public:
  // Useful typedefs.
  typedef std::shared_ptr<const Analytic_Eloss_Model> const_SP_Model;
  typedef std::shared_ptr<Analytic_Eloss_Model> SP_Analytic_Model;
  typedef std::vector<double> sf_double;
  typedef std::string std_string;

private:
  // Analytic eloss model.
  SP_Analytic_Model analytic_model;

  // Particle type being transported.
  rtt_cdi::CParticleType particle;

  // Target species.
  rtt_cdi::CParticleType target;

  // Data model (e.g. Eloss, large-angle scatter, etc.)
  // Currently only ELOSS is implemented.
  //rtt_cdi::CPModel cpmodel;

public:
  // Constructor.
  Analytic_CP_Eloss(SP_Analytic_Model model_in,
                    rtt_cdi::CParticleType target_in,
                    rtt_cdi::CParticleType particle_in);

  // >>> ACCESSORS
  const_SP_Model get_Analytic_Model() const { return analytic_model; }

  // >>> INTERFACE SPECIFIED BY rtt_cdi::CPEloss

  // Get an eloss.
  double getEloss(double, double, double) const;

  // Get an eloss field given a field of temperatures.
  sf_double getEloss(const sf_double &, double, double) const;

  // Get an eloss field given a field of densities.
  sf_double getEloss(double, const sf_double &, double) const;

  //! Query to see if data is in tabular or functional form (false).
  bool data_in_tabular_form() const { return false; }

  //! Query to get the target species type.
  rtt_cdi::CParticleType getTargetType() const { return target; }

  //! Query for transport particle type.
  rtt_cdi::CParticleType getParticleType() const { return particle; }

  rtt_cdi::CPModel getModel() const { return rtt_cdi::ELOSS; }

  // Return the energy policy (gray).
  inline std_string getEnergyPolicyDescriptor() const;

  // Get the name of the associated data file.
  inline std_string getDataFilename() const;

  //! Get the temperature grid (size 0 for function-based analytic data).
  sf_double getTemperatureGrid() const { return sf_double(0); }

  //! Get the density grid (size 0 for function-based analytic data).
  sf_double getDensityGrid() const { return sf_double(0); }

  //! Get the size of the temperature grid (size 0).
  size_t getNumTemperatures() const { return 0; }

  //! Get the size of the density grid (size 0).
  size_t getNumDensities() const { return 0; }

  /*!
     * \brief Returns the general eloss model type
     *
     * Since this is an analytic model, return 1 (rtt_cdi::ANALYTIC_ETYPE)
     */
  rtt_cdi::CPModelType getModelType() const { return rtt_cdi::ANALYTIC_ETYPE; }
};

//---------------------------------------------------------------------------//
// INLINE FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Return the energy policy descriptor (gray for
 * Analytic_CP_Eloss).
 */
Analytic_CP_Eloss::std_string
Analytic_CP_Eloss::getEnergyPolicyDescriptor() const {
  return std_string("gray");
}

//---------------------------------------------------------------------------//
/*!
 * \brief Return NULL string for the data filename.
 */
Analytic_CP_Eloss::std_string Analytic_CP_Eloss::getDataFilename() const {
  return std_string();
}

} // namespace rtt_cdi_analytic

#endif
