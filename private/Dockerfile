FROM debian

# Update & upgrade APT.

RUN apt-get update && apt-get upgrade -y

COPY ./ ./src

RUN echo "NeKernel: Contents have been copied to Docker."

# Reserved for NeKernel Debugger Protocol
EXPOSE 8081/tcp

# For the NeKernel Debugger Protocol as well (UDP)
EXPOSE 8081/udp

