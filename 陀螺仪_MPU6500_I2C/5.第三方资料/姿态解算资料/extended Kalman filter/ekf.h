/****************************************************
**�ļ���ekf.h
**Ŀ�ģ�ʵ����չ�������˲�ͷ�ļ�
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
**����ṹ��
******************************************************/

/****************����ָ��У�����ݵ�ָ��**************/
struct calibr_data
{
	gsl_matrix  *K_xti;            /*Xsens����Ԫ����TUD�������ת����*/
    gsl_matrix  *K_xtm;            /*Xsens��������TUD�������ת����*/
    gsl_matrix  *K_htm;            /*Honey ������TUD�����ת��*/
	gsl_matrix  *K_a;              /*���ٶȼƱ������Ӿ���*/
	gsl_matrix  *K_w;              /*�����Ǳ������Ӿ���*/
	gsl_matrix  *K_B;              /*�����Ʊ������Ӿ���*/
	gsl_vector  *lambda_a;         /*���ٶȼ�ƫ������*/
	gsl_vector  *lambda_w;         /*������ƫ������*/
	gsl_vector  *lambda_B;        /*������ƫ������*/
};


/****************����ָ�򻷾����ݵ�ָ��**************/
struct enviro_data
{
	gsl_vector *a_n;           /*TUD���������µ�������*/
	gsl_vector *B_n;           /*TUD���������µĴų�*/
};

struct timeval
{
	double hour;
	double min;
    double sec;
};

/****************������ʱ�����ʸ��**************/
struct temp_data
{
	/***���� ekf()***/
	gsl_vector  *f;              
    gsl_vector  *h;             
	gsl_vector  *Y_tmp;      
	gsl_vector  *Yminh;          
	gsl_vector  *KYminh;
    gsl_vector  *xpKYminh;
	gsl_matrix  *F_x;             /**״̬ת�ƾ���**/
    gsl_matrix  *H_x;             /**�۲�������**/
	gsl_matrix  *Eye;             /**��λ����**/
	gsl_matrix  *Phi;             /**״̬ת�ƾ���Phi**/ 
	gsl_matrix  *Psi;             /**����Psi**/ 
	gsl_matrix  *G;               /**����ϵͳ���������������**/          
	gsl_matrix  *Gamma;           /**��ϵͳ���������������**/ 
	gsl_matrix  *QGammat;         /**Q����(Gamma��ת�þ���)**/
    gsl_matrix  *GammaQGammat;    /**Gamma����Q����(Gamma��ת��)****/
    gsl_matrix  *P_kkPhit;        /**P_kk(���Ʒ������)*(Phi��ת��)***/
    gsl_matrix  *PhiP_kkPhit;     /**Phi*P_kk(���Ʒ������)*(Phi��ת��)***/
    gsl_matrix  *P_kplkHt;
    gsl_matrix  *HP_kplkHt;
    gsl_matrix  *Inv;             /**�����**/
	gsl_matrix  *HtInv;     
	gsl_matrix  *K_kplk;
	gsl_matrix  *I;               /**����I,��ʾ��λ����**/         
	gsl_matrix  *ImKH;            /**(I-K*H)******/
	gsl_matrix  *PImKHt;          /*P*(I-K*H)��ת��***********/
	gsl_matrix  *ImKHPImKHt;      /**(I-K*H)*P*(I-K*H)��ת��***********/
	gsl_matrix  *RKt;             /**����R(������������Ҿ���)���Ծ���K��ת��****/
	gsl_matrix  *KRKt;            /**����k���Ծ���R����(K��ת��)***/
	gsl_permutation  *Perm;       /**
  
    /***���� ekf_f***/
	gsl_vector  *x_kk;            /***״̬ʧ��****/
    gsl_matrix  *A;               /**�˶�ѧ����A***/
   	
	
	/***���� ekf_h***/
	gsl_vector  *a_n_tmp;         /***��������ϵ�µļ��ٶ�****/
    gsl_vector  *B_n_tmp;         /***��������ϵ�µĴų�****/
    gsl_vector  *a_b;             /***��������ϵ�µļ��ٶ�****/
    gsl_vector  *B_b;             /***��������ϵ�µĴų�****/
	gsl_vector  *a_b_xsens;       /***��������ϵ�µļ��ٶȵ�Xsens****/
    gsl_vector  *B_b_xsens;       /***��������ϵ�µĴų���xsens****/
    gsl_vector  *a_b_digital;     /***��������ϵ�µļ��ٶȵ�����ֵ****/
    gsl_vector  *B_b_digital;     /***��������ϵ�µĴų�������ֵ****/
    gsl_vector  *omega;           /**������ʸ��*******************/        
	gsl_vector  *omega_xsens;     /**����Xsens�Ľ�����ʸ��*******************/  
	gsl_vector  *omega_digital;   /**����SISO IMU�Ľ�����ʸ��*******************/  
    gsl_vector  *lambda_omega;    /** ��������ƫ��ʸ��*******/
	gsl_vector  *x_kk_tmp;        /** kʱ�̹��Ƶ�kʱ�̵���ʱ״̬ʸ��*******/
	gsl_vector  *T_nb;            /**����ת�����󡪵������굽��������***/
    

