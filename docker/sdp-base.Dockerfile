FROM ubuntu:24.04

RUN apt-get update                  && \
    apt-get install -y                 \
        build-essential                \
        git                            \
        cmake                          \
        libopencv-dev                  \
        libboost-all-dev               \
        libsystemd-dev              && \
   apt-get clean                    && \
    rm -rf /var/lib/apt/lists/*

RUN git clone -b v1.64.0                           \
              https://github.com/grpc/grpc      && \
    cd grpc                                     && \
    git submodule update --init                 && \
    mkdir -p cmake/build                        && \
    cd cmake/build                              && \
    cmake ../..  -DgRPC_INSTALL=ON                 \
                 -DCMAKE_BUILD_TYPE=Release        \
                 -DBUILD_SHARED_LIBS=ON            \
                 -DgRPC_ABSL_PROVIDER=module      \
                 -DgRPC_CARES_PROVIDER=module     \
                 -DgRPC_PROTOBUF_PROVIDER=module  \
                 -DgRPC_RE2_PROVIDER=module       \
                 -DgRPC_SSL_PROVIDER=module       \
                 -DgRPC_ZLIB_PROVIDER=module   && \
    cmake --build . --target install -- -j4    && \
    ldconfig

CMD ["/bin/bash"]

