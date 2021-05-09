//
// MATLAB Compiler: 6.6 (R2018a)
// Date: Thu Oct 18 09:03:30 2018
// Arguments:
// "-B""macro_default""-W""cpplib:fit_3d_plane,all""-T""link:lib""-d""C:\Users\W
// angbingqian\Documents\MATLAB\TLS\fit_3d_plane\for_testing""-v""C:\Users\Wangb
// ingqian\Documents\MATLAB\TLS\fit_3D_data.m"
//

#ifndef __fit_3d_plane_h
#define __fit_3d_plane_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_fit_3d_plane_C_API 
#define LIB_fit_3d_plane_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_fit_3d_plane_C_API 
bool MW_CALL_CONV fit_3d_planeInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_fit_3d_plane_C_API 
bool MW_CALL_CONV fit_3d_planeInitialize(void);

extern LIB_fit_3d_plane_C_API 
void MW_CALL_CONV fit_3d_planeTerminate(void);

extern LIB_fit_3d_plane_C_API 
void MW_CALL_CONV fit_3d_planePrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_fit_3d_plane_C_API 
bool MW_CALL_CONV mlxFit_3D_data(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#ifdef __cplusplus
}
#endif


/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__MINGW64__)

#ifdef EXPORTING_fit_3d_plane
#define PUBLIC_fit_3d_plane_CPP_API __declspec(dllexport)
#else
#define PUBLIC_fit_3d_plane_CPP_API __declspec(dllimport)
#endif

#define LIB_fit_3d_plane_CPP_API PUBLIC_fit_3d_plane_CPP_API

#else

#if !defined(LIB_fit_3d_plane_CPP_API)
#if defined(LIB_fit_3d_plane_C_API)
#define LIB_fit_3d_plane_CPP_API LIB_fit_3d_plane_C_API
#else
#define LIB_fit_3d_plane_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_fit_3d_plane_CPP_API void MW_CALL_CONV fit_3D_data(int nargout, mwArray& Err, mwArray& N, mwArray& P, const mwArray& XData, const mwArray& YData, const mwArray& ZData, const mwArray& geometry, const mwArray& visualization, const mwArray& sod);

/* C++ INTERFACE -- WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */
#endif

#endif
