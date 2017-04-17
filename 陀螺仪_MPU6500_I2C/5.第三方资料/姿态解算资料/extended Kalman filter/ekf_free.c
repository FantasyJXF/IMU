/****�ļ���ekf_free.c***************************/
/****���ܣ�ͨ����չ�������˲���մ洢�ռ�*******/
//#define _POSIX_SOURCE

#include "ekf.h"
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_permutation.h>

int ekf_free( gsl_vector  **x_kk,
			  gsl_vector  **x_kkpl,
			  gsl_vector  **x_kplkpl,
			  gsl_matrix  **P_kk,
			  gsl_matrix  **P_kkpl,
			  gsl_matrix  **P_kplkpl,
			  gsl_vector  **u_k,
			  gsl_vector  **Y,
			  double      **t_k,
			  double      **t_kpl,
		 	  gsl_matrix  **R,
			  gsl_matrix  **Q,
			  struct  calibr_data   **cal,
			  struct  enviro_data   **env,
			  struct  temp_data     **tmp)
{
	/****Ϊ��չ�������˲����ͷŴ洢�ռ�*********/
	/****����洢�ռ䲢����ָ��ʸ����ָ��*******/
	gsl_vector_free    (*x_kk);
    gsl_vector_free    (*x_kkpl);
    gsl_vector_free    (*x_kplkpl);
	gsl_vector_free    (*u_k);
	gsl_vector_free    (*Y);
	gsl_vector_free    ((*cal)->lambda_a);
    gsl_vector_free    ((*cal)->lambda_w);
	gsl_vector_free    ((*cal)->lambda_B);
	gsl_vector_free    ((*env)->a_n);
    gsl_vector_free    ((*env)->B_n);


	/****�����ڴ�����ָ�����ݾ����ָ��********/
	gsl_matrix_free (*P_kk);
    gsl_matrix_free (*P_kkpl);
	gsl_matrix_free (*P_kplkpl);
	gsl_matrix_free (*Q);
    gsl_matrix_free (*R);
	gsl_matrix_free ((*cal)->K_xti);
	gsl_matrix_free ((*cal)->K_xtm);
	gsl_matrix_free ((*cal)->K_htm);
	gsl_matrix_free ((*cal)->K_a);
	gsl_matrix_free ((*cal)->K_w);
	gsl_matrix_free ((*cal)->K_B);

	/************Ϊʱ���ͷŴ洢�ռ�*********/
	free(*t_k);
    free(*t_kpl);


	/********����洢�ռ䣬����ָ��tmpʸ���;����ָ��*********/
	gsl_vector_free       ((*tmp)->f);
    gsl_vector_free       ((*tmp)->h);
	gsl_vector_free       ((*tmp)->Y_tmp);
    gsl_vector_free       ((*tmp)->Yminh);
	gsl_vector_free       ((*tmp)->KYminh);
	gsl_vector_free       ((*tmp)->xpKYminh);
	gsl_permutation_free  ((*tmp)->Perm);
    gsl_matrix_free       ((*tmp)->F_x);
    gsl_matrix_free       ((*tmp)->H_x);
	gsl_matrix_free       ((*tmp)->Eye);
	gsl_matrix_free       ((*tmp)->Phi);
	gsl_matrix_free       ((*tmp)->Psi);
	gsl_matrix_free       ((*tmp)->G);
	gsl_matrix_free       ((*tmp)->Gamma);
	gsl_matrix_free       ((*tmp)->QGammat);
	gsl_matrix_free       ((*tmp)->GammaQGammat);
	gsl_matrix_free       ((*tmp)->P_kkPhit);
	gsl_matrix_free       ((*tmp)->PhiP_kkPhit);
	gsl_matrix_free       ((*tmp)->P_kplkHt);
	gsl_matrix_free       ((*tmp)->Inv);
	gsl_matrix_free       ((*tmp)->HtInv);
	gsl_matrix_free       ((*tmp)->K_kplk);
	gsl_matrix_free       ((*tmp)->I);
	gsl_matrix_free       ((*tmp)->ImKH);
	gsl_matrix_free       ((*tmp)->PImKHt);
	gsl_matrix_free       ((*tmp)->ImKHPImKHt);
	gsl_matrix_free       ((*tmp)->RKt);
	gsl_matrix_free       ((*tmp)->KRKt);


