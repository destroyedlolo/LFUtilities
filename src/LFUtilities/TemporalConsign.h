/* TemporalConsign.h
 * 
 * Manage simple consign
 *
 * 30/06/2018 First version from SondePiscine probe code
 * 22/03/2022 Correct time remaining :(
 */
#ifndef TEMPORAL_CONSIGN_H
#define TEMPORAL_CONSIGN_H 0.0300

#include <KeepInRTC.h>

class TemporalConsign : public KeepInRTC::KeepMe {
	uint32_t consign;	// Consign to keep
	uint32_t next;		// Next run

public:
	TemporalConsign( KeepInRTC &kir ) : KeepInRTC::KeepMe( kir, (uint32_t *)&this->consign, sizeof(this->consign) ), next(0) {}

	void setConsign( unsigned long val ){	this->consign = val; this->save(); }
	unsigned long getConsign( void ){ return this->consign; }

	/* Reset consign if kir is unset
	 * -> val : Default value
	 * <- true if kir was invalid
	 */
	bool begin( unsigned long val ){
		if( !kir.isValid() ){
			this->setConsign( val );
			return true;
		}
		return false;
	}

	/* set next deadline
	 * -> no arg : current next is increased by the consign
	 * -> value : set to an absolute value
	 */
	void setNext( void ){ this->next += this->consign; }
	void setNext( unsigned long val ){	this->next = val; }

	/* set next deadline to provided value + consign
	 * typically, value is related to millis()
	 */
	void restart( unsigned long val ){
		this->next = val + this->consign;
	}

	unsigned long getNext( void ){ return this->next; }

	bool isExhausted( unsigned long now ){
		return( this->next < now );
	}
	unsigned long remain( unsigned long now ){
		if( this->next < now )
			return this->next - now;
		else
			return 0;
	}
};

#endif
