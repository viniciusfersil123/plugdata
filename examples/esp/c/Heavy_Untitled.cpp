/**
 * Copyright (c) 2026 Enzien Audio, Ltd.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the phrase "powered by heavy",
 *    the heavy logo, and a hyperlink to https://enzienaudio.com, all in a visible
 *    form.
 * 
 *   2.1 If the Application is distributed in a store system (for example,
 *       the Apple "App Store" or "Google Play"), the phrase "powered by heavy"
 *       shall be included in the app description or the copyright text as well as
 *       the in the app itself. The heavy logo will shall be visible in the app
 *       itself as well.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include "Heavy_Untitled.hpp"

#include <new>

#define Context(_c) static_cast<Heavy_Untitled *>(_c)


/*
 * C Functions
 */

extern "C" {
  HV_EXPORT HeavyContextInterface *hv_Untitled_new(double sampleRate) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_Untitled));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_Untitled(sampleRate);
    return Context(ptr);
  }

  HV_EXPORT HeavyContextInterface *hv_Untitled_new_with_options(double sampleRate,
      int poolKb, int inQueueKb, int outQueueKb) {
    // allocate aligned memory
    void *ptr = hv_malloc(sizeof(Heavy_Untitled));
    // ensure non-null
    if (!ptr) return nullptr;
    // call constructor
    new(ptr) Heavy_Untitled(sampleRate, poolKb, inQueueKb, outQueueKb);
    return Context(ptr);
  }

  HV_EXPORT void hv_Untitled_free(HeavyContextInterface *instance) {
    // call destructor
    Context(instance)->~Heavy_Untitled();
    // free memory
    hv_free(instance);
  }
} // extern "C"







/*
 * Class Functions
 */

Heavy_Untitled::Heavy_Untitled(double sampleRate, int poolKb, int inQueueKb, int outQueueKb)
    : HeavyContext(sampleRate, poolKb, inQueueKb, outQueueKb) {
  numBytes += sPhasor_k_init(&sPhasor_IzN1USIZ, 220.0f, sampleRate);
  numBytes += sTabread_init(&sTabread_Pbav1iaY, &hTable_sRVyTwlZ, true);
  numBytes += sTabwrite_init(&sTabwrite_ky5r2TMU, &hTable_sRVyTwlZ);
  numBytes += sLine_init(&sLine_6etJVKFb);
  numBytes += cDelay_init(this, &cDelay_XnzUr0Oh, 0.0f);
  numBytes += cDelay_init(this, &cDelay_313vDnWc, 0.0f);
  numBytes += hTable_init(&hTable_sRVyTwlZ, 256);
  numBytes += cVar_init_f(&cVar_PmbysH6A, 0.0f);
  numBytes += cBinop_init(&cBinop_8yBxqxnF, 1.0f); // __pow
  numBytes += cDelay_init(this, &cDelay_gBtFQ7Kq, 0.0f);
  numBytes += cVar_init_f(&cVar_8NfhCLlk, 20.0f);
  numBytes += cBinop_init(&cBinop_4OpOCCBO, 0.0f); // __mul
  numBytes += cVar_init_f(&cVar_WwKmJumd, 0.0f);
  numBytes += cSlice_init(&cSlice_fycj9DH0, 1, -1);
  numBytes += cSlice_init(&cSlice_OFonwLJL, 1, -1);
  numBytes += cVar_init_f(&cVar_kw4Y3zR6, 0.0f);
  numBytes += cVar_init_f(&cVar_xEescJU2, 20.0f);
  numBytes += cVar_init_f(&cVar_TC9InOYO, 0.0f);
  numBytes += cVar_init_f(&cVar_zc6Eyrov, 0.0f);
  numBytes += cVar_init_f(&cVar_DI8ZKZ2H, 0.0f);
  numBytes += cSlice_init(&cSlice_jW9YO008, 1, 1);
  numBytes += cSlice_init(&cSlice_Fu4gydLB, 0, 1);
  numBytes += cBinop_init(&cBinop_Z7rNXf3p, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_2zHrLwnR, 0.0f); // __mul
  numBytes += cBinop_init(&cBinop_s1mEvnme, 0.0f); // __sub
  numBytes += cBinop_init(&cBinop_YRjUkg0h, 0.0f); // __add
  numBytes += cBinop_init(&cBinop_sT7rlI8p, 20.0f); // __div
  numBytes += cBinop_init(&cBinop_a20bM1PO, 0.0f); // __div
  numBytes += cBinop_init(&cBinop_VqkdPSuP, 0.0f); // __add
  numBytes += cBinop_init(&cBinop_YN8wyJ5m, 0.0f); // __sub
  numBytes += cRandom_init(&cRandom_S6JljoU8, 248587082);
  numBytes += cSlice_init(&cSlice_PL8crhqS, 1, 1);
  numBytes += cDelay_init(this, &cDelay_U8Xu12Pw, 0.0f);
  numBytes += cVar_init_f(&cVar_gyHTRgwW, 500.0f);
  numBytes += cBinop_init(&cBinop_AgEFsytW, 0.0f); // __mul
  numBytes += cTabhead_init(&cTabhead_r2G6CGXf, &hTable_sRVyTwlZ);
  numBytes += cVar_init_s(&cVar_JcyjQ9Xn, "del-teste");
  numBytes += cDelay_init(this, &cDelay_JjWXGYms, 300.0f);
  numBytes += cDelay_init(this, &cDelay_Wm2Q3dMb, 0.0f);
  numBytes += cBinop_init(&cBinop_G1svjTPE, 300.0f); // __mul
  numBytes += cBinop_init(&cBinop_NDnpkYMa, 0.0f); // __sub
  numBytes += cBinop_init(&cBinop_FOGYJ5Um, 0.0f); // __max
  numBytes += cBinop_init(&cBinop_xwAdx1wT, 0.0f); // __sub
  
  // schedule a message to trigger all loadbangs via the __hv_init receiver
  scheduleMessageForReceiver(0xCE5CC65B, msg_initWithBang(HV_MESSAGE_ON_STACK(1), 0));
}

