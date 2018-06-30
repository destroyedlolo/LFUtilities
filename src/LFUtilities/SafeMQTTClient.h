/* Safe MQTT client
 *
 * Ensure reconnection with a timeout
 * Store misses in RTC memory
 *
 *	27/06/2018 First version
 *	30/06/2018 Handle all networking aspect
 */
#ifndef SAFEMQTTCLIENT_H
#define SAFEMQTTCLIENT_H	0.0200

#include <PubSubClient.h>

#include <LFUtilities/Duration.h>

#define SMC_RETRY_DELAY 100			// Delay b/w attempts (ms)
#define SMC_MQTT_MAX_RETRY 1500		// how long we will try for MQTT ? (mS)
#define SMC_WIFI_MAX_RETRY 15000	// how long we will try for WiFi ? (mS)

#	define SERIAL_ENABLED


class SafeMQTTClient {
	PubSubClient clientMQTT;
	String clientID;		// MQTT ClientID
	String mqtttopic;		// topic to publish mqtt connection duration to
	String MQTT_Message;	// Information topics
	String MQTT_WiFi;		// Wifi connection duration
	String MQTT_MQTT;		// MQTT connection duration

	String WSSID;
	String WPWD;

public:
	SafeMQTTClient( 
		Client &client,		// Network client
		const char *assid, const char *apwd,	// WiFi
		const char *aclientID,	// MQTT CLientID
		const char *atopic	// root of topics to publish too (if empty, doesn't publish)
	) : 
		clientMQTT( client ),
		clientID( aclientID ),
		WSSID( assid), WPWD(apwd)
	{
		if( atopic ){
			this->mqtttopic = String( atopic );
			this->MQTT_Message = this->mqtttopic + "Message";
			this->MQTT_WiFi = this->mqtttopic + "WiFi";
			this->MQTT_MQTT = this->mqtttopic + "MQTT";
		}
	}

		/* Networks */
	bool connectWiFi(
		Duration &duration,
		uint32_t maxtries=SMC_WIFI_MAX_RETRY
	){
		duration.reInit();
		WiFi.begin( WSSID.c_str(), WPWD.c_str() );
		for(;;){
			if( WiFi.status() == WL_CONNECTED ){
				duration.Finished();
#ifdef SERIAL_ENABLED
				Serial.print("\nok : ");
				Serial.println( *duration );
#endif
				return true;
			}

			if( *duration > maxtries ) // Too long
				return false;

			delay(500);
#ifdef SERIAL_ENABLED
			Serial.print("=");
#endif
		}
	}

#if 0
		/* MQTT */
	bool connectMQTT( 
		const char *client_name, 
		Duration &duration,
		uint32_t maxtries=SMC_MQTT_MAX_RETRY,
		bool clear_session=true
	){
		duration.reInit();
		for(;;){
			if( this->clientMQTT.connect( client_name, clear_session ) ){	// Connected
				duration.Finished();
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

		/* Publish messages */
	void logMsg( const char *msg ){
		this->publish( this->MQTT_Message, msg );
#	ifdef SERIAL_ENABLED
		if(Serial)
			Serial.println( msg );
#endif
	}

	void logMsg( String &msg ){ this->logMsg( msg.c_str() ); }
#endif
};

#endif
