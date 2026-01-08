***************************************************************************
Readme file for the TotalDose command-line utility program, version: 1.0

Description: This program provides post-processing support for the calculation 
of 'total dose' values from existing Ae9/Ap9 model run output files.  

Copyright 2016 Atmospheric and Environmental Research, Inc. (AER)

DISTRIBUTION A. Approved for public release; distribution is unlimited.

Contact:
  author:  AER, Inc.
  address: 131 Hartwell Avenue
           Lexington, MA 02421
  phone:   781 761-2288
  email:   spwx@aer.com

Revision History:

Version    Date       Notes
1.0        5/25/2016  Created

**************************************************************************

This TotalDose utility is used for the calculation of the sum of the electron
and proton dose results from a pair of electron & proton model runs.  Normally,
this is just the sum of the respective dose values, but the aggregation 
percentile results are required to be recalculated from their associated (summed) 
scenario file results.
This utility program can be used to perform this necessary post-processing.  
Because of the separate (but coordinated) model runs required, no automation is 
possible.  This utility is used by the GUI when necessary.

The TotalDose utility will examine the input and output files for the 
specified model runs to determine if all requirements are satisfied before 
proceding with the total dose output file generation.

The requirements to be contained in the pair of model run input files:
* The electron and proton models must be in the same class:
   ie   AE9 & AP9, AE8 & AP8, CRRESELE & CRRESPRO
  -For the Legacy models, the Epoch/SAA Shift settings must be the same
  -the PLASMA (SPM) model is not valid, as no dose is calculated for this
* identical ephemeris input specifications
* identical flux modes (mean/percentile/perturbed/montecarlo) and numeric lists
* identical aggregation percentiles, if any
* All dose-related parameters must be identical:
   -Calculation of and Ascii output of doserate and/or doseaccum results
   -Shielding units and list of depth values
   -Shielding geometry
   -Detector target type
   -Nuclear Attenuation mode
* Any accumulation modes specified must be identical
* The optional "TimeSpec:" and "DataDelim:" values, if used, must be identical.

The TotalDose command-line arguments can be specified in two ways, 
depending on the naming convention used for the model run input and output 
files.  It is assumed that all model run input and output files are in the 
same directory.

The naming convention used by the GUI is :
<RunDir>/<RunName>.<Model>.input.txt
<RunDir>/<RunName>.<Model>.output.txt  <-- specified as the 'OutFile' value
where <RunDir> and <RunName> are the user's choice
      <Model> is one of 'AE8','AP8','CRRESELE','CRRESPRO','AE9' or 'AP9'
      
If this GUI naming convention is used, then this utility program may be invoked 
using the first form:
% TotalDose <RunDir> <RunName>

If some other name convention is used, then the utility program should be 
invoked using the second form:
% IntegralPlasma <RunDir> <ElectronInput> <ProtonInput>
where <...Input> are the names of the respective model run input files.  Both 
input file names are *required*, and in this specific order. 

The TotalDose program first verifies that the specified model run input 
files exist, then confirm that they contain all the required model parameters 
and settings.  The base names of the model run output files are collected from 
these input files.  Assuming the correct model parameters are present, the 
summation of the electron and protron dose results will be performed.  
This produces new output files, the names of which are formed by inserting the 
word 'Total' before 'doserate' and/or 'doseaccum', using the electron model 
OutFile prefix filename name base.

During the execution of the TotalDose program, informative messages are 
displayed to provide the status of the processing, and any errors encountered.

