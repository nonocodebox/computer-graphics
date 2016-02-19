//
//  MyVecs.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : Type definitions for vectors.
//

#ifndef __MY_VECS_
#define __MY_VECS_

//////////////////////////////
// Library Includes         //
//////////////////////////////

// for some reason, in windows the file is located on a different location  //
//#ifdef WIN32
//#include <OpenMesh/Core/Math/VectorT.hh>
//#else
#include <OpenMesh/Core/Geometry/VectorT.hh>
//#endif


//////////////////////////////
// Type Definitions         //
//////////////////////////////

typedef OpenMesh::Vec3d Point3d;  // A point in the 3D space  //
typedef OpenMesh::Vec3d Vector3d; // A vector in the 3D space //
typedef OpenMesh::Vec3d Color3d;  // A 3D color               //
typedef OpenMesh::Vec2d Point2d;
typedef OpenMesh::Vec2d TexCoord;

#endif

