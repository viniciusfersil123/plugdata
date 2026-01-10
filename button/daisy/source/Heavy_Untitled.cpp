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
  numBytes += sPhasor_k_init(&sPhasor_QqgK6T45, 440.0f, sampleRate);
  numBytes += sLine_init(&sLine_nRv5uUrb);
  numBytes += sTabread_init(&sTabread_f5ov1vsV, &hTable_QSjz4tBy, true);
  numBytes += sTabwrite_init(&sTabwrite_olVHaTCC, &hTable_QSjz4tBy);
  numBytes += cDelay_init(this, &cDelay_rq2TVTrX, 0.0f);
  numBytes += cDelay_init(this, &cDelay_PqT89BFj, 0.0f);
  numBytes += hTable_init(&hTable_QSjz4tBy, 256);
  numBytes += cTabhead_init(&cTabhead_6ogtjGW5, &hTable_QSjz4tBy);
  numBytes += cVar_init_s(&cVar_LcGJIeab, "del-teste");
  numBytes += cDelay_init(this, &cDelay_K3JqNWf7, 500.0f);
  numBytes += cDelay_init(this, &cDelay_jLIKHcQi, 0.0f);
  numBytes += cBinop_init(&cBinop_1sbNQHAZ, 500.0f); // __mul
  numBytes += cBinop_init(&cBinop_JLNk2RGh, 0.0f); // __sub
  numBytes += cBinop_init(&cBinop_H8ZEDsUl, 0.0f); // __max
  numBytes += cBinop_init(&cBinop_VHsscUY7, 0.0f); // __sub
  numBytes += cDelay_init(this, &cDelay_kgnMHK2b, 100.0f);
  
  // schedule a message to trigger all loadbangs via the __hv_init receiver
  scheduleMessageForReceiver(0xCE5CC65B, msg_initWithBang(HV_MESSAGE_ON_STACK(1), 0));
}

Heavy_Untitled::~Heavy_Untitled() {
  hTable_free(&hTable_QSjz4tBy);
}

HvTable *Heavy_Untitled::getTableForHash(hv_uint32_t tableHash) {switch (tableHash) {
    case 0xB0952E2D: return &hTable_QSjz4tBy; // del-teste
    default: return nullptr;
  }
}

void Heavy_Untitled::scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) {
  switch (receiverHash) {
    case 0xB4D78F23: { // Button1
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_00i9dPph_sendMessage);
      break;
    }
    case 0xCE5CC65B: { // __hv_init
      mq_addMessageByTimestamp(&mq, m, 0, &cReceive_QNrkqVME_sendMessage);
      break;
    }
    default: return;
  }
}

int Heavy_Untitled::getParameterInfo(int index, HvParameterInfo *info) {
  if (info != nullptr) {
    switch (index) {
      case 0: {
        info->name = "Button1";
        info->hash = 0xB4D78F23;
        info->type = HvParameterType::HV_PARAM_TYPE_PARAMETER_IN;
        info->minVal = 0.0f;
        info->maxVal = 1.0f;
        info->defaultVal = 0.5f;
        break;
      }
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
  return 1;
}



/*
 * Send Function Implementations
 */


void Heavy_Untitled::cMsg_zPTgOwAy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_EJB1sctY_sendMessage);
}

void Heavy_Untitled::cSystem_EJB1sctY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_BXXVq4t4_sendMessage);
}

void Heavy_Untitled::cDelay_rq2TVTrX_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_rq2TVTrX, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_PqT89BFj, 0, m, &cDelay_PqT89BFj_sendMessage);
  cDelay_onMessage(_c, &Context(_c)->cDelay_rq2TVTrX, 0, m, &cDelay_rq2TVTrX_sendMessage);
  sTabwrite_onMessage(_c, &Context(_c)->sTabwrite_olVHaTCC, 1, m, NULL);
}

void Heavy_Untitled::cDelay_PqT89BFj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_PqT89BFj, m);
  cMsg_zdtO2YYO_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cSwitchcase_fBCHgoq5_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x47BE8354: { // "clear"
      cMsg_ltxg4rHM_sendMessage(_c, 0, m);
      break;
    }
    default: {
      break;
    }
  }
}

