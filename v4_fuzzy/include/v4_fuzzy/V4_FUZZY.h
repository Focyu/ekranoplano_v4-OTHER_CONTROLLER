/*
 * V4_FUZZY.h
 *
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * Code generation for model "V4_FUZZY".
 *
 * Model version              : 12.185
 * Simulink Coder version : 26.1 (R2026a) 20-Nov-2025
 * C++ source code generated on : Wed May 20 13:23:22 2026
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef V4_FUZZY_H_
#define V4_FUZZY_H_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "V4_FUZZY_types.h"
#include "rmw/qos_profiles.h"

extern "C"
{

#include "rt_nonfinite.h"

}

#include <string.h>

extern "C"
{

#include "rtGetInf.h"

}

extern "C"
{

#include "rtGetNaN.h"

}

#include <stddef.h>
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Block signals for system '<Root>/MATLAB Function-ned_to_PoseStamped' */
struct B_MATLABFunctionned_to_PoseSt_T {
  uint32_T lengthOut;          /* '<Root>/MATLAB Function-ned_to_PoseStamped' */
  uint8_T stringOut[128];      /* '<Root>/MATLAB Function-ned_to_PoseStamped' */
};

/* Block signals for system '<S155>/Enabled Subsystem' */
struct B_EnabledSubsystem_V4_FUZZY_T {
  SL_Bus_std_msgs_Bool In1;            /* '<S198>/In1' */
};

/* Block signals for system '<S156>/Enabled Subsystem' */
struct B_EnabledSubsystem_V4_FUZZY_d_T {
  SL_Bus_std_msgs_Float64 In1;         /* '<S199>/In1' */
};

