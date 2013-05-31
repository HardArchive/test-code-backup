include Makefile.define

# 默认超作
default:all

# 主应用文件列表
PATS = ../Reactor/AceReactorManager.o \
       ../Common/AppConfig.o \
       ../Common/BuffPacket.o \
       ../PacketParse/PacketParsePool.o \
       ../Common/MakePacket.o \
       ../Common/MessageBlockManager.o \
       ../Common/ClientParse.o \
       ../Common/ClientParsePool.o \
       ../Common/ForbiddenIP.o \
       ../Reactor/ConnectAccept.o \
       ../Reactor/ConnectHandler.o \
       ../Reactor/ConnectClient.o \
       ../Reactor/ClientReConnectManager.o \
       ../Reactor/ConsoleHandler.o \
       ../PurenessScopeServer.o \
       ../Message/LoadModule.o \
       ../LogSystem/LogManager.o \
       ../Common/MainConfig.o \
       ../Message/Message.o \
       ../Message/MessageManager.o \
       ../Message/MessageService.o \
       ../Reactor/ServerManager.o \
       ../Reactor/SigHandle.o \
       ../Common/ThreadInfo.o \
       ../LogSystem/FileLogger.o \
       ../Encrypt/3Des.o \
       ../Encrypt/Encrypt.o \
       ../Common/RandomNumber.o \
       ../Common/BuffPacketManager.o \
       ../Common/ACEMemory.o \
       ../Common/TimerManager.o \
       ../Console/ConsoleMessage.o \
       ../UDP/ReactorUDPHander.o \
       ../UDP/ReactorUDPClient.o \
       ../Common/LinuxCPU.o 

       
OBJS = AceReactorManager.o \
       AppConfig.o \
       BuffPacket.o \
       PacketParsePool.o \
       MakePacket.o \
       MessageBlockManager.o \
       ClientParse.o \
       ClientParsePool.o \
       ForbiddenIP.o \
       ConnectAccept.o \
       ConnectHandler.o \
       ConnectClient.o \
       ClientReConnectManager.o \
       ConsoleHandler.o \
       PurenessScopeServer.o \
       LoadModule.o \
       LogManager.o \
       MainConfig.o \
       Message.o \
       MessageManager.o \
       MessageService.o \
       ServerManager.o \
       SigHandle.o \
       ThreadInfo.o \
       FileLogger.o \
       3Des.o \
       Encrypt.o \
       RandomNumber.o \
       BuffPacketManager.o \
       ACEMemory.o \
       TimerManager.o \
       ConsoleMessage.o \
       ReactorUDPHander.o \
       ReactorUDPClient.o \
       LinuxCPU.o 
       
			
# 主应用程序
APP_NAME = PurenessScopeServer

all:$(APP_NAME) Makefile

$(APP_NAME):$(PATS) 
	$(CC) -rdynamic -o $(APP_NAME) $(OBJS) $(LIBS)

# 清理
clean:
	rm -rf *.o $(APP_NAME)
	
cl:		
	rm -rf *.o 



