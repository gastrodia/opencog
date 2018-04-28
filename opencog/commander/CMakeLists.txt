


ADD_LIBRARY (commander SHARED

	WebSocketIO/Server.h
	WebSocketIO/Server.cpp
	WebSocketIO/Client.h
	WebSocketIO/Client.cpp
	WebSocketIO/Socket.h
	WebSocketIO/Socket.cpp
	WebSocketIO/EventEmitter.h
	WebSocketIO/mongoose/mongoose.h
	WebSocketIO/mongoose/mongoose.c
	WebSocketIO/json/json.hpp	

	sole/sole.hpp
	sole/sole.cpp

	CommanderServer.cc
	RpcSyncExecutor.cc
)



TARGET_LINK_LIBRARIES (commander
	${ATOMSPACE_LIBRARIES}
)

INSTALL (TARGETS commander DESTINATION "lib${LIB_DIR_SUFFIX}/opencog")

INSTALL (FILES CommanderServer.h
	DESTINATION "include/opencog/commander/"
)

ADD_EXECUTABLE (commander-server 

	WebSocketIO/Server.h
	WebSocketIO/Server.cpp
	WebSocketIO/Client.h
	WebSocketIO/Client.cpp
	WebSocketIO/Socket.h
	WebSocketIO/Socket.cpp
	WebSocketIO/EventEmitter.h
	WebSocketIO/mongoose/mongoose.h
	WebSocketIO/mongoose/mongoose.c
	WebSocketIO/json/json.hpp	

	sole/sole.hpp
	sole/sole.cpp

	CommanderServer
	RpcSyncExecutor
	CommanderServerMain
)

TARGET_LINK_LIBRARIES (commander-server
	${ATOMSPACE_LIBRARIES}
	${GUILE_LIBRARIES}
	${COGUTIL_LIBRARY}
)


INSTALL (TARGETS commander-server RUNTIME DESTINATION "bin")




