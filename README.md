kmd
====

An OS kernel for x86-64 I am working on for no reason. Currently it can not run user-mode programs. I don't even remember if it works. The only thing I remember it doing was triple-faulting. It attempts to initialize the CPU, paging, CPU tables and APIC.

Advice
------
Don't run the damn thing on bare metal ! Use qemu or something.


Dependencies
------------
I forgot....

Compiling
---------
mkdir out; cmake ..; make -j9
