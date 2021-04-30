# ORCA RT-Bench
![C++ Build](https://github.com/andersondomingues/orca-rt-bench/actions/workflows/c-cpp.yml/badge.svg)![Java Build](https://github.com/andersondomingues//orca-rt-bench/actions/workflows/main.yml/badge.svg) ![Code Factor](https://www.codefactor.io/repository/github/andersondomingues/orca-rt-bench)

ORCA RT-Bench is (or should be) a workbench for modeling and simulating real-time systems. The project is at its initial steps, and is likely to change a lot in the next weeks. No recommended for production-ready stuff.

### Scheduling Algorithms

Rate Monotonic (RM) scheduling: fixed-priority policy where the higher the frequency (1/period) of a task, the higher is its priority.
Earliest DeadLine First (EDF) scheduling: fixed-priority policy where the earlier the deadline of a task, the higher is its priority.

## Licensing

This is free software! See ``LICENSE.MD`` for details. 

## Contact

Feel free to contact me ([andersondomingues](https://github.com/andersondomingues)), the maintainer of this project: mailto:ti.andersondomingues@gmail.com.

## Third-Party Work and Acknowledgement 

The Java application "orca-rt-bench: Kprofiller" uses the "Kprofiller Tool", developed by Professor Sergio Johann. More information on his work can be found at [his repository](https://github.com/sjohann81).
