Code used for benchmarking of the two-state model simulated using Gillespie or the Poisson sampler (Psampler) siumlation method described in Lenive et al 2016 (Inferring extrinsic noise from single-cell gene expression data using approximate Bayesian computation).
Please note that this is intended as a benchmark only and is not a stand-alone tool in it's current version.
An example file containing model parameters is found in the data directory.  The path to this file is currently hard-coded.

To run the benchmark:
make
./benchmark.exe

Depends on a C++ compiler and libgsl for random number generation (see Makefile).

The model is hard coded in the Gillespie algorithm file and (unavoidably) in the Psampler.
Aims:
- Read nominal parameter vectors from rows in a text file.
- Implement a noise function (specific to the 2-state model), the same as was used in inference procedure described in the manuscript.
- Run simulations corresponding to a fixed number of samples of perturbed parameters obtained from the nominal parameters using the noise function.
- Optional output to files of perturbed parameter vectors and final states and simulation times.
- A benchmarking script that runs the above simulations.
- Run benchmark with output to files and compare to see if Gillespie and Psampler produce similar results.
- Run benchmark with minimal output to files so as not to conflate simulation time and I/O time.

