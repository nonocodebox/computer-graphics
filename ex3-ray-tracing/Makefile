#################################################################
#
#				Warning!!!!
#				============
#
# this make file compiles all the sources with implicit rules
# therefore in order to make sure all the changes were made
# run it with "make clean all"
#
##################################################################

EXEC = ex3

#OpenMesh definitions
CG_HOME = /cs/course/2013/cg
OM_DIR = $(CG_HOME)/OpenMesh
OM_LIBS = -L$(OM_DIR)/lib -lOpenMeshCore
OM_INCLUDE=  -I$(OM_DIR)/include

# list of all the sources files
SRCS =  bimage.cpp \
				bounding_box.cpp \
				bvh.cpp \
				camera.cpp \
				cone.cpp \
				cylinder.cpp \
				dynamic_object.cpp \
				ellipsoid.cpp \
				ex5.cpp \
				MyMeshObject.cpp \
				object.cpp \
				polygon.cpp \
				scene.cpp \
				sphere.cpp \
				triangle.cpp

HDRS =  bimage.h \
				bounding_box.h \
				bvh.h \
				camera.h \
				cone.h \
				cylinder.h \
				dynamic_object.h \
				ellipsoid.h \
				general.h \
				lights.h \
				MyMesh.h \
				MyMeshObject.h \
				MyVecs.h \
				MyScene1.h \
				MyScene2.h \
				object.h \
				polygon.h \
				profiling_block_timer.h \
				ray.h \
				scene.h \
				Scene1.h \
				Scene2.h \
				Scene3.h \
				SceneChooser.h \
				sphere.h \
				triangle.h

TEXTURES =   textures/checkerboard_lg.bmp \
             textures/warning.bmp \
			 textures/face_uv.bmp \
             textures/bb8.bmp \
             textures/earth.bmp \
             textures/window.bmp \
             textures/paper.bmp
MESHES =     meshes/girl_face.obj

# compiler
CC	= g++

INCLUDE = -I/usr/include -I/usr/X11R6/include -I/cs/course/current/cg/include $(OM_INCLUDE)

OBJS = $(subst .cpp,.o,$(SRCS)) 		#substituting .cc -> .o

# in order to run in debug mode use the command "make DBG=1"
ifdef DBG

	CFLAGS = 	\
		-Wall 	\
#		-Werror \
		-DDEBUG	\
		-O0		\
		-g3		\
		-Wno-deprecated


else

	CFLAGS = 		\
		-Wall		\
		-DNDEBUG    \
		-O3

endif

# use DOF=1 to enable DOF
ifdef DOF
	CFLAGS += -DENABLE_DOF
endif


# linker
LD	= $(CC)
LDFLAGS	=


# each lib should add with -l flag
LIBS =  -L/usr/lib -lm -lGLEW -lglut -lGL -lGLU -ldl -L/usr/X11R6/lib $(OM_LIBS)


# rule to change the suffix of the sources files
OBJS	= ${SRCS:.cpp=.o}


# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .o .cpp

# define a suffix rule for .c -> .o
.cpp.o :
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

##################################
# 	default target 		##
##################################
default: $(EXEC)

all : $(EXEC)

$(EXEC) : $(OBJS)
	$(LD)  $(OBJS) $(LIBS) -o $(EXEC)

$(OBJS) : $(SRCS)


zip:
	zip $(EXEC).zip Makefile $(SRCS) $(HDRS) $(TEXTURES) $(MESHES) Readme.txt


.PHONY:
clean :
	rm -f $(OBJS) $(EXEC) *~
