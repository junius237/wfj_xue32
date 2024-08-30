#include "esp8266.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "core_json.h"

/*
	productKey:k16yvRAEk49
	deviceName:PWMf103
	deviceSecret:a07404666d4c1cf613f3e6b865696654
	
	mqttClientId: k16yvRAEk49|securemode=2,signmethod=hmacsha1,timestamp=1717427796022|
	username: PWMf103&k16yvRAEk49
	password: ADCE68826BF5CBF61E4FD53608117BF77C443E8E

*/


/*
*************************************
宏定义
*************************************
*/

#define WIFI_SSID        "realme_GT"		//wifi账号
#define WIFI_PASSWD      "159357456"  		//wifi密码
                         
#define MQTT_CLIENT_ID   "k16yvRAEk49|securemode=2\\,signmethod=hmacsha1\\,timestamp=1717427796022|" 
                        
#define MQTT_USER_NAME   "PWMf103&k16yvRAEk49"
                         
#define MQTT_PASSWD      "ADCE68826BF5CBF61E4FD53608117BF77C443E8E"
#define BROKER_ASDDRESS  "k16yvRAEk49.iot-as-mqtt.cn-shanghai.aliyuncs.com"
#define SUB_TOPIC        "/sys/k16yvRAEk49/PWMf103/thing/service/property/set"
#define PUB_TOPIC        "/sys/k16yvRAEk49/PWMf103/thing/event/property/post"
#define JSON_FORMAT      "{\\\"params\\\":{\\\"LED\\\":%d\\}\\,\\\"version\\\":\\\"1.0.0\\\"}"


/*
*************************************
变量声明
*************************************
*/
uint8_t LED_status;
uint8_t Wave_status;
uint16_t Freq_status;
uint16_t AMP1_status;
uint16_t AMP2_status;
uint16_t AMP3_status;
/*
*************************************
变量定义
*************************************
*/

unsigned char receive_buf[512];	 	//串口3接收缓存数组
unsigned char receive_start = 0;	//串口3接收开始标志位
uint16_t receive_count = 0;	      	//串口3接收数据计数器
uint16_t receive_finish = 0;	    //串口3接收结束标志位 
/*
**************************************
函数定义
**************************************
*/
/**
  * @brief         解析json数据
  * @param[in]     json_msg:json数据,json_len:json数据长度
  * @retval        返回0找到指定json数据，否则返回11
  */
//uint8_t parse_json_msg(uint8_t *json_msg,uint8_t json_len)
//{
//  uint8_t retval =0;
//  
//  JSONStatus_t result;
//	char query[] = "params.LED";
//  size_t queryLength = sizeof( query ) - 1;
//  char * value;
//  size_t valueLength;
//  result = JSON_Validate((const char *)json_msg, json_len);
//  if( result == JSONSuccess)
//  {
//    result = JSON_Search((char *)json_msg, json_len, query, queryLength,&value, &valueLength );
//    if( result == JSONSuccess)
//    {
//      char save = value[valueLength];
//      value[valueLength] = '\0';
//      printf("Found: %s %d-> %s\n", query, valueLength,value);
//      value[valueLength] = save;
//	  LED_status = atoi(value);
//      retval = 0;
//    }
//    else
//    {
//      retval = 1;
//    }
//  }
//  else
//  {
//    retval = 1;
//  }  
//  return retval;
//}

