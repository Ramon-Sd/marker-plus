#if !defined _samp_included
    #include "a_samp.inc"
#endif

#if !defined _streamer_included
    #include "streamer.inc"
#endif

#if defined _INC_markerplus
	#endinput
#endif
#define _INC_markerplus


#if !defined MAX_MARKERS
    #define MAX_MARKERS (401)
#endif

#define INVALID_MARKER_ID (-1)


enum {
    ICON_INVALID,
    ICON_FACTORY,
    ICON_CONSTRUCTION,
    ICON_BANK,
    ICON_HOUSE,
    ICON_PRAIA,
    ICON_ROAD,
    ICON_CITY,
    ICON_HOUSE_2,
    ICON_AMBULANCE,
    ICON_FIRE_TRUCK,
    ICON_POLICE_CAR,
    ICON_SHIP,
    ICON_BUS
}

static const ICON_LETTER[][5] = {
    "NULL", "F", "A", "G", "H", "J", "K", "C", "B", "h", "f", "p", "o", "v"
};

static const ICON_FONT[][15] = {
    "NULL", "Webdings", "Webdings", "Webdings", "Webdings", "Webdings",
    "Webdings", "Webdings", "Webdings", "Webdings", "Webdings", "Webdings",
    "Webdings", "Webdings"
};


enum MarkerData {

    STREAMER_TAG_AREA:markerCalledArea,
    markerIcon,
    bool:markerCalledActive,
    Float:markerPosition[3],
    markerWorldID,
    markerInteriorID,
    Float:markerDrawDistance
}

enum PlayerMarkerData {

    markerObject[MAX_MARKERS],
    markerObjectIcon[MAX_MARKERS],
    bool:markerVisible[MAX_MARKERS]
}

static markerData[MAX_MARKERS][MarkerData];
static playerMarkerData[MAX_PLAYERS][PlayerMarkerData];


static IsValidMarker(markerid) {
    return !(

        markerData[markerid][markerPosition][0] == 0.0 &&
        markerData[markerid][markerPosition][1] == 0.0 &&
        markerData[markerid][markerPosition][2] == 0.0
    );
}

static GetFreeMarkerID() {

    for (new i = 0; i < MAX_MARKERS; i++) {

        if (!IsValidMarker(i))
            return i;
    }
    return INVALID_MARKER_ID;
}

static GetIconLetter(icon) {

    return ICON_LETTER[icon];
}

static GetIconFont(icon) {

    return ICON_FONT[icon];
}

static Player_CreateMarkers(playerID) {

    for (new i = 0; i < MAX_MARKERS; i++) {

        if (!IsValidMarker(i))
            continue;

        playerMarkerData[playerID][markerObject][i] = CreatePlayerObject(
            playerID,
            19197,
            markerData[i][markerPosition][0], markerData[i][markerPosition][1], markerData[i][markerPosition][2],
            0.0, 0.0, 0.0,
            markerData[i][markerDrawDistance]
        );

        SetPlayerObjectMaterial(
            playerID,
            playerMarkerData[playerID][markerObject][i],
            0, 18646,
            "matcolours", "lightblue", 0x00000000
        );

        playerMarkerData[playerID][markerObjectIcon][i] = CreatePlayerObject(
            playerID,
            11737,
            markerData[i][markerPosition][0], markerData[i][markerPosition][1], (markerData[i][markerPosition][2] + 1.5),
            90.0, 0.0, 0.0,
            markerData[i][markerDrawDistance]
        );

        SetPlayerObjectMaterialText(
            playerID,
            playerMarkerData[playerID][markerObjectIcon][i],
            GetIconLetter(markerData[i][markerIcon]), 0, OBJECT_MATERIAL_SIZE_256x256,
            GetIconFont(markerData[i][markerIcon]), 150, 1, 0xFFF8F8FF, 0x00000000, 1
        );
        playerMarkerData[playerID][markerVisible][i] = true;
    }
}

