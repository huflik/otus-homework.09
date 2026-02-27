#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include "async.h"


void SendCommand(async::handle_t handle, const std::string& cmd, bool addNewline = true) {
    std::string data = cmd;
    if (addNewline) {
        data += '\n';
    }
    async::receive(handle, data.c_str(), data.size());
}

void AsyncLibrary() {

    std::size_t bulk1 = 3;  
    std::size_t bulk2 = 5;  
    std::size_t bulk3 = 2;  
    std::size_t bulk4 = 1;  
    std::size_t bulk5 = 10; 
    
    auto h1 = async::connect(bulk1);
    auto h2 = async::connect(bulk2);
    auto h3 = async::connect(bulk3);
    auto h4 = async::connect(bulk4);
    auto h5 = async::connect(bulk5);
    
    SendCommand(h1, "cmd1", false);  
    SendCommand(h2, "cmdA", false);  
    SendCommand(h3, "single", false); 
    
    SendCommand(h1, "", true);  
    SendCommand(h2, "", true);  
    SendCommand(h3, "", true);  

    SendCommand(h1, "cmd2");
    SendCommand(h1, "cmd3");
    SendCommand(h4, "x");
    SendCommand(h4, "y");
    SendCommand(h4, "z");

    SendCommand(h2, "start");
    SendCommand(h2, "{");
    SendCommand(h2, "inner1");
    SendCommand(h2, "inner2");
    SendCommand(h2, "{");
    SendCommand(h2, "deep1");
    SendCommand(h2, "deep2");
    SendCommand(h2, "}");  
    SendCommand(h2, "inner3");
    SendCommand(h2, "}");  
    SendCommand(h2, "end");

    SendCommand(h3, "A");
    SendCommand(h3, "B");
    SendCommand(h3, "C");
    SendCommand(h5, "long1");
    SendCommand(h5, "long2");
    SendCommand(h3, "D");
    SendCommand(h3, "E");
    SendCommand(h1, "extra1");
    SendCommand(h1, "extra2");

    for (int i = 1; i <= 10; ++i) {
        SendCommand(h4, std::to_string(i));
    }

    SendCommand(h2, "", true);  
    SendCommand(h2, "", true);  
    SendCommand(h2, "", true);  
    SendCommand(h2, "cmdX");
    SendCommand(h2, "", true);  
    SendCommand(h2, "", true);  
    SendCommand(h2, "cmdY");
    SendCommand(h2, "", true);  
    SendCommand(h2, "", true);  

    SendCommand(h5, "!@#$%");
    SendCommand(h5, "^&*()");

    std::string longCmd = "very_long_command_with_many_characters_";
    longCmd += std::string(50, 'x');
    SendCommand(h3, longCmd);

    SendCommand(h4, "outside");
    SendCommand(h4, "{");  
    SendCommand(h4, "inside1", false);  
    SendCommand(h4, "inside2", false);  

    SendCommand(h1, "{");  
    SendCommand(h1, "block1_cmd1");
    SendCommand(h1, "block1_cmd2");
    SendCommand(h1, "}");  
    SendCommand(h1, "{");  
    SendCommand(h1, "block2_cmd1");
    SendCommand(h1, "}");  

    for (int i = 0; i < 5; ++i) {
        SendCommand(h5, "micro");
    }

    SendCommand(h2, "normal1");
    SendCommand(h2, "normal2");
    SendCommand(h2, "}"); 
    SendCommand(h2, "normal3");

    SendCommand(h5, "pre1");
    SendCommand(h5, "pre2");
    SendCommand(h5, "pre3");
    SendCommand(h5, "pre4");
    SendCommand(h5, "{");  
    SendCommand(h5, "dyn1");
    SendCommand(h5, "dyn2");
    SendCommand(h5, "}");  
    SendCommand(h5, "post1");
    SendCommand(h5, "post2");
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    async::disconnect(h3);
    async::disconnect(h1);
    async::disconnect(h5);
    async::disconnect(h2);
    async::disconnect(h4);
    
    async::stop();

}

int main() {
    try {
        AsyncLibrary();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}