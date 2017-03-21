CC = g++
INCLUDES = -I.. -I../Matrix/Include
#
#	For the 'DEFINES' variable, you have following options:
#	  -DUSE_SET -> better use this.
#	  -DUSE_MCHUNK -> I'm trying to evaluate this at present.  I would
#	                  suggest not using it for now.
#	  -DMDEBUG -> For diagnostics.  Don't use it
#	  -DHAVE_NR -> Set if you have the routine 'gauleg' from
#	               "Numerical Recipes in C" and you prefer it over
#	               my replacement.  I think mine is good enough that
#	               you won't have to worry, I would suggest not using
#	               it
#
# Summary, I suggest -> "DEFINES = -DUSE_SET"
#
DEFINES = -DUSE_SET # -DUSE_MCHUNK -MDEBUG
CFLAGS = -g -Wall $(INCLUDES) $(DEFINES) # -O4
#SRCS = Node.c Element.c Domain.c System.c DOT.c Mappings.c Gauss.c \
#		BasicTypes.c Local.c Finite.c Geom.c Cache.c MatProp.c MemChunk.c

SRCS =	Assembler.c BasicTypes.c Basis.c Cache.c Const.c Coupler.c \
		Domain.c DomainMap.c DomainOpt.c Elem1.c Elem2.c Element.c \
		Essential.c Finite.c Gauss.c GaussContext.c GPInfo.c \
		Integrator.c Local.c MatProp.c MemChunk.c Node.c Objective.c \
		Props1.c ResSystem.c ScElement.c SimObject.c SteadyDomain.c \
		StdDomain.c System.c Trans.c VecSpec.c mygauleg.c # gauleg.c

OBJS = $(SRCS:%.c=%.o)

NOBJS := $(foreach obj,$(OBJS),nobj/$(obj))
POBJS := $(foreach obj,$(OBJS),pobj/$(obj))

all: libFE.a libnostd.a

libFE.a:	$(NOBJS)
	ar rv libFE.a $(NOBJS)
	ranlib libFE.a

libnostd.a:	nobj/nostd.o
	ar rv libnostd.a nobj/nostd.o
	ranlib libnostd.a

libFE_p.a:	$(POBJS)
	ar rv libFE_p.a $(POBJS)
	ranlib libFE_p.a

%.o:	%.c
	echo "Can't make .o files in this directory!"

nobj/%.o:	%.c
	@if [ ! -d nobj ]; then mkdir nobj; fi
	$(CC) -c $(CFLAGS) $< -o $@

pobj/%.o:	%.c
	@if [ ! -d pobj ]; then mkdir pobj; fi
	$(CC) -c -pg $(CFLAGS) $< -o $@

depend:	$(SRCS)
	touch	tmp
	makedepend -pnobj/ $(INCLUDES) $(DEFINES) -I$(CPPINC) $(SRCS) nostd.c
	makedepend -ppobj/ $(INCLUDES) $(DEFINES) -I$(CPPINC) $(SRCS) nostd.c -f tmp
	cp Makefile Makefile.tmp
	cat Makefile.tmp tmp > Makefile
	rm Makefile.tmp tmp tmp.bak

clean:
	-rm $(NOBJS) $(POBJS) nobj/nostd.o

# DO NOT DELETE THIS LINE -- make depend depends on it.

