#include "markerplus.inc"

main()
{
	return 0;
}

static myMarker;

public OnGameModeInit() {

	myMarker = CreateMarker(ICON_FACTORY, true, 0.0, 0.0, 0.0);
	return 1;
}

public OnGameModeExit() {

	DestroyMarker(myMarker);
	return 1;
}

public OnPlayerEnterMarker(playerid, markerid) {

	return 1;
}

public OnPlayerLeaveMarker(playerid, markerid) {

	return 1;
}