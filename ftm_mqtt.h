#ifndef	__LCGW_MQTT_H__
#define __LCGW_MQTT_H__

#include "ftm.h"
#include <mosquitto.h>
#include <pthread.h>

#define	FTM_MQTT_CLIENT_ID_LEN	24
#define	FTM_MQTT_BROKER_IP_LEN	256

typedef	void (*FTM_MQTT_CB_CONNECT)(void *pMQTT, int nResult);
typedef void (*FTM_MQTT_CB_DISCONNECT)(void *pMQTT, int nResult);
typedef	void (*FTM_MQTT_CB_MESSAGE)(void *pMQTT, const struct mosquitto_message *pMessage);

typedef	struct
{
	char			 	pClientID[FTM_MQTT_CLIENT_ID_LEN];
	char				pBrokerIP[FTM_MQTT_BROKER_IP_LEN];
	unsigned short		usPort;
	int					nKeepAlive;

	int					nMessageID;
}	FTM_MQTT_CONFIG, _PTR_ FTM_MQTT_CONFIG_PTR;

typedef struct
{
	FTM_MQTT_CONFIG			xConfig;

	FTM_MQTT_CB_CONNECT		CB_connect;
	FTM_MQTT_CB_DISCONNECT	CB_disconnect;
	FTM_MQTT_CB_MESSAGE		CB_message;
	int						bRun;
	struct mosquitto*		pMOSQ;
	pthread_t				hThread;
}	FTM_MQTT, _PTR_ FTM_MQTT_PTR;


FTM_MQTT_PTR FTM_MQTT_create(FTM_MQTT_CONFIG_PTR pMQTT);
FTM_RET FTM_MQTT_destroy(FTM_MQTT_PTR pMQTT);
FTM_RET	FTM_MQTT_start(FTM_MQTT_PTR pMQTT);
FTM_RET	FTM_MQTT_stop(FTM_MQTT_PTR pMQTT);

FTM_RET	FTM_MQTT_subscribe(FTM_MQTT_PTR pMQTT, const char *pSubscription, int nQoS);
FTM_RET FTM_MQTT_publish(FTM_MQTT_PTR pMQTT, char *pTopic, void *pPayload, int nPayload, int nQoS);

FTM_RET FTM_MQTT_setConnectCB(FTM_MQTT_PTR pMQTT, FTM_MQTT_CB_CONNECT pCB);
FTM_RET FTM_MQTT_setDisconnectCB(FTM_MQTT_PTR pMQTT, FTM_MQTT_CB_DISCONNECT pCB);
FTM_RET FTM_MQTT_setMessageCB(FTM_MQTT_PTR pMQTT, FTM_MQTT_CB_MESSAGE pCB);

#endif