	/***���� ekf_H_x***/
	gsl_vector  *x_kkpl;                     /***k+1ʱ�̹���Kʱ�̵�״̬*******/        
	gsl_vector  *a_n_scale_a;                /****��������ϵ�¼��ٶȼƵ�ʸ������****/
    gsl_vector  *B_n_scale_B;                /****��������ϵ�´����Ƶ�ʸ������****/
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


	/***���� ekf_Bg...***/
    gsl_vector  *Y;                 /***ʸ��Y****/
	gsl_vector  *lambda_a;          /***���Ƶļ��ٶȼ�ƫ��ʸ��****/
	gsl_vector  *lambda_B;          /***���ƵĴ�����ƫ��ʸ��****/
    gsl_matrix  *K_a;               /*���ٶȼƱ������Ӿ���*****/
	gsl_matrix  *K_B;               /*�����Ʊ������Ӿ���****/
	gsl_matrix  *K_a_inv;           /*���ٶȼƱ������Ӿ������*****/
	gsl_matrix  *K_B_inv;           /*�����Ʊ������Ӿ������****/
    gsl_permutation  *Perm3;  
    gsl_vector  *a_m_xsens;         /*����Xsens���ٶȼƵĲ���ʸ��*****/      
    gsl_vector  *a_m;               /*���ٶȼƵĲ���ʸ��*****/
    gsl_vector  *B_m_xsens;         /*����Xsens�����ƼƵĲ���ʸ��*****/   
    gsl_vector  *B_m;               /*�����ƵĲ���ʸ��*****/
};
   
    int view_vector(const gsl_vector  *a);
	int view_matrix(const gsl_matrix  *m); 	
     
	int ekf_init(gsl_vector  **x_kk,      /***kʱ�̹��Ƶ�kʱ�̵�״̬*****/
	             gsl_vector  **x_kkpl,    /***kʱ�̹��Ƶ�k+1ʱ�̵�״̬*****/
				 gsl_vector  **x_kplkpl,  /***k+1ʱ�̹��Ƶ�k+1ʱ�̵�״̬*****/
				 gsl_matrix  **P_kk,      /***kʱ�̹��Ƶ�kʱ�̵ķ������*****/
                 gsl_matrix  **P_kplkpl,  /***k+1ʱ�̹��Ƶ�k+1ʱ�̵ķ������*****/
                 gsl_vector  **u_k,       /***kʱ�̵�����*******************/
                 gsl_vector  **Y,         /***�۲�ʸ��*******************/
	             double  **t_k,           /***kʱ�̵�ʱ��*******************/
                 double  **t_kpl,         /***k+1ʱ�̵�ʱ��*******************/
                 gsl_matrix  **R,         /****�۲�����*********/
				 gsl_matrix  **Q,         /****��������*********/
				 struct  calibr_data  **cal,     /****������У������*********/
                 struct  enviro_data  **env,     /****��������������*********/
                 struct  temp_data  **tmp);      /****�������¶�����*********/
 
	/****�۲���*****/
	int ekf_measure(gsl_vector  *Y);  

	
	/***��ʼ��ʱ��*****/
	int ekf_time(double  *t_kpl,
		         double  *t_k);    

    /*****��չ�������˲�*****/	
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










