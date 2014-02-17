#include <lacewing.h>
#include "Server.h"
#include "Logger.h"
#include "fmitcp.pb.h"

using namespace std;
using namespace fmitcp;

void Server::onClientConnect(){}
void Server::onClientDisconnect(){}
void Server::onError(string message){}

void serverOnConnect(lw_server s, lw_client c) {
    //printf("serverOnConnect\n");fflush(NULL);

    // For some reason the server needs to send the first message
    lw_stream_write(c, "\n", 1);

    Server * server = (Server*)lw_server_tag(s);
    server->clientConnected(c);
}
void serverOnData(lw_server s, lw_client client, const char* data, size_t size) {
    //printf("serverOnData\n");fflush(NULL);
    Server * server = (Server*)lw_server_tag(s);
    server->clientData(client,data,size);
}
void serverOnDisconnect(lw_server s, lw_client c) {
    //printf("serverOnDisconnect\n");fflush(NULL);
    Server * server = (Server*)lw_server_tag(s);
    server->clientDisconnected(c);
}
void serverOnError(lw_server s, lw_error error) {
    //printf("serverOnError\n");fflush(NULL);
    Server * server = (Server*)lw_server_tag(s);
    server->error(s,error);
}

void Server::clientConnected(lw_client c){
    m_logger.log(Logger::NETWORK,"+ Client connected.\n");
    onClientConnect();
    fflush(NULL);
}
void Server::clientDisconnected(lw_client c){
    m_logger.log(Logger::NETWORK,"- Client disconnected.\n");
    onClientDisconnect();
}
void Server::clientData(lw_client c, const char* data, size_t size){
    //printf("Got client data: %s\n", data);fflush(NULL);

    if(strcmp(data,"\n") == 0)
        return;

    // Construct message
    fmitcp_proto::fmitcp_message req;
    req.ParseFromString(data);
    fmitcp_proto::fmitcp_message_Type type = req.type();

    fmitcp_proto::fmitcp_message res;
    bool sendResponse = true;

    if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_slave_req){

        // Unpack message
        fmitcp_proto::fmi2_import_instantiate_slave_req * instantiateReq = req.mutable_fmi2_import_instantiate_slave_req();
        int fmuId = instantiateReq->fmuid();
        int messageId = instantiateReq->message_id();
        string instanceName = instantiateReq->instancename();
        string fmuResourceLocation = instantiateReq->fmuresourcelocation();
        bool visible = instantiateReq->visible();

        m_logger.log(Logger::NETWORK,"< fmi2_import_instantiate_slave_req(fmuId=%d,instanceName=%s,resourceLoc=%s,visible=%d)\n",fmuId,instanceName.c_str(),fmuResourceLocation.c_str(),visible);

        // instantiate FMU
        // TODO

        // Create response
        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_slave_res);
        fmitcp_proto::jm_status_enu_t status = fmitcp_proto::jm_status_success;
        fmitcp_proto::fmi2_import_instantiate_slave_res * instantiateRes = res.mutable_fmi2_import_instantiate_slave_res();
        instantiateRes->set_message_id(1);
        instantiateRes->set_status(status);
        m_logger.log(Logger::NETWORK,"> fmi2_import_instantiate_slave_res(mid=%d,status=%d)\n",1,status);

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_initialize_slave_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_terminate_slave_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_reset_slave_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_slave_instance_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_real_input_derivatives_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_output_derivatives_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_cancel_step_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_req){

        // Unpack message
        fmitcp_proto::fmi2_import_do_step_req * doStepReq = req.mutable_fmi2_import_do_step_req();
        int fmuId = doStepReq->fmuid();
        double currentCommunicationPoint = doStepReq->currentcommunicationpoint(),
               communicationStepSize = doStepReq->communicationstepsize();
        bool newStep = doStepReq->newstep();
        m_logger.log(Logger::NETWORK,"< fmi2_import_do_step_req(fmuId=%d,commPoint=%g,stepSize=%g,newStep=%d)\n",fmuId,currentCommunicationPoint,communicationStepSize,newStep?1:0);

        // Step the FMU
        // TODO

        //printf("Setting type=%d\n", fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_res);fflush(NULL);

        // Create response
        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_res);
        fmitcp_proto::fmi2_status_t status = fmitcp_proto::fmi2_status_ok;
        fmitcp_proto::fmi2_import_do_step_res * doStepRes = res.mutable_fmi2_import_do_step_res();
        doStepRes->set_message_id(0);
        doStepRes->set_status(status);
        m_logger.log(Logger::NETWORK,"> fmi2_import_do_step_res(status=%d)\n",status);

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_status_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_status_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_integer_status_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_boolean_status_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_string_status_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_model_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_model_instance_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_time_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_continuous_states_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_completed_integrator_step_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_initialize_model_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_derivatives_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_event_indicators_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_eventUpdate_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_completed_event_iteration_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_continuous_states_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_nominal_continuous_states_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_terminate_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_version_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_debug_logging_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_real_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_integer_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_boolean_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_string_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_integer_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_boolean_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_string_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_fmu_state_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_fmu_state_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_fmu_state_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_serialized_fmu_state_size_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_serialize_fmu_state_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_de_serialize_fmu_state_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_directional_derivative_req){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_get_xml_req){
    } else {
        // Something is wrong.
        sendResponse = false;
        m_logger.log(Logger::ERROR,"Message type not recognized: %d.\n",type);
    }

    if(sendResponse){
        string response;
        res.SerializeToString(&response);
        fflush(NULL);
        lw_stream_write(c, response.c_str(), response.size());
        lw_stream_write(c, "\n", 1);
        fflush(NULL);
    }
}
void Server::error(lw_server s, lw_error error){
    string err = lw_error_tostring(error);
    onError(err);
}

Server::Server(EventPump * pump){
    init(pump);
}
Server::Server(EventPump * pump, const Logger& logger){
    m_logger = logger;
    init(pump);
}
void Server::init(EventPump * pump){
    m_pump = pump;
    m_server = lw_server_new(pump->getPump());
}

Server::~Server(){
    lw_server_delete(m_server);
}

void Server::addFMU(string path){

}

void Server::host(string hostName, long port){

    // save this object in the server tag so we can use it later on.
    lw_server_set_tag(m_server, (void*)this);

    // connect the hooks
    lw_server_on_connect(   m_server, serverOnConnect);
    lw_server_on_data(      m_server, serverOnData);
    lw_server_on_disconnect(m_server, serverOnDisconnect);
    lw_server_on_error(     m_server, serverOnError);

    // setup the server host name and port
    lw_addr host = lw_addr_new_port(hostName.c_str(), port);
    lw_filter filter = lw_filter_new();
    lw_filter_set_ipv6(filter, lw_false);
    lw_filter_set_local(filter, host);

    // host/start the server
    lw_server_host_filter(m_server, filter);
    lw_filter_delete(filter);

    m_logger.log(Logger::NETWORK,"Listening to %s:%ld\n",hostName.c_str(),port);

    fflush(NULL);
}

Logger Server::getLogger() const {
    return m_logger;
}