Heavy_Untitled::~Heavy_Untitled() {
  hTable_free(&hTable_sRVyTwlZ);
}

HvTable *Heavy_Untitled::getTableForHash(hv_uint32_t tableHash) {switch (tableHash) {
    case 0xB0952E2D: return &hTable_sRVyTwlZ; // del-teste
    default: return nullptr;
  }
}

void Heavy_Untitled::scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) {
  switch (receiverHash) {
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_OEYXkWDM_sendMessage);
      break;
    }
    default: return;
  }
}

int Heavy_Untitled::getParameterInfo(int index, HvParameterInfo *info) {
  if (info != nullptr) {
    switch (index) {
      default: {
        info->name = "invalid parameter index";
        info->hash = 0;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 0.0f;
        info->defaultVal = 0.0f;
        break;
      }
    }
  }
  return 0;
}



/*
 * Send Function Implementations
 */


void Heavy_Untitled::cMsg_5UN3EJZp_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_3aftrZmR_sendMessage);
}

void Heavy_Untitled::cSystem_3aftrZmR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_zpnszzis_sendMessage);
}

void Heavy_Untitled::cDelay_XnzUr0Oh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_XnzUr0Oh, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_313vDnWc, 0, m, &cDelay_313vDnWc_sendMessage);
  cDelay_onMessage(_c, &Context(_c)->cDelay_XnzUr0Oh, 0, m, &cDelay_XnzUr0Oh_sendMessage);
  sTabwrite_onMessage(_c, &Context(_c)->sTabwrite_ky5r2TMU, 1, m, NULL);
}

void Heavy_Untitled::cDelay_313vDnWc_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_313vDnWc, m);
  cMsg_L3VukneU_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cSwitchcase_TT61NSIN_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x47BE8354: { // "clear"
      cMsg_72P9NvKW_sendMessage(_c, 0, m);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_Untitled::cBinop_66UkTQBl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_Ls6Tj94R_sendMessage(_c, 0, m);
}

void Heavy_Untitled::hTable_sRVyTwlZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_wbBzFrwb_sendMessage(_c, 0, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_XnzUr0Oh, 2, m, &cDelay_XnzUr0Oh_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_fxJfjZgm_sendMessage);
}

void Heavy_Untitled::cMsg_Ls6Tj94R_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "resize");
  msg_setElementToFrom(m, 1, n, 0);
  hTable_onMessage(_c, &Context(_c)->hTable_sRVyTwlZ, 0, m, &hTable_sRVyTwlZ_sendMessage);
}

void Heavy_Untitled::cBinop_zpnszzis_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 500.0f, 0, m, &cBinop_66UkTQBl_sendMessage);
}

void Heavy_Untitled::cMsg_L3VukneU_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "mirror");
  hTable_onMessage(_c, &Context(_c)->hTable_sRVyTwlZ, 0, m, &hTable_sRVyTwlZ_sendMessage);
}

void Heavy_Untitled::cCast_fxJfjZgm_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_XnzUr0Oh, 0, m, &cDelay_XnzUr0Oh_sendMessage);
}

void Heavy_Untitled::cMsg_wbBzFrwb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0,  static_cast<float>(HV_N_SIMD));
  cDelay_onMessage(_c, &Context(_c)->cDelay_313vDnWc, 2, m, &cDelay_313vDnWc_sendMessage);
}

void Heavy_Untitled::cMsg_72P9NvKW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  sTabwrite_onMessage(_c, &Context(_c)->sTabwrite_ky5r2TMU, 1, m, NULL);
}

