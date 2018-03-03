#include "EKF_DCM.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "Timing_Manage.h"
#include "SDCARD.h"
#include "Serial_Link.h"
#include "Parameter.h"
#include "Computation.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"
#include "arm_math.h"

//#define KFDCM_Q 	0.00001f
//#define KFDCM_R 	0.00001f
#define KFDCM_Q 	0.00001f
#define KFDCM_R 	0.001f

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Variable
//

static arm_matrix_instance_f32 	Xminus1, Pminus1,  Xplus1,  Pplus1,
																Xminus2, Pminus2,	 Xplus2, Pplus2, 
																F, Ft, K, PR, PRt, Correct, E,  PF, FPF;
											

static 	float32_t Data_Pminus1[3*3] = {
																					0.01	,0,		0,	
																					0,		0.01,	0,	
																					0,		0,		0.01,
																			};

static 	float32_t Data_Pminus2[3*3] = {
																					0.01	,0,		0,	
																					0,		0.01,	0,	
																					0,		0,		0.01,
																			};
																										
												
static 	float32_t Data_Xminus1[3] ;											
static 	float32_t Data_Xminus2[3] ;
static	float32_t Data_Xplus1 [3];
static	float32_t Data_Xplus2 [3];	
static	float32_t Data_Pplus1 [3*3];	
static	float32_t Data_Pplus2 [3*3];	
																												

static 	float32_t	Data_F[3*3];
static 	float32_t Data_Ft[3*3];	
static	float32_t Data_PR[3*3];
static	float32_t Data_PRt[3*3];
static	float32_t Data_K[3*3] ;
static	float32_t Data_E [3];
static	float32_t Data_Correct [3];	
static	float32_t Data_PF [3*3];	
static	float32_t Data_FPF [3*3];	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////EKF	function
//
void Initialize_EKFDCM (float_3D *Acce, float_3D *Magn)
{		
	//Init Kalman Matrix	
	arm_mat_init_f32(&F, 				3, 3, (float32_t *)Data_F); 
	arm_mat_init_f32(&Ft, 			3, 3, (float32_t *)Data_Ft); 	
	arm_mat_init_f32(&K, 				3, 3, (float32_t *)Data_K); 
	
	arm_mat_init_f32(&Pplus1, 	3, 3, (float32_t *)Data_Pplus1); 
	arm_mat_init_f32(&Pminus1, 	3, 3, (float32_t *)Data_Pminus1); 	
	arm_mat_init_f32(&Xplus1, 	3, 1, (float32_t *)Data_Xplus1); 	
	arm_mat_init_f32(&Xminus1, 	3, 1, (float32_t *)Data_Xminus1); 	
		
	arm_mat_init_f32(&Pplus2, 	3, 3, (float32_t *)Data_Pplus2); 	
	arm_mat_init_f32(&Pminus2, 	3, 3, (float32_t *)Data_Pminus2); 
	arm_mat_init_f32(&Xplus2, 	3, 1, (float32_t *)Data_Xplus2); 		
	arm_mat_init_f32(&Xminus2, 	3, 1, (float32_t *)Data_Xminus2); 
	
	//Init Kalman intemediate Matrix
	
	arm_mat_init_f32(&E, 				3, 1, (float32_t *)Data_E); 	
	arm_mat_init_f32(&Correct,	3, 1, (float32_t *)Data_Correct); 
	arm_mat_init_f32(&PR, 			3, 3, (float32_t *)Data_PR); 			
	arm_mat_init_f32(&PF, 			3, 3, (float32_t *)Data_PF); 		
	arm_mat_init_f32(&PRt, 			3, 3, (float32_t *)Data_PRt);			
	arm_mat_init_f32(&FPF, 			3, 3, (float32_t *)Data_FPF); 
	
	//Init Kalman State Vector
	
//	Tilt_Compensate(&tmp, Acce, Magn);
	
	Data_Xminus1[0] = 1;
	Data_Xminus1[1] = 0;
	Data_Xminus1[2] = 0;
	
	Data_Xminus2[0] = 1;
	Data_Xminus2[1] = 0;
	Data_Xminus2[2] = 0;
}

void KFDCM_Update(
/*observe*/														float *OBS,
/*Pminus*/														float32_t *Data_Pminus, 	 arm_matrix_instance_f32 *Pminus,
/*Xminus*/														float32_t *Data_Xminus,	 	 arm_matrix_instance_f32 *Xminus,
/*Pplus*/															float32_t *Data_Pplus, 		 arm_matrix_instance_f32 *Pplus,
/*Xplus*/															float32_t *Data_Xplus, 		 arm_matrix_instance_f32 *Xplus,
																			float q, float r
																	)
{	
	int i;
	
	//Update K = P*(P+R)
		//Compute (P+R)'
		for (i = 0; i<9; i++)
		{
			if (i== 0 || i== 4 || i== 8)				
				Data_PR [i] = Data_Pminus[i] + r;		// if diagonal PR = P+R
			else 								
				Data_PR [i] = Data_Pminus[i];								// else PR = P
		}
		arm_mat_inverse_f32(&PR, &PRt);		
		//Compute K 
		arm_mat_mult_f32 (Pminus, &PRt, &K); 																		
		
	//Update Xplus = Xminus	+ K*(Zmeasure - Zestimate)
		//Compute Innovation: Zerror = Zmeasure - Zestimate
		Data_E[0] = OBS[0] - Data_Xminus[0];
		Data_E[1] = OBS[1] - Data_Xminus[1];
		Data_E[2] = OBS[2] - Data_Xminus[2];
		//Compute Correction:	K*Zerror 		
		arm_mat_mult_f32 (&K, &E, &Correct); 			
		//Compute Posteriori state: 					
		Data_Xplus[0] = Data_Xminus[0] + Data_Correct[0];			
		Data_Xplus[1] = Data_Xminus[1] + Data_Correct[1];			
		Data_Xplus[2] = Data_Xminus[2] + Data_Correct[2];			
		//normalize
		Normalize_Three(&Data_Xplus[0],&Data_Xplus[1],&Data_Xplus[2]);
	
	//Update Pplus  = (I - K)*P_minus
		//Compute I-K		
		for (i = 0; i<9; i++)	
		{
			if (i== 0 || i== 4 || i== 8)			
				Data_K[i] = 1 - Data_K[i];	
			else 						
				Data_K[i] = - Data_K[i];
		}			
		//Compute P_plus
		arm_mat_mult_f32 (&K, Pminus, Pplus); 		
	
	//Predict Xminus: 																	X_minus = F * X_plus				
		arm_mat_mult_f32 (&F, Xplus, Xminus); 		
		Normalize_Three(&Data_Xminus[0],&Data_Xminus[1],&Data_Xminus[2]);
				
	//Predict P_minus = F*P_plus*F' + Q   	
		//Compute P_plus*F'
		arm_mat_mult_f32 (Pplus, &Ft, &PF); 
		//Compute F*P_plus*F'
		arm_mat_mult_f32 (&F,&PF, &FPF); 
		//Compute P_minus
			Data_Pminus[0] 	= Data_FPF[0] 	+ q;		
			Data_Pminus[4] 	= Data_FPF[4] 	+ q;		
			Data_Pminus[8] 	= Data_FPF[8] 	+ q;		
}

