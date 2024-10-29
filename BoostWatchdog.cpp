#include <boost/process.hpp>
#include <iostream>
#include <string>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>



namespace bp = boost::process;


static void wait(int seconds) {
    boost::this_thread::sleep(boost::chrono::seconds(seconds));
}

static bool IsRunning(const std::string ProcessName) {
    bp::ipstream pipe_stream;
    bp::child c("tasklist", bp::std_out > pipe_stream);

    std::string line;
    while (pipe_stream && getline(pipe_stream, line) && !line.empty()) {
       // printf("%s\n", line);
        if (line.find(ProcessName) != std::string::npos) {
            return true;
        }
    }

    c.wait();
    return false;
}


int main(int argc, char* argv[]) {
    
    const std::string ProcessName = "chrome.exe";
    const std::string ProcessPath = "C:\\Program Files\\Google\\Chrome\\Application\\chrome.exe";

    std::string ticket = argv[1];

        bp::ipstream pipe;
        bp::child c("tasklist", bp::std_out > pipe);

        std::string info;
        std::string pid;
        while (getline(pipe, info)) {
            if (info.find(ProcessName) != std::string::npos) {
                std::istringstream iss(info);
                std::string token;
                iss >> token;
                iss >> pid;
                break;
            }
        }
        
        if (ticket.compare("create") == 0) {
            bp::child c1(ProcessPath);
            c1.detach();
            printf("Process created successfully!");
        }
        
        if(ticket.compare("stop")==0) {
            if (pid.empty()) {
                printf("Process not found!");
            }
            else {
                bp::child c2("taskkill /PID " + pid + " /F");
                c2.wait();
                printf("Process terminated successfully!");
            }
        }

        if(ticket.compare("check")==0) {
            if (!IsRunning(ProcessName)) {
                printf("Process is not running!");
            }
            else {
                printf("Process is running!");

            }
        }
            
        if(ticket.compare("monitoring")==0){
                for (;;) {
                    if (!IsRunning(ProcessName)) {
                        printf("Process is not running!\n");
                    }
                    else {
                        printf("Process is running!\n");
                    }
                    wait(1);
                    continue;
            }
        }

        if (ticket.compare("list") == 0) {
            printf("Type create to start the process:\n");
            printf("Type stop to stop the process:\n");
            printf("Type check to check if the process is running:\n");
            printf("Type monitoring to start the monitoring process:\n");
        }
        


    return 0;
}