void Heavy_Untitled::cBinop_4bpesiLS_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_mFDJF7Ch_sendMessage(_c, 0, m);
}

void Heavy_Untitled::hTable_QSjz4tBy_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_8W5gOQ5j_sendMessage(_c, 0, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_rq2TVTrX, 2, m, &cDelay_rq2TVTrX_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_YLcY2ZuR_sendMessage);
}

void Heavy_Untitled::cMsg_mFDJF7Ch_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "resize");
  msg_setElementToFrom(m, 1, n, 0);
  hTable_onMessage(_c, &Context(_c)->hTable_QSjz4tBy, 0, m, &hTable_QSjz4tBy_sendMessage);
}

void Heavy_Untitled::cBinop_BXXVq4t4_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, 500.0f, 0, m, &cBinop_4bpesiLS_sendMessage);
}

void Heavy_Untitled::cMsg_zdtO2YYO_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "mirror");
  hTable_onMessage(_c, &Context(_c)->hTable_QSjz4tBy, 0, m, &hTable_QSjz4tBy_sendMessage);
}

void Heavy_Untitled::cCast_YLcY2ZuR_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_rq2TVTrX, 0, m, &cDelay_rq2TVTrX_sendMessage);
}

void Heavy_Untitled::cMsg_8W5gOQ5j_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0,  static_cast<float>(HV_N_SIMD));
  cDelay_onMessage(_c, &Context(_c)->cDelay_PqT89BFj, 2, m, &cDelay_PqT89BFj_sendMessage);
}

void Heavy_Untitled::cMsg_ltxg4rHM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  sTabwrite_onMessage(_c, &Context(_c)->sTabwrite_olVHaTCC, 1, m, NULL);
}

void Heavy_Untitled::cTabhead_6ogtjGW5_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_JLNk2RGh, HV_BINOP_SUBTRACT, 0, m, &cBinop_JLNk2RGh_sendMessage);
}

void Heavy_Untitled::cMsg_EFRDNu6j_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "samplerate");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_7ftrQzQj_sendMessage);
}

void Heavy_Untitled::cSystem_7ftrQzQj_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_DIVIDE, 1000.0f, 0, m, &cBinop_v7WipM72_sendMessage);
}

void Heavy_Untitled::cVar_LcGJIeab_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_sg3wSlWV_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cDelay_K3JqNWf7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_K3JqNWf7, m);
  cDelay_onMessage(_c, &Context(_c)->cDelay_jLIKHcQi, 0, m, &cDelay_jLIKHcQi_sendMessage);
  sTabread_onMessage(_c, &Context(_c)->sTabread_f5ov1vsV, 0, m, &sTabread_f5ov1vsV_sendMessage);
}

void Heavy_Untitled::cDelay_jLIKHcQi_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_jLIKHcQi, m);
  sTabread_onMessage(_c, &Context(_c)->sTabread_f5ov1vsV, 0, m, &sTabread_f5ov1vsV_sendMessage);
  cDelay_onMessage(_c, &Context(_c)->cDelay_jLIKHcQi, 0, m, &cDelay_jLIKHcQi_sendMessage);
}

void Heavy_Untitled::sTabread_f5ov1vsV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  switch (letIn) {
    case 0: {
      break;
    }
    case 1: {
      cBinop_onMessage(_c, &Context(_c)->cBinop_VHsscUY7, HV_BINOP_SUBTRACT, 0, m, &cBinop_VHsscUY7_sendMessage);
      break;
    }
    default: return;
  }
}

void Heavy_Untitled::cBinop_1sbNQHAZ_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_H8ZEDsUl, HV_BINOP_MAX, 0, m, &cBinop_H8ZEDsUl_sendMessage);
}

void Heavy_Untitled::cBinop_v7WipM72_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_1sbNQHAZ, HV_BINOP_MULTIPLY, 0, m, &cBinop_1sbNQHAZ_sendMessage);
}

