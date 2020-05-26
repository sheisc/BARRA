# BARRA
Burn After Reading: A Shadow Stack with Microsecond-level Runtime Rerandomization for Protecting Return Addresses


## Requirement

 Ubuntu 16.04, pip, wllvm, gcc-multilib, g++-multilib, git 

```sh
barra@ubuntu:~$ sudo apt install python-pip
barra@ubuntu:~$ sudo pip install wllvm
barra@ubuntu:~$ sudo apt-get install gcc-multilib g++-multilib
barra@ubuntu:~$ sudo apt-get install git
```

## How to Build

```sh
barra@ubuntu:~$ mkdir -p src
barra@ubuntu:~$ cd src
barra@ubuntu:~/src$ git clone https://github.com/sheisc/BARRA.git
```

Suppose BARRA is installed in /home/iron/src/BARRA.
Then please use gedit to open /home/iron/src/BARRA/R4/env.sh and 
set the environment variable R4PATH to /home/iron/src/BARRA/R4
(i.e., <span style="color:red">export R4PATH=/home/iron/src/BARRA/R4</span>).
Then you can build BARRA as follows.


```sh
barra@ubuntu:~/src$ cd BARRA/R4
barra@ubuntu:~/src/BARRA/R4$ . ./env.sh 
barra@ubuntu:~/src/BARRA/R4$ ./build.sh
```

## How to Use


### 1. Download and intall [VMware Workstation Player 15](https://www.vmware.com/products/workstation-player/workstation-player-evaluation.html). 
	 
> The free version is available for non-commercial use. 

### 2. Download the compressed image of our virtual machine [Ubuntu16.04.vmx](https://drive.google.com/file/d/1MnK8p9TcZIYDOr9hZKJOoL50cldv0J7W/view?usp=sharing)  and uncompress it.

> Ubuntu16.04(Password_123456).tar.bz2

### 3. Use VMware Workstation Player 15 to open the uncompressed virtual machine Ubuntu16.04.vmx.

> The **password** to login is **123456**



#### (1) Figure 7 in our paper (A "Hello World" example is given in ~/src/BARRA/R4/example).

    Please search .unsw.randomval in the file ~/src/BARRA/R4/example/demo.instr.s for some of the instrumented assembly code 
    shown in Figure 7(d).
    The instrumented data section shown in Figure 7(e) is contained in ~/src/BARRA/R4/example/unsw_addr.s.
    Due to the requirement for anonymity, we manually rename some variables in our paper:
       .unsw.retaddr.bak => all_ret_addrs,   .unsw.randomval => BAR_randval,  .unsw.retaddr => BAR_mtable.
    UNSW is the abbreviation of University of New South Wales.

    barra@ubuntu:~$ cd ~/src/BARRA/R4/example
    barra@ubuntu:~/src/BARRA/R4/example$ . ../env.sh 
    barra@ubuntu:~/src/BARRA/R4/example$ make CC="wllvm"
    barra@ubuntu:~/src/BARRA/R4/example$ ${R4PATH}/tools/barra.sh demo

#### (2) Table 1 in our paper

    barra@ubuntu:~$ cd ~/src/BARRA/R4
    barra@ubuntu:~/src/BARRA/R4$ ./Table1.sh

#### (3) Table 3 in our paper

    barra@ubuntu:~$ cd ~/src/BARRA/R4
    barra@ubuntu:~/src/BARRA/R4$ ./Table3.sh


#### (4) Figure 10 in our paper (see ~/src/BARRA/R4/Fig10.png)

    Please open 4 terminals, enter the directory ~/src/BARRA/R4/echo, 
    and then follow the instructions as shown in ~/src/BARRA/R4/Fig10.png

    barra@ubuntu:~$ cd ~/src/BARRA/R4/echo


