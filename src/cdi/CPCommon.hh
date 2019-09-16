//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   cdi/CPCommon.hh
 * \author Kendra Long
 * \date   Mon Jan 19 13:41:01 2001
 * \brief  Datatypes needed in CPEloss classes
 * \note   Copyright (C) 2016-2019 Los Alamos National Securty, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#ifndef __cdi_CPCommon_hh__
#define __cdi_CPCommon_hh__

namespace rtt_cdi {

namespace constants {
//! Number of models contained in rtt_cdi::CPModel.
unsigned int const num_CPModels(2);

//! Number of charged particle types in ParticleType above.
unsigned int const num_CPs(6);
} // namespace constants

enum CPModel {
  ELOSS = 0,        /*!< energy loss model */
  LA_SCATTERING = 1 /*!< large-angle scattering model */
};

enum CPModelType {
  UNASSIGNED_ETYPE =
      0, /*!< unassigned type; used as a placeholder before deciding type */
  ANALYTIC_ETYPE = 1, /*!< an Analytic eloss model */
};

enum CParticleType {
  UNKNOWN_PART = 0, /*!< unknown particle :-O */
  ELECTRON = 1,     /*!< rest are pretty self-explanatory... */
  HYDROGEN = 2,
  DEUTERIUM = 3,
  TRITIUM = 4,
  ALPHA = 5
};

} // namespace rtt_cdi

#endif
