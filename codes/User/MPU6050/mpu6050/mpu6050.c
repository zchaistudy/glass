#include "stm32f10x_it.h"
#include "./usart/bsp_usart1.h"
#include "./i2c/bsp_i2c.h"
#include "./systick/bsp_SysTick.h"

#include "./MPU6050/mpu6050/mpu6050.h"


#include "mltypes.h"

#include "inv_mpu_dmp_motion_driver.h"
#include "log.h"
#include "invensense.h"
#include "invensense_adv.h"
#include "eMPL_outputs.h"
#include "mpu.h"
#include "inv_mpu.h"
#include "log.h"
#include "packet.h"
#include <stdio.h>


#define TASK_ENABLE 0
extern int flag_volume;
extern unsigned int Task_Delay[NumOfTask];
int flag_FALLING = 0;	//ä��ˤ����־ =1��ʾˤ���� =0��ʾ����
 


/* Private typedef -----------------------------------------------------------*/
/* Data read from MPL. */
#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)
#define PRINT_COMPASS   (0x08)
#define PRINT_EULER     (0x10)
#define PRINT_ROT_MAT   (0x20)
#define PRINT_HEADING   (0x40)
#define PRINT_PEDO      (0x80)
#define PRINT_LINEAR_ACCEL (0x100)
#define PRINT_GRAVITY_VECTOR (0x200)

volatile uint32_t hal_timestamp = 0;
#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)
#define COMPASS_ON      (0x04)

#define MOTION          (0)
#define NO_MOTION       (1)

/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (20)

#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)

#define PEDO_READ_MS    (1000)
#define TEMP_READ_MS    (500)
#define COMPASS_READ_MS (100)

struct rx_s {
    unsigned char header[3];
    unsigned char cmd;
};

struct hal_s {
    unsigned char lp_accel_mode;
    unsigned char sensors;
    unsigned char dmp_on;
    unsigned char wait_for_tap;
    volatile unsigned char new_gyro;
    unsigned char motion_int_mode;
    unsigned long no_dmp_hz;
    unsigned long next_pedo_ms;
    unsigned long next_temp_ms;
    unsigned long next_compass_ms;
    unsigned int report;
    unsigned short dmp_features;
    struct rx_s rx;
};
static struct hal_s hal = {0};
/* Every time new gyro data is available, this function is called in an
 * ISR context. In this example, it sets a flag protecting the FIFO read
 * function.
 */

/* USB RX binary semaphore. Actually, it's just a flag. Not included in struct
 * because it's declared extern elsewhere.
 */
volatile unsigned char rx_new;

unsigned char *mpl_key = (unsigned char*)"eMPL 5.1";

/* Platform-specific information. Kinda like a boardfile. */
struct platform_data_s {
    signed char orientation[9];
};

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from the
 * driver(s).
 * TODO: The following matrices refer to the configuration on internal test
 * boards at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static struct platform_data_s gyro_pdata = {
		
		 .orientation = { 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1}
};

#if defined MPU9150 || defined MPU9250
static struct platform_data_s compass_pdata = {
    .orientation = { 0, 1, 0,
                     1, 0, 0,
                     0, 0, -1}
};
#define COMPASS_ENABLED 1
#elif defined AK8975_SECONDARY
static struct platform_data_s compass_pdata = {
    .orientation = {-1, 0, 0,
                     0, 1, 0,
                     0, 0,-1}
};
#define COMPASS_ENABLED 1
#elif defined AK8963_SECONDARY
static struct platform_data_s compass_pdata = {
    .orientation = {-1, 0, 0,
                     0,-1, 0,
                     0, 0, 1}
};
#define COMPASS_ENABLED 1
#endif



/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* ---------------------------------------------------------------------------*/



#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
/**
  * @brief  ���ƴ��ڷ���1���ַ�
  * @param  c:Ҫ���͵��ַ�
  * @retval none
  */
//void usart_send_char(uint8_t c)
//{
//	while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_TXE)==RESET); //ѭ������,ֱ���������
//	USART_SendData(DEBUG_USARTx,c);
//}



/*�������ܣ���������������λ��Э��д����ʾ��̬�ĳ�����λ��0512�汾��
 *����Э��˵����鿴��λ������İ���˵����
 */
//void Data_Send_Status(float Pitch,float Roll,float Yaw)
//{
//	unsigned char i=0;
//	unsigned char _cnt=0,sum = 0;
//	unsigned int _temp;
//	u8 data_to_send[50];

//	data_to_send[_cnt++]=0xAA;
//	data_to_send[_cnt++]=0xAA;
//	data_to_send[_cnt++]=0x01;
//	data_to_send[_cnt++]=0;
//	
//	_temp = (int)(Roll*100);
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = 0-(int)(Pitch*100);
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = (int)(Yaw*100);
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp = 0;
//	data_to_send[_cnt++]=BYTE3(_temp);
//	data_to_send[_cnt++]=BYTE2(_temp);
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	
//	data_to_send[_cnt++]=0xA0;
//	
//	data_to_send[3] = _cnt-4;
//	//��У��
//	for(i=0;i<_cnt;i++)
//		sum+= data_to_send[i];
//	data_to_send[_cnt++]=sum;
//	
//	//���ڷ�������
//	for(i=0;i<_cnt;i++)
//		usart_send_char(data_to_send[i]);
//}

