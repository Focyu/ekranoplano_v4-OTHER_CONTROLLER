/*
 * V4_FUZZY.cpp
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

#include "V4_FUZZY.h"
#include "slros2_initialize.h"
#include "rtwtypes.h"
#include "V4_FUZZY_types.h"
#include "rmw/qos_profiles.h"
#include <string.h>
#include "V4_FUZZY_private.h"
#include <emmintrin.h>
#include <math.h>

extern "C"
{

#include "rt_nonfinite.h"

}

#include <stddef.h>
#include "zero_crossing_types.h"
#include "rt_defines.h"

uint32_T plook_bincpa(real_T u, const real_T bp[], uint32_T maxIndex, real_T
                      *fraction, uint32_T *prevIndex)
{
  uint32_T bpIndex;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0U]) {
    bpIndex = 0U;
    *fraction = 0.0;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32d_prevIdx(u, bp, *prevIndex, maxIndex);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1U] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex;
    *fraction = 0.0;
  }

  *prevIndex = bpIndex;
  return bpIndex;
}

real_T intrp2d_la_pw(const uint32_T bpIndex[], const real_T frac[], const real_T
                     table[], const uint32_T stride, const uint32_T maxIndex[])
{
  real_T y;
  real_T yL_0d0;
  uint32_T offset_1d;

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  offset_1d = bpIndex[1U] * stride + bpIndex[0U];
  if (bpIndex[0U] == maxIndex[0U]) {
    y = table[offset_1d];
  } else {
    yL_0d0 = table[offset_1d];
    y = (table[offset_1d + 1U] - yL_0d0) * frac[0U] + yL_0d0;
  }

  if (bpIndex[1U] == maxIndex[1U]) {
  } else {
    offset_1d += stride;
    if (bpIndex[0U] == maxIndex[0U]) {
      yL_0d0 = table[offset_1d];
    } else {
      yL_0d0 = table[offset_1d];
      yL_0d0 += (table[offset_1d + 1U] - yL_0d0) * frac[0U];
    }

    y += (yL_0d0 - y) * frac[1U];
  }

  return y;
}

uint32_T binsearch_u32d_prevIdx(real_T u, const real_T bp[], uint32_T startIndex,
  uint32_T maxIndex)
{
  uint32_T bpIndex;
  uint32_T found;
  uint32_T iLeft;
  uint32_T iRght;

  /* Binary Search using Previous Index */
  bpIndex = startIndex;
  iLeft = 0U;
  iRght = maxIndex;
  found = 0U;
  while (found == 0U) {
    if (u < bp[bpIndex]) {
      iRght = bpIndex - 1U;
      bpIndex = ((bpIndex + iLeft) - 1U) >> 1U;
    } else if (u < bp[bpIndex + 1U]) {
      found = 1U;
    } else {
      iLeft = bpIndex + 1U;
      bpIndex = ((bpIndex + iRght) + 1U) >> 1U;
    }
  }

  return bpIndex;
}

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
void V4_FUZZY::rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = static_cast<ODE4_IntgData *>(rtsiGetSolverData(si));
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 36;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                static_cast<uint_T>(nXc)*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  V4_FUZZY_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  this->step();
  V4_FUZZY_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  this->step();
  V4_FUZZY_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  this->step();
  V4_FUZZY_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/*
 * Output and update for atomic system:
 *    '<Root>/MATLAB Function-ned_to_PoseStamped'
 *    '<Root>/MATLAB Function-ned_to_TwistStamped'
 */
void V4_FUZZY::MATLABFunctionned_to_PoseStampe(B_MATLABFunctionned_to_PoseSt_T
  *localB)
{
  static const uint8_T b[5] = { 119U, 111U, 114U, 108U, 100U };

  memset(&localB->stringOut[0], 0, sizeof(uint8_T) << 7U);
  for (int32_T i = 0; i < 5; i++) {
    localB->stringOut[i] = b[i];
  }

  localB->lengthOut = 5U;
}

/*
 * System initialize for enable system:
 *    '<S155>/Enabled Subsystem'
 *    '<S158>/Enabled Subsystem'
 */
void V4_FUZZY::V4_FUZZY_EnabledSubsystem_Init(B_EnabledSubsystem_V4_FUZZY_T
  *localB)
{
  /* SystemInitialize for SignalConversion generated from: '<S198>/In1' */
  memset(&localB->In1, 0, sizeof(SL_Bus_std_msgs_Bool));
}

/*
 * Output and update for enable system:
 *    '<S155>/Enabled Subsystem'
 *    '<S158>/Enabled Subsystem'
 */
void V4_FUZZY::V4_FUZZY_EnabledSubsystem(boolean_T rtu_Enable, const
  SL_Bus_std_msgs_Bool *rtu_In1, B_EnabledSubsystem_V4_FUZZY_T *localB)
{
  /* Outputs for Enabled SubSystem: '<S155>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S198>/Enable'
   */
  if (rtu_Enable) {
    /* SignalConversion generated from: '<S198>/In1' */
    localB->In1 = *rtu_In1;
  }

  /* End of Outputs for SubSystem: '<S155>/Enabled Subsystem' */
}

/*
 * System initialize for enable system:
 *    '<S156>/Enabled Subsystem'
 *    '<S157>/Enabled Subsystem'
 *    '<S31>/Enabled Subsystem'
 *    '<S32>/Enabled Subsystem'
 */
void V4_FUZZY::V4_FUZZ_EnabledSubsystem_p_Init(B_EnabledSubsystem_V4_FUZZY_d_T
  *localB)
{
  /* SystemInitialize for SignalConversion generated from: '<S199>/In1' */
  memset(&localB->In1, 0, sizeof(SL_Bus_std_msgs_Float64));
}

/*
 * Output and update for enable system:
 *    '<S156>/Enabled Subsystem'
 *    '<S157>/Enabled Subsystem'
 *    '<S31>/Enabled Subsystem'
 *    '<S32>/Enabled Subsystem'
 */
void V4_FUZZY::V4_FUZZY_EnabledSubsystem_k(boolean_T rtu_Enable, const
  SL_Bus_std_msgs_Float64 *rtu_In1, B_EnabledSubsystem_V4_FUZZY_d_T *localB)
{
  /* Outputs for Enabled SubSystem: '<S156>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S199>/Enable'
   */
  if (rtu_Enable) {
    /* SignalConversion generated from: '<S199>/In1' */
    localB->In1 = *rtu_In1;
  }

  /* End of Outputs for SubSystem: '<S156>/Enabled Subsystem' */
}

void V4_FUZZY::V4_FUZZY_Publisher_setupImpl(const
  ros_slros2_internal_block_Pub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[17] = "/ekranoplano/imu";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S25>/SinkBlock' */
  V4_FUZZY_B.deadline_d.sec = 0.0;
  V4_FUZZY_B.deadline_d.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_d,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 17; i++) {
    /* Start for MATLABSystem: '<S25>/SinkBlock' */
    V4_FUZZY_B.b_zeroDelimTopic_g[i] = b_zeroDelimTopic[i];
  }

  Pub_V4_FUZZY_548.createPublisher(&V4_FUZZY_B.b_zeroDelimTopic_g[0],
    qos_profile);
}

rmw_qos_profile_t V4_FUZZY::V4_ROS2PubSubBase_setQOSProfile(rmw_qos_profile_t
  rmwProfile, real_T qosDepth, real_T qosDeadline, real_T qosLifespan, real_T
  qosLeaseDuration, boolean_T qosAvoidROSNamespaceConventions)
{
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  real_T input;
  real_T sec;
  input = qosDeadline;

  /* Start for MATLABSystem: '<S26>/SinkBlock' */
  if (qosDeadline == (rtInf)) {
    input = 0.0;
  }

  sec = floor(input);
  V4_FUZZY_B.deadline_dy.sec = sec;

  /* Start for MATLABSystem: '<S26>/SinkBlock' */
  V4_FUZZY_B.deadline_dy.nsec = (input - sec) * 1.0E+9;
  input = qosLifespan;

  /* Start for MATLABSystem: '<S26>/SinkBlock' */
  if (qosLifespan == (rtInf)) {
    input = 0.0;
  }

  sec = floor(input);
  lifespan.sec = sec;

  /* Start for MATLABSystem: '<S26>/SinkBlock' */
  lifespan.nsec = (input - sec) * 1.0E+9;
  input = qosLeaseDuration;

  /* Start for MATLABSystem: '<S26>/SinkBlock' */
  if (qosLeaseDuration == (rtInf)) {
    input = 0.0;
  }

  sec = floor(input);
  liveliness_lease_duration.sec = sec;

  /* Start for MATLABSystem: '<S26>/SinkBlock' */
  liveliness_lease_duration.nsec = (input - sec) * 1.0E+9;
  SET_QOS_VALUES(rmwProfile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(qosDepth),
                 RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_dy,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 qosAvoidROSNamespaceConventions);
  return rmwProfile;
}

void V4_FUZZY::V4_FUZZY_Publisher_setupImpl_l(const
  ros_slros2_internal_block_Pub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[25] = "/ekranoplano/PoseStamped";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S27>/SinkBlock' */
  V4_FUZZY_B.deadline_m.sec = 0.0;
  V4_FUZZY_B.deadline_m.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_m,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 25; i++) {
    /* Start for MATLABSystem: '<S27>/SinkBlock' */
    V4_FUZZY_B.b_zeroDelimTopic_c[i] = b_zeroDelimTopic[i];
  }

  Pub_V4_FUZZY_811.createPublisher(&V4_FUZZY_B.b_zeroDelimTopic_c[0],
    qos_profile);
}

void V4_FUZZY::V4_FUZZY_Publisher_setupImpl_lz(const
  ros_slros2_internal_block_Pub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[26] = "/ekranoplano/TwistStamped";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S28>/SinkBlock' */
  V4_FUZZY_B.deadline.sec = 0.0;
  V4_FUZZY_B.deadline.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 26; i++) {
    /* Start for MATLABSystem: '<S28>/SinkBlock' */
    V4_FUZZY_B.b_zeroDelimTopic[i] = b_zeroDelimTopic[i];
  }

  Pub_V4_FUZZY_835.createPublisher(&V4_FUZZY_B.b_zeroDelimTopic[0], qos_profile);
}

void V4_FUZZY::V4_FU_Subscriber_setupImpl_lziw(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[17] = "/setpoint/altura";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S31>/SourceBlock' */
  V4_FUZZY_B.deadline_j.sec = 0.0;
  V4_FUZZY_B.deadline_j.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_j,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 17; i++) {
    /* Start for MATLABSystem: '<S31>/SourceBlock' */
    V4_FUZZY_B.b_zeroDelimTopic_f[i] = b_zeroDelimTopic[i];
  }

  Sub_V4_FUZZY_435.createSubscriber(&V4_FUZZY_B.b_zeroDelimTopic_f[0],
    qos_profile);
}

void V4_FUZZY::V4_F_Subscriber_setupImpl_lziwo(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  char_T b_zeroDelimTopic[14];
  static const char_T b_zeroDelimTopic_0[14] = "/setpoint/yaw";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S32>/SourceBlock' */
  V4_FUZZY_B.deadline_gu.sec = 0.0;
  V4_FUZZY_B.deadline_gu.nsec = 0.0;
  V4_FUZZY_B.lifespan.sec = 0.0;
  V4_FUZZY_B.lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_gu,
                 V4_FUZZY_B.lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 14; i++) {
    /* Start for MATLABSystem: '<S32>/SourceBlock' */
    b_zeroDelimTopic[i] = b_zeroDelimTopic_0[i];
  }

  Sub_V4_FUZZY_377.createSubscriber(&b_zeroDelimTopic[0], qos_profile);
}

void V4_FUZZY::V4_FUZZ_ServiceCaller_setupImpl(ros_slros2_internal_block_Ser_T
  *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[25] = "/gazebo/set_entity_state";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S6>/ServiceCaller' */
  V4_FUZZY_B.deadline_g.sec = 0.0;
  V4_FUZZY_B.deadline_g.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(1.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_g,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 25; i++) {
    /* Start for MATLABSystem: '<S6>/ServiceCaller' */
    V4_FUZZY_B.b_zeroDelimTopic_m[i] = b_zeroDelimTopic[i];
  }

  ServCall_V4_FUZZY_326.createServiceCaller(&V4_FUZZY_B.b_zeroDelimTopic_m[0],
    qos_profile);
}

void V4_FUZZY::V4_FUZ_Subscriber_setupImpl_lzi(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[22] = "/setpoint/turbulencia";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S158>/SourceBlock' */
  V4_FUZZY_B.deadline_p.sec = 0.0;
  V4_FUZZY_B.deadline_p.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_p,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 22; i++) {
    /* Start for MATLABSystem: '<S158>/SourceBlock' */
    V4_FUZZY_B.b_zeroDelimTopic_b[i] = b_zeroDelimTopic[i];
  }

  Sub_V4_FUZZY_417.createSubscriber(&V4_FUZZY_B.b_zeroDelimTopic_b[0],
    qos_profile);
}

void V4_FUZZY::V4_FUZZY_Subscriber_setupImpl(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[22] = "/setpoint/turbulencia";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S155>/SourceBlock' */
  V4_FUZZY_B.deadline_n.sec = 0.0;
  V4_FUZZY_B.deadline_n.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_n,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 22; i++) {
    /* Start for MATLABSystem: '<S155>/SourceBlock' */
    V4_FUZZY_B.b_zeroDelimTopic_cx[i] = b_zeroDelimTopic[i];
  }

  Sub_V4_FUZZY_423.createSubscriber(&V4_FUZZY_B.b_zeroDelimTopic_cx[0],
    qos_profile);
}

void V4_FUZZY::V4_FUZZY_Subscriber_setupImpl_l(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  char_T b_zeroDelimTopic[12];
  static const char_T b_zeroDelimTopic_0[12] = "/olas/heave";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S156>/SourceBlock' */
  V4_FUZZY_B.deadline_ld.sec = 0.0;
  V4_FUZZY_B.deadline_ld.nsec = 0.0;
  V4_FUZZY_B.lifespan_d.sec = 0.0;
  V4_FUZZY_B.lifespan_d.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_ld,
                 V4_FUZZY_B.lifespan_d, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 12; i++) {
    /* Start for MATLABSystem: '<S156>/SourceBlock' */
    b_zeroDelimTopic[i] = b_zeroDelimTopic_0[i];
  }

  Sub_V4_FUZZY_443.createSubscriber(&b_zeroDelimTopic[0], qos_profile);
}

void V4_FUZZY::V4_FUZZ_Subscriber_setupImpl_lz(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[17] = "/olas/pitch_rate";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S157>/SourceBlock' */
  V4_FUZZY_B.deadline_l.sec = 0.0;
  V4_FUZZY_B.deadline_l.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, V4_FUZZY_B.deadline_l,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 17; i++) {
    /* Start for MATLABSystem: '<S157>/SourceBlock' */
    V4_FUZZY_B.b_zeroDelimTopic_cv[i] = b_zeroDelimTopic[i];
  }

  Sub_V4_FUZZY_445.createSubscriber(&V4_FUZZY_B.b_zeroDelimTopic_cv[0],
    qos_profile);
}

real_T rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  uint32_T hi;
  uint32_T lo;

  /* Uniform random number generator (random number between 0 and 1)

     #define IA      16807                      magic multiplier = 7^5
     #define IM      2147483647                 modulus = 2^31-1
     #define IQ      127773                     IM div IA
     #define IR      2836                       IM modulo IA
     #define S       4.656612875245797e-10      reciprocal of 2^31-1
     test = IA * (seed % IQ) - IR * (seed/IQ)
     seed = test < 0 ? (test + IM) : test
     return (seed*S)
   */
  lo = *u % 127773U * 16807U;
  hi = *u / 127773U * 2836U;
  if (lo < hi) {
    *u = 2147483647U - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return static_cast<real_T>(*u) * 4.656612875245797E-10;
}

real_T rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  real_T si;
  real_T sr;
  real_T y;

  /* Normal (Gaussian) random number generator */
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = sqrt(-2.0 * log(si) / si) * sr;
  return y;
}

real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/* Function for MATLAB Function: '<S7>/Evaluate Rule Antecedents' */
real_T V4_FUZZY::V4_FUZZY_trimf(real_T x, const real_T params[3])
{
  real_T y;
  y = 0.0;
  if ((params[0] != params[1]) && (params[0] < x) && (x < params[1])) {
    y = 1.0 / (params[1] - params[0]) * (x - params[0]);
  }

  if ((params[1] != params[2]) && (params[1] < x) && (x < params[2])) {
    y = 1.0 / (params[2] - params[1]) * (params[2] - x);
  }

  if (x == params[1]) {
    y = 1.0;
  }

  return y;
}

