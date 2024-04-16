#ifndef SOBEL_FILTER_H_
#define SOBEL_FILTER_H_
#include <systemc>
using namespace sc_core;

#include "tlm"
#include "tlm_utils/simple_target_socket.h"
//多了tlm socket宣告

#include "filter_def.h"

//we add a TLM target socket to be bound to initiator socket of Testbench.


// we register the transport functions 
// to the blocking transport interface of the target socket
class SobelFilter : public sc_module {
public:
  tlm_utils::simple_target_socket<SobelFilter> t_skt;
  //多了t_socket

  sc_fifo<unsigned char> i_r;//channel
  sc_fifo<unsigned char> i_g;//channel
  sc_fifo<unsigned char> i_b;//channel
  sc_fifo<int> o_result;//channel

  //使用對應的 FIFO 來控制 SystemC 執行緒 do_filter()。

  SC_HAS_PROCESS(SobelFilter);
  SobelFilter(sc_module_name n);
  ~SobelFilter() = default;

private:
  void do_filter();
  int val[MASK_N];
  unsigned char buffer_gray[BUFFER_SIZE];

  unsigned int base_offset;
  void blocking_transport(tlm::tlm_generic_payload &payload,
                          sc_core::sc_time &delay);
};
#endif
