FROM ubuntu:24.04

RUN apt-get update                  && \
    apt-get install -y                 \
        build-essential                \
        cmake                          \
        libopencv-dev                  \
        libboost-all-dev            && \
    apt-get clean                   && \
    rm -rf /var/lib/apt/lists/*

CMD ["/bin/bash"]