/* Function for MATLAB Function: '<S7>/Evaluate Rule Consequents' */
void V4_FUZZY::V4_FUZZY_trimf_l(const real_T x[101], const real_T params[3],
  real_T y[101])
{
  real_T c;
  V4_FUZZY_B.a = params[0];
  V4_FUZZY_B.b = params[1];
  c = params[2];
  for (int32_T i = 0; i < 101; i++) {
    real_T x_0;
    y[i] = 0.0;
    if (V4_FUZZY_B.a != V4_FUZZY_B.b) {
      x_0 = x[i];
      if ((V4_FUZZY_B.a < x_0) && (x_0 < V4_FUZZY_B.b)) {
        y[i] = 1.0 / (V4_FUZZY_B.b - V4_FUZZY_B.a) * (x_0 - V4_FUZZY_B.a);
      }
    }

    if (V4_FUZZY_B.b != c) {
      x_0 = x[i];
      if ((V4_FUZZY_B.b < x_0) && (x_0 < c)) {
        y[i] = 1.0 / (c - V4_FUZZY_B.b) * (c - x_0);
      }
    }

    if (x[i] == V4_FUZZY_B.b) {
      y[i] = 1.0;
    }
  }
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u0 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u1 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(static_cast<real_T>(tmp), static_cast<real_T>(tmp_0));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    real_T tmp;
    real_T tmp_0;
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = 1.0;
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/* Model step function */
void V4_FUZZY::step()
{
  /* local block i/o variables */
  SL_Bus_std_msgs_Bool rtb_SourceBlock_o2_j;
  SL_Bus_std_msgs_Bool rtb_SourceBlock_o2_dd;
  __m128d tmp_1;
  SL_Bus_gazebo_msgs_SetEntityStateResponse tmp;
  int32_T i;
  int32_T rtb_sec_0;
  uint32_T rtb_nsec_0;
  int8_T rtAction;
  int8_T rtPrevAction;
  boolean_T serverAvailableOnTime;
  boolean_T tmp_0;
  static const uint8_T b[15] = { 101U, 107U, 114U, 97U, 110U, 111U, 112U, 108U,
    97U, 110U, 111U, 45U, 105U, 109U, 117U };

  static const uint8_T b_0[5] = { 119U, 111U, 114U, 108U, 100U };

  static const uint8_T b_1[11] = { 101U, 107U, 114U, 97U, 110U, 111U, 112U, 108U,
    97U, 110U, 111U };

  static const real_T b_2[3] = { -3.0, -3.0, -1.0 };

  static const real_T c[3] = { 1.0, 3.0, 3.0 };

  static const real_T d[3] = { -1.0, -1.0, 0.0 };

  static const real_T e[3] = { 0.0, 1.0, 1.0 };

  static const int8_T f[16] = { 5, 5, 4, 4, 3, 3, 2, 1, 3, 2, 3, 2, 2, 1, 2, 1 };

  static const real_T b_3[3] = { -0.1, -0.1, -0.02 };

  static const real_T c_0[3] = { -0.05, -0.02, 0.02 };

  static const real_T d_0[3] = { 0.02, 0.05, 0.1 };

  static const real_T e_0[3] = { 0.08, 0.15, 0.15 };

  static const int8_T f_0[8] = { 5, 4, 4, 3, 3, 2, 2, 1 };

  static const real_T b_4[3] = { -0.09, -0.09, -0.036 };

  static const real_T c_1[3] = { -0.063, -0.027, 0.0 };

  static const real_T d_1[3] = { 0.0, 0.027, 0.063 };

  static const real_T e_1[3] = { 0.036, 0.09, 0.09 };

  static const real_T f_1[3] = { -0.15, -0.15, 0.0 };

  static const real_T g[3] = { 0.0, 0.15, 0.15 };

  static const int8_T h[30] = { 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 1, 1,
    2, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 3 };

  static const int8_T f_2[15] = { 1, 1, 1, 2, 3, 2, 3, 3, 3, 4, 4, 4, 5, 5, 2 };

  static const real_T b_5[3] = { -0.1, -0.1, -0.04 };

  static const real_T c_2[3] = { -0.07, -0.03, 0.0 };

  static const real_T d_2[3] = { 0.0, 0.03, 0.07 };

  static const real_T e_2[3] = { 0.04, 0.1, 0.1 };

  static const real_T f_3[3] = { -0.2, -0.2, 0.0 };

  static const real_T g_0[3] = { 0.0, 0.2, 0.2 };

  if (rtmIsMajorTimeStep((&V4_FUZZY_M))) {
    /* set solver stop time */
    if (!((&V4_FUZZY_M)->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&(&V4_FUZZY_M)->solverInfo, (((&V4_FUZZY_M)
        ->Timing.clockTickH0 + 1) * (&V4_FUZZY_M)->Timing.stepSize0 *
        4294967296.0));
    } else {
      rtsiSetSolverStopTime(&(&V4_FUZZY_M)->solverInfo, (((&V4_FUZZY_M)
        ->Timing.clockTick0 + 1) * (&V4_FUZZY_M)->Timing.stepSize0 +
        (&V4_FUZZY_M)->Timing.clockTickH0 * (&V4_FUZZY_M)->Timing.stepSize0 *
        4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep((&V4_FUZZY_M))) {
    (&V4_FUZZY_M)->Timing.t[0] = rtsiGetT(&(&V4_FUZZY_M)->solverInfo);
  }

  /* BusAssignment: '<Root>/Bus Assignment-IMU' */
  memset(&V4_FUZZY_B.BusAssignmentIMU, 0, sizeof(SL_Bus_sensor_msgs_Imu));

  /* RelationalOperator: '<S151>/Compare' incorporates:
   *  Constant: '<S151>/Constant'
   */
  V4_FUZZY_B.Compare = (V4_FUZZY_X.Integrator_CSTATE[11] >= 0.05);

  /* Outputs for Enabled SubSystem: '<S160>/Hrgw' incorporates:
   *  EnablePort: '<S173>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S160>/Hqgw' incorporates:
   *  EnablePort: '<S172>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S160>/Hpgw' incorporates:
   *  EnablePort: '<S171>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S161>/Hwgw(s)' incorporates:
   *  EnablePort: '<S176>/Enable'
   */
  /* Outputs for Enabled SubSystem: '<S161>/Hvgw(s)' incorporates:
   *  EnablePort: '<S175>/Enable'
   */
  tmp_0 = (rtmIsMajorTimeStep((&V4_FUZZY_M)) &&
           (&V4_FUZZY_M)->Timing.TaskCounters.TID[1] == 0);

  /* End of Outputs for SubSystem: '<S161>/Hvgw(s)' */
  /* End of Outputs for SubSystem: '<S161>/Hwgw(s)' */
  /* End of Outputs for SubSystem: '<S160>/Hpgw' */
  /* End of Outputs for SubSystem: '<S160>/Hqgw' */
  /* End of Outputs for SubSystem: '<S160>/Hrgw' */
  if (tmp_0) {
    /* MATLAB Function: '<S30>/MATLAB Function-reset' incorporates:
     *  Memory: '<S30>/Memory2'
     */
    memcpy(&V4_FUZZY_B.IC[0], &V4_FUZZY_DW.Memory2_PreviousInput[0], 12U *
           sizeof(real_T));
    V4_FUZZY_B.IC[2] = 0.0;
    V4_FUZZY_B.IC[11] = 0.0;

    /* InitialCondition: '<S30>/IC' */
    if (V4_FUZZY_DW.IC_FirstOutputTime) {
      V4_FUZZY_DW.IC_FirstOutputTime = false;

      /* InitialCondition: '<S30>/IC' */
      memcpy(&V4_FUZZY_B.IC[0], &V4_FUZZY_ConstP.pooled11[0], 12U * sizeof
             (real_T));
    }

    /* End of InitialCondition: '<S30>/IC' */
  }

  /* Integrator: '<S30>/Integrator' incorporates:
   *  InitialCondition: '<S30>/IC'
   */
  if (rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo)) {
    serverAvailableOnTime = (((V4_FUZZY_PrevZCX.Integrator_Reset_ZCE ==
      POS_ZCSIG) != V4_FUZZY_B.Compare) &&
      (V4_FUZZY_PrevZCX.Integrator_Reset_ZCE != UNINITIALIZED_ZCSIG));
    V4_FUZZY_PrevZCX.Integrator_Reset_ZCE = V4_FUZZY_B.Compare;

    /* evaluate zero-crossings and the level of the reset signal */
    if (serverAvailableOnTime || V4_FUZZY_B.Compare ||
        V4_FUZZY_DW.Integrator_DWORK1) {
      memcpy(&V4_FUZZY_X.Integrator_CSTATE[0], &V4_FUZZY_B.IC[0], 12U * sizeof
             (real_T));
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&V4_FUZZY_M)->solverInfo,
        true);
      rtsiSetContTimeOutputInconsistentWithStateAtMajorStep(&(&V4_FUZZY_M)
        ->solverInfo, true);
    }
  }

  /* Integrator: '<S30>/Integrator' */
  memcpy(&V4_FUZZY_B.x[0], &V4_FUZZY_X.Integrator_CSTATE[0], 12U * sizeof(real_T));

  /* MATLABSystem: '<Root>/Coordinate Transformation Conversion' */
  if (!V4_FUZZY_DW.fctrlobj_not_empty) {
    V4_FUZZY_DW.fctrlobj.FeatureControlFlag = false;
    V4_FUZZY_DW.fctrlobj_not_empty = true;
  }

  /* SignalConversion generated from: '<Root>/Coordinate Transformation Conversion' incorporates:
   *  MATLABSystem: '<Root>/Coordinate Transformation Conversion'
   */
  _mm_storeu_pd(&V4_FUZZY_B.wbe_b[0], _mm_div_pd(_mm_set_pd(V4_FUZZY_B.x[6],
    V4_FUZZY_B.x[8]), _mm_set1_pd(2.0)));

  /* MATLABSystem: '<Root>/Coordinate Transformation Conversion' incorporates:
   *  SignalConversion generated from: '<Root>/Coordinate Transformation Conversion'
   */
  V4_FUZZY_B.wbe_b[2] = V4_FUZZY_B.x[7] / 2.0;
  V4_FUZZY_B.sina = sin(V4_FUZZY_B.wbe_b[0]);
  V4_FUZZY_B.sinb = sin(V4_FUZZY_B.wbe_b[1]);
  V4_FUZZY_B.sinc = sin(V4_FUZZY_B.wbe_b[2]);
  V4_FUZZY_B.cosa = cos(V4_FUZZY_B.wbe_b[0]);
  V4_FUZZY_B.cosb = cos(V4_FUZZY_B.wbe_b[1]);
  V4_FUZZY_B.cosc = cos(V4_FUZZY_B.wbe_b[2]);

  /* Start for MATLABSystem: '<Root>/Coordinate Transformation Conversion' */
  V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.cosa * V4_FUZZY_B.cosb;

  /* MATLABSystem: '<Root>/Coordinate Transformation Conversion' */
  V4_FUZZY_B.rtb_CoordinateTransformationC_n = V4_FUZZY_B.sina * V4_FUZZY_B.sinb
    * V4_FUZZY_B.sinc + V4_FUZZY_B.rtb_CoordinateTransformationC_b *
    V4_FUZZY_B.cosc;
  V4_FUZZY_B.rtb_CoordinateTransformationC_b =
    V4_FUZZY_B.rtb_CoordinateTransformationC_b * V4_FUZZY_B.sinc -
    V4_FUZZY_B.cosc * V4_FUZZY_B.sina * V4_FUZZY_B.sinb;
  V4_FUZZY_B.rtb_CoordinateTransformationC_o = V4_FUZZY_B.cosa * V4_FUZZY_B.cosc
    * V4_FUZZY_B.sinb + V4_FUZZY_B.cosb * V4_FUZZY_B.sina * V4_FUZZY_B.sinc;
  V4_FUZZY_B.cosb = V4_FUZZY_B.cosb * V4_FUZZY_B.cosc * V4_FUZZY_B.sina -
    V4_FUZZY_B.cosa * V4_FUZZY_B.sinb * V4_FUZZY_B.sinc;
  if (tmp_0) {
    /* MATLAB Function: '<Root>/MATLAB Function-IMU2' */
    memset(&V4_FUZZY_B.stringOut_c[0], 0, sizeof(uint8_T) << 7U);
    for (i = 0; i < 15; i++) {
      V4_FUZZY_B.stringOut_c[i] = b[i];
    }

    V4_FUZZY_B.lengthOut_o = 15U;

    /* End of MATLAB Function: '<Root>/MATLAB Function-IMU2' */
  }

  /* MATLAB Function: '<Root>/MATLAB Function-ned_to_imu' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
   */
  V4_FUZZY_B.cosa = 0.0 * V4_FUZZY_B.rtb_CoordinateTransformationC_o +
    V4_FUZZY_B.rtb_CoordinateTransformationC_b;

  /* BusAssignment: '<Root>/Bus Assignment-IMU' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_imu'
   */
  V4_FUZZY_B.BusAssignmentIMU.orientation.x = -V4_FUZZY_B.cosa;

  /* MATLAB Function: '<Root>/MATLAB Function-ned_to_imu' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
   */
  V4_FUZZY_B.sina = 0.0 * V4_FUZZY_B.rtb_CoordinateTransformationC_b -
    V4_FUZZY_B.rtb_CoordinateTransformationC_o;

  /* BusAssignment: '<Root>/Bus Assignment-IMU' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_imu'
   */
  V4_FUZZY_B.BusAssignmentIMU.orientation.y = V4_FUZZY_B.sina;

  /* MATLAB Function: '<Root>/MATLAB Function-ned_to_imu' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
   */
  V4_FUZZY_B.sinb = 0.0 * V4_FUZZY_B.rtb_CoordinateTransformationC_n -
    V4_FUZZY_B.cosb;

  /* BusAssignment: '<Root>/Bus Assignment-IMU' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_imu'
   */
  V4_FUZZY_B.BusAssignmentIMU.orientation.z = -V4_FUZZY_B.sinb;

  /* MATLAB Function: '<Root>/MATLAB Function-ned_to_imu' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
   */
  V4_FUZZY_B.sinc = 0.0 * V4_FUZZY_B.cosb +
    V4_FUZZY_B.rtb_CoordinateTransformationC_n;

  /* BusAssignment: '<Root>/Bus Assignment-IMU' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_imu'
   */
  V4_FUZZY_B.BusAssignmentIMU.orientation.w = V4_FUZZY_B.sinc;
  V4_FUZZY_B.BusAssignmentIMU.angular_velocity.x = V4_FUZZY_B.x[0];
  V4_FUZZY_B.BusAssignmentIMU.angular_velocity.y = V4_FUZZY_B.x[1];
  V4_FUZZY_B.BusAssignmentIMU.angular_velocity.z = V4_FUZZY_B.x[2];
  memcpy(&V4_FUZZY_B.BusAssignmentIMU.header.frame_id[0],
         &V4_FUZZY_B.stringOut_c[0], sizeof(uint8_T) << 7U);
  V4_FUZZY_B.BusAssignmentIMU.header.frame_id_SL_Info.CurrentLength =
    V4_FUZZY_B.lengthOut_o;

  /* MATLABSystem: '<S25>/SinkBlock' */
  Pub_V4_FUZZY_548.publish(&V4_FUZZY_B.BusAssignmentIMU);

  /* Gain: '<Root>/Gain-Z' */
  V4_FUZZY_B.GainZ = -V4_FUZZY_B.x[11];
  if (tmp_0) {
    /* MATLAB Function: '<Root>/MATLAB Function-Odometry' */
    memset(&V4_FUZZY_B.str_out_c[0], 0, sizeof(uint8_T) << 7U);
    for (i = 0; i < 5; i++) {
      V4_FUZZY_B.str_out_c[i] = b_0[i];
    }

    V4_FUZZY_B.currentLen_l = 5U;
    V4_FUZZY_B.receivedLen_b = 5U;

    /* End of MATLAB Function: '<Root>/MATLAB Function-Odometry' */

    /* MATLAB Function: '<Root>/MATLAB Function-Odometry1' */
    memset(&V4_FUZZY_B.str_out[0], 0, sizeof(uint8_T) << 7U);
    for (i = 0; i < 11; i++) {
      V4_FUZZY_B.str_out[i] = b_1[i];
    }

    V4_FUZZY_B.currentLen = 11U;
    V4_FUZZY_B.receivedLen = 11U;

    /* End of MATLAB Function: '<Root>/MATLAB Function-Odometry1' */
  }

  /* Clock: '<Root>/Clock' */
  V4_FUZZY_B.cosc = (&V4_FUZZY_M)->Timing.t[0];

  /* MATLAB Function: '<Root>/MATLAB Function-clocl' */
  V4_FUZZY_B.CD_ih_IGE = floor(V4_FUZZY_B.cosc);

  /* BusAssignment: '<Root>/Bus Assignment-ODOM' */
  memset(&V4_FUZZY_B.BusAssignmentODOM, 0, sizeof(SL_Bus_nav_msgs_Odometry));

  /* MATLAB Function: '<Root>/MATLAB Function-clocl' */
  if (V4_FUZZY_B.CD_ih_IGE < 2.147483648E+9) {
    if (V4_FUZZY_B.CD_ih_IGE >= -2.147483648E+9) {
      rtb_sec_0 = static_cast<int32_T>(V4_FUZZY_B.CD_ih_IGE);
    } else {
      rtb_sec_0 = MIN_int32_T;
    }
  } else {
    rtb_sec_0 = MAX_int32_T;
  }

  V4_FUZZY_B.cosc = rt_roundd_snf((V4_FUZZY_B.cosc - V4_FUZZY_B.CD_ih_IGE) *
    1.0E+9);
  if (V4_FUZZY_B.cosc < 4.294967296E+9) {
    if (V4_FUZZY_B.cosc >= 0.0) {
      rtb_nsec_0 = static_cast<uint32_T>(V4_FUZZY_B.cosc);
    } else {
      rtb_nsec_0 = 0U;
    }
  } else {
    rtb_nsec_0 = MAX_uint32_T;
  }

  /* BusAssignment: '<Root>/Bus Assignment-ODOM' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
   */
  V4_FUZZY_B.BusAssignmentODOM.pose.pose.position.x = V4_FUZZY_B.x[10];
  V4_FUZZY_B.BusAssignmentODOM.pose.pose.position.y = V4_FUZZY_B.x[9];
  V4_FUZZY_B.BusAssignmentODOM.pose.pose.position.z = -V4_FUZZY_B.GainZ;

  /* MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry' */
  V4_FUZZY_B.cosc = 0.7071 * V4_FUZZY_B.sina;
  V4_FUZZY_B.CD_ih_IGE = 0.7071 * -V4_FUZZY_B.cosa;

  /* BusAssignment: '<Root>/Bus Assignment-ODOM' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
   */
  V4_FUZZY_B.BusAssignmentODOM.pose.pose.orientation.x = V4_FUZZY_B.CD_ih_IGE +
    V4_FUZZY_B.cosc;
  V4_FUZZY_B.BusAssignmentODOM.pose.pose.orientation.y = V4_FUZZY_B.cosc -
    V4_FUZZY_B.CD_ih_IGE;

  /* MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry' */
  V4_FUZZY_B.cosc = 0.7071 * V4_FUZZY_B.sinc;
  V4_FUZZY_B.CD_ih_IGE = 0.7071 * V4_FUZZY_B.sinb;

  /* BusAssignment: '<Root>/Bus Assignment-ODOM' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-clocl'
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
   */
  V4_FUZZY_B.BusAssignmentODOM.pose.pose.orientation.z = V4_FUZZY_B.CD_ih_IGE +
    V4_FUZZY_B.cosc;
  V4_FUZZY_B.BusAssignmentODOM.pose.pose.orientation.w = V4_FUZZY_B.cosc -
    V4_FUZZY_B.CD_ih_IGE;
  V4_FUZZY_B.BusAssignmentODOM.header.frame_id_SL_Info.CurrentLength =
    V4_FUZZY_B.currentLen_l;
  V4_FUZZY_B.BusAssignmentODOM.header.frame_id_SL_Info.ReceivedLength =
    V4_FUZZY_B.receivedLen_b;
  memcpy(&V4_FUZZY_B.BusAssignmentODOM.header.frame_id[0],
         &V4_FUZZY_B.str_out_c[0], sizeof(uint8_T) << 7U);
  memcpy(&V4_FUZZY_B.BusAssignmentODOM.child_frame_id[0], &V4_FUZZY_B.str_out[0],
         sizeof(uint8_T) << 7U);
  V4_FUZZY_B.BusAssignmentODOM.child_frame_id_SL_Info.CurrentLength =
    V4_FUZZY_B.currentLen;
  V4_FUZZY_B.BusAssignmentODOM.child_frame_id_SL_Info.ReceivedLength =
    V4_FUZZY_B.receivedLen;
  V4_FUZZY_B.BusAssignmentODOM.header.stamp.sec = rtb_sec_0;
  V4_FUZZY_B.BusAssignmentODOM.header.stamp.nanosec = rtb_nsec_0;

  /* MATLABSystem: '<S26>/SinkBlock' */
  Pub_V4_FUZZY_697.publish(&V4_FUZZY_B.BusAssignmentODOM);
  if (tmp_0) {
    /* MATLAB Function: '<Root>/MATLAB Function-ned_to_PoseStamped' */
    MATLABFunctionned_to_PoseStampe(&V4_FUZZY_B.sf_MATLABFunctionned_to_PoseS_b);
  }

  /* BusAssignment: '<Root>/Bus Assignment-PoseStaamped' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-clocl'
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_PoseStaamped'
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
   */
  memset(&V4_FUZZY_B.BusAssignmentPoseStaamped, 0, sizeof
         (SL_Bus_geometry_msgs_PoseStamped));
  memcpy(&V4_FUZZY_B.BusAssignmentPoseStaamped.header.frame_id[0],
         &V4_FUZZY_B.sf_MATLABFunctionned_to_PoseS_b.stringOut[0], sizeof
         (uint8_T) << 7U);
  V4_FUZZY_B.BusAssignmentPoseStaamped.header.frame_id_SL_Info.CurrentLength =
    V4_FUZZY_B.sf_MATLABFunctionned_to_PoseS_b.lengthOut;
  V4_FUZZY_B.BusAssignmentPoseStaamped.pose.position.x = -V4_FUZZY_B.x[10];
  V4_FUZZY_B.BusAssignmentPoseStaamped.pose.position.y = V4_FUZZY_B.x[9];
  V4_FUZZY_B.BusAssignmentPoseStaamped.pose.position.z = -V4_FUZZY_B.GainZ;
  V4_FUZZY_B.BusAssignmentPoseStaamped.pose.orientation.x = 0.7071 *
    V4_FUZZY_B.rtb_CoordinateTransformationC_o + -0.7071 *
    V4_FUZZY_B.rtb_CoordinateTransformationC_b;
  V4_FUZZY_B.BusAssignmentPoseStaamped.pose.orientation.y = 0.7071 *
    V4_FUZZY_B.rtb_CoordinateTransformationC_b - -0.7071 *
    V4_FUZZY_B.rtb_CoordinateTransformationC_o;
  V4_FUZZY_B.BusAssignmentPoseStaamped.pose.orientation.z = -(0.7071 *
    V4_FUZZY_B.cosb + -0.7071 * V4_FUZZY_B.rtb_CoordinateTransformationC_n);
  V4_FUZZY_B.BusAssignmentPoseStaamped.pose.orientation.w = 0.7071 *
    V4_FUZZY_B.rtb_CoordinateTransformationC_n - -0.7071 * V4_FUZZY_B.cosb;
  V4_FUZZY_B.BusAssignmentPoseStaamped.header.stamp.sec = rtb_sec_0;
  V4_FUZZY_B.BusAssignmentPoseStaamped.header.stamp.nanosec = rtb_nsec_0;

  /* MATLABSystem: '<S27>/SinkBlock' */
  Pub_V4_FUZZY_811.publish(&V4_FUZZY_B.BusAssignmentPoseStaamped);
  if (tmp_0) {
    /* MATLAB Function: '<Root>/MATLAB Function-ned_to_TwistStamped' */
    MATLABFunctionned_to_PoseStampe(&V4_FUZZY_B.sf_MATLABFunctionned_to_TwistSt);
  }

  /* BusAssignment: '<Root>/Bus Assignment-TwistStamped' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-clocl'
   *  MATLAB Function: '<Root>/MATLAB Function2'
   */
  memset(&V4_FUZZY_B.BusAssignmentTwistStamped, 0, sizeof
         (SL_Bus_geometry_msgs_TwistStamped));
  memcpy(&V4_FUZZY_B.BusAssignmentTwistStamped.header.frame_id[0],
         &V4_FUZZY_B.sf_MATLABFunctionned_to_TwistSt.stringOut[0], sizeof
         (uint8_T) << 7U);
  V4_FUZZY_B.BusAssignmentTwistStamped.header.frame_id_SL_Info.CurrentLength =
    V4_FUZZY_B.sf_MATLABFunctionned_to_TwistSt.lengthOut;
  V4_FUZZY_B.BusAssignmentTwistStamped.twist.linear.x = V4_FUZZY_B.x[1];
  V4_FUZZY_B.BusAssignmentTwistStamped.twist.linear.y = V4_FUZZY_B.x[0];
  V4_FUZZY_B.BusAssignmentTwistStamped.twist.linear.z = -V4_FUZZY_B.x[2];
  V4_FUZZY_B.BusAssignmentTwistStamped.header.stamp.sec = rtb_sec_0;
  V4_FUZZY_B.BusAssignmentTwistStamped.header.stamp.nanosec = rtb_nsec_0;

  /* MATLABSystem: '<S28>/SinkBlock' */
  Pub_V4_FUZZY_835.publish(&V4_FUZZY_B.BusAssignmentTwistStamped);
  if (tmp_0) {
  }

  /* Gain: '<S135>/Filter Coefficient' incorporates:
   *  Constant: '<Root>/Constant_U'
   *  Gain: '<S125>/Derivative Gain'
   *  Integrator: '<S127>/Filter'
   *  Sum: '<Root>/Sum3'
   *  Sum: '<S127>/SumD'
   */
  V4_FUZZY_B.FilterCoefficient = ((20.2 - V4_FUZZY_B.x[0]) * 0.005 -
    V4_FUZZY_X.Filter_CSTATE) * 100.0;

  /* Sum: '<S141>/Sum' incorporates:
   *  Constant: '<Root>/Constant_U'
   *  Gain: '<S137>/Proportional Gain'
   *  Integrator: '<S132>/Integrator'
   *  Sum: '<Root>/Sum3'
   */
  V4_FUZZY_B.rtb_CoordinateTransformationC_n = ((20.2 - V4_FUZZY_B.x[0]) * 0.02
    + V4_FUZZY_X.Integrator_CSTATE_d) + V4_FUZZY_B.FilterCoefficient;

  /* Saturate: '<S139>/Saturation' */
  if (V4_FUZZY_B.rtb_CoordinateTransformationC_n > 1.0) {
    /* Saturate: '<S139>/Saturation' */
    V4_FUZZY_B.Saturation = 1.0;
  } else if (V4_FUZZY_B.rtb_CoordinateTransformationC_n < 0.0) {
    /* Saturate: '<S139>/Saturation' */
    V4_FUZZY_B.Saturation = 0.0;
  } else {
    /* Saturate: '<S139>/Saturation' */
    V4_FUZZY_B.Saturation = V4_FUZZY_B.rtb_CoordinateTransformationC_n;
  }

  /* End of Saturate: '<S139>/Saturation' */
  if (tmp_0) {
    /* MATLABSystem: '<S31>/SourceBlock' */
    V4_FUZZY_B.SourceBlock_o1_o = Sub_V4_FUZZY_435.getLatestMessage
      (&V4_FUZZY_B.SourceBlock_o2_d);

    /* Outputs for Enabled SubSystem: '<S31>/Enabled Subsystem' */
    V4_FUZZY_EnabledSubsystem_k(V4_FUZZY_B.SourceBlock_o1_o,
      &V4_FUZZY_B.SourceBlock_o2_d, &V4_FUZZY_B.EnabledSubsystem_b);

    /* End of Outputs for SubSystem: '<S31>/Enabled Subsystem' */

    /* Switch: '<Root>/Switch3' */
    if (V4_FUZZY_B.SourceBlock_o1_o) {
      /* Switch: '<Root>/Switch3' */
      V4_FUZZY_B.Switch3[0] = V4_FUZZY_B.EnabledSubsystem_b.In1.data;
      V4_FUZZY_B.Switch3[1] = V4_FUZZY_B.EnabledSubsystem_b.In1.data;
    } else {
      /* Switch: '<Root>/Switch3' incorporates:
       *  UnitDelay: '<Root>/Unit Delay3'
       */
      V4_FUZZY_B.Switch3[0] = V4_FUZZY_DW.UnitDelay3_DSTATE[0];
      V4_FUZZY_B.Switch3[1] = V4_FUZZY_DW.UnitDelay3_DSTATE[1];
    }

    /* End of Switch: '<Root>/Switch3' */
  }

  /* Sum: '<Root>/Sum2' */
  tmp_1 = _mm_sub_pd(_mm_loadu_pd(&V4_FUZZY_B.Switch3[0]), _mm_set1_pd
                     (V4_FUZZY_B.GainZ));
  _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);

  /* Outputs for Atomic SubSystem: '<Root>/Fuzzy Logic  Controller - Altura' */
  /* MATLAB Function: '<S8>/Evaluate Rule Antecedents' */
  V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.0;
  V4_FUZZY_B.inputMFCache[0] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], b_2);
  V4_FUZZY_B.Dtot[0] = -2.0;
  V4_FUZZY_B.Dtot[1] = -1.0;
  V4_FUZZY_B.Dtot[2] = 0.0;
  V4_FUZZY_B.inputMFCache[1] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], V4_FUZZY_B.Dtot);
  V4_FUZZY_B.Dtot[0] = -1.0;
  V4_FUZZY_B.Dtot[1] = 0.0;
  V4_FUZZY_B.Dtot[2] = 1.0;
  V4_FUZZY_B.inputMFCache[2] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], V4_FUZZY_B.Dtot);
  V4_FUZZY_B.Dtot[0] = 0.0;
  V4_FUZZY_B.Dtot[1] = 1.0;
  V4_FUZZY_B.Dtot[2] = 2.0;
  V4_FUZZY_B.inputMFCache[3] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], V4_FUZZY_B.Dtot);
  V4_FUZZY_B.inputMFCache[4] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], c);
  V4_FUZZY_B.inputMFCache[5] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[1], d);
  V4_FUZZY_B.Dtot[0] = -0.5;
  V4_FUZZY_B.Dtot[1] = 0.0;
  V4_FUZZY_B.Dtot[2] = 0.5;
  V4_FUZZY_B.inputMFCache[6] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[1], V4_FUZZY_B.Dtot);
  V4_FUZZY_B.inputMFCache[7] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[1], e);
  for (i = 0; i < 8; i++) {
    V4_FUZZY_B.cosc = 1.0;
    for (rtb_sec_0 = 0; rtb_sec_0 < 2; rtb_sec_0++) {
      V4_FUZZY_B.cosb = V4_FUZZY_B.inputMFCache[(f[(rtb_sec_0 << 3) + i] + 5 *
        rtb_sec_0) - 1];
      if ((V4_FUZZY_B.cosc > V4_FUZZY_B.cosb) || (rtIsNaN(V4_FUZZY_B.cosc) &&
           !rtIsNaN(V4_FUZZY_B.cosb))) {
        V4_FUZZY_B.cosc = V4_FUZZY_B.cosb;
      }
    }

    V4_FUZZY_B.antecedentOutputs_l[i] = V4_FUZZY_B.cosc;
    V4_FUZZY_B.rtb_CoordinateTransformationC_o += V4_FUZZY_B.cosc;
  }

  /* MATLAB Function: '<S8>/Evaluate Rule Consequents' incorporates:
   *  Constant: '<S8>/Output Sample Points'
   */
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_c, b_3,
                   V4_FUZZY_B.dv);
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_c, c_0,
                   V4_FUZZY_B.dv1);
  V4_FUZZY_B.Dtot[0] = -0.02;
  V4_FUZZY_B.Dtot[1] = 0.0;
  V4_FUZZY_B.Dtot[2] = 0.02;
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_c, V4_FUZZY_B.Dtot,
                   V4_FUZZY_B.dv2);
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_c, d_0,
                   V4_FUZZY_B.dv3);
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_c, e_0,
                   V4_FUZZY_B.dv4);
  for (i = 0; i < 101; i++) {
    V4_FUZZY_B.aggregatedOutputs_b[i] = 0.0;
    V4_FUZZY_B.outputMFCache[5 * i] = V4_FUZZY_B.dv[i];
    V4_FUZZY_B.outputMFCache[5 * i + 1] = V4_FUZZY_B.dv1[i];
    V4_FUZZY_B.outputMFCache[5 * i + 2] = V4_FUZZY_B.dv2[i];
    V4_FUZZY_B.outputMFCache[5 * i + 3] = V4_FUZZY_B.dv3[i];
    V4_FUZZY_B.outputMFCache[5 * i + 4] = V4_FUZZY_B.dv4[i];
  }

  for (i = 0; i < 8; i++) {
    rtPrevAction = f_0[i];
    V4_FUZZY_B.cosc = V4_FUZZY_B.antecedentOutputs_l[i];
    for (rtb_sec_0 = 0; rtb_sec_0 < 101; rtb_sec_0++) {
      V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.outputMFCache[(5 *
        rtb_sec_0 + rtPrevAction) - 1];
      if ((V4_FUZZY_B.rtb_CoordinateTransformationC_b > V4_FUZZY_B.cosc) ||
          (rtIsNaN(V4_FUZZY_B.rtb_CoordinateTransformationC_b) && !rtIsNaN
           (V4_FUZZY_B.cosc))) {
        V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.cosc;
      }

      if (V4_FUZZY_B.aggregatedOutputs_b[rtb_sec_0] <
          V4_FUZZY_B.rtb_CoordinateTransformationC_b) {
        V4_FUZZY_B.aggregatedOutputs_b[rtb_sec_0] =
          V4_FUZZY_B.rtb_CoordinateTransformationC_b;
      }
    }
  }

  /* MATLAB Function: '<S8>/Defuzzify Outputs' incorporates:
   *  Constant: '<S8>/Output Sample Points'
   *  MATLAB Function: '<S8>/Evaluate Rule Antecedents'
   */
  if (V4_FUZZY_B.rtb_CoordinateTransformationC_o == 0.0) {
    V4_FUZZY_B.defuzzifiedOutputs_o = 0.024999999999999994;
  } else {
    V4_FUZZY_B.defuzzifiedOutputs = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.0;
    for (i = 0; i < 101; i++) {
      V4_FUZZY_B.rtb_CoordinateTransformationC_b +=
        V4_FUZZY_B.aggregatedOutputs_b[i];
    }

    if (V4_FUZZY_B.rtb_CoordinateTransformationC_b == 0.0) {
      V4_FUZZY_B.defuzzifiedOutputs_o = 0.024999999999999994;
    } else {
      for (i = 0; i < 101; i++) {
        V4_FUZZY_B.defuzzifiedOutputs +=
          V4_FUZZY_ConstP.OutputSamplePoints_Value_c[i] *
          V4_FUZZY_B.aggregatedOutputs_b[i];
      }

      V4_FUZZY_B.defuzzifiedOutputs_o = 1.0 /
        V4_FUZZY_B.rtb_CoordinateTransformationC_b *
        V4_FUZZY_B.defuzzifiedOutputs;
    }
  }

  /* End of MATLAB Function: '<S8>/Defuzzify Outputs' */
  /* End of Outputs for SubSystem: '<Root>/Fuzzy Logic  Controller - Altura' */
  if (tmp_0) {
    /* MATLABSystem: '<S32>/SourceBlock' */
    V4_FUZZY_B.SourceBlock_o1 = Sub_V4_FUZZY_377.getLatestMessage
      (&V4_FUZZY_B.SourceBlock_o2);

    /* Outputs for Enabled SubSystem: '<S32>/Enabled Subsystem' */
    V4_FUZZY_EnabledSubsystem_k(V4_FUZZY_B.SourceBlock_o1,
      &V4_FUZZY_B.SourceBlock_o2, &V4_FUZZY_B.EnabledSubsystem_a);

    /* End of Outputs for SubSystem: '<S32>/Enabled Subsystem' */

    /* Switch: '<Root>/Switch2' */
    if (V4_FUZZY_B.SourceBlock_o1) {
      /* Switch: '<Root>/Switch2' */
      V4_FUZZY_B.rtb_CoordinateTransformationC_b =
        V4_FUZZY_B.EnabledSubsystem_a.In1.data;
    } else {
      /* Switch: '<Root>/Switch2' incorporates:
       *  UnitDelay: '<Root>/Unit Delay2'
       */
      V4_FUZZY_B.rtb_CoordinateTransformationC_b =
        V4_FUZZY_DW.UnitDelay2_DSTATE[0];
    }

    /* Switch: '<Root>/Switch2' */
    V4_FUZZY_B.Switch2_l[0] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;

    /* RateLimiter: '<Root>/Rate Limiter2' */
    V4_FUZZY_B.rtb_CoordinateTransformationC_o =
      V4_FUZZY_B.rtb_CoordinateTransformationC_b - V4_FUZZY_DW.PrevY[0];
    if (V4_FUZZY_B.rtb_CoordinateTransformationC_o > 0.0026000000000000003) {
      /* RateLimiter: '<Root>/Rate Limiter2' */
      V4_FUZZY_B.rtb_CoordinateTransformationC_o = V4_FUZZY_DW.PrevY[0] +
        0.0026000000000000003;
    } else if (V4_FUZZY_B.rtb_CoordinateTransformationC_o <
               -0.0026000000000000003) {
      /* RateLimiter: '<Root>/Rate Limiter2' */
      V4_FUZZY_B.rtb_CoordinateTransformationC_o = V4_FUZZY_DW.PrevY[0] -
        0.0026000000000000003;
    } else {
      /* RateLimiter: '<Root>/Rate Limiter2' */
      V4_FUZZY_B.rtb_CoordinateTransformationC_o =
        V4_FUZZY_B.rtb_CoordinateTransformationC_b;
    }

    /* RateLimiter: '<Root>/Rate Limiter2' */
    V4_FUZZY_B.RateLimiter2[0] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;

    /* RateLimiter: '<Root>/Rate Limiter2' */
    V4_FUZZY_DW.PrevY[0] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;

    /* Switch: '<Root>/Switch2' */
    if (V4_FUZZY_B.SourceBlock_o1) {
      /* Switch: '<Root>/Switch2' */
      V4_FUZZY_B.rtb_CoordinateTransformationC_b =
        V4_FUZZY_B.EnabledSubsystem_a.In1.data;
    } else {
      /* Switch: '<Root>/Switch2' incorporates:
       *  UnitDelay: '<Root>/Unit Delay2'
       */
      V4_FUZZY_B.rtb_CoordinateTransformationC_b =
        V4_FUZZY_DW.UnitDelay2_DSTATE[1];
    }

    /* Switch: '<Root>/Switch2' */
    V4_FUZZY_B.Switch2_l[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;

    /* RateLimiter: '<Root>/Rate Limiter2' */
    V4_FUZZY_B.rtb_CoordinateTransformationC_o =
      V4_FUZZY_B.rtb_CoordinateTransformationC_b - V4_FUZZY_DW.PrevY[1];
    if (V4_FUZZY_B.rtb_CoordinateTransformationC_o > 0.0026000000000000003) {
      /* RateLimiter: '<Root>/Rate Limiter2' */
      V4_FUZZY_B.rtb_CoordinateTransformationC_o = V4_FUZZY_DW.PrevY[1] +
        0.0026000000000000003;
    } else if (V4_FUZZY_B.rtb_CoordinateTransformationC_o <
               -0.0026000000000000003) {
      /* RateLimiter: '<Root>/Rate Limiter2' */
      V4_FUZZY_B.rtb_CoordinateTransformationC_o = V4_FUZZY_DW.PrevY[1] -
        0.0026000000000000003;
    } else {
      /* RateLimiter: '<Root>/Rate Limiter2' */
      V4_FUZZY_B.rtb_CoordinateTransformationC_o =
        V4_FUZZY_B.rtb_CoordinateTransformationC_b;
    }

    /* RateLimiter: '<Root>/Rate Limiter2' */
    V4_FUZZY_B.RateLimiter2[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;

    /* RateLimiter: '<Root>/Rate Limiter2' */
    V4_FUZZY_DW.PrevY[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;
  }

  /* Sum: '<Root>/Sum5' */
  tmp_1 = _mm_sub_pd(_mm_loadu_pd(&V4_FUZZY_B.RateLimiter2[0]), _mm_set1_pd
                     (V4_FUZZY_B.x[8]));
  _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);

  /* Outputs for Atomic SubSystem: '<Root>/Fuzzy Logic  Controller - Timon' */
  /* MATLAB Function: '<S9>/Evaluate Rule Antecedents' */
  V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.0;
  V4_FUZZY_B.inputMFCache[0] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], b_4);
  V4_FUZZY_B.inputMFCache[1] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], c_1);
  V4_FUZZY_B.Dtot[0] = -0.018;
  V4_FUZZY_B.Dtot[1] = 0.0;
  V4_FUZZY_B.Dtot[2] = 0.018;
  V4_FUZZY_B.inputMFCache[2] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], V4_FUZZY_B.Dtot);
  V4_FUZZY_B.inputMFCache[3] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], d_1);
  V4_FUZZY_B.inputMFCache[4] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[0], e_1);
  V4_FUZZY_B.inputMFCache[5] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[1], f_1);
  V4_FUZZY_B.Dtot[0] = -0.06;
  V4_FUZZY_B.Dtot[1] = 0.0;
  V4_FUZZY_B.Dtot[2] = 0.06;
  V4_FUZZY_B.inputMFCache[6] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[1], V4_FUZZY_B.Dtot);
  V4_FUZZY_B.inputMFCache[7] = V4_FUZZY_trimf(V4_FUZZY_B.dv6[1], g);
  for (i = 0; i < 15; i++) {
    V4_FUZZY_B.cosc = 1.0;
    for (rtb_sec_0 = 0; rtb_sec_0 < 2; rtb_sec_0++) {
      V4_FUZZY_B.cosb = V4_FUZZY_B.inputMFCache[(h[15 * rtb_sec_0 + i] + 5 *
        rtb_sec_0) - 1];
      if ((V4_FUZZY_B.cosc > V4_FUZZY_B.cosb) || (rtIsNaN(V4_FUZZY_B.cosc) &&
           !rtIsNaN(V4_FUZZY_B.cosb))) {
        V4_FUZZY_B.cosc = V4_FUZZY_B.cosb;
      }
    }

    V4_FUZZY_B.antecedentOutputs[i] = V4_FUZZY_B.cosc;
    V4_FUZZY_B.rtb_CoordinateTransformationC_o += V4_FUZZY_B.cosc;
  }

  /* MATLAB Function: '<S9>/Evaluate Rule Consequents' incorporates:
   *  Constant: '<S9>/Output Sample Points'
   *  MATLAB Function: '<S8>/Evaluate Rule Consequents'
   */
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_o, b_4,
                   V4_FUZZY_B.dv);
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_o, c_1,
                   V4_FUZZY_B.dv1);
  V4_FUZZY_B.Dtot[0] = -0.018;
  V4_FUZZY_B.Dtot[1] = 0.0;
  V4_FUZZY_B.Dtot[2] = 0.018;
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_o, V4_FUZZY_B.Dtot,
                   V4_FUZZY_B.dv2);
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_o, d_1,
                   V4_FUZZY_B.dv3);
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value_o, e_1,
                   V4_FUZZY_B.dv4);
  for (i = 0; i < 101; i++) {
    /* Outputs for Atomic SubSystem: '<Root>/Fuzzy Logic  Controller - Altura' */
    V4_FUZZY_B.aggregatedOutputs_b[i] = 0.0;

    /* End of Outputs for SubSystem: '<Root>/Fuzzy Logic  Controller - Altura' */
    V4_FUZZY_B.outputMFCache[5 * i] = V4_FUZZY_B.dv[i];
    V4_FUZZY_B.outputMFCache[5 * i + 1] = V4_FUZZY_B.dv1[i];
    V4_FUZZY_B.outputMFCache[5 * i + 2] = V4_FUZZY_B.dv2[i];
    V4_FUZZY_B.outputMFCache[5 * i + 3] = V4_FUZZY_B.dv3[i];
    V4_FUZZY_B.outputMFCache[5 * i + 4] = V4_FUZZY_B.dv4[i];
  }

  for (i = 0; i < 15; i++) {
    rtPrevAction = f_2[i];
    V4_FUZZY_B.cosc = V4_FUZZY_B.antecedentOutputs[i];
    for (rtb_sec_0 = 0; rtb_sec_0 < 101; rtb_sec_0++) {
      V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.outputMFCache[(5 *
        rtb_sec_0 + rtPrevAction) - 1];
      if ((V4_FUZZY_B.rtb_CoordinateTransformationC_b > V4_FUZZY_B.cosc) ||
          (rtIsNaN(V4_FUZZY_B.rtb_CoordinateTransformationC_b) && !rtIsNaN
           (V4_FUZZY_B.cosc))) {
        V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.cosc;
      }

      if (V4_FUZZY_B.aggregatedOutputs_b[rtb_sec_0] <
          V4_FUZZY_B.rtb_CoordinateTransformationC_b) {
        V4_FUZZY_B.aggregatedOutputs_b[rtb_sec_0] =
          V4_FUZZY_B.rtb_CoordinateTransformationC_b;
      }
    }
  }

  /* End of MATLAB Function: '<S9>/Evaluate Rule Consequents' */

  /* MATLAB Function: '<S9>/Defuzzify Outputs' incorporates:
   *  Constant: '<S9>/Output Sample Points'
   *  MATLAB Function: '<S9>/Evaluate Rule Antecedents'
   */
  if (V4_FUZZY_B.rtb_CoordinateTransformationC_o == 0.0) {
    V4_FUZZY_B.defuzzifiedOutputs = 0.0;
  } else {
    V4_FUZZY_B.defuzzifiedOutputs = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.0;
    for (i = 0; i < 101; i++) {
      V4_FUZZY_B.rtb_CoordinateTransformationC_b +=
        V4_FUZZY_B.aggregatedOutputs_b[i];
    }

    if (V4_FUZZY_B.rtb_CoordinateTransformationC_b == 0.0) {
      V4_FUZZY_B.defuzzifiedOutputs = 0.0;
    } else {
      for (i = 0; i < 101; i++) {
        V4_FUZZY_B.defuzzifiedOutputs +=
          V4_FUZZY_ConstP.OutputSamplePoints_Value_o[i] *
          V4_FUZZY_B.aggregatedOutputs_b[i];
      }

      V4_FUZZY_B.defuzzifiedOutputs *= 1.0 /
        V4_FUZZY_B.rtb_CoordinateTransformationC_b;
    }
  }

  /* End of MATLAB Function: '<S9>/Defuzzify Outputs' */
  /* End of Outputs for SubSystem: '<Root>/Fuzzy Logic  Controller - Timon' */

  /* Saturate: '<Root>/Saturation_roll_sp' incorporates:
   *  Gain: '<Root>/Gain-roll_sp'
   */
  if (-V4_FUZZY_B.defuzzifiedOutputs > 0.1) {
    /* Saturate: '<Root>/Saturation_roll_sp' */
    V4_FUZZY_B.Saturation_roll_sp = 0.1;
  } else if (-V4_FUZZY_B.defuzzifiedOutputs < -0.1) {
    /* Saturate: '<Root>/Saturation_roll_sp' */
    V4_FUZZY_B.Saturation_roll_sp = -0.1;
  } else {
    /* Saturate: '<Root>/Saturation_roll_sp' */
    V4_FUZZY_B.Saturation_roll_sp = -V4_FUZZY_B.defuzzifiedOutputs;
  }

  /* End of Saturate: '<Root>/Saturation_roll_sp' */

  /* Gain: '<Root>/Gain' */
  V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.13 * V4_FUZZY_B.dv6[0];

  /* Saturate: '<Root>/Saturation' */
  if (V4_FUZZY_B.rtb_CoordinateTransformationC_o > 0.14) {
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.14;
  } else if (V4_FUZZY_B.rtb_CoordinateTransformationC_o < -0.14) {
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = -0.14;
  }

  /* Sum: '<Root>/Sum4' incorporates:
   *  Saturate: '<Root>/Saturation'
   */
  V4_FUZZY_B.rtb_CoordinateTransformationC_b = (V4_FUZZY_B.Saturation_roll_sp +
    V4_FUZZY_B.rtb_CoordinateTransformationC_o) - V4_FUZZY_B.x[6];

  /* Gain: '<Root>/Gain' */
  V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.13 * V4_FUZZY_B.dv6[1];

  /* Saturate: '<Root>/Saturation' */
  if (V4_FUZZY_B.rtb_CoordinateTransformationC_o > 0.14) {
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.14;
  } else if (V4_FUZZY_B.rtb_CoordinateTransformationC_o < -0.14) {
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = -0.14;
  }

  /* Sum: '<Root>/Sum4' incorporates:
   *  Saturate: '<Root>/Saturation'
   */
  V4_FUZZY_B.cosb = (V4_FUZZY_B.Saturation_roll_sp +
                     V4_FUZZY_B.rtb_CoordinateTransformationC_o) - V4_FUZZY_B.x
    [6];

  /* Outputs for Atomic SubSystem: '<Root>/Fuzzy Logic  Controller - Aleron' */
  /* MATLAB Function: '<S7>/Evaluate Rule Antecedents' */
  V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.0;
  V4_FUZZY_B.inputMFCache[0] = V4_FUZZY_trimf
    (V4_FUZZY_B.rtb_CoordinateTransformationC_b, b_5);
  V4_FUZZY_B.inputMFCache[1] = V4_FUZZY_trimf
    (V4_FUZZY_B.rtb_CoordinateTransformationC_b, c_2);
  V4_FUZZY_B.Dtot[0] = -0.02;
  V4_FUZZY_B.Dtot[1] = 0.0;
  V4_FUZZY_B.Dtot[2] = 0.02;
  V4_FUZZY_B.inputMFCache[2] = V4_FUZZY_trimf
    (V4_FUZZY_B.rtb_CoordinateTransformationC_b, V4_FUZZY_B.Dtot);
  V4_FUZZY_B.inputMFCache[3] = V4_FUZZY_trimf
    (V4_FUZZY_B.rtb_CoordinateTransformationC_b, d_2);
  V4_FUZZY_B.inputMFCache[4] = V4_FUZZY_trimf
    (V4_FUZZY_B.rtb_CoordinateTransformationC_b, e_2);
  V4_FUZZY_B.inputMFCache[5] = V4_FUZZY_trimf(V4_FUZZY_B.cosb, f_3);
  V4_FUZZY_B.Dtot[0] = -0.08;
  V4_FUZZY_B.Dtot[1] = 0.0;
  V4_FUZZY_B.Dtot[2] = 0.08;
  V4_FUZZY_B.inputMFCache[6] = V4_FUZZY_trimf(V4_FUZZY_B.cosb, V4_FUZZY_B.Dtot);
  V4_FUZZY_B.inputMFCache[7] = V4_FUZZY_trimf(V4_FUZZY_B.cosb, g_0);
  for (i = 0; i < 15; i++) {
    V4_FUZZY_B.cosc = 1.0;
    for (rtb_sec_0 = 0; rtb_sec_0 < 2; rtb_sec_0++) {
      V4_FUZZY_B.cosb = V4_FUZZY_B.inputMFCache[(h[15 * rtb_sec_0 + i] + 5 *
        rtb_sec_0) - 1];
      if ((V4_FUZZY_B.cosc > V4_FUZZY_B.cosb) || (rtIsNaN(V4_FUZZY_B.cosc) &&
           !rtIsNaN(V4_FUZZY_B.cosb))) {
        V4_FUZZY_B.cosc = V4_FUZZY_B.cosb;
      }
    }

    V4_FUZZY_B.antecedentOutputs[i] = V4_FUZZY_B.cosc;
    V4_FUZZY_B.rtb_CoordinateTransformationC_o += V4_FUZZY_B.cosc;
  }

  /* MATLAB Function: '<S7>/Evaluate Rule Consequents' incorporates:
   *  Constant: '<S7>/Output Sample Points'
   *  MATLAB Function: '<S8>/Evaluate Rule Consequents'
   */
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value, b_5, V4_FUZZY_B.dv);
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value, c_2, V4_FUZZY_B.dv1);
  V4_FUZZY_B.Dtot[0] = -0.02;
  V4_FUZZY_B.Dtot[1] = 0.0;
  V4_FUZZY_B.Dtot[2] = 0.02;
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value, V4_FUZZY_B.Dtot,
                   V4_FUZZY_B.dv2);
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value, d_2, V4_FUZZY_B.dv3);
  V4_FUZZY_trimf_l(V4_FUZZY_ConstP.OutputSamplePoints_Value, e_2, V4_FUZZY_B.dv4);
  for (i = 0; i < 101; i++) {
    /* Outputs for Atomic SubSystem: '<Root>/Fuzzy Logic  Controller - Altura' */
    V4_FUZZY_B.aggregatedOutputs_b[i] = 0.0;

    /* End of Outputs for SubSystem: '<Root>/Fuzzy Logic  Controller - Altura' */
    V4_FUZZY_B.outputMFCache[5 * i] = V4_FUZZY_B.dv[i];
    V4_FUZZY_B.outputMFCache[5 * i + 1] = V4_FUZZY_B.dv1[i];
    V4_FUZZY_B.outputMFCache[5 * i + 2] = V4_FUZZY_B.dv2[i];
    V4_FUZZY_B.outputMFCache[5 * i + 3] = V4_FUZZY_B.dv3[i];
    V4_FUZZY_B.outputMFCache[5 * i + 4] = V4_FUZZY_B.dv4[i];
  }

  for (i = 0; i < 15; i++) {
    rtPrevAction = f_2[i];
    V4_FUZZY_B.cosc = V4_FUZZY_B.antecedentOutputs[i];
    for (rtb_sec_0 = 0; rtb_sec_0 < 101; rtb_sec_0++) {
      V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.outputMFCache[(5 *
        rtb_sec_0 + rtPrevAction) - 1];
      if ((V4_FUZZY_B.rtb_CoordinateTransformationC_b > V4_FUZZY_B.cosc) ||
          (rtIsNaN(V4_FUZZY_B.rtb_CoordinateTransformationC_b) && !rtIsNaN
           (V4_FUZZY_B.cosc))) {
        V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.cosc;
      }

      if (V4_FUZZY_B.aggregatedOutputs_b[rtb_sec_0] <
          V4_FUZZY_B.rtb_CoordinateTransformationC_b) {
        V4_FUZZY_B.aggregatedOutputs_b[rtb_sec_0] =
          V4_FUZZY_B.rtb_CoordinateTransformationC_b;
      }
    }
  }

  /* End of MATLAB Function: '<S7>/Evaluate Rule Consequents' */

  /* MATLAB Function: '<S7>/Defuzzify Outputs' incorporates:
   *  Constant: '<S7>/Output Sample Points'
   *  MATLAB Function: '<S7>/Evaluate Rule Antecedents'
   */
  if (V4_FUZZY_B.rtb_CoordinateTransformationC_o == 0.0) {
    V4_FUZZY_B.defuzzifiedOutputs_k = 0.0;
  } else {
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.0;
    for (i = 0; i < 101; i++) {
      V4_FUZZY_B.rtb_CoordinateTransformationC_b +=
        V4_FUZZY_B.aggregatedOutputs_b[i];
    }

    if (V4_FUZZY_B.rtb_CoordinateTransformationC_b == 0.0) {
      V4_FUZZY_B.defuzzifiedOutputs_k = 0.0;
    } else {
      for (i = 0; i < 101; i++) {
        V4_FUZZY_B.rtb_CoordinateTransformationC_o +=
          V4_FUZZY_ConstP.OutputSamplePoints_Value[i] *
          V4_FUZZY_B.aggregatedOutputs_b[i];
      }

      V4_FUZZY_B.defuzzifiedOutputs_k = 1.0 /
        V4_FUZZY_B.rtb_CoordinateTransformationC_b *
        V4_FUZZY_B.rtb_CoordinateTransformationC_o;
    }
  }

  /* End of MATLAB Function: '<S7>/Defuzzify Outputs' */
  /* End of Outputs for SubSystem: '<Root>/Fuzzy Logic  Controller - Aleron' */

  /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
  if (V4_FUZZY_DW.LastMajorTime == (rtInf)) {
    /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
    V4_FUZZY_B.RateLimitertheta_sp = V4_FUZZY_B.defuzzifiedOutputs_o;
  } else {
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = (&V4_FUZZY_M)->Timing.t[0];
    V4_FUZZY_B.rtb_CoordinateTransformationC_b =
      V4_FUZZY_B.rtb_CoordinateTransformationC_o - V4_FUZZY_DW.LastMajorTime;
    if (V4_FUZZY_DW.LastMajorTime == V4_FUZZY_B.rtb_CoordinateTransformationC_o)
    {
      if (V4_FUZZY_DW.PrevLimited) {
        /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
        V4_FUZZY_B.RateLimitertheta_sp = V4_FUZZY_DW.PrevY_g;
      } else {
        /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
        V4_FUZZY_B.RateLimitertheta_sp = V4_FUZZY_B.defuzzifiedOutputs_o;
      }
    } else {
      V4_FUZZY_B.cosb = V4_FUZZY_B.rtb_CoordinateTransformationC_b * 0.4;
      V4_FUZZY_B.rtb_CoordinateTransformationC_o =
        V4_FUZZY_B.defuzzifiedOutputs_o - V4_FUZZY_DW.PrevY_g;
      if (V4_FUZZY_B.rtb_CoordinateTransformationC_o > V4_FUZZY_B.cosb) {
        /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
        V4_FUZZY_B.RateLimitertheta_sp = V4_FUZZY_DW.PrevY_g + V4_FUZZY_B.cosb;
        serverAvailableOnTime = true;
      } else {
        V4_FUZZY_B.rtb_CoordinateTransformationC_b *= -0.4;
        if (V4_FUZZY_B.rtb_CoordinateTransformationC_o <
            V4_FUZZY_B.rtb_CoordinateTransformationC_b) {
          /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
          V4_FUZZY_B.RateLimitertheta_sp = V4_FUZZY_DW.PrevY_g +
            V4_FUZZY_B.rtb_CoordinateTransformationC_b;
          serverAvailableOnTime = true;
        } else {
          /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
          V4_FUZZY_B.RateLimitertheta_sp = V4_FUZZY_B.defuzzifiedOutputs_o;
          serverAvailableOnTime = false;
        }
      }

      if (rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo)) {
        V4_FUZZY_DW.PrevLimited = serverAvailableOnTime;
      }
    }
  }

  /* End of RateLimiter: '<Root>/Rate Limiter-theta_sp' */

  /* Math: '<S30>/Square1' incorporates:
   *  Sum: '<Root>/Sum1'
   */
  V4_FUZZY_B.Switch = V4_FUZZY_B.RateLimitertheta_sp - V4_FUZZY_B.x[7];

  /* Gain: '<S81>/Filter Coefficient' incorporates:
   *  Gain: '<S71>/Derivative Gain'
   *  Integrator: '<S73>/Filter'
   *  Sum: '<S73>/SumD'
   */
  V4_FUZZY_B.FilterCoefficient_a = (-0.2 * V4_FUZZY_B.Switch -
    V4_FUZZY_X.Filter_CSTATE_f) * 15.0;

  /* Sum: '<S87>/Sum' incorporates:
   *  Gain: '<S83>/Proportional Gain'
   *  Integrator: '<S78>/Integrator'
   */
  V4_FUZZY_B.rtb_CoordinateTransformationC_b = (-0.35 * V4_FUZZY_B.Switch +
    V4_FUZZY_X.Integrator_CSTATE_e) + V4_FUZZY_B.FilterCoefficient_a;

  /* Saturate: '<S85>/Saturation' */
  if (V4_FUZZY_B.rtb_CoordinateTransformationC_b > 0.2) {
    /* Saturate: '<S85>/Saturation' */
    V4_FUZZY_B.Saturation_d = 0.2;
  } else if (V4_FUZZY_B.rtb_CoordinateTransformationC_b < -0.2) {
    /* Saturate: '<S85>/Saturation' */
    V4_FUZZY_B.Saturation_d = -0.2;
  } else {
    /* Saturate: '<S85>/Saturation' */
    V4_FUZZY_B.Saturation_d = V4_FUZZY_B.rtb_CoordinateTransformationC_b;
  }

  /* End of Saturate: '<S85>/Saturation' */
  if (tmp_0) {
    /* MATLAB Function: '<Root>/MATLAB Function' */
    memset(&V4_FUZZY_B.stringOut_b[0], 0, sizeof(uint8_T) << 7U);
    for (i = 0; i < 11; i++) {
      V4_FUZZY_B.stringOut_b[i] = b_1[i];
    }

    V4_FUZZY_B.lengthOut_l = 11U;

    /* End of MATLAB Function: '<Root>/MATLAB Function' */

    /* MATLAB Function: '<Root>/MATLAB Function-MODELO' */
    memset(&V4_FUZZY_B.stringOut[0], 0, sizeof(uint8_T) << 7U);
    for (i = 0; i < 5; i++) {
      V4_FUZZY_B.stringOut[i] = b_0[i];
    }

    V4_FUZZY_B.lengthOut = 5U;

    /* End of MATLAB Function: '<Root>/MATLAB Function-MODELO' */
  }

  /* BusAssignment: '<Root>/Bus Assignment-MODELO' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
   */
  memset(&V4_FUZZY_B.BusAssignmentMODELO, 0, sizeof
         (SL_Bus_gazebo_msgs_SetEntityStateRequest));
  V4_FUZZY_B.BusAssignmentMODELO.state.name_SL_Info.CurrentLength =
    V4_FUZZY_B.lengthOut_l;
  V4_FUZZY_B.BusAssignmentMODELO.state.pose.position.x = -V4_FUZZY_B.x[10];
  V4_FUZZY_B.BusAssignmentMODELO.state.pose.position.y = V4_FUZZY_B.x[9];
  V4_FUZZY_B.BusAssignmentMODELO.state.pose.position.z = V4_FUZZY_B.GainZ;
  V4_FUZZY_B.BusAssignmentMODELO.state.pose.orientation.w = V4_FUZZY_B.sinb;
  V4_FUZZY_B.BusAssignmentMODELO.state.pose.orientation.z = V4_FUZZY_B.sinc;
  V4_FUZZY_B.BusAssignmentMODELO.state.pose.orientation.y = V4_FUZZY_B.cosa;
  V4_FUZZY_B.BusAssignmentMODELO.state.pose.orientation.x = V4_FUZZY_B.sina;
  memcpy(&V4_FUZZY_B.BusAssignmentMODELO.state.name[0], &V4_FUZZY_B.stringOut_b
         [0], sizeof(uint8_T) << 7U);
  memcpy(&V4_FUZZY_B.BusAssignmentMODELO.state.reference_frame[0],
         &V4_FUZZY_B.stringOut[0], sizeof(uint8_T) << 7U);
  V4_FUZZY_B.BusAssignmentMODELO.state.reference_frame_SL_Info.CurrentLength =
    V4_FUZZY_B.lengthOut;

  /* Outputs for Atomic SubSystem: '<Root>/Call Service' */
  /* MATLABSystem: '<S6>/ServiceCaller' */
  serverAvailableOnTime = ServCall_V4_FUZZY_326.waitForServer(5.0);
  if (serverAvailableOnTime) {
    ServCall_V4_FUZZY_326.call(&V4_FUZZY_B.BusAssignmentMODELO, &tmp);
  }

  /* End of MATLABSystem: '<S6>/ServiceCaller' */
  /* End of Outputs for SubSystem: '<Root>/Call Service' */

  /* Gain: '<S68>/ZeroGain' */
  V4_FUZZY_B.sina = 0.0 * V4_FUZZY_B.rtb_CoordinateTransformationC_b;

  /* DeadZone: '<S70>/DeadZone' */
  if (V4_FUZZY_B.rtb_CoordinateTransformationC_b > 0.2) {
    V4_FUZZY_B.rtb_CoordinateTransformationC_b -= 0.2;
  } else if (V4_FUZZY_B.rtb_CoordinateTransformationC_b >= -0.2) {
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.0;
  } else {
    V4_FUZZY_B.rtb_CoordinateTransformationC_b -= -0.2;
  }

  /* End of DeadZone: '<S70>/DeadZone' */

  /* Math: '<S30>/Square1' incorporates:
   *  Gain: '<S75>/Integral Gain'
   */
  V4_FUZZY_B.Switch *= -0.05;

  /* Signum: '<S68>/SignPreSat' */
  if (rtIsNaN(V4_FUZZY_B.rtb_CoordinateTransformationC_b)) {
    /* DataTypeConversion: '<S68>/DataTypeConv1' */
    i = 0;
  } else {
    if (V4_FUZZY_B.rtb_CoordinateTransformationC_b < 0.0) {
      /* DataTypeConversion: '<S68>/DataTypeConv1' */
      V4_FUZZY_B.cosa = -1.0;
    } else {
      /* DataTypeConversion: '<S68>/DataTypeConv1' */
      V4_FUZZY_B.cosa = (V4_FUZZY_B.rtb_CoordinateTransformationC_b > 0.0);
    }

    /* DataTypeConversion: '<S68>/DataTypeConv1' */
    i = static_cast<int32_T>(fmod(V4_FUZZY_B.cosa, 256.0));
  }

  /* End of Signum: '<S68>/SignPreSat' */

  /* Signum: '<S68>/SignPreIntegrator' */
  if (rtIsNaN(V4_FUZZY_B.Switch)) {
    /* DataTypeConversion: '<S68>/DataTypeConv2' */
    rtb_sec_0 = 0;
  } else {
    if (V4_FUZZY_B.Switch < 0.0) {
      /* DataTypeConversion: '<S68>/DataTypeConv2' */
      V4_FUZZY_B.cosa = -1.0;
    } else {
      /* DataTypeConversion: '<S68>/DataTypeConv2' */
      V4_FUZZY_B.cosa = (V4_FUZZY_B.Switch > 0.0);
    }

    /* DataTypeConversion: '<S68>/DataTypeConv2' */
    rtb_sec_0 = static_cast<int32_T>(fmod(V4_FUZZY_B.cosa, 256.0));
  }

  /* End of Signum: '<S68>/SignPreIntegrator' */

  /* DataTypeConversion: '<S68>/DataTypeConv1' */
  if (i < 0) {
    i = static_cast<int8_T>(-static_cast<int8_T>(static_cast<uint8_T>(-
      static_cast<real_T>(i))));
  }

  /* DataTypeConversion: '<S68>/DataTypeConv2' */
  if (rtb_sec_0 < 0) {
    rtb_sec_0 = static_cast<int8_T>(-static_cast<int8_T>(static_cast<uint8_T>(-
      static_cast<real_T>(rtb_sec_0))));
  }

  /* Logic: '<S68>/AND3' incorporates:
   *  DataTypeConversion: '<S68>/DataTypeConv1'
   *  DataTypeConversion: '<S68>/DataTypeConv2'
   *  RelationalOperator: '<S68>/Equal1'
   *  RelationalOperator: '<S68>/NotEqual'
   */
  V4_FUZZY_B.AND3 = ((V4_FUZZY_B.sina !=
                      V4_FUZZY_B.rtb_CoordinateTransformationC_b) && (i ==
    rtb_sec_0));
  if (tmp_0) {
    /* Memory: '<S68>/Memory' */
    V4_FUZZY_B.Memory_k = V4_FUZZY_DW.Memory_PreviousInput_f;
  }

  /* Switch: '<S68>/Switch' */
  if (V4_FUZZY_B.Memory_k) {
    /* Math: '<S30>/Square1' incorporates:
     *  Constant: '<S68>/Constant1'
     *  Switch: '<S68>/Switch'
     */
    V4_FUZZY_B.Switch = 0.0;
  }

  /* End of Switch: '<S68>/Switch' */

  /* Gain: '<S122>/ZeroGain' */
  V4_FUZZY_B.sina = 0.0 * V4_FUZZY_B.rtb_CoordinateTransformationC_n;

  /* DeadZone: '<S124>/DeadZone' */
  if (V4_FUZZY_B.rtb_CoordinateTransformationC_n > 1.0) {
    V4_FUZZY_B.rtb_CoordinateTransformationC_n--;
  } else if (V4_FUZZY_B.rtb_CoordinateTransformationC_n >= 0.0) {
    V4_FUZZY_B.rtb_CoordinateTransformationC_n = 0.0;
  }

  /* End of DeadZone: '<S124>/DeadZone' */

  /* Gain: '<S129>/Integral Gain' incorporates:
   *  Constant: '<Root>/Constant_U'
   *  Sum: '<Root>/Sum3'
   */
  V4_FUZZY_B.Switch_f = (20.2 - V4_FUZZY_B.x[0]) * 0.015;

  /* Signum: '<S122>/SignPreSat' */
  if (rtIsNaN(V4_FUZZY_B.rtb_CoordinateTransformationC_n)) {
    /* DataTypeConversion: '<S122>/DataTypeConv1' */
    i = 0;
  } else {
    if (V4_FUZZY_B.rtb_CoordinateTransformationC_n < 0.0) {
      /* DataTypeConversion: '<S122>/DataTypeConv1' */
      V4_FUZZY_B.cosa = -1.0;
    } else {
      /* DataTypeConversion: '<S122>/DataTypeConv1' */
      V4_FUZZY_B.cosa = (V4_FUZZY_B.rtb_CoordinateTransformationC_n > 0.0);
    }

    /* DataTypeConversion: '<S122>/DataTypeConv1' */
    i = static_cast<int32_T>(fmod(V4_FUZZY_B.cosa, 256.0));
  }

  /* End of Signum: '<S122>/SignPreSat' */

  /* Signum: '<S122>/SignPreIntegrator' */
  if (rtIsNaN(V4_FUZZY_B.Switch_f)) {
    /* DataTypeConversion: '<S122>/DataTypeConv2' */
    rtb_sec_0 = 0;
  } else {
    if (V4_FUZZY_B.Switch_f < 0.0) {
      /* DataTypeConversion: '<S122>/DataTypeConv2' */
      V4_FUZZY_B.cosa = -1.0;
    } else {
      /* DataTypeConversion: '<S122>/DataTypeConv2' */
      V4_FUZZY_B.cosa = (V4_FUZZY_B.Switch_f > 0.0);
    }

    /* DataTypeConversion: '<S122>/DataTypeConv2' */
    rtb_sec_0 = static_cast<int32_T>(fmod(V4_FUZZY_B.cosa, 256.0));
  }

  /* End of Signum: '<S122>/SignPreIntegrator' */

  /* DataTypeConversion: '<S122>/DataTypeConv1' */
  if (i < 0) {
    i = static_cast<int8_T>(-static_cast<int8_T>(static_cast<uint8_T>(-
      static_cast<real_T>(i))));
  }

  /* DataTypeConversion: '<S122>/DataTypeConv2' */
  if (rtb_sec_0 < 0) {
    rtb_sec_0 = static_cast<int8_T>(-static_cast<int8_T>(static_cast<uint8_T>(-
      static_cast<real_T>(rtb_sec_0))));
  }

  /* Logic: '<S122>/AND3' incorporates:
   *  DataTypeConversion: '<S122>/DataTypeConv1'
   *  DataTypeConversion: '<S122>/DataTypeConv2'
   *  RelationalOperator: '<S122>/Equal1'
   *  RelationalOperator: '<S122>/NotEqual'
   */
  V4_FUZZY_B.AND3_p = ((V4_FUZZY_B.sina !=
                        V4_FUZZY_B.rtb_CoordinateTransformationC_n) && (i ==
    rtb_sec_0));
  if (tmp_0) {
    /* Memory: '<S122>/Memory' */
    V4_FUZZY_B.Memory_h = V4_FUZZY_DW.Memory_PreviousInput_k;
  }

  /* Switch: '<S122>/Switch' */
  if (V4_FUZZY_B.Memory_h) {
    /* Gain: '<S129>/Integral Gain' incorporates:
     *  Constant: '<S122>/Constant1'
     *  Switch: '<S122>/Switch'
     */
    V4_FUZZY_B.Switch_f = 0.0;
  }

  /* End of Switch: '<S122>/Switch' */

  /* UnitConversion: '<S162>/Unit Conversion' */
  /* Unit Conversion - from: m to: ft
     Expression: output = (3.28084*input) + (0) */
  V4_FUZZY_B.sina = 3.280839895013123 * V4_FUZZY_B.x[11];
  if (tmp_0) {
    /* Memory: '<S30>/Memory' */
    V4_FUZZY_B.Memory[0] = V4_FUZZY_DW.Memory_PreviousInput[0];

    /* Memory: '<S30>/Memory1' */
    V4_FUZZY_B.Memory1[0] = V4_FUZZY_DW.Memory1_PreviousInput[0];

    /* Memory: '<S30>/Memory' */
    V4_FUZZY_B.Memory[1] = V4_FUZZY_DW.Memory_PreviousInput[1];

    /* Memory: '<S30>/Memory1' */
    V4_FUZZY_B.Memory1[1] = V4_FUZZY_DW.Memory1_PreviousInput[1];

    /* Memory: '<S30>/Memory' */
    V4_FUZZY_B.Memory[2] = V4_FUZZY_DW.Memory_PreviousInput[2];

    /* Memory: '<S30>/Memory1' */
    V4_FUZZY_B.Memory1[2] = V4_FUZZY_DW.Memory1_PreviousInput[2];
  }

  /* MATLAB Function: '<S30>/MATLAB Function - MODEL' incorporates:
   *  Memory: '<S30>/Memory'
   *  SignalConversion generated from: '<S153>/ SFunction '
   */
  if (V4_FUZZY_B.defuzzifiedOutputs_k <= 0.3490658503988659) {
    V4_FUZZY_B.u1 = V4_FUZZY_B.defuzzifiedOutputs_k;
  } else {
    V4_FUZZY_B.u1 = 0.3490658503988659;
  }

  if (!(V4_FUZZY_B.u1 >= -0.3490658503988659)) {
    V4_FUZZY_B.u1 = -0.3490658503988659;
  }

  if (V4_FUZZY_B.Saturation_d <= 0.3490658503988659) {
    V4_FUZZY_B.u2 = V4_FUZZY_B.Saturation_d;
  } else {
    V4_FUZZY_B.u2 = 0.3490658503988659;
  }

  if (!(V4_FUZZY_B.u2 >= -0.3490658503988659)) {
    V4_FUZZY_B.u2 = -0.3490658503988659;
  }

  tmp_1 = _mm_add_pd(_mm_loadu_pd(&V4_FUZZY_B.x[0]), _mm_loadu_pd
                     (&V4_FUZZY_B.Memory[0]));
  _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);

  /* MATLAB Function: '<S30>/MATLAB Function - MODEL' incorporates:
   *  Memory: '<S30>/Memory'
   *  Memory: '<S30>/Memory1'
   *  SignalConversion generated from: '<S153>/ SFunction '
   */
  V4_FUZZY_B.sinc = V4_FUZZY_B.x[2] + V4_FUZZY_B.Memory[2];
  V4_FUZZY_B.sinb = sqrt((V4_FUZZY_B.dv6[0] * V4_FUZZY_B.dv6[0] +
    V4_FUZZY_B.dv6[1] * V4_FUZZY_B.dv6[1]) + V4_FUZZY_B.sinc * V4_FUZZY_B.sinc);
  if (V4_FUZZY_B.sinb < 0.5) {
    V4_FUZZY_B.sinb = 0.5;
    V4_FUZZY_B.sinc = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_n = 0.0;
  } else {
    V4_FUZZY_B.sinc = rt_atan2d_snf(V4_FUZZY_B.sinc, V4_FUZZY_B.dv6[0]);
    V4_FUZZY_B.rtb_CoordinateTransformationC_n = V4_FUZZY_B.dv6[1] /
      V4_FUZZY_B.sinb;
    if ((V4_FUZZY_B.rtb_CoordinateTransformationC_n >= 1.0) || rtIsNaN
        (V4_FUZZY_B.rtb_CoordinateTransformationC_n)) {
      V4_FUZZY_B.rtb_CoordinateTransformationC_n = 1.0;
    }

    if (V4_FUZZY_B.rtb_CoordinateTransformationC_n <= -1.0) {
      V4_FUZZY_B.rtb_CoordinateTransformationC_n = -1.0;
    }

    V4_FUZZY_B.rtb_CoordinateTransformationC_n = asin
      (V4_FUZZY_B.rtb_CoordinateTransformationC_n);
  }

  V4_FUZZY_B.q_aero = V4_FUZZY_B.Memory1[1] + V4_FUZZY_B.x[4];
  if ((-V4_FUZZY_B.x[11] - 0.0505 <= 0.001) || rtIsNaN(-V4_FUZZY_B.x[11] -
       0.0505)) {
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.001;
  } else {
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = -V4_FUZZY_B.x[11] - 0.0505;
  }

  if ((-V4_FUZZY_B.x[11] + 0.3475 <= 0.001) || rtIsNaN(-V4_FUZZY_B.x[11] +
       0.3475)) {
    V4_FUZZY_B.cosa = 0.001;
  } else {
    V4_FUZZY_B.cosa = -V4_FUZZY_B.x[11] + 0.3475;
  }

  V4_FUZZY_B.Q = V4_FUZZY_B.sinb * V4_FUZZY_B.sinb * 0.6125;
  V4_FUZZY_B.wbe_b[0] = V4_FUZZY_B.x[3];
  V4_FUZZY_B.wbe_b[1] = V4_FUZZY_B.x[4];
  V4_FUZZY_B.wbe_b[2] = V4_FUZZY_B.x[5];
  V4_FUZZY_B.Vd1 = V4_FUZZY_B.rtb_CoordinateTransformationC_b / 0.6977;
  V4_FUZZY_B.CD_ih_IGE = V4_FUZZY_B.cosa / 0.3808;
  V4_FUZZY_B.cosa = ((V4_FUZZY_B.sinc - -0.06544984694978735) +
                     0.02617993877991494) * 4.960409453036515;
  V4_FUZZY_B.rtb_CoordinateTransformationC_b = (((V4_FUZZY_B.sinc -
    -0.04363323129985824) + 0.008726646259971648) - (0.56 / V4_FUZZY_B.sinb *
    0.35 * V4_FUZZY_B.q_aero + (V4_FUZZY_B.sinc - -0.06544984694978735) * 0.35))
    * 4.838774891736003;
  V4_FUZZY_B.rtb_CoordinateTransformationC_o = (rt_powd_snf(V4_FUZZY_B.Vd1,
    0.787) * 288.0 * exp(rt_powd_snf(V4_FUZZY_B.Vd1, 0.327) * -9.14) *
    0.9798630886207249 / 5.912947654095886 + 1.0) * V4_FUZZY_B.cosa;
  V4_FUZZY_B.cosb = (rt_powd_snf(V4_FUZZY_B.CD_ih_IGE, 0.787) * 288.0 * exp
                     (rt_powd_snf(V4_FUZZY_B.CD_ih_IGE, 0.327) * -9.14) *
                     0.9562859020012823 / 5.35300902982722 + 1.0) *
    V4_FUZZY_B.rtb_CoordinateTransformationC_b;
  V4_FUZZY_B.cosc = ((1.0 - exp(rt_powd_snf(V4_FUZZY_B.Vd1, 0.814) * -4.74) *
                      0.9791664172678959) - exp(rt_powd_snf(V4_FUZZY_B.Vd1,
    0.758) * -3.88) * (V4_FUZZY_B.Vd1 * V4_FUZZY_B.Vd1)) *
    (V4_FUZZY_B.rtb_CoordinateTransformationC_o *
     V4_FUZZY_B.rtb_CoordinateTransformationC_o / 21.205750411731103);
  V4_FUZZY_B.CD_ih_IGE = ((1.0 - exp(rt_powd_snf(V4_FUZZY_B.CD_ih_IGE, 0.814) *
    -4.74) * 0.9677063475148586) - exp(rt_powd_snf(V4_FUZZY_B.CD_ih_IGE, 0.758) *
    -3.88) * (V4_FUZZY_B.CD_ih_IGE * V4_FUZZY_B.CD_ih_IGE)) * (V4_FUZZY_B.cosb *
    V4_FUZZY_B.cosb / 18.943803701146454);
  V4_FUZZY_B.u2_deg = V4_FUZZY_B.u2 * 180.0 / 3.141592653589793;
  V4_FUZZY_B.u2_deg = ((V4_FUZZY_B.u2_deg * V4_FUZZY_B.u2_deg * -1.08E-5 +
                        0.000715 * V4_FUZZY_B.u2_deg) * 0.02164 +
                       ((V4_FUZZY_B.cosc * 0.0649 + 0.0027258) +
                        V4_FUZZY_B.CD_ih_IGE * 0.02164)) * V4_FUZZY_B.Q;
  V4_FUZZY_B.Ltot = (V4_FUZZY_B.rtb_CoordinateTransformationC_o * 0.0649 +
                     V4_FUZZY_B.cosb * 0.02164) * V4_FUZZY_B.Q;
  V4_FUZZY_B.CQ = -0.019 * V4_FUZZY_B.rtb_CoordinateTransformationC_n * 180.0 /
    3.141592653589793;
  V4_FUZZY_B.FA_b_idx_0 = sin(V4_FUZZY_B.sinc);
  V4_FUZZY_B.FA_b_idx_1 = cos(V4_FUZZY_B.sinc);
  V4_FUZZY_B.R[0] = V4_FUZZY_B.FA_b_idx_1;
  V4_FUZZY_B.R[3] = 0.0;
  V4_FUZZY_B.R[6] = -V4_FUZZY_B.FA_b_idx_0;
  V4_FUZZY_B.R[2] = V4_FUZZY_B.FA_b_idx_0;
  V4_FUZZY_B.R[5] = 0.0;
  V4_FUZZY_B.R[8] = V4_FUZZY_B.FA_b_idx_1;
  V4_FUZZY_B.Dtot[0] = -V4_FUZZY_B.u2_deg;
  V4_FUZZY_B.Dtot[1] = V4_FUZZY_B.CQ * V4_FUZZY_B.Q * 0.0649;
  V4_FUZZY_B.Dtot[2] = -V4_FUZZY_B.Ltot;
  V4_FUZZY_B.R[1] = 0.0;
  V4_FUZZY_B.FA_b_idx_0 = 0.0;
  V4_FUZZY_B.R[4] = 1.0;
  V4_FUZZY_B.FA_b_idx_1 = 0.0;
  V4_FUZZY_B.R[7] = 0.0;
  V4_FUZZY_B.FA_b_idx_2 = 0.0;
  for (i = 0; i < 3; i++) {
    tmp_1 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&V4_FUZZY_B.R[3 * i]),
      _mm_set1_pd(V4_FUZZY_B.Dtot[i])), _mm_set_pd(V4_FUZZY_B.FA_b_idx_1,
      V4_FUZZY_B.FA_b_idx_0));
    _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);
    V4_FUZZY_B.FA_b_idx_0 = V4_FUZZY_B.dv6[0];
    V4_FUZZY_B.FA_b_idx_1 = V4_FUZZY_B.dv6[1];
    V4_FUZZY_B.FA_b_idx_2 += V4_FUZZY_B.R[3 * i + 2] * V4_FUZZY_B.Dtot[i];
  }

  V4_FUZZY_B.Fg_b_idx_2 = 2.0 * V4_FUZZY_B.sinb;
  V4_FUZZY_B.Cl = ((V4_FUZZY_B.Memory1[0] + V4_FUZZY_B.x[3]) * 0.6977 /
                   V4_FUZZY_B.Fg_b_idx_2 * -2.0 + -0.12 *
                   V4_FUZZY_B.rtb_CoordinateTransformationC_n) + -0.5 *
    V4_FUZZY_B.u1;
  V4_FUZZY_B.u2 = ((exp(V4_FUZZY_B.Vd1 * -4.0) * -0.05 + -1.14 * V4_FUZZY_B.sinc)
                   + V4_FUZZY_B.q_aero * 0.093 / V4_FUZZY_B.Fg_b_idx_2 * -5.0) +
    -3.0 * V4_FUZZY_B.u2;
  if (V4_FUZZY_B.defuzzifiedOutputs <= 0.2617993877991494) {
    V4_FUZZY_B.q_aero = V4_FUZZY_B.defuzzifiedOutputs;
  } else {
    V4_FUZZY_B.q_aero = 0.2617993877991494;
  }

  if (!(V4_FUZZY_B.q_aero >= -0.2617993877991494)) {
    V4_FUZZY_B.q_aero = -0.2617993877991494;
  }

  V4_FUZZY_B.u1 = (((V4_FUZZY_B.Memory1[2] + V4_FUZZY_B.x[5]) * 0.6977 /
                    V4_FUZZY_B.Fg_b_idx_2 * -1.5 + -0.1146 *
                    V4_FUZZY_B.rtb_CoordinateTransformationC_n) + -0.3 *
                   V4_FUZZY_B.q_aero) + -0.05 * V4_FUZZY_B.u1;
  if (V4_FUZZY_B.Saturation <= 1.0) {
    V4_FUZZY_B.q_aero = V4_FUZZY_B.Saturation;
  } else {
    V4_FUZZY_B.q_aero = 1.0;
  }

  if (!(V4_FUZZY_B.q_aero >= 0.0)) {
    V4_FUZZY_B.q_aero = 0.0;
  }

  V4_FUZZY_B.Vd1 = (25.0 - V4_FUZZY_B.sinb) * V4_FUZZY_B.q_aero +
    V4_FUZZY_B.sinb;
  if (V4_FUZZY_B.Saturation <= 1.0) {
    V4_FUZZY_B.q_aero = V4_FUZZY_B.Saturation;
  } else {
    V4_FUZZY_B.q_aero = 1.0;
  }

  if (!(V4_FUZZY_B.q_aero >= 0.0)) {
    V4_FUZZY_B.q_aero = 0.0;
  }

  V4_FUZZY_B.q_aero = (25.0 - V4_FUZZY_B.sinb) * V4_FUZZY_B.q_aero +
    V4_FUZZY_B.sinb;
  V4_FUZZY_B.Vd1 = 0.004422606215997847 * V4_FUZZY_B.Vd1 * (V4_FUZZY_B.Vd1 -
    V4_FUZZY_B.sinb);
  V4_FUZZY_B.q_aero = 0.004422606215997847 * V4_FUZZY_B.q_aero *
    (V4_FUZZY_B.q_aero - V4_FUZZY_B.sinb);
  V4_FUZZY_B.FE1_b[0] = V4_FUZZY_B.Vd1 * 0.9961946980917455;
  V4_FUZZY_B.FE1_b[2] = -V4_FUZZY_B.Vd1 * 0.08715574274765817;
  V4_FUZZY_B.FE2_b_idx_0 = V4_FUZZY_B.q_aero * 0.9961946980917455;
  V4_FUZZY_B.FE2_b_idx_2 = -V4_FUZZY_B.q_aero * 0.08715574274765817;
  V4_FUZZY_B.q_aero = -9.81 * sin(V4_FUZZY_B.x[7]) * 1.2;
  _mm_storeu_pd(&V4_FUZZY_B.dv6[0], _mm_mul_pd(_mm_mul_pd(_mm_mul_pd(_mm_set1_pd
    (9.81), _mm_set1_pd(cos(V4_FUZZY_B.x[7]))), _mm_set_pd(cos(V4_FUZZY_B.x[6]),
    sin(V4_FUZZY_B.x[6]))), _mm_set1_pd(1.2)));
  V4_FUZZY_B.Vd1 = V4_FUZZY_B.dv6[0];
  V4_FUZZY_B.Fg_b_idx_2 = V4_FUZZY_B.dv6[1];
  V4_FUZZY_B.FE_b = V4_FUZZY_B.FE1_b[0] + V4_FUZZY_B.FE2_b_idx_0;
  V4_FUZZY_B.FE_b_idx_0 = V4_FUZZY_B.FE_b;
  V4_FUZZY_B.F_b[0] = (V4_FUZZY_B.q_aero + V4_FUZZY_B.FE_b) +
    V4_FUZZY_B.FA_b_idx_0;
  V4_FUZZY_B.F_b[1] = V4_FUZZY_B.dv6[0] + V4_FUZZY_B.FA_b_idx_1;
  V4_FUZZY_B.FE_b = V4_FUZZY_B.FE1_b[2] + V4_FUZZY_B.FE2_b_idx_2;
  V4_FUZZY_B.F_b[2] = (V4_FUZZY_B.dv6[1] + V4_FUZZY_B.FE_b) +
    V4_FUZZY_B.FA_b_idx_2;
  V4_FUZZY_B.L_dec = 0.6977 * V4_FUZZY_B.Q * 0.0649;
  V4_FUZZY_B.Mcg_b_idx_0 = (0.0834 * V4_FUZZY_B.FE1_b[2] + -0.0834 *
    V4_FUZZY_B.FE2_b_idx_2) + V4_FUZZY_B.L_dec * V4_FUZZY_B.Cl;
  V4_FUZZY_B.Q = 0.093 * V4_FUZZY_B.Q * 0.0649 * V4_FUZZY_B.u2 + ((-0.0396 *
    V4_FUZZY_B.FE1_b[0] - 0.0721 * V4_FUZZY_B.FE1_b[2]) + (-0.0396 *
    V4_FUZZY_B.FE2_b_idx_0 - 0.0721 * V4_FUZZY_B.FE2_b_idx_2));
  V4_FUZZY_B.FE2_b_idx_0 = ((0.0 - 0.0834 * V4_FUZZY_B.FE1_b[0]) + (0.0 -
    -0.0834 * V4_FUZZY_B.FE2_b_idx_0)) + V4_FUZZY_B.L_dec * V4_FUZZY_B.u1;
  V4_FUZZY_B.L_dec = V4_FUZZY_B.Mcg_b_idx_0 - 0.07841480417423635 *
    V4_FUZZY_B.FE2_b_idx_0;
  V4_FUZZY_B.FE2_b_idx_2 = V4_FUZZY_B.FE2_b_idx_0 - 0.2258731897286114 *
    V4_FUZZY_B.Mcg_b_idx_0;
  memcpy(&V4_FUZZY_B.R[0], &V4_FUZZY_B.x[0], 9U * sizeof(real_T));
  V4_FUZZY_B.FE1_b[0] = V4_FUZZY_B.R[0];
  V4_FUZZY_B.FE1_b[1] = V4_FUZZY_B.R[1];
  V4_FUZZY_B.FE1_b[2] = V4_FUZZY_B.R[2];
  V4_FUZZY_B.c_phi = cos(V4_FUZZY_B.R[6]);
  V4_FUZZY_B.s_phi = sin(V4_FUZZY_B.R[6]);
  V4_FUZZY_B.c_the = cos(V4_FUZZY_B.R[7]);
  V4_FUZZY_B.s_the = sin(V4_FUZZY_B.R[7]);
  V4_FUZZY_B.c_psi = cos(V4_FUZZY_B.R[8]);
  V4_FUZZY_B.s_psi = sin(V4_FUZZY_B.R[8]);
  V4_FUZZY_B.R[0] = V4_FUZZY_B.c_the * V4_FUZZY_B.c_psi;
  V4_FUZZY_B.R_tmp = V4_FUZZY_B.s_phi * V4_FUZZY_B.s_the;
  V4_FUZZY_B.R[3] = V4_FUZZY_B.R_tmp * V4_FUZZY_B.c_psi - V4_FUZZY_B.c_phi *
    V4_FUZZY_B.s_psi;
  V4_FUZZY_B.R_tmp_b = V4_FUZZY_B.c_phi * V4_FUZZY_B.s_the;
  V4_FUZZY_B.R[6] = V4_FUZZY_B.R_tmp_b * V4_FUZZY_B.c_psi + V4_FUZZY_B.s_phi *
    V4_FUZZY_B.s_psi;
  V4_FUZZY_B.R[1] = V4_FUZZY_B.c_the * V4_FUZZY_B.s_psi;
  V4_FUZZY_B.R[4] = V4_FUZZY_B.R_tmp * V4_FUZZY_B.s_psi + V4_FUZZY_B.c_phi *
    V4_FUZZY_B.c_psi;
  V4_FUZZY_B.R[7] = V4_FUZZY_B.R_tmp_b * V4_FUZZY_B.s_psi - V4_FUZZY_B.s_phi *
    V4_FUZZY_B.c_psi;
  V4_FUZZY_B.R[2] = -V4_FUZZY_B.s_the;
  V4_FUZZY_B.R[5] = V4_FUZZY_B.s_phi * V4_FUZZY_B.c_the;
  V4_FUZZY_B.R[8] = V4_FUZZY_B.c_phi * V4_FUZZY_B.c_the;
  tmp_1 = _mm_sub_pd(_mm_mul_pd(_mm_set_pd(V4_FUZZY_B.x[0], V4_FUZZY_B.x[2]),
    _mm_loadu_pd(&V4_FUZZY_B.x[4])), _mm_mul_pd(_mm_loadu_pd(&V4_FUZZY_B.x[1]),
    _mm_set_pd(V4_FUZZY_B.x[3], V4_FUZZY_B.x[5])));
  _mm_storeu_pd(&V4_FUZZY_B.Dtot[0], tmp_1);
  V4_FUZZY_B.Dtot[2] = V4_FUZZY_B.x[1] * V4_FUZZY_B.x[3] - V4_FUZZY_B.x[0] *
    V4_FUZZY_B.x[4];
  V4_FUZZY_B.dv5[0] = 1.0;
  _mm_storeu_pd(&V4_FUZZY_B.dv6[0], _mm_mul_pd(_mm_set_pd(cos(V4_FUZZY_B.x[6]),
    sin(V4_FUZZY_B.x[6])), _mm_set1_pd(tan(V4_FUZZY_B.x[7]))));
  V4_FUZZY_B.dv5[3] = V4_FUZZY_B.dv6[0];
  V4_FUZZY_B.dv5[6] = V4_FUZZY_B.dv6[1];
  V4_FUZZY_B.dv5[1] = 0.0;
  V4_FUZZY_B.dv5[4] = cos(V4_FUZZY_B.x[6]);
  V4_FUZZY_B.dv5[7] = -sin(V4_FUZZY_B.x[6]);
  V4_FUZZY_B.dv5[2] = 0.0;
  _mm_storeu_pd(&V4_FUZZY_B.dv6[0], _mm_div_pd(_mm_set_pd(cos(V4_FUZZY_B.x[6]),
    sin(V4_FUZZY_B.x[6])), _mm_set1_pd(cos(V4_FUZZY_B.x[7]))));
  V4_FUZZY_B.dv5[5] = V4_FUZZY_B.dv6[0];
  V4_FUZZY_B.dv5[8] = V4_FUZZY_B.dv6[1];
  V4_FUZZY_B.c_phi = 0.0;
  V4_FUZZY_B.s_phi = 0.0;
  V4_FUZZY_B.c_the = 0.0;
  V4_FUZZY_B.s_the = 0.0;
  V4_FUZZY_B.c_psi = 0.0;
  V4_FUZZY_B.s_psi = 0.0;
  for (i = 0; i < 3; i++) {
    tmp_1 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&V4_FUZZY_B.dv5[3 * i]),
      _mm_set1_pd(V4_FUZZY_B.wbe_b[i])), _mm_set_pd(V4_FUZZY_B.s_phi,
      V4_FUZZY_B.c_phi));
    _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);
    V4_FUZZY_B.c_phi = V4_FUZZY_B.dv6[0];
    V4_FUZZY_B.s_phi = V4_FUZZY_B.dv6[1];
    _mm_storeu_pd(&V4_FUZZY_B.dv6[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
      (V4_FUZZY_B.R[3 * i], V4_FUZZY_B.dv5[3 * i + 2]), _mm_set_pd
      (V4_FUZZY_B.FE1_b[i], V4_FUZZY_B.wbe_b[i])), _mm_set_pd(V4_FUZZY_B.s_the,
      V4_FUZZY_B.c_the)));
    V4_FUZZY_B.c_the = V4_FUZZY_B.dv6[0];
    V4_FUZZY_B.s_the = V4_FUZZY_B.dv6[1];
    tmp_1 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&V4_FUZZY_B.R[3 * i + 1]),
      _mm_set1_pd(V4_FUZZY_B.FE1_b[i])), _mm_set_pd(V4_FUZZY_B.s_psi,
      V4_FUZZY_B.c_psi));
    _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);
    V4_FUZZY_B.c_psi = V4_FUZZY_B.dv6[0];
    V4_FUZZY_B.s_psi = V4_FUZZY_B.dv6[1];
    V4_FUZZY_B.XDOT[i] = 0.8333333333333334 * V4_FUZZY_B.F_b[i] -
      V4_FUZZY_B.Dtot[i];
  }

  V4_FUZZY_B.XDOT[3] = ((0.023669 * V4_FUZZY_B.L_dec + 0.001856 *
    V4_FUZZY_B.FE2_b_idx_2) - (-2.6338496E-5 * V4_FUZZY_B.x[3] + 0.000144843342 *
    V4_FUZZY_B.x[5]) * V4_FUZZY_B.x[4]) / 0.000191043437;
  V4_FUZZY_B.XDOT[4] = ((V4_FUZZY_B.Q - -0.015451999999999999 * V4_FUZZY_B.x[3] *
    V4_FUZZY_B.x[5]) - (V4_FUZZY_B.x[3] * V4_FUZZY_B.x[3] - V4_FUZZY_B.x[5] *
                        V4_FUZZY_B.x[5]) * 0.001856) / 0.017695;
  V4_FUZZY_B.XDOT[5] = ((-2.6338496E-5 * V4_FUZZY_B.x[5] + -7.443598999999999E-5
    * V4_FUZZY_B.x[3]) * V4_FUZZY_B.x[4] + (0.001856 * V4_FUZZY_B.L_dec +
    0.008217 * V4_FUZZY_B.FE2_b_idx_2)) / 0.000191043437;
  V4_FUZZY_B.XDOT[9] = V4_FUZZY_B.s_the;
  V4_FUZZY_B.XDOT[10] = V4_FUZZY_B.c_psi;
  V4_FUZZY_B.XDOT[11] = V4_FUZZY_B.s_psi;
  if (!(V4_FUZZY_B.u2_deg >= 0.001)) {
    V4_FUZZY_B.u2_deg = 0.001;
  }

  V4_FUZZY_B.XDOT[12] = V4_FUZZY_B.Ltot / V4_FUZZY_B.u2_deg;
  V4_FUZZY_B.XDOT[19] = V4_FUZZY_B.CQ;
  V4_FUZZY_B.XDOT[20] = V4_FUZZY_B.Cl;
  V4_FUZZY_B.XDOT[21] = V4_FUZZY_B.u2;
  V4_FUZZY_B.XDOT[22] = V4_FUZZY_B.u1;
  V4_FUZZY_B.XDOT[23] = V4_FUZZY_B.sinc;
  V4_FUZZY_B.XDOT[24] = V4_FUZZY_B.rtb_CoordinateTransformationC_n;
  V4_FUZZY_B.XDOT[25] = V4_FUZZY_B.cosa;
  V4_FUZZY_B.XDOT[26] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;
  V4_FUZZY_B.XDOT[27] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;
  V4_FUZZY_B.XDOT[28] = V4_FUZZY_B.cosb;
  V4_FUZZY_B.XDOT[29] = V4_FUZZY_B.cosc;
  V4_FUZZY_B.XDOT[30] = V4_FUZZY_B.CD_ih_IGE;
  V4_FUZZY_B.XDOT[6] = V4_FUZZY_B.c_phi;
  V4_FUZZY_B.XDOT[13] = V4_FUZZY_B.F_b[0];
  V4_FUZZY_B.XDOT[16] = V4_FUZZY_B.Mcg_b_idx_0;
  V4_FUZZY_B.XDOT[31] = V4_FUZZY_B.q_aero;
  V4_FUZZY_B.XDOT[34] = V4_FUZZY_B.FE_b_idx_0;
  V4_FUZZY_B.XDOT[37] = V4_FUZZY_B.FA_b_idx_0;
  V4_FUZZY_B.XDOT[7] = V4_FUZZY_B.s_phi;
  V4_FUZZY_B.XDOT[14] = V4_FUZZY_B.F_b[1];
  V4_FUZZY_B.XDOT[17] = V4_FUZZY_B.Q;
  V4_FUZZY_B.XDOT[32] = V4_FUZZY_B.Vd1;
  V4_FUZZY_B.XDOT[35] = 0.0;
  V4_FUZZY_B.XDOT[38] = V4_FUZZY_B.FA_b_idx_1;
  V4_FUZZY_B.XDOT[8] = V4_FUZZY_B.c_the;
  V4_FUZZY_B.XDOT[15] = V4_FUZZY_B.F_b[2];
  V4_FUZZY_B.XDOT[18] = V4_FUZZY_B.FE2_b_idx_0;
  V4_FUZZY_B.XDOT[33] = V4_FUZZY_B.Fg_b_idx_2;
  V4_FUZZY_B.XDOT[36] = V4_FUZZY_B.FE_b;
  V4_FUZZY_B.XDOT[39] = V4_FUZZY_B.FA_b_idx_2;

  /* UnitConversion: '<S168>/Unit Conversion' incorporates:
   *  MATLAB Function: '<S30>/MATLAB Function - MODEL'
   */
  /* Unit Conversion - from: m/s to: ft/s
     Expression: output = (3.28084*input) + (0) */
  V4_FUZZY_B.sinb *= 3.280839895013123;

  /* Saturate: '<S195>/Limit Function 10ft to 1000ft' incorporates:
   *  Saturate: '<S178>/Limit Height h<1000ft'
   */
  if (V4_FUZZY_B.sina > 1000.0) {
    V4_FUZZY_B.sinc = 1000.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 1000.0;
  } else {
    if (V4_FUZZY_B.sina < 10.0) {
      V4_FUZZY_B.sinc = 10.0;
    } else {
      V4_FUZZY_B.sinc = V4_FUZZY_B.sina;
    }

    if (V4_FUZZY_B.sina < 0.0) {
      V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.0;
    } else {
      V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.sina;
    }
  }

  /* End of Saturate: '<S195>/Limit Function 10ft to 1000ft' */

  /* Fcn: '<S195>/Low Altitude Scale Length' */
  V4_FUZZY_B.rtb_CoordinateTransformationC_o = V4_FUZZY_B.sinc / rt_powd_snf
    (0.000823 * V4_FUZZY_B.sinc + 0.177, 1.2);

  /* Product: '<S178>/sigma_ug, sigma_vg' incorporates:
   *  Fcn: '<S178>/Low Altitude Intensity'
   */
  V4_FUZZY_B.cosa = 1.0 / rt_powd_snf(0.000823 *
    V4_FUZZY_B.rtb_CoordinateTransformationC_b + 0.177, 0.4) *
    V4_FUZZY_ConstB.sigma_wg;

  /* Interpolation_n-D: '<S177>/Medium//High Altitude Intensity' incorporates:
   *  Math: '<S30>/Square2'
   *  PreLookup: '<S177>/PreLook-Up Index Search  (altitude)'
   *  PreLookup: '<S177>/PreLook-Up Index Search  (prob of exceed)'
   */
  V4_FUZZY_B.bpIndex[0] = plook_bincpa(V4_FUZZY_B.sina,
    V4_FUZZY_ConstP.PreLookUpIndexSearchaltitude_Br, 11U,
    &V4_FUZZY_B.rtb_CoordinateTransformationC_b,
    &V4_FUZZY_DW.PreLookUpIndexSearchaltitude_DW);
  V4_FUZZY_B.Lv[0] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;
  V4_FUZZY_B.Lv[1] = V4_FUZZY_ConstB.PreLookUpIndexSearchprobofe;
  V4_FUZZY_B.bpIndex[1] = V4_FUZZY_ConstB.PreLookUpIndexSearchprobo_g;
  V4_FUZZY_B.rtb_CoordinateTransformationC_n = intrp2d_la_pw(V4_FUZZY_B.bpIndex,
    V4_FUZZY_B.Lv, V4_FUZZY_ConstP.MediumHighAltitudeIntensity_Tab, 12U,
    V4_FUZZY_ConstP.MediumHighAltitudeIntensity_max);
  if (tmp_0) {
    /* Product: '<S170>/Divide' incorporates:
     *  Product: '<S170>/Product'
     *  RandomNumber: '<S170>/White Noise'
     */
    tmp_1 = _mm_mul_pd(_mm_loadu_pd(&V4_FUZZY_ConstB.Divide[0]), _mm_loadu_pd
                       (&V4_FUZZY_DW.NextOutput[0]));

    /* Product: '<S170>/Product' */
    _mm_storeu_pd(&V4_FUZZY_B.Product[0], tmp_1);

    /* Product: '<S170>/Divide' incorporates:
     *  Product: '<S170>/Product'
     *  RandomNumber: '<S170>/White Noise'
     */
    tmp_1 = _mm_mul_pd(_mm_loadu_pd(&V4_FUZZY_ConstB.Divide[2]), _mm_loadu_pd
                       (&V4_FUZZY_DW.NextOutput[2]));

    /* Product: '<S170>/Product' */
    _mm_storeu_pd(&V4_FUZZY_B.Product[2], tmp_1);

    /* Outputs for Enabled SubSystem: '<S161>/Hugw(s)' incorporates:
     *  EnablePort: '<S174>/Enable'
     */
    if (rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo) &&
        !V4_FUZZY_DW.Hugws_MODE) {
      (void) memset(&(V4_FUZZY_XDis.ug_p_CSTATE), 0,
                    2*sizeof(boolean_T));

      /* InitializeConditions for Integrator: '<S174>/ug_p' */
      V4_FUZZY_X.ug_p_CSTATE[0] = 0.0;
      V4_FUZZY_X.ug_p_CSTATE[1] = 0.0;
      V4_FUZZY_DW.Hugws_MODE = true;
    }

    /* End of Outputs for SubSystem: '<S161>/Hugw(s)' */
  }

  /* Outputs for Enabled SubSystem: '<S161>/Hugw(s)' incorporates:
   *  EnablePort: '<S174>/Enable'
   */
  if (V4_FUZZY_DW.Hugws_MODE) {
    /* Product: '<S174>/Lug//V' */
    V4_FUZZY_B.Lv[0] = V4_FUZZY_B.rtb_CoordinateTransformationC_o /
      V4_FUZZY_B.sinb;
    V4_FUZZY_B.Lv[1] = V4_FUZZY_ConstB.UnitConversion_c / V4_FUZZY_B.sinb;

    /* Sqrt: '<S174>/sqrt' incorporates:
     *  Gain: '<S174>/(2//pi)'
     *  Integrator: '<S174>/ug_p'
     *  Product: '<S174>/Lug//V1'
     */
    tmp_1 = _mm_div_pd(_mm_sub_pd(_mm_mul_pd(_mm_set_pd(sqrt(0.6366197723675814 *
      V4_FUZZY_B.Lv[1]), sqrt(0.6366197723675814 * V4_FUZZY_B.Lv[0])),
      _mm_set1_pd(V4_FUZZY_B.Product[0])), _mm_loadu_pd(&V4_FUZZY_X.ug_p_CSTATE
      [0])), _mm_loadu_pd(&V4_FUZZY_B.Lv[0]));

    /* Product: '<S174>/w' */
    _mm_storeu_pd(&V4_FUZZY_B.w_n[0], tmp_1);

    /* Integrator: '<S174>/ug_p' incorporates:
     *  Product: '<S174>/w1'
     */
    tmp_1 = _mm_mul_pd(_mm_loadu_pd(&V4_FUZZY_X.ug_p_CSTATE[0]), _mm_set_pd
                       (V4_FUZZY_B.rtb_CoordinateTransformationC_n,
                        V4_FUZZY_B.cosa));

    /* Product: '<S174>/w1' */
    _mm_storeu_pd(&V4_FUZZY_B.w1_c[0], tmp_1);
  }

  /* End of Outputs for SubSystem: '<S161>/Hugw(s)' */

  /* Gain: '<S167>/Lv' */
  V4_FUZZY_B.Lv[1] = V4_FUZZY_ConstB.UnitConversion_c;

  /* Outputs for Enabled SubSystem: '<S161>/Hvgw(s)' incorporates:
   *  EnablePort: '<S175>/Enable'
   */
  if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo) &&
      !V4_FUZZY_DW.Hvgws_MODE) {
    (void) memset(&(V4_FUZZY_XDis.vg_p1_CSTATE), 0,
                  4*sizeof(boolean_T));

    /* InitializeConditions for Integrator: '<S175>/vg_p1' */
    V4_FUZZY_X.vg_p1_CSTATE[0] = 0.0;

    /* InitializeConditions for Integrator: '<S175>/vgw_p2' */
    V4_FUZZY_X.vgw_p2_CSTATE[0] = 0.0;

    /* InitializeConditions for Integrator: '<S175>/vg_p1' */
    V4_FUZZY_X.vg_p1_CSTATE[1] = 0.0;

    /* InitializeConditions for Integrator: '<S175>/vgw_p2' */
    V4_FUZZY_X.vgw_p2_CSTATE[1] = 0.0;
    V4_FUZZY_DW.Hvgws_MODE = true;
  }

  if (V4_FUZZY_DW.Hvgws_MODE) {
    /* Product: '<S175>/Lvg//V' incorporates:
     *  Gain: '<S167>/Lv'
     */
    V4_FUZZY_B.rtb_CoordinateTransformationC_b =
      V4_FUZZY_B.rtb_CoordinateTransformationC_o / V4_FUZZY_B.sinb;

    /* Product: '<S175>/w' incorporates:
     *  Gain: '<S175>/(1//pi)'
     *  Integrator: '<S175>/vg_p1'
     *  Product: '<S175>/Lug//V1'
     *  Sqrt: '<S175>/sqrt'
     *  Sum: '<S175>/Sum'
     */
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = (sqrt(0.3183098861837907 *
      V4_FUZZY_B.rtb_CoordinateTransformationC_b) * V4_FUZZY_B.Product[1] -
      V4_FUZZY_X.vg_p1_CSTATE[0]) / V4_FUZZY_B.rtb_CoordinateTransformationC_b;
    V4_FUZZY_B.w_g[0] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;

    /* Product: '<S175>/w ' incorporates:
     *  Gain: '<S175>/sqrt(3)'
     *  Integrator: '<S175>/vg_p1'
     *  Integrator: '<S175>/vgw_p2'
     *  Product: '<S175>/Lvg//V '
     *  Sum: '<S175>/Sum1'
     */
    V4_FUZZY_B.w_e[0] = (V4_FUZZY_B.rtb_CoordinateTransformationC_o *
                         V4_FUZZY_B.rtb_CoordinateTransformationC_b *
                         1.7320508075688772 + (V4_FUZZY_X.vg_p1_CSTATE[0] -
      V4_FUZZY_X.vgw_p2_CSTATE[0])) / V4_FUZZY_B.rtb_CoordinateTransformationC_b;

    /* Product: '<S175>/Lvg//V' */
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.Lv[1] /
      V4_FUZZY_B.sinb;

    /* Product: '<S175>/w' incorporates:
     *  Gain: '<S175>/(1//pi)'
     *  Integrator: '<S175>/vg_p1'
     *  Product: '<S175>/Lug//V1'
     *  Sqrt: '<S175>/sqrt'
     *  Sum: '<S175>/Sum'
     */
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = (sqrt(0.3183098861837907 *
      V4_FUZZY_B.rtb_CoordinateTransformationC_b) * V4_FUZZY_B.Product[1] -
      V4_FUZZY_X.vg_p1_CSTATE[1]) / V4_FUZZY_B.rtb_CoordinateTransformationC_b;
    V4_FUZZY_B.w_g[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;

    /* Product: '<S175>/w ' incorporates:
     *  Gain: '<S175>/sqrt(3)'
     *  Integrator: '<S175>/vg_p1'
     *  Integrator: '<S175>/vgw_p2'
     *  Product: '<S175>/Lvg//V '
     *  Sum: '<S175>/Sum1'
     */
    V4_FUZZY_B.w_e[1] = (V4_FUZZY_B.rtb_CoordinateTransformationC_o *
                         V4_FUZZY_B.rtb_CoordinateTransformationC_b *
                         1.7320508075688772 + (V4_FUZZY_X.vg_p1_CSTATE[1] -
      V4_FUZZY_X.vgw_p2_CSTATE[1])) / V4_FUZZY_B.rtb_CoordinateTransformationC_b;

    /* Product: '<S175>/w 1' incorporates:
     *  Integrator: '<S175>/vgw_p2'
     */
    tmp_1 = _mm_mul_pd(_mm_set_pd(V4_FUZZY_B.rtb_CoordinateTransformationC_n,
      V4_FUZZY_B.cosa), _mm_loadu_pd(&V4_FUZZY_X.vgw_p2_CSTATE[0]));

    /* Product: '<S175>/w 1' */
    _mm_storeu_pd(&V4_FUZZY_B.w1[0], tmp_1);
  }

  /* End of Outputs for SubSystem: '<S161>/Hvgw(s)' */

  /* Gain: '<S167>/Lw' */
  V4_FUZZY_B.Lv[1] = V4_FUZZY_ConstB.UnitConversion_c;

  /* Outputs for Enabled SubSystem: '<S161>/Hwgw(s)' incorporates:
   *  EnablePort: '<S176>/Enable'
   */
  if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo) &&
      !V4_FUZZY_DW.Hwgws_MODE) {
    (void) memset(&(V4_FUZZY_XDis.wg_p1_CSTATE), 0,
                  4*sizeof(boolean_T));

    /* InitializeConditions for Integrator: '<S176>/wg_p1' */
    V4_FUZZY_X.wg_p1_CSTATE[0] = 0.0;

    /* InitializeConditions for Integrator: '<S176>/wg_p2' */
    V4_FUZZY_X.wg_p2_CSTATE[0] = 0.0;

    /* InitializeConditions for Integrator: '<S176>/wg_p1' */
    V4_FUZZY_X.wg_p1_CSTATE[1] = 0.0;

    /* InitializeConditions for Integrator: '<S176>/wg_p2' */
    V4_FUZZY_X.wg_p2_CSTATE[1] = 0.0;
    V4_FUZZY_DW.Hwgws_MODE = true;
  }

  if (V4_FUZZY_DW.Hwgws_MODE) {
    /* Product: '<S176>/Lwg//V' incorporates:
     *  Gain: '<S167>/Lw'
     */
    V4_FUZZY_B.cosa = V4_FUZZY_B.sinc / V4_FUZZY_B.sinb;

    /* Product: '<S176>/w' incorporates:
     *  Gain: '<S176>/1//pi'
     *  Integrator: '<S176>/wg_p1'
     *  Product: '<S176>/Lug//V1'
     *  Sqrt: '<S176>/sqrt1'
     *  Sum: '<S176>/Sum'
     */
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = (sqrt(0.3183098861837907 *
      V4_FUZZY_B.cosa) * V4_FUZZY_B.Product[2] - V4_FUZZY_X.wg_p1_CSTATE[0]) /
      V4_FUZZY_B.cosa;
    V4_FUZZY_B.w[0] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;

    /* Product: '<S176>/w ' incorporates:
     *  Integrator: '<S176>/wg_p1'
     *  Integrator: '<S176>/wg_p2'
     *  Product: '<S176>/Lwg//V'
     *  Product: '<S176>/Lwg//V '
     *  Sum: '<S176>/Sum1'
     */
    V4_FUZZY_B.w_a[0] = (V4_FUZZY_B.rtb_CoordinateTransformationC_b *
                         V4_FUZZY_ConstB.sqrt_a * V4_FUZZY_B.cosa +
                         (V4_FUZZY_X.wg_p1_CSTATE[0] - V4_FUZZY_X.wg_p2_CSTATE[0]))
      / V4_FUZZY_B.cosa;

    /* Product: '<S176>/Lwg//V' */
    V4_FUZZY_B.cosa = V4_FUZZY_B.Lv[1] / V4_FUZZY_B.sinb;

    /* Product: '<S176>/w' incorporates:
     *  Gain: '<S176>/1//pi'
     *  Integrator: '<S176>/wg_p1'
     *  Product: '<S176>/Lug//V1'
     *  Sqrt: '<S176>/sqrt1'
     *  Sum: '<S176>/Sum'
     */
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = (sqrt(0.3183098861837907 *
      V4_FUZZY_B.cosa) * V4_FUZZY_B.Product[2] - V4_FUZZY_X.wg_p1_CSTATE[1]) /
      V4_FUZZY_B.cosa;
    V4_FUZZY_B.w[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;

    /* Product: '<S176>/w ' incorporates:
     *  Integrator: '<S176>/wg_p1'
     *  Integrator: '<S176>/wg_p2'
     *  Product: '<S176>/Lwg//V'
     *  Product: '<S176>/Lwg//V '
     *  Sum: '<S176>/Sum1'
     */
    V4_FUZZY_B.w_a[1] = (V4_FUZZY_B.rtb_CoordinateTransformationC_b *
                         V4_FUZZY_ConstB.sqrt_a * V4_FUZZY_B.cosa +
                         (V4_FUZZY_X.wg_p1_CSTATE[1] - V4_FUZZY_X.wg_p2_CSTATE[1]))
      / V4_FUZZY_B.cosa;

    /* Product: '<S176>/Lwg//V 1' incorporates:
     *  Integrator: '<S176>/wg_p2'
     */
    tmp_1 = _mm_mul_pd(_mm_set_pd(V4_FUZZY_B.rtb_CoordinateTransformationC_n,
      V4_FUZZY_ConstB.sigma_wg), _mm_loadu_pd(&V4_FUZZY_X.wg_p2_CSTATE[0]));

    /* Product: '<S176>/Lwg//V 1' */
    _mm_storeu_pd(&V4_FUZZY_B.LwgV1[0], tmp_1);
  }

  /* End of Outputs for SubSystem: '<S161>/Hwgw(s)' */

  /* Angle2Dcm: '<S30>/Rotation Angles to Direction Cosine Matrix' */
  V4_FUZZY_B.cosa = cos(V4_FUZZY_B.x[6]);
  V4_FUZZY_B.rtb_CoordinateTransformationC_b = sin(V4_FUZZY_B.x[6]);
  V4_FUZZY_B.rtb_CoordinateTransformationC_o = -sin(V4_FUZZY_B.x[6]);
  V4_FUZZY_B.cosb = cos(V4_FUZZY_B.x[6]);
  V4_FUZZY_B.FA_b_idx_2 = cos(V4_FUZZY_B.x[7]);
  V4_FUZZY_B.q_aero = -sin(V4_FUZZY_B.x[7]);
  V4_FUZZY_B.cosc = sin(V4_FUZZY_B.x[7]);
  V4_FUZZY_B.CD_ih_IGE = cos(V4_FUZZY_B.x[7]);
  V4_FUZZY_B.u2_deg = cos(V4_FUZZY_B.x[8]);
  V4_FUZZY_B.Ltot = sin(V4_FUZZY_B.x[8]);
  V4_FUZZY_B.CQ = -sin(V4_FUZZY_B.x[8]);
  V4_FUZZY_B.Cl = cos(V4_FUZZY_B.x[8]);
  V4_FUZZY_B.FA_b_idx_0 = 0.0 * V4_FUZZY_B.cosc + V4_FUZZY_B.FA_b_idx_2;
  V4_FUZZY_B.FA_b_idx_1 = 0.0 * V4_FUZZY_B.CD_ih_IGE + V4_FUZZY_B.q_aero;
  V4_FUZZY_B.u2 = V4_FUZZY_B.u2_deg * 0.0;
  V4_FUZZY_B.u1 = 0.0 * V4_FUZZY_B.FA_b_idx_2;
  V4_FUZZY_B.FA_b_idx_2 = (V4_FUZZY_B.u1 + V4_FUZZY_B.u2) + V4_FUZZY_B.Ltot *
    V4_FUZZY_B.cosc;
  V4_FUZZY_B.u2_deg += V4_FUZZY_B.Ltot * 0.0;
  V4_FUZZY_B.q_aero *= 0.0;
  V4_FUZZY_B.Ltot = (V4_FUZZY_B.q_aero + V4_FUZZY_B.u2) + V4_FUZZY_B.Ltot *
    V4_FUZZY_B.CD_ih_IGE;
  V4_FUZZY_B.u2 = V4_FUZZY_B.CQ * 0.0;
  V4_FUZZY_B.cosc = (V4_FUZZY_B.u1 + V4_FUZZY_B.u2) + V4_FUZZY_B.cosc *
    V4_FUZZY_B.Cl;
  V4_FUZZY_B.CQ += V4_FUZZY_B.Cl * 0.0;
  V4_FUZZY_B.CD_ih_IGE = (V4_FUZZY_B.q_aero + V4_FUZZY_B.u2) + V4_FUZZY_B.Cl *
    V4_FUZZY_B.CD_ih_IGE;
  V4_FUZZY_B.Cl = V4_FUZZY_B.FA_b_idx_1 * 0.0;
  V4_FUZZY_B.RotationAnglestoDirectionCo[0] = (V4_FUZZY_B.FA_b_idx_0 *
    V4_FUZZY_B.cosa + 0.0 * V4_FUZZY_B.rtb_CoordinateTransformationC_o) +
    V4_FUZZY_B.Cl;
  V4_FUZZY_B.u2 = V4_FUZZY_B.Ltot * 0.0;
  V4_FUZZY_B.RotationAnglestoDirectionCo[1] = (V4_FUZZY_B.cosa *
    V4_FUZZY_B.FA_b_idx_2 + V4_FUZZY_B.rtb_CoordinateTransformationC_o *
    V4_FUZZY_B.u2_deg) + V4_FUZZY_B.u2;
  V4_FUZZY_B.u1 = V4_FUZZY_B.CD_ih_IGE * 0.0;
  V4_FUZZY_B.RotationAnglestoDirectionCo[2] = (V4_FUZZY_B.cosa * V4_FUZZY_B.cosc
    + V4_FUZZY_B.rtb_CoordinateTransformationC_o * V4_FUZZY_B.CQ) +
    V4_FUZZY_B.u1;
  V4_FUZZY_B.RotationAnglestoDirectionCo[3] = (V4_FUZZY_B.FA_b_idx_0 *
    V4_FUZZY_B.rtb_CoordinateTransformationC_b + 0.0 * V4_FUZZY_B.cosb) +
    V4_FUZZY_B.Cl;
  V4_FUZZY_B.RotationAnglestoDirectionCo[4] =
    (V4_FUZZY_B.rtb_CoordinateTransformationC_b * V4_FUZZY_B.FA_b_idx_2 +
     V4_FUZZY_B.u2_deg * V4_FUZZY_B.cosb) + V4_FUZZY_B.u2;
  V4_FUZZY_B.RotationAnglestoDirectionCo[5] =
    (V4_FUZZY_B.rtb_CoordinateTransformationC_b * V4_FUZZY_B.cosc +
     V4_FUZZY_B.cosb * V4_FUZZY_B.CQ) + V4_FUZZY_B.u1;
  V4_FUZZY_B.RotationAnglestoDirectionCo[6] = V4_FUZZY_B.FA_b_idx_0 * 0.0 +
    V4_FUZZY_B.FA_b_idx_1;
  V4_FUZZY_B.RotationAnglestoDirectionCo[7] = (V4_FUZZY_B.FA_b_idx_2 * 0.0 +
    V4_FUZZY_B.u2_deg * 0.0) + V4_FUZZY_B.Ltot;
  V4_FUZZY_B.RotationAnglestoDirectionCo[8] = (V4_FUZZY_B.cosc * 0.0 +
    V4_FUZZY_B.CQ * 0.0) + V4_FUZZY_B.CD_ih_IGE;

  /* If: '<S166>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' incorporates:
   *  Constant: '<S187>/max_height_low'
   *  Product: '<S187>/Product1'
   *  Product: '<S192>/Product1'
   *  Product: '<S192>/Product2'
   *  Product: '<S194>/Product1'
   *  Product: '<S194>/Product2'
   *  Sum: '<S187>/Sum1'
   *  Sum: '<S187>/Sum2'
   *  Sum: '<S187>/Sum3'
   *  Sum: '<S192>/Sum'
   *  Sum: '<S194>/Sum'
   */
  rtPrevAction = V4_FUZZY_DW.ifHeightMaxlowaltitudeelseifHei;
  if (rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo)) {
    if (V4_FUZZY_B.sina <= 1000.0) {
      rtAction = 0;
    } else if (V4_FUZZY_B.sina >= 2000.0) {
      rtAction = 1;
    } else {
      rtAction = 2;
    }

    V4_FUZZY_DW.ifHeightMaxlowaltitudeelseifHei = rtAction;
  } else {
    rtAction = V4_FUZZY_DW.ifHeightMaxlowaltitudeelseifHei;
  }

  if (rtPrevAction != rtAction) {
    rtsiSetBlockStateForSolverChangedAtMajorStep(&(&V4_FUZZY_M)->solverInfo,
      true);
  }

  switch (rtAction) {
   case 0:
    /* Outputs for IfAction SubSystem: '<S166>/Low altitude  velocities' incorporates:
     *  ActionPort: '<S188>/Action Port'
     */
    /* SignalConversion generated from: '<S193>/Vector Concatenate' */
    V4_FUZZY_B.Product_k[2] = V4_FUZZY_B.LwgV1[0];

    /* Trigonometry: '<S194>/Trigonometric Function' incorporates:
     *  UnitConversion: '<S159>/Unit Conversion'
     */
    V4_FUZZY_B.cosa = sin(V4_FUZZY_ConstB.UnitConversion);
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = cos
      (V4_FUZZY_ConstB.UnitConversion);
    _mm_storeu_pd(&V4_FUZZY_B.Product_k[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
      (V4_FUZZY_B.cosa, V4_FUZZY_B.w1_c[0]), _mm_set_pd(V4_FUZZY_B.w1_c[0],
      V4_FUZZY_B.rtb_CoordinateTransformationC_b)), _mm_mul_pd(_mm_mul_pd
      (_mm_set_pd(V4_FUZZY_B.w1[0], V4_FUZZY_B.cosa), _mm_set_pd
       (V4_FUZZY_B.rtb_CoordinateTransformationC_b, V4_FUZZY_B.w1[0])),
      _mm_set_pd(1.0, -1.0))));

    /* Product: '<S193>/Product' incorporates:
     *  Angle2Dcm: '<S30>/Rotation Angles to Direction Cosine Matrix'
     *  Concatenate: '<S193>/Vector Concatenate'
     *  Product: '<S194>/Product1'
     *  Product: '<S194>/Product2'
     *  Reshape: '<S193>/Reshape1'
     *  Sum: '<S194>/Sum'
     */
    V4_FUZZY_B.cosa = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.0;
    for (i = 0; i < 3; i++) {
      tmp_1 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&V4_FUZZY_B.RotationAnglestoDirectionCo[3 * i]), _mm_set1_pd
        (V4_FUZZY_B.Product_k[i])), _mm_set_pd
                         (V4_FUZZY_B.rtb_CoordinateTransformationC_b,
                          V4_FUZZY_B.cosa));
      _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);
      V4_FUZZY_B.cosa = V4_FUZZY_B.dv6[0];
      V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.dv6[1];
      V4_FUZZY_B.rtb_CoordinateTransformationC_o +=
        V4_FUZZY_B.RotationAnglestoDirectionCo[3 * i + 2] *
        V4_FUZZY_B.Product_k[i];
    }

    V4_FUZZY_B.wbe_b[2] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;
    V4_FUZZY_B.wbe_b[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;
    V4_FUZZY_B.wbe_b[0] = V4_FUZZY_B.cosa;

    /* End of Product: '<S193>/Product' */
    /* End of Outputs for SubSystem: '<S166>/Low altitude  velocities' */
    break;

   case 1:
    /* Outputs for IfAction SubSystem: '<S166>/Medium//High  altitude velocities' incorporates:
     *  ActionPort: '<S189>/Action Port'
     */
    /* Gain: '<S189>/Gain' */
    V4_FUZZY_B.wbe_b[0] = V4_FUZZY_B.w1_c[1];
    V4_FUZZY_B.wbe_b[1] = V4_FUZZY_B.w1[1];
    V4_FUZZY_B.wbe_b[2] = V4_FUZZY_B.LwgV1[1];

    /* End of Outputs for SubSystem: '<S166>/Medium//High  altitude velocities' */
    break;

   default:
    /* Outputs for IfAction SubSystem: '<S166>/Interpolate  velocities' incorporates:
     *  ActionPort: '<S187>/Action Port'
     */
    /* Trigonometry: '<S192>/Trigonometric Function' incorporates:
     *  UnitConversion: '<S159>/Unit Conversion'
     */
    V4_FUZZY_B.cosa = sin(V4_FUZZY_ConstB.UnitConversion);
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = cos
      (V4_FUZZY_ConstB.UnitConversion);
    _mm_storeu_pd(&V4_FUZZY_B.wbe_b[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
      (V4_FUZZY_B.cosa, V4_FUZZY_B.w1_c[0]), _mm_set_pd(V4_FUZZY_B.w1_c[0],
      V4_FUZZY_B.rtb_CoordinateTransformationC_b)), _mm_mul_pd(_mm_mul_pd
      (_mm_set_pd(V4_FUZZY_B.w1[0], V4_FUZZY_B.cosa), _mm_set_pd
       (V4_FUZZY_B.rtb_CoordinateTransformationC_b, V4_FUZZY_B.w1[0])),
      _mm_set_pd(1.0, -1.0))));

    /* SignalConversion generated from: '<S191>/Vector Concatenate' incorporates:
     *  Product: '<S192>/Product1'
     *  Product: '<S192>/Product2'
     *  Sum: '<S192>/Sum'
     */
    V4_FUZZY_B.wbe_b[2] = V4_FUZZY_B.LwgV1[0];

    /* Product: '<S191>/Product' incorporates:
     *  Angle2Dcm: '<S30>/Rotation Angles to Direction Cosine Matrix'
     *  Concatenate: '<S191>/Vector Concatenate'
     */
    V4_FUZZY_B.cosa = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.0;
    for (i = 0; i < 3; i++) {
      tmp_1 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&V4_FUZZY_B.RotationAnglestoDirectionCo[3 * i]), _mm_set1_pd
        (V4_FUZZY_B.wbe_b[i])), _mm_set_pd
                         (V4_FUZZY_B.rtb_CoordinateTransformationC_b,
                          V4_FUZZY_B.cosa));
      _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);
      V4_FUZZY_B.cosa = V4_FUZZY_B.dv6[0];
      V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.dv6[1];
      V4_FUZZY_B.rtb_CoordinateTransformationC_o +=
        V4_FUZZY_B.RotationAnglestoDirectionCo[3 * i + 2] * V4_FUZZY_B.wbe_b[i];
    }

    V4_FUZZY_B.Product_k[2] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;
    V4_FUZZY_B.Product_k[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;
    V4_FUZZY_B.Product_k[0] = V4_FUZZY_B.cosa;
    tmp_1 = _mm_add_pd(_mm_div_pd(_mm_mul_pd(_mm_sub_pd(_mm_set_pd
      (V4_FUZZY_B.w1[1], V4_FUZZY_B.w1_c[1]), _mm_loadu_pd
      (&V4_FUZZY_B.Product_k[0])), _mm_sub_pd(_mm_set1_pd(V4_FUZZY_B.sina),
      _mm_set1_pd(1000.0))), _mm_set1_pd(V4_FUZZY_ConstB.Sum)), _mm_loadu_pd
                       (&V4_FUZZY_B.Product_k[0]));
    _mm_storeu_pd(&V4_FUZZY_B.wbe_b[0], tmp_1);

    /* Sum: '<S187>/Sum3' incorporates:
     *  Constant: '<S187>/max_height_low'
     *  Product: '<S187>/Product1'
     *  Product: '<S191>/Product'
     *  Sum: '<S187>/Sum1'
     *  Sum: '<S187>/Sum2'
     */
    V4_FUZZY_B.wbe_b[2] = (V4_FUZZY_B.LwgV1[1] -
      V4_FUZZY_B.rtb_CoordinateTransformationC_o) * (V4_FUZZY_B.sina - 1000.0) /
      V4_FUZZY_ConstB.Sum + V4_FUZZY_B.rtb_CoordinateTransformationC_o;

    /* End of Outputs for SubSystem: '<S166>/Interpolate  velocities' */
    break;
  }

  /* End of If: '<S166>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  /* Unit Conversion - from: ft/s to: m/s
     Expression: output = (0.3048*input) + (0) */
  if (tmp_0) {
    /* MATLABSystem: '<S158>/SourceBlock' */
    V4_FUZZY_B.SourceBlock_o1_h = Sub_V4_FUZZY_417.getLatestMessage
      (&rtb_SourceBlock_o2_j);

    /* Outputs for Enabled SubSystem: '<S158>/Enabled Subsystem' */
    V4_FUZZY_EnabledSubsystem(V4_FUZZY_B.SourceBlock_o1_h, &rtb_SourceBlock_o2_j,
      &V4_FUZZY_B.EnabledSubsystem_pt);

    /* End of Outputs for SubSystem: '<S158>/Enabled Subsystem' */
  }

  /* Switch: '<S30>/Switch' incorporates:
   *  UnitConversion: '<S152>/Unit Conversion'
   */
  if (V4_FUZZY_B.EnabledSubsystem_pt.In1.data) {
    tmp_1 = _mm_mul_pd(_mm_set1_pd(0.3048), _mm_loadu_pd(&V4_FUZZY_B.wbe_b[0]));
    _mm_storeu_pd(&V4_FUZZY_B.Switch_p[0], tmp_1);

    /* Switch: '<S30>/Switch' incorporates:
     *  UnitConversion: '<S152>/Unit Conversion'
     */
    V4_FUZZY_B.Switch_p[2] = 0.3048 * V4_FUZZY_B.wbe_b[2];
  } else {
    /* Switch: '<S30>/Switch' incorporates:
     *  Constant: '<S30>/Constant'
     */
    V4_FUZZY_B.Switch_p[0] = 0.0;
    V4_FUZZY_B.Switch_p[1] = 0.0;
    V4_FUZZY_B.Switch_p[2] = 0.0;
  }

  /* End of Switch: '<S30>/Switch' */

  /* Outputs for Enabled SubSystem: '<S160>/Hpgw' incorporates:
   *  EnablePort: '<S171>/Enable'
   */
  if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo) &&
      !V4_FUZZY_DW.Hpgw_MODE) {
    (void) memset(&(V4_FUZZY_XDis.pgw_p_CSTATE), 0,
                  2*sizeof(boolean_T));

    /* InitializeConditions for Integrator: '<S171>/pgw_p' */
    V4_FUZZY_X.pgw_p_CSTATE[0] = 0.0;
    V4_FUZZY_X.pgw_p_CSTATE[1] = 0.0;
    V4_FUZZY_DW.Hpgw_MODE = true;
  }

  if (V4_FUZZY_DW.Hpgw_MODE) {
    /* Fcn: '<S171>/sqrt(0.8//V)' */
    V4_FUZZY_B.cosa = sqrt(0.8 / V4_FUZZY_B.sinb);

    /* Product: '<S171>/w3' */
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.sinb *
      V4_FUZZY_ConstB.w4;

    /* Product: '<S171>/w' incorporates:
     *  Fcn: '<S171>/sqrt(0.8//V)'
     *  Gain: '<S167>/Lw'
     *  Integrator: '<S171>/pgw_p'
     *  Math: '<S171>/L^1//3'
     *  Product: '<S171>/Lug//V1'
     *  Product: '<S171>/w1'
     *  Product: '<S171>/w2'
     *  Sum: '<S171>/Sum'
     */
    V4_FUZZY_B.w_o[0] = (V4_FUZZY_B.cosa / rt_powd_snf(V4_FUZZY_B.sinc,
      0.3333333333333333) * V4_FUZZY_ConstB.u16 * V4_FUZZY_B.Product[3] -
                         V4_FUZZY_X.pgw_p_CSTATE[0]) *
      V4_FUZZY_B.rtb_CoordinateTransformationC_b;

    /* Math: '<S171>/L^1//3' */
    if (V4_FUZZY_B.Lv[1] < 0.0) {
      V4_FUZZY_B.q_aero = -rt_powd_snf(-V4_FUZZY_B.Lv[1], 0.3333333333333333);
    } else {
      V4_FUZZY_B.q_aero = rt_powd_snf(V4_FUZZY_B.Lv[1], 0.3333333333333333);
    }

    /* Product: '<S171>/w' incorporates:
     *  Fcn: '<S171>/sqrt(0.8//V)'
     *  Integrator: '<S171>/pgw_p'
     *  Product: '<S171>/Lug//V1'
     *  Product: '<S171>/w1'
     *  Product: '<S171>/w2'
     *  Sum: '<S171>/Sum'
     */
    V4_FUZZY_B.w_o[1] = (V4_FUZZY_B.cosa / V4_FUZZY_B.q_aero *
                         V4_FUZZY_ConstB.u16 * V4_FUZZY_B.Product[3] -
                         V4_FUZZY_X.pgw_p_CSTATE[1]) *
      V4_FUZZY_B.rtb_CoordinateTransformationC_b;

    /* Product: '<S171>/sigma_w' incorporates:
     *  Integrator: '<S171>/pgw_p'
     */
    tmp_1 = _mm_mul_pd(_mm_set_pd(V4_FUZZY_B.rtb_CoordinateTransformationC_n,
      V4_FUZZY_ConstB.sigma_wg), _mm_loadu_pd(&V4_FUZZY_X.pgw_p_CSTATE[0]));

    /* Product: '<S171>/sigma_w' */
    _mm_storeu_pd(&V4_FUZZY_B.sigma_w[0], tmp_1);
  }

  /* End of Outputs for SubSystem: '<S160>/Hpgw' */

  /* Outputs for Enabled SubSystem: '<S160>/Hqgw' incorporates:
   *  EnablePort: '<S172>/Enable'
   */
  if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo) &&
      !V4_FUZZY_DW.Hqgw_MODE) {
    (void) memset(&(V4_FUZZY_XDis.qgw_p_CSTATE), 0,
                  2*sizeof(boolean_T));

    /* InitializeConditions for Integrator: '<S172>/qgw_p' */
    V4_FUZZY_X.qgw_p_CSTATE[0] = 0.0;
    V4_FUZZY_X.qgw_p_CSTATE[1] = 0.0;
    V4_FUZZY_DW.Hqgw_MODE = true;
  }

  if (V4_FUZZY_DW.Hqgw_MODE) {
    /* Gain: '<S172>/pi//4' */
    V4_FUZZY_B.sinc = 0.7853981633974483 * V4_FUZZY_B.sinb;

    /* Product: '<S172>/w' incorporates:
     *  Integrator: '<S172>/qgw_p'
     *  Product: '<S172>/wg//V'
     *  Sum: '<S172>/Sum'
     */
    V4_FUZZY_B.rtb_CoordinateTransformationC_n = (V4_FUZZY_B.LwgV1[0] /
      V4_FUZZY_B.sinb - V4_FUZZY_X.qgw_p_CSTATE[0]) * (V4_FUZZY_B.sinc /
      V4_FUZZY_ConstB.UnitConversion_n);
    V4_FUZZY_B.w_e0[0] = V4_FUZZY_B.rtb_CoordinateTransformationC_n;

    /* UnaryMinus: '<S172>/Unary Minus' */
    V4_FUZZY_B.UnaryMinus[0] = -V4_FUZZY_B.rtb_CoordinateTransformationC_n;

    /* Product: '<S172>/w' incorporates:
     *  Integrator: '<S172>/qgw_p'
     *  Product: '<S172>/wg//V'
     *  Sum: '<S172>/Sum'
     */
    V4_FUZZY_B.rtb_CoordinateTransformationC_n = (V4_FUZZY_B.LwgV1[1] /
      V4_FUZZY_B.sinb - V4_FUZZY_X.qgw_p_CSTATE[1]) * (V4_FUZZY_B.sinc /
      V4_FUZZY_ConstB.UnitConversion_n);
    V4_FUZZY_B.w_e0[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_n;

    /* UnaryMinus: '<S172>/Unary Minus' */
    V4_FUZZY_B.UnaryMinus[1] = -V4_FUZZY_B.rtb_CoordinateTransformationC_n;
  }

  /* End of Outputs for SubSystem: '<S160>/Hqgw' */

  /* Outputs for Enabled SubSystem: '<S160>/Hrgw' incorporates:
   *  EnablePort: '<S173>/Enable'
   */
  if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo) &&
      !V4_FUZZY_DW.Hrgw_MODE) {
    (void) memset(&(V4_FUZZY_XDis.rgw_p_CSTATE), 0,
                  2*sizeof(boolean_T));

    /* InitializeConditions for Integrator: '<S173>/rgw_p' */
    V4_FUZZY_X.rgw_p_CSTATE[0] = 0.0;
    V4_FUZZY_X.rgw_p_CSTATE[1] = 0.0;
    V4_FUZZY_DW.Hrgw_MODE = true;
  }

  if (V4_FUZZY_DW.Hrgw_MODE) {
    /* Product: '<S173>/vg//V' incorporates:
     *  Gain: '<S173>/pi//3'
     *  Integrator: '<S173>/rgw_p'
     *  Product: '<S173>/w'
     */
    tmp_1 = _mm_mul_pd(_mm_sub_pd(_mm_div_pd(_mm_loadu_pd(&V4_FUZZY_B.w1[0]),
      _mm_set1_pd(V4_FUZZY_B.sinb)), _mm_loadu_pd(&V4_FUZZY_X.rgw_p_CSTATE[0])),
                       _mm_div_pd(_mm_set1_pd(1.0471975511965976 *
      V4_FUZZY_B.sinb), _mm_set1_pd(V4_FUZZY_ConstB.UnitConversion_n)));

    /* Product: '<S173>/w' */
    _mm_storeu_pd(&V4_FUZZY_B.w_d[0], tmp_1);
  }

  /* End of Outputs for SubSystem: '<S160>/Hrgw' */

  /* If: '<S165>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' incorporates:
   *  Constant: '<S179>/max_height_low'
   *  Product: '<S179>/Product1'
   *  Product: '<S184>/Product1'
   *  Product: '<S184>/Product2'
   *  Product: '<S186>/Product1'
   *  Product: '<S186>/Product2'
   *  Sum: '<S179>/Sum1'
   *  Sum: '<S179>/Sum2'
   *  Sum: '<S179>/Sum3'
   *  Sum: '<S184>/Sum'
   *  Sum: '<S186>/Sum'
   */
  rtPrevAction = V4_FUZZY_DW.ifHeightMaxlowaltitudeelseifH_a;
  if (rtsiIsModeUpdateTimeStep(&(&V4_FUZZY_M)->solverInfo)) {
    if (V4_FUZZY_B.sina <= 1000.0) {
      rtAction = 0;
    } else if (V4_FUZZY_B.sina >= 2000.0) {
      rtAction = 1;
    } else {
      rtAction = 2;
    }

    V4_FUZZY_DW.ifHeightMaxlowaltitudeelseifH_a = rtAction;
  } else {
    rtAction = V4_FUZZY_DW.ifHeightMaxlowaltitudeelseifH_a;
  }

  if (rtPrevAction != rtAction) {
    rtsiSetBlockStateForSolverChangedAtMajorStep(&(&V4_FUZZY_M)->solverInfo,
      true);
  }

  switch (rtAction) {
   case 0:
    /* Outputs for IfAction SubSystem: '<S165>/Low altitude  rates' incorporates:
     *  ActionPort: '<S180>/Action Port'
     */
    /* SignalConversion generated from: '<S185>/Vector Concatenate' */
    V4_FUZZY_B.Product_k[2] = V4_FUZZY_B.w_d[0];

    /* Trigonometry: '<S186>/Trigonometric Function1' incorporates:
     *  UnitConversion: '<S159>/Unit Conversion'
     */
    V4_FUZZY_B.sina = sin(V4_FUZZY_ConstB.UnitConversion);
    V4_FUZZY_B.sinb = cos(V4_FUZZY_ConstB.UnitConversion);
    _mm_storeu_pd(&V4_FUZZY_B.Product_k[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
      (V4_FUZZY_B.sina, V4_FUZZY_B.sigma_w[0]), _mm_set_pd(V4_FUZZY_B.sigma_w[0],
      V4_FUZZY_B.sinb)), _mm_mul_pd(_mm_mul_pd(_mm_set_pd(V4_FUZZY_B.UnaryMinus
      [0], V4_FUZZY_B.sina), _mm_set_pd(V4_FUZZY_B.sinb, V4_FUZZY_B.UnaryMinus[0])),
      _mm_set_pd(1.0, -1.0))));

    /* Product: '<S185>/Product' incorporates:
     *  Angle2Dcm: '<S30>/Rotation Angles to Direction Cosine Matrix'
     *  Concatenate: '<S185>/Vector Concatenate'
     *  Product: '<S186>/Product1'
     *  Product: '<S186>/Product2'
     *  Reshape: '<S185>/Reshape1'
     *  Sum: '<S186>/Sum'
     */
    V4_FUZZY_B.cosa = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.0;
    for (i = 0; i < 3; i++) {
      tmp_1 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&V4_FUZZY_B.RotationAnglestoDirectionCo[3 * i]), _mm_set1_pd
        (V4_FUZZY_B.Product_k[i])), _mm_set_pd
                         (V4_FUZZY_B.rtb_CoordinateTransformationC_b,
                          V4_FUZZY_B.cosa));
      _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);
      V4_FUZZY_B.cosa = V4_FUZZY_B.dv6[0];
      V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.dv6[1];
      V4_FUZZY_B.rtb_CoordinateTransformationC_o +=
        V4_FUZZY_B.RotationAnglestoDirectionCo[3 * i + 2] *
        V4_FUZZY_B.Product_k[i];
    }

    V4_FUZZY_B.wbe_b[2] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;
    V4_FUZZY_B.wbe_b[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;
    V4_FUZZY_B.wbe_b[0] = V4_FUZZY_B.cosa;

    /* End of Product: '<S185>/Product' */
    /* End of Outputs for SubSystem: '<S165>/Low altitude  rates' */
    break;

   case 1:
    /* Outputs for IfAction SubSystem: '<S165>/Medium//High  altitude rates' incorporates:
     *  ActionPort: '<S181>/Action Port'
     */
    /* Gain: '<S181>/Gain' */
    V4_FUZZY_B.wbe_b[0] = V4_FUZZY_B.sigma_w[1];
    V4_FUZZY_B.wbe_b[1] = V4_FUZZY_B.UnaryMinus[1];
    V4_FUZZY_B.wbe_b[2] = V4_FUZZY_B.w_d[1];

    /* End of Outputs for SubSystem: '<S165>/Medium//High  altitude rates' */
    break;

   default:
    /* Outputs for IfAction SubSystem: '<S165>/Interpolate  rates' incorporates:
     *  ActionPort: '<S179>/Action Port'
     */
    /* Trigonometry: '<S184>/Trigonometric Function' incorporates:
     *  UnitConversion: '<S159>/Unit Conversion'
     */
    V4_FUZZY_B.sinb = sin(V4_FUZZY_ConstB.UnitConversion);
    V4_FUZZY_B.sinc = cos(V4_FUZZY_ConstB.UnitConversion);
    _mm_storeu_pd(&V4_FUZZY_B.wbe_b[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
      (V4_FUZZY_B.sinb, V4_FUZZY_B.sigma_w[0]), _mm_set_pd(V4_FUZZY_B.sigma_w[0],
      V4_FUZZY_B.sinc)), _mm_mul_pd(_mm_mul_pd(_mm_set_pd(V4_FUZZY_B.UnaryMinus
      [0], V4_FUZZY_B.sinb), _mm_set_pd(V4_FUZZY_B.sinc, V4_FUZZY_B.UnaryMinus[0])),
      _mm_set_pd(1.0, -1.0))));

    /* SignalConversion generated from: '<S183>/Vector Concatenate' incorporates:
     *  Product: '<S184>/Product1'
     *  Product: '<S184>/Product2'
     *  Sum: '<S184>/Sum'
     */
    V4_FUZZY_B.wbe_b[2] = V4_FUZZY_B.w_d[0];

    /* Product: '<S183>/Product' incorporates:
     *  Angle2Dcm: '<S30>/Rotation Angles to Direction Cosine Matrix'
     *  Concatenate: '<S183>/Vector Concatenate'
     */
    V4_FUZZY_B.cosa = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_b = 0.0;
    V4_FUZZY_B.rtb_CoordinateTransformationC_o = 0.0;
    for (i = 0; i < 3; i++) {
      tmp_1 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&V4_FUZZY_B.RotationAnglestoDirectionCo[3 * i]), _mm_set1_pd
        (V4_FUZZY_B.wbe_b[i])), _mm_set_pd
                         (V4_FUZZY_B.rtb_CoordinateTransformationC_b,
                          V4_FUZZY_B.cosa));
      _mm_storeu_pd(&V4_FUZZY_B.dv6[0], tmp_1);
      V4_FUZZY_B.cosa = V4_FUZZY_B.dv6[0];
      V4_FUZZY_B.rtb_CoordinateTransformationC_b = V4_FUZZY_B.dv6[1];
      V4_FUZZY_B.rtb_CoordinateTransformationC_o +=
        V4_FUZZY_B.RotationAnglestoDirectionCo[3 * i + 2] * V4_FUZZY_B.wbe_b[i];
    }

    V4_FUZZY_B.Product_k[2] = V4_FUZZY_B.rtb_CoordinateTransformationC_o;
    V4_FUZZY_B.Product_k[1] = V4_FUZZY_B.rtb_CoordinateTransformationC_b;
    V4_FUZZY_B.Product_k[0] = V4_FUZZY_B.cosa;
    tmp_1 = _mm_add_pd(_mm_div_pd(_mm_mul_pd(_mm_sub_pd(_mm_set_pd
      (V4_FUZZY_B.UnaryMinus[1], V4_FUZZY_B.sigma_w[1]), _mm_loadu_pd
      (&V4_FUZZY_B.Product_k[0])), _mm_sub_pd(_mm_set1_pd(V4_FUZZY_B.sina),
      _mm_set1_pd(1000.0))), _mm_set1_pd(V4_FUZZY_ConstB.Sum_a)), _mm_loadu_pd
                       (&V4_FUZZY_B.Product_k[0]));
    _mm_storeu_pd(&V4_FUZZY_B.wbe_b[0], tmp_1);

    /* Sum: '<S179>/Sum3' incorporates:
     *  Constant: '<S179>/max_height_low'
     *  Product: '<S179>/Product1'
     *  Product: '<S183>/Product'
     *  Sum: '<S179>/Sum1'
     *  Sum: '<S179>/Sum2'
     */
    V4_FUZZY_B.wbe_b[2] = (V4_FUZZY_B.w_d[1] -
      V4_FUZZY_B.rtb_CoordinateTransformationC_o) * (V4_FUZZY_B.sina - 1000.0) /
      V4_FUZZY_ConstB.Sum_a + V4_FUZZY_B.rtb_CoordinateTransformationC_o;

    /* End of Outputs for SubSystem: '<S165>/Interpolate  rates' */
    break;
  }

  /* End of If: '<S165>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  if (tmp_0) {
    /* MATLABSystem: '<S155>/SourceBlock' */
    V4_FUZZY_B.SourceBlock_o1_k = Sub_V4_FUZZY_423.getLatestMessage
      (&rtb_SourceBlock_o2_dd);

    /* Outputs for Enabled SubSystem: '<S155>/Enabled Subsystem' */
    V4_FUZZY_EnabledSubsystem(V4_FUZZY_B.SourceBlock_o1_k,
      &rtb_SourceBlock_o2_dd, &V4_FUZZY_B.EnabledSubsystem);

    /* End of Outputs for SubSystem: '<S155>/Enabled Subsystem' */
  }

  /* Switch: '<S30>/Switch1' */
  if (V4_FUZZY_B.EnabledSubsystem.In1.data) {
    /* Switch: '<S30>/Switch1' */
    V4_FUZZY_B.Switch1[0] = V4_FUZZY_B.wbe_b[0];
    V4_FUZZY_B.Switch1[1] = V4_FUZZY_B.wbe_b[1];
    V4_FUZZY_B.Switch1[2] = V4_FUZZY_B.wbe_b[2];
  } else {
    /* Switch: '<S30>/Switch1' incorporates:
     *  Constant: '<S30>/Constant2'
     */
    V4_FUZZY_B.Switch1[0] = 0.0;
    V4_FUZZY_B.Switch1[1] = 0.0;
    V4_FUZZY_B.Switch1[2] = 0.0;
  }

  /* End of Switch: '<S30>/Switch1' */
  if (tmp_0) {
    /* MATLABSystem: '<S156>/SourceBlock' */
    V4_FUZZY_B.SourceBlock_o1_c = Sub_V4_FUZZY_443.getLatestMessage
      (&V4_FUZZY_B.SourceBlock_o2_p);

    /* Outputs for Enabled SubSystem: '<S156>/Enabled Subsystem' */
    V4_FUZZY_EnabledSubsystem_k(V4_FUZZY_B.SourceBlock_o1_c,
      &V4_FUZZY_B.SourceBlock_o2_p, &V4_FUZZY_B.EnabledSubsystem_k);

    /* End of Outputs for SubSystem: '<S156>/Enabled Subsystem' */
  }

  /* Switch: '<S30>/Switch2' */
  if (V4_FUZZY_B.EnabledSubsystem_k.In1.data != 0.0) {
    /* Switch: '<S30>/Switch2' incorporates:
     *  TransferFcn: '<S30>/Transfer Fcn'
     */
    V4_FUZZY_B.Switch2 = 0.5303 * V4_FUZZY_X.TransferFcn_CSTATE[0] + 0.0 *
      V4_FUZZY_X.TransferFcn_CSTATE[1];
  } else {
    /* Switch: '<S30>/Switch2' incorporates:
     *  Constant: '<S30>/Constant3'
     */
    V4_FUZZY_B.Switch2 = 0.0;
  }

  /* End of Switch: '<S30>/Switch2' */
  if (tmp_0) {
    /* Gain: '<S150>/Output' incorporates:
     *  RandomNumber: '<S150>/White Noise'
     */
    V4_FUZZY_B.Output = 10.0 * V4_FUZZY_DW.NextOutput_k;

    /* MATLABSystem: '<S157>/SourceBlock' */
    V4_FUZZY_B.SourceBlock_o1_d = Sub_V4_FUZZY_445.getLatestMessage
      (&V4_FUZZY_B.SourceBlock_o2_k);

    /* Outputs for Enabled SubSystem: '<S157>/Enabled Subsystem' */
    V4_FUZZY_EnabledSubsystem_k(V4_FUZZY_B.SourceBlock_o1_d,
      &V4_FUZZY_B.SourceBlock_o2_k, &V4_FUZZY_B.EnabledSubsystem_p);

    /* End of Outputs for SubSystem: '<S157>/Enabled Subsystem' */
  }

  /* Switch: '<S30>/Switch3' */
  if (V4_FUZZY_B.EnabledSubsystem_p.In1.data != 0.0) {
    /* Switch: '<S30>/Switch3' incorporates:
     *  TransferFcn: '<S30>/Transfer Fcn1'
     */
    V4_FUZZY_B.Switch3_l = -0.0003571 * V4_FUZZY_X.TransferFcn1_CSTATE + 0.03571
      * V4_FUZZY_B.Output;
  } else {
    /* Switch: '<S30>/Switch3' incorporates:
     *  Constant: '<S30>/Constant4'
     */
    V4_FUZZY_B.Switch3_l = 0.0;
  }

  /* End of Switch: '<S30>/Switch3' */
  if (tmp_0) {
  }

  /* Product: '<S30>/Product2' incorporates:
   *  Math: '<S30>/Square'
   *  Math: '<S30>/Square1'
   *  Math: '<S30>/Square2'
   *  Sqrt: '<S30>/Sqrt'
   *  Sum: '<S30>/Sum2'
   */
  V4_FUZZY_B.Power = sqrt((V4_FUZZY_B.x[0] * V4_FUZZY_B.x[0] + V4_FUZZY_B.x[1] *
    V4_FUZZY_B.x[1]) + V4_FUZZY_B.x[2] * V4_FUZZY_B.x[2]) * V4_FUZZY_B.XDOT[34];

  /* Gain: '<S30>/Gain3' */
  V4_FUZZY_B.Gain3 = 0.001 * V4_FUZZY_B.Power;
  if (tmp_0) {
  }

  /* Gain: '<S30>/Gain1' incorporates:
   *  Integrator: '<S30>/Integrator1'
   */
  V4_FUZZY_B.EnergykWh = 2.7777777777777776E-7 * V4_FUZZY_X.Integrator1_CSTATE;
  if (tmp_0) {
  }

  /* Product: '<S30>/Divide' incorporates:
   *  Constant: '<S30>/thrust efficiency Cp?'
   */
  V4_FUZZY_B.powerdemand = V4_FUZZY_B.Gain3 / 0.57;

  /* Product: '<S30>/Divide1' */
  V4_FUZZY_B.loadtorque = V4_FUZZY_B.powerdemand / V4_FUZZY_ConstB.motorspeed;

  /* Sum: '<S30>/SumOLA' */
  V4_FUZZY_B.SumOLA[0] = V4_FUZZY_B.Switch_p[0];
  V4_FUZZY_B.SumOLA[1] = V4_FUZZY_B.Switch_p[1] + V4_FUZZY_B.Switch2;
  V4_FUZZY_B.SumOLA[2] = V4_FUZZY_B.Switch_p[2];

  /* Sum: '<S30>/SumOLA1' */
  V4_FUZZY_B.SumOLA1[0] = V4_FUZZY_B.Switch1[0];
  V4_FUZZY_B.SumOLA1[1] = V4_FUZZY_B.Switch1[1] + V4_FUZZY_B.Switch3_l;
  V4_FUZZY_B.SumOLA1[2] = V4_FUZZY_B.Switch1[2];
  if (rtmIsMajorTimeStep((&V4_FUZZY_M))) {
    /* Update for Integrator: '<S30>/Integrator' */
    V4_FUZZY_DW.Integrator_DWORK1 = false;

    /* Update for RateLimiter: '<Root>/Rate Limiter-theta_sp' */
    V4_FUZZY_DW.PrevY_g = V4_FUZZY_B.RateLimitertheta_sp;
    V4_FUZZY_DW.LastMajorTime = (&V4_FUZZY_M)->Timing.t[0];
    if (rtmIsMajorTimeStep((&V4_FUZZY_M)) &&
        (&V4_FUZZY_M)->Timing.TaskCounters.TID[1] == 0) {
      /* Update for Memory: '<S30>/Memory2' incorporates:
       *  Integrator: '<S30>/Integrator'
       */
      memcpy(&V4_FUZZY_DW.Memory2_PreviousInput[0], &V4_FUZZY_B.x[0], 12U *
             sizeof(real_T));

      /* Update for UnitDelay: '<Root>/Unit Delay3' */
      V4_FUZZY_DW.UnitDelay3_DSTATE[0] = V4_FUZZY_B.Switch3[0];

      /* Update for UnitDelay: '<Root>/Unit Delay2' */
      V4_FUZZY_DW.UnitDelay2_DSTATE[0] = V4_FUZZY_B.Switch2_l[0];

      /* Update for UnitDelay: '<Root>/Unit Delay3' */
      V4_FUZZY_DW.UnitDelay3_DSTATE[1] = V4_FUZZY_B.Switch3[1];

      /* Update for UnitDelay: '<Root>/Unit Delay2' */
      V4_FUZZY_DW.UnitDelay2_DSTATE[1] = V4_FUZZY_B.Switch2_l[1];

      /* Update for Memory: '<S68>/Memory' */
      V4_FUZZY_DW.Memory_PreviousInput_f = V4_FUZZY_B.AND3;

      /* Update for Memory: '<S122>/Memory' */
      V4_FUZZY_DW.Memory_PreviousInput_k = V4_FUZZY_B.AND3_p;

      /* Update for Memory: '<S30>/Memory' incorporates:
       *  Sum: '<S30>/SumOLA'
       */
      V4_FUZZY_DW.Memory_PreviousInput[0] = V4_FUZZY_B.SumOLA[0];

      /* Update for Memory: '<S30>/Memory1' incorporates:
       *  Sum: '<S30>/SumOLA1'
       */
      V4_FUZZY_DW.Memory1_PreviousInput[0] = V4_FUZZY_B.SumOLA1[0];

      /* Update for Memory: '<S30>/Memory' incorporates:
       *  Sum: '<S30>/SumOLA'
       */
      V4_FUZZY_DW.Memory_PreviousInput[1] = V4_FUZZY_B.SumOLA[1];

      /* Update for Memory: '<S30>/Memory1' incorporates:
       *  Sum: '<S30>/SumOLA1'
       */
      V4_FUZZY_DW.Memory1_PreviousInput[1] = V4_FUZZY_B.SumOLA1[1];

      /* Update for Memory: '<S30>/Memory' incorporates:
       *  Sum: '<S30>/SumOLA'
       */
      V4_FUZZY_DW.Memory_PreviousInput[2] = V4_FUZZY_B.SumOLA[2];

      /* Update for Memory: '<S30>/Memory1' incorporates:
       *  Sum: '<S30>/SumOLA1'
       */
      V4_FUZZY_DW.Memory1_PreviousInput[2] = V4_FUZZY_B.SumOLA1[2];

      /* Update for RandomNumber: '<S170>/White Noise' */
      V4_FUZZY_DW.NextOutput[0] = rt_nrand_Upu32_Yd_f_pw_snf
        (&V4_FUZZY_DW.RandSeed[0]);
      V4_FUZZY_DW.NextOutput[1] = rt_nrand_Upu32_Yd_f_pw_snf
        (&V4_FUZZY_DW.RandSeed[1]);
      V4_FUZZY_DW.NextOutput[2] = rt_nrand_Upu32_Yd_f_pw_snf
        (&V4_FUZZY_DW.RandSeed[2]);
      V4_FUZZY_DW.NextOutput[3] = rt_nrand_Upu32_Yd_f_pw_snf
        (&V4_FUZZY_DW.RandSeed[3]);

      /* Update for RandomNumber: '<S150>/White Noise' */
      V4_FUZZY_DW.NextOutput_k = rt_nrand_Upu32_Yd_f_pw_snf
        (&V4_FUZZY_DW.RandSeed_a);
    }

    /* ContTimeOutputInconsistentWithStateAtMajorOutputFlag is set, need to run a minor output */
    if (rtmIsMajorTimeStep((&V4_FUZZY_M))) {
      if (rtsiGetContTimeOutputInconsistentWithStateAtMajorStep(&(&V4_FUZZY_M)
           ->solverInfo)) {
        rtsiSetSimTimeStep(&(&V4_FUZZY_M)->solverInfo,MINOR_TIME_STEP);
        rtsiSetContTimeOutputInconsistentWithStateAtMajorStep(&(&V4_FUZZY_M)
          ->solverInfo, false);
        V4_FUZZY::step();
        rtsiSetSimTimeStep(&(&V4_FUZZY_M)->solverInfo, MAJOR_TIME_STEP);
      }
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep((&V4_FUZZY_M))) {
    rt_ertODEUpdateContinuousStates(&(&V4_FUZZY_M)->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++(&V4_FUZZY_M)->Timing.clockTick0)) {
      ++(&V4_FUZZY_M)->Timing.clockTickH0;
    }

    (&V4_FUZZY_M)->Timing.t[0] = rtsiGetSolverStopTime(&(&V4_FUZZY_M)
      ->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      (&V4_FUZZY_M)->Timing.clockTick1++;
      if (!(&V4_FUZZY_M)->Timing.clockTick1) {
        (&V4_FUZZY_M)->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void V4_FUZZY::V4_FUZZY_derivatives()
{
  XDot_V4_FUZZY_T *_rtXdot;
  real_T tmp[2];
  _rtXdot = ((XDot_V4_FUZZY_T *) (&V4_FUZZY_M)->derivs);

  /* Derivatives for Integrator: '<S30>/Integrator' */
  if (!V4_FUZZY_B.Compare) {
    memcpy(&_rtXdot->Integrator_CSTATE[0], &V4_FUZZY_B.XDOT[0], 12U * sizeof
           (real_T));
  } else {
    /* level reset is active */
    memset(&_rtXdot->Integrator_CSTATE[0], 0, 12U * sizeof(real_T));
  }

  /* End of Derivatives for Integrator: '<S30>/Integrator' */

  /* Derivatives for Integrator: '<S132>/Integrator' */
  _rtXdot->Integrator_CSTATE_d = V4_FUZZY_B.Switch_f;

  /* Derivatives for Integrator: '<S127>/Filter' */
  _rtXdot->Filter_CSTATE = V4_FUZZY_B.FilterCoefficient;

  /* Derivatives for Integrator: '<S78>/Integrator' */
  _rtXdot->Integrator_CSTATE_e = V4_FUZZY_B.Switch;

  /* Derivatives for Integrator: '<S73>/Filter' */
  _rtXdot->Filter_CSTATE_f = V4_FUZZY_B.FilterCoefficient_a;

  /* Derivatives for Enabled SubSystem: '<S161>/Hugw(s)' */
  if (V4_FUZZY_DW.Hugws_MODE) {
    /* Derivatives for Integrator: '<S174>/ug_p' */
    _rtXdot->ug_p_CSTATE[0] = V4_FUZZY_B.w_n[0];
    _rtXdot->ug_p_CSTATE[1] = V4_FUZZY_B.w_n[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_V4_FUZZY_T *) (&V4_FUZZY_M)->derivs)->ug_p_CSTATE[0]);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S161>/Hugw(s)' */

  /* Derivatives for Enabled SubSystem: '<S161>/Hvgw(s)' */
  if (V4_FUZZY_DW.Hvgws_MODE) {
    /* Derivatives for Integrator: '<S175>/vg_p1' */
    _rtXdot->vg_p1_CSTATE[0] = V4_FUZZY_B.w_g[0];

    /* Derivatives for Integrator: '<S175>/vgw_p2' */
    _rtXdot->vgw_p2_CSTATE[0] = V4_FUZZY_B.w_e[0];

    /* Derivatives for Integrator: '<S175>/vg_p1' */
    _rtXdot->vg_p1_CSTATE[1] = V4_FUZZY_B.w_g[1];

    /* Derivatives for Integrator: '<S175>/vgw_p2' */
    _rtXdot->vgw_p2_CSTATE[1] = V4_FUZZY_B.w_e[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_V4_FUZZY_T *) (&V4_FUZZY_M)->derivs)->vg_p1_CSTATE[0]);
      for (i1=0; i1 < 4; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S161>/Hvgw(s)' */

  /* Derivatives for Enabled SubSystem: '<S161>/Hwgw(s)' */
  if (V4_FUZZY_DW.Hwgws_MODE) {
    /* Derivatives for Integrator: '<S176>/wg_p1' */
    _rtXdot->wg_p1_CSTATE[0] = V4_FUZZY_B.w[0];

    /* Derivatives for Integrator: '<S176>/wg_p2' */
    _rtXdot->wg_p2_CSTATE[0] = V4_FUZZY_B.w_a[0];

    /* Derivatives for Integrator: '<S176>/wg_p1' */
    _rtXdot->wg_p1_CSTATE[1] = V4_FUZZY_B.w[1];

    /* Derivatives for Integrator: '<S176>/wg_p2' */
    _rtXdot->wg_p2_CSTATE[1] = V4_FUZZY_B.w_a[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_V4_FUZZY_T *) (&V4_FUZZY_M)->derivs)->wg_p1_CSTATE[0]);
      for (i1=0; i1 < 4; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S161>/Hwgw(s)' */

  /* Derivatives for Enabled SubSystem: '<S160>/Hpgw' */
  if (V4_FUZZY_DW.Hpgw_MODE) {
    /* Derivatives for Integrator: '<S171>/pgw_p' */
    _rtXdot->pgw_p_CSTATE[0] = V4_FUZZY_B.w_o[0];
    _rtXdot->pgw_p_CSTATE[1] = V4_FUZZY_B.w_o[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_V4_FUZZY_T *) (&V4_FUZZY_M)->derivs)->pgw_p_CSTATE[0]);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S160>/Hpgw' */

  /* Derivatives for Enabled SubSystem: '<S160>/Hqgw' */
  if (V4_FUZZY_DW.Hqgw_MODE) {
    /* Derivatives for Integrator: '<S172>/qgw_p' */
    _rtXdot->qgw_p_CSTATE[0] = V4_FUZZY_B.w_e0[0];
    _rtXdot->qgw_p_CSTATE[1] = V4_FUZZY_B.w_e0[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_V4_FUZZY_T *) (&V4_FUZZY_M)->derivs)->qgw_p_CSTATE[0]);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S160>/Hqgw' */

  /* Derivatives for Enabled SubSystem: '<S160>/Hrgw' */
  if (V4_FUZZY_DW.Hrgw_MODE) {
    /* Derivatives for Integrator: '<S173>/rgw_p' */
    _rtXdot->rgw_p_CSTATE[0] = V4_FUZZY_B.w_d[0];
    _rtXdot->rgw_p_CSTATE[1] = V4_FUZZY_B.w_d[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_V4_FUZZY_T *) (&V4_FUZZY_M)->derivs)->rgw_p_CSTATE[0]);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S160>/Hrgw' */

  /* Derivatives for TransferFcn: '<S30>/Transfer Fcn' */
  _rtXdot->TransferFcn_CSTATE[0] = 0.0;
  _rtXdot->TransferFcn_CSTATE[0] += -0.898 * V4_FUZZY_X.TransferFcn_CSTATE[0];
  _rtXdot->TransferFcn_CSTATE[1] = 0.0;
  _rtXdot->TransferFcn_CSTATE[0] += -0.806 * V4_FUZZY_X.TransferFcn_CSTATE[1];
  _mm_storeu_pd(&tmp[0], _mm_add_pd(_mm_set_pd(_rtXdot->TransferFcn_CSTATE[0],
    V4_FUZZY_X.TransferFcn_CSTATE[0]), _mm_set_pd(V4_FUZZY_B.Output,
    _rtXdot->TransferFcn_CSTATE[1])));
  _rtXdot->TransferFcn_CSTATE[1] = tmp[0];
  _rtXdot->TransferFcn_CSTATE[0] = tmp[1];

  /* Derivatives for TransferFcn: '<S30>/Transfer Fcn1' */
  _rtXdot->TransferFcn1_CSTATE = 0.0;
  _rtXdot->TransferFcn1_CSTATE += -0.01 * V4_FUZZY_X.TransferFcn1_CSTATE;
  _rtXdot->TransferFcn1_CSTATE += V4_FUZZY_B.Output;

  /* Derivatives for Integrator: '<S30>/Integrator1' */
  _rtXdot->Integrator1_CSTATE = V4_FUZZY_B.Power;
}

