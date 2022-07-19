FROM gcc:4.9
COPY . /usr/src/printTNs
WORKDIR /usr/src/printTNs
RUN gcc -Wall -o printTNs main.c
CMD [ "./printTNs", "sip_tel_list.txt"]