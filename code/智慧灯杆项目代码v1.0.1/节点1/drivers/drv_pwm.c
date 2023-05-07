#include "drv_pwm.h"

void pwm1_siet_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_TIMER2);
	rcu_periph_clock_enable(RCU_AF);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
	timer_deinit(TIMER_2);

	
	timer_parameter_struct timer_init_struct;
	timer_oc_parameter_struct timer_oc_init_struct;
	
	timer_init_struct.prescaler = 107;
	timer_init_struct.alignedmode = TIMER_COUNTER_EDGE;
	timer_init_struct.counterdirection = TIMER_COUNTER_UP;
	timer_init_struct.clockdivision = TIMER_CKDIV_DIV1;
	timer_init_struct.period = 500;
	timer_init_struct.repetitioncounter = 0;
	
	
	timer_init(TIMER_2,&timer_init_struct);

	timer_oc_init_struct.outputstate  = TIMER_CCX_ENABLE;						/* ͨ��ʹ�� */
  timer_oc_init_struct.outputnstate = TIMER_CCXN_DISABLE;					/* ͨ���������ʹ�ܣ���ʱ��2��Ч�� */
  timer_oc_init_struct.ocpolarity   = TIMER_OC_POLARITY_HIGH;			/* ͨ������ */
  timer_oc_init_struct.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;		/* ����ͨ�����ԣ���ʱ��2��Ч��*/
  timer_oc_init_struct.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;		/* ͨ������״̬�������ʱ��2��Ч��*/
  timer_oc_init_struct.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;		/*����ͨ������״̬�������ʱ��2��Ч�� */

	timer_channel_output_config(TIMER_2, TIMER_CH_1,&timer_oc_init_struct);
   /* ͨ��2ռ�ձ����� */
  timer_channel_output_pulse_value_config(TIMER_2, TIMER_CH_1, 0);
    /* PWMģʽ0 */
  timer_channel_output_mode_config(TIMER_2,TIMER_CH_1,TIMER_OC_MODE_PWM0);
    /* ��ʹ������Ƚ�Ӱ�ӼĴ��� */
  timer_channel_output_shadow_config(TIMER_2,TIMER_CH_1,TIMER_OC_SHADOW_DISABLE);
		
	timer_auto_reload_shadow_enable(TIMER_2);
    /* auto-reload preload enable */
  timer_enable(TIMER_2);
}


uint16_t PWM1_CH1_Crol(uint16_t out_value)
{
	timer_channel_output_pulse_value_config(TIMER_2, TIMER_CH_1, out_value);
	
	return out_value;

}

