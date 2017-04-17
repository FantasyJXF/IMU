/****************************************************
**文件：ekf.h
**目的：实现扩展卡尔曼滤波头文件
******************************************************/

#ifndef   _EKF_H
#define   _EKF_H

#include  "ekf.h"

#include <stdio.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_permutation.h>

/****************************************************
**定义结构体
******************************************************/

/****************定义指向校正数据的指针**************/
struct calibr_data
{
	gsl_matrix  *K_xti;            /*Xsens惯性元件向TUD坐标的旋转矩阵*/
    gsl_matrix  *K_xtm;            /*Xsens磁力计向TUD坐标的旋转矩阵*/
    gsl_matrix  *K_htm;            /*Honey 磁性向TUD坐标的转换*/
	gsl_matrix  *K_a;              /*加速度计比例因子矩阵*/
	gsl_matrix  *K_w;              /*陀螺仪比例因子矩阵*/
	gsl_matrix  *K_B;              /*磁力计比例因子矩阵*/
	gsl_vector  *lambda_a;         /*加速度计偏差数据*/
	gsl_vector  *lambda_w;         /*陀螺仪偏差数据*/
	gsl_vector  *lambda_B;        /*磁力计偏差数据*/
};


/****************定义指向环境数据的指针**************/
struct enviro_data
{
	gsl_vector *a_n;           /*TUD航迹坐标下的重力场*/
	gsl_vector *B_n;           /*TUD航迹坐标下的磁场*/
};

struct timeval
{
	double hour;
	double min;
    double sec;
};

/****************定义临时矩阵和矢量**************/
struct temp_data
{
	/***函数 ekf()***/
	gsl_vector  *f;              
    gsl_vector  *h;             
	gsl_vector  *Y_tmp;      
	gsl_vector  *Yminh;          
	gsl_vector  *KYminh;
    gsl_vector  *xpKYminh;
	gsl_matrix  *F_x;             /**状态转移矩阵**/
    gsl_matrix  *H_x;             /**观测矩阵矩阵**/
	gsl_matrix  *Eye;             /**单位矩阵**/
	gsl_matrix  *Phi;             /**状态转移矩阵Phi**/ 
	gsl_matrix  *Psi;             /**矩阵Psi**/ 
	gsl_matrix  *G;               /**线性系统噪声矩阵增益矩阵**/          
	gsl_matrix  *Gamma;           /**非系统噪声矩阵增益矩阵**/ 
	gsl_matrix  *QGammat;         /**Q乘以(Gamma的转置矩阵)**/
    gsl_matrix  *GammaQGammat;    /**Gamma乘以Q乘以(Gamma的转置)****/
    gsl_matrix  *P_kkPhit;        /**P_kk(估计方差矩阵)*(Phi的转置)***/
    gsl_matrix  *PhiP_kkPhit;     /**Phi*P_kk(估计方差矩阵)*(Phi的转置)***/
    gsl_matrix  *P_kplkHt;
    gsl_matrix  *HP_kplkHt;
    gsl_matrix  *Inv;             /**逆矩阵**/
	gsl_matrix  *HtInv;     
	gsl_matrix  *K_kplk;
	gsl_matrix  *I;               /**矩阵I,表示单位矩阵**/         
	gsl_matrix  *ImKH;            /**(I-K*H)******/
	gsl_matrix  *PImKHt;          /*P*(I-K*H)的转置***********/
	gsl_matrix  *ImKHPImKHt;      /**(I-K*H)*P*(I-K*H)的转置***********/
	gsl_matrix  *RKt;             /**矩阵R(量测噪声方差家矩阵)乘以矩阵K的转置****/
	gsl_matrix  *KRKt;            /**矩阵k乘以矩阵R乘以(K的转置)***/
	gsl_permutation  *Perm;       /**
  
    /***函数 ekf_f***/
	gsl_vector  *x_kk;            /***状态失量****/
    gsl_matrix  *A;               /**运动学矩阵A***/
   	
	
	/***函数 ekf_h***/
	gsl_vector  *a_n_tmp;         /***导航坐标系下的加速度****/
    gsl_vector  *B_n_tmp;         /***导航坐标系下的磁场****/
    gsl_vector  *a_b;             /***机体坐标系下的加速度****/
    gsl_vector  *B_b;             /***机体坐标系下的磁场****/
	gsl_vector  *a_b_xsens;       /***机体坐标系下的加速度的Xsens****/
    gsl_vector  *B_b_xsens;       /***机体坐标系下的磁场的xsens****/
    gsl_vector  *a_b_digital;     /***机体坐标系下的加速度的数字值****/
    gsl_vector  *B_b_digital;     /***机体坐标系下的磁场的数字值****/
    gsl_vector  *omega;           /**角速率矢量*******************/        
	gsl_vector  *omega_xsens;     /**基于Xsens的角速率矢量*******************/  
	gsl_vector  *omega_digital;   /**基于SISO IMU的角速率矢量*******************/  
    gsl_vector  *lambda_omega;    /** 估计陀螺偏差矢量*******/
	gsl_vector  *x_kk_tmp;        /** k时刻估计的k时刻的临时状态矢量*******/
	gsl_vector  *T_nb;            /**坐标转换矩阵—导航坐标到机体坐标***/
    

