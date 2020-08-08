# AudioServer
A simple audio server.
Monitor a tcp/ip port, receive a connection from a client;
Then receive PCM data, use ffmpeg, encode to AAC or MP3;
Write to a file or push as UDP or RTP stream, etc.

一个简单的音频服务器， 监听一个TCP/IP端口；
接收客户连接， 然后接收PCM数据，利用ffmpeg，编码为AAC或者MP3格式；
保存为文件或者推流UDP或者RTP等。