/*�������ܣ���������������λ��Э��д����ʾ���������ݣ���λ��0512�汾��
 *����Э��˵����鿴��λ������İ���˵����
 */
//void Send_Data(int16_t *Gyro,int16_t *Accel)
//{
//	unsigned char i=0;
//	unsigned char _cnt=0,sum = 0;
////	unsigned int _temp;
//	u8 data_to_send[50];

//	data_to_send[_cnt++]=0xAA;
//	data_to_send[_cnt++]=0xAA;
//	data_to_send[_cnt++]=0x02;
//	data_to_send[_cnt++]=0;
//	

//	data_to_send[_cnt++]=BYTE1(Accel[0]);
//	data_to_send[_cnt++]=BYTE0(Accel[0]);
//	data_to_send[_cnt++]=BYTE1(Accel[1]);
//	data_to_send[_cnt++]=BYTE0(Accel[1]);
//	data_to_send[_cnt++]=BYTE1(Accel[2]);
//	data_to_send[_cnt++]=BYTE0(Accel[2]);
//	
//	data_to_send[_cnt++]=BYTE1(Gyro[0]);
//	data_to_send[_cnt++]=BYTE0(Gyro[0]);
//	data_to_send[_cnt++]=BYTE1(Gyro[1]);
//	data_to_send[_cnt++]=BYTE0(Gyro[1]);
//	data_to_send[_cnt++]=BYTE1(Gyro[2]);
//	data_to_send[_cnt++]=BYTE0(Gyro[2]);
//	data_to_send[_cnt++]=0;
//	data_to_send[_cnt++]=0;
//	data_to_send[_cnt++]=0;
//	data_to_send[_cnt++]=0;
//	data_to_send[_cnt++]=0;
//	data_to_send[_cnt++]=0;
//	
//	data_to_send[3] = _cnt-4;
//	//��У��
//	for(i=0;i<_cnt;i++)
//		sum+= data_to_send[i];
//	data_to_send[_cnt++]=sum;
//	
//	//���ڷ�������
//	for(i=0;i<_cnt;i++)
//		usart_send_char(data_to_send[i]);
//}

extern struct inv_sensor_cal_t sensors;


/* Get data from MPL.
 * TODO: Add return values to the inv_get_sensor_type_xxx APIs to differentiate
 * between new and stale data.
 */
static void read_from_mpl(float Angle[4])
{
    long msg, data[9];
    int8_t accuracy;
    unsigned long timestamp;
    float float_data[3] = {0};

    MPU_DEBUG_FUNC();
    if (inv_get_sensor_type_quat(data, &accuracy, (inv_time_t*)&timestamp)) {
       /* Sends a quaternion packet to the PC. Since this is used by the Python
        * test app to visually represent a 3D quaternion, it's sent each time
        * the MPL has new data.
        */
        eMPL_send_quat(data);

        /* Specific data packets can be sent or suppressed using USB commands. */
        if (hal.report & PRINT_QUAT)
            eMPL_send_data(PACKET_DATA_QUAT, data);
    }

    if (hal.report & PRINT_ACCEL) {
        if (inv_get_sensor_type_accel(data, &accuracy,
            (inv_time_t*)&timestamp))
            eMPL_send_data(PACKET_DATA_ACCEL, data);
    }
    if (hal.report & PRINT_GYRO) {
        if (inv_get_sensor_type_gyro(data, &accuracy,
            (inv_time_t*)&timestamp))
            eMPL_send_data(PACKET_DATA_GYRO, data);

    }
#ifdef COMPASS_ENABLED
    if (hal.report & PRINT_COMPASS) {
        if (inv_get_sensor_type_compass(data, &accuracy,
            (inv_time_t*)&timestamp))
            eMPL_send_data(PACKET_DATA_COMPASS, data);
    }
#endif
    if (hal.report & PRINT_EULER) {
        if (inv_get_sensor_type_euler(data, &accuracy,
            (inv_time_t*)&timestamp))
            eMPL_send_data(PACKET_DATA_EULER, data);

    }
		
		/* send data to Upper computer */
    if(1)
    {
				unsigned long timestamp;
			
				/* ��ȡŷ���� */
			  if (inv_get_sensor_type_euler(data, &accuracy,(inv_time_t*)&timestamp))
				{
					float Pitch,Roll,Yaw;
					Pitch =data[0]*1.0/(1<<16) ;
					Roll = data[1]*1.0/(1<<16);
					Yaw = data[2]*1.0/(1<<16);							

					Angle[0] =	Pitch;
					Angle[1] =	Roll;
					Angle[2] =	Yaw;

						/* ��ȡ�¶� */
						mpu_get_temperature(data,(inv_time_t*)&timestamp); 							
						Angle[3] =	data[0]*1.0/(1<<16);
				
				}	
		}
		
    if (hal.report & PRINT_ROT_MAT) {
        if (inv_get_sensor_type_rot_mat(data, &accuracy,
            (inv_time_t*)&timestamp))
            eMPL_send_data(PACKET_DATA_ROT, data);
    }
    if (hal.report & PRINT_HEADING) {
        if (inv_get_sensor_type_heading(data, &accuracy,
            (inv_time_t*)&timestamp))
            eMPL_send_data(PACKET_DATA_HEADING, data);
    }
    if (hal.report & PRINT_LINEAR_ACCEL) {
        if (inv_get_sensor_type_linear_acceleration(float_data, &accuracy, (inv_time_t*)&timestamp)) {
        	MPL_LOGI("Linear Accel: %7.5f %7.5f %7.5f\r\n",
        			float_data[0], float_data[1], float_data[2]);  


         }
    }
    if (hal.report & PRINT_GRAVITY_VECTOR) {
            if (inv_get_sensor_type_gravity(float_data, &accuracy,
                (inv_time_t*)&timestamp))
            	MPL_LOGI("Gravity Vector: %7.5f %7.5f %7.5f\r\n",
            			float_data[0], float_data[1], float_data[2]);
						

    }
    if (hal.report & PRINT_PEDO) {
        unsigned long timestamp;
        get_tick_count(&timestamp);
        if (timestamp > hal.next_pedo_ms) {
            hal.next_pedo_ms = timestamp + PEDO_READ_MS;
            unsigned long step_count, walk_time;
            dmp_get_pedometer_step_count(&step_count);
            dmp_get_pedometer_walk_time(&walk_time);
            MPL_LOGI("Walked %ld steps over %ld milliseconds..\n", step_count,
            walk_time);
					
					
        }
    }

    /* Whenever the MPL detects a change in motion state, the application can
     * be notified. For this example, we use an LED to represent the current
     * motion state.
     */
    msg = inv_get_message_level_0(INV_MSG_MOTION_EVENT |
            INV_MSG_NO_MOTION_EVENT);
    if (msg) {
        if (msg & INV_MSG_MOTION_EVENT) {
            MPL_LOGI("Motion!\n");
        } else if (msg & INV_MSG_NO_MOTION_EVENT) {
            MPL_LOGI("No motion!\n");
        }
    }
}

