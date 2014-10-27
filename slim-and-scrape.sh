#/usr/bin/env bash

if [[ $# != 1 ]]; then
	echo 'usage: slim-and-scrape.sh DIRECTORY_TO_MAKE'
	exit 0
fi


SUBDIR=$1

mkdir -p $SUBDIR

cp yaml-cpp/yaml-cpp/node/detail/bool_type.h ${SUBDIR}/detail-bool_type.h
cp yaml-cpp/yaml-cpp/node/detail/impl.h ${SUBDIR}/detail-impl.h
cp yaml-cpp/yaml-cpp/node/detail/iterator.h ${SUBDIR}/detail-iterator.h
cp yaml-cpp/yaml-cpp/node/detail/iterator_fwd.h ${SUBDIR}/detail-iterator_fwd.h
cp yaml-cpp/yaml-cpp/node/detail/memory.h ${SUBDIR}/detail-memory.h
cp yaml-cpp/yaml-cpp/node/detail/node.h ${SUBDIR}/detail-node.h
cp yaml-cpp/yaml-cpp/node/detail/node_data.h ${SUBDIR}/detail-node_data.h
cp yaml-cpp/yaml-cpp/node/detail/node_iterator.h ${SUBDIR}/detail-node_iterator.h
cp yaml-cpp/yaml-cpp/node/detail/node_ref.h ${SUBDIR}/detail-node_ref.h
cp yaml-cpp/src/binary.cpp ${SUBDIR}/
cp yaml-cpp/src/collectionstack.h ${SUBDIR}/
cp yaml-cpp/src/contrib/graphbuilder.cpp ${SUBDIR}/
cp yaml-cpp/src/contrib/graphbuilderadapter.cpp ${SUBDIR}/
cp yaml-cpp/src/contrib/graphbuilderadapter.h ${SUBDIR}/
cp yaml-cpp/src/convert.cpp ${SUBDIR}/
cp yaml-cpp/src/directives.cpp ${SUBDIR}/
cp yaml-cpp/src/directives.h ${SUBDIR}/
cp yaml-cpp/src/emit.cpp ${SUBDIR}/
cp yaml-cpp/src/emitfromevents.cpp ${SUBDIR}/
cp yaml-cpp/src/emitter.cpp ${SUBDIR}/
cp yaml-cpp/src/emitterstate.cpp ${SUBDIR}/
cp yaml-cpp/src/emitterstate.h ${SUBDIR}/
cp yaml-cpp/src/emitterutils.cpp ${SUBDIR}/
cp yaml-cpp/src/emitterutils.h ${SUBDIR}/
cp yaml-cpp/src/exp.cpp ${SUBDIR}/
cp yaml-cpp/src/exp.h ${SUBDIR}/
cp yaml-cpp/src/indentation.h ${SUBDIR}/
cp yaml-cpp/src/memory.cpp ${SUBDIR}/
cp yaml-cpp/src/node.cpp ${SUBDIR}/
cp yaml-cpp/src/node_data.cpp ${SUBDIR}/
cp yaml-cpp/src/nodebuilder.cpp ${SUBDIR}/
cp yaml-cpp/src/nodebuilder.h ${SUBDIR}/
cp yaml-cpp/src/nodeevents.cpp ${SUBDIR}/
cp yaml-cpp/src/nodeevents.h ${SUBDIR}/
cp yaml-cpp/src/null.cpp ${SUBDIR}/
cp yaml-cpp/src/ostream_wrapper.cpp ${SUBDIR}/
cp yaml-cpp/src/parse.cpp ${SUBDIR}/
cp yaml-cpp/src/parser.cpp ${SUBDIR}/
cp yaml-cpp/src/ptr_stack.h ${SUBDIR}/
cp yaml-cpp/src/ptr_vector.h ${SUBDIR}/
cp yaml-cpp/src/regex.cpp ${SUBDIR}/
cp yaml-cpp/src/regex.h ${SUBDIR}/
cp yaml-cpp/src/regeximpl.h ${SUBDIR}/
cp yaml-cpp/src/scanner.cpp ${SUBDIR}/
cp yaml-cpp/src/scanner.h ${SUBDIR}/
cp yaml-cpp/src/scanscalar.cpp ${SUBDIR}/
cp yaml-cpp/src/scanscalar.h ${SUBDIR}/
cp yaml-cpp/src/scantag.cpp ${SUBDIR}/
cp yaml-cpp/src/scantag.h ${SUBDIR}/
cp yaml-cpp/src/scantoken.cpp ${SUBDIR}/
cp yaml-cpp/src/setting.h ${SUBDIR}/
cp yaml-cpp/src/simplekey.cpp ${SUBDIR}/
cp yaml-cpp/src/singledocparser.cpp ${SUBDIR}/
cp yaml-cpp/src/singledocparser.h ${SUBDIR}/
cp yaml-cpp/src/stream.cpp ${SUBDIR}/
cp yaml-cpp/src/stream.h ${SUBDIR}/
cp yaml-cpp/src/streamcharsource.h ${SUBDIR}/
cp yaml-cpp/src/stringsource.h ${SUBDIR}/
cp yaml-cpp/src/tag.cpp ${SUBDIR}/
cp yaml-cpp/src/tag.h ${SUBDIR}/
cp yaml-cpp/src/token.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/anchor.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/binary.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/contrib/anchordict.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/contrib/graphbuilder.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/dll.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/emitfromevents.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/emitter.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/emitterdef.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/emittermanip.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/eventhandler.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/exceptions.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/mark.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/node/convert.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/node/emit.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/node/impl.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/node/iterator.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/node/node.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/node/parse.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/node/ptr.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/node/type.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/noncopyable.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/null.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/ostream_wrapper.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/parser.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/stlemitter.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/traits.h ${SUBDIR}/
cp yaml-cpp/yaml-cpp/yaml.h ${SUBDIR}/
cp yaml-cpp/yaml_core.hh ${SUBDIR}/

cd $SUBDIR


for file in *.cpp; do
    mv "$file" "`basename $file .cpp`.cxx"
done

for file in *.hh; do
    mv "$file" "`basename $file .hh`.h"
done

mkdir -p src 
mkdir -p AGILEPack

mv *.h AGILEPack/ 
mv *.cxx src/

perl -pi -e 's/include \"yaml-cpp\/yaml-cpp\//include \"/g' AGILEPack/*.h
perl -pi -e 's/include \"yaml-cpp\//include \"/g' AGILEPack/*.h
perl -pi -e 's/include \"node\//include \"/g' AGILEPack/*.h
perl -pi -e 's/include \"detail\//include \"detail-/g' AGILEPack/*.h
perl -pi -e 's/include \"contrib\//include \"/g' AGILEPack/*.h
perl -pi -e 's/include \"boost_mod\//include <boost\//g' AGILEPack/*.h

perl -pi -e 's/\.hpp\"/\.hpp>/g' AGILEPack/*.h
perl -pi -e 's/\.hpp\"/\.hpp>/g' src/*.cxx

perl -pi -e 's/include \"\.\.\//include \"/g' AGILEPack/*.h

perl -pi -e 's/include \"yaml-cpp\/yaml-cpp\//include \"/g' src/*.cxx
perl -pi -e 's/include \"yaml-cpp\//include \"/g' src/*.cxx
perl -pi -e 's/include \"node\//include \"/g' src/*.cxx
perl -pi -e 's/include \"detail\//include \"detail-/g' src/*.cxx
perl -pi -e 's/include \"contrib\//include \"/g' src/*.cxx
perl -pi -e 's/include \"boost_mod\//include <boost\//g' src/*.cxx


cd $OLDPWD

cp AGILEPack ${SUBDIR}/AGILEPack.h
cp Base ${SUBDIR}/Base.h
cp Core.hh ${SUBDIR}/
cp ROOT.hh ${SUBDIR}/
cp agile/agile_base.hh ${SUBDIR}/
cp agile/include/activation.hh ${SUBDIR}/
cp agile/include/architecture.hh ${SUBDIR}/
cp agile/include/autoencoder.hh ${SUBDIR}/
cp agile/include/basedefs.hh ${SUBDIR}/
cp agile/include/layer.hh ${SUBDIR}/
cp agile/src/activation.cxx ${SUBDIR}/
cp agile/src/architecture.cxx ${SUBDIR}/
cp agile/src/autoencoder.cxx ${SUBDIR}/
cp agile/src/basedefs.cxx ${SUBDIR}/
cp agile/src/layer.cxx ${SUBDIR}/
cp dataframe/dataframe_core.hh ${SUBDIR}/
cp dataframe/include/csv_reader.hh ${SUBDIR}/
cp dataframe/include/dataframe.hh ${SUBDIR}/
cp dataframe/src/csv_reader.cxx ${SUBDIR}/
cp dataframe/src/dataframe.cxx ${SUBDIR}/
cp include/AGILEClient.hh ${SUBDIR}/
cp include/model_frame.hh ${SUBDIR}/
cp include/neural_net.hh ${SUBDIR}/
cp root/include/binner.hh ${SUBDIR}/
cp root/include/numeric_handler.hh ${SUBDIR}/
cp root/include/smart_chain.hh ${SUBDIR}/
cp root/include/tree_reader.hh ${SUBDIR}/
cp root/root_core.hh ${SUBDIR}/
cp root/src/smart_chain.cxx ${SUBDIR}/
cp root/src/tree_reader.cxx ${SUBDIR}/
cp src/model_frame.cxx ${SUBDIR}/
cp src/neural_net.cxx ${SUBDIR}/


cd $SUBDIR 

for file in *.hh; do
    mv "$file" "`basename $file .hh`.h"
done

mv *.h AGILEPack/ 
mv *.cxx src/

perl -pi -e 's/include \"yaml-cpp\//include \"/g' AGILEPack/*.h
perl -pi -e 's/include \"yaml-cpp\//include \"/g' src/*.cxx
perl -pi -e 's/include \"agile\//include \"/g' AGILEPack/*.h
perl -pi -e 's/include \"agile\//include \"/g' src/*.cxx
perl -pi -e 's/include \"dataframe\//include \"/g' AGILEPack/*.h
perl -pi -e 's/include \"dataframe\//include \"/g' src/*.cxx
perl -pi -e 's/include \"root\//include \"/g' AGILEPack/*.h
perl -pi -e 's/include \"root\//include \"/g' src/*.cxx
perl -pi -e 's/include \"include\//include \"/g' AGILEPack/*.h
perl -pi -e 's/include \"include\//include \"/g' src/*.cxx
perl -pi -e 's/\.hh\"/\.h\"/g' AGILEPack/*.h
perl -pi -e 's/\.hh\"/\.h\"/g' src/*.cxx
perl -pi -e 's/Base/Base\.h/g' AGILEPack/AGILEPack.h

# don't know why this is necessary for compilation
perl -pi -e 's/token\.h/\.\.\/AGILEPack\/token\.h/g' src/tag.cxx

echo "
# Makefile for athena agilepack

# --- depends on ROOT, Eigen and Boost
EIGEN = /afs/cern.ch/atlas/offline/external/Eigen/3.2.1/x86_64-slc6-gcc48-opt
BOOST = /afs/cern.ch/sw/lcg/external/Boost/1.55.0_python2.7/x86_64-slc6-gcc47-opt/include/boost-1_55

# --- set compiler and flags
CC  = gcc
CXX = g++
CPPFLAGS = -I./AGILEPack -I\$(EIGEN) -I\$(BOOST)  -I./ -std=c++11

ifeq (\$(CXX),clang++)
CXXFLAGS += -stdlib=libc++
endif

ROOTCFLAGS    := \$(shell root-config --cflags)
CXXFLAGS      += \$(ROOTCFLAGS)

# --- define objects
SRCS=\$(shell ls ./src/*.cxx | xargs -n 1 basename)
OBJS=\$(subst .cxx,.o,\$(SRCS))

# -----------------------------------------------------

# compile rule
all: libAGILEPack.a
	@echo \"libAGILEPack.a build successful.\"

libAGILEPack.a: \$(OBJS:%=%)
	@echo \"linking objects to --> \$@\"
	@ar rc \$@ $^ && ranlib \$@

./%.o:./src/%.cxx
	@echo compiling $<
	@\$(CXX) -c \$(CXXFLAGS) \$(CPPFLAGS) $< -o \$@

# clean
clean:
	rm -f *.o *.d libAGILEPack.a core" > Makefile
