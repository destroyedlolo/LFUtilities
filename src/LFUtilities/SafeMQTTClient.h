/* Safe MQTT client
 *
 * Ensure reconnection with a timeout
 * Store misses in RTC memory
 *
 *	27/06/2018 First version
 */
#ifndef SAFEMQTTCLIENT_H
#define SAFEMQTTCLIENT_H	0.0102

#include <PubSubClient.h>

#include <LFUtilities/Duree.h>

#define SMC_RETRY_DELAY 100		// Delay b/w attempts (ms)
#define SMC_MQTT_MAX_RETRY 1500	// how long we will try ? (mS)


class SafeMQTTClient {
	PubSubClient clientMQTT;
	String mqtttopic;	// topic to publish mqtt connection duration to

public:
	SafeMQTTClient( 
		Client &client,		// Network client
		const char *atopic	// root of topics to publish too (if empty, doesn't publish)
	) : 
		clientMQTT( client )
	{
		if( atopic )
			mqtttopic = String( atopic );
	}

	bool connect( 
		const char *client_name, 
		Duree &duration,
		uint32_t maxtries=SMC_MQTT_MAX_RETRY,
		bool clear_session=true
	){
		duration.reInit();
		for(;;){
			if( this->clientMQTT.connect( client_name, clear_session ) ){	// Connected
				duration.Fini();
				if( mqtttopic.length() )	// Publish connection duration
					this->clientMQTT.publish( mqtttopic.c_str(), String(*duration, DEC).c_str() );
				return true;
			} else {	// Failure
				/*D* Peut-être ajouter un callback pour afficher l'erreur */
				if( *duration > maxtries ) // Too long
					return false;
				delay( SMC_RETRY_DELAY );
			}
		}
	}

	PubSubClient &getClient( void ){ return this->clientMQTT; }
};

#endif
