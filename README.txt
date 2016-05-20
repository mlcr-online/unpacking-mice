Two different versions of the unpacking library are needed in order to unpack the binary data collected during StepI and StepIV. This is due to the impossible backward compatibility of the data structure, defined in the versions of the DATE software, used in StepI and StepIV.

In order to build a library that can unpack the StepI data do:

 cd unpacking-mice/build
 cmake ..
 make


In order to build a library that can unpack the StepIV data do:

 cd unpacking-mice/build
 cmake  -DSTEPIV_DATA=1 ..
 make

