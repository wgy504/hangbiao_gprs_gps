#ifndef __SYSTEM_H__
#define	__SYSTEM_H__
//��ʱ������IAR�Դ�������ʹ�õ�
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 


//�Զ������ݽṹ������ʹ��
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

typedef unsigned char INT8U;
typedef unsigned int INT16U;
typedef unsigned char BOOL;
typedef unsigned long INT32U;

#define FALSE		0
#define TRUE		(!FALSE)


#define MAX_BATT_COUNT			16		                        /*���������*/
#define MAX_BASE_COUNT			7		                        /*����վ������1�����ӻ�վ+6���ڽ���վ*/
#define MAX_RHYTHM_COUNT		10		                        /*����Զ����������*/


/////////////////////////////////////eep address assignment end////////////////////////////////
#define ABATT_COUNT		4                                               //�������
#define BBATT_COUNT		1                                               //���õ����
#define MBATT_COUNT             0                                               //��������

#define ALARM_B0_ERR	(1<<0)
#define ALARM_B1_ERR	(1<<1)
#define ALARM_B2_ERR	(1<<2)
#define ALARM_B3_ERR	(1<<3)
#define ALARM_ALL_LOW	(1<<7)


#define DBG_ERR	(1<<0)//������Ϣ��־
#define DBG_INF	(1<<1)//������Ϣ��־
#define DBG_MSG	(1<<2)//��������Ϣ
#define DBG_OFF (0)//ȫ��

/*BasicInfo*/
typedef struct dev_basic_info
{
	INT8U	OEMCode[5];			                                /*���̴���*/
	INT8U	SWVersion[3];		                                        /*����汾*/
	INT8U	PTType;				                                /*Э������*/
	INT8U	PTVersion[3];		                                        /*Э��汾*/
	INT8U	HWVersion[3];		                                        /*Ӳ���汾*/
}DEV_BASIC_INFO;



//enum battStatus
//{
//  Status_Idle=0,							        /*����*/
//  Status_Charge=1,						                /*���*/
//  Status_Discharge=2,		 				                /*�ŵ�*/
//  Status_Bypass=3							        /*��·*/
//};
//



typedef struct batt_info
{
  INT16U Volt;			                                                /*��ǰ��ѹmV  */

  INT8U ErrCount;
  INT8U LastErrCount;


}BATT_INFO;


/*�豸������Ϣ-������Ϣ*/
typedef struct device_info
{
  INT8U BattCount;			                                        /*��������� �����16*/
  INT32U BattSoc;				                                /*���������mAh*/
  INT16U MaxChgVolt;			                                        /*������ѹ��mV*/
  INT16U MinDischgVolt;		                                                /*��С�ŵ��ѹ��mV*/
  INT16U ChargePollIntval;	                                                /*�����ѯ���ڣ�S*/
  INT16U DisChargePollIntval;	                                                /*�ŵ���ѯ���ڣ�S*/


  INT8U SysDebugLevel;		                                                /*��־��ӡ����*/			

  INT8U SubDebugLevel;		                                                /*��־��ӡ����*/	

  INT8U ChgDebugLevel;		                                                /*��־��ӡ����*/
  
}DEVICE_INFO;


/*�豸ʵʱ����-����ʵʱ����*/
typedef struct device_data
{
  
  //�����Ϣ
  BATT_INFO	BattInfo[ABATT_COUNT+BBATT_COUNT+MBATT_COUNT];	                /*�����Ϣ---ʵʱ*/
  INT16U ChargeCurr;                                                            /*������---��λmA ʵʱ*/
  INT16U DisChargeCurr;                                                         /*�ŵ����---��λmA ʵʱ*/
  INT32U ChargeSoc;                                                             /*�������---��λmAh ʵʱ*/
  INT32U DisChargeSoc;                                                          /*�ŵ�����---��λmAh ʵʱ*/
  
  INT8U ChargeBattNo;								/*��ǰ��������---ʵʱ*/
  INT8U DischargeBattNo;							/*��ǰ�ŵ������---ʵʱ*/
  INT8U LastChargeBattNo;							/*�ϴγ�������---ʵʱ*/
  INT8U LastDischargeBattNo;						        /*�ϴηŵ������---ʵʱ*/
  INT8U BattEnFlag;                                                             /*�����Ч��־---ʵʱ*/  
  
  INT8U CurrBackBattNo;                                                         /*��ǰ���õı��õ�����---ʵʱ*/  
}DEVICE_DATA;


#endif
