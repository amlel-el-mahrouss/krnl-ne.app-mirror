FROM debian

# Update & upgrade APT.

RUN apt-get update && apt-get upgrade -y

COPY ./ ./src

RUN echo "The NeKernel has been copied to the Docker instance."

# Reserved for NeKernel Debugger Protocol
EXPOSE 8081/tcp

# For the Debugger too (UDP)
EXPOSE 8081/udp

