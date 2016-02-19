//
//  MyMesh.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : Type definition for an OpenMesh mesh class.
//

#ifndef __MYMESH__
#define __MYMESH__

//////////////////////////////
// Library Includes         //
//////////////////////////////

#include "OpenMesh/Core/IO/MeshIO.hh"
// for some reason, in windows the file is located on a different location  //
//#ifdef WIN32
//#include "OpenMesh/Core/Mesh/Types/PolyMesh_ArrayKernelT.hh"
//#else
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
//#endif


//////////////////////////////
// Type Definitions         //
//////////////////////////////

struct MyTraits : public OpenMesh::DefaultTraits
{
  typedef OpenMesh::Vec3d Point;  // use double valued coordinates
  typedef OpenMesh::Vec3d Normal; // use double valued normals
};

//typedef OpenMesh::PolyMesh_ArrayKernelT<MyTraits>  MyMesh;
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;

#endif
