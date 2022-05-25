#include <vector>
#include <iostream>

class MS;

typedef struct 	s_fd {
    void(MS::*fct_read) (int);
    void(MS::*fct_write) (int);
} 	t_fd;

class MS
{
    private:
        std::vector< t_fd >            _fdSet;

        void        server_accept(int s)
        {
            int cs;

            if (cs % 2 == 0)
                _fdSet[cs].fct_read = MS::client_read;
            else
                _fdSet[cs].fct_write = MS::client_write;
        }

        void        client_read(int fd)
        {
            std::cout << "I'm reading\n";
        }

        void        client_write(int fd)
        {
            std::cout << "I'm writing\n";
        }

        void        check_fd()
        {
            int i = 0;
            int size = 10;

            while (i < size)
            {
                if (i < 5)
                    (this->*(_fdSet[i].fct_read))(i); // ! correct syntax
                if (i >= 5)
                    (this->*(_fdSet[i].fct_write))(i);
                i++;
            }
        }
};

