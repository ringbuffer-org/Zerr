#include "featurebank.h"
// #include "type.h"

/**
* -1 means no specific requirement for this config
* and the client config will be used in loading the algorithm
* an error will raise if conflict exists
*/

/**
* Spectral Centroid
*/
centroid.name = "Spectral Centroid";
centroid.processing_mode = "FRAME";
centroid.output_mode = "FLOATING_POINTS";

centroid.sample_rate = -1; 
centroid.input_channel = 1;
centroid.input_size = -1; 
centroid.output_channel = 1;
centroid.output_size = 1;
/**
* Spectral Flatness
*/

/**
* Spectral Flux
*/

/**
* Zero Crossing
*/
zero_crossing.name = "Zero Crossing";
zero_crossing.processing_mode = "SAMPLE";
zero_crossing.output_mode = "TRIGGER_BANG";

zero_crossing.sample_rate = -1; 
zero_crossing.input_channel = 1;
zero_crossing.input_size = -1; 
zero_crossing.output_channel = 1;
zero_crossing.output_size = 1;


/**
* Inter-channel Level Difference
*/
centroid.name = "Inter-channel Level Difference";
centroid.processing_mode = "BLOCK";
centroid.output_mode = "FLOATING_POINTS";

centroid.sample_rate = -1; 
centroid.input_channel = 1;
centroid.input_size = -1; 
centroid.output_channel = 1;
centroid.output_size = 1;