#ifdef COMPASS_ENABLED
void send_status_compass() {
	long data[3] = { 0 };
	int8_t accuracy = { 0 };
	unsigned long timestamp;
	inv_get_compass_set(data, &accuracy, (inv_time_t*) &timestamp);
	MPL_LOGI("Compass: %7.4f %7.4f %7.4f ",
			data[0]/65536.f, data[1]/65536.f, data[2]/65536.f);
	MPL_LOGI("Accuracy= %d\r\n", accuracy);

}
#endif

///* Handle sensor on/off combinations. */
//static void setup_gyro(void)
//{
//    unsigned char mask = 0, lp_accel_was_on = 0;

//    MPU_DEBUG_FUNC();
//    if (hal.sensors & ACCEL_ON)
//        mask |= INV_XYZ_ACCEL;
//    if (hal.sensors & GYRO_ON) {
//        mask |= INV_XYZ_GYRO;
//        lp_accel_was_on |= hal.lp_accel_mode;
//    }
//#ifdef COMPASS_ENABLED
//    if (hal.sensors & COMPASS_ON) {
//        mask |= INV_XYZ_COMPASS;
//        lp_accel_was_on |= hal.lp_accel_mode;
//    }
//#endif
//    /* If you need a power transition, this function should be called with a
//     * mask of the sensors still enabled. The driver turns off any sensors
//     * excluded from this mask.
//     */
//    mpu_set_sensors(mask);
//    mpu_configure_fifo(mask);
//    if (lp_accel_was_on) {
//        unsigned short rate;
//        hal.lp_accel_mode = 0;
//        /* Switching out of LP accel, notify MPL of new accel sampling rate. */
//        mpu_get_sample_rate(&rate);
//        inv_set_accel_sample_rate(1000000L / rate);
//    }
//}

static void tap_cb(unsigned char direction, unsigned char count)
{
	MPU_DEBUG_FUNC();
    switch (direction) {
    case TAP_X_UP:
        MPL_LOGI("Tap X+ ");
        break;
    case TAP_X_DOWN:
        MPL_LOGI("Tap X- ");
        break;
    case TAP_Y_UP:
        MPL_LOGI("Tap Y+ ");
        break;
    case TAP_Y_DOWN:
        MPL_LOGI("Tap Y- ");
        break;
    case TAP_Z_UP:
        MPL_LOGI("Tap Z+ ");
        break;
    case TAP_Z_DOWN:
        MPL_LOGI("Tap Z- ");
        break;
    default:
        return;
    }
    MPL_LOGI("x%d\n", count);
    return;
}

static void android_orient_cb(unsigned char orientation)
{
	MPU_DEBUG_FUNC();
	switch (orientation) {
	case ANDROID_ORIENT_PORTRAIT:
        MPL_LOGI("Portrait\n");
        break;
	case ANDROID_ORIENT_LANDSCAPE:
        MPL_LOGI("Landscape\n");
        break;
	case ANDROID_ORIENT_REVERSE_PORTRAIT:
        MPL_LOGI("Reverse Portrait\n");
        break;
	case ANDROID_ORIENT_REVERSE_LANDSCAPE:
        MPL_LOGI("Reverse Landscape\n");
        break;
	default:
		return;
	}
}