void Heavy_Untitled::cVar_PmbysH6A_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_SUBTRACT, 69.0f, 0, m, &cBinop_fDs9Rh4H_sendMessage);
}

void Heavy_Untitled::cBinop_0ssPsyii_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_IzN1USIZ, 0, m);
}

void Heavy_Untitled::cBinop_fDs9Rh4H_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 12.0f, 0, m, &cBinop_Wcw5EJiI_sendMessage);
}

void Heavy_Untitled::cBinop_Wcw5EJiI_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_8yBxqxnF, HV_BINOP_POW, 1, m, &cBinop_8yBxqxnF_sendMessage);
  cMsg_6PjPr9Zo_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cBinop_8yBxqxnF_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 440.0f, 0, m, &cBinop_0ssPsyii_sendMessage);
}

void Heavy_Untitled::cMsg_6PjPr9Zo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 2.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_8yBxqxnF, HV_BINOP_POW, 0, m, &cBinop_8yBxqxnF_sendMessage);
}

void Heavy_Untitled::cCast_sr7QTgrD_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_PN2HfNAx_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_Untitled::cMsg_GpAOfsSE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_3BQvV8Mo_sendMessage);
}

void Heavy_Untitled::cSystem_3BQvV8Mo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_2zHrLwnR, HV_BINOP_MULTIPLY, 1, m, &cBinop_2zHrLwnR_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_Z7rNXf3p, HV_BINOP_MULTIPLY, 1, m, &cBinop_Z7rNXf3p_sendMessage);
}

void Heavy_Untitled::cSwitchcase_CpqTe71j_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cMsg_Amu2hPzA_sendMessage(_c, 0, m);
      break;
    }
    case 0x7A5B032D: { // "stop"
      cMsg_Amu2hPzA_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_vXAv9D3G_sendMessage);
      break;
    }
  }
}

void Heavy_Untitled::cDelay_gBtFQ7Kq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_gBtFQ7Kq, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_gBtFQ7Kq, 0, m, &cDelay_gBtFQ7Kq_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_WwKmJumd, 0, m, &cVar_WwKmJumd_sendMessage);
}

void Heavy_Untitled::cCast_vXAv9D3G_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_Amu2hPzA_sendMessage(_c, 0, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_gBtFQ7Kq, 0, m, &cDelay_gBtFQ7Kq_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_WwKmJumd, 0, m, &cVar_WwKmJumd_sendMessage);
}

void Heavy_Untitled::cMsg_szg5ZRIq_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_52afweua_sendMessage);
}

void Heavy_Untitled::cSystem_52afweua_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_Xc5thTWo_sendMessage);
}

void Heavy_Untitled::cVar_8NfhCLlk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_4OpOCCBO, HV_BINOP_MULTIPLY, 0, m, &cBinop_4OpOCCBO_sendMessage);
}

void Heavy_Untitled::cMsg_Amu2hPzA_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_gBtFQ7Kq, 0, m, &cDelay_gBtFQ7Kq_sendMessage);
}

void Heavy_Untitled::cBinop_2EuBfoS3_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_gBtFQ7Kq, 2, m, &cDelay_gBtFQ7Kq_sendMessage);
}

void Heavy_Untitled::cBinop_Xc5thTWo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_4OpOCCBO, HV_BINOP_MULTIPLY, 1, m, &cBinop_4OpOCCBO_sendMessage);
}

void Heavy_Untitled::cBinop_4OpOCCBO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_2EuBfoS3_sendMessage);
}

void Heavy_Untitled::cVar_WwKmJumd_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_s1mEvnme, HV_BINOP_SUBTRACT, 0, m, &cBinop_s1mEvnme_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_LESS_THAN_EQL, 0.0f, 0, m, &cBinop_MT3iWDDa_sendMessage);
}

void Heavy_Untitled::cSwitchcase_ule5EMYV_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_AlgoR2A1_sendMessage);
      break;
    }
    case 0x3F800000: { // "1.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_dnnbkcGR_sendMessage);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_Untitled::cCast_AlgoR2A1_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_DI8ZKZ2H, 0, m, &cVar_DI8ZKZ2H_sendMessage);
}

void Heavy_Untitled::cCast_dnnbkcGR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_iqZZMLRj_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_4mWSEeRx_sendMessage);
}

