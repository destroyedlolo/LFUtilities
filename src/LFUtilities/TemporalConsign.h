/* TemporalConsign.h
 * 
 * Manage simple consign
 *
 * 30/06/2018 First version from SondePiscine probe code
 */
#ifndef TEMPORAL_CONSIGN_H
#define TEMPORAL_CONSIGN_H 0.0100

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

	void setNext( unsigned long val ){	this->next = val; }
	unsigned long getNext( void ){ return this->next; }
};

#endif