//static inline void run_self_test(void)
//{
//    int result;
//    long gyro[3], accel[3];
//    MPU_DEBUG_FUNC();
//#if defined (MPU6500) || defined (MPU9250)
//    result = mpu_run_6500_self_test(gyro, accel, 0);
//#elif defined (MPU6050) || defined (MPU9150)
//    result = mpu_run_self_test(gyro, accel);
//#endif
//    if (result == 0x7) {
//	MPL_LOGI("Passed!\n");
//        MPL_LOGI("accel: %7.4f %7.4f %7.4f\n",
//                    accel[0]/65536.f,
//                    accel[1]/65536.f,
//                    accel[2]/65536.f);
//        MPL_LOGI("gyro: %7.4f %7.4f %7.4f\n",
//                    gyro[0]/65536.f,
//                    gyro[1]/65536.f,
//                    gyro[2]/65536.f);
//        /* Test passed. We can trust the gyro data here, so now we need to update calibrated data*/

//#ifdef USE_CAL_HW_REGISTERS
//        /*
//         * This portion of the code uses the HW offset registers that are in the MPUxxxx devices
//         * instead of pushing the cal data to the MPL software library
//         */
//        unsigned char i = 0;

//        for(i = 0; i<3; i++) {
//        	gyro[i] = (long)(gyro[i] * 32.8f); //convert to +-1000dps
//        	accel[i] *= 2048.f; //convert to +-16G
//        	accel[i] = accel[i] >> 16;
//        	gyro[i] = (long)(gyro[i] >> 16);
//        }

//        mpu_set_gyro_bias_reg(gyro);

//#if defined (MPU6500) || defined (MPU9250)
//        mpu_set_accel_bias_6500_reg(accel);
//#elif defined (MPU6050) || defined (MPU9150)
//        mpu_set_accel_bias_6050_reg(accel);
//#endif
//#else
//        /* Push the calibrated data to the MPL library.
//         *
//         * MPL expects biases in hardware units << 16, but self test returns
//		 * biases in g's << 16.
//		 */
//    	unsigned short accel_sens;
//    	float gyro_sens;

//		mpu_get_accel_sens(&accel_sens);
//		accel[0] *= accel_sens;
//		accel[1] *= accel_sens;
//		accel[2] *= accel_sens;
//		inv_set_accel_bias(accel, 3);
//		mpu_get_gyro_sens(&gyro_sens);
//		gyro[0] = (long) (gyro[0] * gyro_sens);
//		gyro[1] = (long) (gyro[1] * gyro_sens);
//		gyro[2] = (long) (gyro[2] * gyro_sens);
//		inv_set_gyro_bias(gyro, 3);
//#endif
//    }
//    else {
//            if (!(result & 0x1))
//                MPL_LOGE("Gyro failed.\n");
//            if (!(result & 0x2))
//                MPL_LOGE("Accel failed.\n");
//            if (!(result & 0x4))
//                MPL_LOGE("Compass failed.\n");
//     }

//}

