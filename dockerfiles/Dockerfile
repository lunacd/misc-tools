FROM rockylinux/rockylinux:9 AS builder
# Install system dependencies
RUN dnf install -y 'dnf-command(config-manager)' && \
    dnf config-manager --set-enabled crb && dnf install -y epel-release && crb enable && \
    dnf install -y gcc-toolset-14-gcc-c++ cmake python3.12 python3.12-pip perl make ImageMagick-c++-devel && \
    dnf clean all
# Install conan
RUN python3.12 -m pip install conan
# Setup conan profile
COPY dockerfiles/conan_profile /root/.conan2/profiles/default
# Install conan deps
COPY conanfile.txt /backend/conanfile.txt
COPY CMakeLists.txt /backend/CMakeLists.txt
RUN cd /backend && \
    source /opt/rh/gcc-toolset-14/enable && \
    conan install . --build=missing --profile=default
# Build backend
COPY src /backend/src/
COPY tests/ /backend/tests/
RUN source /opt/rh/gcc-toolset-14/enable && \
    cd /backend/build && \
    cmake .. --preset conan-release && \
    cd /backend && cmake --build ./build --preset conan-release

FROM rockylinux/rockylinux:9 AS app

ARG COMPONENT=""

# There is this library that's unfortunately linked dynamically
RUN dnf install -y epel-release && \
    dnf install -y ImageMagick-c++ && \
    dnf clean all
COPY --from=builder /backend/build/Release/src/${COMPONENT}/${COMPONENT} /backend/

WORKDIR /
SHELL [ "/bin/bash", "-c" ]
ENV LUNACD_COMPONENT=${COMPONENT}
CMD /backend/${LUNACD_COMPONENT}
