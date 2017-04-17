/*
1�������������
2���ߣ����У����ӣ�����
3��Ԫ�������õ���һ���㷨�����ж�����������ȫ�ף�����Խ�߾���Խ�ã�����û����Ҫ��
	һ�׶�����Щ���㷨�����ü򵥵�ֵȡ����һЩ���Ǻ�������
4��ϸ�鼮���Կ�������ʽ���Ե���ԭ����Ԭ����������ǰ�����Ȿ��PDF��þ��ˣ�����һ�뻹û�ܳ�͸��
5�������ü��ٶȼ���������̬�����ò�ƶ������ɻ��ģ��ɻ��ϵļ��ٶȼƵ�����������Ķ������̫��
Ӧ�ò���ֱ����������һ�ף���һ���������������Ȩ�ط������������ɣ�������������̬�͸�����˵��~
*/
#define EulerAngle_Type float	//��������
#define Quaternion_Type float
#define Acc_Type int
#define Gyro_Type int
#define Euler_Martix_Type float

struct EulerAngle		//ŷ���ǽṹ��
{
	EulerAngle_Type Roll, Pitch, Yaw;
}
struct Quaternion		//��Ԫ���ṹ��
{
	Quaternion_Type q0, q1, q2, q3;
}
struct Acc		//���ٶ�ֵ�ṹ��
{
	Acc_Type x, y, z;
}
struct Gyro		//������ֵ�ṹ��
{
	Gyro_Type x, y, z;
}
struct Euler_Martix		//ŷ������̬������ṹ��
{
	Euler_Martix_Type T11,T12,T13,	T21,T22,T23,	T31,T32,T33;
}

Quaternion Normalize(Quaternion e)		//��Ԫ����һ��
{
	Quaternion_Type s = (Quaternion_Type)Math.Sqrt(e.q0 * e.q0 + e.q1 * e.q1 + e.q2 * e.q2 + e.q3 * e.q3);
	e.q0 /= s;
	e.q1 /= s;
	e.q2 /= s;
	e.q3 /= s;
	return e;
}

Quaternion Multiply_L1(Acc lacc)		//һ���㷨
{
	Quaternion Q_result;
	Q_result.q0 = BQ.q0 - BQ.q1 * lacc.x / 2 - BQ.q2 * lacc.y / 2 - BQ.q3 * lacc.z / 2;
	Q_result.q1 = BQ.q1 + BQ.q0 * lacc.x / 2 + BQ.q2 * lacc.z / 2 - BQ.q3 * lacc.y / 2;
	Q_result.q2 = BQ.q2 + BQ.q0 * lacc.y / 2 - BQ.q1 * lacc.z / 2 + BQ.q3 * lacc.x / 2;
	Q_result.q3 = BQ.q3 + BQ.q0 * lacc.z / 2 + BQ.q1 * lacc.y / 2 - BQ.q2 * lacc.x / 2;
	Q_result = Quaternion_Normalize(Q_result);
	return Q_result;
}

Euler_Martix Q_to_EM(Quaternion e)		//����Ԫ���任��ŷ���ǣ���̬������T
{
	Euler_Martix result;
	Euler_Martix_Type q00,q01,q02,q03,q11,q12,q13,q22,q23,q33;
	q00=e.q0*e.q0;
	q01=e.q0*e.q1;
	q02=e.q0*e.q2;
	q03=e.q0*e.q3;
	q11=e.q1*e.q1;
	q12=e.q1*e.q2;
	q13=e.q1*e.q3;
	q22=e.q2*e.q2;
	q23=e.q2*e.q3;
	q33=e.q3*e.q3;
	result.T11=q00+q11-q22-q33;
	result.T12=2*(q12+q03);
	result.T13=2*(q13-q02);
	result.T21=2*(q12-q03);
	result.T22=q22-q33+q00-q11;
	result.T23=2*(q23+q01);
	result.T31=2*(q13+q02);
	result.T32=2*(q23-q01);
	result.T33=q33-q22-q11+q00;
	return result;
}

Quaternion Ea_to_Qu(EulerAngle ea)		//��ŷ���Ǳ任����Ԫ��      ����������������ˣ��þ����Ǹ���
{
	Quaternion result;

	Quaternion_Type CosY = Math.Cos(ea.Yaw * .5);
	Quaternion_Type SinY = Math.Sin(ea.Yaw * .5);	
	Quaternion_Type CosP = Math.Cos(ea.Pitch * .5f);
	Quaternion_Type SinP = Math.Sin(ea.Pitch * .5);
	Quaternion_Type CosR = Math.Cos(ea.Roll * .5f);
	Quaternion_Type SinR = Math.Sin(ea.Roll * .5f);
	
	result.q0 = CosY * CosP * CosR + SinY * SinP * SinR;
	result.q1 = CosY * CosP * SinR - SinY * SinP * CosR;
	result.q2 = CosY * SinP * CosR + SinY * CosP * SinR;
	result.q3 = SinY * CosP * CosR - CosY * SinP * SinR;
	return result;
}

Acc	coordinate_body_to_inertia(Euler_Martix EM,Acc lacc)		//����������ٶȱ任����������
{
//���ɻ�����Ҫ��ʡ��
}

EulerAngle EM_to_EU(Euler_Martix lem)        //����̬��������ȡ��̬��
{
	EulerAngle result;
	result.Yaw = Math.Atan2(lem.T12, lem.T11);
	result.Pitch = -Math.Asin(lem.T13);
	result.Roll = Math.Atan2(lem.T23, lem.T33);
	return result;
}

void main()
{
	Quaternion BQ;//������̬��Ԫ��
	Euler_Martix BEM;//����ŷ������
	EulerAngle BEA;//����ŷ����
	
	BQ.q0=1;//��ʼ����Ԫ��
	BQ.q1=BQ.q2=BQ.q3=0;//��ʼ����Ԫ��

	char gx,gy,gz;//���������������ᣬ����װֵ
	
	while��1��//Ҫ���ϸ��£�����Ū��ѭ������Ҫ֪����ĸ������ʣ�����ת�������һЩ������
			//����ת���Ĳ��ɽ����ԣ���������Խ��Խ�ã����飡
	{
		gx=25;
		gy=14;
		gz=4;
		//����Ϊ��ȡ�����ǵ�����ֵ������㸳ֵ��Ϊ���ӹ�
		
		EulerAngle lea;//����ŷ��Сת�ǣ�ÿ�θ��µ�Сת�ǣ�������õ�
		
		lea.Yaw = ((float)gx / 62200 * Math.PI);
		lea.Pitch = ((float)gy / 62200 * Math.PI);
		lea.Roll = ((float)gz/ 62200 * Math.PI);
		//�������ǵ�����ֵת��Ϊ�Ƕ�ֵ�������Լ��Ĳ����ʡ����ȵȲ���ת�����������������в����ϸ㶮
		
		BQ = Multiply_L1(BQ, lea);//������̬��Ԫ�أ��ɾ���Ԫ����Сת�Ǹ��µõ�����Ԫ��
		
		BEM = Q_to_EM(BQ);//�����������̬��Ԫ��ת��ŷ������
		BEA = EM_to_EU(BEM);//��ŷ����������ȡŷ����
	}
}