void DCMKF_Estimate(DCM *Kalman_dcm, float_3D *Gyro, float_3D *Acce, float_3D *Magn, float sampletime)
{
	float obs[3];
	float phy, s_phi, c_phi, s_theta, c_theta, s_psi, c_psi, Xh, Yh, XYh; 
	float m11, m12, m13;
	float x, y, z;
				
	//Update EKF Transition Matrix
		x = sampletime * Gyro->x;
		y = sampletime * Gyro->y;
		z = sampletime * Gyro->z;
		
		Data_F[0] 	= 1;		Data_F[1] 	= z;		Data_F[2] 	= -y;	
		Data_F[3] 	= -z;		Data_F[4] 	= 1;		Data_F[5] 	= x;	
		Data_F[6] 	= y;		Data_F[7] 	= -x;		Data_F[8] 	= 1;	
		
		Data_Ft[0] 	= 1;		Data_Ft[1] 	= -z;		Data_Ft[2] 	= y;	
		Data_Ft[3] 	= z;		Data_Ft[4] 	= 1;		Data_Ft[5] 	= -x;	
		Data_Ft[6] 	= -y;		Data_Ft[7] 	= x;		Data_Ft[8] 	= 1;	
			
	//Update EKF1 Observation
//		obs[0] = Acce->x / 9.81f;
//		obs[1] = Acce->y / 9.81f;
//		obs[2] = Acce->z / 9.81f;
		obs[0] = Acce->x ;
		obs[1] = Acce->y ;
		obs[2] = Acce->z ;
		Normalize_Three(&obs[0], &obs[1], &obs[2]);
	
	//Update EKF1
		KFDCM_Update (
											obs, 
											Data_Pminus1, &Pminus1, 
											Data_Xminus1, &Xminus1,
											Data_Pplus1, &Pplus1, 
											Data_Xplus1, &Xplus1,										
											KFDCM_q1, KFDCM_r1
										);
	
	//Update EKF2 Observation
		phy = atan2(Data_Xplus1[1],Data_Xplus1[2]);
		s_phi = sin(phy);
		c_phi = cos(phy);;  

		s_theta = -Data_Xplus1[0];              
		c_theta = sqrt(1 - s_theta*s_theta);    

		Xh = Magn->x * c_theta + Magn->y * s_theta*s_phi + Magn->z * s_theta * c_phi;
		Yh = Magn->y * c_phi - Magn->z * s_phi;
		XYh = sqrt(Xh*Xh + Yh*Yh);
		s_psi = -Yh / XYh;
		c_psi = Xh / XYh;

		obs[0] = c_theta * s_psi;
		obs[1] = s_phi * s_theta * s_psi + c_phi * c_psi;
		obs[2] = c_phi * s_theta * s_psi - s_phi * c_psi;
		
	//Update EKF2
		KFDCM_Update (
											obs, 
											Data_Pminus2, &Pminus2, 
											Data_Xminus2, &Xminus2,
											Data_Pplus2, &Pplus2, 
											Data_Xplus2, &Xplus2,
											KFDCM_q2, KFDCM_r2
										);					
		
	//Compute DCM			
		m11 = Data_Xplus2[1]*Data_Xplus1[2] - Data_Xplus2[2]*Data_Xplus1[1];
		m12 = Data_Xplus2[2]*Data_Xplus1[0] - Data_Xplus2[0]*Data_Xplus1[2];
		m13 = Data_Xplus2[0]*Data_Xplus1[1] - Data_Xplus2[1]*Data_Xplus1[0];		
		Normalize_Three(&m11, &m12, &m13);
		
		Kalman_dcm->dcm0 = m11;  								Kalman_dcm->dcm1 = m12; 							Kalman_dcm->dcm2 = m13; 
		Kalman_dcm->dcm3 = Data_Xplus2[0];  		Kalman_dcm->dcm4 = Data_Xplus2[1]; 		Kalman_dcm->dcm5 = Data_Xplus2[2]; 
		Kalman_dcm->dcm6 = Data_Xplus1[0];  		Kalman_dcm->dcm7 = Data_Xplus1[1]; 		Kalman_dcm->dcm8 = Data_Xplus1[2]; 
		
}



