/********************************************************
**文件：ekf_init.c
**功能：卡尔曼滤波器初始化
********************************************************/


//#define  _POSIX_SOURCE    /*POSIX 1  可移植性操作系统接口*/

#include "ekf.h"
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>
#include  <time.h> 


int ekf_init(gsl_vector  **x_kk,
			 gsl_vector  **x_kkpl,
			 gsl_vector  **x_kplkpl,
			 gsl_matrix  **P_kk,
			 gsl_matrix  **P_kkpl,
			 gsl_matrix  **P_kplkpl,
			 gsl_vector  **u_k,
			 gsl_vector  **Y,
			 double  **t_k,
			 double  **t_kpl,
			 gsl_matrix  **R,
			 gsl_matrix  **Q,
			 struct calibr_data  **cal,
			 struct enviro_data  **env,
			 struct temp_data  **tmp){
 /******************************************************
    **扩展卡尔曼滤波初始化函数：
**生命和初始化滤波器的初始变量
**函数不需输入任何参数，初始化之后，可从初始化文件读取
**函数返回：
**变量的初始值:x_kk,P_kk,u_k,t_k,R,Q,cal,env,tmp
**函数状态
**成功返回0，如果函数中出现问题返回值>0,
**若函数内部没有检测，函数返回1，
*******************************************************/

/**size_t i,j;**/
  size_t nP_kk,nQ,nR,nK_xti,nK_xtm,nK_htm,nK_a,nK_w,nK_B,nd;

/**缓存矩阵**/
  gsl_vector_view vect;
  gsl_matrix_view matr;

/**初始化扩展卡尔曼滤波矢量和矩阵**/
/**	初始化的值使用数组形式，但是最终将以"_ar"扩展名的矢量和矩阵的形式传递，
 ** "_ar"说表示是矢量和矩阵的数组形式 ，而并非gsl_vector或者gsl_matrix的形式
 **	初始化的值使用数组形式，但是最终数据结构（例如'cal','env'）形式传递，
 ** "_ar"说表示是矢量和矩阵的数组形式 ，而并非gsl_vector或者gsl_matrix的形式*/


  /**状态初始时刻估计值**/
  double x_kk_ar[]={0.0,0.0,0.0,0.0,0.0,0.0,13422.969,16271.605,19081.002};

   double u_k_ar[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

  double Y_ar[]={32768.0,32768.0,32768.0,16384.0,16384.0,16384.0,32768.0,32768.0,32768.0};

  double P_kk_ar[]={
 1.0e1,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
   0.0, 1.0e1, 0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
   0.0, 0.0, 1.0e1,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,
   0.0, 0.0,   0.0,  1.0,  0.0,  0.0,  0.0,  0.0,  0.0,
   0.0, 0.0,   0.0,  0.0,  1.0,  0.0,  0.0,  0.0,  0.0,
   0.0, 0.0,   0.0,  0.0,  0.0,  1.0,  0.0,  0.0,  0.0,
   0.0, 0.0,   0.0,  0.0,  0.0, 0.0, 1.0e3,  0.0,  0.0,
   0.0, 0.0,   0.0,  0.0,  0.0,  0.0, 0.0, 1.0e3,  0.0,
   0.0, 0.0,   0.0,  0.0,  0.0, 0.0,  0.0,  0.0,  1.0e3};

  double Q_ar[]={
 1.0e-3,  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,
   0.0, 1.0e-3,  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,
   0.0,   0.0,  1.0e-3, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,
   0.0,   0.0,   0.0, 1.0e-4,  0.0,   0.0,   0.0,   0.0,   0.0,
   0.0,   0.0,   0.0,  0.0,  1.0e-4,  0.0,   0.0,   0.0,   0.0,
   0.0,   0.0,   0.0,  0.0,    0.0, 1.0e-4,  0.0,   0.0,   0.0,
   0.0,   0.0,   0.0,  0.0,    0.0,   0.0, 1.0e-1,  0.0,   0.0,
   0.0,   0.0,   0.0,  0.0,    0.0,   0.0,   0.0, 1.0e-1,  0.0,
   0.0,   0.0,   0.0,  0.0,    0.0,   0.0,   0.0,   0.0,  1.0e-1};

   double R_ar[]={
 2.5e1,  0.0,   0.0,   0.0,  0.0,   0.0,   0.0,   0.0,   0.0,
   0.0,  2.5e1, 0.0,   0.0,  0.0,   0.0,   0.0,   0.0,   0.0,
   0.0,  0.0,  2.5e1,  0.0,  0.0,   0.0,   0.0,   0.0,   0.0,
   0.0,  0.0,   0.0,  1.0e2, 0.0,   0.0,   0.0,   0.0,   0.0,
   0.0,  0.0,   0.0,   0.0, 1.0e2,  0.0,   0.0,   0.0,   0.0,
   0.0,  0.0,   0.0,   0.0,  0.0,  1.0e2,  0.0,   0.0,   0.0,
   0.0,  0.0,   0.0,   0.0,  0.0,   0.0, 5625.0,  0.0,   0.0,
   0.0,  0.0,   0.0,   0.0,  0.0,   0.0,   0.0, 5625.0,  0.0,
   0.0,  0.0,   0.0,   0.0,  0.0,   0.0,   0.0,   0.0, 5625.0,};


 /**初始化校正矢量和矩阵**/ 
 

 /*Xsens惯性元件向TUD坐标的旋转矩阵*/
   double K_xti[]={
	 1.0,  0.0,  0.0,
	 0.0, -1.0,  0.0,
	 0.0,  0.0, -1.0};

   /*Xsens磁力计向TUD坐标的旋转矩阵*/
	 double K_xtm[]={
	 1.0,  0.0,  0.0,
	 0.0, -1.0,  0.0,
	 0.0,  0.0, -1.0};
  
	 
	 /*Honey 磁性向TUD坐标的转换*/
	double K_htm[]={
	-1.0,  0.0,  0.0,
	 0.0,  1.0,  0.0,
	 0.0,  0.0, -1.0};


    /*加速度计比例因子矩阵*/
	double K_a[]={
	   8.592,  -270.000,   -0.394,
	 278.078,     80658,    4.240,
	   2.139,     4.272,  -278.536};


	/*陀螺仪比例因子矩阵*/
    double K_w[]={
	 -5.165,   1083.844,   9.602,
  -1004.712,     -3.125,  -0.119,
  15.251,    -31.690,  1011.761};

	/*磁力计比例因子矩阵*/
    double K_B[]={
	 7764.251,   -105.615,   -138.957,
	 -138.653,  -7469.236,   -258.980,
	 175.147,    137.201,  -8020.158};

    /*加速度计偏差数据*/
	 double lambda_a[]={32616.258,  30835.090,  -138.957};

     /*陀螺仪偏差数据*/
	 double lambda_w[]={13422.258,  16271.605,  19081.002};

     /*磁力计偏差数据*/
	 double lambda_B[]={32272.345,  32871.619,  32192.637};

     /**初始化环境矩阵**/ 

      double a_n[]={0.0, 0.0, -9.81};

   /**国际参考磁场，海湾位置，海岸线**/ 
     double B_n[]={1.88111e-01,-3.565e-02,4.49136e-01};

 /**分配存储空间设置矩阵指针**/ 
 *cal=(struct calibr_data *)
  malloc(sizeof(struct calibr_data));

 *env=(struct enviro_data *)
  malloc(sizeof(struct enviro_data));

 *tmp=(struct tmp_data *)
  malloc(sizeof(struct temp_data));

  nd=(size_t)sizeof(double);

 /**分配存储空间设置指向数据的指针**/
  *x_kk       =gsl_vector_calloc(sizeof(x_kk_ar)/nd);
  *x_kkpl     =gsl_vector_calloc(sizeof(x_kk_ar)/nd);
  *x_kplkpl   =gsl_vector_calloc(sizeof(x_kk_ar)/nd);
  *u_k        =gsl_vector_calloc(sizeof(u_k_ar)/nd);
  *Y          =gsl_vector_calloc(sizeof(Y_ar)/nd);

  (*cal)->lambda_a  =gsl_vector_calloc(sizeof(lambda_a)/nd);
  (*cal)->lambda_w  =gsl_vector_calloc(sizeof(lambda_w)/nd);
  (*cal)->lambda_B  =gsl_vector_calloc(sizeof(lambda_B)/nd);
  (*env)->a_n       =gsl_vector_calloc(sizeof(a_n)/nd);
  (*env)->B_n       =gsl_vector_calloc(sizeof(B_n)/nd);


 /**分配存储空间设置指向缓存矢量的指针**/

  /**针对函数ekf()**/
  (*tmp)->f           =gsl_vector_calloc(9);
  (*tmp)->h           =gsl_vector_calloc(9);
  (*tmp)->Y_tmp       =gsl_vector_calloc(9);
  (*tmp)->Yminh       =gsl_vector_calloc(9);
  (*tmp)->KYminh      =gsl_vector_calloc(9);
  (*tmp)->xpKYminh    =gsl_vector_calloc(9);
  (*tmp)->Perm        =gsl_permutation_calloc(9);
  
 /**针对函数ekf_f()**/ 
  (*tmp)->x_kk        =gsl_vector_calloc(9);
  (*tmp)->A           =gsl_matrix_calloc(3,3);

  /**针对函数ekf_h()**/ 
  (*tmp)->a_n_tmp          =gsl_vector_calloc(3);
  (*tmp)->B_n_tmp          =gsl_vector_calloc(3);
  (*tmp)->a_b              =gsl_vector_calloc(3);
  (*tmp)->B_b              =gsl_vector_calloc(3);
  (*tmp)->a_b_xsens        =gsl_vector_calloc(3);
  (*tmp)->B_b_xsens        =gsl_vector_calloc(3);
  (*tmp)->a_b_digital      =gsl_vector_calloc(3);
  (*tmp)->B_b_digital      =gsl_vector_calloc(3);
  (*tmp)->omega            =gsl_vector_calloc(3);
  (*tmp)->omega_xsens      =gsl_vector_calloc(3);
  (*tmp)->omega_digital    =gsl_vector_calloc(3);
  (*tmp)->lambda_omega     =gsl_vector_calloc(3);
  (*tmp)->x_kk_tmp         =gsl_vector_calloc(9);

 /**针对函数ekf_H_x**/
  (*tmp)->x_kkpl                       =gsl_vector_calloc(3);
  (*tmp)->a_n_scale_a                  =gsl_vector_calloc(3);
  (*tmp)->B_n_scale_B                  =gsl_vector_calloc(3);
  (*tmp)->dTdx1_a_n_scale_a            =gsl_vector_calloc(3);
  (*tmp)->dTdx2_a_n_scale_a            =gsl_vector_calloc(3);
  (*tmp)->dTdx3_a_n_scale_a            =gsl_vector_calloc(3);
  (*tmp)->dTdx1_B_n_scale_B            =gsl_vector_calloc(3);
  (*tmp)->dTdx2_B_n_scale_B            =gsl_vector_calloc(3);
  (*tmp)->dTdx3_B_n_scale_B            =gsl_vector_calloc(3);
  (*tmp)->K_xti_dTdx1_a_n_scale_a      =gsl_vector_calloc(3);
  (*tmp)->K_xti_dTdx2_a_n_scale_a      =gsl_vector_calloc(3);
  (*tmp)->K_xti_dTdx3_a_n_scale_a      =gsl_vector_calloc(3);
  (*tmp)->K_xtm_dTdx1_B_n_scale_B      =gsl_vector_calloc(3);
  (*tmp)->K_xtm_dTdx2_B_n_scale_B      =gsl_vector_calloc(3);
  (*tmp)->K_xtm_dTdx3_B_n_scale_B      =gsl_vector_calloc(3);
  (*tmp)->dTdx1                        =gsl_matrix_calloc(3,3);
  (*tmp)->dTdx2                        =gsl_matrix_calloc(3,3);
  (*tmp)->dTdx3                        =gsl_matrix_calloc(3,3);


 /**针对函数Bg....**/
               
  (*tmp)->Y             =gsl_vector_calloc(9);
  (*tmp)->lambda_a      =gsl_vector_calloc(3);
  (*tmp)->lambda_B      =gsl_vector_calloc(3);
  (*tmp)->K_a           =gsl_matrix_calloc(3,3);
  (*tmp)->K_B           =gsl_matrix_calloc(3,3);
  (*tmp)->K_a_inv       =gsl_matrix_calloc(3,3);
  (*tmp)->K_B_inv       =gsl_matrix_calloc(3,3);
  (*tmp)->Perm3         =gsl_permutation_calloc(3);
  (*tmp)->a_m_xsens     =gsl_vector_calloc(3);
  (*tmp)->a_m           =gsl_vector_calloc(3);
  (*tmp)->B_m_xsens     =gsl_vector_calloc(3);
  (*tmp)->B_m           =gsl_vector_calloc(3);

 /**分配存储空间，设置指针指向缓存矩阵的指针**/
 /**针对函数ekf()**/
  (*tmp)->F_x            =gsl_matrix_calloc(9,9); 
  (*tmp)->H_x            =gsl_matrix_calloc(9,9); 
  (*tmp)->Eye            =gsl_matrix_calloc(9,9); 
  (*tmp)->Phi            =gsl_matrix_calloc(9,9); 
  (*tmp)->Psi            =gsl_matrix_calloc(9,9);  
  (*tmp)->G              =gsl_matrix_calloc(9,9);
  (*tmp)->Gamma          =gsl_matrix_calloc(9,9);
  (*tmp)->QGammat        =gsl_matrix_calloc(9,9);
  (*tmp)->GammaQGammat   =gsl_matrix_calloc(9,9);
  (*tmp)->P_kkPhit       =gsl_matrix_calloc(9,9);
  (*tmp)->PhiP_kkPhit     =gsl_matrix_calloc(9,9);
  (*tmp)->P_kplkHt        =gsl_matrix_calloc(9,9);
  (*tmp)->HP_kplkHt      =gsl_matrix_calloc(9,9);
  (*tmp)->Inv            =gsl_matrix_calloc(9,9);
  (*tmp)->HtInv          =gsl_matrix_calloc(9,9);
  (*tmp)->K_kplk         =gsl_matrix_calloc(9,9);
  (*tmp)->I              =gsl_matrix_calloc(9,9);
  (*tmp)->ImKH           =gsl_matrix_calloc(9,9);
  (*tmp)->PImKHt         =gsl_matrix_calloc(9,9);
  (*tmp)->ImKHPImKHt     =gsl_matrix_calloc(9,9);
  (*tmp)->RKt            =gsl_matrix_calloc(9,9);
  (*tmp)->KRKt           =gsl_matrix_calloc(9,9);

 /**针对函数ekf_h**/
  (*tmp)->T_nb           =gsl_matrix_calloc(3,3);


  /**分配存储空间，设置指针指向数据矩阵的指针**/
  /**假设：矩阵为方阵**/
  nP_kk                  =(int)sqrt((double)(sizeof(P_kk_ar)/nd));
  nQ                     =(int)sqrt((double)(sizeof(Q_ar)/nd));
  nR                     =(int)sqrt((double)(sizeof(R_ar)/nd));
  nK_xti                 =(int)sqrt((double)(sizeof(K_xti)/nd)); 
  nK_xtm                 =(int)sqrt((double)(sizeof(K_xtm)/nd));
  nK_htm                 =(int)sqrt((double)(sizeof(K_htm)/nd));
  nK_a                   =(int)sqrt((double)(sizeof(K_a)/nd));
  nK_w                   =(int)sqrt((double)(sizeof(K_w)/nd));
  nK_B                   =(int)sqrt((double)(sizeof(K_B)/nd));

  *P_kk                  =gsl_matrix_calloc(nP_kk,nP_kk);
  *P_kkpl                =gsl_matrix_calloc(nP_kk,nP_kk);
  *P_kplkpl              =gsl_matrix_calloc(nP_kk,nP_kk);
  *Q                     =gsl_matrix_calloc(nQ,nQ);
  *R                     =gsl_matrix_calloc(nR,nR);
  (*cal)->K_xti          =gsl_matrix_calloc(nK_xti,nK_xti);
  (*cal)->K_xtm          =gsl_matrix_calloc(nK_xtm,nK_xtm);
  (*cal)->K_htm          =gsl_matrix_calloc(nK_htm,nK_htm);
  (*cal)->K_a            =gsl_matrix_calloc(nK_a,nK_a);
  (*cal)->K_w            =gsl_matrix_calloc(nK_w,nK_w);
  (*cal)->K_B            =gsl_matrix_calloc(nK_B,nK_B);

 
  /**分配存储空间，设置指针指向时间**/
  *t_k                =malloc(sizeof(double));
  *t_kpl              =malloc(sizeof(double));


  /**为矢量和矩阵填充数据**/
  vect  =gsl_vector_view_array(x_kk_ar,sizeof(x_kk_ar)/nd);
  gsl_vector_memcpy(*x_kk,&vect.vector);
  vect  =gsl_vector_view_array(u_k_ar,sizeof(u_k_ar)/nd);
  gsl_vector_memcpy(*u_k,&vect.vector);
  vect  =gsl_vector_view_array(Y_ar,sizeof(Y_ar)/nd);
  gsl_vector_memcpy(*Y,&vect.vector);

  vect  =gsl_vector_view_array(lambda_a,sizeof(lambda_a)/nd);
  gsl_vector_memcpy((*cal)->lambda_a,&vect.vector);
  vect  =gsl_vector_view_array(lambda_w,sizeof(lambda_w)/nd);
  gsl_vector_memcpy((*cal)->lambda_w,&vect.vector);
  vect  =gsl_vector_view_array(lambda_B,sizeof(lambda_B)/nd);
  gsl_vector_memcpy((*cal)->lambda_B,&vect.vector);
  

  vect  =gsl_vector_view_array(a_n,sizeof(a_n)/nd);
  gsl_vector_memcpy((*env)->a_n,&vect.vector);
  vect  =gsl_vector_view_array(B_n,sizeof(B_n)/nd);
  gsl_vector_memcpy((*env)->B_n,&vect.vector);


 /**数据矩阵**/
  matr  =gsl_matrix_view_array(P_kk_ar,nP_kk,nP_kk);
  gsl_matrix_memcpy(*P_kk,&matr.matrix);
  matr  =gsl_matrix_view_array(Q_ar,nQ,nQ);
  gsl_matrix_memcpy(*Q,&matr.matrix);
  matr  =gsl_matrix_view_array(R_ar,nR,nR);
  gsl_matrix_memcpy(*R,&matr.matrix);

   
  matr  =gsl_matrix_view_array(K_xti,nK_xti,nK_xti);
  gsl_matrix_memcpy((*cal)->K_xti,&matr.matrix);
  matr  =gsl_matrix_view_array(K_xtm,nK_xtm,nK_xtm);
  gsl_matrix_memcpy((*cal)->K_xtm,&matr.matrix);
  matr  =gsl_matrix_view_array(K_htm,nK_htm,nK_htm);
  gsl_matrix_memcpy((*cal)->K_htm,&matr.matrix);

  matr  =gsl_matrix_view_array(K_a,nK_a,nK_a);
  gsl_matrix_memcpy((*cal)->K_a,&matr.matrix);
  matr  =gsl_matrix_view_array(K_w,nK_w,nK_w);
  gsl_matrix_memcpy((*cal)->K_w,&matr.matrix);
  matr  =gsl_matrix_view_array(K_B,nK_B,nK_B);
  gsl_matrix_memcpy((*cal)->K_B,&matr.matrix);


  /**初始化时间**/
  **t_k  = 0.0;
  **t_kpl= 0.01;

  return  1;
  }

