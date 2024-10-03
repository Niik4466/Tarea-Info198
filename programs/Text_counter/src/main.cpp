#include "txtCounterinterface.h"

int main(){

    setEnv();
    Interface& interface = Interface::getInstance();
    interface.interfaceInputOutput();

    return EXIT_SUCCESS;
}
