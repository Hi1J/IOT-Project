#include "mqtt.h"

char ClientID[128]; //存放客户端 ID 的缓冲区
int ClientID_len; //存放客户端 ID 的长度
char Username[128]; //存放用户名的缓冲区
int Username_len; //存放用户名的长度
char Passward[128]; //存放密码的缓冲区
int Passward_len; //存放密码的长度
char ServerIP[128]; //存放服务器 IP 或是域名
int ServerPort; //存放服务器的端口号
int Fixed_len; //固定报头长度
int Variable_len; //可变报头长度
int Payload_len; //有效负荷长度
char Ping_flag; //ping 报文状态 0：正常状态，等待计时时间到，发送Ping 报文

 //ping 报文状态 1：Ping 报文已发送，当收到 服务器回复报文的后 将 1 置为 0
char Connect_flag; //同服务器连接状态 0：还没有连接服务器 1：连接上服务器了
char ConnectPack_flag; //CONNECT 报文状态 1：CONNECT 报文成功
char SubcribePack_flag; //订阅报文状态 1：订阅报文成功

uint8_t data_buf[256]={0};
/*----------------------------------------------------------*/
/*函数名：阿里云初始化参数，得到客户端 ID，用户名和密码 */
/*参 数：无 */
/*返回值：无 */
/*----------------------------------------------------------*/
void AliIoT_Parameter_Init(void)
{
	char temp[128]; //计算加密的时候，临时使用的缓冲区
	memset(ClientID,0,128); //客户端 ID 的缓冲区全部清零
	sprintf(ClientID,"%s",CLIENTID); 
	//构建客户端 ID，并存入缓冲区
	ClientID_len = strlen(ClientID); //计算客户端 ID 的长度
	memset(Username,0,128); //用户名的缓冲区全部清零
	sprintf(Username,"%s",USERNAME); 
	//构建用户名，并存入缓冲区
	Username_len = strlen(Username); //计算用户名的长度
	memset(Passward,0,strlen(Passward));
	//填充密码
//	sprintf(Passward,"%s",PASSWARD);
	sprintf(temp,"clientId%sdeviceName%sproductKey%s",DEVICENAME,DEVICENAME,PRODUCTKEY); 
	utils_hmac_sha1(temp,strlen(temp),Passward,DEVICESECRE,DEVICESECRE_LEN);  
	Passward_len = strlen(Passward); //计算用户名的长度
	memset(ServerIP,0,128); 
	sprintf(ServerIP,"%s.iot-as-mqtt.cn-shanghai.aliyuncs.com",PRODUCTKEY);  //构建服务器域名
	ServerPort = PORT; //服务器端口号 1883
	
	printf("服 务 器：%s:%d\r\n",ServerIP,ServerPort); //串口输出调试信息
	printf("客户端 ID：%s\r\n",ClientID); //串口输出调试信息
	printf("用 户 名：%s\r\n",Username); //串口输出调试信息
	printf("密 码：%s\r\n",Passward); //串口输出调试信息
	ConnectServer("TCP",ServerIP,ServerPort);
	// ConnectServer("TCP","a12aIr3m6Z3.iot-as-mqtt.cn?shanghai.aliyuncs.com",1883);
}
/*----------------------------------------------------------*/
/*函数名：连接服务器报文 */
/*参 数：无 */
/*返回值：无 */
/*----------------------------------------------------------*/
void MQTT_ConectPack(void)
{
	Fixed_len = 2; //连接报文中，固定报头长度=2
	Variable_len = 10; //连接报文中，可变报头长度=10
	Payload_len = 2 + ClientID_len + 2 + Username_len + 2 + Passward_len; //总报文长度 
	memset(WiFi_Tx_Buff,0,512);
	WiFi_Tx_Buff[0]=0x10; //第 1 个字节 ：固定 0x01
	WiFi_Tx_Buff[1]=Variable_len + Payload_len; //第 2 个字节 ：可变报头+有效负荷的长度
	WiFi_Tx_Buff[2]=0x00; //第 3 个字节 ：固定 0x00 
	WiFi_Tx_Buff[3]=0x04; //第 4 个字节 ：固定 0x04
	WiFi_Tx_Buff[4]=0x4D; //第 5 个字节 ：固定0x4D
	WiFi_Tx_Buff[5]=0x51; //第 6 个字节 ：固定0x51
	WiFi_Tx_Buff[6]=0x54; //第 7 个字节 ：固定0x54
	WiFi_Tx_Buff[7]=0x54; //第 8 个字节 ：固定0x54
	WiFi_Tx_Buff[8]=0x04; //第 9 个字节 ：固定0x04
	WiFi_Tx_Buff[9]=0xC2; //第 10 个字节：使能用户名和密码校验，不使用遗嘱，不保留会话
	WiFi_Tx_Buff[10]=0x00; //第 11 个字节：保活时间高字节 0x00
	WiFi_Tx_Buff[11]=0x64; //第 12 个字节：保活时间高字节 0x64 100s
	/* CLIENT_ID */
	WiFi_Tx_Buff[12] = ClientID_len/256; //客户端 ID 长度高字节
	WiFi_Tx_Buff[13] = ClientID_len%256; //客户端 ID 长度低字节
	memcpy(&WiFi_Tx_Buff[14],ClientID,ClientID_len); //复制过来客户端 ID 字串
	/* 用户名 */
	WiFi_Tx_Buff[14+ClientID_len] = Username_len/256;  //用户名长度高字节
	WiFi_Tx_Buff[15+ClientID_len] = Username_len%256; //用户名长度低字节
	memcpy(&WiFi_Tx_Buff[16+ClientID_len],Username,Username_len); //复制过来用户名字串
	/* 密码 */
	WiFi_Tx_Buff[16+ClientID_len+Username_len] = Passward_len/256;
	 //密码长度高字节
	WiFi_Tx_Buff[17+ClientID_len+Username_len] = Passward_len%256;
	 //密码长度低字节
	memcpy(&WiFi_Tx_Buff[18+ClientID_len+Username_len],Passward,Passward_len); //复制过来密码字串
	WIFI_Send_Buf(WiFi_Tx_Buff, Fixed_len + Variable_len + Payload_len); //加入发送数据缓冲区
}
/*----------------------------------------------------------*/
/*函数名：SUBSCRIBE 订阅 topic 报文 */
/*参 数：QoS：订阅等级 */
/*参 数：topic_name：订阅 topic 报文名称 */
/*返回值：无 */
/*----------------------------------------------------------*/
void MQTT_Subscribe(char *topic_name, int QoS)
{
	Fixed_len = 2; //SUBSCRIBE 报文中，固定报头长度=2
	Variable_len = 2; //SUBSCRIBE 报文中，可变报头长度=2
	Payload_len = 2 + strlen(topic_name) + 1; //计算有效负荷长度 = 2 字节(topic_name 长度)+ topic_name 字符串的长度 + 1 字节服务等级
	memset(WiFi_Tx_Buff,0,512);
	WiFi_Tx_Buff[0]=0x82; //第 1 个字节 ：固定0x82 
	WiFi_Tx_Buff[1]=Variable_len + Payload_len; //第 2 个字节 ：可变报头+有效负荷的长度
	WiFi_Tx_Buff[2]=0x00; //第 3 个字节 ：报文标识符高字节，固定使用 0x00
	WiFi_Tx_Buff[3]=0x01; //第 4 个字节 ：报文标识符低字节，固定使用 0x01
	WiFi_Tx_Buff[4]=strlen(topic_name)/256; //第 5 个字节 ：topic_name 长度高字节
	WiFi_Tx_Buff[5]=strlen(topic_name)%256; //第 6 个字节 ：topic_name 长度低字节
	memcpy(&WiFi_Tx_Buff[6],topic_name,strlen(topic_name)); //第 7 个字节开始 ：复制过来 topic_name 字串
	WiFi_Tx_Buff[6+strlen(topic_name)]=QoS; //最后 1 个字节：订阅等级
	WIFI_Send_Buf(WiFi_Tx_Buff,Fixed_len + Variable_len + Payload_len);
}

