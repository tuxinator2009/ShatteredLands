#pragma once

#include <Arduboy2.h>

class Arduboy2Ext : public Arduboy2Base {

  public:

    Arduboy2Ext();

    uint8_t justPressedButtons() const;
    uint8_t pressedButtons() const;
    void clearButtonState();

};