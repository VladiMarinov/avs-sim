#include "component.h"
#include "type.h"
#include "node.h"

Component::Component()
{

}

Component::Component(ComponentType type_, std::string designator_, std::string n1_, std::string n2_, double val_)
{
  type = type_;
  designator = designator_;
  nodes.push_back(n1_);
  nodes.push_back(n2_);
  value_type = CONSTANT_VAL;
  const_value = std::make_shared<Const_value>(val_);

}

Component::Component(ComponentType type_, std::string designator_, std::string n1_, std::string n2_, std::string n3_, std::string n4_, double val_)
{
  type = type_;
  designator = designator_;
  nodes.push_back(n1_);
  nodes.push_back(n2_);
  nodes.push_back(n3_);
  nodes.push_back(n4_);
  value_type = CONSTANT_VAL;
  const_value = std::make_shared<Const_value>(val_);

}

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
