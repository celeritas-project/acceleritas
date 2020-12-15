//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file MagneticField.hh
//---------------------------------------------------------------------------//
#pragma once

#include "globals.hh"
#include "G4MagneticField.hh"
#include "G4TwoVector.hh"

class MagneticField : public G4MagneticField
{
public:
  MagneticField();
  ~MagneticField();

  void GetFieldValue(const double point[3], double *bField ) const;
  void SetFlux(G4double mag); 

  void ReadFieldMap(const char* filename);

  bool IsDefined(const double& z, const double& rho) const;
  void GetVolumeBaseBfield(G4double const *point, G4double *bField) const ;

private:
  G4double fFlux;
  G4TwoVector **fieldMap;
};