void Heavy_Untitled::cSwitchcase_5UjoJQef_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7A5B032D: { // "stop"
      cSlice_onMessage(_c, &Context(_c)->cSlice_fycj9DH0, 0, m, &cSlice_fycj9DH0_sendMessage);
      break;
    }
    case 0x3E004DAB: { // "set"
      cSlice_onMessage(_c, &Context(_c)->cSlice_OFonwLJL, 0, m, &cSlice_OFonwLJL_sendMessage);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_r7fP0KS6_sendMessage);
      cSlice_onMessage(_c, &Context(_c)->cSlice_jW9YO008, 0, m, &cSlice_jW9YO008_sendMessage);
      cSlice_onMessage(_c, &Context(_c)->cSlice_Fu4gydLB, 0, m, &cSlice_Fu4gydLB_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_JezzTHMu_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_Mk6vwJs6_sendMessage);
      break;
    }
  }
}

void Heavy_Untitled::cSlice_fycj9DH0_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cMsg_9OweGO3x_sendMessage(_c, 0, m);
      break;
    }
    case 1: {
      cMsg_9OweGO3x_sendMessage(_c, 0, m);
      break;
    }
    default: return;
  }
}

void Heavy_Untitled::cSlice_OFonwLJL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_sIaX48hn_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_NNMGMx06_sendMessage);
      break;
    }
    case 1: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_sIaX48hn_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_NNMGMx06_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_Untitled::cVar_kw4Y3zR6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_hwC0Dj0L_sendMessage);
  cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_aBAP9c9v_sendMessage);
}

void Heavy_Untitled::cVar_xEescJU2_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_onIjYgKN_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_Untitled::cSwitchcase_onIjYgKN_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_mJTfm0HZ_sendMessage);
      break;
    }
    default: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_2zHrLwnR, HV_BINOP_MULTIPLY, 0, m, &cBinop_2zHrLwnR_sendMessage);
      cBinop_onMessage(_c, &Context(_c)->cBinop_sT7rlI8p, HV_BINOP_DIVIDE, 1, m, &cBinop_sT7rlI8p_sendMessage);
      cVar_onMessage(_c, &Context(_c)->cVar_8NfhCLlk, 0, m, &cVar_8NfhCLlk_sendMessage);
      break;
    }
  }
}

void Heavy_Untitled::cCast_mJTfm0HZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_AbcwLLoj_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cVar_TC9InOYO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_YN8wyJ5m, HV_BINOP_SUBTRACT, 1, m, &cBinop_YN8wyJ5m_sendMessage);
}

void Heavy_Untitled::cVar_zc6Eyrov_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_DI8ZKZ2H, 0, m, &cVar_DI8ZKZ2H_sendMessage);
}

void Heavy_Untitled::cVar_DI8ZKZ2H_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_YRjUkg0h, HV_BINOP_ADD, 0, m, &cBinop_YRjUkg0h_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_VqkdPSuP, HV_BINOP_ADD, 0, m, &cBinop_VqkdPSuP_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_PmbysH6A, 0, m, &cVar_PmbysH6A_sendMessage);
}

void Heavy_Untitled::cSlice_jW9YO008_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_hwC0Dj0L_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_aBAP9c9v_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_Untitled::cSlice_Fu4gydLB_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_LmUAACDv_sendMessage);
      cCast_onMessage(_c, HV_CAST_FLOAT, 0, m, &cCast_AZSWybCz_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_Untitled::cBinop_S3oylruG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_WwKmJumd, 1, m, &cVar_WwKmJumd_sendMessage);
}

void Heavy_Untitled::cBinop_Z7rNXf3p_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_S3oylruG_sendMessage);
}

void Heavy_Untitled::cBinop_2zHrLwnR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_AgtfjRLM_sendMessage);
}

void Heavy_Untitled::cBinop_AgtfjRLM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_s1mEvnme, HV_BINOP_SUBTRACT, 1, m, &cBinop_s1mEvnme_sendMessage);
}

void Heavy_Untitled::cBinop_s1mEvnme_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_WwKmJumd, 1, m, &cVar_WwKmJumd_sendMessage);
}

void Heavy_Untitled::cMsg_XxP1dfCG_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  cSwitchcase_CpqTe71j_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_Untitled::cMsg_N6awwdfL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cSwitchcase_CpqTe71j_onMessage(_c, NULL, 0, m, NULL);
  cBinop_onMessage(_c, &Context(_c)->cBinop_VqkdPSuP, HV_BINOP_ADD, 1, m, &cBinop_VqkdPSuP_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_YRjUkg0h, HV_BINOP_ADD, 1, m, &cBinop_YRjUkg0h_sendMessage);
}

void Heavy_Untitled::cBinop_MT3iWDDa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cSwitchcase_ule5EMYV_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_Untitled::cBinop_YRjUkg0h_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_DI8ZKZ2H, 1, m, &cVar_DI8ZKZ2H_sendMessage);
}

void Heavy_Untitled::cBinop_sT7rlI8p_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_a20bM1PO, HV_BINOP_DIVIDE, 1, m, &cBinop_a20bM1PO_sendMessage);
}

