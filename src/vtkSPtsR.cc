/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkSPtsR.cc
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

This file is part of the Visualization Toolkit. No part of this file
or its contents may be copied, reproduced or altered in any way
without the express written consent of the authors.

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen 1993, 1994 

=========================================================================*/
#include "vtkSPtsR.hh"

vtkStructuredPointsReader::vtkStructuredPointsReader()
{
}

vtkStructuredPointsReader::~vtkStructuredPointsReader()
{
}

unsigned long int vtkStructuredPointsReader::GetMTime()
{
  unsigned long dtime = this->vtkStructuredPointsSource::GetMTime();
  unsigned long rtime = this->Reader.GetMTime();
  return (dtime > rtime ? dtime : rtime);
}

// Description:
// Specify file name of vtk polygonal data file to read.
void vtkStructuredPointsReader::SetFilename(char *name) 
{
  this->Reader.SetFilename(name);
}
char *vtkStructuredPointsReader::GetFilename() 
{
  return this->Reader.GetFilename();
}

// Description:
// Get the type of file (ASCII or BINARY)
int vtkStructuredPointsReader::GetFileType() 
{
  return this->Reader.GetFileType();
}

// Description:
// Set the name of the scalar data to extract. If not specified, first 
// scalar data encountered is extracted.
void vtkStructuredPointsReader::SetScalarsName(char *name) 
{
  this->Reader.SetScalarsName(name);
}
char *vtkStructuredPointsReader::GetScalarsName() 
{
  return this->Reader.GetScalarsName();
}

// Description:
// Set the name of the vector data to extract. If not specified, first 
// vector data encountered is extracted.
void vtkStructuredPointsReader::SetVectorsName(char *name) 
{
  this->Reader.SetVectorsName(name);
}
char *vtkStructuredPointsReader::GetVectorsName() 
{
  return this->Reader.GetVectorsName();
}

// Description:
// Set the name of the tensor data to extract. If not specified, first 
// tensor data encountered is extracted.
void vtkStructuredPointsReader::SetTensorsName(char *name) 
{
  this->Reader.SetTensorsName(name);
}
char *vtkStructuredPointsReader::GetTensorsName() 
{
  return this->Reader.GetTensorsName();
}

// Description:
// Set the name of the normal data to extract. If not specified, first 
// normal data encountered is extracted.
void vtkStructuredPointsReader::SetNormalsName(char *name) 
{
  this->Reader.SetNormalsName(name);
}
char *vtkStructuredPointsReader::GetNormalsName() 
{
  return this->Reader.GetNormalsName();
}

// Description:
// Set the name of the texture coordinate data to extract. If not specified,
// first texture coordinate data encountered is extracted.
void vtkStructuredPointsReader::SetTCoordsName(char *name) 
{
  this->Reader.SetTCoordsName(name);
}
char *vtkStructuredPointsReader::GetTCoordsName() 
{
  return this->Reader.GetTCoordsName();
}

// Description:
// Set the name of the lookup table data to extract. If not specified, uses 
// lookup table named by scalar. Otherwise, this specification supersedes.
void vtkStructuredPointsReader::SetLookupTableName(char *name) 
{
  this->Reader.SetLookupTableName(name);
}
char *vtkStructuredPointsReader::GetLookupTableName() 
{
  return this->Reader.GetLookupTableName();
}

void vtkStructuredPointsReader::Execute()
{
  FILE *fp;
  int numPts=0;
  int retStat;
  char line[257];
  int npts;
  int dimsRead=0, arRead=0, originRead=0;

  vtkDebugMacro(<<"Reading vtk structured points file...");
  this->Initialize();
  if ( this->Debug ) this->Reader.DebugOn();
  else this->Reader.DebugOff();

  if ( !(fp=this->Reader.OpenVTKFile()) || !this->Reader.ReadHeader(fp) )
      return;
//
// Read structured points specific stuff
//
  if ( (retStat=fscanf(fp,"%256s",line)) == EOF || retStat < 1 ) 
    {
    vtkErrorMacro(<<"Data file ends prematurely!");
    return;
    }

  if ( !strncmp(this->Reader.LowerCase(line),"dataset",(unsigned long)7) )
    {
//
// Make sure we're reading right type of geometry
//
    if ( (retStat=fscanf(fp,"%256s",line)) == EOF || retStat < 1 ) 
      {
      vtkErrorMacro(<<"Data file ends prematurely!");
      return;
      } 

    if ( strncmp(this->Reader.LowerCase(line),"structured_points",17) )
      {
      vtkErrorMacro(<< "Cannot read dataset type: " << line);
      return;
      }
//
// Read keyword and number of points
//
    numPts = this->GetNumberOfPoints(); // get default
    while (1)
      {
      if ( (retStat=fscanf(fp,"%256s",line)) == EOF || retStat < 1 ) break;

      if ( ! strncmp(this->Reader.LowerCase(line),"dimensions",10) )
        {
        int dim[3];
        if ( (retStat=fscanf(fp,"%d %d %d",dim, dim+1, dim+2)) == EOF
        || retStat < 3 ) 
          {
          vtkErrorMacro(<<"Error reading dimensions!");
          return;
          }

        numPts = dim[0] * dim[1] * dim[2];
        this->SetDimensions(dim);
        dimsRead = 1;
        }

      else if ( ! strncmp(line,"aspect_ratio",12) )
        {
        float ar[3];
        if ( (retStat=fscanf(fp,"%f %f %f",ar, ar+1, ar+2)) == EOF
        || retStat < 3 ) 
          {
          vtkErrorMacro(<<"Error reading aspect ratio!");
          return;
          }

        this->SetAspectRatio(ar);
        arRead = 1;
        }

      else if ( ! strncmp(line,"origin",6) )
        {
        float origin[3];
        if ( (retStat=fscanf(fp,"%f %f %f",origin, origin+1, origin+2)) == EOF
        || retStat < 3 ) 
          {
          vtkErrorMacro(<<"Error reading origin!");
          return;
          }

        this->SetOrigin(origin);
        originRead = 1;
        }

      else if ( ! strncmp(line, "point_data", 10) )
        {
        if ( (retStat=fscanf(fp,"%d", &npts)) == EOF || retStat < 1 ) 
          {
          vtkErrorMacro(<<"Cannot read point data!");
          return;
          }
        
        if ( npts != numPts )
          {
          vtkErrorMacro(<<"Number of points don't match data values!");
          return;
          }

        this->Reader.ReadPointData(fp, (vtkDataSet *)this, npts);
        break; //out of this loop
        }

      else
        {
        vtkErrorMacro(<< "Unrecognized keyord: " << line);
        return;
        }
      }

      if ( !dimsRead ) vtkWarningMacro(<<"No dimensions read.");
      if ( !arRead ) vtkWarningMacro(<<"No aspect ratio read.");
      if ( !originRead ) vtkWarningMacro(<<"No origin read.");
    }

  else if ( !strncmp(line,"point_data",10) )
    {
    vtkWarningMacro(<<"No geometry defined in data file!");
    if ( (retStat=fscanf(fp,"%d", &npts)) == EOF || retStat < 1 ) 
      {
      vtkErrorMacro(<<"Cannot read point data!");
      return;
      }
    this->Reader.ReadPointData(fp, (vtkDataSet *)this, numPts);
    }

  else 
    {
    vtkErrorMacro(<< "Unrecognized keyord: " << line);
    }
}


void vtkStructuredPointsReader::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkStructuredPointsSource::PrintSelf(os,indent);
  this->Reader.PrintSelf(os,indent);
}
