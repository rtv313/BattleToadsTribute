FROM ubuntu:20.04

ENV TZ=America/Sao_Paulo
RUN apt update && DEBIAN_FRONTEND=noninteractive apt install --no-install-recommends -y make xterm sudo build-essential \
libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libsdl2-mixer-dev \
pulseaudio pulseaudio-utils \
mesa-utils libgl1-mesa-glx \
indent \
&& rm -rf /var/lib/apt/lists/*

ENV SDL_AUDIODRIVER=PulseAudio
COPY pulse-client.conf /etc/pulse/client.conf

ARG USER=docker
ARG UID=1000
ARG GID=1000
# default password for user
ARG PW=docker

# Option1: Using unencrypted password/ specifying password
RUN useradd -m ${USER} --uid=${UID} && echo "${USER}:${PW}" | chpasswd && adduser docker sudo && usermod -aG audio,pulse,pulse-access docker

# Setup default user, when enter docker container
USER ${UID}:${GID}
WORKDIR /home/${USER}