void Heavy_Untitled::cBinop_a20bM1PO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_VqkdPSuP, HV_BINOP_ADD, 1, m, &cBinop_VqkdPSuP_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_YRjUkg0h, HV_BINOP_ADD, 1, m, &cBinop_YRjUkg0h_sendMessage);
}

void Heavy_Untitled::cCast_aBAP9c9v_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_sT7rlI8p, HV_BINOP_DIVIDE, 0, m, &cBinop_sT7rlI8p_sendMessage);
}

void Heavy_Untitled::cCast_hwC0Dj0L_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_Z7rNXf3p, HV_BINOP_MULTIPLY, 0, m, &cBinop_Z7rNXf3p_sendMessage);
}

void Heavy_Untitled::cCast_LmUAACDv_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_zc6Eyrov, 1, m, &cVar_zc6Eyrov_sendMessage);
}

void Heavy_Untitled::cCast_AZSWybCz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_YN8wyJ5m, HV_BINOP_SUBTRACT, 0, m, &cBinop_YN8wyJ5m_sendMessage);
}

void Heavy_Untitled::cCast_4mWSEeRx_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_zc6Eyrov, 0, m, &cVar_zc6Eyrov_sendMessage);
}

void Heavy_Untitled::cCast_iqZZMLRj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_N6awwdfL_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cBinop_VqkdPSuP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_TC9InOYO, 0, m, &cVar_TC9InOYO_sendMessage);
}

void Heavy_Untitled::cMsg_9OweGO3x_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cSwitchcase_CpqTe71j_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_Untitled::cMsg_5TOErsuQ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  cVar_onMessage(_c, &Context(_c)->cVar_kw4Y3zR6, 1, m, &cVar_kw4Y3zR6_sendMessage);
}

void Heavy_Untitled::cMsg_AbcwLLoj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0, 20.0f);
  cBinop_onMessage(_c, &Context(_c)->cBinop_2zHrLwnR, HV_BINOP_MULTIPLY, 0, m, &cBinop_2zHrLwnR_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_sT7rlI8p, HV_BINOP_DIVIDE, 1, m, &cBinop_sT7rlI8p_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_8NfhCLlk, 0, m, &cVar_8NfhCLlk_sendMessage);
}

void Heavy_Untitled::cCast_NNMGMx06_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_hOJxpNNn_sendMessage);
  cBinop_onMessage(_c, &Context(_c)->cBinop_VqkdPSuP, HV_BINOP_ADD, 0, m, &cBinop_VqkdPSuP_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_DI8ZKZ2H, 1, m, &cVar_DI8ZKZ2H_sendMessage);
}

void Heavy_Untitled::cCast_sIaX48hn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_9OweGO3x_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cBinop_YN8wyJ5m_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_a20bM1PO, HV_BINOP_DIVIDE, 0, m, &cBinop_a20bM1PO_sendMessage);
}

void Heavy_Untitled::cCast_hOJxpNNn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_N6awwdfL_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cCast_r7fP0KS6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_kw4Y3zR6, 0, m, &cVar_kw4Y3zR6_sendMessage);
}

void Heavy_Untitled::cCast_Mk6vwJs6_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_5TOErsuQ_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cCast_JezzTHMu_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_XxP1dfCG_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cSwitchcase_5os46LQM_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7E64BD01: { // "seed"
      cSlice_onMessage(_c, &Context(_c)->cSlice_PL8crhqS, 0, m, &cSlice_PL8crhqS_sendMessage);
      break;
    }
    default: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_S6JljoU8, 0, m, &cRandom_S6JljoU8_sendMessage);
      break;
    }
  }
}

void Heavy_Untitled::cBinop_9SdfZCmY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cUnop_onMessage(_c, HV_UNOP_FLOOR, m, &cUnop_tVisrK9O_sendMessage);
}

void Heavy_Untitled::cUnop_tVisrK9O_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_ADD, 60.0f, 0, m, &cBinop_JFjtwhzz_sendMessage);
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 200.0f, 0, m, &cBinop_jUyzpAYe_sendMessage);
}

void Heavy_Untitled::cRandom_S6JljoU8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 20.0f, 0, m, &cBinop_9SdfZCmY_sendMessage);
}

void Heavy_Untitled::cSlice_PL8crhqS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      cRandom_onMessage(_c, &Context(_c)->cRandom_S6JljoU8, 1, m, &cRandom_S6JljoU8_sendMessage);
      break;
    }
    case 1: {
      break;
    }
    default: return;
  }
}

void Heavy_Untitled::cSwitchcase_PN2HfNAx_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x0: { // "0.0"
      cMsg_gxJ12ypP_sendMessage(_c, 0, m);
      break;
    }
    case 0x7A5B032D: { // "stop"
      cMsg_gxJ12ypP_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_ZLBEO7iW_sendMessage);
      break;
    }
  }
}

