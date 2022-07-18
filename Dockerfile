FROM gcc:4.9
COPY . /usr/src/printTNs
WORKDIR /usr/src/printTNs
RUN gcc -std=c11 -o printTNs main.c
CMD [ "./printTNs"]