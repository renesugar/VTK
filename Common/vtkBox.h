/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkBox.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkBox - implicit function for a bounding box
// .SECTION Description
// vtkBox computes the implicit function and/or gradient for a axis-aligned
// bounding box. (The superclasses transform can be used to modify this
// orientation.) Each side of the box is orthogonal to all other sides
// meeting along shared edges and all faces are orthogonal to the x-y-z
// coordinate axes.  (If you wish to orient this box differently, recall that
// the superclass vtkImplicitFunction supports a transformation matrix.)
// vtkCube is a concrete implementation of vtkImplicitFunction.
// 
// .SECTION See Also
// vtkCubeSource vtkImplicitFunction

#ifndef __vtkBox_h
#define __vtkBox_h

#include "vtkImplicitFunction.h"

class VTK_COMMON_EXPORT vtkBox : public vtkImplicitFunction
{
public:
  vtkTypeRevisionMacro(vtkBox,vtkImplicitFunction);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description
  // Construct box with center at (0,0,0) and each side of length 1.0.
  static vtkBox *New();

  // Description
  // Evaluate box defined by the two points (pMin,pMax).
  double EvaluateFunction(double x[3]);
  double EvaluateFunction(double x, double y, double z)
    {return this->vtkImplicitFunction::EvaluateFunction(x, y, z); }

  // Description
  // Evaluate the gradient of the box.
  void EvaluateGradient(double x[3], double n[3]);

  // Description:
  // Set / get the bounding box using various methods.
  vtkSetVector3Macro(XMin,double);
  vtkGetVector3Macro(XMin,double);
  vtkSetVector3Macro(XMax,double);
  vtkGetVector3Macro(XMax,double);
  void SetBounds(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax);
  void SetBounds(double bounds[6]);
  void GetBounds(double &xMin, double &xMax,
                 double &yMin, double &yMax,
                 double &zMin, double &zMax);
  void GetBounds(double bounds[6]);

  // Description:
  // Bounding box intersection modified from Graphics Gems Vol I. The method
  // returns a non-zero value if the bounding box is hit. Origin[3] starts
  // the ray, dir[3] is the vector components of the ray in the x-y-z
  // directions, coord[3] is the location of hit, and t is the parametric
  // coordinate along line. (Notes: the intersection ray dir[3] is NOT
  // normalized.  Valid intersections will only occur between 0<=t<=1.)
  static char IntersectBox(double bounds[6], double origin[3], double dir[3], 
                           double coord[3], double& t);

protected:
  vtkBox();
  ~vtkBox() {}

  double XMin[3];
  double XMax[3];

private:
  vtkBox(const vtkBox&);  // Not implemented.
  void operator=(const vtkBox&);  // Not implemented.
};

#endif


