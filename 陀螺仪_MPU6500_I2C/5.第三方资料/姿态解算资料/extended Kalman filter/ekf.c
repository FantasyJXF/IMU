/**ekf.c***/
/**���ܣ�ʵ����չ�������˲�������logfiles***/
//#define _POSIX_SOURCE

#include "ekf.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>

//using namespace std;

#define  MaxModelError  0.05         //���ģ�����
#define  MAXLINE        1000         //��ȡ�ļ��������


int view_vector(const   gsl_vector   *a)
{
   /**��ʾ���������Ϣ��С����**/	
	size_t    i;
	fprintf(stdout," pointer to a = %i, a%i element vector \n",
		    (int)a,
			(int)(a->size));  //��δ���*m

	for(i=0;i<(a->size);i++)
	{
			
			printf(" a(%i) = %.9g\n",i,gsl_vector_get(a,i));
	}		
	
	return  0;
}

int view_matrix(const   gsl_matrix   *m)
{
   /**��ʾ���������Ϣ��С����**/	
	size_t    i,j;
	fprintf(stdout," pointer to m = %i, a (%i x %i) matrix \n",
		    (int)m,
			(int)m->size1,
			(int)m->size2);  //��δ���*m

	for(i=0;i<(m->size1);i++)
	{
		printf("\t");
		for(j=0;j<(m->size2);j++)
		{
			printf("%.3e",gsl_matrix_get(m,i,j));
		}
		printf("\n");
	}
	return  0;
}

int  ekf_read_log(FILE *fd_log,gsl_vector *Y,double *t_kpl)
{
	char line[MAXLINE];
	/***����������xsens���ݴ洢��asci***/
	/****/

	if(fgets(line,MAXLINE,fd_log)!=NULL)
	{
		if(strlen(line)==89)
		{
			/**�����ǹ��Բ�������**/
			gsl_vector_set(Y,0,(double)atoi(line+0));
			gsl_vector_set(Y,1,(double)atoi(line+6));
			gsl_vector_set(Y,2,(double)atoi(line+12));
			gsl_vector_set(Y,3,(double)atoi(line+18));
			gsl_vector_set(Y,4,(double)atoi(line+24));
			gsl_vector_set(Y,5,(double)atoi(line+30));
			gsl_vector_set(Y,6,(double)atoi(line+36));
			gsl_vector_set(Y,7,(double)atoi(line+42));
			gsl_vector_set(Y,8,(double)atoi(line+48));
			*t_kpl=(double)atof(line+60);
		}
		else
		{
			if(line[0]==0x25)
			{
				fprintf(stderr,
					    " main(): line starts like a comment, ignore \n");
			}
			else
			{
				fprintf(stderr,
					     " main(): line has wrong length and is not a comment \n"); 
				return 2;
			}
		}
	}
	else
	{
		fprintf(stderr,
		        " main(): error or just newline, exit \n");
        return 1;
	}     
   return 0;
}

int ekf_measure(gsl_vector  *Y)  //�������˲��Ĺ۲���
{
	int dummy;
	
	/**size_t  i;**/
	/**���Ժ����ĳ�ʼ��**/
	/**��ʵ����ʸ��������h()**/
	gsl_vector_set(Y,0,32700.0);
	gsl_vector_set(Y,1,31300.0);
	gsl_vector_set(Y,2,30000.0);
	gsl_vector_set(Y,3,15000.0);
    gsl_vector_set(Y,4,16300.0);
	gsl_vector_set(Y,5,17700.0);
	gsl_vector_set(Y,6,27800.0);
	gsl_vector_set(Y,7,35500.0);
	gsl_vector_set(Y,8,42800.0);

	dummy = (int) gsl_vector_get (Y,1);
	return 1;
}


int ekf_time(double  *t_kpl,double *t_k) //ʱ���ʼ��
{
	/*********���Գ�ʼ������������t_kpl��ֵ********/ 
  *t_kpl  = 0.2;
  *t_k    = 0.01;

  return  1;
}


int ekf_f(const gsl_vector  *x_kk,
          struct temp_data   *tmp,
          gsl_vector   *f)
{
   /**��ȡ��ǰ����״̬x_kk,ϵͳ������u_k,ϵͳʱ��t_k,
    **������������ϵʽf(x(t),u(t),t)*************/
   int mult_ok;
   unsigned int status=0;


   double  Phi,Theta,Psi;
   double  sin_Phi,cos_Phi,tan_Theta,cos_Theta;

   gsl_vector_view   pqr;
   gsl_vector_view   f_top;

   /**�������ʸ��fΪ0***/
   gsl_vector_set_zero(f);
   
   /*********���øú����γ���ʱ״̬����********/
   gsl_vector_memcpy(tmp->x_kk,x_kk);

   /*********��״̬״̬������ȡ����********/
   Phi     =gsl_vector_get(tmp->x_kk,0);
   Theta   =gsl_vector_get(tmp->x_kk,1);
   Psi     =gsl_vector_get(tmp->x_kk,2);

   /*********�����ܴ��ڵ�����********/
   if(fabs(cos(Theta))<1e-12)
      {
            Theta  = 1.57;
            status = 1;
            fprintf(stderr,
                    " ekf: ekf_f() : nearly singular ,crude fix applie....\n");
       }
 
  /****���нǶȼ��㣨����6��ֵ��**************/
   sin_Phi      =  sin(Phi);
   cos_Phi      =  cos(Phi);
   tan_Theta    =  tan(Theta);
   cos_Theta    =  cos(Theta);


 /*****�˶�ѧ����A************************************/  
   gsl_matrix_set(tmp->A,0,0,1.0);
   gsl_matrix_set(tmp->A,0,1,sin_Phi*tan_Theta);  
   gsl_matrix_set(tmp->A,0,2,cos_Phi*tan_Theta);
   gsl_matrix_set(tmp->A,1,0,0.0);
   gsl_matrix_set(tmp->A,1,1,cos_Phi);
   gsl_matrix_set(tmp->A,1,2,-sin_Phi);
   gsl_matrix_set(tmp->A,2,0,0.0);
   gsl_matrix_set(tmp->A,2,1,sin_Phi/cos_Theta);
   gsl_matrix_set(tmp->A,2,1,cos_Phi/cos_Theta);
  

 /*****ѡȡpqrʸ��������������˵Ľ��**************/  
   pqr     = gsl_vector_subvector(tmp->x_kk,3,3);
   f_top   = gsl_vector_subvector(f,0,3);
   
   mult_ok  = gsl_blas_dgemv(CblasNoTrans,1.0,tmp->A, &pqr.vector,1.0,&f_top.vector);
   return (mult_ok);
}


