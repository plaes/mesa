#!/bin/bash

set -e
set -o xtrace

export DEBIAN_FRONTEND=noninteractive

apt-get install -y ca-certificates

sed -i -e 's/http:\/\/deb/https:\/\/deb/g' /etc/apt/sources.list
echo 'deb https://deb.debian.org/debian buster-backports main' >/etc/apt/sources.list.d/backports.list

apt-get update

# Use newer packages from backports by default
cat >/etc/apt/preferences <<EOF
Package: *
Pin: release a=buster-backports
Pin-Priority: 500
EOF

apt-get dist-upgrade -y

apt-get install -y --no-remove \
      cmake \
      g++ \
      git \
      gcc \
      libexpat1 \
      libgbm-dev \
      libgles2-mesa-dev \
      libpng16-16 \
      libpng-dev \
      libvulkan1 \
      libvulkan-dev \
      libwayland-server0 \
      libxcb-randr0 \
      libxcb-xfixes0 \
      libxkbcommon0 \
      libxkbcommon-dev \
      libxrender1 \
      libxrender-dev \
      libllvm8 \
      meson \
      patch \
      pkg-config \
      python3-distutils \
      python \
      xauth \
      xvfb


############### Build dEQP runner

. .gitlab-ci/build-cts-runner.sh

############### Build dEQP VK

. .gitlab-ci/build-deqp-vk.sh


############### Uninstall the build software

apt-get purge -y \
      cmake \
      g++ \
      gcc \
      git \
      libgbm-dev \
      libgles2-mesa-dev \
      libpng-dev \
      libvulkan-dev \
      libxkbcommon-dev \
      libxrender-dev \
      meson \
      patch \
      pkg-config \
      python

apt-get autoremove -y --purge