static Player_DestroyMarkers(playerID) {

    for (new  i = 0; i < MAX_MARKERS; i++) {

        if (playerMarkerData[playerID][markerObjectIcon][i] >= 0) {
            DestroyPlayerObject(playerID, playerMarkerData[playerID][markerObjectIcon][i]);
            playerMarkerData[playerID][markerObjectIcon][i] = INVALID_OBJECT_ID;
            DestroyPlayerObject(playerID, playerMarkerData[playerID][markerObject][i]);
            playerMarkerData[playerID][markerObject][i] = INVALID_OBJECT_ID;
            playerMarkerData[playerID][markerVisible][i] = false;
        }
    }
}

static Player_SetIconFace(playerID) {

    for (new i = 0; i < MAX_MARKERS; i++) {

        if (!IsValidMarker(i))
            continue;
        
        if (!playerMarkerData[playerID][markerVisible][i])
            continue;

        new Float:playerPosition[3];
        GetPlayerPos(playerID, playerPosition[0], playerPosition[1], playerPosition[2]);

        new Float:objectPosition[3];
        GetPlayerObjectPos(
            playerID,
            playerMarkerData[playerID][markerObjectIcon][i],
            objectPosition[0],
            objectPosition[1],
            objectPosition[2]
        );

        playerPosition[2] = atan2(playerPosition[1] - objectPosition[1], playerPosition[0] - objectPosition[0]) - 90.0;
        
        SetPlayerObjectRot(
            playerID,
            playerMarkerData[playerID][markerObjectIcon][i],
            90.0, 0.0, playerPosition[2] + 180
        );
    }
}


forward OnPlayerEnterMarker(playerid, markerid);
forward OnPlayerLeaveMarker(playerid, markerid);


stock CreateMarker(icon, calledActive, Float:x, Float:y, Float:z, worldID = -1, interiorID = -1, Float:drawDistance = 0.0) {

    new markerid = GetFreeMarkerID();
    if (markerid == INVALID_MARKER_ID) {
        return printf("(!) Marker.INC -> Warning : Reached limit. (MAX_MARKERS: %d)", MAX_MARKERS);
    }

    markerData[markerid][markerCalledArea] = CreateDynamicCylinder(x, y, (z - 0.1), (z + 10.0), 1.0, worldID, interiorID, -1);

    markerData[markerid][markerIcon] = icon;
    markerData[markerid][markerCalledArea] = calledActive;
    markerData[markerid][markerPosition][0] = x;
    markerData[markerid][markerPosition][1] = y;
    markerData[markerid][markerPosition][2] = z;
    markerData[markerid][markerWorldID] = worldID;
    markerData[markerid][markerInteriorID] = interiorID;
    markerData[markerid][markerDrawDistance] = drawDistance;

    return markerid;
}

stock DestroyMarker(markerid) {

    new markerDataVoid[MarkerData];
    markerData[markerid] = markerDataVoid;

    for (new i = GetPlayerPoolSize(); i >= 0; i--) {

        if (!IsPlayerConnected(i))
            continue;

        if (playerMarkerData[i][markerObjectIcon][markerid] >= 0) {
            DestroyPlayerObject(i, playerMarkerData[i][markerObjectIcon][markerid]);
            playerMarkerData[i][markerObjectIcon][markerid] = INVALID_OBJECT_ID;
        }
    }
}


public OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid) {
    
    for (new i = 0; i < MAX_MARKERS; i++) {

        if (!IsValidMarker(i))
            continue;

        if (!markerData[i][markerCalledArea])
            continue;

        if (areaid == markerData[i][markerCalledArea]) {

            CallRemoteFunction("OnPlayerEnterMarker", "ii", playerid, i);
        }
    }

    #if defined MPHook_OnPlayerEnterDynamicArea
        return MPHook_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerEnterDynamicArea
    #undef OnPlayerEnterDynamicArea
#else
    #define _ALS_OnPlayerEnterDynamicArea
#endif

#define OnPlayerEnterDynamicArea MPHook_OnPlayerEnterDynamicArea
#if defined MPHook_OnPlayerEnterDynamicArea
    forward MPHook_OnPlayerEnterDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
#endif

public OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA:areaid) {
    
    for (new i = 0; i < MAX_MARKERS; i++) {

        if (!IsValidMarker(i))
            continue;

        if (!markerData[i][markerCalledActive])
            continue;

        if (areaid == markerData[i][markerCalledArea]) {

            CallRemoteFunction("OnPlayerLeaveMarker", "ii", playerid, i);
        }
    }

    #if defined MPHook_OnPlayerLeaveDynamicArea
        return MPHook_OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerLeaveDynamicArea
    #undef OnPlayerLeaveDynamicArea
#else
    #define _ALS_OnPlayerLeaveDynamicArea
#endif

#define OnPlayerLeaveDynamicArea MPHook_OnPlayerLeaveDynamicArea
#if defined MPHook_OnPlayerLeaveDynamicArea
    forward MPHook_OnPlayerLeaveDynamicArea(playerid, STREAMER_TAG_AREA:areaid);
#endif

public OnPlayerUpdate(playerid) {
    
    if (!IsPlayerConnected(playerid))
        return 0;

    for (new i = 0; i < MAX_MARKERS; i++) {

        if (!IsValidMarker(i))
            continue;
        
        new Float:playerPosition[3];
        GetPlayerPos(playerid, playerPosition[0], playerPosition[1], playerPosition[2]);

        if (
            playerPosition[0] > markerData[i][markerPosition][0] - 5.0 && playerPosition[1] > markerData[i][markerPosition][1] - 5.0 &&
            playerPosition[0] < markerData[i][markerPosition][0] + 5.0 && playerPosition[1] < markerData[i][markerPosition][1] + 5.0
        ) {

            if (playerMarkerData[playerid][markerVisible][i]) {
                SetPlayerObjectMaterialText(
                    playerid,
                    playerMarkerData[playerid][markerObjectIcon][i],
                    GetIconLetter(markerData[i][markerIcon]), 0, OBJECT_MATERIAL_SIZE_256x256,
                    GetIconFont(markerData[i][markerIcon]), 150, 1, 0x00000000, 0x00000000, 1
                );

                SetPlayerObjectPos(
                    playerid,
                    playerMarkerData[playerid][markerObjectIcon][i],
                    markerData[i][markerPosition][0],
                    markerData[i][markerPosition][1],
                    ((markerData[i][markerPosition][2] + 1.5) * 2)
                );

                playerMarkerData[playerid][markerVisible][i] = false;
            }
        }
        else {

            if (!playerMarkerData[playerid][markerVisible][i]) {

                SetPlayerObjectMaterialText(
                    playerid,
                    playerMarkerData[playerid][markerObjectIcon][i],
                    GetIconLetter(markerData[i][markerIcon]), 0, OBJECT_MATERIAL_SIZE_256x256,
                    GetIconFont(markerData[i][markerIcon]), 150, 1, 0xFFF8F8FF, 0x00000000, 1
                );

                SetPlayerObjectPos(
                    playerid,
                    playerMarkerData[playerid][markerObjectIcon][i],
                    markerData[i][markerPosition][0],
                    markerData[i][markerPosition][1],
                    (markerData[i][markerPosition][2] + 1.5)
                );

                playerMarkerData[playerid][markerVisible][i] = true;
            }
        }
        Player_SetIconFace(playerid);
    }

    #if defined MPHook_OnPlayerUpdate
        return MPHook_OnPlayerUpdate(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate MPHook_OnPlayerUpdate
#if defined MPHook_OnPlayerUpdate
    forward MPHook_OnPlayerUpdate(playerid);
#endif

public OnPlayerConnect(playerid) {
    
    Player_CreateMarkers(playerid);

    #if defined MPHook_OnPlayerConnect
        return MPHook_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect MPHook_OnPlayerConnect
#if defined MPHook_OnPlayerConnect
    forward MPHook_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason) {

    Player_DestroyMarkers(playerid);

    #if defined MPHook_OnPlayerDisconnect
        return MPHook_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect MPHook_OnPlayerDisconnect
#if defined MPHook_OnPlayerDisconnect
    forward MPHook_OnPlayerDisconnect(playerid, reason);
#endif