void Heavy_Untitled::cBinop_JLNk2RGh_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_tTjN9c33_sendMessage(_c, 0, m);
  sTabread_onMessage(_c, &Context(_c)->sTabread_f5ov1vsV, 0, m, &sTabread_f5ov1vsV_sendMessage);
  cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_sVuU4X5Q_sendMessage);
}

void Heavy_Untitled::cSystem_fs2bOaqY_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_VHsscUY7, HV_BINOP_SUBTRACT, 1, m, &cBinop_VHsscUY7_sendMessage);
  cDelay_onMessage(_c, &Context(_c)->cDelay_jLIKHcQi, 2, m, &cDelay_jLIKHcQi_sendMessage);
}

void Heavy_Untitled::cMsg_sg3wSlWV_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(3);
  msg_init(m, 3, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "table");
  msg_setElementToFrom(m, 1, n, 0);
  msg_setSymbol(m, 2, "size");
  cSystem_onMessage(_c, NULL, 0, m, &cSystem_fs2bOaqY_sendMessage);
}

void Heavy_Untitled::cMsg_tTjN9c33_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_K3JqNWf7, 0, m, &cDelay_K3JqNWf7_sendMessage);
  cDelay_onMessage(_c, &Context(_c)->cDelay_jLIKHcQi, 0, m, &cDelay_jLIKHcQi_sendMessage);
}

void Heavy_Untitled::cMsg_Iakk25yM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setFloat(m, 0,  static_cast<float>(HV_N_SIMD));
  cBinop_onMessage(_c, &Context(_c)->cBinop_H8ZEDsUl, HV_BINOP_MAX, 1, m, &cBinop_H8ZEDsUl_sendMessage);
}

void Heavy_Untitled::cBinop_H8ZEDsUl_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_onMessage(_c, &Context(_c)->cBinop_JLNk2RGh, HV_BINOP_SUBTRACT, 1, m, &cBinop_JLNk2RGh_sendMessage);
}

void Heavy_Untitled::cCast_sVuU4X5Q_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_K3JqNWf7, 0, m, &cDelay_K3JqNWf7_sendMessage);
}

void Heavy_Untitled::cBinop_rGzTyaCT_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_K3JqNWf7, 2, m, &cDelay_K3JqNWf7_sendMessage);
}

void Heavy_Untitled::cBinop_VHsscUY7_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cBinop_k_onMessage(_c, NULL, HV_BINOP_MULTIPLY, -1.0f, 0, m, &cBinop_rGzTyaCT_sendMessage);
}

void Heavy_Untitled::cCast_OKasV6ro_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cVar_onMessage(_c, &Context(_c)->cVar_LcGJIeab, 0, m, &cVar_LcGJIeab_sendMessage);
  cMsg_EFRDNu6j_sendMessage(_c, 0, m);
  cTabhead_onMessage(_c, &Context(_c)->cTabhead_6ogtjGW5, 0, m, &cTabhead_6ogtjGW5_sendMessage);
}

void Heavy_Untitled::cCast_DJ2XNwde_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_2jAOmoqM_sendMessage(_c, 0, m);
  cSwitchcase_i4tXrnDW_onMessage(_c, NULL, 0, m, NULL);
}

void Heavy_Untitled::cSwitchcase_i4tXrnDW_onMessage(HeavyContextInterface *_c, void *o, int letIn, const HvMessage *const m, void *sendMessage) {
  int msgIndex = 0;
  switch (msg_getHash(m, msgIndex)) {
    case 0x6D60E6E: { // "symbol"
      msgIndex = 1;
      break;
    }
  }
  switch (msg_getHash(m, msgIndex)) {
    case 0x7A5B032D: { // "stop"
      cMsg_aq59ijdf_sendMessage(_c, 0, m);
      break;
    }
    default: {
      cMsg_aq59ijdf_sendMessage(_c, 0, m);
      cDelay_onMessage(_c, &Context(_c)->cDelay_kgnMHK2b, 1, m, &cDelay_kgnMHK2b_sendMessage);
      cCast_onMessage(_c, HV_CAST_BANG, 0, m, &cCast_6ZRmLAL8_sendMessage);
      break;
    }
  }
}