//static void handle_input(void)
//{
//  
//    char c = USART_ReceiveData(DEBUG_USARTx);
//    MPU_DEBUG_FUNC();
//    switch (c) {
//    /* These commands turn off individual sensors. */
//    case '8':
//        hal.sensors ^= ACCEL_ON;
//        setup_gyro();
//        if (!(hal.sensors & ACCEL_ON))
//            inv_accel_was_turned_off();
//        break;
//    case '9':
//        hal.sensors ^= GYRO_ON;
//        setup_gyro();
//        if (!(hal.sensors & GYRO_ON))
//            inv_gyro_was_turned_off();
//        break;
//#ifdef COMPASS_ENABLED
//    case '0':
//        hal.sensors ^= COMPASS_ON;
//        setup_gyro();
//        if (!(hal.sensors & COMPASS_ON))
//            inv_compass_was_turned_off();
//        break;
//#endif
//    /* The commands send individual sensor data or fused data to the PC. */
//    case 'a':
//        hal.report ^= PRINT_ACCEL;
//        break;
//    case 'g':
//        hal.report ^= PRINT_GYRO;
//        break;
//#ifdef COMPASS_ENABLED
//    case 'c':
//        hal.report ^= PRINT_COMPASS;
//        break;
//#endif
//    case 'e':
//        hal.report ^= PRINT_EULER;
//        break;
//    case 'r':
//        hal.report ^= PRINT_ROT_MAT;
//        break;
//    case 'q':
//        hal.report ^= PRINT_QUAT;
//        break;
//    case 'h':
//        hal.report ^= PRINT_HEADING;
//        break;
//    case 'i':
//        hal.report ^= PRINT_LINEAR_ACCEL;
//        break;
//    case 'o':
//        hal.report ^= PRINT_GRAVITY_VECTOR;
//        break;
//#ifdef COMPASS_ENABLED
//	case 'w':
//		send_status_compass();
//		break;
//#endif
//    /* This command prints out the value of each gyro register for debugging.
//     * If logging is disabled, this function has no effect.
//     */
//    case 'd':
//        mpu_reg_dump();
//        break;
//    /* Test out low-power accel mode. */
//    case 'p':
//        if (hal.dmp_on)
//            /* LP accel is not compatible with the DMP. */
//            break;
//        mpu_lp_accel_mode(20);
//        /* When LP accel mode is enabled, the driver automatically configures
//         * the hardware for latched interrupts. However, the MCU sometimes
//         * misses the rising/falling edge, and the hal.new_gyro flag is never
//         * set. To avoid getting locked in this state, we're overriding the
//         * driver's configuration and sticking to unlatched interrupt mode.
//         *
//         * TODO: The MCU supports level-triggered interrupts.
//         */
//        mpu_set_int_latched(0);
//        hal.sensors &= ~(GYRO_ON|COMPASS_ON);
//        hal.sensors |= ACCEL_ON;
//        hal.lp_accel_mode = 1;
//        inv_gyro_was_turned_off();
//        inv_compass_was_turned_off();
//        break;
//    /* The hardware self test can be run without any interaction with the
//     * MPL since it's completely localized in the gyro driver. Logging is
//     * assumed to be enabled; otherwise, a couple LEDs could probably be used
//     * here to display the test results.
//     */
//    case 't':
//        run_self_test();
//        /* Let MPL know that contiguity was broken. */
//        inv_accel_was_turned_off();
//        inv_gyro_was_turned_off();
//        inv_compass_was_turned_off();
//        break;
//    /* Depending on your application, sensor data may be needed at a faster or
//     * slower rate. These commands can speed up or slow down the rate at which
//     * the sensor data is pushed to the MPL.
//     *
//     * In this example, the compass rate is never changed.
//     */
//    case '1':
//        if (hal.dmp_on) {
//            dmp_set_fifo_rate(10);
//            inv_set_quat_sample_rate(100000L);
//        } else
//            mpu_set_sample_rate(10);
//        inv_set_gyro_sample_rate(100000L);
//        inv_set_accel_sample_rate(100000L);
//        break;
//    case '2':
//        if (hal.dmp_on) {
//            dmp_set_fifo_rate(20);
//            inv_set_quat_sample_rate(50000L);
//        } else
//            mpu_set_sample_rate(20);
//        inv_set_gyro_sample_rate(50000L);
//        inv_set_accel_sample_rate(50000L);
//        break;
//    case '3':
//        if (hal.dmp_on) {
//            dmp_set_fifo_rate(40);
//            inv_set_quat_sample_rate(25000L);
//        } else
//            mpu_set_sample_rate(40);
//        inv_set_gyro_sample_rate(25000L);
//        inv_set_accel_sample_rate(25000L);
//        break;
//    case '4':
//        if (hal.dmp_on) {
//            dmp_set_fifo_rate(50);
//            inv_set_quat_sample_rate(20000L);
//        } else
//            mpu_set_sample_rate(50);
//        inv_set_gyro_sample_rate(20000L);
//        inv_set_accel_sample_rate(20000L);
//        break;
//    case '5':
//        if (hal.dmp_on) {
//            dmp_set_fifo_rate(100);
//            inv_set_quat_sample_rate(10000L);
//        } else
//            mpu_set_sample_rate(100);
//        inv_set_gyro_sample_rate(10000L);
//        inv_set_accel_sample_rate(10000L);
//        break;
//	case ',':
//        /* Set hardware to interrupt on gesture event only. This feature is
//         * useful for keeping the MCU asleep until the DMP detects as a tap or
//         * orientation event.
//         */
//        dmp_set_interrupt_mode(DMP_INT_GESTURE);
//        break;
//    case '.':
//        /* Set hardware to interrupt periodically. */
//        dmp_set_interrupt_mode(DMP_INT_CONTINUOUS);
//        break;
//    case '6':
//        /* Toggle pedometer display. */
//        hal.report ^= PRINT_PEDO;
//        break;
//    case '7':
//        /* Reset pedometer. */
//        dmp_set_pedometer_step_count(0);
//        dmp_set_pedometer_walk_time(0);
//        break;
//    case 'f':
//        if (hal.lp_accel_mode)
//            /* LP accel is not compatible with the DMP. */
//            return;
//        /* Toggle DMP. */
//        if (hal.dmp_on) {
//            unsigned short dmp_rate;
//            unsigned char mask = 0;
//            hal.dmp_on = 0;
//            mpu_set_dmp_state(0);
//            /* Restore FIFO settings. */
//            if (hal.sensors & ACCEL_ON)
//                mask |= INV_XYZ_ACCEL;
//            if (hal.sensors & GYRO_ON)
//                mask |= INV_XYZ_GYRO;
//            if (hal.sensors & COMPASS_ON)
//                mask |= INV_XYZ_COMPASS;
//            mpu_configure_fifo(mask);
//            /* When the DMP is used, the hardware sampling rate is fixed at
//             * 200Hz, and the DMP is configured to downsample the FIFO output
//             * using the function dmp_set_fifo_rate. However, when the DMP is
//             * turned off, the sampling rate remains at 200Hz. This could be
//             * handled in inv_mpu.c, but it would need to know that
//             * inv_mpu_dmp_motion_driver.c exists. To avoid this, we'll just
//             * put the extra logic in the application layer.
//             */
//            dmp_get_fifo_rate(&dmp_rate);
//            mpu_set_sample_rate(dmp_rate);
//            inv_quaternion_sensor_was_turned_off();
//            MPL_LOGI("DMP disabled.\n");
//        } else {
//            unsigned short sample_rate;
//            hal.dmp_on = 1;
//            /* Preserve current FIFO rate. */
//            mpu_get_sample_rate(&sample_rate);
//            dmp_set_fifo_rate(sample_rate);
//            inv_set_quat_sample_rate(1000000L / sample_rate);
//            mpu_set_dmp_state(1);
//            MPL_LOGI("DMP enabled.\n");
//        }
//        break;
//    case 'm':
//        /* Test the motion interrupt hardware feature. */
//	#ifndef MPU6050 // not enabled for 6050 product
//	hal.motion_int_mode = 1;
//	#endif 
//        break;