	/***函数 ekf_H_x***/
	gsl_vector  *x_kkpl;                     /***k+1时刻估计K时刻的状态*******/        
	gsl_vector  *a_n_scale_a;                /****导航坐标系下加速度计的矢量因子****/
    gsl_vector  *B_n_scale_B;                /****导航坐标系下磁力计的矢量因子****/
	gsl_vector  *dTdx1_a_n_scale_a;
    gsl_vector  *dTdx2_a_n_scale_a;
    gsl_vector  *dTdx3_a_n_scale_a;
    gsl_vector  *dTdx1_B_n_scale_B;
	gsl_vector  *dTdx2_B_n_scale_B;
	gsl_vector  *dTdx3_B_n_scale_B;
    gsl_vector  *K_xti_dTdx1_a_n_scale_a;
	gsl_vector  *K_xti_dTdx2_a_n_scale_a;
	gsl_vector  *K_xti_dTdx3_a_n_scale_a;
    gsl_vector  *K_xtm_dTdx1_B_n_scale_B;
	gsl_vector  *K_xtm_dTdx2_B_n_scale_B;
	gsl_vector  *K_xtm_dTdx3_B_n_scale_B;
    gsl_matrix  *dTdx1;
    gsl_matrix  *dTdx2;
	gsl_matrix  *dTdx3;


