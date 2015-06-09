#include <streambuf>
#include "net/socket.hpp"
#include "net/network.hpp"

namespace net
{

const std::size_t tcp_buffer_size{4096};
    
const std::size_t udp_buffer_size{512};

template<std::size_t N>
class stream_buffer : public std::streambuf
{
public:

    explicit stream_buffer(socket&& s) : m_socket{std::move(s)}, m_input_sequence{}, m_output_sequence{}
    {
        setg(&m_input_sequence[0], &m_input_sequence[sizeof m_input_sequence], &m_input_sequence[sizeof m_input_sequence]);
        setp(&m_output_sequence[0], &m_output_sequence[sizeof m_output_sequence]);
    }

protected:

    int_type underflow()
    {
        const auto bytes_read = net::read(m_socket, &m_input_sequence[0], sizeof m_input_sequence);
        //const auto bytes_read = ::recvfrom(m_socket, &m_input_sequence[0], sizeof m_input_sequence, 0, nullptr, 0);
        if(bytes_read < 1)
            return traits_type::eof();
        setg(&m_input_sequence[0], &m_input_sequence[0], &m_input_sequence[bytes_read]);
        return m_input_sequence[0];
    }

    int_type overflow(int_type c = traits_type::eof())
    {
        const auto error = sync();
        if(error < 0)
            return traits_type::eof();
        if(traits_type::eq_int_type(c, traits_type::eof()))
            return traits_type::not_eof(c);
        *pptr() = traits_type::to_char_type(c);
        pbump(1);
        return traits_type::not_eof(c);
    }

    int sync()
    {
        const char_type* buf = &m_output_sequence[0];
        while(buf < pptr())
        {
            const auto bytes_written = net::write(m_socket, buf, pptr() - buf);
            //const auto bytes_written = ::sendto(m_socket, buf, pptr() - buf, 0, nullptr, 0);
            if(bytes_written < 1)
                return -1;
            buf += bytes_written;
        }
        setp(&m_output_sequence[0], &m_output_sequence[sizeof m_output_sequence]);
        return 0;
    }

private:

    socket m_socket;

    char_type m_input_sequence[N];

    char_type m_output_sequence[N];
};

} // namespace net