/* Model initialize function */
void V4_FUZZY::initialize()
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&(&V4_FUZZY_M)->solverInfo, &(&V4_FUZZY_M)
                          ->Timing.simTimeStep);
    rtsiSetTPtr(&(&V4_FUZZY_M)->solverInfo, &rtmGetTPtr((&V4_FUZZY_M)));
    rtsiSetStepSizePtr(&(&V4_FUZZY_M)->solverInfo, &(&V4_FUZZY_M)
                       ->Timing.stepSize0);
    rtsiSetdXPtr(&(&V4_FUZZY_M)->solverInfo, &(&V4_FUZZY_M)->derivs);
    rtsiSetContStatesPtr(&(&V4_FUZZY_M)->solverInfo, (real_T **) &(&V4_FUZZY_M
                         )->contStates);
    rtsiSetNumContStatesPtr(&(&V4_FUZZY_M)->solverInfo, &(&V4_FUZZY_M)
      ->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&(&V4_FUZZY_M)->solverInfo, &(&V4_FUZZY_M)
      ->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&(&V4_FUZZY_M)->solverInfo, &(&V4_FUZZY_M)
      ->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&(&V4_FUZZY_M)->solverInfo, &(&V4_FUZZY_M
      )->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&(&V4_FUZZY_M)->solverInfo, (boolean_T**)
      &(&V4_FUZZY_M)->contStateDisabled);
    rtsiSetErrorStatusPtr(&(&V4_FUZZY_M)->solverInfo, (&rtmGetErrorStatus
      ((&V4_FUZZY_M))));
    rtsiSetRTModelPtr(&(&V4_FUZZY_M)->solverInfo, (&V4_FUZZY_M));
  }

  rtsiSetSimTimeStep(&(&V4_FUZZY_M)->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&(&V4_FUZZY_M)->solverInfo, false);
  rtsiSetIsContModeFrozen(&(&V4_FUZZY_M)->solverInfo, false);
  (&V4_FUZZY_M)->intgData.y = (&V4_FUZZY_M)->odeY;
  (&V4_FUZZY_M)->intgData.f[0] = (&V4_FUZZY_M)->odeF[0];
  (&V4_FUZZY_M)->intgData.f[1] = (&V4_FUZZY_M)->odeF[1];
  (&V4_FUZZY_M)->intgData.f[2] = (&V4_FUZZY_M)->odeF[2];
  (&V4_FUZZY_M)->intgData.f[3] = (&V4_FUZZY_M)->odeF[3];
  (&V4_FUZZY_M)->contStates = ((X_V4_FUZZY_T *) &V4_FUZZY_X);
  (&V4_FUZZY_M)->contStateDisabled = ((XDis_V4_FUZZY_T *) &V4_FUZZY_XDis);
  (&V4_FUZZY_M)->Timing.tStart = (0.0);
  rtsiSetSolverData(&(&V4_FUZZY_M)->solverInfo, static_cast<void *>
                    (&(&V4_FUZZY_M)->intgData));
  rtsiSetSolverName(&(&V4_FUZZY_M)->solverInfo,"ode4");
  rtmSetTPtr((&V4_FUZZY_M), &(&V4_FUZZY_M)->Timing.tArray[0]);
  (&V4_FUZZY_M)->Timing.stepSize0 = 0.01;
  rtmSetFirstInitCond((&V4_FUZZY_M), 1);

  {
    rmw_qos_profile_t qos_profile;
    int32_T i;
    static const char_T b_zeroDelimTopic[18] = "/ekranoplano/odom";

    /* Start for InitialCondition: '<S30>/IC' */
    memcpy(&V4_FUZZY_B.IC[0], &V4_FUZZY_ConstP.pooled11[0], 12U * sizeof(real_T));

    /* Start for InitialCondition: '<S30>/IC' */
    V4_FUZZY_DW.IC_FirstOutputTime = true;

    /* Start for MATLABSystem: '<Root>/Coordinate Transformation Conversion' */
    V4_FUZZY_DW.objisempty_d = true;
    V4_FUZZY_DW.obj_c.isInitialized = 1;

    /* Start for MATLABSystem: '<S25>/SinkBlock' */
    V4_FUZZY_DW.obj_i.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_i.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_m = true;
    V4_FUZZY_DW.obj_i.isSetupComplete = false;
    V4_FUZZY_DW.obj_i.isInitialized = 1;
    V4_FUZZY_Publisher_setupImpl(&V4_FUZZY_DW.obj_i);
    V4_FUZZY_DW.obj_i.isSetupComplete = true;

    /* Start for MATLABSystem: '<S26>/SinkBlock' */
    V4_FUZZY_DW.obj_o.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_o.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_ev = true;
    V4_FUZZY_DW.obj_o.isSetupComplete = false;
    V4_FUZZY_DW.obj_o.isInitialized = 1;
    qos_profile = V4_ROS2PubSubBase_setQOSProfile(rmw_qos_profile_default, 10.0,
      (rtInf), (rtInf), (rtInf),
      V4_FUZZY_DW.obj_o.QOSAvoidROSNamespaceConventions);
    for (i = 0; i < 18; i++) {
      V4_FUZZY_B.b_zeroDelimTopic_p[i] = b_zeroDelimTopic[i];
    }

    Pub_V4_FUZZY_697.createPublisher(&V4_FUZZY_B.b_zeroDelimTopic_p[0],
      qos_profile);
    V4_FUZZY_DW.obj_o.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S26>/SinkBlock' */

    /* Start for MATLABSystem: '<S27>/SinkBlock' */
    V4_FUZZY_DW.obj_dl.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_dl.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_ld = true;
    V4_FUZZY_DW.obj_dl.isSetupComplete = false;
    V4_FUZZY_DW.obj_dl.isInitialized = 1;
    V4_FUZZY_Publisher_setupImpl_l(&V4_FUZZY_DW.obj_dl);
    V4_FUZZY_DW.obj_dl.isSetupComplete = true;

    /* Start for MATLABSystem: '<S28>/SinkBlock' */
    V4_FUZZY_DW.obj_d.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_d.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_cr = true;
    V4_FUZZY_DW.obj_d.isSetupComplete = false;
    V4_FUZZY_DW.obj_d.isInitialized = 1;
    V4_FUZZY_Publisher_setupImpl_lz(&V4_FUZZY_DW.obj_d);
    V4_FUZZY_DW.obj_d.isSetupComplete = true;

    /* Start for MATLABSystem: '<S31>/SourceBlock' */
    V4_FUZZY_DW.obj_m.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_m.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_g = true;
    V4_FUZZY_DW.obj_m.isSetupComplete = false;
    V4_FUZZY_DW.obj_m.isInitialized = 1;
    V4_FU_Subscriber_setupImpl_lziw(&V4_FUZZY_DW.obj_m);
    V4_FUZZY_DW.obj_m.isSetupComplete = true;

    /* Start for MATLABSystem: '<S32>/SourceBlock' */
    V4_FUZZY_DW.obj_k.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_k.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty = true;
    V4_FUZZY_DW.obj_k.isSetupComplete = false;
    V4_FUZZY_DW.obj_k.isInitialized = 1;
    V4_F_Subscriber_setupImpl_lziwo(&V4_FUZZY_DW.obj_k);
    V4_FUZZY_DW.obj_k.isSetupComplete = true;

    /* Start for Atomic SubSystem: '<Root>/Call Service' */
    /* Start for MATLABSystem: '<S6>/ServiceCaller' */
    V4_FUZZY_DW.obj.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_f = true;
    V4_FUZZY_DW.obj.isSetupComplete = false;
    V4_FUZZY_DW.obj.isInitialized = 1;
    V4_FUZZ_ServiceCaller_setupImpl(&V4_FUZZY_DW.obj);
    V4_FUZZY_DW.obj.isSetupComplete = true;

    /* End of Start for SubSystem: '<Root>/Call Service' */

    /* Start for Enabled SubSystem: '<S161>/Hugw(s)' */
    (void) memset(&(V4_FUZZY_XDis.ug_p_CSTATE), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S161>/Hugw(s)' */

    /* Start for Enabled SubSystem: '<S161>/Hvgw(s)' */
    (void) memset(&(V4_FUZZY_XDis.vg_p1_CSTATE), 1,
                  4*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S161>/Hvgw(s)' */

    /* Start for Enabled SubSystem: '<S161>/Hwgw(s)' */
    (void) memset(&(V4_FUZZY_XDis.wg_p1_CSTATE), 1,
                  4*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S161>/Hwgw(s)' */

    /* Start for If: '<S166>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
    V4_FUZZY_DW.ifHeightMaxlowaltitudeelseifHei = -1;

    /* Start for MATLABSystem: '<S158>/SourceBlock' */
    V4_FUZZY_DW.obj_h.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_h.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_a = true;
    V4_FUZZY_DW.obj_h.isSetupComplete = false;
    V4_FUZZY_DW.obj_h.isInitialized = 1;
    V4_FUZ_Subscriber_setupImpl_lzi(&V4_FUZZY_DW.obj_h);
    V4_FUZZY_DW.obj_h.isSetupComplete = true;

    /* Start for Enabled SubSystem: '<S160>/Hpgw' */
    (void) memset(&(V4_FUZZY_XDis.pgw_p_CSTATE), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S160>/Hpgw' */

    /* Start for Enabled SubSystem: '<S160>/Hqgw' */
    (void) memset(&(V4_FUZZY_XDis.qgw_p_CSTATE), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S160>/Hqgw' */

    /* Start for Enabled SubSystem: '<S160>/Hrgw' */
    (void) memset(&(V4_FUZZY_XDis.rgw_p_CSTATE), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S160>/Hrgw' */

    /* Start for If: '<S165>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
    V4_FUZZY_DW.ifHeightMaxlowaltitudeelseifH_a = -1;

    /* Start for MATLABSystem: '<S155>/SourceBlock' */
    V4_FUZZY_DW.obj_h4.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_h4.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_c = true;
    V4_FUZZY_DW.obj_h4.isSetupComplete = false;
    V4_FUZZY_DW.obj_h4.isInitialized = 1;
    V4_FUZZY_Subscriber_setupImpl(&V4_FUZZY_DW.obj_h4);
    V4_FUZZY_DW.obj_h4.isSetupComplete = true;

    /* Start for MATLABSystem: '<S156>/SourceBlock' */
    V4_FUZZY_DW.obj_hy.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_hy.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_l = true;
    V4_FUZZY_DW.obj_hy.isSetupComplete = false;
    V4_FUZZY_DW.obj_hy.isInitialized = 1;
    V4_FUZZY_Subscriber_setupImpl_l(&V4_FUZZY_DW.obj_hy);
    V4_FUZZY_DW.obj_hy.isSetupComplete = true;

    /* Start for MATLABSystem: '<S157>/SourceBlock' */
    V4_FUZZY_DW.obj_p.QOSAvoidROSNamespaceConventions = false;
    V4_FUZZY_DW.obj_p.matlabCodegenIsDeleted = false;
    V4_FUZZY_DW.objisempty_e = true;
    V4_FUZZY_DW.obj_p.isSetupComplete = false;
    V4_FUZZY_DW.obj_p.isInitialized = 1;
    V4_FUZZ_Subscriber_setupImpl_lz(&V4_FUZZY_DW.obj_p);
    V4_FUZZY_DW.obj_p.isSetupComplete = true;
  }

  V4_FUZZY_PrevZCX.Integrator_Reset_ZCE = UNINITIALIZED_ZCSIG;

  /* InitializeConditions for Memory: '<S30>/Memory2' */
  memcpy(&V4_FUZZY_DW.Memory2_PreviousInput[0], &V4_FUZZY_ConstP.pooled11[0],
         12U * sizeof(real_T));

  /* InitializeConditions for Integrator: '<S30>/Integrator' */
  if (rtmIsFirstInitCond((&V4_FUZZY_M))) {
    V4_FUZZY_X.Integrator_CSTATE[0] = 0.1;
    V4_FUZZY_X.Integrator_CSTATE[1] = 0.0;
    V4_FUZZY_X.Integrator_CSTATE[2] = 0.0;
    V4_FUZZY_X.Integrator_CSTATE[3] = 0.0;
    V4_FUZZY_X.Integrator_CSTATE[4] = 0.0;
    V4_FUZZY_X.Integrator_CSTATE[5] = 0.0;
    V4_FUZZY_X.Integrator_CSTATE[6] = 0.0;
    V4_FUZZY_X.Integrator_CSTATE[7] = 0.026179938779914945;
    V4_FUZZY_X.Integrator_CSTATE[8] = 0.0;
    V4_FUZZY_X.Integrator_CSTATE[9] = 0.0;
    V4_FUZZY_X.Integrator_CSTATE[10] = 0.0;
    V4_FUZZY_X.Integrator_CSTATE[11] = -0.5;
  }

  V4_FUZZY_DW.Integrator_DWORK1 = true;

  /* End of InitializeConditions for Integrator: '<S30>/Integrator' */

  /* InitializeConditions for Integrator: '<S132>/Integrator' */
  V4_FUZZY_X.Integrator_CSTATE_d = 0.0;

  /* InitializeConditions for Integrator: '<S127>/Filter' */
  V4_FUZZY_X.Filter_CSTATE = 0.0;

  /* InitializeConditions for UnitDelay: '<Root>/Unit Delay3' */
  V4_FUZZY_DW.UnitDelay3_DSTATE[0] = 1.0;
  V4_FUZZY_DW.UnitDelay3_DSTATE[1] = 1.0;

  /* InitializeConditions for RateLimiter: '<Root>/Rate Limiter-theta_sp' */
  V4_FUZZY_DW.LastMajorTime = (rtInf);

  /* InitializeConditions for Integrator: '<S78>/Integrator' */
  V4_FUZZY_X.Integrator_CSTATE_e = 0.0;

  /* InitializeConditions for Integrator: '<S73>/Filter' */
  V4_FUZZY_X.Filter_CSTATE_f = 0.0;

  /* InitializeConditions for RandomNumber: '<S170>/White Noise' */
  V4_FUZZY_DW.RandSeed[0] = 1529675776U;
  V4_FUZZY_DW.NextOutput[0] = rt_nrand_Upu32_Yd_f_pw_snf(&V4_FUZZY_DW.RandSeed[0]);
  V4_FUZZY_DW.RandSeed[1] = 1529741312U;
  V4_FUZZY_DW.NextOutput[1] = rt_nrand_Upu32_Yd_f_pw_snf(&V4_FUZZY_DW.RandSeed[1]);
  V4_FUZZY_DW.RandSeed[2] = 1529806848U;
  V4_FUZZY_DW.NextOutput[2] = rt_nrand_Upu32_Yd_f_pw_snf(&V4_FUZZY_DW.RandSeed[2]);
  V4_FUZZY_DW.RandSeed[3] = 1529872384U;
  V4_FUZZY_DW.NextOutput[3] = rt_nrand_Upu32_Yd_f_pw_snf(&V4_FUZZY_DW.RandSeed[3]);

  /* InitializeConditions for TransferFcn: '<S30>/Transfer Fcn' */
  V4_FUZZY_X.TransferFcn_CSTATE[0] = 0.0;
  V4_FUZZY_X.TransferFcn_CSTATE[1] = 0.0;

  /* InitializeConditions for RandomNumber: '<S150>/White Noise' */
  V4_FUZZY_DW.RandSeed_a = 1529675776U;
  V4_FUZZY_DW.NextOutput_k = rt_nrand_Upu32_Yd_f_pw_snf(&V4_FUZZY_DW.RandSeed_a);

  /* InitializeConditions for TransferFcn: '<S30>/Transfer Fcn1' */
  V4_FUZZY_X.TransferFcn1_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S30>/Integrator1' */
  V4_FUZZY_X.Integrator1_CSTATE = 0.0;

  /* SystemInitialize for Enabled SubSystem: '<S31>/Enabled Subsystem' */
  V4_FUZZ_EnabledSubsystem_p_Init(&V4_FUZZY_B.EnabledSubsystem_b);

  /* End of SystemInitialize for SubSystem: '<S31>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<S32>/Enabled Subsystem' */
  V4_FUZZ_EnabledSubsystem_p_Init(&V4_FUZZY_B.EnabledSubsystem_a);

  /* End of SystemInitialize for SubSystem: '<S32>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hugw(s)' */
  /* InitializeConditions for Integrator: '<S174>/ug_p' */
  V4_FUZZY_X.ug_p_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hugw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hvgw(s)' */
  /* InitializeConditions for Integrator: '<S175>/vg_p1' */
  V4_FUZZY_X.vg_p1_CSTATE[0] = 0.0;

  /* InitializeConditions for Integrator: '<S175>/vgw_p2' */
  V4_FUZZY_X.vgw_p2_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hvgw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hwgw(s)' */
  /* InitializeConditions for Integrator: '<S176>/wg_p1' */
  V4_FUZZY_X.wg_p1_CSTATE[0] = 0.0;

  /* InitializeConditions for Integrator: '<S176>/wg_p2' */
  V4_FUZZY_X.wg_p2_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hwgw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hugw(s)' */
  /* InitializeConditions for Integrator: '<S174>/ug_p' */
  V4_FUZZY_X.ug_p_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hugw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hvgw(s)' */
  /* InitializeConditions for Integrator: '<S175>/vg_p1' */
  V4_FUZZY_X.vg_p1_CSTATE[1] = 0.0;

  /* InitializeConditions for Integrator: '<S175>/vgw_p2' */
  V4_FUZZY_X.vgw_p2_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hvgw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hwgw(s)' */
  /* InitializeConditions for Integrator: '<S176>/wg_p1' */
  V4_FUZZY_X.wg_p1_CSTATE[1] = 0.0;

  /* InitializeConditions for Integrator: '<S176>/wg_p2' */
  V4_FUZZY_X.wg_p2_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hwgw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S158>/Enabled Subsystem' */
  V4_FUZZY_EnabledSubsystem_Init(&V4_FUZZY_B.EnabledSubsystem_pt);

  /* End of SystemInitialize for SubSystem: '<S158>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<S160>/Hpgw' */
  /* InitializeConditions for Integrator: '<S171>/pgw_p' */
  V4_FUZZY_X.pgw_p_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S160>/Hpgw' */

  /* SystemInitialize for Enabled SubSystem: '<S160>/Hqgw' */
  /* InitializeConditions for Integrator: '<S172>/qgw_p' */
  V4_FUZZY_X.qgw_p_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S160>/Hqgw' */

  /* SystemInitialize for Enabled SubSystem: '<S160>/Hrgw' */
  /* InitializeConditions for Integrator: '<S173>/rgw_p' */
  V4_FUZZY_X.rgw_p_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S160>/Hrgw' */

  /* SystemInitialize for Enabled SubSystem: '<S160>/Hpgw' */
  /* InitializeConditions for Integrator: '<S171>/pgw_p' */
  V4_FUZZY_X.pgw_p_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S160>/Hpgw' */

  /* SystemInitialize for Enabled SubSystem: '<S160>/Hqgw' */
  /* InitializeConditions for Integrator: '<S172>/qgw_p' */
  V4_FUZZY_X.qgw_p_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S160>/Hqgw' */

  /* SystemInitialize for Enabled SubSystem: '<S160>/Hrgw' */
  /* InitializeConditions for Integrator: '<S173>/rgw_p' */
  V4_FUZZY_X.rgw_p_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S160>/Hrgw' */

  /* SystemInitialize for Enabled SubSystem: '<S155>/Enabled Subsystem' */
  V4_FUZZY_EnabledSubsystem_Init(&V4_FUZZY_B.EnabledSubsystem);

  /* End of SystemInitialize for SubSystem: '<S155>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<S156>/Enabled Subsystem' */
  V4_FUZZ_EnabledSubsystem_p_Init(&V4_FUZZY_B.EnabledSubsystem_k);

  /* End of SystemInitialize for SubSystem: '<S156>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<S157>/Enabled Subsystem' */
  V4_FUZZ_EnabledSubsystem_p_Init(&V4_FUZZY_B.EnabledSubsystem_p);

  /* End of SystemInitialize for SubSystem: '<S157>/Enabled Subsystem' */

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond((&V4_FUZZY_M))) {
    rtmSetFirstInitCond((&V4_FUZZY_M), 0);
  }
}

/* Model terminate function */
void V4_FUZZY::terminate()
{
  /* Terminate for MATLABSystem: '<S25>/SinkBlock' */
  if (!V4_FUZZY_DW.obj_i.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_i.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_i.isInitialized == 1) &&
        V4_FUZZY_DW.obj_i.isSetupComplete) {
      Pub_V4_FUZZY_548.resetPublisherPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S25>/SinkBlock' */

  /* Terminate for MATLABSystem: '<S26>/SinkBlock' */
  if (!V4_FUZZY_DW.obj_o.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_o.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_o.isInitialized == 1) &&
        V4_FUZZY_DW.obj_o.isSetupComplete) {
      Pub_V4_FUZZY_697.resetPublisherPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S26>/SinkBlock' */

  /* Terminate for MATLABSystem: '<S27>/SinkBlock' */
  if (!V4_FUZZY_DW.obj_dl.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_dl.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_dl.isInitialized == 1) &&
        V4_FUZZY_DW.obj_dl.isSetupComplete) {
      Pub_V4_FUZZY_811.resetPublisherPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S27>/SinkBlock' */

  /* Terminate for MATLABSystem: '<S28>/SinkBlock' */
  if (!V4_FUZZY_DW.obj_d.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_d.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_d.isInitialized == 1) &&
        V4_FUZZY_DW.obj_d.isSetupComplete) {
      Pub_V4_FUZZY_835.resetPublisherPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S28>/SinkBlock' */

  /* Terminate for MATLABSystem: '<S31>/SourceBlock' */
  if (!V4_FUZZY_DW.obj_m.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_m.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_m.isInitialized == 1) &&
        V4_FUZZY_DW.obj_m.isSetupComplete) {
      Sub_V4_FUZZY_435.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S31>/SourceBlock' */

  /* Terminate for MATLABSystem: '<S32>/SourceBlock' */
  if (!V4_FUZZY_DW.obj_k.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_k.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_k.isInitialized == 1) &&
        V4_FUZZY_DW.obj_k.isSetupComplete) {
      Sub_V4_FUZZY_377.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S32>/SourceBlock' */

  /* Terminate for Atomic SubSystem: '<Root>/Call Service' */
  /* Terminate for MATLABSystem: '<S6>/ServiceCaller' */
  if (!V4_FUZZY_DW.obj.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj.isInitialized == 1) && V4_FUZZY_DW.obj.isSetupComplete)
    {
      ServCall_V4_FUZZY_326.resetSvcClientPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S6>/ServiceCaller' */
  /* End of Terminate for SubSystem: '<Root>/Call Service' */

  /* Terminate for MATLABSystem: '<S158>/SourceBlock' */
  if (!V4_FUZZY_DW.obj_h.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_h.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_h.isInitialized == 1) &&
        V4_FUZZY_DW.obj_h.isSetupComplete) {
      Sub_V4_FUZZY_417.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S158>/SourceBlock' */

  /* Terminate for MATLABSystem: '<S155>/SourceBlock' */
  if (!V4_FUZZY_DW.obj_h4.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_h4.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_h4.isInitialized == 1) &&
        V4_FUZZY_DW.obj_h4.isSetupComplete) {
      Sub_V4_FUZZY_423.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S155>/SourceBlock' */

  /* Terminate for MATLABSystem: '<S156>/SourceBlock' */
  if (!V4_FUZZY_DW.obj_hy.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_hy.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_hy.isInitialized == 1) &&
        V4_FUZZY_DW.obj_hy.isSetupComplete) {
      Sub_V4_FUZZY_443.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S156>/SourceBlock' */

  /* Terminate for MATLABSystem: '<S157>/SourceBlock' */
  if (!V4_FUZZY_DW.obj_p.matlabCodegenIsDeleted) {
    V4_FUZZY_DW.obj_p.matlabCodegenIsDeleted = true;
    if ((V4_FUZZY_DW.obj_p.isInitialized == 1) &&
        V4_FUZZY_DW.obj_p.isSetupComplete) {
      Sub_V4_FUZZY_445.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S157>/SourceBlock' */
}

/* Constructor */
V4_FUZZY::V4_FUZZY() :
  V4_FUZZY_B(),
  V4_FUZZY_DW(),
  V4_FUZZY_X(),
  V4_FUZZY_XDis(),
  V4_FUZZY_PrevZCX(),
  V4_FUZZY_M()
{
  /* Currently there is no constructor body generated.*/
}

/* Destructor */
V4_FUZZY::~V4_FUZZY()
{
  /* Currently there is no destructor body generated.*/
}

/* Real-Time Model get method */
RT_MODEL_V4_FUZZY_T * V4_FUZZY::getRTM()
{
  return (&V4_FUZZY_M);
}