void MQTT_UNSUBSCRIBE(char *topic_name)
{
	Fixed_len = 2; //SUBSCRIBE 报文中，固定报头长度=2
	Variable_len = 2; //SUBSCRIBE 报文中，可变报头长度=2
	Payload_len = 2 + strlen(topic_name) ; //计算有效负荷长度 = 2 字节(topic_name 长度)+ topic_name 字符串的长度
	WiFi_Tx_Buff[0]=0xA2; //第 1 个字节 ：固定0x82 
	WiFi_Tx_Buff[1]=Variable_len + Payload_len; //第 2 个字节 ：可变报头+有效负荷的长度
	WiFi_Tx_Buff[2]=0x00; //第 3 个字节 ：报文标识符高字节，固定使用 0x00
	WiFi_Tx_Buff[3]=0x02; //第 4 个字节 ：报文标识符低字节，固定使用 0x01
	WiFi_Tx_Buff[4]=strlen(topic_name)/256; //第 5 个字节 ：topic_name 长度高字节
	WiFi_Tx_Buff[5]=strlen(topic_name)%256; //第 6 个字节 ：topic_name 长度低字节
	memcpy(&WiFi_Tx_Buff[6],topic_name,strlen(topic_name)); //第 7 个字节开始 ：复制过来 topic_name 字串
	WIFI_Send_Buf(WiFi_Tx_Buff,Fixed_len + Variable_len + Payload_len);

}
/*----------------------------------------------------------*/
/*函数名：PING 报文，心跳包 */
/*参 数：无 */
/*返回值：无 */
/*----------------------------------------------------------*/
void MQTT_PingREQ(void)
{
	memset(WIFI_Send_Buf,0,512);
	WiFi_Tx_Buff[0]=0xC0; //第 1 个字节 ：固定 0xC0 
	WiFi_Tx_Buff[1]=0x00; //第 2 个字节 ：固定 0x00 
	WIFI_Send_Buf(WiFi_Tx_Buff, 2); //加入数据到缓冲区
}
/*----------------------------------------------------------*/
/*函数名：等级 0 发布消息报文 */
/*参 数：topic_name：topic 名称 */
/*参 数：data：数据 */
/*参 数：data_len：数据长度 */
/*返回值：无 */
/*----------------------------------------------------------*/
void MQTT_PublishQs0(char *topic, char *data, int data_len)
{
	Fixed_len = 2; //固定报头长度：2 字节
	Variable_len = 2 + strlen(topic); //可变报头长度：2 字节(topic 长度)+ topic 字符串的长度
	Payload_len = data_len; //有效负荷长度：就是 data_len
	memset(WiFi_Tx_Buff,0,512);
	WiFi_Tx_Buff[0]=0x30; //第 1 个字节 ：固定 0xC0 
	WiFi_Tx_Buff[1]=Variable_len + Payload_len; //第 2 个字节 ：可变报头+有效负荷的长度
//	printf("DATA_LEN==%d\r\n",Variable_len + Payload_len);
	if(Variable_len + Payload_len>=128)
	{

		Fixed_len = 3;
		WiFi_Tx_Buff[2]=0x01;
		WiFi_Tx_Buff[3]=strlen(topic)/256; //第 3 个字节 ：topic 长度高字节
		WiFi_Tx_Buff[4]=strlen(topic)%256; //第 4 个字节 ：topic 长度低字节
		memcpy(&WiFi_Tx_Buff[5],topic,strlen(topic)); //第 5 个字节开始 ：拷贝 topic 字符串
		memcpy(&WiFi_Tx_Buff[5+strlen(topic)],data,data_len); //拷贝data 数据
	}else
	{
		WiFi_Tx_Buff[2]=strlen(topic)/256; //第 3 个字节 ：topic 长度高字节
		WiFi_Tx_Buff[3]=strlen(topic)%256; //第 4 个字节 ：topic 长度低字节
		memcpy(&WiFi_Tx_Buff[4],topic,strlen(topic)); //第 5 个字节开始 ：拷贝 topic 字符串
		memcpy(&WiFi_Tx_Buff[4+strlen(topic)],data,data_len); //拷贝data 数据
	}
	WIFI_Send_Buf(WiFi_Tx_Buff, Fixed_len + Variable_len + Payload_len); //加入发送数据缓冲区
}