int ekf_F_xa(const gsl_vector *x_kk,gsl_matrix *F_x)
{
  /*****���㿨�����˲����Ķ�̬���̵��ſ˱Ⱦ���
  **�ⲿ�ֽ��������ϵļ���*******************/
  unsigned int status;
  double Phi,Theta,Psi,p,q,r;
  double sin_Phi,cos_Phi,cos_Theta,sin_Theta,tan_Theta;

  
/*****ѡȡ״̬ʸ����ҪԪ��(����״̬��)********************/  
  Phi   = gsl_vector_get(x_kk,0);
  Theta = gsl_vector_get(x_kk,1);
  Psi   = gsl_vector_get(x_kk,2);
      p = gsl_vector_get(x_kk,3);
      q = gsl_vector_get(x_kk,4);
      r = gsl_vector_get(x_kk,5);

/***********���þ���Ϊ0******************/ 
   gsl_matrix_set_zero(F_x);



 /****У���Ƕ�***************/ 
 if (fabs(cos(Theta))<1e-12)
{
      Theta  =  1.57;
      status =  1;
      fprintf(stderr,
              " ekf: ekf_f() : nearly singular, crude fix applied...\n");

    /****�Ƕȼ��㣬**************/ 
     sin_Phi      =  sin(Phi);
     cos_Phi      =  cos(Phi);
     sin_Theta    =  sin(Theta);
     cos_Theta    =  cos(Theta);
     tan_Theta    =  tan(Theta);


  /****�����ſ˱Ⱦ���321��ת˳��**************************/
    gsl_matrix_set(F_x,0,0,
                   cos_Phi*tan_Theta*q-sin_Phi*tan_Theta*r);
    gsl_matrix_set(F_x,0,1,
                   sin_Phi*(pow(tan_Theta,2.0)+1)*q+
				   cos_Phi*(pow(tan_Theta,2.0)+1)*r);
    gsl_matrix_set(F_x,0,2,0.0);
	gsl_matrix_set(F_x,0,3,1.0);
	gsl_matrix_set(F_x,0,4,sin_Phi*tan_Theta);
	gsl_matrix_set(F_x,0,5,cos_Phi*tan_Theta);
	gsl_matrix_set(F_x,0,6,0.0);
	gsl_matrix_set(F_x,0,7,0.0);
	gsl_matrix_set(F_x,0,8,0.0);


	gsl_matrix_set(F_x,1,0,-sin_Phi*q-cos_Phi*r);
	gsl_matrix_set(F_x,1,1,0.0);
	gsl_matrix_set(F_x,1,2,0.0);
	gsl_matrix_set(F_x,1,3,0.0);
	gsl_matrix_set(F_x,1,4,cos_Phi);
	gsl_matrix_set(F_x,1,5,-sin_Phi);
	gsl_matrix_set(F_x,1,6,0.0);
	gsl_matrix_set(F_x,1,7,0.0);
	gsl_matrix_set(F_x,1,8,0.0);


    gsl_matrix_set(F_x,2,0,
                  (cos_Phi*q/cos_Theta)-(sin_Phi*r/cos_Theta));
    gsl_matrix_set(F_x,2,1,
		           sin_Phi*sin_Theta*q/pow(cos_Theta,2.0)+
				   cos_Phi*sin_Theta*r/pow(cos_Theta,2.0));
    gsl_matrix_set(F_x,2,2,0.0);
	gsl_matrix_set(F_x,2,3,0.0);
	gsl_matrix_set(F_x,2,4,sin_Phi/cos_Theta);
    gsl_matrix_set(F_x,2,5,cos_Phi/cos_Theta);
	gsl_matrix_set(F_x,2,6,0.0);
    gsl_matrix_set(F_x,2,7,0.0);
	gsl_matrix_set(F_x,2,8,0.0);


    return  1;
 }
}