nobj/Assembler.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Assembler.o: /usr/include/CC/_G_config.h ../Matrix/Include/BIMatrix.h
nobj/Assembler.o: ../Matrix/Include/Matrix.h /usr/include/CC/stdio.h
nobj/Assembler.o: ../Matrix/Include/Vector.h ../Misc/GC.h
nobj/Assembler.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
nobj/Assembler.o: /usr/include/CC/math.h ../Matrix/Include/Entry.h
nobj/Assembler.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h Assembler.h
nobj/Assembler.o: GenSystem.h System.h ResSystem.h Domain.h SimObject.h
nobj/Assembler.o: Design.h VecSpec.h Objective.h Element.h Essential.h
nobj/BasicTypes.o: BasicTypes.h Const.h
nobj/Basis.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
nobj/Basis.o: /usr/include/CC/stddef.h /usr/include/CC/assert.h
nobj/Basis.o: /usr/include/CC/stdio.h /usr/include/CC/math.h
nobj/Basis.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h Basis.h
nobj/Basis.o: ../space/coord3d.h ../Set/SimpleSet.h ../Set/Set.h Node.h
nobj/Basis.o: ../Set/AssocSet.h Element.h GPInfo.h GaussContext.h
nobj/Cache.o: Cache.h BasicTypes.h Const.h Element.h ../Set/SimpleSet.h
nobj/Cache.o: ../Set/Set.h /usr/include/CC/iostream.h
nobj/Cache.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
nobj/Cache.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
nobj/Cache.o: /usr/include/CC/stdio.h ../Matrix/Include/Vector.h ../Misc/GC.h
nobj/Cache.o: /usr/include/CC/stddef.h /usr/include/CC/math.h
nobj/Cache.o: ../Matrix/Include/Entry.h ../Matrix/Include/SVector.h
nobj/Cache.o: ../Set/AssocSet.h
nobj/Const.o: Const.h
nobj/Coupler.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/Coupler.o: /usr/include/CC/_G_config.h ../Matrix/Include/Vector.h
nobj/Coupler.o: ../Misc/GC.h /usr/include/CC/iostream.h
nobj/Coupler.o: /usr/include/CC/streambuf.h /usr/include/CC/stddef.h
nobj/Coupler.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/math.h
nobj/Coupler.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
nobj/Coupler.o: ../Matrix/Include/Entry.h ../Matrix/Include/SVector.h
nobj/Coupler.o: ../Set/AssocSet.h Coupler.h GenSystem.h System.h ResSystem.h
nobj/Coupler.o: DomainMap.h
nobj/Domain.o: /usr/include/CC/string.h /usr/include/CC/_G_config.h
nobj/Domain.o: /usr/include/CC/stdlib.h /usr/include/CC/stddef.h
nobj/Domain.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
nobj/Domain.o: /usr/include/CC/stdio.h ../Matrix/Include/Vector.h
nobj/Domain.o: ../Misc/GC.h /usr/include/CC/iostream.h
nobj/Domain.o: /usr/include/CC/streambuf.h ../Set/SimpleSet.h ../Set/Set.h
nobj/Domain.o: /usr/include/CC/math.h ../Matrix/Include/Entry.h
nobj/Domain.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h
nobj/Domain.o: /usr/include/CC/assert.h Domain.h SimObject.h Design.h
nobj/Domain.o: VecSpec.h Objective.h Element.h Essential.h Const.h Node.h
nobj/Domain.o: ../space/coord3d.h Integrator.h GenSystem.h System.h
nobj/Domain.o: ResSystem.h
nobj/DomainMap.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/DomainMap.o: /usr/include/CC/_G_config.h DomainMap.h ../Set/SimpleSet.h
nobj/DomainMap.o: ../Set/Set.h /usr/include/CC/iostream.h
nobj/DomainMap.o: /usr/include/CC/streambuf.h
nobj/DomainOpt.o: DomainOpt.h ../Opt/Opt.h /usr/include/CC/stdio.h
nobj/DomainOpt.o: /usr/include/CC/_G_config.h /usr/include/CC/iostream.h
nobj/DomainOpt.o: /usr/include/CC/streambuf.h Domain.h
nobj/DomainOpt.o: ../Matrix/Include/Vector.h ../Misc/GC.h
nobj/DomainOpt.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
nobj/DomainOpt.o: /usr/include/CC/math.h SimObject.h Design.h VecSpec.h
nobj/DomainOpt.o: Objective.h
nobj/Elem1.o: Domain.h /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Elem1.o: /usr/include/CC/_G_config.h ../Matrix/Include/Vector.h
nobj/Elem1.o: ../Misc/GC.h /usr/include/CC/stddef.h ../Set/SimpleSet.h
nobj/Elem1.o: ../Set/Set.h /usr/include/CC/math.h SimObject.h Design.h
nobj/Elem1.o: VecSpec.h Objective.h Elem1.h Finite.h Element.h Gauss.h
nobj/Elem1.o: ../space/coord3d.h Basis.h Trans.h Node.h ../Set/AssocSet.h
nobj/Elem1.o: Const.h
nobj/Elem2.o: Domain.h /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Elem2.o: /usr/include/CC/_G_config.h ../Matrix/Include/Vector.h
nobj/Elem2.o: ../Misc/GC.h /usr/include/CC/stddef.h ../Set/SimpleSet.h
nobj/Elem2.o: ../Set/Set.h /usr/include/CC/math.h SimObject.h Design.h
nobj/Elem2.o: VecSpec.h Objective.h Elem2.h Finite.h Element.h Gauss.h
nobj/Elem2.o: ../space/coord3d.h Basis.h Trans.h Node.h ../Set/AssocSet.h
nobj/Elem2.o: Const.h
nobj/Element.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/Element.o: /usr/include/CC/_G_config.h /usr/include/CC/stdlib.h
nobj/Element.o: /usr/include/CC/stddef.h Element.h ../Set/SimpleSet.h
nobj/Element.o: ../Set/Set.h /usr/include/CC/iostream.h
nobj/Element.o: /usr/include/CC/streambuf.h Domain.h
nobj/Element.o: ../Matrix/Include/Vector.h ../Misc/GC.h
nobj/Element.o: /usr/include/CC/math.h SimObject.h Design.h VecSpec.h
nobj/Element.o: Objective.h Node.h ../space/coord3d.h ../Set/AssocSet.h
nobj/Essential.o: Essential.h Design.h
nobj/Finite.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/Finite.o: /usr/include/CC/_G_config.h Finite.h Element.h
nobj/Finite.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/iostream.h
nobj/Finite.o: /usr/include/CC/streambuf.h Gauss.h /usr/include/CC/math.h
nobj/Finite.o: ../space/coord3d.h Domain.h ../Matrix/Include/Vector.h
nobj/Finite.o: ../Misc/GC.h /usr/include/CC/stddef.h SimObject.h Design.h
nobj/Finite.o: VecSpec.h Objective.h BasicTypes.h Const.h MatProp.h Cache.h
nobj/Finite.o: GaussContext.h GPInfo.h Basis.h
nobj/Gauss.o: Gauss.h /usr/include/CC/math.h ../space/coord3d.h
nobj/Gauss.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/iostream.h
nobj/Gauss.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
nobj/GaussContext.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
nobj/GaussContext.o: /usr/include/CC/stddef.h /usr/include/CC/math.h
nobj/GaussContext.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/GaussContext.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/GaussContext.o: GaussContext.h ../space/coord3d.h Node.h
nobj/GaussContext.o: ../Set/AssocSet.h ../Set/SimpleSet.h ../Set/Set.h
nobj/GaussContext.o: Element.h Trans.h
nobj/GPInfo.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
nobj/GPInfo.o: /usr/include/CC/stddef.h /usr/include/CC/math.h
nobj/GPInfo.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/GPInfo.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h GPInfo.h
nobj/GPInfo.o: ../Set/SimpleSet.h ../Set/Set.h Node.h ../space/coord3d.h
nobj/GPInfo.o: ../Set/AssocSet.h Element.h Trans.h GaussContext.h
nobj/Integrator.o: Integrator.h /usr/include/CC/math.h GenSystem.h System.h
nobj/Integrator.o: ResSystem.h Assembler.h ../Matrix/Include/BIMatrix.h
nobj/Integrator.o: ../Matrix/Include/Matrix.h /usr/include/CC/stdio.h
nobj/Integrator.o: /usr/include/CC/_G_config.h ../Matrix/Include/Vector.h
nobj/Integrator.o: ../Misc/GC.h /usr/include/CC/iostream.h
nobj/Integrator.o: /usr/include/CC/streambuf.h /usr/include/CC/stddef.h
nobj/Integrator.o: ../Set/SimpleSet.h ../Set/Set.h ../Matrix/Include/Entry.h
nobj/Integrator.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h
nobj/Local.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
nobj/Local.o: /usr/include/CC/stddef.h /usr/include/CC/assert.h
nobj/Local.o: /usr/include/CC/stdio.h Local.h Node.h ../space/coord3d.h
nobj/Local.o: ../Set/AssocSet.h ../Set/SimpleSet.h ../Set/Set.h
nobj/Local.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Local.o: ../Scalar/Scalar.h /usr/include/CC/math.h ../Scalar/adExpr.h
nobj/Local.o: ../Misc/GC.h ../Matrix/Include/BIMatrix.h
nobj/Local.o: ../Matrix/Include/Matrix.h ../Matrix/Include/Vector.h
nobj/Local.o: ../Matrix/Include/Entry.h ../Matrix/Include/SVector.h Domain.h
nobj/Local.o: SimObject.h Design.h VecSpec.h Objective.h
nobj/MatProp.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/MatProp.o: /usr/include/CC/_G_config.h /usr/include/CC/assert.h
nobj/MatProp.o: /usr/include/CC/stdio.h Domain.h ../Matrix/Include/Vector.h
nobj/MatProp.o: ../Misc/GC.h /usr/include/CC/stddef.h ../Set/SimpleSet.h
nobj/MatProp.o: ../Set/Set.h /usr/include/CC/math.h SimObject.h Design.h
nobj/MatProp.o: VecSpec.h Objective.h MatProp.h Element.h BasicTypes.h
nobj/MatProp.o: Const.h GPInfo.h GaussContext.h ../space/coord3d.h
nobj/MemChunk.o: MemChunk.h ../Misc/GC.h /usr/include/CC/iostream.h
nobj/MemChunk.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
nobj/MemChunk.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
nobj/MemChunk.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/Node.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/Node.o: /usr/include/CC/_G_config.h Domain.h /usr/include/CC/iostream.h
nobj/Node.o: /usr/include/CC/streambuf.h ../Matrix/Include/Vector.h
nobj/Node.o: ../Misc/GC.h /usr/include/CC/stddef.h ../Set/SimpleSet.h
nobj/Node.o: ../Set/Set.h /usr/include/CC/math.h SimObject.h Design.h
nobj/Node.o: VecSpec.h Objective.h Node.h ../space/coord3d.h
nobj/Node.o: ../Set/AssocSet.h
nobj/Objective.o: SimObject.h Objective.h Design.h ../Matrix/Include/Vector.h
nobj/Objective.o: ../Misc/GC.h /usr/include/CC/iostream.h
nobj/Objective.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
nobj/Objective.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
nobj/Objective.o: /usr/include/CC/math.h
nobj/Props1.o: Props1.h MatProp.h GPInfo.h ../Set/SimpleSet.h ../Set/Set.h
nobj/Props1.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Props1.o: /usr/include/CC/_G_config.h Const.h /usr/include/CC/math.h
nobj/ResSystem.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
nobj/ResSystem.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
nobj/ResSystem.o: ../Matrix/Include/Vector.h ../Misc/GC.h
nobj/ResSystem.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/ResSystem.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
nobj/ResSystem.o: /usr/include/CC/math.h ../Matrix/Include/Entry.h
nobj/ResSystem.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h
nobj/ResSystem.o: ../Matrix/Include/Conj.h ../Matrix/Include/Solver.h
nobj/ResSystem.o: ../Misc/Timer.h /usr/include/CC/sys/time.h
nobj/ResSystem.o: /usr/include/CC/sys/resource.h /usr/include/CC/unistd.h
nobj/ResSystem.o: Domain.h SimObject.h Design.h VecSpec.h Objective.h
nobj/ResSystem.o: ResSystem.h System.h Essential.h
nobj/ScElement.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/ScElement.o: /usr/include/CC/_G_config.h /usr/include/CC/stdlib.h
nobj/ScElement.o: /usr/include/CC/stddef.h ScElement.h Element.h
nobj/ScElement.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/iostream.h
nobj/ScElement.o: /usr/include/CC/streambuf.h ../Scalar/Scalar.h
nobj/ScElement.o: /usr/include/CC/math.h ../Scalar/adExpr.h ../Misc/GC.h
nobj/ScElement.o: Local.h Node.h ../space/coord3d.h ../Set/AssocSet.h
nobj/ScElement.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
nobj/ScElement.o: ../Matrix/Include/Vector.h ../Matrix/Include/Entry.h
nobj/ScElement.o: ../Matrix/Include/SVector.h Domain.h SimObject.h Design.h
nobj/ScElement.o: VecSpec.h Objective.h
nobj/SimObject.o: SimObject.h /usr/include/CC/iostream.h
nobj/SimObject.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
nobj/SimObject.o: ../Matrix/Include/Vector.h ../Misc/GC.h
nobj/SimObject.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
nobj/SimObject.o: /usr/include/CC/math.h
nobj/SteadyDomain.o: /usr/include/CC/string.h /usr/include/CC/_G_config.h
nobj/SteadyDomain.o: /usr/include/CC/stdlib.h /usr/include/CC/stddef.h
nobj/SteadyDomain.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/SteadyDomain.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
nobj/SteadyDomain.o: ../Matrix/Include/Vector.h ../Misc/GC.h
nobj/SteadyDomain.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/SteadyDomain.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/math.h
nobj/SteadyDomain.o: ../Matrix/Include/Entry.h ../Matrix/Include/SVector.h
nobj/SteadyDomain.o: ../Set/AssocSet.h StdLocalDomain.h Domain.h SimObject.h
nobj/SteadyDomain.o: Design.h VecSpec.h Objective.h SteadyDomain.h
nobj/SteadyDomain.o: Integrator.h GenSystem.h System.h ResSystem.h Coupler.h
nobj/SteadyDomain.o: DomainMap.h Element.h Essential.h Const.h Node.h
nobj/SteadyDomain.o: ../space/coord3d.h Assembler.h
nobj/StdDomain.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/StdDomain.o: /usr/include/CC/_G_config.h /usr/include/CC/stdlib.h
nobj/StdDomain.o: /usr/include/CC/stddef.h ../Matrix/Include/BIMatrix.h
nobj/StdDomain.o: ../Matrix/Include/Matrix.h ../Matrix/Include/Vector.h
nobj/StdDomain.o: ../Misc/GC.h /usr/include/CC/iostream.h
nobj/StdDomain.o: /usr/include/CC/streambuf.h ../Set/SimpleSet.h ../Set/Set.h
nobj/StdDomain.o: /usr/include/CC/math.h ../Matrix/Include/Entry.h
nobj/StdDomain.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h StdDomain.h
nobj/StdDomain.o: Domain.h SimObject.h Design.h VecSpec.h Objective.h Node.h
nobj/StdDomain.o: ../space/coord3d.h Element.h
nobj/System.o: System.h
nobj/Trans.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
nobj/Trans.o: /usr/include/CC/stddef.h /usr/include/CC/math.h
nobj/Trans.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/Trans.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h Trans.h
nobj/Trans.o: ../space/coord3d.h ../Set/SimpleSet.h ../Set/Set.h Node.h
nobj/Trans.o: ../Set/AssocSet.h Element.h Basis.h GaussContext.h GPInfo.h
nobj/VecSpec.o: VecSpec.h ../Matrix/Include/Vector.h ../Misc/GC.h
nobj/VecSpec.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
nobj/VecSpec.o: /usr/include/CC/_G_config.h /usr/include/CC/stddef.h
nobj/VecSpec.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/math.h
nobj/VecSpec.o: Design.h /usr/include/CC/assert.h /usr/include/CC/stdio.h
nobj/mygauleg.o: /usr/include/CC/math.h /usr/include/CC/stdio.h
nobj/mygauleg.o: /usr/include/CC/_G_config.h
nobj/nostd.o: MatProp.h /usr/include/CC/iostream.h
nobj/nostd.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
nobj/nostd.o: /usr/include/CC/stdlib.h /usr/include/CC/stddef.h
# DO NOT DELETE THIS LINE -- make depend depends on it.

