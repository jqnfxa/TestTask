FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive


# Dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    qtbase5-dev \
    qt5-qmake \
    qtdeclarative5-dev \
    qml-module-qtquick-controls2 \
    qml-module-qtquick-layouts \
    qml-module-qt-labs-platform \
    && rm -rf /var/lib/apt/lists/*


WORKDIR /usr/src/project
COPY . .
RUN mkdir -p build

CMD ["/bin/bash", "-c", "cd build && cmake .. && make && ctest --output-on-failure"]
