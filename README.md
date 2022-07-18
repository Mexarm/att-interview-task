# AT&T interview task
- Armando Hernandez Marin

- docker is required to run the example

### run the example
#### this compiles main.c in a docker container and then executes:  cat sip_tel_list.txt | printTNs 
#### printTNs is the compiled binary.

```
./start-container.sh
```

### compiler does not report warnings:

```
...
#8 [4/4] RUN gcc -std=c11 -o printTNs main.c
#8 sha256:006da23eb8ac11a59eabcc34dabacd40963cb0c54d5a8d0cdf53b823a1f1f03e
#8 DONE 0.3s
...
```

#### output

```
TN: 18664190071                 TON: 0          NPI: 0
TN: 19736194383                 TON: 1          NPI: 1
TN: 16177503774                 TON: 1          NPI: 1
TN: 15618567581                 TON: 1          NPI: 1
TN: 18668322363                 TON: 2          NPI: 1
TN: 7736825791                  TON: 2          NPI: 1
TN: 9786597900                  TON: 2          NPI: 1
TN: 5212291533253               TON: 1          NPI: 1
TN: 7065268527                  TON: 2          NPI: 1
TN: 3733267                     TON: 3          NPI: 1
TN: 2674010405                  TON: 2          NPI: 1
TN: 9786597900                  TON: 2          NPI: 1
TN: 13478963834                 TON: 1          NPI: 1
TN: 13526169323                 TON: 1          NPI: 1
TN: 19736194383                 TON: 1          NPI: 1
```