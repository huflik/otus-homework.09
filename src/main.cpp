#include <iostream>
#include <thread>
#include "async.h"

void demonstrateAsyncLibrary() {
    
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
    

    async::receive(h1, "cmd1", 4);      
    async::receive(h2, "cmdA", 4);      
    async::receive(h3, "single", 6);   
    
    async::receive(h1, "\ncmd2\ncmd3\n", 11);     
    async::receive(h4, "x\ny\nz\n", 6);          
    
    async::receive(h2, "start\n{\ninner1\ninner2\n{\ndeep1\ndeep2\n}\ninner3\n}\nend\n", 50);
    
    async::receive(h3, "A\nB\nC\n", 6);            
    async::receive(h5, "long1\nlong2\n", 12);      
    async::receive(h3, "D\nE\n", 4);               
    async::receive(h1, "extra1\nextra2\n", 14);   
    

    async::receive(h4, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n", 30);

    async::receive(h2, "\n\n\ncmdX\n\ncmdY\n\n", 18);
    async::receive(h5, "!@#$%\n^&*()\n", 14);
    
    std::string longCmd = "very_long_command_with_many_characters_";
    longCmd += std::string(50, 'x');
    longCmd += "\n";
    async::receive(h3, longCmd.c_str(), longCmd.size());
    
    async::receive(h4, "outside\n{\ninside1\ninside2\n", 27);
    
    async::receive(h1, "{\nblock1_cmd1\nblock1_cmd2\n}\n{\nblock2_cmd1\n}\n", 45);
    
    for (int i = 0; i < 5; ++i) {
        async::receive(h5, "m", 1);
        async::receive(h5, "i", 1);
        async::receive(h5, "c", 1);
        async::receive(h5, "r", 1);
        async::receive(h5, "o", 1);
        async::receive(h5, "\n", 1);
    }
    
    async::receive(h2, "normal1\nnormal2\n}\nnormal3\n", 27);
    
    async::receive(h5, "pre1\npre2\npre3\npre4\n{\ndyn1\ndyn2\n}\npost1\npost2\n", 52);
    
    async::disconnect(h3);   
    async::disconnect(h1);
    async::disconnect(h5);
    async::disconnect(h2);
    async::disconnect(h4); 
    
    async::stop();
}

int main(int, char *[]) {
    demonstrateAsyncLibrary();  
    return 0;
}