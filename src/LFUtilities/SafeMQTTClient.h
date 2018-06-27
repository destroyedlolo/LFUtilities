/* Safe MQTT client
 *
 * Ensure reconnection with a timeout
 * Store misses in RTC memory
 *
 *	27/06/2018 First version
 */
#ifndef SAFEMQTTCLIENT_H
#define SAFEMQTTCLIENT_H	0.0100

#include <KeepInRTC.h>
#include <PubSubClient.h>

#define SMC_RETRY_DELAY 500	// Delay b/w attempts (ms)

class SafeMQTTClient : public KeepInRTC::KeepMe {
	struct {
		uint32_t misses;	// number of failled connection
	} data_to_keep;

	PubSubClient clientMQTT;

public:
	SafeMQTTClient( 
		KeepInRTC &kir, // RTC context
		Client &client	// Network client
	) : 
		KeepInRTC::KeepMe( kir, (uint32_t *)&this->data_to_keep, sizeof(this->data_to_keep) ),
		clientMQTT( client )
	{
		if( !kir.isValid() ){	// nothing in RTC, set default values
			this->data_to_keep.misses = 0;
			this->save();
		}
	}
};

#endif
