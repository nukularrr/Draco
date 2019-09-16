//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   cdi/CPEloss.hh
 * \author Kendra Long
 * \date   Fri Aug  2 14:28:08 2019
 * \brief  CPEloss class header file (an abstract class)
 * \note   Copyright (C) 2016-2019 Triad National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#ifndef __cdi_CPEloss_hh__
#define __cdi_CPEloss_hh__

#include "CPCommon.hh"
#include "ds++/config.h"
#include <string>
#include <vector>

namespace rtt_cdi {

//========================================================================
/*!
 * \class CPEloss
 *
 * \brief This is a pure virtual class that defines a standard interface for
 * all derived CPEloss objects.
 *
 * Any derived CPEloss object must provide as a minumum the functionality
 * outlined in this routine.  This functionality includes access to the data
 * grid and the ability to return calculated eloss values.
 *
 */
//========================================================================

class CPEloss {
public:
  // ---------- //
  // Destructor //
  // ---------- //

  /*!
   * \brief Default CPEloss() destructor.
   *
   * This is required to correctly release memory when any
   * object derived from CPEloss is destroyed.
   */
  virtual ~CPEloss(){/*empty*/};

  // --------- //
  // Accessors //
  // --------- //

  /*!
     * \brief Eloss accessor that returns a single eloss that 
     *     corresponds to the provided temperature, density, and
     *     incident particle speed.
     *
     * \param targetTemperature The temperature value for which an
     *     eloss value is being requested (keV).
     *
     * \param targetDensity The density value for which an eloss 
     *     value is being requested (g/cm^3).
     *
     * \param partSpeed The incident speed of the particle (cm/shk).
     *
     * \return A single Eloss rate coefficient (shk^-1).
     */
  virtual double getEloss(double targetTemperature, double targetDensity,
                          double partSpeed) const = 0;

  /*!
     * \brief Eloss accessor that returns a vector of opacities that
     *     correspond to the provided vector of temperatures and a
     *     single density / particle speed value. 
     *
     * \param targetTemperature A vector of temperature values for
     *     which eloss values are being requested (keV).
     *
     * \param targetDensity The density value for which an eloss 
     *     value is being requested (g/cm^3).
     *
     * \param partSpeed The incident speed of the particle (cm/shk).
     *
     * \return A vector of Eloss rate coefficients (shk^-1).
     */
  virtual std::vector<double>
  getEloss(const std::vector<double> &targetTemperature, double targetDensity,
           double partSpeed) const = 0;

  /*!
     * \brief Eloss accessor that returns a vector of opacities
     *     that correspond to the provided vector of densities and a
     *     single temperature / particle speed value. 
     *
     * \param targetTemperature The temperature value for which an 
     *     eloss value is being requested (keV).
     *
     * \param targetDensity A vector of density values for which
     *     eloss values are being requested (g/cm^3).
     *
     * \param partSpeed The incident speed of the particle (cm/shk).
     *
     * \return A vector of Eloss rate coefficients (shk^-1).
     */
  virtual std::vector<double> getEloss(double targetTemperature,
                                       const std::vector<double> &targetDensity,
                                       double partSpeed) const = 0;

  /*!
     * \brief Query whether the data is in tables or functional form.
     */
  virtual bool data_in_tabular_form() const = 0;

  /*!
     * \brief Query to determine the target species type.
     */
  virtual rtt_cdi::CParticleType getTargetType() const = 0;

  /*!
     * \brief Query to determine the transporting particle type.
     */
  virtual rtt_cdi::CParticleType getParticleType() const = 0;

  /*!
     * \brief Query to determine the CP Model type.
     */
  virtual rtt_cdi::CPModel getModel() const = 0;

  /*!
     * \brief Returns a string that describes the EnergyPolicy.
     *     Currently this will return either "mg" or "gray."
     */
  virtual std::string getEnergyPolicyDescriptor() const = 0;

  /*!
     * \brief Returns the name of the associated data file (if any).
     */
  virtual std::string getDataFilename() const = 0;

  /*!
     * \brief Returns a vector of temperatures that define the cached
     *     Eloss data table (keV).
     */
  virtual std::vector<double> getTemperatureGrid() const = 0;

  /*!
     * \brief Returns a vector of densities that define the cached
     *     Eloss data table (g/cm^3).
     */
  virtual std::vector<double> getDensityGrid() const = 0;

  /*!
     * \brief Returns the size of the temperature grid.
     */
  virtual size_t getNumTemperatures() const = 0;

  /*! 
     * \brief Returns the size of the density grid.
     */
  virtual size_t getNumDensities() const = 0;

  /*!
	 * \brief Returns the general Eloss model type (Analytic),
	 * defined in the enum at the top of this file.
	 */
  virtual rtt_cdi::CPModelType getModelType() const = 0;
};

} // namespace rtt_cdi

#endif
