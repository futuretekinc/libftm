#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "ftm_mqtt.h"
#include "ftm_mem.h"
#include "ftm_debug.h"

static void *FTM_MQTT_process(void *pData);
static void FTM_MQTT_CB_connect(struct mosquitto *pMOSQ, void *pObj, int nResult);
static void FTM_MQTT_CB_disconnect(struct mosquitto *pMOSQ, void *pObj, int nResult);
static void FTM_MQTT_CB_message(struct mosquitto *pMOSQ, void *pObj, const struct mosquitto_message *pMessage);


FTM_MQTT_PTR FTM_MQTT_create(FTM_MQTT_CONFIG_PTR pConfig)
{
	FTM_MQTT_PTR pMQTT = NULL;
	static int bInit = 0;

	if (bInit == 0)
	{
		mosquitto_lib_init();
	}

	pMQTT = (FTM_MQTT_PTR)FTM_MEM_malloc(sizeof(FTM_MQTT));
	if (pMQTT == NULL)
	{
		return	NULL;	
	}
	
	memset(pMQTT, 0, sizeof(FTM_MQTT));
	memcpy(&pMQTT->xConfig, pConfig, sizeof(FTM_MQTT_CONFIG));

	pMQTT->pMOSQ = mosquitto_new(pMQTT->xConfig.pClientID, true, pMQTT);
	if (pMQTT->pMOSQ == NULL)
	{
		free(pMQTT);
		return NULL;	
	}

	mosquitto_connect_callback_set(pMQTT->pMOSQ, FTM_MQTT_CB_connect);
	mosquitto_disconnect_callback_set(pMQTT->pMOSQ, FTM_MQTT_CB_disconnect);
	mosquitto_message_callback_set(pMQTT->pMOSQ, FTM_MQTT_CB_message);

	return	pMQTT;
}


FTM_RET FTM_MQTT_destroy(FTM_MQTT_PTR pMQTT)
{
	if (pMQTT == NULL)
	{
		return	FTM_RET_ERROR;	
	}
	
	FTM_MQTT_stop(pMQTT);

	FTM_MEM_free(pMQTT);

	return	FTM_RET_OK;
}

FTM_RET	FTM_MQTT_start(FTM_MQTT_PTR pMQTT)
{
	if (pMQTT == NULL)
	{
		return	FTM_RET_ERROR;
	}

	if (pMQTT->hThread != 0)
	{
		return	FTM_RET_ERROR;	
	}

	pMQTT->bRun = 1;

	if (pthread_create(&pMQTT->hThread, NULL, FTM_MQTT_process, pMQTT) != 0)
	{
		pMQTT->bRun = 0;

		return	FTM_RET_ERROR;	
	}

	return	FTM_RET_OK;
}

FTM_RET	FTM_MQTT_stop(FTM_MQTT_PTR pMQTT)
{
	if (pMQTT->hThread == 0)
	{
		return	FTM_RET_ERROR;	
	}

	pMQTT->bRun = 0;
	
	while(pMQTT->hThread != 0)
	{
		sleep(1);	
	}

	return	FTM_RET_OK;
}

FTM_RET	FTM_MQTT_subscribe(FTM_MQTT_PTR pMQTT, const char *pSubscription, int nQoS)
{
	if (mosquitto_subscribe(pMQTT->pMOSQ, NULL, pSubscription, nQoS) != MOSQ_ERR_SUCCESS)
	{
		return	FTM_RET_ERROR;	
	}

	return	FTM_RET_OK;
}

FTM_RET FTM_MQTT_publish(FTM_MQTT_PTR pMQTT, char *pTopic, void *pPayload, int nPayload, int nQoS)
{
	if (mosquitto_publish(pMQTT->pMOSQ, NULL, pTopic, nPayload, pPayload, nQoS,  0) != MOSQ_ERR_SUCCESS)
	{
		return	FTM_RET_ERROR;	
	}

	return	FTM_RET_OK;
}


FTM_RET FTM_MQTT_setConnectCB(FTM_MQTT_PTR pMQTT, FTM_MQTT_CB_CONNECT pCB)
{
	ASSERT(pMQTT != NULL);

	pMQTT->CB_connect = pCB;

	return	FTM_RET_OK;
}

FTM_RET FTM_MQTT_setDisconnectCB(FTM_MQTT_PTR pMQTT, FTM_MQTT_CB_DISCONNECT pCB)
{
	ASSERT(pMQTT != NULL);

	pMQTT->CB_disconnect = pCB;

	return	FTM_RET_OK;
}

FTM_RET FTM_MQTT_setMessageCB(FTM_MQTT_PTR pMQTT, FTM_MQTT_CB_MESSAGE pCB)
{
	ASSERT(pMQTT != NULL);

	pMQTT->CB_message = pCB;

	return	FTM_RET_OK;
}

void *FTM_MQTT_process(void *pData)
{
	int	rc = 0;

	FTM_MQTT_PTR	pMQTT = (FTM_MQTT_PTR)pData;

	rc = mosquitto_connect(	pMQTT->pMOSQ, 
							pMQTT->xConfig.pBrokerIP, 
							pMQTT->xConfig.usPort, 
							pMQTT->xConfig.nKeepAlive);
	
	while(pMQTT->bRun)
	{
		rc = mosquitto_loop(pMQTT->pMOSQ, -1, 1);
		if (pMQTT->bRun && rc)
		{
			sleep(20);	
			rc = mosquitto_reconnect(pMQTT->pMOSQ);
		}
	}


	mosquitto_disconnect(pMQTT->pMOSQ);
	pMQTT->hThread = 0;

	return	NULL;
}

void FTM_MQTT_CB_connect(struct mosquitto *pMOSQ, void *pObj, int nResult)
{
	ASSERT(pObj != NULL);

	if (((FTM_MQTT_PTR)pObj)->CB_connect != NULL)
	{
		((FTM_MQTT_PTR)pObj)->CB_connect(pObj, nResult);
	}
}

void FTM_MQTT_CB_disconnect(struct mosquitto *pMOSQ, void *pObj, int nResult)
{
	ASSERT(pObj != NULL);

	if (((FTM_MQTT_PTR)pObj)->CB_disconnect != NULL)
	{
		((FTM_MQTT_PTR)pObj)->CB_disconnect(pObj, nResult);
	}
}

void FTM_MQTT_CB_message(struct mosquitto *pMOSQ, void *pObj, const struct mosquitto_message *pMessage)
{
	ASSERT(pObj != NULL);

	TRACE("  TOPIC : %s\n", pMessage->topic);
	TRACE("PAYLOAD : %s\n", (char *)pMessage->payload);

	if (((FTM_MQTT_PTR)pObj)->CB_message != NULL)
	{
		((FTM_MQTT_PTR)pObj)->CB_message(pObj, pMessage);
	}

}

