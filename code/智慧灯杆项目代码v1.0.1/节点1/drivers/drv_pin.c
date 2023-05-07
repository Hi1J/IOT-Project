#include "drv_pin.h"

/* ���ų�ʼ��
�˺�������Ӱ��ʱ�ӵ�������,���벻�ñ���ע�͵�rcu_periph_clock_enable(PIN_RCU(pin));
��RCU_REG_VAL(PIN_RCU(pin)) |= BIT(RCU_BIT_POS(PIN_RCU(pin)));��ע�ͼ���.
*/
rt_err_t rt_pin_init(rt_base_t pin, rt_base_t mode)
{
	rcu_periph_clock_enable(PIN_RCU(pin));
//	RCU_REG_VAL(PIN_RCU(pin)) |= BIT(RCU_BIT_POS(PIN_RCU(pin)));
	rt_pin_mode(pin, mode);
	gpio_bit_reset(GPIOC, GPIO_PIN_13);
	return RT_EOK;
}

/* ��������ģʽ */
void rt_pin_mode(rt_base_t pin, rt_base_t mode)
{
	rt_base_t mode_tmp = RT_NULL;
	switch(mode){
		case PIN_MODE_OUTPUT:
			mode_tmp = GPIO_MODE_OUT_PP;
			break;
		case PIN_MODE_INPUT:
			mode_tmp = GPIO_MODE_IN_FLOATING;
			break;
		case PIN_MODE_INPUT_PULLUP:
			mode_tmp = GPIO_MODE_IPU;
			break;
		case PIN_MODE_INPUT_PULLDOWN:
			mode_tmp = GPIO_MODE_IPD;
			break;
		case PIN_MODE_OUTPUT_OD:
			mode_tmp = GPIO_MODE_OUT_OD;
			break;
		default :
			RT_ASSERT(PIN_OPS_ERR);
			break;
	}
	
	gpio_init(PIN_GDPORT(pin), mode_tmp, GPIO_OSPEED_50MHZ, PIN_GDPIN(pin));
}

/* �������ŵ�ƽ */
void rt_pin_write(rt_base_t pin, rt_base_t value)	
{
	
	switch(value){
		case PIN_LOW:
			gpio_bit_reset(PIN_GDPORT(pin), PIN_GDPIN(pin));
			break;
		case PIN_HIGH:
			gpio_bit_set(PIN_GDPORT(pin), PIN_GDPIN(pin));
			break;
		default :
			RT_ASSERT(PIN_OPS_ERR);
			break;
	}
	
}

/* ��ȡ���ŵ�ƽ */
int rt_pin_read(rt_base_t pin)
{
	return gpio_input_bit_get(PIN_GDPORT(pin), PIN_GDPIN(pin));
}

/* �������жϻص����� */
rt_err_t rt_pin_attach_irq(rt_int32_t pin, rt_uint32_t mode,
                             void (*hdr)(void *args), void  *args)
{

	return -RT_ENOSYS;
}

/* ���������жϻص����� */
rt_err_t rt_pin_detach_irq(rt_int32_t pin)
{

	return -RT_ENOSYS;
}

/* ʹ�������ж� */
rt_err_t rt_pin_irq_enable(rt_base_t pin, rt_uint32_t enabled)
{

	return -RT_ENOSYS;
}

/* ��ȡ���ű�� */
/* Get pin number by name,such as PA.0,P0.12 */
rt_base_t rt_pin_get(const char *name)
{

    return 1;
}




