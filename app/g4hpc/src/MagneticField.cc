//----------------------------------*-C++-*----------------------------------//
// Copyright 2021-2022 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file TMagneticField.cc
//---------------------------------------------------------------------------//
#include "MagneticField.hh"
#include "NameSpace.hh"

#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>

using namespace std;

MagneticField::MagneticField() : fFlux(0. * tesla)
{
    fieldMap = new G4TwoVector*[g4hpc::nbinZ];
    for (int j = 0; j < g4hpc::nbinZ; j++)
    {
        fieldMap[j] = new G4TwoVector[g4hpc::nbinR];
    }

    const char* fieldMapFile = getenv("G4HPC_BFIELD_MAP");
    fieldMapFile = (fieldMapFile) ? fieldMapFile : "input/cms.mag.3_8T";

    ReadFieldMap(fieldMapFile);
}

MagneticField::~MagneticField()
{
    delete[] fieldMap;
}

void MagneticField::GetFieldValue(const double point[3], double* bField) const
{
    // default zero field
    bField[0] = bField[1] = bField[2] = 0.;

    G4double rho = std::sqrt(point[0] * point[0] + point[1] * point[1]);
    if (IsDefined(point[2], rho))
        GetVolumeBaseBfield(point, bField);
}

void MagneticField::ReadFieldMap(const char* filename)
{
    ifstream ifile(filename, ios::in | ios::binary | ios::ate);

    if (ifile.is_open())
    {
        // field map structure
        g4hpc::cmsFieldMapData fd;

        ifstream::pos_type fsize = ifile.tellg();
        size_t             dsize = sizeof(g4hpc::cmsFieldMapData);

        long int ngrid = fsize / dsize;
        ifile.seekg(0, ios::beg);

        G4cout << "g4hpc ... Loading magnetic field map: " << filename
               << G4endl;

        for (G4int i = 0; i < ngrid; i++)
        {
            ifile.read((char*)&fd, sizeof(g4hpc::cmsFieldMapData));

            // check validity of input data
            if (abs(fd.iz) > g4hpc::noffZ || fd.ir > g4hpc::nbinR)
            {
                G4Exception("MagneticField::ReadFieldMap()",
                            "BadIndex",
                            FatalException,
                            "Out of magnetic field index!");
            }
            else
            {
                fieldMap[g4hpc::noffZ + fd.iz][fd.ir].set(fd.Bz, fd.Br);
            }
        }
        ifile.close();
    }
    else
    {
        G4Exception("g4hpcMagneticField::ReadFieldMap()",
                    "BadFile",
                    FatalException,
                    "Invalide Magnetic Field Map!");
    }
}

bool MagneticField::IsDefined(const double& z, const double& rho) const
{
    return (std::fabs(z) < g4hpc::maxZ && rho < g4hpc::maxR);
}

void MagneticField::GetVolumeBaseBfield(G4double const* point,
                                        G4double*       bField) const
{
    // unit conversion: [mm] to [cm]
    G4double rho = std::sqrt(point[0] * point[0] + point[1] * point[1]) / cm;
    G4double z   = point[2] / cm;

    // volume based magnetic field map - [0:900][-1600:1600] grid in [r][z]
    G4int    ir = int(rho);
    G4double dr = rho - ir;

    G4int    iz = int(z + g4hpc::noffZ);
    G4double dz = z + g4hpc::noffZ - iz;

    G4double Bz_lb = fieldMap[iz][ir].x();
    G4double Bz_ub = fieldMap[iz + 1][ir].x();

    bField[2] = fFlux * (Bz_lb + (Bz_ub - Bz_lb) * dz);

    if (rho > 0)
    {
        G4double Br_lb = fieldMap[iz][ir].y();
        G4double Br_ub = fieldMap[iz][ir + 1].y();
        bField[0] = fFlux * (Br_lb + (Br_ub - Br_lb) * dr) * point[0] / rho;
        bField[1] = fFlux * (Br_lb + (Br_ub - Br_lb) * dr) * point[1] / rho;
    }
}

void MagneticField::SetFlux(G4double mag)
{
    // scaling factor 3.8 relative to the default field map - approximation
    fFlux = tesla * mag / 3.8;
}
