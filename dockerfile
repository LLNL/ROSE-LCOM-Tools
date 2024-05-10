#dockerfile

FROM amd64/ubuntu:20.04

# Fix caching issues caused by MDM on macOS.
# RUN touch /etc/apt/apt.conf.d/99fixbadproxy
# RUN echo "Acquire::http::Pipeline-Depth 0;Acquire::http::No-Cache true;Acquire::BrokenProxy true;" >> /etc/apt/apt.conf.d/99fixbadproxy

# Prepare to install the required system packages.
RUN apt-get update
# Needed to prevent tzdata install from requesting time zone info interactively.
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get install -y bison byacc cmake dbus flex fontconfig g++ git gnat gprbuild groff libtool libx11-xcb1 make perl-doc python3 wget
RUN apt-get clean

# Set up GNAT.
WORKDIR /
ENV GNAT_HOME="/GNAT/2019"
ENV PATH="$GNAT_HOME/bin:$PATH"
# ENV LD_LIBRARY_PATH="$GNAT_HOME/lib64:$GNAT_HOME/lib:$LD_LIBRARY_PATH"
RUN git clone --depth 1 https://github.com/AdaCore/gnat_community_install_script.git
WORKDIR /gnat_community_install_script
ADD https://community.download.adacore.com/v1/0cd3e2a668332613b522d9612ffa27ef3eb0815b?filename=gnat-community-2019-20190517-x86_64-linux-bin /gnat-community-2019-20190517-x86_64-linux-bin
RUN sh install_package.sh /gnat-community-2019-20190517-x86_64-linux-bin $GNAT_HOME

# Set up ASIS.
WORKDIR /
ADD https://community.download.adacore.com/v1/52c69e7295dc301ce670334f8150193ecbec580d?filename=asis-2019-20190517-18AB5-src.tar.gz /asis.tar.gz
RUN tar -xvzf asis.tar.gz
WORKDIR /asis-2019-20190517-18AB5-src
RUN sed -i 's/for Library_Kind use \"static\";/for Library_Kind use \"dynamic\";/g' asis.gpr
RUN make all install prefix=$GNAT_HOME

# Set up Boost.
WORKDIR /
ENV BOOST_HOME="/boost_1_83_0/install"
ENV BOOST_ROOT="$BOOST_HOME"
ENV LD_LIBRARY_PATH="$BOOST_HOME/stage/lib":$LD_LIBRARY_PATH
ENV BOOST_LIB="$BOOST_HOME/stage/libexport"
ADD https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.tar.bz2 /
RUN tar -xvf boost_1_83_0.tar.bz2
WORKDIR /boost_1_83_0
RUN mkdir -p tools/build/src/
RUN echo "using gcc : 8.3.1 : $GNAT_HOME/bin/g++ ; " >> tools/build/src/user-config.jam
RUN bash bootstrap.sh
RUN ./b2 -j$(nproc)
RUN ./b2 install --prefix=$BOOST_HOME

# Set up ROSE.
WORKDIR /
ENV ROSE_REPO="/rose"
ENV ROSE_HOME="$ROSE_REPO/install_tree"
ENV ASIS_ADAPTER="$ROSE_REPO/build_tree/src/frontend/Experimental_Ada_ROSE_Connection/parser/asis_adapter"
RUN git clone --depth 1 https://github.com/rose-compiler/rose.git
WORKDIR $ROSE_REPO
RUN ./build
RUN mkdir -p build_tree
WORKDIR build_tree

# NOTE: Adding GNAT to the path provides an outdated dynamic link library, so we only use it temporarily during the build steps that require it.
RUN LD_LIBRARY_PATH="$GNAT_HOME/lib64:$GNAT_HOME/lib:$LD_LIBRARY_PATH" ../configure --prefix=$ROSE_HOME --enable-languages=c,c++ --enable-experimental_ada_frontend --without-swi-prolog --without-cuda --without-java --without-python --with-boost=$BOOST_HOME --with-boost-libdir=$BOOST_HOME/lib --verbose --with-DEBUG=-ggdb --with-alloc-memset=2 --with-OPTIMIZE="-O0 -march=native -p -DBOOST_TIMER_ENABLE_DEPRECATED" --with-WARNINGS="-Wall -Wextra -Wno-misleading-indentation -Wno-unused-parameter" CXX=$GNAT_HOME/bin/g++ CC=$GNAT_HOME/bin/gcc
RUN make core -j$(nproc)
RUN make install-core -j$(nproc)
RUN make check-core -j$(nproc)
# Build the ROSE AST DOT graph generator.
WORKDIR exampleTranslators
RUN make -j$(nproc)

# Set up GTests Parallel.
WORKDIR /
ENV GTEST_REPO="/gtest-parallel"
RUN git clone --depth 1 https://github.com/google/gtest-parallel.git

# Set up LCOM Metric Analyzer.
ENV LCOM_HOME="/ROSE-LCOM-TOOL"
WORKDIR $LCOM_HOME
# Copy the repository data.
COPY . .
# Ensure the build directory exists.
RUN mkdir -p build

RUN rm -r build/; exit 0
RUN cmake -S . -B build
RUN cmake --build build --parallel $(nproc)
RUN cd build && ctest

# Optional code.
RUN bash acats.sh & bash osc.sh

# ENTRYPOINT ["python3", "script/allTest.py"]