//    case 'v':
//        /* Toggle LP quaternion.
//         * The DMP features can be enabled/disabled at runtime. Use this same
//         * approach for other features.
//         */
//        hal.dmp_features ^= DMP_FEATURE_6X_LP_QUAT;
//        dmp_enable_feature(hal.dmp_features);
//        if (!(hal.dmp_features & DMP_FEATURE_6X_LP_QUAT)) {
//            inv_quaternion_sensor_was_turned_off();
//            MPL_LOGI("LP quaternion disabled.\n");
//        } else
//            MPL_LOGI("LP quaternion enabled.\n");
//        break;
//    default:
//        break;
//    }
//    hal.rx.cmd = 0;
//}


void gyro_data_ready_cb(void)
{

    hal.new_gyro = 1;
}

static void getAnglesFront(void)
{
	inv_error_t result;
	unsigned char accel_fsr;
//  unsigned char new_temp = 0;
	unsigned short gyro_rate, gyro_fsr;
	unsigned long timestamp;
	struct int_param_s int_param;
	 
	/*��ʼ��MPU6050*/
	result = mpu_init(&int_param);
	  if (result) {
		  MPL_LOGE("Could not initialize gyro.result =  %d\n",result);
	  }
		else
		{
			printf("\ndecteted gyro!\n");
		}

	  result = inv_init_mpl();
	  if (result) {
		  MPL_LOGE("Could not initialize MPL.\n");
	  }
	  
	  /* ��������;�����Ԫ�� */
    inv_enable_quaternion();
    inv_enable_9x_sensor_fusion();
    inv_enable_fast_nomot();
    inv_enable_gyro_tc();

    /* Allows use of the MPL APIs in read_from_mpl. */
    inv_enable_eMPL_outputs();

  result = inv_start_mpl();
  if (result == INV_ERROR_NOT_AUTHORIZED) {
      while (1) {
          MPL_LOGE("Not authorized.\n");
      }
  }
  if (result) {
      MPL_LOGE("Could not start the MPL.\n");
  }

    /* Ӳ�����ã��������� */
    /* �������д����� */
    mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);

    /* Push both gyro and accel data into the FIFO. */
    mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
    mpu_set_sample_rate(DEFAULT_MPU_HZ);
	
    /* ������ò���ȷ����ض�����. */
    mpu_get_sample_rate(&gyro_rate);
    mpu_get_gyro_fsr(&gyro_fsr);
    mpu_get_accel_fsr(&accel_fsr);

    /* Sync driver configuration with MPL. */
    /* Sample rate expected in microseconds. */
    inv_set_gyro_sample_rate(1000000L / gyro_rate);
    inv_set_accel_sample_rate(1000000L / gyro_rate);

    /* ����оƬ������ķ�����󡣽�Ӳ����λ����Ϊdps/g/�ȵı������ӡ�*/
    inv_set_gyro_orientation_and_scale(
            inv_orientation_matrix_to_scalar(gyro_pdata.orientation),
            (long)gyro_fsr<<15);
    inv_set_accel_orientation_and_scale(
            inv_orientation_matrix_to_scalar(gyro_pdata.orientation),
            (long)accel_fsr<<15);

    /* ��ʼ��HAL״̬���� */
    hal.sensors = ACCEL_ON | GYRO_ON;
    hal.dmp_on = 0;
    hal.report = 0;
    hal.rx.cmd = 0;
    hal.next_pedo_ms = 0;
    hal.next_compass_ms = 0;
    hal.next_temp_ms = 0;

  /* ָ�����ȡ�ɵ��ȳ�����*/
  get_tick_count(&timestamp);

    dmp_load_motion_driver_firmware();
    dmp_set_orientation(
        inv_orientation_matrix_to_scalar(gyro_pdata.orientation));
    dmp_register_tap_cb(tap_cb);

    dmp_register_android_orient_cb(android_orient_cb);
   
    hal.dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL;
    dmp_enable_feature(hal.dmp_features);
    dmp_set_fifo_rate(DEFAULT_MPU_HZ);
    mpu_set_dmp_state(1);//״̬����
    hal.dmp_on = 1;
}