uint8_t parse_json_msg(uint8_t *json_msg, uint8_t json_len)
{
    uint8_t retval = 0;
    
    JSONStatus_t result;

    // 定义需要解析的查询字符串及其对应的目标变量
    typedef struct {
        char *query;
        size_t queryLength;
        uint16_t *targetVariable;
    } JsonQueryInfo_t;

    JsonQueryInfo_t queries[] = {
        { "params.Device_Switch", sizeof("params.Device_Switch") - 1, &LED_status },
        { "params.wave_form", sizeof("params.wave_form") - 1, &Wave_status },
        { "params.Frequency", sizeof("params.Frequency") - 1, &Freq_status },
		{ "params.Channel_Amplitude_1", sizeof("params.Channel_Amplitude_1") - 1, &AMP1_status },
		{ "params.Channel_Amplitude_2", sizeof("params.Channel_Amplitude_2") - 1, &AMP2_status },
         
    };

    result = JSON_Validate((const char *)json_msg, json_len);
    if (result == JSONSuccess)
    {
        // 遍历需要解析的查询字符串
        for (size_t i = 0; i < sizeof(queries) / sizeof(queries[0]); i++)
        {
            char *value;
            size_t valueLength;

            result = JSON_Search((char *)json_msg, json_len, queries[i].query, queries[i].queryLength, &value, &valueLength);
            if (result == JSONSuccess)
            {
                char save = value[valueLength];
                value[valueLength] = '\0';
                printf("Found: %s %d-> %s\n", queries[i].query, valueLength, value);
                value[valueLength] = save;
                *(queries[i].targetVariable) = atoi(value);
            }
            else
            {
                retval |= (1 << i);
            }
        }
    }
    else
    {
        retval |= 0x04;
    }

    return retval;
}


/**
  * @brief          串口3数据接收处理函数
  * @param[in]      none
  * @retval         none
  */
void uart3_receiver_handle(void)
{
  unsigned char receive_data = 0;   
  if(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_RXNE) != RESET)  //它检查是否设置了 UART3 接收标志 （），指示接收缓冲区中有新数据可用。UART_FLAG_RXNE: Receive data register not empty : Receive data register not empty flag
  {
    HAL_UART_Receive(&huart3, &receive_data, 1, 1000);//串口3接收1位数据(阻塞接收)
    receive_buf[receive_count++] = receive_data;      
    receive_start = 1;	                              //串口3接收数据开始标志位置1
    receive_finish = 0;	                              //串口3接收数据完成标志位清0
  }
}

/**
  * @brief          串口3数据接收清0函数
  * @param[in]      len:清空的数据长度
  * @retval         none
  */
void uart3_receiver_clear(uint16_t len)	
{
	memset(receive_buf,0x00,len); 	//memset 函数用于将内存块设置为指定值。void *memset(void *str, int c, size_t n); 
									//str ：这是指向要填充的内存块的指针。													receive_buf 是指向表示 UART3 接收缓冲区的内存块的指针。
									//c ：这是要设置的值。该值作为 int 传递，但该函数使用此值的字节表示形式填充内存块。		0x00 是将用于填充内存块的值。
									//n ：这是要设置为该值的字节数															len 是要设置为 的 0x00 值的字节数。
	receive_count = 0;
	receive_start = 0;
	receive_finish = 0;
}

/**
  * @brief          esp8266发送命令函数
  * @param[in]      cmd:发送的命令,len:命令的长度,rec_data:期望接收数据
  * @retval         none
  */
uint8_t esp8266_send_cmd(unsigned char *cmd,unsigned char len,char *rec_data)	
{
  unsigned char retval =0;
  unsigned int count = 0;

  HAL_UART_Transmit(&huart3, cmd, len, 1000);	                                   
  while((receive_start == 0)&&(count<1000))
  {
    count++;
    HAL_Delay(1);
  }

  if(count >= 1000)	
  {
    retval = 1;	
  }
  else	
  {
    do
    {
      receive_finish++;
      HAL_Delay(1);
    }
    while(receive_finish < 500);
    retval = 2;
    if(strstr((const char*)receive_buf, rec_data))	
    {
      retval = 0;	
    }
  }
  uart3_receiver_clear(receive_count);
  return retval;
}


/**
  * @brief          esp8266配置wifi网络
  * @param[in]      none
  * @retval         网络配置成功返回0,否则返回1
  */
uint8_t esp8266_config_network(void)
{
	uint8_t retval =0;
	uint16_t count = 0;
	
	HAL_UART_Transmit(&huart3, (unsigned char *)"AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASSWD"\"\r\n",strlen("AT+CWJAP=\""WIFI_SSID"\",\""WIFI_PASSWD"\"\r\n"), 1000);
	
	while((receive_start == 0)&&(count<1000))
	{
		count++;
		HAL_Delay(1);
	}
	
	if(count >= 1000)	
	{
		retval = 1;	
	}
	else
	{
		HAL_Delay(8000);
		if(strstr((const char*)receive_buf, "OK"))	
		{
			retval = 0;	
		}
    else
    {
      retval = 1;
    }
	}
  uart3_receiver_clear(receive_count);
	return retval;
}
/**
  * @brief          esp8266连接服务
  * @param[in]      none
  * @retval         连接成功返回0,否则返回1
  */
