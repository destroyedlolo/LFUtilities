/* Safe MQTT client
 *
 * Ensure reconnection with a timeout
 * Store misses in RTC memory
 *
 *	27/06/2018 First version
 */
#ifndef SAFEMQTTCLIENT_H
#define SAFEMQTTCLIENT_H	0.0100

#include <PubSubClient.h>

#include <LFUtilities/Duree.h>

#define SMC_RETRY_DELAY 100	// Delay b/w attempts (ms)

class SafeMQTTClient {
	PubSubClient clientMQTT;

public:
	SafeMQTTClient( 
		Client &client	// Network client
	) : 
		clientMQTT( client )
	{ }

	bool connect( 
		const char *client_name, 
		Duree &duration,
		unsigned long int maxtries=1500,	// how long we will try ? (mS)
		bool clear_session=true
	){
		duration.reInit();
		for(;;){
			if( this->clientMQTT.connect( client_name, clear_session ) ){	// Connected
				duration.Fini();
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