void Heavy_Untitled::cDelay_U8Xu12Pw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_U8Xu12Pw, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_U8Xu12Pw, 0, m, &cDelay_U8Xu12Pw_sendMessage);
  cSwitchcase_5os46LQM_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_Untitled::cCast_ZLBEO7iW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_gxJ12ypP_sendMessage(_c, 0, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_U8Xu12Pw, 0, m, &cDelay_U8Xu12Pw_sendMessage);
  cSwitchcase_5os46LQM_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_Untitled::cMsg_vsh6CCUE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_wX5aLXbJ_sendMessage);
}

void Heavy_Untitled::cSystem_wX5aLXbJ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_JZyzDSFr_sendMessage);
}

void Heavy_Untitled::cVar_gyHTRgwW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_AgEFsytW, HV_BINOP_MULTIPLY, 0, m, &cBinop_AgEFsytW_sendMessage);
}

void Heavy_Untitled::cMsg_gxJ12ypP_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_U8Xu12Pw, 0, m, &cDelay_U8Xu12Pw_sendMessage);
}

void Heavy_Untitled::cBinop_T206xAou_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_U8Xu12Pw, 2, m, &cDelay_U8Xu12Pw_sendMessage);
}

void Heavy_Untitled::cBinop_JZyzDSFr_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_AgEFsytW, HV_BINOP_MULTIPLY, 1, m, &cBinop_AgEFsytW_sendMessage);
}

void Heavy_Untitled::cBinop_AgEFsytW_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MAX, 1.0f, 0, m, &cBinop_T206xAou_sendMessage);
}

void Heavy_Untitled::cTabhead_r2G6CGXf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_NDnpkYMa, HV_BINOP_SUBTRACT, 0, m, &cBinop_NDnpkYMa_sendMessage);
}

void Heavy_Untitled::cMsg_T11ZiFbk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_pfQRYcGZ_sendMessage);
}

void Heavy_Untitled::cSystem_pfQRYcGZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_00vxQLL9_sendMessage);
}

void Heavy_Untitled::cVar_JcyjQ9Xn_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_PqMEzv70_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cDelay_JjWXGYms_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_JjWXGYms, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_Wm2Q3dMb, 0, m, &cDelay_Wm2Q3dMb_sendMessage);
  sTabread_onMessage(_c, &Context(_c)->sTabread_Pbav1iaY, 0, m, &sTabread_Pbav1iaY_sendMessage);
}

void Heavy_Untitled::cDelay_Wm2Q3dMb_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_Wm2Q3dMb, m);
  sTabread_onMessage(_c, &Context(_c)->sTabread_Pbav1iaY, 0, m, &sTabread_Pbav1iaY_sendMessage);
  cDelay_onMessage(_c, &Context(_c)->cDelay_Wm2Q3dMb, 0, m, &cDelay_Wm2Q3dMb_sendMessage);
}

void Heavy_Untitled::sTabread_Pbav1iaY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_xwAdx1wT, HV_BINOP_SUBTRACT, 0, m, &cBinop_xwAdx1wT_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_Untitled::cBinop_G1svjTPE_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_FOGYJ5Um, HV_BINOP_MAX, 0, m, &cBinop_FOGYJ5Um_sendMessage);
}

void Heavy_Untitled::cBinop_00vxQLL9_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_G1svjTPE, HV_BINOP_MULTIPLY, 0, m, &cBinop_G1svjTPE_sendMessage);
}

void Heavy_Untitled::cBinop_NDnpkYMa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_tPSCvsIa_sendMessage(_c, 0, m);
  sTabread_onMessage(_c, &Context(_c)->sTabread_Pbav1iaY, 0, m, &sTabread_Pbav1iaY_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_niLKzlIL_sendMessage);
}

void Heavy_Untitled::cSystem_6dbSnqjY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_xwAdx1wT, HV_BINOP_SUBTRACT, 1, m, &cBinop_xwAdx1wT_sendMessage);
  cDelay_onMessage(_c, &Context(_c)->cDelay_Wm2Q3dMb, 2, m, &cDelay_Wm2Q3dMb_sendMessage);
}

void Heavy_Untitled::cMsg_PqMEzv70_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(3);
  msg_init(m, 3, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "table");
  msg_setElementToFrom(m, 1, n, 0);
  msg_setSymbol(m, 2, "size");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_6dbSnqjY_sendMessage);
}

void Heavy_Untitled::cMsg_tPSCvsIa_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_JjWXGYms, 0, m, &cDelay_JjWXGYms_sendMessage);
  cDelay_onMessage(_c, &Context(_c)->cDelay_Wm2Q3dMb, 0, m, &cDelay_Wm2Q3dMb_sendMessage);
}

