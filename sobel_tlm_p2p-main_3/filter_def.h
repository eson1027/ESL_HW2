#ifndef FILTER_DEF_H_
#define FILTER_DEF_H_

const int MASK_N = 1;
const int MASK_X = 5;
const int MASK_Y = 5;
const int BUFFER_SIZE = 25;

//we map each FIFO to a specific address in TLM transaction.
const int SOBEL_FILTER_R_ADDR = 0x00000000;
const int SOBEL_FILTER_RESULT_ADDR = 0x00000004;
const int SOBEL_FILTER_CHECK_ADDR = 0x00000008;


//允許在相同的記憶體位置存儲不同的數據類型
//通过 uc 數組，程序員可以直接讀取或修改存儲在 sint 或 uint 中的各個字節
union word {
  int sint;//4個byte
  unsigned int uint;//4個byte
  unsigned char uc[4];//分成四組，每一個佔1個byte
};
// sobel mask 1個5*5的矩陣作為filter
const int mask[MASK_N][MASK_X][MASK_Y] = {{{1, 4, 7, 4, 1}, 
                                        {4, 16, 26, 16, 4}, 
                                        {7, 26, 41, 26, 7}, 
                                        {4, 16, 26, 16, 4}, 
                                        {1, 4, 7, 4, 1}}};
#endif
