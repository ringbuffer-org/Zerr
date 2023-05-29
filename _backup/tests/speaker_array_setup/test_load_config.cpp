#include <iostream>
#include "speakerarray.h"

using namespace zerr;

/**    
* Unit test: Read Speaker array config file and initiate the SpeakerArray object
* Usage: ./run_test '../../Zerr/configs/speakerarray/eruption_8.yaml'
*/
void print_arguments(int argc, char const *argv[]){
    std::cout <<"Number of arguments: "<< argc << std::endl;
    for (int i = 0; i < argc; ++i){
        std::cout << i << ": "<<argv[i] << std::endl;
    }
}

void print_unit_test_info(){
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Start testing speaker array class" << std::endl;
    std::cout << "---------------------------------" << std::endl;
}

int main(int argc, char const *argv[])
{   
    print_unit_test_info();
    std::string config = argv[1];
    print_arguments(argc, argv);

    SpeakerArray testArray;

    testArray.initialize(config);
    std::cout<<"---------------------------"<<std::endl;
    std::cout<<testArray.get_n_speakers()<<" speakers in this setup."<<std::endl;
    std::cout<<"---------------------------"<<std::endl;
    Speaker s = testArray.get_speaker_by_index(2);
    s.print();
    std::cout<<"---------------------------"<<std::endl;
    std::vector<int> idxs = testArray.get_random_speakers(4,1);
    std::cout << "Random Speaker indexs:\n"; 
    for (int i : idxs)
    {
        std::cout << i << ' '; 
    }
    std::cout << "\n"; 

    int next_idx = testArray.get_next_one_speaker(2,0);
    std::cout<<"Current speaker index:"<<2<<std::endl; 
    std::cout<<"Next speaker index:   "<<next_idx<<std::endl; 

    return 0;
}