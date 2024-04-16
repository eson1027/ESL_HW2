#include <cmath>
#include <iomanip>
#include "SobelFilter.h"

SobelFilter::SobelFilter(sc_module_name n)
    : sc_module(n), t_skt("t_skt"), base_offset(0) {
  SC_THREAD(do_filter);

  //The transport function blocking_transport() is registered to the target socket
  t_skt.register_b_transport(this, &SobelFilter::blocking_transport); //將b_trans註冊到t_skt上
}

// sobel mask 1個5*5的矩陣作為filter
const int mask[MASK_N][MASK_X][MASK_Y] = {{{1, 4, 7, 4, 1}, 
                                        {4, 16, 26, 16, 4}, 
                                        {7, 26, 41, 26, 7}, 
                                        {4, 16, 26, 16, 4}, 
                                        {1, 4, 7, 4, 1}}};

void SobelFilter::do_filter() {
  int filled_elements = 25; //check buffer是不是滿的
  while (true) {
    
    val[0] = 0;

    for(int i = BUFFER_SIZE-1 ; i > 0 ; i-- ){
      buffer_gray[i]=buffer_gray[i-1];
     }

    buffer_gray[0]=(i_r.read()+i_g.read()+i_b.read())/3;
    // cout << k <<endl;
    // k++;
    
    //更新已填充的元素數輛
    filled_elements = std::min(filled_elements + 1, BUFFER_SIZE);

    // cout << filled_elements << endl;

    // 判斷buffer是否滿了
    bool buffer_full = (filled_elements == BUFFER_SIZE);
    if(buffer_full){
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        for (unsigned int u = 0; u < MASK_X; ++u) {
           //unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;//將RGB的亮度加總做平均，這就顯示轉呈灰色要多黑
            //buffer_gray[0]=(i_r.read()+i_g.read()+i_b.read())/3;
            val[0] += buffer_gray[BUFFER_SIZE-(5*v+u)-1] * mask[0][u][v];
            // val[0] += grey * mask[0][u][v];
        }
      }
      // for(unsigned int g= 0; g<25; ++g){
      //     cout << "buffer_gray[" << g << "]" << buffer_gray[g] << endl;
      // }
      filled_elements = 20;
      // cout << "complete: " << l << endl;
      // l++;
      
      
      double total = 0;
        total += val[0] ;

      int result = (int)(total/273);
      o_result.write(result);
      //wait(10 * CLOCK_PERIOD, SC_NS); //May cause system to hang
    }
    else{
      //o_result.write(0);
      //wait(10);
    }
  }
}

//we assume the data length is always 4 bytes.


//由這個generic payload的資料結構來拿資料
//SobelFilter::blocking_transport(), which is registered to t_skt
void SobelFilter::blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
  sc_dt::uint64 addr = payload.get_address();
  addr = addr - base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case SOBEL_FILTER_RESULT_ADDR:
      buffer.uint = o_result.read();
      break;
    case SOBEL_FILTER_CHECK_ADDR:
      buffer.uint = o_result.num_available();
      break;
    default:
      std::cerr << "Error! SobelFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
      break;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
    break;

  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case SOBEL_FILTER_R_ADDR:
      if (mask_ptr[0] == 0xff) {
        i_r.write(data_ptr[0]);
      }
      if (mask_ptr[1] == 0xff) {
        i_g.write(data_ptr[1]);
      }
      if (mask_ptr[2] == 0xff) {
        i_b.write(data_ptr[2]);
      }
      break;
    default:
      std::cerr << "Error! SobelFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
      break;
    }
    break;

  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}
