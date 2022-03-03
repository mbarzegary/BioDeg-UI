FROM ubuntu AS builder

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    automake \
    bison \
    cmake \
    flex \
    g++ \
    gfortran \
    git \
    make \
    libopenblas-dev \
    libhdf5-dev \
    libgsl-dev \
    libfftw3-dev \
    libnlopt-dev \
    patch \
    pkg-config \
    python3-minimal \
    python3-distutils \
    unzip \
    wget \
    && rm -rf /var/lib/apt/lists/*

ENV DIRPATH /root/FreeFem-sources

RUN git clone --depth 1 --single-branch https://github.com/FreeFem/FreeFem-sources.git $DIRPATH

WORKDIR $DIRPATH

RUN autoreconf -i \
    && ./configure --enable-download --enable-optim --enable-generic --prefix=/usr/freefem/ \
    && ./3rdparty/getall -o PETSc -a

RUN cd $DIRPATH/3rdparty/ff-petsc \
    && make petsc-slepc

RUN ./reconfigure && make -j 4

RUN make install

FROM ubuntu

# Install dependencies
RUN apt-get update && apt-get install -y \
    libopenblas-dev \
    libhdf5-dev \
    libgsl-dev \
    libfftw3-dev \
    libnlopt-dev \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir /usr/freefem
COPY --from=builder /usr/freefem /usr/freefem

ENV PATH /usr/freefem/bin:$PATH
