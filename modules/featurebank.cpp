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

centroid.input_channel = 1;
centroid.sample_rate = -1;
centroid.frame_size = -1;
centroid.step_length = -1;
/**
* Spectral Flatness
*/

/**
* Spectral Flux
*/

/**
* Zero Crossing
*/
centroid.name = "Zero Crossing";
centroid.processing_mode = "SAMPLE";
centroid.output_mode = "TRIGGER_BANG";

centroid.input_channel = 1;
centroid.sample_rate = -1;
centroid.frame_size = -1;
centroid.step_length = -1;

/**
* Inter-channel Level Difference
*/
centroid.name = "Inter-channel Level Difference";
centroid.processing_mode = "BLOCK";
centroid.output_mode = "FLOATING_POINTS";

centroid.input_channel = 2;
centroid.sample_rate = -1;
centroid.frame_size = -1;
centroid.step_length = -1;



