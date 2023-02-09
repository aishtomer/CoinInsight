/*"main.cpp" is the main entry point for the program. It includes necessary headers and creates 
an instance of the AdvisorMain class. It then calls the init function of the AdvisorMain class.*/

// for AdvisorMain class
#include "AdvisorMain.h"

int main() {
    // create an instance of AdvisorMain class
    AdvisorMain app{};

    // call the init function of AdvisorMain
    app.init();

    return 0;
}
