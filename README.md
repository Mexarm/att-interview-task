# AT&T interview task
- Armando Hernandez Marin


## instructions
### run the example
- docker is required to run the example

#### this shell script compiles main.c in a docker container and then executes:  cat sip_tel_list.txt | printTNs 
#### (printTNs is the compiled binary.)

```
./start-container.sh
```

### compiler does not report warnings:

```
...
#8 [4/4] RUN gcc -Wall -o printTNs main.c
#8 sha256:34ea93ab6c54f002c42d91b46f3e72ab28f341a6f6edc08b68f0f2659a33499b
#8 DONE 0.4s
...
```

#### usage

```
./printTNs inputfile.txt
cat inputfile.txt | ./printTNs -
cat inputfile.txt | ./printTNs
./printTNs < inputfile.txt
```

#### example

```
./printTNs sip_tel_list.txt
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