void Heavy_Untitled::cMsg_hIaJ4Rro_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0,  static_cast<float>(HV_N_SIMD));
  cBinop_onMessage(_c, &Context(_c)->cBinop_FOGYJ5Um, HV_BINOP_MAX, 1, m, &cBinop_FOGYJ5Um_sendMessage);
}

void Heavy_Untitled::cBinop_FOGYJ5Um_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_NDnpkYMa, HV_BINOP_SUBTRACT, 1, m, &cBinop_NDnpkYMa_sendMessage);
}

void Heavy_Untitled::cCast_niLKzlIL_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_JjWXGYms, 0, m, &cDelay_JjWXGYms_sendMessage);
}

void Heavy_Untitled::cBinop_8BG1tUhw_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_JjWXGYms, 2, m, &cDelay_JjWXGYms_sendMessage);
}

void Heavy_Untitled::cBinop_xwAdx1wT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -1.0f, 0, m, &cBinop_8BG1tUhw_sendMessage);
}

void Heavy_Untitled::cCast_8PPOL1tV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_JcyjQ9Xn, 0, m, &cVar_JcyjQ9Xn_sendMessage);
  cMsg_T11ZiFbk_sendMessage(_c, 0, m);
  cTabhead_onMessage(_c, &Context(_c)->cTabhead_r2G6CGXf, 0, m, &cTabhead_r2G6CGXf_sendMessage);
}

void Heavy_Untitled::cMsg_KsbxfZKo_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 200.0f);
  cSwitchcase_5UjoJQef_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_Untitled::cBinop_JFjtwhzz_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_KsbxfZKo_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cBinop_jUyzpAYe_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_QDgEJH0W_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cMsg_QDgEJH0W_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setElementToFrom(m, 0, n, 0);
  msg_setFloat(m, 1, 400.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_6etJVKFb, 0, m, NULL);
}

void Heavy_Untitled::cReceive_OEYXkWDM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_szg5ZRIq_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_8NfhCLlk, 0, m, &cVar_8NfhCLlk_sendMessage);
  cMsg_5UN3EJZp_sendMessage(_c, 0, m);
  cMsg_GpAOfsSE_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_TC9InOYO, 0, m, &cVar_TC9InOYO_sendMessage);
  cVar_onMessage(_c, &Context(_c)->cVar_xEescJU2, 0, m, &cVar_xEescJU2_sendMessage);
  cMsg_vsh6CCUE_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_gyHTRgwW, 0, m, &cVar_gyHTRgwW_sendMessage);
  cSwitchcase_PN2HfNAx_onMessage(_c, NULL, 0, m, NULL);
  cMsg_hIaJ4Rro_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_JcyjQ9Xn, 0, m, &cVar_JcyjQ9Xn_sendMessage);
  cMsg_T11ZiFbk_sendMessage(_c, 0, m);
  cTabhead_onMessage(_c, &Context(_c)->cTabhead_r2G6CGXf, 0, m, &cTabhead_r2G6CGXf_sendMessage);
}



/*
 * Code for expr~ implementation
 * Write out the generic implementation code
 */

 // per class code

 // per object code


/*
 * Context Process Implementation
 */

