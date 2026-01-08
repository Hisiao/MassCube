***************************************************************************
Readme file for the IntegralPlasma command-line utility program, version: 2.0

Description: This program provides post-processing support for the calculation 
of 'integral flux' values from coordinated Plasma and Ae9/Ap9 model runs.  

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
1.0        7/05/2012  Created
2.0        5/25/2016  Minor updates to description for rewritten utility

**************************************************************************

This IntegralPlasma utility is used for the conversion of the output file data 
for Ae9Ap9 'integral flux' calculations, but only for the 'plasma' energy level 
results.  
Due to the limitations of the Plasma model, electron and/or proton full 
integral flux values cannot be determined independently, and so requires 
integral flux values from the respective Ae9 and/or Ap9 model results (at 
specific energy levels) to convert the plasma results.
This utility program can be used to perform this necessary post-processing.  
Because of the separate (but coordinated) model runs required, no automation 
is possible.  This utility is used by the GUI when necessary.

The IntegralPlasma utility will examine the input and output files for the 
specified model runs to determine if all requirements are satisfied before 
proceding with the non-reversible plasma output file modifications.

Specific keyword requirements to be contained in each model run input file: 

 keyword    Plasma Electrons                   AE9
ModelType:    Plasma                            AE9
ModelDB:      .../SPMEV<xx>_runtime_tables.mat  .../AE9V<xx>_runtime_tables.mat
FluxType:     2PtDiff                           Integral
Energies:     [whatever needed, all<0.04]       0.04,[whatever needed, all>0.04]
Energies2:    0.04,[all =0.04]                  --n/a---
OrbitFile:    [whatever needed]                 [must be identical orbit specs]

 keyword    Plasma Protons                     AP9
ModelType:    Plasma                            AP9
ModelDB:      .../SPMHV<xx>_runtime_tables.mat  .../AP9V<xx>_runtime_tables.mat
FluxType:     2PtDiff                           Integral
Energies:     [whatever needed, all<0.1]        0.1,[whatever needed, all>0.1]
Energies2:    0.1,[all =0.1]                    --n/a---
OrbitFile:    [whatever needed]                 [must be identical orbit specs]

The optional "TimeSpec:" and "DataDelim:" values, if used, must be identical
between all model run input files.

The IntegralPlasma command-line arguments can be specified in two ways, 
depending on the naming convention used for the model run input and output 
files.  It is assumed that all model run input and output files are in the 
same directory, and that the identical ephemeris information is used.

The naming convention used by the GUI is :
<RunDir>/<RunName>.<Model>.input.txt
<RunDir>/<RunName>.<Model>.output.txt  <-- specified as the 'OutFile' value
where <RunDir> and <RunName> are the user's choice
      <Model> is one of 'PLASMAelec','PLASMAprot', 'AE9' or 'AP9' in this case
      
If this GUI naming convention is used, then this utility program may be invoked 
using the first form:
% IntegralPlasma <RunDir> <RunName>

If some other name convention is used, then the utility program should be 
invoked using the second form:
% IntegralPlasma <RunDir> <PlasmaElectronInput> <AE9Input> <PlasmaProtonInput> <AP9Input>
where <...Input> are the names of the respective model run input files.  All 
four input file names are *required*, and in this specific order.  The word 
'none' can be specified as a placeholder file name to ignore, if needed.

The IntegralPlasma program first verifies that the specified model run input 
files exist, then confirm that they contain all the required model parameters 
and settings.  The base names of the model run output files are collected from 
these input files.  Assuming the correct model parameters are present, the 
conversion of both electron and protron plasma output files will be performed.  
This rewrites the information in the plasma output files, and cannot be undone.
To return to the original files, the only option is to re-run the plasma model.
Attempts to perform the conversion on files that have already been converted 
will be identified, and skipped.

During the execution of the IntegralPlasma program, informative messages are 
displayed to provide the status of the processing, and any errors encountered.

