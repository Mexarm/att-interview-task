docker rmi -f print-tn-app
docker build --no-cache --progress=plain -t print-tn-app .
cat sip_tel_list.txt | docker run -i print-tn-app /bin/bash -c /usr/src/printTNs/printTNs 