#include "jackclient.h"
using namespace zerr;


JackClient::initialize(){
    nInputs = 1;
    nOutputs = 12;
    nSpeakers = 12;

    this->client = jack_client_open("zerr", JackNullOption, &status, NULL);

    jack_set_process_callback(this->client, this->callback_process, this);
    input_port = new jack_port_t*[nInputs];
}

JackClient::run(){
    jack_activate(this->client);
}
