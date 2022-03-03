FROM ubuntu:20.04 AS builder

ENV DEBIAN_FRONTEND noninteractive
ENV QT_VERSION_MAJOR 5.15
ENV QT_VERSION 5.15.2
ENV QT_DEST /usr/local/Qt-"$QT_VERSION"
ENV QT5BINDIR /usr/local/Qt-"$QT_VERSION"/bin
ENV LD_LIBRARY_PATH /usr/local/Qt-$QT_VERSION/lib
ENV QT_QPA_PLATFORM_PLUGIN_PATH /usr/lib/x86_64-linux-gnu/qt5/plugins/platforms
ENV QT_QPA_FONTDIR /usr/lib/x86_64-linux-gnu/qt5/lib/fonts
ENV XDG_DATA_HOME /root/.local/share
ENV PATH $QT_DEST/bin:$PATH

RUN apt-get update && apt autoremove -y && apt install -y wget xz-utils

RUN wget -N https://download.qt.io/official_releases/qt/$QT_VERSION_MAJOR/$QT_VERSION/single/qt-everywhere-src-$QT_VERSION.tar.xz -P /root/Downloads/qt \
    && tar -xf /root/Downloads/qt/qt-everywhere-src-$QT_VERSION.tar.xz -C /root/Downloads/qt

RUN apt install -y   libfontconfig1-dev libfreetype6-dev libx11-dev libx11-xcb-dev libxext-dev libxfixes-dev libxi-dev libxrender-dev   \
	         libxcb1-dev libxcb-glx0-dev libxcb-keysyms1-dev libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev                \
	         libxcb-sync0-dev libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev libxcb-render-util0-dev                    \
	         libxcb-xinerama0-dev libxkbcommon-dev libxkbcommon-x11-dev libclang-dev                                            \
	         freeglut3-dev mesa-utils libdrm-dev libgles2-mesa-dev		                                                    			\
	         binutils g++ cmake g++ mesa-common-dev build-essential libglew-dev libglm-dev                                      \
	         make gcc pkg-config libgl1-mesa-dev libxcb1-dev libfontconfig1-dev libxkbcommon-x11-dev python libgtk-3-dev        \
	         build-essential default-jre openjdk-8-jdk-headless android-sdk android-sdk-platform-23 libc6-i386                  \
	         libdrm-dev libgles2-mesa-dev libzc-dev libxcb-sync-dev libsmartcols-dev libicecc-dev libpthread-workqueue-dev      \
	         libgstreamer1.0-dev libgcrypt20-dev libqt5gui5-gles qca-qt5-2-utils xorg xorg-dev

RUN cd /root/Downloads/qt/qt-everywhere-src-$QT_VERSION/ \
    && ./configure -release -no-opengl -opensource -confirm-license -make libs -nomake tools -nomake examples \
     -nomake tests -skip qt3d -skip qtandroidextras -skip qtcanvas3d -skip qtconnectivity -skip qtdatavis3d -skip qtdeclarative -skip qtdoc \
      -skip qtgamepad -skip qtgraphicaleffects -skip qtimageformats -skip qtlocation -skip qtlottie -skip qtmacextras -skip qtmultimedia \
      -skip qtnetworkauth -skip qtpurchasing -skip qtquick3d -skip qtquickcontrols -skip qtquickcontrols2 -skip qtquicktimeline \
      -skip qtremoteobjects -skip qtscript -skip qtscxml -skip qtsensors -skip qtserialbus -skip qtserialport -skip qtspeech -skip qtsvg \
      -skip qttools -skip qttranslations -skip qtvirtualkeyboard -skip qtwebchannel -skip qtwebengine -skip qtwebglplugin -skip qtwebsockets \
      -skip qtwebview -skip qtwinextras -skip qtxmlpatterns \
      -sysconfdir /etc/xdg -system-harfbuzz -no-rpath \
    && make -j8 \
    && make install

RUN rm /root/Downloads/qt/qt-everywhere-src-$QT_VERSION.tar.xz \
    && rm -r /root/Downloads/qt/qt-everywhere-src-$QT_VERSION


FROM ubuntu:20.04

ENV QT_VERSION_MAJOR 5.15
ENV QT_VERSION 5.15.2
ENV QT_DEST /usr/local/Qt-"$QT_VERSION"
ENV QT5BINDIR /usr/local/Qt-"$QT_VERSION"/bin
ENV LD_LIBRARY_PATH /usr/local/Qt-$QT_VERSION/lib
ENV QT_QPA_PLATFORM_PLUGIN_PATH /usr/lib/x86_64-linux-gnu/qt5/plugins/platforms
ENV QT_QPA_FONTDIR /usr/lib/x86_64-linux-gnu/qt5/lib/fonts
ENV XDG_DATA_HOME /root/.local/share
ENV PATH $QT_DEST/bin:$PATH

RUN apt-get update && apt-get install -y \
    build-essential \
    mesa-common-dev \
    cmake \
    libpcre2-dev \
    libglib2.0-0 \
    libpng16-16 \
    libharfbuzz-dev \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir $QT_DEST
COPY --from=builder $QT_DEST $QT_DEST