/* Block signals (default storage) */
struct B_V4_FUZZY_T {
  real_T outputMFCache[505];
  SL_Bus_nav_msgs_Odometry BusAssignmentODOM;/* '<Root>/Bus Assignment-ODOM' */
  real_T aggregatedOutputs_b[101];     /* '<S8>/Evaluate Rule Consequents' */
  real_T dv[101];
  real_T dv1[101];
  real_T dv2[101];
  real_T dv3[101];
  real_T dv4[101];
  SL_Bus_sensor_msgs_Imu BusAssignmentIMU;/* '<Root>/Bus Assignment-IMU' */
  SL_Bus_gazebo_msgs_SetEntityStateRequest BusAssignmentMODELO;/* '<Root>/Bus Assignment-MODELO' */
  SL_Bus_geometry_msgs_PoseStamped BusAssignmentPoseStaamped;
                                      /* '<Root>/Bus Assignment-PoseStaamped' */
  SL_Bus_geometry_msgs_TwistStamped BusAssignmentTwistStamped;
                                      /* '<Root>/Bus Assignment-TwistStamped' */
  uint8_T str_out[128];                /* '<Root>/MATLAB Function-Odometry1' */
  uint8_T str_out_c[128];              /* '<Root>/MATLAB Function-Odometry' */
  uint8_T stringOut[128];              /* '<Root>/MATLAB Function-MODELO' */
  uint8_T stringOut_c[128];            /* '<Root>/MATLAB Function-IMU2' */
  uint8_T stringOut_b[128];            /* '<Root>/MATLAB Function' */
  real_T antecedentOutputs[15];        /* '<S9>/Evaluate Rule Antecedents' */
  real_T IC[12];                       /* '<S30>/IC' */
  real_T x[12];                        /* '<S30>/Integrator' */
  real_T R[9];
  real_T RotationAnglestoDirectionCo[9];
                        /* '<S30>/Rotation Angles to Direction Cosine Matrix' */
  real_T dv5[9];
  real_T inputMFCache[8];
  real_T antecedentOutputs_l[8];       /* '<S8>/Evaluate Rule Antecedents' */
  char_T b_zeroDelimTopic[26];
  char_T b_zeroDelimTopic_m[25];
  char_T b_zeroDelimTopic_c[25];
  real_T Switch_p[3];                  /* '<S30>/Switch' */
  real_T Switch1[3];                   /* '<S30>/Switch1' */
  real_T wbe_b[3];
  real_T FE1_b[3];
  real_T F_b[3];
  real_T Product_k[3];                 /* '<S191>/Product' */
  real_T Dtot[3];
  char_T b_zeroDelimTopic_cx[22];
  char_T b_zeroDelimTopic_b[22];
  char_T b_zeroDelimTopic_p[18];
  char_T b_zeroDelimTopic_cv[17];
  char_T b_zeroDelimTopic_f[17];
  char_T b_zeroDelimTopic_g[17];
  sJ4ih70VmKcvCeguWN0mNVF deadline;
  sJ4ih70VmKcvCeguWN0mNVF deadline_g;
  sJ4ih70VmKcvCeguWN0mNVF deadline_m;
  sJ4ih70VmKcvCeguWN0mNVF deadline_n;
  sJ4ih70VmKcvCeguWN0mNVF deadline_p;
  sJ4ih70VmKcvCeguWN0mNVF deadline_l;
  sJ4ih70VmKcvCeguWN0mNVF deadline_j;
  sJ4ih70VmKcvCeguWN0mNVF deadline_d;
  sJ4ih70VmKcvCeguWN0mNVF deadline_gu;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF deadline_ld;
  sJ4ih70VmKcvCeguWN0mNVF lifespan_d;
  sJ4ih70VmKcvCeguWN0mNVF deadline_dy;
  real_T Lv[2];                        /* '<S167>/Lv' */
  real_T dv6[2];
  real_T Switch2_l[2];                 /* '<Root>/Switch2' */
  real_T GainZ;                        /* '<Root>/Gain-Z' */
  real_T FilterCoefficient;            /* '<S135>/Filter Coefficient' */
  real_T Saturation;                   /* '<S139>/Saturation' */
  real_T Switch3[2];                   /* '<Root>/Switch3' */
  real_T RateLimiter2[2];              /* '<Root>/Rate Limiter2' */
  real_T Saturation_roll_sp;           /* '<Root>/Saturation_roll_sp' */
  real_T RateLimitertheta_sp;          /* '<Root>/Rate Limiter-theta_sp' */
  real_T FilterCoefficient_a;          /* '<S81>/Filter Coefficient' */
  real_T Saturation_d;                 /* '<S85>/Saturation' */
  real_T Switch;                       /* '<S68>/Switch' */
  real_T Switch_f;                     /* '<S122>/Switch' */
  real_T Memory[3];                    /* '<S30>/Memory' */
  real_T Memory1[3];                   /* '<S30>/Memory1' */
  real_T Product[4];                   /* '<S170>/Product' */
  real_T Switch2;                      /* '<S30>/Switch2' */
  real_T Output;                       /* '<S150>/Output' */
  real_T Switch3_l;                    /* '<S30>/Switch3' */
  real_T Power;                        /* '<S30>/Product2' */
  real_T Gain3;                        /* '<S30>/Gain3' */
  real_T EnergykWh;                    /* '<S30>/Gain1' */
  real_T powerdemand;                  /* '<S30>/Divide' */
  real_T loadtorque;                   /* '<S30>/Divide1' */
  real_T SumOLA[3];                    /* '<S30>/SumOLA' */
  real_T SumOLA1[3];                   /* '<S30>/SumOLA1' */
  real_T XDOT[40];                     /* '<S30>/MATLAB Function - MODEL' */
  real_T w[2];                         /* '<S176>/w' */
  real_T w_a[2];                       /* '<S176>/w ' */
  real_T LwgV1[2];                     /* '<S176>/Lwg//V 1' */
  real_T w_g[2];                       /* '<S175>/w' */
  real_T w_e[2];                       /* '<S175>/w ' */
  real_T w1[2];                        /* '<S175>/w 1' */
  real_T w_n[2];                       /* '<S174>/w' */
  real_T w1_c[2];                      /* '<S174>/w1' */
  real_T w_d[2];                       /* '<S173>/w' */
  real_T w_e0[2];                      /* '<S172>/w' */
  real_T UnaryMinus[2];                /* '<S172>/Unary Minus' */
  real_T w_o[2];                       /* '<S171>/w' */
  real_T sigma_w[2];                   /* '<S171>/sigma_w' */
  real_T defuzzifiedOutputs;           /* '<S9>/Defuzzify Outputs' */
  real_T defuzzifiedOutputs_o;         /* '<S8>/Defuzzify Outputs' */
  real_T defuzzifiedOutputs_k;         /* '<S7>/Defuzzify Outputs' */
  real_T u1;
  real_T u2;
  real_T q_aero;
  real_T Q;
  real_T CD_ih_IGE;
  real_T u2_deg;
  real_T Ltot;
  real_T CQ;
  real_T Cl;
  real_T Vd1;
  real_T L_dec;
  real_T c_phi;
  real_T s_phi;
  real_T c_the;
  real_T s_the;
  real_T c_psi;
  real_T s_psi;
  real_T sina;
  real_T sinb;
  real_T sinc;
  real_T cosa;
  real_T cosb;
  real_T cosc;
  real_T FE_b;
  real_T Mcg_b_idx_0;
  real_T FE2_b_idx_0;
  real_T FE2_b_idx_2;
  real_T Fg_b_idx_2;
  real_T rtb_CoordinateTransformationC_o;
  real_T rtb_CoordinateTransformationC_b;
  real_T rtb_CoordinateTransformationC_n;
  real_T FE_b_idx_0;
  real_T FA_b_idx_0;
  real_T FA_b_idx_1;
  real_T FA_b_idx_2;
  real_T R_tmp;
  real_T R_tmp_b;
  real_T a;
  real_T b;
  SL_Bus_std_msgs_Float64 SourceBlock_o2_k;/* '<S157>/SourceBlock' */
  SL_Bus_std_msgs_Float64 SourceBlock_o2_p;/* '<S156>/SourceBlock' */
  SL_Bus_std_msgs_Float64 SourceBlock_o2;/* '<S32>/SourceBlock' */
  SL_Bus_std_msgs_Float64 SourceBlock_o2_d;/* '<S31>/SourceBlock' */
  uint32_T bpIndex[2];
  uint32_T currentLen;                 /* '<Root>/MATLAB Function-Odometry1' */
  uint32_T receivedLen;                /* '<Root>/MATLAB Function-Odometry1' */
  uint32_T currentLen_l;               /* '<Root>/MATLAB Function-Odometry' */
  uint32_T receivedLen_b;              /* '<Root>/MATLAB Function-Odometry' */
  uint32_T lengthOut;                  /* '<Root>/MATLAB Function-MODELO' */
  uint32_T lengthOut_o;                /* '<Root>/MATLAB Function-IMU2' */
  uint32_T lengthOut_l;                /* '<Root>/MATLAB Function' */
  boolean_T Compare;                   /* '<S151>/Compare' */
  boolean_T AND3;                      /* '<S68>/AND3' */
  boolean_T Memory_k;                  /* '<S68>/Memory' */
  boolean_T AND3_p;                    /* '<S122>/AND3' */
  boolean_T Memory_h;                  /* '<S122>/Memory' */
  boolean_T SourceBlock_o1;            /* '<S32>/SourceBlock' */
  boolean_T SourceBlock_o1_o;          /* '<S31>/SourceBlock' */
  boolean_T SourceBlock_o1_h;          /* '<S158>/SourceBlock' */
  boolean_T SourceBlock_o1_d;          /* '<S157>/SourceBlock' */
  boolean_T SourceBlock_o1_c;          /* '<S156>/SourceBlock' */
  boolean_T SourceBlock_o1_k;          /* '<S155>/SourceBlock' */
  B_EnabledSubsystem_V4_FUZZY_d_T EnabledSubsystem_a;/* '<S32>/Enabled Subsystem' */
  B_EnabledSubsystem_V4_FUZZY_d_T EnabledSubsystem_b;/* '<S31>/Enabled Subsystem' */
  B_EnabledSubsystem_V4_FUZZY_T EnabledSubsystem_pt;/* '<S158>/Enabled Subsystem' */
  B_EnabledSubsystem_V4_FUZZY_d_T EnabledSubsystem_p;/* '<S157>/Enabled Subsystem' */
  B_EnabledSubsystem_V4_FUZZY_d_T EnabledSubsystem_k;/* '<S156>/Enabled Subsystem' */
  B_EnabledSubsystem_V4_FUZZY_T EnabledSubsystem;/* '<S155>/Enabled Subsystem' */
  B_MATLABFunctionned_to_PoseSt_T sf_MATLABFunctionned_to_TwistSt;
                              /* '<Root>/MATLAB Function-ned_to_TwistStamped' */
  B_MATLABFunctionned_to_PoseSt_T sf_MATLABFunctionned_to_PoseS_b;
                               /* '<Root>/MATLAB Function-ned_to_PoseStamped' */
};

