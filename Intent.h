#ifndef _INTENT_H_
#define _INTENT_H_

#include <SDL.h>

enum class INTENT {
	MESSAGE		= 0x8001,
	EMESSAGE	= 0x8002,
	MOVE		= 0x8003,
	MOVECAMERA	= 0x8004,
	STOPCAMERA	= 0x8005,
	HOVER		= 0x8006,
	POINTER		= 0x8007,
	ACTION		= 0x8008,
	TURN		= 0x8009,
};

enum class MESSAGE {
	CHANGED,
};

enum class EMESSAGE {
	CREATE,
	DESTROY,
};

enum class POINTER {
	NONE,
	SET,
	MOVE,
	STOP,
	UPDATE,
	ACTION1,
	ACTION2,
	ACTION3,
};

enum class ACTION {
	NONE,
	FIRST,
	SECOND,
	THIRTH,
};

enum class CAMERACODE {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

enum class HOVER {
	OFF,
	ON,
};

enum class TURN {
	START,
	END,
};


#endif //_INTENT_H_