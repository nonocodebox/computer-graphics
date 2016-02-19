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

EXEC = ex0

# list of all the sources files
SRCS =  ex0.cpp \
		Model.cpp \
		ShaderIO.cpp
HDRS =  ex0.h \
        Model.h \
        ShaderIO.h

SHADERS =   shaders/SimpleShader.frag \
            shaders/SimpleShader.vert

# compiler
CC	= g++

INCLUDE = -I/usr/include -I/usr/X11R6/include -I/cs/course/current/cg/include

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
LIBS =  -L/usr/lib -lm -lGLEW -lglut -lGL -lGLU -ldl -L/usr/X11R6/lib


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
	zip $(EXEC).zip Makefile $(SRCS) $(HDRS) $(SHADERS) Readme.txt


.PHONY:
clean :
	rm -f $(OBJS) $(EXEC) *~
