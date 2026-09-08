#include <iostream>
#include "core/engine.h"

int main(int argc, char* argv[])
{
    try { 
        
        Engine engine;  
        engine.init();    
        engine.run();    
        std::cout << "Finalized" << std::endl;
        engine.cleanup();    

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}
