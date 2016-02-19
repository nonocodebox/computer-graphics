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

EXEC = ex2c

#OpenMesh definitions
CG_HOME = /cs/course/2013/cg
OM_DIR = $(CG_HOME)/OpenMesh
OM_LIBS = -L$(OM_DIR)/lib -lOpenMeshCore
OM_INCLUDE=  -I$(OM_DIR)/include

# list of all the sources files
SRCS =  ex2.cpp \
		Model.cpp \
		ShaderIO.cpp \
		bimage.cpp \
		BallShader.cpp \
		FrameBuffer.cpp \
		GaussianShader.cpp \
		GlowShader.cpp \
		PhongShader.cpp \
		RGBShader.cpp \
		Shader.cpp \
		GouraudShader.cpp \
		Texture.cpp \
		UniformVariable.cpp

HDRS =  ex2.h \
        Model.h \
        ShaderIO.h \
		bimage.h \
		BallShader.h \
		FrameBuffer.h \
		GaussianShader.h \
		GlowShader.h \
		PhongShader.h \
		RGBShader.h \
		Shader.h \
		GouraudShader.h \
		Texture.h \
		UniformVariable.h

SHADERS =   shaders/BallShader.frag \
            shaders/BallShader.vert \
			shaders/BoxShader.frag \
			shaders/BoxShader.vert \
			shaders/GouraudShader.frag \
			shaders/GouraudShader.vert \
			shaders/PhongShader.frag \
			shaders/PhongShader.vert \
			shaders/GaussianShader.frag \
			shaders/GaussianShader.vert \
			shaders/GlowShader.frag \
			shaders/GlowShader.vert

TEXTURES =	textures/brickwork-bump-map.bmp \
			textures/brickwork-texture.bmp \
			textures/spheremap2.bmp

# compiler
CC	= g++

INCLUDE = -I/usr/include -I/usr/X11R6/include -I/cs/course/current/cg/include $(OM_INCLUDE)

OBJS = $(subst .cpp,.o,$(SRCS)) 		#substituting .cc -> .o

# in order to run in debug mode use the command "make DBG=1"
ifdef DBG

	CFLAGS = 	\
		-Wall 	\
		-Werror \
		-DDEBUG	\
		-O0		\
		-g3		\
		-Wno-deprecated


else

	CFLAGS = 		\
		-Wall		\
		-DNDEBUG    \
		-O2

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
all : $(EXEC)

$(EXEC) : $(OBJS)
	$(LD)  $(OBJS) $(LIBS) -o $(EXEC)

$(OBJS) : $(SRCS)


zip:
	zip $(EXEC).zip Makefile $(SRCS) $(HDRS) $(SHADERS) $(TEXTURES) Readme.txt


.PHONY:
clean :
	rm -f $(OBJS) $(EXEC) *~
