#ifndef INITIATOR_H_
#define INITIATOR_H_
#include <systemc>
using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"

#include <cstdint>

class Initiator : public sc_module {
public:
  tlm_utils::simple_initiator_socket<Initiator> i_skt;
  tlm_utils::tlm_quantumkeeper m_qk; // Quantum keeper for temporal decoupling

  SC_HAS_PROCESS(Initiator);
  Initiator(sc_module_name n);
  //Initiator(sc_module_name n) : sc_module(n), i_skt("i_skt") {
  //  m_qk.set_global_quantum(sc_time(10, SC_NS)); // Setting the global quantum
  //  m_qk.reset(); // Resetting the quantum keeper
  //}

  int read_from_socket(unsigned long int addr, unsigned char mask[],
                       unsigned char rdata[], int dataLen);

  int write_to_socket(unsigned long int addr, unsigned char mask[],
                      unsigned char wdata[], int dataLen);

  void do_trans(tlm::tlm_generic_payload &trans);
  tlm::tlm_generic_payload trans; //prepare a TLM transaction data structure
  
};
#endif
