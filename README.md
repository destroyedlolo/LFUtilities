# LFUtilities

This library contains various stuffs/helpers to Arduino like developments.

-------

## Duree

    #include <LFUtilities/Duree.h>
Abstracts duration measurement.

 - `Duree( void )` or `void reInit( void )` - Starts a duration measurement.
 - `unsigned long int Fini( void )` - End of measurement, return the duration in milliseconds.
 - `unsigned long int operator &ast;( void )` - return the duration in milliseconds, up to the `Fini()` call or till now.

-------

## SafeMQTTClient

    #include <LFUtilities/SafeMQTTClient.h>