int ekf_g(const gsl_vector *x_kk,const double *t_k, gsl_matrix *G)
  {
	 /*****����������ǰ����״̬x_kk��ϵͳʱ��t_k��������ϵ������
	  *****��ϵG(x(t),t),ϵͳ�����������
	  *****�����У�
	  *****1 t_kû��ʹ�ã����ں���g()�Ķ������Ѿ������������
	  *****���ֲ��䣬��������ekf()��g()��ʵ�ֲ��ܱ���һ��*****/
	int dummy;
	unsigned int status = 0;


	gsl_matrix_set_identity(&G);


    dummy  =  gsl_vector_get(x_kk,1);
	dummy  = (int) t_k;


	return(status);
}


 int ekf_h(const gsl_vector  *x_kk,
	       const gsl_vector  *u_k,
		   const double      *t_k,
		   const gsl_vector  *Y,
		   const struct calibr_data  *cal,
		   const struct enviro_data  *env,
		   const double *scale_a,
		   const double *scale_B,
		   struct temp_data *tmp,
		   gsl_vector  *h)

 {
	 /*****���㿨�����˲����Ĺ۲ⷽ��*********/
     /*****����������ǰ����״̬x_kk��ϵͳ����Ҫ��u_k�Լ�ϵͳʱ�䷵��������
	  *****��ϵh(x(t),u(t),t)
	  *****�����У�
	  *****1 ��Ԫ����ʾ
	  *****2 ��k_a�ȵ�ת��������ʼ������ *****/

	 int dummy;
	 int add_ok,scale_ok,mult_ok;
	 double Phi,Theta,Psi;
	 double sin_Phi,cos_Phi,sin_Theta,cos_Theta,sin_Psi,cos_Psi;
   
     gsl_vector_view  Y_view;
	 gsl_vector_view  h_view;
        
    /**************�ӵ������굽���������ת�ƾ���*************/

	 /****��״̬ʸ������ȡ�����Ĳ�����ʸ��********************/
	 Phi   = gsl_vector_get(x_kk,0);
	Theta  = gsl_vector_get(x_kk,1);
     Psi   = gsl_vector_get(x_kk,2);

	 gsl_vector_memcpy(tmp->x_kk_tmp,x_kk);
     Y_view  = gsl_vector_subvector(tmp->x_kk_tmp,3,3);
     gsl_vector_memcpy(tmp->omega,&Y_view.vector);
	 Y_view  = gsl_vector_subvector(tmp->x_kk_tmp,6,3);
	 gsl_vector_memcpy(tmp->lambda_omega,&Y_view.vector);

	/*****�Ƕȼ��㸳ֵ*************/
	 sin_Phi      =sin(Phi);    
     cos_Phi      =  cos(Phi);
     sin_Theta    =  sin(Theta);
     cos_Theta    =  cos(Theta);
	 sin_Psi      =sin(Psi);    
     cos_Psi      =  cos(Psi);
    

	 /*****���ת�ƾ���(321�� ˳��)******/
      gsl_matrix_set  (tmp->T_nb,0,0,cos_Theta*cos_Psi);
	  gsl_matrix_set  (tmp->T_nb,0,1,cos_Theta*sin_Psi);
	  gsl_matrix_set  (tmp->T_nb,0,2,-sin_Theta);
	  gsl_matrix_set  (tmp->T_nb,1,0,sin_Phi*sin_Theta*cos_Psi-cos_Phi*sin_Psi);
      gsl_matrix_set  (tmp->T_nb,1,1,sin_Phi*sin_Theta*sin_Psi+cos_Phi*cos_Psi);
      gsl_matrix_set  (tmp->T_nb,1,2,sin_Phi*cos_Theta);
	  gsl_matrix_set  (tmp->T_nb,2,0,cos_Phi*sin_Theta*cos_Psi+sin_Phi*sin_Psi);
      gsl_matrix_set  (tmp->T_nb,2,1,cos_Phi*sin_Theta*sin_Psi-sin_Phi*cos_Psi);
	  gsl_matrix_set  (tmp->T_nb,2,2,cos_Phi*cos_Theta);


	  /******����h����***************/
	  gsl_vector_memcpy(tmp->a_n_tmp,env->a_n);
	  scale_ok   = gsl_vector_scale(tmp->a_n_tmp,*scale_a);
	  mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                          tmp->T_nb,tmp->a_n_tmp,0.0,tmp->a_b);
	  mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                          cal->K_xti,tmp->a_b,0.0,tmp->a_b_xsens);
	  mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                          cal->K_a,tmp->a_b_xsens,0.0,tmp->a_b_digital);
      add_ok     = gsl_vector_add(tmp->a_b_digital,cal->lambda_a);

      /******�۲����ת�����ʷ���***********/
      mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                          cal->K_xti,��tmp->omega,0.0,tmp->omega_xsens);
      mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                          cal->K_w,tmp->omega_xsens,0.0,tmp->omega_digital);
	  add_ok     = gsl_vector_add(tmp->omega_digital,tmp->lambda_omega);

      /******�۲�����ų�����***********/
      gsl_vector_memcpy(tmp->B_n_tmp,env->B_n);
	  scale_ok   = gsl_vector_scale(tmp->B_n_tmp,*scale_B);
	  mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                          tmp->T_nb,tmp->B_n_tmp,0.0,tmp->B_b);
	  mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                          cal->K_xtm,tmp->B_b,0.0,tmp->B_b_xsens);
	  mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                          cal->K_B,tmp->B_b_xsens,0.0,tmp->B_b_digital);
      add_ok     = gsl_vector_add(tmp->B_b_digital,cal->lambda_B);

      /******������Ĺ۲�ֵ�������ʸ��***********/
      h_view  =  gsl_vector_subvector(h,0,3);
      gsl_vector_memcpy(&h_view.vector,tmp->a_b_digital);
	  h_view  =  gsl_vector_subvector(h,3,3);
	  gsl_vector_memcpy(&h_view.vector,tmp->omega_digital);
      h_view  =  gsl_vector_subvector(h,6,3);
      gsl_vector_memcpy(&h_view.vector,tmp->B_b_digital);


	 /******��������ֵ***********/
	  dummy = (int)gsl_vector_get(u_k,1);
	  dummy = (int)t_k;
	  dummy = (int)env;
	  dummy = (int)Y;

	  return 1;
 }

int ekf_H_xa(const  gsl_vector *x_kkpl,
			 const  double *t_k,
			 const  struct calibr_data *cal,
			 const  struct enviro_data *env,
			 const  double *scale_a,
			 const  double *scale_B,
			 struct temp_data  *tmp,
			 gsl_matrix  *H_x)

