#include "stdafx.h"

using namespace System;
using namespace System::IO::Ports;

int main(array<System::String ^> ^arges){
    SerialPort port("COM3", 9600);
    port.Open();
    while (true){
        int input = Convert::ToInt32(Console::ReadLine());
        if(input == 1)
            port.Write("A");
        else
            port.Write("b");
    }
}