int Heavy_Untitled::process(float **inputBuffers, float **outputBuffers, int n) {
  while (hLp_hasData(&inQueue)) {
    hv_uint32_t numBytes = 0;
    ReceiverMessagePair *p = reinterpret_cast<ReceiverMessagePair *>(hLp_getReadBuffer(&inQueue, &numBytes));
    hv_assert(numBytes >= sizeof(ReceiverMessagePair));
    scheduleMessageForReceiver(p->receiverHash, &p->msg);
    hLp_consume(&inQueue);
  }

  sendBangToReceiver(0xDD21C0EB); // send to __hv_bang~ on next cycle
  const int n4 = n & ~HV_N_SIMD_MASK; // ensure that the block size is a multiple of HV_N_SIMD

  // temporary signal vars
  hv_bufferf_t Bf0, Bf1, Bf2, Bf3, Bf4;

  // input and output vars
  hv_bufferf_t O0, O1;

  // declare and init the zero buffer
  hv_bufferf_t ZERO; __hv_zero_f(VOf(ZERO));

  hv_uint32_t nextBlock = blockStartTimestamp;
  for (int n = 0; n < n4; n += HV_N_SIMD) {

    // process all of the messages for this block
    nextBlock += HV_N_SIMD;
    while (mq_hasMessageBefore(&mq, nextBlock)) {
      MessageNode *const node = mq_peek(&mq);
      node->sendMessage(this, node->let, node->m);
      mq_pop(&mq);
    }

    

    // zero output buffers
    __hv_zero_f(VOf(O0));
    __hv_zero_f(VOf(O1));

    // process all signal functions
    __hv_phasor_k_f(&sPhasor_IzN1USIZ, VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_abs_f(VIf(Bf1), VOf(Bf1));
    __hv_var_k_f(VOf(Bf0), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f, 6.283185307179586f);
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_mul_f(VIf(Bf1), VIf(Bf1), VOf(Bf0));
    __hv_mul_f(VIf(Bf1), VIf(Bf0), VOf(Bf2));
    __hv_mul_f(VIf(Bf2), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf3), 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f, 0.007833333333333f);
    __hv_var_k_f(VOf(Bf4), -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f, -0.166666666666667f);
    __hv_fma_f(VIf(Bf2), VIf(Bf4), VIf(Bf1), VOf(Bf1));
    __hv_fma_f(VIf(Bf0), VIf(Bf3), VIf(Bf1), VOf(Bf1));
    __hv_tabread_f(&sTabread_Pbav1iaY, VOf(Bf3));
    __hv_var_k_f(VOf(Bf0), 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f);
    __hv_mul_f(VIf(Bf3), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf1), VIf(Bf0), VOf(Bf1));
    __hv_tabwrite_f(&sTabwrite_ky5r2TMU, VIf(Bf1));
    __hv_line_f(&sLine_6etJVKFb, VOf(Bf1));
    __hv_var_k_f(VOf(Bf1), 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
    __hv_mul_f(VIf(Bf3), VIf(Bf1), VOf(Bf1));
    __hv_var_k_f(VOf(Bf3), 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
    __hv_mul_f(VIf(Bf0), VIf(Bf3), VOf(Bf3));
    __hv_add_f(VIf(Bf1), VIf(Bf3), VOf(Bf0));
    __hv_add_f(VIf(Bf0), VIf(O0), VOf(O0));
    __hv_add_f(VIf(Bf1), VIf(Bf3), VOf(Bf3));
    __hv_add_f(VIf(Bf3), VIf(O1), VOf(O1));

    // save output vars to output buffer
    __hv_store_f(outputBuffers[0]+n, VIf(O0));
    __hv_store_f(outputBuffers[1]+n, VIf(O1));
  }

  blockStartTimestamp = nextBlock;

  return n4; // return the number of frames processed

}

int Heavy_Untitled::processInline(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(!(n4 & HV_N_SIMD_MASK)); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s)
  float **const bIn = NULL;

  // define the heavy output buffer for 2 channel(s)
  float **const bOut = reinterpret_cast<float **>(hv_alloca(2*sizeof(float *)));
  bOut[0] = outputBuffers+(0*n4);
  bOut[1] = outputBuffers+(1*n4);

  int n = process(bIn, bOut, n4);
  return n;
}

int Heavy_Untitled::processInlineInterleaved(float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(n4 & ~HV_N_SIMD_MASK); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s), uninterleave
  float *const bIn = NULL;

  // define the heavy output buffer for 2 channel(s)
  float *const bOut = reinterpret_cast<float *>(hv_alloca(2*n4*sizeof(float)));

  int n = processInline(bIn, bOut, n4);

  // interleave the heavy output into the output buffer
  #if HV_SIMD_AVX
  for (int i = 0, j = 0; j < n4; j += 8, i += 16) {
    __m256 x = _mm256_load_ps(bOut+j);    // LLLLLLLL
    __m256 y = _mm256_load_ps(bOut+n4+j); // RRRRRRRR
    __m256 a = _mm256_unpacklo_ps(x, y);  // LRLRLRLR
    __m256 b = _mm256_unpackhi_ps(x, y);  // LRLRLRLR
    _mm256_store_ps(outputBuffers+i, a);
    _mm256_store_ps(outputBuffers+8+i, b);
  }
  #elif HV_SIMD_SSE
  for (int i = 0, j = 0; j < n4; j += 4, i += 8) {
    __m128 x = _mm_load_ps(bOut+j);    // LLLL
    __m128 y = _mm_load_ps(bOut+n4+j); // RRRR
    __m128 a = _mm_unpacklo_ps(x, y);  // LRLR
    __m128 b = _mm_unpackhi_ps(x, y);  // LRLR
    _mm_store_ps(outputBuffers+i, a);
    _mm_store_ps(outputBuffers+4+i, b);
  }
  #elif HV_SIMD_NEON
  // https://community.arm.com/groups/processors/blog/2012/03/13/coding-for-neon--part-5-rearranging-vectors
  for (int i = 0, j = 0; j < n4; j += 4, i += 8) {
    float32x4_t x = vld1q_f32(bOut+j);
    float32x4_t y = vld1q_f32(bOut+n4+j);
    float32x4x2_t z = {x, y};
    vst2q_f32(outputBuffers+i, z); // interleave and store
  }
  #else // HV_SIMD_NONE
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < n4; ++j) {
      outputBuffers[i+2*j] = bOut[i*n4+j];
    }
  }
  #endif

  return n;
}