uint8_t esp8266_connect_server(void)
{
	uint8_t retval=0;
	uint16_t count = 0;

	HAL_UART_Transmit(&huart3, (unsigned char *)"AT+MQTTCONN=0,\""BROKER_ASDDRESS"\",1883,0\r\n",strlen("AT+MQTTCONN=0,\""BROKER_ASDDRESS"\",1883,0\r\n"), 1000);	
	while((receive_start == 0)&&(count<1000))	
	{
		count++;
		HAL_Delay(1);
	}
	
	if(count >= 1000)	
	{
		retval = 1;	
	}
	else	
	{
		HAL_Delay(5000);
		if(strstr((const char*)receive_buf, "OK"))	
		{
			retval = 0;	
		}
    else
    {
      retval = 1;	
    }
	}
  uart3_receiver_clear(receive_count);	
	return retval;
}
/**
  * @brief           esp8266复位
  * @param[in]       none
  * @retval          返回0复位成功,返回1复位失败
  */
uint8_t esp8266_reset(void)
{
	uint8_t retval =0;
	uint16_t count = 0;
	
	HAL_UART_Transmit(&huart3, (unsigned char *)"AT+RST\r\n",8, 1000);
	while((receive_start == 0)&&(count<2000))	
	{
		count++;
		HAL_Delay(1);
	}
	if(count >= 2000)	
	{
		retval = 1;	
	}
	else	
	{
		HAL_Delay(5000);
		if(strstr((const char*)receive_buf, "OK"))	
		{
			retval = 0;	
		}
    else
    {
      retval = 1;	
    }
	}
  uart3_receiver_clear(receive_count);	
	return retval;
}
/**
  * @brief          esp8266发送数据
  * @param[in]      none
  * @retval         返回0发送数据成功,返回1发送数据失败
  */
uint8_t esp8266_send_msg(void)	
{
	uint8_t retval =0;	
  uint16_t count = 0;			
	static uint8_t error_count=0;
	unsigned char msg_buf[256];
  
	sprintf((char *)msg_buf,"AT+MQTTPUB=0,\""PUB_TOPIC"\",\""JSON_FORMAT"\",0,0\r\n",LED_status/*,temp_value,humi_value*/);
	HAL_UART_Transmit(&huart3, (unsigned char *)msg_buf,strlen((const char *)msg_buf), 1000);	
	HAL_UART_Transmit(&huart1, (unsigned char *)msg_buf,strlen((const char *)msg_buf), 1000);	
	while((receive_start == 0)&&(count<500))	
	{
		count++;
		HAL_Delay(1);
	}
	if(count >= 500)	
	{
		retval = 1;	
	}
	else	
	{
		HAL_Delay(50);
		if(strstr((const char*)receive_buf, "OK"))	
		{
			retval = 0;	
			error_count=0;
		}
		else 
		{
			error_count++;
			if(error_count==5)
			{
				error_count=0;
        printf("RECONNECT MQTT BROKER!!!\r\n");
				esp8266_init();
			}
		}
	}
  uart3_receiver_clear(receive_count);	
	return retval;
}
/**
  * @brief         esp8266接收数据
  * @param[in]     none
  * @retval        返回0接收数据正常,返回1接收数据异常或无数据
  */
