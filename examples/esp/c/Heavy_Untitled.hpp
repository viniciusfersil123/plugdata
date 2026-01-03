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
#include "HvSignalPhasor.h"
#include "HvControlUnop.h"
#include "HvControlSlice.h"
#include "HvControlRandom.h"
#include "HvSignalTabread.h"
#include "HvSignalVar.h"
#include "HvControlTabhead.h"
#include "HvControlCast.h"
#include "HvControlDelay.h"
#include "HvSignalLine.h"
#include "HvSignalTabwrite.h"
#include "HvControlSystem.h"
#include "HvMath.h"
#include "HvControlVar.h"
#include "HvTable.h"
#include "HvControlBinop.h"

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

 private:
  HvTable *getTableForHash(hv_uint32_t tableHash) override;
  void scheduleMessageForReceiver(hv_uint32_t receiverHash, HvMessage *m) override;


  /*
  * Code for expr~ implementation
  * Write out the generic header code
  */

  // per class code

  // per object code


  // static sendMessage functions
  static void cMsg_5UN3EJZp_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_3aftrZmR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cDelay_XnzUr0Oh_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cDelay_313vDnWc_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_TT61NSIN_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_66UkTQBl_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void hTable_sRVyTwlZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_Ls6Tj94R_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_zpnszzis_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_L3VukneU_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_fxJfjZgm_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_wbBzFrwb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_72P9NvKW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_PmbysH6A_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_0ssPsyii_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_fDs9Rh4H_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Wcw5EJiI_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_8yBxqxnF_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_6PjPr9Zo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_sr7QTgrD_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_GpAOfsSE_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_3BQvV8Mo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_CpqTe71j_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_gBtFQ7Kq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_vXAv9D3G_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_szg5ZRIq_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_52afweua_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_8NfhCLlk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_Amu2hPzA_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_2EuBfoS3_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Xc5thTWo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_4OpOCCBO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_WwKmJumd_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_ule5EMYV_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_AlgoR2A1_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_dnnbkcGR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_5UjoJQef_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cSlice_fycj9DH0_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_OFonwLJL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_kw4Y3zR6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_xEescJU2_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_onIjYgKN_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cCast_mJTfm0HZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_TC9InOYO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_zc6Eyrov_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_DI8ZKZ2H_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_jW9YO008_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_Fu4gydLB_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_S3oylruG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_Z7rNXf3p_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_2zHrLwnR_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_AgtfjRLM_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_s1mEvnme_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_XxP1dfCG_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_N6awwdfL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_MT3iWDDa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_YRjUkg0h_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_sT7rlI8p_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_a20bM1PO_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_aBAP9c9v_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_hwC0Dj0L_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_LmUAACDv_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_AZSWybCz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_4mWSEeRx_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_iqZZMLRj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_VqkdPSuP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_9OweGO3x_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_5TOErsuQ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_AbcwLLoj_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_NNMGMx06_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_sIaX48hn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_YN8wyJ5m_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_hOJxpNNn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_r7fP0KS6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_Mk6vwJs6_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_JezzTHMu_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_5os46LQM_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cBinop_9SdfZCmY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cUnop_tVisrK9O_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cRandom_S6JljoU8_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSlice_PL8crhqS_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSwitchcase_PN2HfNAx_onMessage(HeavyContextInterface *, void *, int letIn, const HvMessage *const, void *);
  static void cDelay_U8Xu12Pw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_ZLBEO7iW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_vsh6CCUE_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_wX5aLXbJ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_gyHTRgwW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_gxJ12ypP_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_T206xAou_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_JZyzDSFr_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_AgEFsytW_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cTabhead_r2G6CGXf_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_T11ZiFbk_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_pfQRYcGZ_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cVar_JcyjQ9Xn_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cDelay_JjWXGYms_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cDelay_Wm2Q3dMb_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void sTabread_Pbav1iaY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_G1svjTPE_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_00vxQLL9_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_NDnpkYMa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cSystem_6dbSnqjY_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_PqMEzv70_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_tPSCvsIa_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_hIaJ4Rro_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_FOGYJ5Um_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_niLKzlIL_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_8BG1tUhw_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_xwAdx1wT_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cCast_8PPOL1tV_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_KsbxfZKo_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_JFjtwhzz_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cBinop_jUyzpAYe_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cMsg_QDgEJH0W_sendMessage(HeavyContextInterface *, int, const HvMessage *);
  static void cReceive_OEYXkWDM_sendMessage(HeavyContextInterface *, int, const HvMessage *);

  // objects
  SignalPhasor sPhasor_IzN1USIZ;
  SignalTabread sTabread_Pbav1iaY;
  SignalTabwrite sTabwrite_ky5r2TMU;
  SignalLine sLine_6etJVKFb;
  ControlDelay cDelay_XnzUr0Oh;
  ControlDelay cDelay_313vDnWc;
  ControlBinop cBinop_66UkTQBl;
  HvTable hTable_sRVyTwlZ;
  ControlBinop cBinop_zpnszzis;
  ControlVar cVar_PmbysH6A;
  ControlBinop cBinop_0ssPsyii;
  ControlBinop cBinop_fDs9Rh4H;
  ControlBinop cBinop_Wcw5EJiI;
  ControlBinop cBinop_8yBxqxnF;
  ControlDelay cDelay_gBtFQ7Kq;
  ControlVar cVar_8NfhCLlk;
  ControlBinop cBinop_2EuBfoS3;
  ControlBinop cBinop_Xc5thTWo;
  ControlBinop cBinop_4OpOCCBO;
  ControlVar cVar_WwKmJumd;
  ControlSlice cSlice_fycj9DH0;
  ControlSlice cSlice_OFonwLJL;
  ControlVar cVar_kw4Y3zR6;
  ControlVar cVar_xEescJU2;
  ControlVar cVar_TC9InOYO;
  ControlVar cVar_zc6Eyrov;
  ControlVar cVar_DI8ZKZ2H;
  ControlSlice cSlice_jW9YO008;
  ControlSlice cSlice_Fu4gydLB;
  ControlBinop cBinop_S3oylruG;
  ControlBinop cBinop_Z7rNXf3p;
  ControlBinop cBinop_2zHrLwnR;
  ControlBinop cBinop_AgtfjRLM;
  ControlBinop cBinop_s1mEvnme;
  ControlBinop cBinop_MT3iWDDa;
  ControlBinop cBinop_YRjUkg0h;
  ControlBinop cBinop_sT7rlI8p;
  ControlBinop cBinop_a20bM1PO;
  ControlBinop cBinop_VqkdPSuP;
  ControlBinop cBinop_YN8wyJ5m;
  ControlBinop cBinop_9SdfZCmY;
  ControlRandom cRandom_S6JljoU8;
  ControlSlice cSlice_PL8crhqS;
  ControlDelay cDelay_U8Xu12Pw;
  ControlVar cVar_gyHTRgwW;
  ControlBinop cBinop_T206xAou;
  ControlBinop cBinop_JZyzDSFr;
  ControlBinop cBinop_AgEFsytW;
  ControlTabhead cTabhead_r2G6CGXf;
  ControlVar cVar_JcyjQ9Xn;
  ControlDelay cDelay_JjWXGYms;
  ControlDelay cDelay_Wm2Q3dMb;
  ControlBinop cBinop_G1svjTPE;
  ControlBinop cBinop_00vxQLL9;
  ControlBinop cBinop_NDnpkYMa;
  ControlBinop cBinop_FOGYJ5Um;
  ControlBinop cBinop_8BG1tUhw;
  ControlBinop cBinop_xwAdx1wT;
  ControlBinop cBinop_JFjtwhzz;
  ControlBinop cBinop_jUyzpAYe;
};

#endif // _HEAVY_CONTEXT_UNTITLED_HPP_