/* Block states (default storage) for system '<Root>' */
struct DW_V4_FUZZY_T {
  ros_slros2_internal_block_Ser_T obj; /* '<S6>/ServiceCaller' */
  ros_slros2_internal_block_Pub_T obj_d;/* '<S28>/SinkBlock' */
  ros_slros2_internal_block_Pub_T obj_dl;/* '<S27>/SinkBlock' */
  ros_slros2_internal_block_Pub_T obj_o;/* '<S26>/SinkBlock' */
  ros_slros2_internal_block_Pub_T obj_i;/* '<S25>/SinkBlock' */
  ros_slros2_internal_block_Sub_T obj_k;/* '<S32>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_m;/* '<S31>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_h;/* '<S158>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_p;/* '<S157>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_hy;/* '<S156>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_h4;/* '<S155>/SourceBlock' */
  real_T UnitDelay3_DSTATE[2];         /* '<Root>/Unit Delay3' */
  real_T UnitDelay2_DSTATE[2];         /* '<Root>/Unit Delay2' */
  real_T Memory2_PreviousInput[12];    /* '<S30>/Memory2' */
  real_T PrevY[2];                     /* '<Root>/Rate Limiter2' */
  real_T PrevY_g;                      /* '<Root>/Rate Limiter-theta_sp' */
  real_T LastMajorTime;                /* '<Root>/Rate Limiter-theta_sp' */
  real_T Memory_PreviousInput[3];      /* '<S30>/Memory' */
  real_T Memory1_PreviousInput[3];     /* '<S30>/Memory1' */
  real_T NextOutput[4];                /* '<S170>/White Noise' */
  real_T NextOutput_k;                 /* '<S150>/White Noise' */
  uint32_T PreLookUpIndexSearchprobofexcee;
                        /* '<S177>/PreLook-Up Index Search  (prob of exceed)' */
  uint32_T PreLookUpIndexSearchaltitude_DW;
                              /* '<S177>/PreLook-Up Index Search  (altitude)' */
  uint32_T RandSeed[4];                /* '<S170>/White Noise' */
  uint32_T RandSeed_a;                 /* '<S150>/White Noise' */
  robotics_slcore_internal_bloc_T obj_c;
                             /* '<Root>/Coordinate Transformation Conversion' */
  int8_T ifHeightMaxlowaltitudeelseifHei;
  /* '<S166>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  int8_T ifHeightMaxlowaltitudeelseifH_a;
  /* '<S165>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  boolean_T IC_FirstOutputTime;        /* '<S30>/IC' */
  boolean_T Integrator_DWORK1;         /* '<S30>/Integrator' */
  boolean_T PrevLimited;               /* '<Root>/Rate Limiter-theta_sp' */
  boolean_T Memory_PreviousInput_f;    /* '<S68>/Memory' */
  boolean_T Memory_PreviousInput_k;    /* '<S122>/Memory' */
  boolean_T objisempty;                /* '<S32>/SourceBlock' */
  boolean_T objisempty_g;              /* '<S31>/SourceBlock' */
  boolean_T objisempty_a;              /* '<S158>/SourceBlock' */
  boolean_T objisempty_e;              /* '<S157>/SourceBlock' */
  boolean_T objisempty_l;              /* '<S156>/SourceBlock' */
  boolean_T objisempty_c;              /* '<S155>/SourceBlock' */
  boolean_T objisempty_cr;             /* '<S28>/SinkBlock' */
  boolean_T objisempty_ld;             /* '<S27>/SinkBlock' */
  boolean_T objisempty_ev;             /* '<S26>/SinkBlock' */
  boolean_T objisempty_m;              /* '<S25>/SinkBlock' */
  boolean_T objisempty_d;    /* '<Root>/Coordinate Transformation Conversion' */
  boolean_T fctrlobj_not_empty;
                             /* '<Root>/Coordinate Transformation Conversion' */
  boolean_T objisempty_f;              /* '<S6>/ServiceCaller' */
  e_robotics_core_internal_vali_T fctrlobj;
                             /* '<Root>/Coordinate Transformation Conversion' */
  boolean_T Hwgws_MODE;                /* '<S161>/Hwgw(s)' */
  boolean_T Hvgws_MODE;                /* '<S161>/Hvgw(s)' */
  boolean_T Hugws_MODE;                /* '<S161>/Hugw(s)' */
  boolean_T Hrgw_MODE;                 /* '<S160>/Hrgw' */
  boolean_T Hqgw_MODE;                 /* '<S160>/Hqgw' */
  boolean_T Hpgw_MODE;                 /* '<S160>/Hpgw' */
};

/* Continuous states (default storage) */
struct X_V4_FUZZY_T {
  real_T Integrator_CSTATE[12];        /* '<S30>/Integrator' */
  real_T Integrator_CSTATE_d;          /* '<S132>/Integrator' */
  real_T Filter_CSTATE;                /* '<S127>/Filter' */
  real_T Integrator_CSTATE_e;          /* '<S78>/Integrator' */
  real_T Filter_CSTATE_f;              /* '<S73>/Filter' */
  real_T TransferFcn_CSTATE[2];        /* '<S30>/Transfer Fcn' */
  real_T TransferFcn1_CSTATE;          /* '<S30>/Transfer Fcn1' */
  real_T Integrator1_CSTATE;           /* '<S30>/Integrator1' */
  real_T wg_p1_CSTATE[2];              /* '<S176>/wg_p1' */
  real_T wg_p2_CSTATE[2];              /* '<S176>/wg_p2' */
  real_T vg_p1_CSTATE[2];              /* '<S175>/vg_p1' */
  real_T vgw_p2_CSTATE[2];             /* '<S175>/vgw_p2' */
  real_T ug_p_CSTATE[2];               /* '<S174>/ug_p' */
  real_T rgw_p_CSTATE[2];              /* '<S173>/rgw_p' */
  real_T qgw_p_CSTATE[2];              /* '<S172>/qgw_p' */
  real_T pgw_p_CSTATE[2];              /* '<S171>/pgw_p' */
};

