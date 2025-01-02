# This file is part of the Clay3DS project.
#
# (c) 2025 Tommaso Dimatore
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

FROM devkitpro/devkitarm:latest AS builder
WORKDIR /app

# Copy the source project's source code to the image.
COPY . .

# Install the 3DS SDK and tools using devkitPro's package manager.
RUN dkp-pacman --sync 3ds-dev --noconfirm

# Use the CMake wrapper provided by devkitPro to configure the project.
RUN ${DEVKITPRO}/portlibs/3ds/bin/arm-none-eabi-cmake -S . -DCLAY3DS_BUILD_EXAMPLES=1 -B build

# Compile the project in release mode.
RUN cmake --build build --config release

FROM scratch AS exporter

# Copy all the compiled 3dsx executables to the exporter stage.
COPY --from=builder /app/build/examples/*.3dsx /
