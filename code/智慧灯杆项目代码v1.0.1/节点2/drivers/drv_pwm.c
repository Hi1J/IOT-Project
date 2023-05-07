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

	timer_oc_init_struct.outputstate  = TIMER_CCX_ENABLE;						/* 通道使能 */
  timer_oc_init_struct.outputnstate = TIMER_CCXN_DISABLE;					/* 通道互补输出使能（定时器2无效） */
  timer_oc_init_struct.ocpolarity   = TIMER_OC_POLARITY_HIGH;			/* 通道极性 */
  timer_oc_init_struct.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;		/* 互补通道极性（定时器2无效）*/
  timer_oc_init_struct.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;		/* 通道空闲状态输出（定时器2无效）*/
  timer_oc_init_struct.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;		/*互补通道空闲状态输出（定时器2无效） */

	timer_channel_output_config(TIMER_2, TIMER_CH_1,&timer_oc_init_struct);
   /* 通道2占空比设置 */
  timer_channel_output_pulse_value_config(TIMER_2, TIMER_CH_1, 0);
    /* PWM模式0 */
  timer_channel_output_mode_config(TIMER_2,TIMER_CH_1,TIMER_OC_MODE_PWM0);
    /* 不使用输出比较影子寄存器 */
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