{
	int dummy,scale_ok,mult_ok;
	double Phi,Theta,Psi;
	double sin_Phi,cos_Phi,sin_Theta,cos_Theta,sin_Psi,cos_Psi;
	double dh1dx1,dh1dx2,dh1dx3,dh2dx1,dh2dx2,dh2dx3,dh3dx1,dh3dx2,dh3dx3;
    double dh7dx1,dh7dx2,dh7dx3,dh8dx1,dh8dx2,dh8dx3,dh9dx1,dh9dx2,dh9dx3;

	gsl_vector_view   x_kkpl_view;
    gsl_vector_view   K_a1_view;
    gsl_vector_view   K_a2_view;
	gsl_vector_view   K_a3_view;
	gsl_vector_view   K_B1_view;
	gsl_vector_view   K_B2_view;
	gsl_vector_view   K_B3_view;
	gsl_matrix_view   H_x_view;
	
		
	 /******����h���ſ˱Ⱦ���***********/

    /******���ɻ���ʸ��**********/
    gsl_vector_memcpy(tmp->x_kkpl,x_kkpl);
    gsl_vector_memcpy(tmp->a_n_scale_a,env->a_n);
	gsl_vector_memcpy(tmp->B_n_scale_B,env->B_n);
    

	/******����T_nb��΢��***********/
    Phi  = gsl_vector_get(tmp->x_kkpl,0);
   Theta = gsl_vector_get(tmp->x_kkpl,1);
    Psi  = gsl_vector_get(tmp->x_kkpl,2);

	x_kkpl_view  = gsl_vector_subvector(tmp->x_kkpl,3,3);
	gsl_vector_memcpy(tmp->omega,&x_kkpl_view.vector);
    x_kkpl_view  = gsl_vector_subvector(tmp->x_kkpl,6,3);
	gsl_vector_memcpy(tmp->lambda_omega,&x_kkpl_view.vector);
  
	/******���нǶȸ�ֵ***********/
	sin_Phi     = sin(Phi);
	cos_Phi     = cos(Phi);
	sin_Theta   = sin(Theta);
	cos_Theta   = cos(Theta);
    sin_Psi     = sin(Psi);
	cos_Psi     = cos(Psi);


	/****dTx1*******************/
	gsl_matrix_set(tmp->dTdx1,0,0,0.0);
    gsl_matrix_set(tmp->dTdx1,0,1,0.0);
	gsl_matrix_set(tmp->dTdx1,0,2,0.0);
	gsl_matrix_set(tmp->dTdx1,1,0,cos_Phi*sin_Theta*cos_Psi+sin_Phi*sin_Psi);
    gsl_matrix_set(tmp->dTdx1,1,1,cos_Phi*sin_Theta*sin_Psi-sin_Phi*cos_Psi);
    gsl_matrix_set(tmp->dTdx1,1,2,cos_Phi*cos_Theta);
	gsl_matrix_set(tmp->dTdx1,2,0,-sin_Phi*sin_Theta*cos_Psi+cos_Phi*sin_Psi);
    gsl_matrix_set(tmp->dTdx1,2,1,-sin_Phi*sin_Theta*sin_Psi-cos_Phi*cos_Psi);
    gsl_matrix_set(tmp->dTdx1,2,2,-sin_Phi*cos_Theta);

	/****dTx2*******************/
    gsl_matrix_set(tmp->dTdx2,0,0,-sin_Theta*cos_Psi);
    gsl_matrix_set(tmp->dTdx2,0,1,-sin_Theta*sin_Psi);
	gsl_matrix_set(tmp->dTdx2,0,2,-cos_Theta);
	gsl_matrix_set(tmp->dTdx2,1,0,sin_Phi*cos_Theta*cos_Psi);
    gsl_matrix_set(tmp->dTdx2,1,1,sin_Phi*cos_Theta*sin_Psi);
    gsl_matrix_set(tmp->dTdx2,1,2,-sin_Phi*sin_Theta);
	gsl_matrix_set(tmp->dTdx2,2,0,cos_Phi*cos_Theta*cos_Psi);
    gsl_matrix_set(tmp->dTdx2,2,1,cos_Phi*cos_Theta*sin_Psi);
    gsl_matrix_set(tmp->dTdx2,2,2,-cos_Phi*sin_Theta);


	/****dTx3*******************/
    gsl_matrix_set(tmp->dTdx3,0,0,-cos_Theta*sin_Psi);
    gsl_matrix_set(tmp->dTdx3,0,1,cos_Theta*cos_Psi);
	gsl_matrix_set(tmp->dTdx3,0,2,0.0);
	gsl_matrix_set(tmp->dTdx3,1,0,-sin_Phi*sin_Theta*sin_Psi-cos_Phi*cos_Psi);
    gsl_matrix_set(tmp->dTdx3,1,1,sin_Phi*cos_Theta*cos_Psi-cos_Phi*sin_Psi);
    gsl_matrix_set(tmp->dTdx3,1,2,0.0);
	gsl_matrix_set(tmp->dTdx3,2,0,-cos_Phi*sin_Theta*sin_Psi+sin_Phi*cos_Psi);
    gsl_matrix_set(tmp->dTdx3,2,1,cos_Phi*sin_Theta*cos_Psi+sin_Phi*sin_Psi);
    gsl_matrix_set(tmp->dTdx3,2,2,2,0.0);


    /******����h()��ƫ΢��***********/
    /****dh1dx1*******************/
    scale_ok   = gsl_vector_scale(tmp->a_n_scale_a,*scale_a);
	K_a1_view  = gsl_matrix_row(cal->K_a,0);

	mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         tmp->dTdx1,tmp->a_n_scale_a,0.0,
								 tmp->dTdx1_a_n_scale_a);
    
	mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         cal->K_xti,tmp->dTdx1_a_n_scale_a,0.0,
								 tmp->K_xti_dTdx1_a_n_scale_a);
	mult_ok    = gsl_blas_ddot(&K_a1_view.vector,tmp->K_xti_dTdx1_a_n_scale_a,
		                       &dh1dx1);

	/****dh1dx2*******************/
   	mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         tmp->dTdx2,tmp->a_n_scale_a,0.0,
								 tmp->dTdx2_a_n_scale_a);
    
	mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         cal->K_xti,tmp->dTdx2_a_n_scale_a,0.0,
								 tmp->K_xti_dTdx2_a_n_scale_a);
	mult_ok    = gsl_blas_ddot(&K_a1_view.vector,tmp->K_xti_dTdx2_a_n_scale_a,
		                       &dh1dx2);


	/****dh1dx3*******************/
   	mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         tmp->dTdx3,tmp->a_n_scale_a,0.0,
								 tmp->dTdx3_a_n_scale_a);
    
	mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         cal->K_xti,tmp->dTdx3_a_n_scale_a,0.0,
								 tmp->K_xti_dTdx3_a_n_scale_a);
	mult_ok    = gsl_blas_ddot(&K_a1_view.vector,tmp->K_xti_dTdx3_a_n_scale_a,
		                       &dh1dx3);

   /****dh2dx1*******************/
   	K_a2_view  = gsl_matrix_row(cal->K_a,1);

    mult_ok    = gsl_blas_ddot(&K_a2_view.vector,tmp->K_xti_dTdx1_a_n_scale_a,
		                       &dh2dx1);
	mult_ok    = gsl_blas_ddot(&K_a2_view.vector,tmp->K_xti_dTdx2_a_n_scale_a,
		                       &dh2dx2);
	mult_ok    = gsl_blas_ddot(&K_a2_view.vector,tmp->K_xti_dTdx3_a_n_scale_a,
		                       &dh2dx3);

	
   /****dh3dx1*******************/
	K_a3_view  = gsl_matrix_row(cal->K_a,2);
	mult_ok    = gsl_blas_ddot(&K_a3_view.vector,tmp->K_xti_dTdx1_a_n_scale_a,
		                       &dh3dx1);

  /****dh3dx2*******************/
	mult_ok    = gsl_blas_ddot(&K_a3_view.vector,tmp->K_xti_dTdx2_a_n_scale_a,
		                       &dh3dx2);
	 
    /****dh3dx3*******************/
	mult_ok    = gsl_blas_ddot(&K_a3_view.vector,tmp->K_xti_dTdx3_a_n_scale_a,
		                       &dh3dx3);
 
   /****dh7dx1*******************/
	scale_ok   = gsl_vector_scale(tmp->B_n_scale_B,*scale_B);
	K_B1_view  = gsl_matrix_row(cal->K_B,0);
    
	mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         tmp->dTdx1,tmp->B_n_scale_B,0.0,
								 tmp->dTdx1_B_n_scale_B);
    
	mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         cal->K_xtm,tmp->dTdx1_B_n_scale_B,0.0,
								 tmp->K_xtm_dTdx1_B_n_scale_B);
    
    mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         tmp->dTdx1,tmp->B_n_scale_B,0.0,
								 tmp->dTdx1_B_n_scale_B);

	mult_ok    = gsl_blas_ddot(&K_B1_view.vector,tmp->K_xtm_dTdx1_B_n_scale_B,
		                       &dh7dx1);

   
	/****dh7dx2*******************/ 
    mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         tmp->dTdx2,tmp->B_n_scale_B,0.0,
								 tmp->dTdx2_B_n_scale_B);
    mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         cal->K_xtm,tmp->dTdx2_B_n_scale_B,0.0,
								 tmp->K_xtm_dTdx2_B_n_scale_B);
	mult_ok    = gsl_blas_ddot(&K_B1_view.vector,tmp->K_xtm_dTdx2_B_n_scale_B,
		                       &dh7dx2);
   
	/****dh7dx3******************/ 
    mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         tmp->dTdx3,tmp->B_n_scale_B,0.0,
								 tmp->dTdx3_B_n_scale_B);
    mult_ok    = gsl_blas_dgemv(CblasNoTrans,1.0,
		                         cal->K_xtm,tmp->dTdx3_B_n_scale_B,0.0,
								 tmp->K_xtm_dTdx3_B_n_scale_B);
	mult_ok    = gsl_blas_ddot(&K_B1_view.vector,tmp->K_xtm_dTdx3_B_n_scale_B,
		                       &dh7dx3);


	/****dh8dx1*******************/
	K_B2_view  = gsl_matrix_row(cal->K_B,1);    
    mult_ok    = gsl_blas_ddot(&K_B2_view.vector,tmp->K_xtm_dTdx1_B_n_scale_B,
		                       &dh8dx1);

   	/****dh8dx2*******************/
	mult_ok    = gsl_blas_ddot(&K_B2_view.vector,tmp->K_xtm_dTdx2_B_n_scale_B,
		                       &dh8dx2);

	/****dh8dx3*******************/
	mult_ok    = gsl_blas_ddot(&K_B2_view.vector,tmp->K_xtm_dTdx3_B_n_scale_B,
		                       &dh8dx3);

   /****dh9dx1*******************/
    K_B3_view  = gsl_matrix_row(cal->K_B,2);    
    mult_ok    = gsl_blas_ddot(&K_B3_view.vector,tmp->K_xtm_dTdx1_B_n_scale_B,
		                       &dh9dx1);
	/****dh9dx2*******************/
    mult_ok    = gsl_blas_ddot(&K_B3_view.vector,tmp->K_xtm_dTdx2_B_n_scale_B,
		                       &dh9dx2);
	/****dh9dx3*******************/
    mult_ok    = gsl_blas_ddot(&K_B3_view.vector,tmp->K_xtm_dTdx3_B_n_scale_B,
		                       &dh9dx3);

	

	/*****���������***********/
	/**dh1...3,dx1...3***********/
	gsl_matrix_set(H_x,0,0,dh1dx1);
	gsl_matrix_set(H_x,0,1,dh1dx2);
	gsl_matrix_set(H_x,0,2,dh1dx3);
    gsl_matrix_set(H_x,1,0,dh2dx1);
	gsl_matrix_set(H_x,1,1,dh2dx2);
	gsl_matrix_set(H_x,1,2,dh2dx3);
	gsl_matrix_set(H_x,2,0,dh3dx1);
	gsl_matrix_set(H_x,2,1,dh3dx2);
	gsl_matrix_set(H_x,2,2,dh3dx3);

	/**dh4...6,dx4...6***********/
	H_x_view = gsl_matrix_submatrix(H_x,3,3,3,3);
	mult_ok  = gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,
		                      1.0,cal->K_w,cal->K_xti,
							  0.0,&H_x_view.matrix);
	/******dh4...6,dx7..9**/
    H_x_view = gsl_matrix_submatrix(H_x,3,6,3,3);
	gsl_matrix_set_identity(&H_x_view.matrix);

    /******dh7...9,dx1..3**/
    gsl_matrix_set(H_x,6,0,dh1dx1);
	gsl_matrix_set(H_x,6,1,dh1dx2);
	gsl_matrix_set(H_x,6,2,dh1dx3);
    gsl_matrix_set(H_x,7,0,dh2dx1);
	gsl_matrix_set(H_x,7,1,dh2dx2);
	gsl_matrix_set(H_x,7,2,dh2dx3);
	gsl_matrix_set(H_x,8,0,dh3dx1);
	gsl_matrix_set(H_x,8,1,dh3dx2);
	gsl_matrix_set(H_x,8,2,dh3dx3);


	/***view_matrix(H_x):***/
	dummy = (int)t_k;
	return 1;
	}

	int ekf_Bg_modle_is_valid(const gsl_vector  *x_kk,
		                      const gsl_vector  *Y,
							  const struct calibr_data  *cal,
                              const struct enviro_data  *env,
							  const double  *t_kpl,
							  struct temp_data *tmp,
							  double *scale_a,
							  double *scale_B)
	{
		/*****������ٶȼƺʹ����ƵĲ���ֵ���������Ϊ״̬�ͼ���У������*****/
		/*****status = 1,�������
		 *****status = 2,���ٶȼ�ģ����Ч��(>���ģ�����MaxModleError)
		 *****status = 3,�ų�ģ����Ч��(>���ģ�����MaxModleError)
         *****status = 4������ģ�;���Ч
         *****MaxModleError����ekf�Ŀ�ʼ�����*****************************/
		int status_a,status_B;
		int scale_ok,add_ok,lu_ok,invert_ok,mult_ok;
		double norm_a_m,norm_a_n,norm_B_m,norm_B_n;
		double modle_error_a,modle_error_B;
		double A,B,Phi_z,Theta_z,Psi_z;

		int signum = 0;

		gsl_vector_view  Y_top;
        gsl_vector_view  Y_bot;

		/***���ƺ����������븴�Ƶ������ʸ�������󡢵�****/
		gsl_vector_memcpy(tmp->x_kk,x_kk);
    	gsl_vector_memcpy(tmp->Y,Y);
		gsl_vector_memcpy(tmp->lambda_a,cal->lambda_a);
        gsl_vector_memcpy(tmp->lambda_B,cal->lambda_B);
        gsl_matrix_memcpy(tmp->K_a,cal->K_a);
        gsl_matrix_memcpy(tmp->K_B,cal->K_B);

		Y_top = gsl_vector_subvector(tmp->Y,0,3);
        Y_bot = gsl_vector_subvector(tmp->Y,6,3);


		/******************************************
		  ��������ļ��ٶ�ֵ
		  ****************************************/
		scale_ok  = gsl_vector_scale(tmp->lambda_a,-1.0);
		add_ok    = gsl_vector_add(&Y_top.vector,tmp->lambda_a);
		

		/**��ת������������ʼ���ļ�������ʱ��******/
        lu_ok      = gsl_linalg_LU_decomp(tmp->K_a,tmp->Perm3,&signum);
		invert_ok  = gsl_linalg_LU_invert(tmp->K_a,tmp->Perm3,tmp->K_a_inv);


		mult_ok  = gsl_blas_dgemv(CblasNoTrans,1.0,
			                      tmp->K_a_inv,
								  &Y_top.vector,0.0,
								  tmp->a_m_xsens);

        mult_ok  = gsl_blas_dgemv(CblasNoTrans,1.0,
			                      cal->K_xti,
				                  tmp->a_m_xsens,0.0,
								  tmp->a_m);

		/****���ݲ���ֵ��view_vector(a_m)=[1.544 0.274 -8.914]'******/
		/****����a_m��a_n�Ĺ淶��************************************/
		norm_a_m = gsl_blas_dnrm2(tmp->a_m);
        norm_a_n = gsl_blas_dnrm2(env->a_n);
		modle_error_a = fabs(norm_a_m-norm_a_n);
		if(modle_error_a > MaxModelError*norm_a_n)
		{
			status_a = 1;
		}

		*scale_a = norm_a_m/norm_a_n;


		scale_ok     = gsl_vector_scale(tmp->lambda_B,-1.0);
		add_ok       = gsl_vector_add(&Y_bot.vector,tmp->lambda_B);
        lu_ok        = gsl_linalg_LU_decomp(tmp->K_B,tmp->Perm3,&signum);
        invert_ok    = gsl_linalg_LU_invert(tmp->K_B,tmp->Perm3,tmp->K_B_inv);

		mult_ok      = gsl_blas_dgemv(CblasNoTrans,1.0,
			                           tmp->K_B_inv,
									   &Y_bot.vector,0.0,
									   tmp->B_m_xsens);

		mult_ok      = gsl_blas_dgemv(CblasNoTrans,1.0,
			                           cal->K_xtm,									   
									   tmp->B_m_xsens,0.0,
									   tmp->B_m);

		norm_B_m = gsl_blas_dnrm2(tmp->B_m);
        norm_B_n = gsl_blas_dnrm2(env->B_n);
		modle_error_B = fabs(norm_B_m-norm_B_n);
		if(modle_error_B > MaxModelError*norm_B_n)
		{
			status_B += 1;
		}
		*scale_B = norm_B_m/norm_B_n;
		/****����Ƕ�*****/
		Phi_z     = atan2(gsl_vector_get(tmp->a_m,1),
			              gsl_vector_get(tmp->a_m,2));

		Theta_z   = asin(-gsl_vector_get(tmp->a_m,0)/9.81);
		A  =  cos(Theta_z)*gsl_vector_get(tmp->B_m,0)/(*scale_B)+
			  sin(Theta_z)*sin(Phi_z)*-gsl_vector_get(tmp->B_m,1)/(*scale_B)+
			  sin(Theta_z)*cos(Phi_z)*-gsl_vector_get(tmp->B_m,2)/(*scale_B);

      	B  =  cos(Phi_z)*-gsl_vector_get(tmp->B_m,1)/(*scale_B)-
			  sin(Phi_z)*-gsl_vector_get(tmp->B_m,2)/(*scale_B);

		Psi_z = atan2(-B,A);
        Psi_z = atan2(gsl_vector_get(tmp->B_m,1),
			          gsl_vector_get(tmp->B_m,0));

		/****��������͵��ĵ���*******/
		fprintf(stdout," %f %f %f %f %f %f %f %f %f \n",
			   *t_kpl,Phi_z*180/3.14,Theta_z*180/3.14,Psi_z*180/3.14,
			   gsl_vector_get(tmp->B_m,0),
			   gsl_vector_get(tmp->B_m,1),
			   gsl_vector_get(tmp->B_m,2),
			   *scale_B,*scale_a);

		return 1;
	}

	int ekf(const gsl_vector *Y,
		    const gsl_vector *x_kk,
			const double *t_k,
			const double *t_kpl,
			const gsl_vector *u_k,
			const gsl_matrix *P_kk,
			const gsl_matrix *R,
			const gsl_matrix *Q,
			struct calibr_data *cal,
			struct enviro_data *env,
			struct temp_data *tmp,
			gsl_vector *x_kkpl,
			gsl_vector *x_kplkpl,
			gsl_matrix *P_kplk,
			gsl_matrix *P_kplkpl)
	{
		/**** ��������*****************************/
		/****1.�۲�ʸ��Y,*************************
		 ****2.��ǰ����״̬x_kk������ʱ��t_k******
		 ****3.��ǰʱ��t_k+1**********************
		 ****4.��������u_k��ʱ��t_k***************
		 ****5.��ǰ����Э������� P_kk************
		 ****6.����������Э����R******************
		 ****7.��������Э����Q********************
		 *************��������********************
         ****1.���ƺ�У��״̬��x_kk+1��x_k+1k+1***
         ****2.���ƺ�У��Э�������P_kk+1��P_k+1k+1

         ****�����˲������ر���******************/
		 int dummy;
		 int lu_ok,inv_ok,mult_ok;
		 unsigned int f_status,g_status,h_status, F_xa_status,H_xa_status;
         unsigned int Bg_valid;
		 double  T;
		 double  scale_a;
         double  scale_B;

		 /****ʱ�����******************/
		 
		 clock_t start_time;
         clock_t stop_time;
		 //struct SYSTEMTIME start_time;
	     //struct SYSTEMTIME stop_time;
 
		 int signum  = 0;

         /****���ڿ�ʼ�˲�****/
        // gettimeofday(&start_time);
		 start_time = clock();

		 /****���㿨�����˲���ʱ�䲽����T=t_k+1-t_k****/
		 T = *t_kpl-*t_k;

         /****����F_x((x_kk,u_k,t_k)�ſ˱Ⱦ���)****/
         F_xa_status = ekf_F_xa(x_kk,tmp->F_x);
		 if(F_xa_status == 0)
		 {
			 fprintf(stderr," ekf(): F xa return: %d exit \n",F_xa_status);
		 }

         /****����Phi(һ��)****/
         gsl_matrix_scale(tmp->F_x,T); 
		 gsl_matrix_set_identity(tmp->Eye); 
         gsl_matrix_add(tmp->Eye,tmp->F_x); 
         gsl_matrix_memcpy(tmp->Phi,tmp->Eye);

         /****����Psi****/
         gsl_matrix_set_zero(tmp->Psi);
         

		 /****����Gamma(һ��)****/
		 g_status = ekf_g(x_kk,0,tmp->G);
		 if(g_status !=0)
		 {
			 fprintf(stderr," ekf(): ekf_g() returns: %d exit \n",g_status);
			 return g_status;
		 }

		 gsl_matrix_scale(tmp->G,T);
         gsl_matrix_set_identity(tmp->Eye);
		 gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,
			            (tmp->Eye),tmp->G,0.0,tmp->Gamma);

        f_status = ekf_f(x_kk,tmp,tmp->f);
		 if(f_status !=0)
		 {
			 fprintf(stderr," ekf(): ekf_g() returns: %d exit \n",f_status);
			 return f_status;
		 }

		 gsl_vector_scale(tmp->f,T);
         gsl_vector_memcpy(x_kkpl,x_kk);
		 gsl_vector_add(x_kkpl,tmp->f);
			 


        /****���㿨����״̬Ԥ��****/
		 gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,Q,
			            tmp->Gamma,0.0,tmp->QGammat);
		 
		 gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,
			            tmp->Gamma,tmp->QGammat,0.0,tmp->GammaQGammat);

		 gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,P_kk,
			            tmp->Phi,0.0,tmp->P_kkPhit);
		 
		 gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,
			            tmp->Phi,tmp->P_kkPhit,0.0,tmp->PhiP_kkPhit);

		 gsl_matrix_add(tmp->PhiP_kkPhit,tmp->GammaQGammat);
		 gsl_matrix_memcpy(P_kplk,tmp->PhiP_kkPhit);


		 /***���㿨�����˲������棬��Ҫ���ȼ���۲ⷽ��**********/
		 Bg_valid = ekf_Bg_modle_is_valid(x_kkpl,
			                              Y,cal,
										  env,t_kpl,
										  tmp,&scale_a,
										  &scale_B);

		 h_status     = ekf_h(x_kkpl,u_k,t_k,Y,cal,env,&scale_a,&scale_B,tmp,tmp->h);
         H_xa_status  = ekf_H_xa(x_kkpl,t_k,cal,env,&scale_a,&scale_B,tmp,tmp->H_x);

		 gsl_blas_dgemm(CblasNoTrans,CblasTrans,
			            1.0,P_kplk,tmp->H_x,0.0,tmp->P_kplkHt);

		 gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,
		                tmp->H_x,tmp->P_kplkHt,0.0,tmp->HP_kplkHt);
	     
		 
		 /****״̬У��***********************************/
		 gsl_vector_memcpy(tmp->Yminh,tmp->h);
         gsl_vector_memcpy(tmp->Y_tmp,Y);
         gsl_vector_scale(tmp->Yminh,-1.0);
         gsl_vector_add(tmp->Yminh,tmp->Y_tmp);

		 mult_ok = gsl_blas_dgemv(CblasNoTrans,1.0,tmp->K_kplk,
			                      tmp->Yminh,0.0,tmp->KYminh);

		 gsl_vector_memcpy(x_kplkpl,x_kkpl);
		 gsl_vector_add(x_kplkpl,tmp->KYminh);

        /****Э����У��***********************************/
         gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,
			            1.0,tmp->K_kplk,tmp->H_x,0.0,tmp->ImKH);

		 gsl_matrix_scale(tmp->ImKH,-1.0);
         gsl_matrix_set_identity(tmp->I);
         gsl_matrix_add(tmp->ImKH,tmp->I);


		 gsl_blas_dgemm(CblasNoTrans,CblasTrans,
			            1.0,P_kplk,tmp->ImKH,0.0,tmp->PImKHt);


         gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,
			            1.0,tmp->ImKH,tmp->PImKHt,0.0,tmp->ImKHPImKHt);

         gsl_blas_dgemm(CblasNoTrans,CblasTrans,
			            1.0,R,tmp->K_kplk,0.0,tmp->RKt);


         gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,
			            1.0,tmp->K_kplk,tmp->RKt,0.0,tmp->KRKt);

		 gsl_matrix_memcpy(P_kplkpl,tmp->KRKt);
         gsl_matrix_add(P_kplkpl,tmp->ImKHPImKHt);
		 dummy = (int)gsl_vector_get(u_k,1);

		 //gettimeofday(&stop_time,0);
         stop_time = clock();
		 return 1;
}
int main(int argc,char *argv[])
{
	int i,log_file_size;
	int init_status,free_status,read_ok;
	int ekf_status = 0;

	/***����ָ�������ָ��***********/
	double *t_k   = NULL;
	double *t_kpl = NULL;
    
   /***����ָ��ʸ����ָ��***********/
	gsl_vector  *x_kk      = NULL;
    gsl_vector  *u_k       = NULL;
	gsl_vector  *Y         = NULL;
    gsl_vector  *x_kkpl    = NULL; 
    gsl_vector  *x_kplkpl  = NULL; 

  /***����ָ������ָ��***********/
	gsl_matrix    *P_kk       = NULL;
    gsl_matrix    *Q          = NULL;
    gsl_matrix    *R          = NULL;
    gsl_matrix    *P_kkpl     = NULL;
	gsl_matrix    *P_kplkpl   = NULL;


  /***����ָ��ṹ���ָ��***********/
   struct calibr_data  *cal = NULL;
   struct enviro_data  *env = NULL;
   struct temp_data    *tmp = NULL;


   /*****��������ļ�˵��**********/
   FILE  *fd_log;
   FILE  *fd_res;


   char    *prog_name;
   char    *log_file = NULL;
   char    *res_file = NULL;

   /****�������뻺����**********/
   char  line[MAXLINE];


   /*****�����������**********/
   if (argc == 1)
   {
	   /****û�������������ӡ��Ϣ���˳�*******/
	   fprintf(stdout," main(): usage: ekf <logfile> <resultfile> \n");
       fprintf(stdout," <resultfile> is overwritten by default \n");

	   exit(1);
   }

   else if(argc == 3)
   {
	   /***������������*********/
	   prog_name  = argv[0];
	   log_file   = argv[1];
	   res_file   = argv[2];

   }

   else
   {
	   /****������������********************/
	   fprintf(stdout," main(): usage: ekf <logfile> <resultfile> \n");
       fprintf(stdout," pls speity a valid logfile and result file name \n");
       exit(2);
   }


   /***��logfile,��resultfile************/
       fprintf(stderr," main(): opening logfile %s for reading \n",log_file);
       fprintf(stderr," main(): opening resultfile %s for writing \n",res_file);

       fd_log = fopen(log_file,"r");
	   fd_res = fopen(res_file,"w");
       
	   if(fd_log == NULL)
	   {
		   fprintf(stdout," main(): error opening log file %s for reading \n",log_file);   
	   
	       exit(3);
	   }

	   if(fd_res == NULL)
	   {
		   fprintf(stdout," main(): error opening result file %s for writing \n",res_file);
           exit(3);
	   }
       

	   log_file_size = 0;
	   while (fgets(line,MAXLINE,fd_log)!=NULL)
		   log_file_size++;
	       rewind(fd_log);

	   /****��ʼ���˲���*********/
        fprintf(stderr," main(): calling ekf_init() \n");
        init_status = ekf_init(&x_kk,&x_kkpl,&x_kplkpl,
			                   &P_kk,&P_kkpl,&P_kplkpl,
							   &u_k, &Y,
							   &t_k,&t_kpl,
							   &R,&Q,&cal,&env,&tmp);

		
		/****�����ݷ������ʸ��Y*********/
		/****���ó�ʼʱ���ͺ�(��ȡ����ʱ����������ʼ��ע��)*********/
		read_ok  = ekf_read_log(fd_log,Y,t_kpl);
		*t_k     = *t_kpl;

      /****��ʼ�����˲�ѭ��*********/
         fprintf(stderr," main(): starting main processing loop \n");
		 i = 0;
		 while(i<log_file_size)
		 {
			 read_ok = ekf_read_log(fd_log,Y,t_kpl);
			 if(read_ok != 0)
			 {
				 fprintf(stderr," main(): error or end of file reached \n");
				 break;
			 }
			 ekf_status = ekf(Y,x_kk,t_k,t_kpl,u_k,P_kk,R,Q,cal,env,tmp,\
				              x_kkpl,x_kplkpl,P_kkpl,P_kplkpl);

			 /***д����ļ�****/

             fprintf(fd_res,
				     "%f %f %f %f %f %f %f %f %f %f\n",
				     *t_kpl,
					 gsl_vector_get(x_kplkpl,0),
					 gsl_vector_get(x_kplkpl,1),
					 gsl_vector_get(x_kplkpl,2),
					 gsl_vector_get(x_kplkpl,3),
					 gsl_vector_get(x_kplkpl,4),
					 gsl_vector_get(x_kplkpl,5),
					 gsl_vector_get(x_kplkpl,6),
					 gsl_vector_get(x_kplkpl,7),
					 gsl_vector_get(x_kplkpl,8));

			 /***�¸�ѭ���������µĳ�ʼ����*********/
			 gsl_vector_memcpy(x_kk,x_kplkpl);
             gsl_matrix_memcpy(P_kk,P_kplkpl);
			 *t_k = *t_kpl;
			 i++;
		 }

		 /****�ͷŴ洢�ռ�***********************/
          fprintf(stderr," main(): calling ekf_free() \n");
		  free_status = ekf_free(&x_kk,&x_kkpl,&x_kplkpl,
			                     &P_kk,&P_kkpl,&P_kplkpl,
								 &u_k, &Y,
								 &t_k,&t_kpl,
								 &R, &Q,&cal,&env,&tmp);

		  /***�ر��ļ�***************/
            fprintf(stderr," main(): closing file \n");
			fclose(fd_log);
			fclose(fd_res);

			fprintf(stderr," main(): end \n");
			exit(0);
}

















      


         