	/***函数 ekf_Bg...***/
    gsl_vector  *Y;                 /***矢量Y****/
	gsl_vector  *lambda_a;          /***估计的加速度计偏差矢量****/
	gsl_vector  *lambda_B;          /***估计的磁力计偏差矢量****/
    gsl_matrix  *K_a;               /*加速度计比例因子矩阵*****/
	gsl_matrix  *K_B;               /*磁力计比例因子矩阵****/
	gsl_matrix  *K_a_inv;           /*加速度计比例因子矩阵的逆*****/
	gsl_matrix  *K_B_inv;           /*磁力计比例因子矩阵的逆****/
    gsl_permutation  *Perm3;  
    gsl_vector  *a_m_xsens;         /*基于Xsens加速度计的测量矢量*****/      
    gsl_vector  *a_m;               /*加速度计的测量矢量*****/
    gsl_vector  *B_m_xsens;         /*基于Xsens磁力计计的测量矢量*****/   
    gsl_vector  *B_m;               /*磁力计的测量矢量*****/
};
   
    int view_vector(const gsl_vector  *a);
	int view_matrix(const gsl_matrix  *m); 	
     
	int ekf_init(gsl_vector  **x_kk,      /***k时刻估计的k时刻的状态*****/
	             gsl_vector  **x_kkpl,    /***k时刻估计的k+1时刻的状态*****/
				 gsl_vector  **x_kplkpl,  /***k+1时刻估计的k+1时刻的状态*****/
				 gsl_matrix  **P_kk,      /***k时刻估计的k时刻的方差矩阵*****/
                 gsl_matrix  **P_kplkpl,  /***k+1时刻估计的k+1时刻的方差矩阵*****/
                 gsl_vector  **u_k,       /***k时刻的输入*******************/
                 gsl_vector  **Y,         /***观测矢量*******************/
	             double  **t_k,           /***k时刻的时间*******************/
                 double  **t_kpl,         /***k+1时刻的时间*******************/
                 gsl_matrix  **R,         /****观测噪声*********/
				 gsl_matrix  **Q,         /****过程噪声*********/
				 struct  calibr_data  **cal,     /****传感器校正数据*********/
                 struct  enviro_data  **env,     /****传感器环境数据*********/
                 struct  temp_data  **tmp);      /****传感器温度数据*********/
 
	/****观测量*****/
	int ekf_measure(gsl_vector  *Y);  

	
	/***初始化时间*****/
	int ekf_time(double  *t_kpl,
		         double  *t_k);    

    /*****扩展卡尔曼滤波*****/	
	int ekf(const  gsl_vector  *Y,
		    const  gsl_vector  *x_kk,
			const  double  *t_k,
			const  double  *t_kpl,
			const  gsl_vector  *u_k,
            const  gsl_matrix  *P_kk,
            const  gsl_matrix  *R,
			const  gsl_matrix  *Q,
			struct  calibr_data *cal,
            struct  enviro_data *env,
			struct  temp_data  *tmp,
			gsl_vector  *x_kkpl,
			gsl_vector  *x_kplkpl,
            gsl_matrix  *P_kkpl,
			gsl_matrix  *P_kplkpl);

	int ekf_f(const gsl_vector  *x_kk,
		      struct temp_data  *tmp,
			  gsl_matrix  *f);

	int ekf_g(const gsl_vector  *x_kk,
		      const  double *t_k,
              gsl_matrix *G);

   int ekf_h(const gsl_vector *x_kk,
	         const gsl_vector *u_k,
			 const  double *t_k,
			 const  gsl_vector  *Y,
			 const struct calibr_data *cal,
			 const struct enviro_data *env,
			 const  double *scale_a,
			 const  double *scale_B,
             struct  temp_data  *tmp,
             gsl_vector *h);

   int ekf_F_xa(const gsl_vector *x_kk,
	             gsl_matrix *F_x);


  int ekf_H_xa(const gsl_vector *x_kplk,
	           const  double *t_k,
			   const struct calibr_data *cal,
			   const struct enviro_data *env,
			   const  double *scale_a,
			   const  double *scale_B,
               struct  temp_data  *tmp,
                gsl_vector *H_x);

  int ekf_Bg_modle_is_valid(const gsl_vector *x_kk,
	                        const gsl_vector *Y,
							const struct calibr_data *cal,
			                const struct enviro_data *env,
							const  double *t_kpl,
							struct  temp_data  *tmp,
							double *scale_a,
			                double *scale_B);

  int ekf_free(gsl_vector **x_kk,
	           gsl_vector **x_kkpl,
			   gsl_vector **x_kplkpl,
			   gsl_matrix **P_kk,			   
               gsl_matrix **P_kplkpl,
               gsl_vector **u_k,
			   gsl_vector **Y,
               double **t_k,
			   double **t_kpl,
               gsl_matrix **R,
               gsl_matrix **Q,
			   struct calibr_data **cal,
			   struct enviro_data **env,
               struct  temp_data  **tmp);

 int ekf_read_log(FILE  *fd_log,
	              gsl_vector *Y,
				  double *t_kpl);


#endif