/* State derivatives (default storage) */
struct XDot_V4_FUZZY_T {
  real_T Integrator_CSTATE[12];        /* '<S30>/Integrator' */
  real_T Integrator_CSTATE_d;          /* '<S132>/Integrator' */
  real_T Filter_CSTATE;                /* '<S127>/Filter' */
  real_T Integrator_CSTATE_e;          /* '<S78>/Integrator' */
  real_T Filter_CSTATE_f;              /* '<S73>/Filter' */
  real_T TransferFcn_CSTATE[2];        /* '<S30>/Transfer Fcn' */
  real_T TransferFcn1_CSTATE;          /* '<S30>/Transfer Fcn1' */
  real_T Integrator1_CSTATE;           /* '<S30>/Integrator1' */
  real_T wg_p1_CSTATE[2];              /* '<S176>/wg_p1' */
  real_T wg_p2_CSTATE[2];              /* '<S176>/wg_p2' */
  real_T vg_p1_CSTATE[2];              /* '<S175>/vg_p1' */
  real_T vgw_p2_CSTATE[2];             /* '<S175>/vgw_p2' */
  real_T ug_p_CSTATE[2];               /* '<S174>/ug_p' */
  real_T rgw_p_CSTATE[2];              /* '<S173>/rgw_p' */
  real_T qgw_p_CSTATE[2];              /* '<S172>/qgw_p' */
  real_T pgw_p_CSTATE[2];              /* '<S171>/pgw_p' */
};

/* State disabled  */
struct XDis_V4_FUZZY_T {
  boolean_T Integrator_CSTATE[12];     /* '<S30>/Integrator' */
  boolean_T Integrator_CSTATE_d;       /* '<S132>/Integrator' */
  boolean_T Filter_CSTATE;             /* '<S127>/Filter' */
  boolean_T Integrator_CSTATE_e;       /* '<S78>/Integrator' */
  boolean_T Filter_CSTATE_f;           /* '<S73>/Filter' */
  boolean_T TransferFcn_CSTATE[2];     /* '<S30>/Transfer Fcn' */
  boolean_T TransferFcn1_CSTATE;       /* '<S30>/Transfer Fcn1' */
  boolean_T Integrator1_CSTATE;        /* '<S30>/Integrator1' */
  boolean_T wg_p1_CSTATE[2];           /* '<S176>/wg_p1' */
  boolean_T wg_p2_CSTATE[2];           /* '<S176>/wg_p2' */
  boolean_T vg_p1_CSTATE[2];           /* '<S175>/vg_p1' */
  boolean_T vgw_p2_CSTATE[2];          /* '<S175>/vgw_p2' */
  boolean_T ug_p_CSTATE[2];            /* '<S174>/ug_p' */
  boolean_T rgw_p_CSTATE[2];           /* '<S173>/rgw_p' */
  boolean_T qgw_p_CSTATE[2];           /* '<S172>/qgw_p' */
  boolean_T pgw_p_CSTATE[2];           /* '<S171>/pgw_p' */
};

/* Zero-crossing (trigger) state */
struct PrevZCX_V4_FUZZY_T {
  ZCSigState Integrator_Reset_ZCE;     /* '<S30>/Integrator' */
};

/* Invariant block signals (default storage) */
struct ConstB_V4_FUZZY_T {
  real_T UnitConversion;               /* '<S159>/Unit Conversion' */
  real_T UnitConversion_k;             /* '<S169>/Unit Conversion' */
  real_T sigma_wg;                     /* '<S178>/sigma_wg ' */
  real_T UnitConversion_n;             /* '<S163>/Unit Conversion' */
  real_T UnitConversion_c;             /* '<S197>/Unit Conversion' */
  real_T PreLookUpIndexSearchprobofe;
                        /* '<S177>/PreLook-Up Index Search  (prob of exceed)' */
  real_T constBlockForifHeightMaxlow;
  real_T constBlockForifHeightMaxl_e;
  real_T constBlockForifHeightMaxl_l;
  real_T constBlockForifHeightMax_eh;
  real_T Sqrt[4];                      /* '<S170>/Sqrt' */
  real_T Sqrt1;                        /* '<S170>/Sqrt1' */
  real_T Divide[4];                    /* '<S170>/Divide' */
  real_T motorspeed;                   /* '<S30>/Gain2' */
  real_T Sum;                          /* '<S187>/Sum' */
  real_T Sum_a;                        /* '<S179>/Sum' */
  real_T sqrt_a;                       /* '<S176>/sqrt' */
  real_T w4;                           /* '<S171>/w4' */
  real_T u16;                          /* '<S171>/u^1//6' */
  uint32_T PreLookUpIndexSearchprobo_g;
                        /* '<S177>/PreLook-Up Index Search  (prob of exceed)' */
};

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
struct ODE4_IntgData {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
};

#endif

/* Constant parameters (default storage) */
struct ConstP_V4_FUZZY_T {
  /* Expression: fis.outputSamplePoints
   * Referenced by: '<S7>/Output Sample Points'
   */
  real_T OutputSamplePoints_Value[101];

  /* Expression: fis.outputSamplePoints
   * Referenced by: '<S8>/Output Sample Points'
   */
  real_T OutputSamplePoints_Value_c[101];

  /* Expression: fis.outputSamplePoints
   * Referenced by: '<S9>/Output Sample Points'
   */
  real_T OutputSamplePoints_Value_o[101];

  /* Pooled Parameter (Expression: x_nom)
   * Referenced by:
   *   '<S30>/IC'
   *   '<S30>/Memory2'
   */
  real_T pooled11[12];

  /* Expression: h_vec
   * Referenced by: '<S177>/PreLook-Up Index Search  (altitude)'
   */
  real_T PreLookUpIndexSearchaltitude_Br[12];

  /* Expression: sigma_vec'
   * Referenced by: '<S177>/Medium//High Altitude Intensity'
   */
  real_T MediumHighAltitudeIntensity_Tab[84];

  /* Computed Parameter: MediumHighAltitudeIntensity_max
   * Referenced by: '<S177>/Medium//High Altitude Intensity'
   */
  uint32_T MediumHighAltitudeIntensity_max[2];
};

/* Real-time Model Data Structure */
struct tag_RTM_V4_FUZZY_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_V4_FUZZY_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_V4_FUZZY_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[36];
  real_T odeF[4][36];
  ODE4_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    boolean_T firstInitCondFlag;
    struct {
      uint8_T TID[2];
    } TaskCounters;

    time_T tStart;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

