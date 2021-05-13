#pragma once

enum ComponentType      //enum is giving numbers some names
{
  RESISTOR,       //0   R
  INDUCTOR,       //1   L
  CAPACITOR,      //2   C
  VOLTAGE_SOURCE, //3   V
  CURRENT_SOURCE, //4   I
  DIODE,          //5   D
  BJT,            //6   Q
  MOSFET,         //7   M
  VCCS,            //8   G
  UNKNOWN
};