pobj/Assembler.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Assembler.o: /usr/include/CC/_G_config.h ../Matrix/Include/BIMatrix.h
pobj/Assembler.o: ../Matrix/Include/Matrix.h /usr/include/CC/stdio.h
pobj/Assembler.o: ../Matrix/Include/Vector.h ../Misc/GC.h
pobj/Assembler.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
pobj/Assembler.o: /usr/include/CC/math.h ../Matrix/Include/Entry.h
pobj/Assembler.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h Assembler.h
pobj/Assembler.o: GenSystem.h System.h ResSystem.h Domain.h SimObject.h
pobj/Assembler.o: Design.h VecSpec.h Objective.h Element.h Essential.h
pobj/BasicTypes.o: BasicTypes.h Const.h
pobj/Basis.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
pobj/Basis.o: /usr/include/CC/stddef.h /usr/include/CC/assert.h
pobj/Basis.o: /usr/include/CC/stdio.h /usr/include/CC/math.h
pobj/Basis.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h Basis.h
pobj/Basis.o: ../space/coord3d.h ../Set/SimpleSet.h ../Set/Set.h Node.h
pobj/Basis.o: ../Set/AssocSet.h Element.h GPInfo.h GaussContext.h
pobj/Cache.o: Cache.h BasicTypes.h Const.h Element.h ../Set/SimpleSet.h
pobj/Cache.o: ../Set/Set.h /usr/include/CC/iostream.h
pobj/Cache.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
pobj/Cache.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
pobj/Cache.o: /usr/include/CC/stdio.h ../Matrix/Include/Vector.h ../Misc/GC.h
pobj/Cache.o: /usr/include/CC/stddef.h /usr/include/CC/math.h
pobj/Cache.o: ../Matrix/Include/Entry.h ../Matrix/Include/SVector.h
pobj/Cache.o: ../Set/AssocSet.h
pobj/Const.o: Const.h
pobj/Coupler.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/Coupler.o: /usr/include/CC/_G_config.h ../Matrix/Include/Vector.h
pobj/Coupler.o: ../Misc/GC.h /usr/include/CC/iostream.h
pobj/Coupler.o: /usr/include/CC/streambuf.h /usr/include/CC/stddef.h
pobj/Coupler.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/math.h
pobj/Coupler.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
pobj/Coupler.o: ../Matrix/Include/Entry.h ../Matrix/Include/SVector.h
pobj/Coupler.o: ../Set/AssocSet.h Coupler.h GenSystem.h System.h ResSystem.h
pobj/Coupler.o: DomainMap.h
pobj/Domain.o: /usr/include/CC/string.h /usr/include/CC/_G_config.h
pobj/Domain.o: /usr/include/CC/stdlib.h /usr/include/CC/stddef.h
pobj/Domain.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
pobj/Domain.o: /usr/include/CC/stdio.h ../Matrix/Include/Vector.h
pobj/Domain.o: ../Misc/GC.h /usr/include/CC/iostream.h
pobj/Domain.o: /usr/include/CC/streambuf.h ../Set/SimpleSet.h ../Set/Set.h
pobj/Domain.o: /usr/include/CC/math.h ../Matrix/Include/Entry.h
pobj/Domain.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h
pobj/Domain.o: /usr/include/CC/assert.h Domain.h SimObject.h Design.h
pobj/Domain.o: VecSpec.h Objective.h Element.h Essential.h Const.h Node.h
pobj/Domain.o: ../space/coord3d.h Integrator.h GenSystem.h System.h
pobj/Domain.o: ResSystem.h
pobj/DomainMap.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/DomainMap.o: /usr/include/CC/_G_config.h DomainMap.h ../Set/SimpleSet.h
pobj/DomainMap.o: ../Set/Set.h /usr/include/CC/iostream.h
pobj/DomainMap.o: /usr/include/CC/streambuf.h
pobj/DomainOpt.o: DomainOpt.h ../Opt/Opt.h /usr/include/CC/stdio.h
pobj/DomainOpt.o: /usr/include/CC/_G_config.h /usr/include/CC/iostream.h
pobj/DomainOpt.o: /usr/include/CC/streambuf.h Domain.h
pobj/DomainOpt.o: ../Matrix/Include/Vector.h ../Misc/GC.h
pobj/DomainOpt.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
pobj/DomainOpt.o: /usr/include/CC/math.h SimObject.h Design.h VecSpec.h
pobj/DomainOpt.o: Objective.h
pobj/Elem1.o: Domain.h /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Elem1.o: /usr/include/CC/_G_config.h ../Matrix/Include/Vector.h
pobj/Elem1.o: ../Misc/GC.h /usr/include/CC/stddef.h ../Set/SimpleSet.h
pobj/Elem1.o: ../Set/Set.h /usr/include/CC/math.h SimObject.h Design.h
pobj/Elem1.o: VecSpec.h Objective.h Elem1.h Finite.h Element.h Gauss.h
pobj/Elem1.o: ../space/coord3d.h Basis.h Trans.h Node.h ../Set/AssocSet.h
pobj/Elem1.o: Const.h
pobj/Elem2.o: Domain.h /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Elem2.o: /usr/include/CC/_G_config.h ../Matrix/Include/Vector.h
pobj/Elem2.o: ../Misc/GC.h /usr/include/CC/stddef.h ../Set/SimpleSet.h
pobj/Elem2.o: ../Set/Set.h /usr/include/CC/math.h SimObject.h Design.h
pobj/Elem2.o: VecSpec.h Objective.h Elem2.h Finite.h Element.h Gauss.h
pobj/Elem2.o: ../space/coord3d.h Basis.h Trans.h Node.h ../Set/AssocSet.h
pobj/Elem2.o: Const.h
pobj/Element.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/Element.o: /usr/include/CC/_G_config.h /usr/include/CC/stdlib.h
pobj/Element.o: /usr/include/CC/stddef.h Element.h ../Set/SimpleSet.h
pobj/Element.o: ../Set/Set.h /usr/include/CC/iostream.h
pobj/Element.o: /usr/include/CC/streambuf.h Domain.h
pobj/Element.o: ../Matrix/Include/Vector.h ../Misc/GC.h
pobj/Element.o: /usr/include/CC/math.h SimObject.h Design.h VecSpec.h
pobj/Element.o: Objective.h Node.h ../space/coord3d.h ../Set/AssocSet.h
pobj/Essential.o: Essential.h Design.h
pobj/Finite.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/Finite.o: /usr/include/CC/_G_config.h Finite.h Element.h
pobj/Finite.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/iostream.h
pobj/Finite.o: /usr/include/CC/streambuf.h Gauss.h /usr/include/CC/math.h
pobj/Finite.o: ../space/coord3d.h Domain.h ../Matrix/Include/Vector.h
pobj/Finite.o: ../Misc/GC.h /usr/include/CC/stddef.h SimObject.h Design.h
pobj/Finite.o: VecSpec.h Objective.h BasicTypes.h Const.h MatProp.h Cache.h
pobj/Finite.o: GaussContext.h GPInfo.h Basis.h
pobj/Gauss.o: Gauss.h /usr/include/CC/math.h ../space/coord3d.h
pobj/Gauss.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/iostream.h
pobj/Gauss.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
pobj/GaussContext.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
pobj/GaussContext.o: /usr/include/CC/stddef.h /usr/include/CC/math.h
pobj/GaussContext.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/GaussContext.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/GaussContext.o: GaussContext.h ../space/coord3d.h Node.h
pobj/GaussContext.o: ../Set/AssocSet.h ../Set/SimpleSet.h ../Set/Set.h
pobj/GaussContext.o: Element.h Trans.h
pobj/GPInfo.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
pobj/GPInfo.o: /usr/include/CC/stddef.h /usr/include/CC/math.h
pobj/GPInfo.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/GPInfo.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h GPInfo.h
pobj/GPInfo.o: ../Set/SimpleSet.h ../Set/Set.h Node.h ../space/coord3d.h
pobj/GPInfo.o: ../Set/AssocSet.h Element.h Trans.h GaussContext.h
pobj/Integrator.o: Integrator.h /usr/include/CC/math.h GenSystem.h System.h
pobj/Integrator.o: ResSystem.h Assembler.h ../Matrix/Include/BIMatrix.h
pobj/Integrator.o: ../Matrix/Include/Matrix.h /usr/include/CC/stdio.h
pobj/Integrator.o: /usr/include/CC/_G_config.h ../Matrix/Include/Vector.h
pobj/Integrator.o: ../Misc/GC.h /usr/include/CC/iostream.h
pobj/Integrator.o: /usr/include/CC/streambuf.h /usr/include/CC/stddef.h
pobj/Integrator.o: ../Set/SimpleSet.h ../Set/Set.h ../Matrix/Include/Entry.h
pobj/Integrator.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h
pobj/Local.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
pobj/Local.o: /usr/include/CC/stddef.h /usr/include/CC/assert.h
pobj/Local.o: /usr/include/CC/stdio.h Local.h Node.h ../space/coord3d.h
pobj/Local.o: ../Set/AssocSet.h ../Set/SimpleSet.h ../Set/Set.h
pobj/Local.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Local.o: ../Scalar/Scalar.h /usr/include/CC/math.h ../Scalar/adExpr.h
pobj/Local.o: ../Misc/GC.h ../Matrix/Include/BIMatrix.h
pobj/Local.o: ../Matrix/Include/Matrix.h ../Matrix/Include/Vector.h
pobj/Local.o: ../Matrix/Include/Entry.h ../Matrix/Include/SVector.h Domain.h
pobj/Local.o: SimObject.h Design.h VecSpec.h Objective.h
pobj/MatProp.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/MatProp.o: /usr/include/CC/_G_config.h /usr/include/CC/assert.h
pobj/MatProp.o: /usr/include/CC/stdio.h Domain.h ../Matrix/Include/Vector.h
pobj/MatProp.o: ../Misc/GC.h /usr/include/CC/stddef.h ../Set/SimpleSet.h
pobj/MatProp.o: ../Set/Set.h /usr/include/CC/math.h SimObject.h Design.h
pobj/MatProp.o: VecSpec.h Objective.h MatProp.h Element.h BasicTypes.h
pobj/MatProp.o: Const.h GPInfo.h GaussContext.h ../space/coord3d.h
pobj/MemChunk.o: MemChunk.h ../Misc/GC.h /usr/include/CC/iostream.h
pobj/MemChunk.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
pobj/MemChunk.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
pobj/MemChunk.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/Node.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/Node.o: /usr/include/CC/_G_config.h Domain.h /usr/include/CC/iostream.h
pobj/Node.o: /usr/include/CC/streambuf.h ../Matrix/Include/Vector.h
pobj/Node.o: ../Misc/GC.h /usr/include/CC/stddef.h ../Set/SimpleSet.h
pobj/Node.o: ../Set/Set.h /usr/include/CC/math.h SimObject.h Design.h
pobj/Node.o: VecSpec.h Objective.h Node.h ../space/coord3d.h
pobj/Node.o: ../Set/AssocSet.h
pobj/Objective.o: SimObject.h Objective.h Design.h ../Matrix/Include/Vector.h
pobj/Objective.o: ../Misc/GC.h /usr/include/CC/iostream.h
pobj/Objective.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
pobj/Objective.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
pobj/Objective.o: /usr/include/CC/math.h
pobj/Props1.o: Props1.h MatProp.h GPInfo.h ../Set/SimpleSet.h ../Set/Set.h
pobj/Props1.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Props1.o: /usr/include/CC/_G_config.h Const.h /usr/include/CC/math.h
pobj/ResSystem.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
pobj/ResSystem.o: /usr/include/CC/stdio.h /usr/include/CC/_G_config.h
pobj/ResSystem.o: ../Matrix/Include/Vector.h ../Misc/GC.h
pobj/ResSystem.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/ResSystem.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
pobj/ResSystem.o: /usr/include/CC/math.h ../Matrix/Include/Entry.h
pobj/ResSystem.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h
pobj/ResSystem.o: ../Matrix/Include/Conj.h ../Matrix/Include/Solver.h
pobj/ResSystem.o: ../Misc/Timer.h /usr/include/CC/sys/time.h
pobj/ResSystem.o: /usr/include/CC/sys/resource.h /usr/include/CC/unistd.h
pobj/ResSystem.o: Domain.h SimObject.h Design.h VecSpec.h Objective.h
pobj/ResSystem.o: ResSystem.h System.h Essential.h
pobj/ScElement.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/ScElement.o: /usr/include/CC/_G_config.h /usr/include/CC/stdlib.h
pobj/ScElement.o: /usr/include/CC/stddef.h ScElement.h Element.h
pobj/ScElement.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/iostream.h
pobj/ScElement.o: /usr/include/CC/streambuf.h ../Scalar/Scalar.h
pobj/ScElement.o: /usr/include/CC/math.h ../Scalar/adExpr.h ../Misc/GC.h
pobj/ScElement.o: Local.h Node.h ../space/coord3d.h ../Set/AssocSet.h
pobj/ScElement.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
pobj/ScElement.o: ../Matrix/Include/Vector.h ../Matrix/Include/Entry.h
pobj/ScElement.o: ../Matrix/Include/SVector.h Domain.h SimObject.h Design.h
pobj/ScElement.o: VecSpec.h Objective.h
pobj/SimObject.o: SimObject.h /usr/include/CC/iostream.h
pobj/SimObject.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
pobj/SimObject.o: ../Matrix/Include/Vector.h ../Misc/GC.h
pobj/SimObject.o: /usr/include/CC/stddef.h ../Set/SimpleSet.h ../Set/Set.h
pobj/SimObject.o: /usr/include/CC/math.h
pobj/SteadyDomain.o: /usr/include/CC/string.h /usr/include/CC/_G_config.h
pobj/SteadyDomain.o: /usr/include/CC/stdlib.h /usr/include/CC/stddef.h
pobj/SteadyDomain.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/SteadyDomain.o: ../Matrix/Include/BIMatrix.h ../Matrix/Include/Matrix.h
pobj/SteadyDomain.o: ../Matrix/Include/Vector.h ../Misc/GC.h
pobj/SteadyDomain.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/SteadyDomain.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/math.h
pobj/SteadyDomain.o: ../Matrix/Include/Entry.h ../Matrix/Include/SVector.h
pobj/SteadyDomain.o: ../Set/AssocSet.h StdLocalDomain.h Domain.h SimObject.h
pobj/SteadyDomain.o: Design.h VecSpec.h Objective.h SteadyDomain.h
pobj/SteadyDomain.o: Integrator.h GenSystem.h System.h ResSystem.h Coupler.h
pobj/SteadyDomain.o: DomainMap.h Element.h Essential.h Const.h Node.h
pobj/SteadyDomain.o: ../space/coord3d.h Assembler.h
pobj/StdDomain.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/StdDomain.o: /usr/include/CC/_G_config.h /usr/include/CC/stdlib.h
pobj/StdDomain.o: /usr/include/CC/stddef.h ../Matrix/Include/BIMatrix.h
pobj/StdDomain.o: ../Matrix/Include/Matrix.h ../Matrix/Include/Vector.h
pobj/StdDomain.o: ../Misc/GC.h /usr/include/CC/iostream.h
pobj/StdDomain.o: /usr/include/CC/streambuf.h ../Set/SimpleSet.h ../Set/Set.h
pobj/StdDomain.o: /usr/include/CC/math.h ../Matrix/Include/Entry.h
pobj/StdDomain.o: ../Matrix/Include/SVector.h ../Set/AssocSet.h StdDomain.h
pobj/StdDomain.o: Domain.h SimObject.h Design.h VecSpec.h Objective.h Node.h
pobj/StdDomain.o: ../space/coord3d.h Element.h
pobj/System.o: System.h
pobj/Trans.o: /usr/include/CC/stdlib.h /usr/include/CC/_G_config.h
pobj/Trans.o: /usr/include/CC/stddef.h /usr/include/CC/math.h
pobj/Trans.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/Trans.o: /usr/include/CC/assert.h /usr/include/CC/stdio.h Trans.h
pobj/Trans.o: ../space/coord3d.h ../Set/SimpleSet.h ../Set/Set.h Node.h
pobj/Trans.o: ../Set/AssocSet.h Element.h Basis.h GaussContext.h GPInfo.h
pobj/VecSpec.o: VecSpec.h ../Matrix/Include/Vector.h ../Misc/GC.h
pobj/VecSpec.o: /usr/include/CC/iostream.h /usr/include/CC/streambuf.h
pobj/VecSpec.o: /usr/include/CC/_G_config.h /usr/include/CC/stddef.h
pobj/VecSpec.o: ../Set/SimpleSet.h ../Set/Set.h /usr/include/CC/math.h
pobj/VecSpec.o: Design.h /usr/include/CC/assert.h /usr/include/CC/stdio.h
pobj/mygauleg.o: /usr/include/CC/math.h /usr/include/CC/stdio.h
pobj/mygauleg.o: /usr/include/CC/_G_config.h
pobj/nostd.o: MatProp.h /usr/include/CC/iostream.h
pobj/nostd.o: /usr/include/CC/streambuf.h /usr/include/CC/_G_config.h
pobj/nostd.o: /usr/include/CC/stdlib.h /usr/include/CC/stddef.h
