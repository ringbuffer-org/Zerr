#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include "Zerr.h"
#include "FeatureBank.h"
/*
Virtual Class for all feature algoirthms
Types of feature are stored in featurelib.h
*/
class FeatureExtractor
{
public:

    FeatureExtractor(int feature_index);

    int get_feature_index(); 

    int get_input_channel(return input_channel);
    int get_feature_level(return feature_level);
    int get_sample_rate(return sample_rate);
    int get_frame_size(return frame_size);

    int extract(); //start monitoring the incoming signal and do feature extraction



private:

    int feature_index = 42; 

    int input_channel = 1;
    int feature_level = 1; // 1=sample-level, 2=frame-level
    int sample_rate = 48000; // 
    int frame_size = 1024; // only useful in the level-2 feature mode.

    // temp params to store needed 
    // todo: reconsider this again
    double temp = 0.0;
    std::vector <double> temp = 0.0; 

    virtual float extract_sample(double input) = 0;
    virtual float extract_frame(std::vector <double> input) = 0;







};

#endif // FEATUREEXTRACTOR_H



    // float centroid(std::vector <double> x);
    // float flatness(std::vector <double> x);

    // float spectral_flux(std::vector <double> power_spectrum);

    // int zero_crossing(double s);