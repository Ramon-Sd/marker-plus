[![samp-include](https://img.shields.io/badge/sa--mp-library-2f2f2f.svg?style=for-the-badge)](https://github.com/Ramon-Sd/marker-plus)

<img src="readme-images/example.png" alt="exemplo imagem">

> Enables the creation of custom markers for SA-MP.

<br>

1. [Installation](#-Installation)

2. [Documentation](#-Documentation)

<br>

## 🚀 Installation

You can install the "Marker Plus" library in two ways:

- Using sampctl: You can install the library using the sampctl package manager. Simply run the following command in the terminal:
```bash
sampctl package install Ramon-Sd/marker-plus
```

- Using Git: You can clone the library's Git repository and manually add it to your project. Run the following command in the terminal:
```bash
git clone https://github.com/Ramon-Sd/marker-plus.git
```

Then, include the "markerplus.inc" include file in your code:

```pawn
#include "markerplus.inc"
```

## ☕ Documentation

### **Functions**


- **CreateMarker**: Creates a custom marker. This function returns a unique marker ID.
 - Parameters:
   - `icon` (icon): The marker icon. It can be one of the icons defined in the `enum`.
   - `calledActive` (active): Indicates whether the marker is active or not.
   - `x`, `y`, `z` (coordinates): The coordinates of the marker in the SA-MP world.
   - `worldID` (optional): The ID of the world where the marker is located. The default is -1 (main world).
   - `interiorID` (optional): The ID of the interior where the marker is located. The default is -1 (no interior).
   - `drawDistance` (optional): The marker's draw distance. The default is 0.0 (always draw).
 - Return:
   - Returns the ID of the created marker, or a warning message if the marker limit is reached.

- **DestroyMarker**: Destroys an existing marker in SA-MP.
 - Parameters:
   - `markerid` (marker ID): The ID of the marker to be destroyed.

### **Callbacks**


- **OnPlayerEnterMarker**: Called when a player enters the area of a marker.
 - Parameters:
   - `playerid` (player ID): The ID of the player who entered the marker area.
   - `markerid` (marker ID): The ID of the marker whose area was accessed.

- **OnPlayerLeaveMarker**: Called when a player leaves the area of a marker.
 - Parameters:
   - `playerid` (player ID): The ID of the player who left the marker area.
   - `markerid` (marker ID): The ID of the marker whose area was left.

### **Example**


```pawn
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
```

<br><br>

This project is licensed. See the [LICENSE](LICENSE.md) file for more details.



