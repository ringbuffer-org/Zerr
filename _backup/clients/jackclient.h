#ifndef JACKCLIENT_H
#define JACKCLIENT_H
#include<jack/jack.h>

namespace zerr{

class JackClient{
public:
    initialize();
    run();
private:
    /// \brief client
    /// the jack client, obviously
    jack_client_t   *client;

    /// \brief status
    /// gets the status from the jack server
    jack_status_t   status;

    /// \brief input_port
    /// the jack input ports
    jack_port_t     **input_port;

    /// \brief output_port
    /// the jack output ports
    jack_port_t     **output_port;

    /// \brief in
    ///
    /// \brief out
    jack_default_audio_sample_t **in, **out;

};

} //namespace zerr
#endif //JACKCLIENT_H