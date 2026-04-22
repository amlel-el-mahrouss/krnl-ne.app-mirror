FROM debian

# Update & upgrade APT.

RUN apt-get update && apt-get upgrade -y

COPY ./ ./src

RUN echo "NeKernel has been copied to the Docker instance."
