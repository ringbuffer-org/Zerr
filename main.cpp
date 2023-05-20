#include "zerr.h"

using namespace zerr;
// spkrCfgFile "/Users/yangzeyu/Downloads/Zerr/configs/spkr_configs/circulation_8.yaml"
// zerrCfgFile "/Users/yangzeyu/Downloads/Zerr/configs/zerr_configs/preset1.yaml"
// ./run_zerr -z "/Users/yangzeyu/Downloads/Zerr/configs/zerr_configs/preset1.yaml" -s "/Users/yangzeyu/Downloads/Zerr/configs/spkr_configs/circulation_8.yaml"

int main(int argc, char *argv[]){
  std::string zerrCfgFile;
  std::string spkrCfgFile;

  // Display each command-line argument.
  std::cout << "\nCommand-line arguments:\n";
  for(int i = 1; i < (argc-1); i++ ){
    if( std::string(argv[i]) == "-z" ){
      std::cout << "zerr configs: " << " " <<argv[i+1] << std::endl;
      zerrCfgFile = argv[i+1];
    }
    if( std::string(argv[i]) == "-s" ){
      std::cout << "spkr configs: " << " " <<argv[i+1] << std::endl;
      spkrCfgFile = argv[i+1];
    }
  }

  Zerr zerr_client(zerrCfgFile, spkrCfgFile);
  zerr_client.initialize();
  zerr_client.run();

  return 0;
}
