# Script reads final population file and writes only the parameters to an output file
using DataFrames

## INPUTS ##
inputFile = "Mar21_S2N_05x_00_04_04_04_04_nominal_Objective1_PopLast.txt"
outputFile = "Mar21_S2N_05x_00_04_04_04_04_nominal_Objective1_PopLast_14cols.txt"
############

## SCRIPT ##
colStart = 1;
colFinish = 14;
df = readtable(inputFile, header = false);
writetable(outputFile, df[:,1:14], separator = ',', header = false)
###########

## NOTES:

## Origin of data
# %load('AllMatchingNominals_Objective1_Pop000'); noise = 1;
# %load(['RawData_FISHsmcPostLast_S2N_05x_00_04_04_04_04/AllMatchingNominals_Objective'  num2str(BigX) '_Pop' POPULATION_num '.mat']); noise = 1;
# load(['RawData_FISHsmcPostLast_S2N_05x_00_04_04_04_04/AllMatchingNominals_Objective'  num2str(BigX) '_Pop' POPULATION_num '.mat'])



## Plotting scraps
# using PyPlot

# df = readtable("AllMatchingNominals_1.txt", header = false, separator = ' ')
# kr=df[1:3,1]
# typeof(kr)
# kf=df[1:3,2]
# typeof(kf)
# koff = kf./(2.0.*kr) #; koff[1]
# kon = koff.*kr ./ ( 1.0 - kr ) #; kon[1]

# scatter(kon, koff); axis([0, 1, 0, 1])

# k_r = df[1,1]
# typeof(k_r)
# k_f = df[1,2]
# typeof(k_f)
# k_off = k_f / (2.0 * k_r)
# k_on = k_off*k_r / (1.0 - k_r)



# kon=df[1];
# koff=df[2];

# kr = kon ./ (kon + koff)
# kf = 2.* kon .* koff ./ (kon + koff)
# scatter(kr, kf); axis([])


# h = PyPlot.plt.hist(df[1],100) # Histogram, PyPlot.plt required to differentiate with conflicting hist command


 # plt[:hist](df[1])

# EOF
