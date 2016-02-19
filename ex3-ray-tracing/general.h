//
//  general.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2012-2013.
//  Purpose : Define general macros, constants, etc.
//

#ifndef __GENERAL_H__
#define __GENERAL_H__

#include "MyVecs.h"

//////////////////////////////
// Macros                   //
//////////////////////////////

#define IN          // Input function argument                        //
#define OUT         // Output function argument                       //
#define INOUT       // Input&Output function argument                 //
#define ALLOCATOR   // Function allocates memory without freeing it   //


#define minXY(X, Y)  ((X) < (Y) ? (X) : (Y)) // minimum of two values //
#define maxXY(X, Y)  ((X) > (Y) ? (X) : (Y)) // maximum of two values //


//////////////////////////////
// Constants                //
//////////////////////////////

// Vector3d,Point3d array index //
static const int X      = 0;
static const int Y      = 1;
static const int Z      = 2;

// Color3d array index  //
static const int RED    = 0;
static const int GREEN  = 1;
static const int BLUE   = 2;

static const double INF         = 1e30;   // Infinity                                               //
static const double EPS         = 1e-6;   // Epsilon (value closest to zero)                        //
static const double PI_DEGREES  = 180.0;	// Value of PI (degrees)                                  //
static const double INDEX_VOID  = 1.0;    // Refraction index of the void (not inside any object)   //

static const double COLOR_NORMALIZE = 1.0 / 255.0;  // Optimization purposes (* is faster than /)   //

static const int    NO_INTERSECTION  = 0; // Return value representing no intersection with object  //
static const int    INTERSECTION     = 1; // Return value representing an intersection with object  //

static const Color3d COLOR_DEFAULT_AMBIENT  = Color3d(0.1,0.1,0.1); // Default ambient color        //
static const Color3d COLOR_BLACK            = Color3d(0.0,0.0,0.0); // Black color                  //
static const Color3d COLOR_WHITE            = Color3d(1.0,1.0,1.0); // White color                  //
static const Color3d COLOR_RED              = Color3d(1.0,0.0,0.0); // Red color                    //
static const Color3d COLOR_GREEN            = Color3d(0.0,1.0,0.0); // Green color                  //
static const Color3d COLOR_BLUE             = Color3d(0.0,0.0,1.0); // Blue color                   //

static const Point3d POINT_ORIGIN           = Point3d(0.0,0.0,0.0); // Origin point                 //
static const Point2d POINT_ORIGIN2D         = Point2d(0.0,0.0);


#endif
