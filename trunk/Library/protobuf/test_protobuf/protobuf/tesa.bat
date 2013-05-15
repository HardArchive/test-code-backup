:;protoc -I=D:\protobuf --cpp_out=D:\protobuf\1 D:\protobuf/addressbook.proto 
:;protoc.exe -proto_path=SRC --cpp_out=DST SRC/addressbook.proto 
protoc.exe --proto_path=D:\protobuf\ --cpp_out=D:\protobuf D:\protobuf\lm.helloworld.proto
pause 