static void getEulerAngles(float Angle[4])
{
	unsigned char new_temp = 0;
	unsigned long timestamp;

	unsigned long sensor_timestamp;
	int new_data = 0;
	
//	if (USART_GetFlagStatus (DEBUG_USARTx, USART_FLAG_RXNE)) {
//			/* A byte has been received via USART. See handle_input for a list of
//			 * valid commands.
//			 */
//			USART_ClearFlag(DEBUG_USARTx, USART_FLAG_RXNE);
//			handle_input();			
//	}
	
	get_tick_count(&timestamp);

	if (timestamp > hal.next_temp_ms) {
			hal.next_temp_ms = timestamp + TEMP_READ_MS;
			new_temp = 1;
	}

	if (hal.motion_int_mode) {
			/* Enable motion interrupt. */
			mpu_lp_motion_interrupt(500, 1, 5);
			/* Notify the MPL that contiguity was broken. */
			inv_accel_was_turned_off();
			inv_gyro_was_turned_off();
			inv_compass_was_turned_off();
			inv_quaternion_sensor_was_turned_off();
			/* Wait for the MPU interrupt. */
			while (!hal.new_gyro) {
				printf("\r\n%s wait\r\n",__FILE__);
			}
			/* Restore the previous sensor configuration. */
			mpu_lp_motion_interrupt(0, 0, 0);
			hal.motion_int_mode = 0;
	}

	if (!hal.sensors || !hal.new_gyro) {
			return;
	}    
	if (hal.new_gyro && hal.lp_accel_mode) {
			short accel_short[3];
			long accel[3];
			mpu_get_accel_reg(accel_short, &sensor_timestamp);
			accel[0] = (long)accel_short[0];
			accel[1] = (long)accel_short[1];
			accel[2] = (long)accel_short[2];
			inv_build_accel(accel, 0, sensor_timestamp);
			new_data = 1;
			hal.new_gyro = 0;
	} 
	else if (hal.new_gyro && hal.dmp_on) {
			short gyro[3], accel_short[3], sensors;
			unsigned char more;
			long accel[3], quat[4];
			long temperature;

			dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more);
			if (!more)
					hal.new_gyro = 0;
			if (sensors & INV_XYZ_GYRO) {
					/* Push the new data to the MPL. */
					inv_build_gyro(gyro, sensor_timestamp);
					new_data = 1;
					if (new_temp) {
							new_temp = 0;
							/* Temperature only used for gyro temp comp. */
							mpu_get_temperature(&temperature, &sensor_timestamp);
							inv_build_temp(temperature, sensor_timestamp);
					}
			}
			if (sensors & INV_XYZ_ACCEL) {
					accel[0] = (long)accel_short[0];
					accel[1] = (long)accel_short[1];
					accel[2] = (long)accel_short[2];
					inv_build_accel(accel, 0, sensor_timestamp);
					new_data = 1;
			}
			if (sensors & INV_WXYZ_QUAT) {
					inv_build_quat(quat, 0, sensor_timestamp);
					new_data = 1;
			}
	} 
	else if (hal.new_gyro) {
			short gyro[3], accel_short[3];
			unsigned char sensors, more;
			long accel[3];
			long temperature;

			hal.new_gyro = 0;
			mpu_read_fifo(gyro, accel_short, &sensor_timestamp,
					&sensors, &more);
			if (more)
					hal.new_gyro = 1;
			if (sensors & INV_XYZ_GYRO) {
					/* Push the new data to the MPL. */
					inv_build_gyro(gyro, sensor_timestamp);
					new_data = 1;
					if (new_temp) {
							new_temp = 0;
							/* Temperature only used for gyro temp comp. */
							mpu_get_temperature(&temperature, &sensor_timestamp);
							inv_build_temp(temperature, sensor_timestamp);
					}
			}
			if (sensors & INV_XYZ_ACCEL) {
					accel[0] = (long)accel_short[0];
					accel[1] = (long)accel_short[1];
					accel[2] = (long)accel_short[2];
					inv_build_accel(accel, 0, sensor_timestamp);
					new_data = 1;
			}
	}

	if (new_data) {
			inv_execute_on_data();

			/* This function reads bias-compensated sensor data and sensor
			 * fusion outputs from the MPL. The outputs are formatted as seen
			 * in eMPL_outputs.c. This function only needs to be called at the
			 * rate requested by the host.
			 */
			read_from_mpl(Angle);
	}
	
}


void MPU6050Config()
{
	getAnglesFront();
}

//static void delay(void)
//{
//	int i;
//	for(i=10000;i>0;i--);
//}

//static float old_angle[3], new_angle[3];

int PitchFalling(float pitch1, float pitch2)
{
	if(pitch1>0.1)
	{
		if((pitch2-pitch1)>15.0|| (pitch2-pitch1)<-15.0)
		{			
			return 1;
		}
		else
		{
			return 0;
		}	
	}
	else
		return 0;
}





void MPU6050Triaxial(float Angle[4])
{
	static float pitch;   //���ڴ洢��һ�ε���̬��
	getEulerAngles(Angle);
//	if(pitch1-pitch2 != 0)
//		printf("pitch1 == %f ,pitch2 == %f , ��Ϊ��%f\r\n",pitch1,pitch2,pitch1-pitch2);
	if(PitchFalling(pitch, Angle[0]))
	{
		printf("\r\nä��ˤ��\n");//�״�ˤ��
		flag_FALLING=1;
	}
	
}


void Filter(float Angle[4])
{
	if(flag_FALLING == 1)
	{
//			float pitch1, pitch2;
			getEulerAngles(Angle);
//			pitch1=Angle[0];
			mdelay(10);	//��ʱ
		getEulerAngles(Angle);
					mdelay(10);	//��ʱ
		getEulerAngles(Angle);
//		pitch2=Angle[0];
//			printf("�Ѷ���Ϣ���й���\r\n");
//			printf("pitch1 == %f ,pitch2 == %f , ��Ϊ��%f\r\n",pitch1,pitch2,pitch1-pitch2);
	}
}