extern const ConstB_V4_FUZZY_T V4_FUZZY_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_V4_FUZZY_T V4_FUZZY_ConstP;

/* Class declaration for model V4_FUZZY */
class V4_FUZZY
{
  /* public data and function members */
 public:
  /* Real-Time Model get method */
  RT_MODEL_V4_FUZZY_T * getRTM();
  void ModelPrevZCStateInit();

  /* model start function */
  void start();

  /* Initial conditions function */
  void initialize();

  /* model step function */
  void step();

  /* model terminate function */
  void terminate();

  /* Constructor */
  V4_FUZZY();

  /* Destructor */
  ~V4_FUZZY();

  /* private data and function members */
 private:
  /* Block signals */
  B_V4_FUZZY_T V4_FUZZY_B;

  /* Block states */
  DW_V4_FUZZY_T V4_FUZZY_DW;

  /* Block continuous states */
  X_V4_FUZZY_T V4_FUZZY_X;

  /* Block Continuous state disabled vector */
  XDis_V4_FUZZY_T V4_FUZZY_XDis;

  /* Triggered events */
  PrevZCX_V4_FUZZY_T V4_FUZZY_PrevZCX;

  /* private member function(s) for subsystem '<Root>/MATLAB Function-ned_to_PoseStamped'*/
  static void MATLABFunctionned_to_PoseStampe(B_MATLABFunctionned_to_PoseSt_T
    *localB);

  /* private member function(s) for subsystem '<S155>/Enabled Subsystem'*/
  static void V4_FUZZY_EnabledSubsystem_Init(B_EnabledSubsystem_V4_FUZZY_T
    *localB);
  static void V4_FUZZY_EnabledSubsystem(boolean_T rtu_Enable, const
    SL_Bus_std_msgs_Bool *rtu_In1, B_EnabledSubsystem_V4_FUZZY_T *localB);

  /* private member function(s) for subsystem '<S156>/Enabled Subsystem'*/
  static void V4_FUZZ_EnabledSubsystem_p_Init(B_EnabledSubsystem_V4_FUZZY_d_T
    *localB);
  static void V4_FUZZY_EnabledSubsystem_k(boolean_T rtu_Enable, const
    SL_Bus_std_msgs_Float64 *rtu_In1, B_EnabledSubsystem_V4_FUZZY_d_T *localB);

  /* private member function(s) for subsystem '<Root>'*/
  void V4_FUZZY_Publisher_setupImpl(const ros_slros2_internal_block_Pub_T *obj);
  rmw_qos_profile_t V4_ROS2PubSubBase_setQOSProfile(rmw_qos_profile_t rmwProfile,
    real_T qosDepth, real_T qosDeadline, real_T qosLifespan, real_T
    qosLeaseDuration, boolean_T qosAvoidROSNamespaceConventions);
  void V4_FUZZY_Publisher_setupImpl_l(const ros_slros2_internal_block_Pub_T *obj);
  void V4_FUZZY_Publisher_setupImpl_lz(const ros_slros2_internal_block_Pub_T
    *obj);
  void V4_FU_Subscriber_setupImpl_lziw(const ros_slros2_internal_block_Sub_T
    *obj);
  void V4_F_Subscriber_setupImpl_lziwo(const ros_slros2_internal_block_Sub_T
    *obj);
  void V4_FUZZ_ServiceCaller_setupImpl(ros_slros2_internal_block_Ser_T *obj);
  void V4_FUZ_Subscriber_setupImpl_lzi(const ros_slros2_internal_block_Sub_T
    *obj);
  void V4_FUZZY_Subscriber_setupImpl(const ros_slros2_internal_block_Sub_T *obj);
  void V4_FUZZY_Subscriber_setupImpl_l(const ros_slros2_internal_block_Sub_T
    *obj);
  void V4_FUZZ_Subscriber_setupImpl_lz(const ros_slros2_internal_block_Sub_T
    *obj);
  real_T V4_FUZZY_trimf(real_T x, const real_T params[3]);
  void V4_FUZZY_trimf_l(const real_T x[101], const real_T params[3], real_T y
                        [101]);

  /* Global mass matrix */

  /* Continuous states update member function*/
  void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si );

  /* Derivatives member function */
  void V4_FUZZY_derivatives();

  /* Real-Time Model */
  RT_MODEL_V4_FUZZY_T V4_FUZZY_M;
};

extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Display' : Unused code path elimination
 * Block '<S30>/Gain4' : Unused code path elimination
 * Block '<S7>/InputConversion' : Eliminate redundant data type conversion
 * Block '<S8>/InputConversion' : Eliminate redundant data type conversion
 * Block '<S9>/InputConversion' : Eliminate redundant data type conversion
 * Block '<Root>/Manual Switch' : Eliminated due to constant selection input
 * Block '<S152>/Cast' : Eliminate redundant data type conversion
 * Block '<S152>/Cast To Double' : Eliminate redundant data type conversion
 * Block '<S152>/Cast To Double1' : Eliminate redundant data type conversion
 * Block '<S152>/Cast To Double2' : Eliminate redundant data type conversion
 * Block '<S152>/Cast To Double3' : Eliminate redundant data type conversion
 * Block '<S152>/Cast To Double4' : Eliminate redundant data type conversion
 * Block '<S183>/Reshape' : Reshape block reduction
 * Block '<S183>/Reshape1' : Reshape block reduction
 * Block '<S185>/Reshape' : Reshape block reduction
 * Block '<S191>/Reshape' : Reshape block reduction
 * Block '<S191>/Reshape1' : Reshape block reduction
 * Block '<S193>/Reshape' : Reshape block reduction
 * Block '<S29>/Clock' : Unused code path elimination
 * Block '<S29>/Constant' : Unused code path elimination
 * Block '<S29>/Constant1' : Unused code path elimination
 * Block '<S29>/Output' : Unused code path elimination
 * Block '<S29>/Product' : Unused code path elimination
 * Block '<S29>/Step' : Unused code path elimination
 * Block '<S29>/Sum' : Unused code path elimination
 * Block '<Root>/Saturation1' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'V4_FUZZY'
 * '<S1>'   : 'V4_FUZZY/Blank Message'
 * '<S2>'   : 'V4_FUZZY/Blank Message1'
 * '<S3>'   : 'V4_FUZZY/Blank Message2'
 * '<S4>'   : 'V4_FUZZY/Blank Message3'
 * '<S5>'   : 'V4_FUZZY/Blank Message4'
 * '<S6>'   : 'V4_FUZZY/Call Service'
 * '<S7>'   : 'V4_FUZZY/Fuzzy Logic  Controller - Aleron'
 * '<S8>'   : 'V4_FUZZY/Fuzzy Logic  Controller - Altura'
 * '<S9>'   : 'V4_FUZZY/Fuzzy Logic  Controller - Timon'
 * '<S10>'  : 'V4_FUZZY/MATLAB Function'
 * '<S11>'  : 'V4_FUZZY/MATLAB Function-IMU2'
 * '<S12>'  : 'V4_FUZZY/MATLAB Function-MODELO'
 * '<S13>'  : 'V4_FUZZY/MATLAB Function-Odometry'
 * '<S14>'  : 'V4_FUZZY/MATLAB Function-Odometry1'
 * '<S15>'  : 'V4_FUZZY/MATLAB Function-clocl'
 * '<S16>'  : 'V4_FUZZY/MATLAB Function-ned_to_PoseStaamped'
 * '<S17>'  : 'V4_FUZZY/MATLAB Function-ned_to_PoseStamped'
 * '<S18>'  : 'V4_FUZZY/MATLAB Function-ned_to_TwistStamped'
 * '<S19>'  : 'V4_FUZZY/MATLAB Function-ned_to_gazebo'
 * '<S20>'  : 'V4_FUZZY/MATLAB Function-ned_to_imu'
 * '<S21>'  : 'V4_FUZZY/MATLAB Function-ned_to_odometry'
 * '<S22>'  : 'V4_FUZZY/MATLAB Function2'
 * '<S23>'  : 'V4_FUZZY/PID ELEVATOR'
 * '<S24>'  : 'V4_FUZZY/PID VELOCIDAD'
 * '<S25>'  : 'V4_FUZZY/Publish'
 * '<S26>'  : 'V4_FUZZY/Publish1'
 * '<S27>'  : 'V4_FUZZY/Publish2'
 * '<S28>'  : 'V4_FUZZY/Publish3'
 * '<S29>'  : 'V4_FUZZY/Ramp'
 * '<S30>'  : 'V4_FUZZY/SUBSYSTEM_MODEL'
 * '<S31>'  : 'V4_FUZZY/Subscribe-ALTURA'
 * '<S32>'  : 'V4_FUZZY/Subscribe-YAW'
 * '<S33>'  : 'V4_FUZZY/Fuzzy Logic  Controller - Aleron/Defuzzify Outputs'
 * '<S34>'  : 'V4_FUZZY/Fuzzy Logic  Controller - Aleron/Evaluate Rule Antecedents'
 * '<S35>'  : 'V4_FUZZY/Fuzzy Logic  Controller - Aleron/Evaluate Rule Consequents'
 * '<S36>'  : 'V4_FUZZY/Fuzzy Logic  Controller - Altura/Defuzzify Outputs'
 * '<S37>'  : 'V4_FUZZY/Fuzzy Logic  Controller - Altura/Evaluate Rule Antecedents'
 * '<S38>'  : 'V4_FUZZY/Fuzzy Logic  Controller - Altura/Evaluate Rule Consequents'
 * '<S39>'  : 'V4_FUZZY/Fuzzy Logic  Controller - Timon/Defuzzify Outputs'
 * '<S40>'  : 'V4_FUZZY/Fuzzy Logic  Controller - Timon/Evaluate Rule Antecedents'
 * '<S41>'  : 'V4_FUZZY/Fuzzy Logic  Controller - Timon/Evaluate Rule Consequents'
 * '<S42>'  : 'V4_FUZZY/PID ELEVATOR/Anti-windup'
 * '<S43>'  : 'V4_FUZZY/PID ELEVATOR/D Gain'
 * '<S44>'  : 'V4_FUZZY/PID ELEVATOR/External Derivative'
 * '<S45>'  : 'V4_FUZZY/PID ELEVATOR/Filter'
 * '<S46>'  : 'V4_FUZZY/PID ELEVATOR/Filter ICs'
 * '<S47>'  : 'V4_FUZZY/PID ELEVATOR/I Gain'
 * '<S48>'  : 'V4_FUZZY/PID ELEVATOR/Ideal P Gain'
 * '<S49>'  : 'V4_FUZZY/PID ELEVATOR/Ideal P Gain Fdbk'
 * '<S50>'  : 'V4_FUZZY/PID ELEVATOR/Integrator'
 * '<S51>'  : 'V4_FUZZY/PID ELEVATOR/Integrator ICs'
 * '<S52>'  : 'V4_FUZZY/PID ELEVATOR/N Copy'
 * '<S53>'  : 'V4_FUZZY/PID ELEVATOR/N Gain'
 * '<S54>'  : 'V4_FUZZY/PID ELEVATOR/P Copy'
 * '<S55>'  : 'V4_FUZZY/PID ELEVATOR/Parallel P Gain'
 * '<S56>'  : 'V4_FUZZY/PID ELEVATOR/Reset Signal'
 * '<S57>'  : 'V4_FUZZY/PID ELEVATOR/Saturation'
 * '<S58>'  : 'V4_FUZZY/PID ELEVATOR/Saturation Fdbk'
 * '<S59>'  : 'V4_FUZZY/PID ELEVATOR/Sum'
 * '<S60>'  : 'V4_FUZZY/PID ELEVATOR/Sum Fdbk'
 * '<S61>'  : 'V4_FUZZY/PID ELEVATOR/Tracking Mode'
 * '<S62>'  : 'V4_FUZZY/PID ELEVATOR/Tracking Mode Sum'
 * '<S63>'  : 'V4_FUZZY/PID ELEVATOR/Tsamp - Integral'
 * '<S64>'  : 'V4_FUZZY/PID ELEVATOR/Tsamp - Ngain'
 * '<S65>'  : 'V4_FUZZY/PID ELEVATOR/postSat Signal'
 * '<S66>'  : 'V4_FUZZY/PID ELEVATOR/preInt Signal'
 * '<S67>'  : 'V4_FUZZY/PID ELEVATOR/preSat Signal'
 * '<S68>'  : 'V4_FUZZY/PID ELEVATOR/Anti-windup/Cont. Clamping Parallel'
 * '<S69>'  : 'V4_FUZZY/PID ELEVATOR/Anti-windup/Cont. Clamping Parallel/Dead Zone'
 * '<S70>'  : 'V4_FUZZY/PID ELEVATOR/Anti-windup/Cont. Clamping Parallel/Dead Zone/Enabled'
 * '<S71>'  : 'V4_FUZZY/PID ELEVATOR/D Gain/Internal Parameters'
 * '<S72>'  : 'V4_FUZZY/PID ELEVATOR/External Derivative/Error'
 * '<S73>'  : 'V4_FUZZY/PID ELEVATOR/Filter/Cont. Filter'
 * '<S74>'  : 'V4_FUZZY/PID ELEVATOR/Filter ICs/Internal IC - Filter'
 * '<S75>'  : 'V4_FUZZY/PID ELEVATOR/I Gain/Internal Parameters'
 * '<S76>'  : 'V4_FUZZY/PID ELEVATOR/Ideal P Gain/Passthrough'
 * '<S77>'  : 'V4_FUZZY/PID ELEVATOR/Ideal P Gain Fdbk/Disabled'
 * '<S78>'  : 'V4_FUZZY/PID ELEVATOR/Integrator/Continuous'
 * '<S79>'  : 'V4_FUZZY/PID ELEVATOR/Integrator ICs/Internal IC'
 * '<S80>'  : 'V4_FUZZY/PID ELEVATOR/N Copy/Disabled'
 * '<S81>'  : 'V4_FUZZY/PID ELEVATOR/N Gain/Internal Parameters'
 * '<S82>'  : 'V4_FUZZY/PID ELEVATOR/P Copy/Disabled'
 * '<S83>'  : 'V4_FUZZY/PID ELEVATOR/Parallel P Gain/Internal Parameters'
 * '<S84>'  : 'V4_FUZZY/PID ELEVATOR/Reset Signal/Disabled'
 * '<S85>'  : 'V4_FUZZY/PID ELEVATOR/Saturation/Enabled'
 * '<S86>'  : 'V4_FUZZY/PID ELEVATOR/Saturation Fdbk/Disabled'
 * '<S87>'  : 'V4_FUZZY/PID ELEVATOR/Sum/Sum_PID'
 * '<S88>'  : 'V4_FUZZY/PID ELEVATOR/Sum Fdbk/Disabled'
 * '<S89>'  : 'V4_FUZZY/PID ELEVATOR/Tracking Mode/Disabled'
 * '<S90>'  : 'V4_FUZZY/PID ELEVATOR/Tracking Mode Sum/Passthrough'
 * '<S91>'  : 'V4_FUZZY/PID ELEVATOR/Tsamp - Integral/TsSignalSpecification'
 * '<S92>'  : 'V4_FUZZY/PID ELEVATOR/Tsamp - Ngain/Passthrough'
 * '<S93>'  : 'V4_FUZZY/PID ELEVATOR/postSat Signal/Forward_Path'
 * '<S94>'  : 'V4_FUZZY/PID ELEVATOR/preInt Signal/Internal PreInt'
 * '<S95>'  : 'V4_FUZZY/PID ELEVATOR/preSat Signal/Forward_Path'
 * '<S96>'  : 'V4_FUZZY/PID VELOCIDAD/Anti-windup'
 * '<S97>'  : 'V4_FUZZY/PID VELOCIDAD/D Gain'
 * '<S98>'  : 'V4_FUZZY/PID VELOCIDAD/External Derivative'
 * '<S99>'  : 'V4_FUZZY/PID VELOCIDAD/Filter'
 * '<S100>' : 'V4_FUZZY/PID VELOCIDAD/Filter ICs'
 * '<S101>' : 'V4_FUZZY/PID VELOCIDAD/I Gain'
 * '<S102>' : 'V4_FUZZY/PID VELOCIDAD/Ideal P Gain'
 * '<S103>' : 'V4_FUZZY/PID VELOCIDAD/Ideal P Gain Fdbk'
 * '<S104>' : 'V4_FUZZY/PID VELOCIDAD/Integrator'
 * '<S105>' : 'V4_FUZZY/PID VELOCIDAD/Integrator ICs'
 * '<S106>' : 'V4_FUZZY/PID VELOCIDAD/N Copy'
 * '<S107>' : 'V4_FUZZY/PID VELOCIDAD/N Gain'
 * '<S108>' : 'V4_FUZZY/PID VELOCIDAD/P Copy'
 * '<S109>' : 'V4_FUZZY/PID VELOCIDAD/Parallel P Gain'
 * '<S110>' : 'V4_FUZZY/PID VELOCIDAD/Reset Signal'
 * '<S111>' : 'V4_FUZZY/PID VELOCIDAD/Saturation'
 * '<S112>' : 'V4_FUZZY/PID VELOCIDAD/Saturation Fdbk'
 * '<S113>' : 'V4_FUZZY/PID VELOCIDAD/Sum'
 * '<S114>' : 'V4_FUZZY/PID VELOCIDAD/Sum Fdbk'
 * '<S115>' : 'V4_FUZZY/PID VELOCIDAD/Tracking Mode'
 * '<S116>' : 'V4_FUZZY/PID VELOCIDAD/Tracking Mode Sum'
 * '<S117>' : 'V4_FUZZY/PID VELOCIDAD/Tsamp - Integral'
 * '<S118>' : 'V4_FUZZY/PID VELOCIDAD/Tsamp - Ngain'
 * '<S119>' : 'V4_FUZZY/PID VELOCIDAD/postSat Signal'
 * '<S120>' : 'V4_FUZZY/PID VELOCIDAD/preInt Signal'
 * '<S121>' : 'V4_FUZZY/PID VELOCIDAD/preSat Signal'
 * '<S122>' : 'V4_FUZZY/PID VELOCIDAD/Anti-windup/Cont. Clamping Parallel'
 * '<S123>' : 'V4_FUZZY/PID VELOCIDAD/Anti-windup/Cont. Clamping Parallel/Dead Zone'
 * '<S124>' : 'V4_FUZZY/PID VELOCIDAD/Anti-windup/Cont. Clamping Parallel/Dead Zone/Enabled'
 * '<S125>' : 'V4_FUZZY/PID VELOCIDAD/D Gain/Internal Parameters'
 * '<S126>' : 'V4_FUZZY/PID VELOCIDAD/External Derivative/Error'
 * '<S127>' : 'V4_FUZZY/PID VELOCIDAD/Filter/Cont. Filter'
 * '<S128>' : 'V4_FUZZY/PID VELOCIDAD/Filter ICs/Internal IC - Filter'
 * '<S129>' : 'V4_FUZZY/PID VELOCIDAD/I Gain/Internal Parameters'
 * '<S130>' : 'V4_FUZZY/PID VELOCIDAD/Ideal P Gain/Passthrough'
 * '<S131>' : 'V4_FUZZY/PID VELOCIDAD/Ideal P Gain Fdbk/Disabled'
 * '<S132>' : 'V4_FUZZY/PID VELOCIDAD/Integrator/Continuous'
 * '<S133>' : 'V4_FUZZY/PID VELOCIDAD/Integrator ICs/Internal IC'
 * '<S134>' : 'V4_FUZZY/PID VELOCIDAD/N Copy/Disabled'
 * '<S135>' : 'V4_FUZZY/PID VELOCIDAD/N Gain/Internal Parameters'
 * '<S136>' : 'V4_FUZZY/PID VELOCIDAD/P Copy/Disabled'
 * '<S137>' : 'V4_FUZZY/PID VELOCIDAD/Parallel P Gain/Internal Parameters'
 * '<S138>' : 'V4_FUZZY/PID VELOCIDAD/Reset Signal/Disabled'
 * '<S139>' : 'V4_FUZZY/PID VELOCIDAD/Saturation/Enabled'
 * '<S140>' : 'V4_FUZZY/PID VELOCIDAD/Saturation Fdbk/Disabled'
 * '<S141>' : 'V4_FUZZY/PID VELOCIDAD/Sum/Sum_PID'
 * '<S142>' : 'V4_FUZZY/PID VELOCIDAD/Sum Fdbk/Disabled'
 * '<S143>' : 'V4_FUZZY/PID VELOCIDAD/Tracking Mode/Disabled'
 * '<S144>' : 'V4_FUZZY/PID VELOCIDAD/Tracking Mode Sum/Passthrough'
 * '<S145>' : 'V4_FUZZY/PID VELOCIDAD/Tsamp - Integral/TsSignalSpecification'
 * '<S146>' : 'V4_FUZZY/PID VELOCIDAD/Tsamp - Ngain/Passthrough'
 * '<S147>' : 'V4_FUZZY/PID VELOCIDAD/postSat Signal/Forward_Path'
 * '<S148>' : 'V4_FUZZY/PID VELOCIDAD/preInt Signal/Internal PreInt'
 * '<S149>' : 'V4_FUZZY/PID VELOCIDAD/preSat Signal/Forward_Path'
 * '<S150>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Band-Limited White Noise'
 * '<S151>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Compare To Constant'
 * '<S152>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))'
 * '<S153>' : 'V4_FUZZY/SUBSYSTEM_MODEL/MATLAB Function - MODEL'
 * '<S154>' : 'V4_FUZZY/SUBSYSTEM_MODEL/MATLAB Function-reset'
 * '<S155>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Subscribe1_TURBULENCIA1'
 * '<S156>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Subscribe_HEAVE'
 * '<S157>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Subscribe_RATE'
 * '<S158>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Subscribe_TURBULENCIA'
 * '<S159>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Angle Conversion'
 * '<S160>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on angular rates'
 * '<S161>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on velocities'
 * '<S162>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Length Conversion'
 * '<S163>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Length Conversion1'
 * '<S164>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/RMS turbulence  intensities'
 * '<S165>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates'
 * '<S166>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities'
 * '<S167>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Turbulence scale lengths'
 * '<S168>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Velocity Conversion'
 * '<S169>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Velocity Conversion2'
 * '<S170>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/White Noise'
 * '<S171>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on angular rates/Hpgw'
 * '<S172>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on angular rates/Hqgw'
 * '<S173>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on angular rates/Hrgw'
 * '<S174>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on velocities/Hugw(s)'
 * '<S175>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on velocities/Hvgw(s)'
 * '<S176>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on velocities/Hwgw(s)'
 * '<S177>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/RMS turbulence  intensities/High Altitude Intensity'
 * '<S178>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/RMS turbulence  intensities/Low Altitude Intensity'
 * '<S179>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Interpolate  rates'
 * '<S180>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Low altitude  rates'
 * '<S181>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Medium//High  altitude rates'
 * '<S182>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Merge Subsystems'
 * '<S183>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Interpolate  rates/wind to body transformation'
 * '<S184>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Interpolate  rates/wind to body transformation/convert to earth coords'
 * '<S185>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Low altitude  rates/wind to body transformation'
 * '<S186>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Low altitude  rates/wind to body transformation/convert to earth coords'
 * '<S187>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Interpolate  velocities'
 * '<S188>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Low altitude  velocities'
 * '<S189>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Medium//High  altitude velocities'
 * '<S190>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Merge Subsystems'
 * '<S191>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Interpolate  velocities/wind to body transformation'
 * '<S192>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Interpolate  velocities/wind to body transformation/convert to earth coords'
 * '<S193>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Low altitude  velocities/wind to body transformation'
 * '<S194>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Low altitude  velocities/wind to body transformation/convert to earth coords'
 * '<S195>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Turbulence scale lengths/Low altitude scale length'
 * '<S196>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Turbulence scale lengths/Medium//High altitude scale length'
 * '<S197>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Turbulence scale lengths/Medium//High altitude scale length/Length Conversion'
 * '<S198>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Subscribe1_TURBULENCIA1/Enabled Subsystem'
 * '<S199>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Subscribe_HEAVE/Enabled Subsystem'
 * '<S200>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Subscribe_RATE/Enabled Subsystem'
 * '<S201>' : 'V4_FUZZY/SUBSYSTEM_MODEL/Subscribe_TURBULENCIA/Enabled Subsystem'
 * '<S202>' : 'V4_FUZZY/Subscribe-ALTURA/Enabled Subsystem'
 * '<S203>' : 'V4_FUZZY/Subscribe-YAW/Enabled Subsystem'
 */
#endif                                 /* V4_FUZZY_H_ */
