# Read file produced by benchmarking.cpp
# Expected file format is rows of doubles separated by ", " without a header.
# Columns are nominal parameter and noise parameters (1-14),
# mean mRNA, mean protein, variance mRNA, variance protein from Psampler (15-18)
# mean mRNA, mean protein, variance mRNA, variance protein from Gillespie (19-22)
# Simulation duration in seconds from Psampler (23) and Gillespie (24)

## INPUTS ##
# Text file produced by benchmarking.cpp
FILE_benchmarking = "output_1.txt"

############

## Packages
using DataFrames

### FUNCTIONS ###
function  summarise_ratios(ratios)
  println("Smalles ratio: ", minimum(ratios))
  println("Largest ratio: ", maximum(ratios))
  println("Mean ratios: ", mean(ratios))
  println("Variance ratios: ", var(ratios))
end
#################

#### SCRIPT ####

# Read benchmarking file into data frame
benchmark = readtable(FILE_benchmarking, header = false);

total_seconds_Psamper = sum(benchmark[:,23]);
println("Total simulation time using Psampler: ", total_seconds_Psamper, " seconds")
total_seconds_Gillespie = sum(benchmark[:,24])
println("Total simulation time using Gillespie: ", total_seconds_Gillespie, " seconds")
# println("Time ratio: ", (total_seconds_Gillespie/total_seconds_Psamper) )

println("Ratios of simulation times.")
summarise_ratios( benchmark[:,24]./benchmark[:,23] ); # Vector of ratios


println("Ratios of mRNA means.")
idx = 15;
summarise_ratios( benchmark[:,idx+4] ./ benchmark[:,idx])

println("Ratios of protein means.")
idx = 16;
summarise_ratios( benchmark[:,idx+4] ./ benchmark[:,idx])

println("Ratios of mRNA variances.")
idx = 17;
summarise_ratios( benchmark[:,idx+4] ./ benchmark[:,idx])

println("Ratios of protein variances.")
idx = 18;
summarise_ratios( benchmark[:,idx+4] ./ benchmark[:,idx])


################

# EOF