#ifdef HHH
//MPU6050��̬�Ǽ����Դ���
void MPU6050Triaxial(float Angle[4])
{	
	unsigned long new_count;
	static unsigned long old_count;
	int i,j;
	float pitch1�� pitch2, roll;
//	float yaw;
//	printf("\nmpu 6050 test start");	
	
	getEulerAngles(Angle);
	pitch1=Angle[0];
	roll=Angle[1];
//	yaw=Angle[2];
	
	mdelay(50);	//��ʱ
	getEulerAngles(Angle);
	pitch2=Angle[0];


//	printf("\n������pitch=%.4f", Angle[0]);
//	printf("\n���ֽ�roll=%.4f", Angle[1]);
//	printf("\nƫ����yaw=%.4f", Angle[2]);		

	printf("\n%.4f	%.4f	%.4f", Angle[0],Angle[1],Angle[2]);
	
//	old_angle[0]=Angle[0];
//	old_angle[1]=Angle[1];
//	old_angle[2]=Angle[2];

	dmp_get_pedometer_step_count(&new_count);

//	printf("\r\nWalked steps :  %ld",new_count);

	if(old_count<new_count)
	{
		printf("\r\nhelp!!!!!!");//�����¹ʣ�����ײ��
		flag_FALLING=1;
	}	
	else if(PitchFalling(pitch1, pitch2) || RollFalling(roll))
	{
		printf("\r\nhelp!!!!!!");//�״�ˤ��
		flag_FALLING=1;
	}
	
	old_count=new_count;
	
//	if(PitchFalling(pitch) || RollFalling(roll))
//	{
//		//��ȡ��ʩ
////		printf("\n����ˤ���ˣ�");
//		mdelay(30);	//��ʱ
//		if(1==FALLING)
//		{
//			FALLING=0;
//			printf("\n���˰�ȫ��");  
//		}
//	}

//	printf("\nmpu 6050 test finish");	
}



#endif

/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   
  * @retval  
  */
void MPU6050_ReadData(u8 reg_add,unsigned char*Read,u8 num)
{
	unsigned char i;
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	
	i2c_Start();
	i2c_SendByte(MPU6050_SLAVE_ADDRESS+1);
	i2c_WaitAck();
	
	for(i=0;i<(num-1);i++){
		*Read=i2c_ReadByte(1);
		Read++;
	}
	*Read=i2c_ReadByte(0);
	i2c_Stop();
}

/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
	if(Re != 0x68)
	{
		printf("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
		printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
}


/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(float *accData)
{
    u8 buf[6];
		short Accel[3];
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    Accel[0] = (buf[0] << 8) | buf[1];
    Accel[1] = (buf[2] << 8) | buf[3];
    Accel[2] = (buf[4] << 8) | buf[5];
		accData[0] = Accel[0]/16384.0;
		accData[1] = Accel[1]/16384.0;
		accData[2] = Accel[2]/16384.0;
}

/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}


/**
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	u8 buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}


/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
  temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}

/**
  * @brief  �ж��Ƿ���Ҫͨ����������Σ����Ϣ
  * @param  ��  
  * @retval ��
  */
void blind_falled()
{
	static float pitch,Level_Accel = 0;  								//���ڴ洢��һ�ε���̬������ٶ�
	float Angle[4],Accel[3];														//���ڴ洢��ǰ����̬������ٶ�
	int i;
	MPU6050ReadAcc(Accel);	                            //��ȡ���ٶ���Ϣ
	getEulerAngles(Angle);															//��ȡ�Ƕ���Ϣ
	
	if(Level_Accel == 0)                                //��һ�α���
	{
		Level_Accel = sqrt ( Accel[0] * Accel[0] + Accel[1] * Accel[1] );
			 pitch    = Angle[0];	
			return;
	}

	if(fabs(Level_Accel-sqrt ( Accel[0] * Accel[0] + Accel[1] * Accel[1] ))>1.0&& fabs(pitch - Angle[0])>1.0)   //�ж��Ƿ����ˤ��
	{
		flag_FALLING = 1;
		printf("ä��aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaˤ��\r\n");		
		for(i=15;i>0;i--)   //��ʱ10��
			{
				mdelay(1000);	
				USART3_Send_String(AutoAlarm,sizeof(AutoAlarm));
				printf("���ţ��Ƿ���Ҫ���������Ϣ,i == %d\r\n",i);
				if(flag_FALLING == 0)
				{
						MPU6050ReadAcc(Accel);	                            //��ȡ���ٶ���Ϣ
						getEulerAngles(Angle);															//��ȡ�Ƕ���Ϣ
						Level_Accel = sqrt ( Accel[0] * Accel[0] + Accel[1] * Accel[1] );
						pitch=Angle[0];
						return ;
				}			
			}	
			SendHelp();
	}
//		printf("fabs(Level_Accel-sqrt ( Accel[0] * Accel[0] + Accel[1] * Accel[1] ))�Ĳ� = %f\r\n",fabs(Level_Accel-sqrt ( Accel[0] * Accel[0] + Accel[1] * Accel[1] )));
//		printf("fabs(pitch - Angle[0])�Ĳ� = %f\r\n",fabs(pitch - Angle[0]));
		Level_Accel = sqrt ( Accel[0] * Accel[0] + Accel[1] * Accel[1] );
		pitch=Angle[0];
}

void SendHelp(void)
{
			if(1==flag_FALLING){		//�����ȫ������û�б����£���ôflag_FALLING��־λ����Ϊ1�����Է���������Ϣ
				
			USART_SendData(USART1, '!');		//����Σ����Ϣ
			while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
				continue;	
			flag_volume=0;
			USART3_Send_String(Alarm,sizeof(Alarm));           //�������������Ϣ
			printf("�������Զ���������\r\n");
			flag_FALLING=0;
		}
}




