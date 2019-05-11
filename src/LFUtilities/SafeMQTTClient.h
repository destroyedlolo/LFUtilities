/* Safe MQTT client
 *
 * Ensure reconnection with a timeout
 *
 *	27/06/2018 First version
 *	30/06/2018 Handle all networking aspect
 */
#ifndef SAFEMQTTCLIENT_H
#define SAFEMQTTCLIENT_H	0.0201

#include <PubSubClient.h>

#include <LFUtilities/Duration.h>

#define SMC_RETRY_DELAY 100			// Delay b/w attempts (ms)
#define SMC_MQTT_MAX_RETRY 1500		// how long we will try for MQTT ? (mS)
#define SMC_WIFI_MAX_RETRY 15000	// how long we will try for WiFi ? (mS)

/* #	define SERIAL_ENABLED */


class SafeMQTTClient {
	PubSubClient clientMQTT;
	String clientID;		// MQTT ClientID
	String mqtttopic;		// topic to publish mqtt connection duration to
	String MQTT_Message;	// Information topics
	String MQTT_WiFi;		// Wifi connection duration
	String MQTT_MQTT;		// MQTT connection duration
	uint32_t mqtt_maxtries;	// For how long are we trying to connect to the broker (seconds)
	bool mqtt_clear_session;	// Clear session when connecting to MQTT ?

	String WSSID;
	String WPWD;

public:
	SafeMQTTClient( 
		Client &client,		// Network client
		const char *assid, const char *apwd,	// WiFi
		const char *abURL, int abport,	// MQTT Broker
		const char *aclientID,	// MQTT CLientID
		const char *atopic = NULL,	// root of topics to publish too (if empty, doesn't publish)
		bool clear_session = false
	) : 
		clientMQTT( client ),
		clientID( aclientID ),
		mqtt_maxtries( SMC_MQTT_MAX_RETRY ),
		mqtt_clear_session( clear_session ),
		WSSID( assid), WPWD(apwd)
	{
		if( atopic ){
			this->mqtttopic = String( atopic );
			this->MQTT_Message = this->mqtttopic + "/Message";
			this->MQTT_WiFi = this->mqtttopic + "/WiFi";
			this->MQTT_MQTT = this->mqtttopic + "/MQTT";
		}

		this->clientMQTT.setServer( abURL, abport );
	}


		/*******
		 * Networks 
		 *******/

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
				Serial.print("\nWiFi ok : ");
				Serial.println( *duration );
#endif
				this->publish( this->MQTT_WiFi, *duration, true );
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

		/*********
		 * MQTT
		 *********/
	void setMQTTMaxTries( uint32_t val ){ this->mqtt_maxtries = val; }
	uint32_t getMQTTMaxTries( void ){ return this->mqtt_maxtries; }

	bool connected( void ){ return this->clientMQTT.connected(); }

	bool connectMQTT( 
		Duration &duration
	){
		duration.reInit();
		for(;;){
			if( this->clientMQTT.connect( this->clientID.c_str(), this->mqtt_clear_session ) ){	// Connected
				duration.Finished();
#ifdef SERIAL_ENABLED
				Serial.print("\nMQTT ok : ");
				Serial.println( *duration );
#endif
				if( this->mqtttopic.length() )	// Publish connection duration
					this->clientMQTT.publish( MQTT_MQTT.c_str(), String(*duration, DEC).c_str());
				return true;
			} else {	// Failure
				/*D* Peut-être ajouter un callback pour afficher l'erreur */
				if( *duration > this->mqtt_maxtries ) // Too long
					return false;
				delay( SMC_RETRY_DELAY );
#ifdef SERIAL_ENABLED
			Serial.print(">");
#endif
			}
		}
	}

	PubSubClient &getClient( void ){ return this->clientMQTT; }

		/* Publish messages */
	void publish(
		String &topic, const char *msg,	// What to publish
		bool reconnect=true 			// reconnect if the network is lost
	){
		if( !this->clientMQTT.connected() ){
			if( reconnect ){
				Duration dMQTT;
				if( !this->connectMQTT( dMQTT ) )	// Re-connection failed
					return;
			} else	// Unable to reconnect
				return;
		}

		this->clientMQTT.publish( topic.c_str(), msg );
	}

	void publish( String &topic, const uint32_t val, bool reconnect=true ){
		this->publish( topic, String(val, DEC).c_str(), reconnect );
	}

	void logMsg( const char *msg, bool reconnect=true ){
		this->publish( this->MQTT_Message, msg, reconnect );
#	ifdef SERIAL_ENABLED
		Serial.println( msg );
#	endif
	}

	void logMsg( String &msg ){ this->logMsg( msg.c_str() ); }
};

#endif
