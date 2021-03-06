/* Class to manage duration
 *
 * 25/10/2017 First version
 * 13/04/2018 Francisation
 * 30/06/2018 Back in english (as put in a library)
 */
#ifndef DURATION_H
#define DURATION_H	0.0201

class Duration {
	uint32_t start, end;

	public:
		Duration( void ) { reInit(); }
		void reInit( void ) { start = millis(); end = 0; }
		uint32_t Finished( void ){ end = millis(); return (end - start); }
		uint32_t operator *( void ){ return( (end ? end : millis()) - start ); }
};

#endif
