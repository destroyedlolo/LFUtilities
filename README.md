# LFUtilities

This library contains various stuffs/helpers to Arduino like developments.

-------

## Duration

    #include <LFUtilities/Duration.h>
Abstracts duration measurement.

 - `Duration( void )` or `void reInit( void )` - Starts a duration measurement.
 - `unsigned long int Finished( void )` - End of measurement, return the duration in milliseconds.
 - `unsigned long int operator &ast;( void )` - return the duration in milliseconds, up to the `Fini()` call or till now.

-------

## SafeMQTTClient

    #include <LFUtilities/SafeMQTTClient.h>
MQTT's client with automatic reconnect facilities.
**Notez-bien :** if `SERIAL_ENABLED` is defined, some debug messages are output to the serial **tx**.

 - `SafeMQTTClient(`
		- `Client &client` - Network client
		- `const char *ssid, const char *pwd` - WiFi's SSID and password
		- `const char *bURL, int bport` - MQTT broker URL and port
		- `const char *clientID` - MQTT clientID
		- `const char *logtopic = NULL` - Root of the logging topics (if not set, no logging is done)
		- `bool clear_session = false` - Among other things, with clean session set, it's not possible to receive asynchonously messages.
	`)` - Constructor

 - `bool connectWiFi( Duration &duration, uint32_t maxtries=SMC_WIFI_MAX_RETRY )` - Connect to the WiFi network (*implies connection to MQTT as well*).
 - `bool connectMQTT( Duration &duration )` - Connect to the MQTT broker (*normaly, there is no need to call it directly*)

 - `bool connected( void )` - Are we connected to the broker ?

 - `void setMQTTMaxTries( uint32_t val )` - For how long are we trying to connect to the broker (seconds)
 - `uint32_t getMQTTMaxTries( void )`

 - `void publish(`
	- `String &topic,`	- Topic to publish to
	- `const char *msg` or `const uint32_t val` - what to publish
	- `bool reconnect=true` - reconnect if the network is lost
	`)`

 - `void logMsg( const char *msg, bool reconnect=true )` - publish a message

### Published topics
	- ***topic*/Message** - Log messages
	- ***topic*/WiFi** - WiFi connection duration
	- ***topic*/MQTT** - MQTT connection duration