	/*****��Ժ���ekf_f()************************/
     gsl_vector_free      ((*tmp)->x_kk);
	 gsl_matrix_free      ((*tmp)->A);

    /*****��Ժ���ekf_h()************************/
    gsl_vector_free      ((*tmp)->a_n_tmp);
    gsl_vector_free      ((*tmp)->B_n_tmp);
	gsl_vector_free      ((*tmp)->a_b);
	gsl_vector_free      ((*tmp)->B_b);
	gsl_vector_free      ((*tmp)->a_b_xsens);
	gsl_vector_free      ((*tmp)->B_b_xsens);
	gsl_vector_free      ((*tmp)->a_b_digital);
	gsl_vector_free      ((*tmp)->B_b_digital);
	gsl_vector_free      ((*tmp)->omega);
	gsl_vector_free      ((*tmp)->omega_xsens);
	gsl_vector_free      ((*tmp)->omega_digital);
	gsl_vector_free      ((*tmp)->lambda_omega);
	gsl_vector_free      ((*tmp)->x_kk_tmp);
	gsl_matrix_free      ((*tmp)->T_nb);

    /*****��Ժ���ekf_H_x()************************/
    gsl_vector_free      ((*tmp)->x_kkpl);
	gsl_vector_free      ((*tmp)->a_n_scale_a);
    gsl_vector_free      ((*tmp)->B_n_scale_B);

	gsl_vector_free      ((*tmp)->dTdx1_a_n_scale_a);
	gsl_vector_free      ((*tmp)->dTdx2_a_n_scale_a);
	gsl_vector_free      ((*tmp)->dTdx3_a_n_scale_a);
	gsl_vector_free      ((*tmp)->dTdx1_B_n_scale_B);
	gsl_vector_free      ((*tmp)->dTdx2_B_n_scale_B);
	gsl_vector_free      ((*tmp)->dTdx3_B_n_scale_B);
	gsl_vector_free      ((*tmp)->K_xti_dTdx1_a_n_scale_a);
	gsl_vector_free      ((*tmp)->K_xti_dTdx2_a_n_scale_a);
	gsl_vector_free      ((*tmp)->K_xti_dTdx2_a_n_scale_a);
    gsl_vector_free      ((*tmp)->K_xtm_dTdx1_B_n_scale_B);
	gsl_vector_free      ((*tmp)->K_xtm_dTdx2_B_n_scale_B);
    gsl_vector_free      ((*tmp)->K_xtm_dTdx3_B_n_scale_B);
    gsl_matrix_free      ((*tmp)->dTdx1);
	gsl_matrix_free      ((*tmp)->dTdx2);
	gsl_matrix_free      ((*tmp)->dTdx3);

	/****��Ժ���Bg.....()****************/
	gsl_vector_free      ((*tmp)->Y);
	gsl_vector_free      ((*tmp)->lambda_a);
	gsl_vector_free      ((*tmp)->lambda_B);
	gsl_matrix_free      ((*tmp)->K_a);
	gsl_matrix_free      ((*tmp)->K_B);
	gsl_matrix_free      ((*tmp)->K_a_inv);
	gsl_matrix_free      ((*tmp)->K_B_inv);
	gsl_permutation_free      ((*tmp)->Perm3);
    gsl_vector_free      ((*tmp)->a_m_xsens);
	gsl_vector_free      ((*tmp)->a_m);
	gsl_vector_free      ((*tmp)->B_m_xsens);
	gsl_vector_free      ((*tmp)->B_m);


	/****�ͷŴ洢�ռ�*********************/
	free(*cal);
	free(*env);
	free(*tmp);

	return 0;
}

