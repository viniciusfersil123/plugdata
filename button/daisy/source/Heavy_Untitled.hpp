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

#ifndef _HEAVY_CONTEXT_UNTITLED_HPP_
#define _HEAVY_CONTEXT_UNTITLED_HPP_

// object includes
#include "HeavyContext.hpp"
#include "HvControlVar.h"
#include "HvSignalTabread.h"
#include "HvSignalVar.h"
#include "HvTable.h"
#include "HvControlDelay.h"
#include "HvSignalLine.h"
#include "HvSignalPhasor.h"
#include "HvControlSystem.h"
#include "HvMath.h"
#include "HvControlBinop.h"
#include "HvControlCast.h"
#include "HvControlTabhead.h"
#include "HvSignalTabwrite.h"

class Heavy_Untitled : public HeavyContext {

 public:
  Heavy_Untitled(double sampleRate, int poolKb=10, int inQueueKb=2, int outQueueKb=0);
  ~Heavy_Untitled();

  const char *getName() override { return "Untitled"; }
  int getNumInputChannels() override { return 0; }
  int getNumOutputChannels() override { return 2; }

  int process(float **inputBuffers, float **outputBuffer, int n) override;
  int processInline(float *inputBuffers, float *outputBuffer, int n) override;
  int processInlineInterleaved(float *inputBuffers, float *outputBuffer, int n) override;

  int getParameterInfo(int index, HvParameterInfo *info) override;
  struct Parameter {
    struct In {
      enum ParameterIn : hv_uint32_t {
        BUTTON1 = 0xB4D78F23, // Button1
      };
    };
  };

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;


  /*
  * Code for expr~ implementation
  * Write out the generic header code
  */

  // per class code
  typedef void(*cExprSig_evaluator)(hv_bInf_t*, hv_bOutf_t);
  cExprSig_evaluator cExprSig_evaluators[1] = {
		cExprSig_hFQcEvWr_evaluate
	};

  // per object code
  static inline void cExprSig_hFQcEvWr_evaluate(hv_bInf_t* bIns, hv_bOutf_t bOut);


  // static sendMessage functions
  static void cMsg_zPTgOwAy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_EJB1sctY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cDelay_rq2TVTrX_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cDelay_PqT89BFj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_fBCHgoq5_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_4bpesiLS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void hTable_QSjz4tBy_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_mFDJF7Ch_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_BXXVq4t4_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_zdtO2YYO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_YLcY2ZuR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_8W5gOQ5j_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_ltxg4rHM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cTabhead_6ogtjGW5_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_EFRDNu6j_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_7ftrQzQj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_LcGJIeab_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cDelay_K3JqNWf7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cDelay_jLIKHcQi_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void sTabread_f5ov1vsV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_1sbNQHAZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_v7WipM72_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_JLNk2RGh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_fs2bOaqY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_sg3wSlWV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_tTjN9c33_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_Iakk25yM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_H8ZEDsUl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_sVuU4X5Q_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_rGzTyaCT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_VHsscUY7_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_OKasV6ro_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_DJ2XNwde_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_i4tXrnDW_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_kgnMHK2b_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_aq59ijdf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_6ZRmLAL8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_2jAOmoqM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_DdKCyhYk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_QNrkqVME_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_00i9dPph_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalPhasor sPhasor_QqgK6T45;
  SignalLine sLine_nRv5uUrb;
  SignalTabread sTabread_f5ov1vsV;
  SignalTabwrite sTabwrite_olVHaTCC;
  ControlDelay cDelay_rq2TVTrX;
  ControlDelay cDelay_PqT89BFj;
  ControlBinop cBinop_4bpesiLS;
  HvTable hTable_QSjz4tBy;
  ControlBinop cBinop_BXXVq4t4;
  ControlTabhead cTabhead_6ogtjGW5;
  ControlVar cVar_LcGJIeab;
  ControlDelay cDelay_K3JqNWf7;
  ControlDelay cDelay_jLIKHcQi;
  ControlBinop cBinop_1sbNQHAZ;
  ControlBinop cBinop_v7WipM72;
  ControlBinop cBinop_JLNk2RGh;
  ControlBinop cBinop_H8ZEDsUl;
  ControlBinop cBinop_rGzTyaCT;
  ControlBinop cBinop_VHsscUY7;
  ControlDelay cDelay_kgnMHK2b;
};

#endif // _HEAVY_CONTEXT_UNTITLED_HPP_