void Heavy_Untitled::cDelay_kgnMHK2b_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const m) {
  cDelay_clearExecutingMessage(&Context(_c)->cDelay_kgnMHK2b, m);
  cMsg_DdKCyhYk_sendMessage(_c, 0, m);
}

void Heavy_Untitled::cMsg_aq59ijdf_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(1);
  msg_init(m, 1, msg_getTimestamp(n));
  msg_setSymbol(m, 0, "clear");
  cDelay_onMessage(_c, &Context(_c)->cDelay_kgnMHK2b, 0, m, &cDelay_kgnMHK2b_sendMessage);
}

void Heavy_Untitled::cCast_6ZRmLAL8_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cDelay_onMessage(_c, &Context(_c)->cDelay_kgnMHK2b, 0, m, &cDelay_kgnMHK2b_sendMessage);
}

void Heavy_Untitled::cMsg_2jAOmoqM_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 1.0f);
  msg_setFloat(m, 1, 50.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_nRv5uUrb, 0, m, NULL);
}

void Heavy_Untitled::cMsg_DdKCyhYk_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *const n) {
  HvMessage *m = nullptr;
  m = HV_MESSAGE_ON_STACK(2);
  msg_init(m, 2, msg_getTimestamp(n));
  msg_setFloat(m, 0, 0.0f);
  msg_setFloat(m, 1, 50.0f);
  sLine_onMessage(_c, &Context(_c)->sLine_nRv5uUrb, 0, m, NULL);
}

void Heavy_Untitled::cReceive_QNrkqVME_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_zPTgOwAy_sendMessage(_c, 0, m);
  cMsg_Iakk25yM_sendMessage(_c, 0, m);
  cVar_onMessage(_c, &Context(_c)->cVar_LcGJIeab, 0, m, &cVar_LcGJIeab_sendMessage);
  cMsg_EFRDNu6j_sendMessage(_c, 0, m);
  cTabhead_onMessage(_c, &Context(_c)->cTabhead_6ogtjGW5, 0, m, &cTabhead_6ogtjGW5_sendMessage);
}

void Heavy_Untitled::cReceive_00i9dPph_sendMessage(HeavyContextInterface *_c, int letIn, const HvMessage *m) {
  cMsg_2jAOmoqM_sendMessage(_c, 0, m);
  cSwitchcase_i4tXrnDW_onMessage(_c, NULL, 0, m, NULL);
}



/*
 * Code for expr~ implementation
 * Write out the generic implementation code
 */

 // per class code

 // per object code
 
 void Heavy_Untitled::cExprSig_hFQcEvWr_evaluate(hv_bInf_t* bIns, hv_bOutf_t bOut) {
 	// no extra buffers needed
 	__hv_mul_f(bIns[0], bIns[1], bOut);
 }


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
    __hv_phasor_k_f(&sPhasor_QqgK6T45, VOf(Bf0));
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
    __hv_line_f(&sLine_nRv5uUrb, VOf(Bf3));
    
    	// !!! declare this buffer once outside the loop
    	hv_bInf_t input_args_hFQcEvWr[2] = {VIf(Bf1), VIf(Bf3)};
    	cExprSig_evaluators[0](input_args_hFQcEvWr, VOf(Bf3));
    __hv_tabread_f(&sTabread_f5ov1vsV, VOf(Bf1));
    __hv_var_k_f(VOf(Bf0), 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f);
    __hv_mul_f(VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_add_f(VIf(Bf3), VIf(Bf0), VOf(Bf1));
    __hv_tabwrite_f(&sTabwrite_olVHaTCC, VIf(Bf1));
    __hv_add_f(VIf(Bf3), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf3), 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f);
    __hv_mul_f(VIf(Bf0), VIf(Bf3), VOf(Bf3));
    __hv_add_f(VIf(Bf3), VIf(O0), VOf(O0));
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
