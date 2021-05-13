#include "component.h"

ComponentType Component::parse_type()
{
    switch(designator[0])
    {
        case 'R': return RESISTOR;
        case 'L': return INDUCTOR;
        case 'C': return CAPACITOR;
        case 'V': return VOLTAGE_SOURCE;
        case 'I': return CURRENT_SOURCE;
        case 'D': return DIODE;
        case 'Q': return BJT;
        case 'M': return MOSFET;
        case 'G': return VCCS;

        default : return UNKNOWN;
    }
}

void Component::findType()
{
    type = parse_type();
}