uint8_t esp8266_receive_msg(void)	
{
	uint8_t retval =0;
	int msg_len=0;
	uint8_t msg_body[128] = {0};
  
	if(receive_start == 1)	
	{
		do
		{
			receive_finish++;
			HAL_Delay(1);	
		}
		while(receive_finish < 5);	//有效等待5ms
		
		/*
			strstr()	用于查找给定字符串中子字符串的第一个匹配项。The syntax for strstr() is:
			char *strstr(const char *haystack, const char *needle); 
			***	  haystack	是要搜索的	  字符串
			***	  needle	是要搜索的	子字符串
		*/
		
		/*
			sscanf()	用于从字符串中读取格式化的输入。The syntax for sscanf() is:
			int sscanf(const char *str, const char *format, ...);
			***   str 		要读取格式化输入的输入字符串。 		const char *  必须强制类型转换
			***   format 	指定预期输入格式的格式字符串。		format 中可以包含	转换说明符,用于告诉 sscanf() 如何解释输入。
																					%d: 读取整数值。
																					%f: 读取浮点数值。
																					%s: 读取字符串(直到遇到空白字符)。
																					%c：读取单个字符。
			***   ... 		存储读取值的变量地址。。
		*/
		
		
		//延迟后，代码检查接收到的缓冲区 （ receive_buf ） 是否包含字符串 "+MQTTSUBRECV:"
		if(strstr((const char*)receive_buf,"+MQTTSUBRECV:"))
		{
			//（ const char* 指向常量字符的指针）
			sscanf((const char *)receive_buf,"+MQTTSUBRECV:0,\""SUB_TOPIC"\",%d,%s",&msg_len,msg_body); 
			printf("len:%d,msg:%s\r\n",msg_len,msg_body);
			if(strlen((const char*)msg_body)== msg_len)
			{
				retval = parse_json_msg(msg_body,msg_len);
			}
			else
			{
				retval = 1;
			}
		}
		else 
		{
		retval = 1;
		}
	}
	else
	{
		retval = 1;
	}
	uart3_receiver_clear(receive_count);	
	return retval;
}

/**
  * @brief           esp8266初始化
  * @param[in]       none
  * @retval          none
  */
void esp8266_init(void)
{
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);           											
	
	printf("1.AT+CWMODE=1 SETTING Station MODE\r\n");
	while(esp8266_send_cmd((uint8_t *)"AT+CWMODE=1\r\n",strlen("AT+CWMODE=1\r\n"),"OK")!=0)
	{
		HAL_Delay(1000);
	}
	printf("2.ATE0 关闭回显\r\n");
	while(esp8266_send_cmd((uint8_t *)"ATE0\r\n",strlen("ATE0\r\n"),"OK")!=0)  
	{
		HAL_Delay(1000);
	}
	printf("3.AT+CWAUTOCONN=0 不让自动连接WiFi\r\n");    
	while(esp8266_send_cmd((uint8_t *)"AT+CWAUTOCONN=0\r\n",strlen("AT+CWAUTOCONN=0\r\n"),"OK")!=0)  	
	{
		HAL_Delay(1000);
	}
	printf("4.RESET ESP8266\r\n");
	while(esp8266_reset() != 0)
	{
		HAL_Delay(5000);
	}
	printf("5.CONFIG WIFI NETWORK\r\n");
	while(esp8266_config_network() != 0)
	{
		HAL_Delay(8000);
	}
	printf("6.MQTT USER CONFIG\r\n");
	while(esp8266_send_cmd((uint8_t *)"AT+MQTTUSERCFG=0,1,\""MQTT_CLIENT_ID"\",\""MQTT_USER_NAME"\",\""MQTT_PASSWD"\",0,0,\"\"\r\n",
                          strlen("AT+MQTTUSERCFG=0,1,\""MQTT_CLIENT_ID"\",\""MQTT_USER_NAME"\",\""MQTT_PASSWD"\",0,0,\"\"\r\n"),"OK")!=0)
	{
		HAL_Delay(2000);
	}
	printf("7.CONNECT MQTT BROKER\r\n");
	while(esp8266_connect_server() != 0)
	{
		HAL_Delay(8000);
	}
	printf("8.SUBSCRIBE TOPIC\r\n");
	while(esp8266_send_cmd((uint8_t *)"AT+MQTTSUB=0,\""SUB_TOPIC"\",0\r\n",strlen("AT+MQTTSUB=0,\""SUB_TOPIC"\",0\r\n"),"OK")!=0)
	{
		HAL_Delay(2000);
	}
	printf("9.ESP8266 INIT OK!!!\r\n");
}









