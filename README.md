# net4cpp
Network library based on C++11 standard

##Echo Server Example

```cpp
try
{
    acceptor ator{"::1", "2112"}; // IPv6 localhost
    iostream ios{ator.accept()};

    while(ios)
    {
        string echo;
        getline(ios, echo);
        ios << echo << endl;
        clog << echo << endl;
    }
}
catch(const exception& e)
{
    cerr << "Exception: " << e.what() << endl;
}
```

##Http Request Example

```cpp
try
{
    iostream ios{connect("www.google.com","http")};

    ios << "GET / HTTP/1.1\r\n"
        << "Host: www.google.com\r\n"
        << "Connection: close\r\n"
        << "Accept: text/plain, text/html\r\n"
        << "Accept-Charset: utf-8\r\n"
        << "\r\n"
        << flush;

    while(ios)
    {
        char c;
        ios >> noskipws >> c;
        clog << c;
    }
    clog << flush;
}
catch(const exception& e)
{
    cerr << "Exception: " << e.what() << endl;
}
```